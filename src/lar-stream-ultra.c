/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * lar-device.c
 * Copyright (C) 2017 LAR 
 *
 * lar-device.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public Device as published by the
 * Free Software Foundation, either version 3 of the Device, or
 * (at your option) any later version.
 *
 * lar-device.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public Device for more details.
 *
 * You should have received a copy of the GNU General Public Device along
 * with this program.  If not, see <http://www.gnu.org/devices/>.
 */

#include "lar-device.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>
#include <string.h>

struct _UltraStreamPrivate {
    guint    volume;
    guint    injection_volume;
    guint    injection_volume_tic;
    gdouble  sample_filling_time;
    gdouble  delay;
    gdouble  delay_tic;
    gboolean pre_rinsing;
    guint    rinsing_count;
    gboolean after_rinsing;
    guint    after_rinsing_count;
    gboolean need_stripping;
    gboolean codo_injection;
    gdouble  stripping_time;
    guint    dilution_type;
    gdouble  dilution_factor;
    gdouble  dilution_pump_time;
    gdouble  dilution_wait_time;
    gdouble  amount_deviation;
    gdouble  autocal_deviation;
    gboolean prinsing_on;
    guint    prinsing_Y1;
    guint    prinsing_Y2;
    guint    inj_rep;
    guint    inj_vol;
    gdouble  wait_inj;
    gdouble  wait_after;
};

enum { PROP_0, PROP_ID, PROP_PRODUCT, PROP_MAC, PROP_SERIAL, PROP_DISCRIPTION, PROP_CREATED, PROP_UPDATED };

static JsonSerializableIface *serializable_iface = NULL;

static void json_serializable_iface_init(gpointer g_iface);

G_DEFINE_TYPE_WITH_CODE(UltraStream, ultra_stream, ULTRA_TYPE_STREAM, G_IMPLEMENT_INTERFACE(JSON_TYPE_SERIALIZABLE, json_serializable_iface_init));

static JsonNode *ultra_stream_serialize_property(JsonSerializable *serializable, const gchar *name, const GValue *value, GParamSpec *pspec) {
    JsonNode *retval = NULL;
    g_debug("Device serealize name %s", name);
    if (g_strcmp0(name, "created") == 0 || g_strcmp0(name, "updated") == 0) {
        GValue copy = {
            0,
        };
        retval = json_node_new(JSON_NODE_VALUE);
        g_value_init(&copy, G_TYPE_STRING);
        GTimeVal timeval = {g_value_get_int64(value) / 1000, 0};
        gchar *  tstr    = g_time_val_to_iso8601(&timeval);
        g_print("  %s:%s\n", name, tstr);
        g_value_set_string(&copy, tstr);
        json_node_set_value(retval, &copy);
        g_free(tstr);
    } else {
        retval = serializable_iface->serialize_property(serializable, name, value, pspec);
    }
    return retval;
}

gboolean ultra_stream_deserialize_property(
    JsonSerializable *serializable, const gchar *property_name, GValue *value, GParamSpec *pspec, JsonNode *property_node) {
    g_debug("deserialize device property %s", property_name);
    g_return_val_if_fail(JSON_IS_SERIALIZABLE(serializable), FALSE);
    g_return_val_if_fail(property_name != NULL, FALSE);
    g_return_val_if_fail(value != NULL, FALSE);
    g_return_val_if_fail(pspec != NULL, FALSE);
    g_return_val_if_fail(property_node != NULL, FALSE);

    if (g_strcmp0(property_name, "id") == 0) {
        if (JSON_NODE_HOLDS_VALUE(property_node)) {
            g_value_set_string(value, json_node_get_string(property_node));
        } else if (JSON_NODE_HOLDS_OBJECT(property_node)) {
            g_warning("id from value deserialize not defined");
        }
    }
    if (g_strcmp0(property_name, "product") == 0 || g_strcmp0(property_name, "serial") == 0 || g_strcmp0(property_name, "type") == 0) {
        g_debug("normal property");
        return serializable_iface->deserialize_property(serializable, property_name, value, pspec, property_node);
    } else if (g_strcmp0(property_name, "created") == 0 || g_strcmp0(property_name, "updated") == 0) {
        g_print("JSON Node type:");
        JsonNodeType ntype = json_node_get_node_type(property_node);
        if (ntype == JSON_NODE_ARRAY) {
            g_print("array.\n");
        } else if (ntype == JSON_NODE_OBJECT) {
            g_print("object.\n");
            JsonNode *val = json_object_get_member(json_node_get_object(property_node), "$date");
            if (val && json_node_get_node_type(val) == JSON_NODE_VALUE) {
                gint64 temp = json_node_get_int(val);
                g_debug("set value : %" G_GINT64_FORMAT, temp);
                g_value_set_int64(value, temp);
                return TRUE;
            }
        } else if (ntype == JSON_NODE_VALUE) {
            g_print("value.\n");
            GTimeVal time = {0, 0};
            if (g_time_val_from_iso8601(json_node_get_string(property_node), &time)) {
                g_value_set_int64(value, time.tv_sec * 1000);
                return TRUE;
            }
        }
        return FALSE;
    }
    return FALSE;
}

static void json_serializable_iface_init(gpointer g_iface) {
    JsonSerializableIface *iface = g_iface;

    serializable_iface          = g_type_default_interface_peek(JSON_TYPE_SERIALIZABLE);
    iface->serialize_property   = ultra_stream_serialize_property;
    iface->deserialize_property = ultra_stream_deserialize_property;
}

static void ultra_stream_init(UltraStream *ultra_stream) {
    ultra_stream->priv = G_TYPE_INSTANCE_GET_PRIVATE(ultra_stream, ULTRA_TYPE_STREAM, UltraStreamPrivate);

    /* TODO: Add initialization code here */
}

static void ultra_stream_finalize(GObject *object) {
    UltraStream *device = ULTRA_STREAM(object);
    if (device->priv->product) g_free(device->priv->product);
    if (device->priv->serial) g_free(device->priv->serial);
    if (device->priv->discription) g_free(device->priv->discription);
    if (device->priv->mac) g_free(device->priv->mac);

    G_OBJECT_CLASS(ultra_stream_parent_class)->finalize(object);
}

static void ultra_stream_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(ULTRA_IS_STREAM(object));
    UltraStream *device = ULTRA_STREAM(object);
    switch (prop_id) {
    case PROP_ID:
        if (device->priv->id) g_free(device->priv->id);
        device->priv->id = g_value_dup_string(value);
        break;
    case PROP_PRODUCT:
        if (device->priv->product) g_free(device->priv->product);
        device->priv->product = g_value_dup_string(value);
        break;
    case PROP_MAC:
        if (device->priv->mac) g_free(device->priv->mac);
        device->priv->mac = g_value_dup_string(value);
        break;
    case PROP_SERIAL:
        if (device->priv->serial) g_free(device->priv->serial);
        device->priv->serial = g_value_dup_string(value);
        break;
    case PROP_DISCRIPTION:
        if (device->priv->discription) g_free(device->priv->discription);
        device->priv->discription = g_value_dup_string(value);
        break;
    case PROP_CREATED:
        device->priv->created = g_value_get_int64(value);
        break;
    case PROP_UPDATED:
        device->priv->updated = g_value_get_int64(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void ultra_stream_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(ULTRA_IS_STREAM(object));
    UltraStream *device = ULTRA_STREAM(object);

    switch (prop_id) {
    case PROP_ID:
        g_value_set_string(value, device->priv->id);
        break;
    case PROP_PRODUCT:
        g_value_set_string(value, device->priv->product);
        break;
    case PROP_MAC:
        g_value_set_string(value, device->priv->mac);
        break;
    case PROP_SERIAL:
        g_value_set_string(value, device->priv->serial);
        break;
    case PROP_DISCRIPTION:
        g_value_set_string(value, device->priv->discription);
        break;
    case PROP_CREATED:
        g_value_set_int64(value, device->priv->created);
        break;
    case PROP_UPDATED:
        g_value_set_int64(value, device->priv->updated);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void ultra_stream_class_init(UltraStreamClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    g_type_class_add_private(klass, sizeof(UltraStreamPrivate));

    object_class->finalize     = ultra_stream_finalize;
    object_class->set_property = ultra_stream_set_property;
    object_class->get_property = ultra_stream_get_property;
    g_object_class_install_property(
        object_class, PROP_ID, g_param_spec_string("id", "id", "id", "none", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_PRODUCT, g_param_spec_string("product", "product", "product", "none", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_MAC, g_param_spec_string("mac", "mac", "mac", "none", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_SERIAL, g_param_spec_string("serial", "serial", "serial", "-", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_DISCRIPTION,
        g_param_spec_string("discription", "discription", "discription", "-", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_CREATED,
        g_param_spec_int64("created", "created", "created time in msec", 0, G_MAXINT64, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_UPDATED,
        g_param_spec_int64("updated", "updated", "updated time in msec", 0, G_MAXINT64, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
}

void ultra_stream_print_test(UltraStream *device) {
    g_debug("TEST Created %" G_GINT64_FORMAT, device->priv->created);
    g_print("Device:%s\n", device->priv->serial);
    GTimeVal timeval = {device->priv->created / 1000, 0};
    // g_time_val_add(&timeval, device->priv->created);
    gchar *tstr = g_time_val_to_iso8601(&timeval);
    g_print("  created:%s\n", tstr);
    g_free(tstr);
    timeval.tv_sec = device->priv->updated / 1000;
    tstr           = g_time_val_to_iso8601(&timeval);
    g_print("  updated:%s\n", tstr);
    g_free(tstr);

    g_print("  product:%s\n", device->priv->product);
    g_print("  mac:%s\n", device->priv->mac);
    g_print("  discription:%s\n", device->priv->discription);
}

const gchar *ultra_stream_get_id(UltraStream *device) {
    g_return_val_if_fail(device != NULL, NULL);
    g_return_val_if_fail(ULTRA_IS_STREAM(device), NULL);
    return device->priv->id;
}

const gchar *ultra_stream_get_product(UltraStream *device) {
    g_return_val_if_fail(device != NULL, NULL);
    g_return_val_if_fail(ULTRA_IS_STREAM(device), NULL);
    return device->priv->product;
}
const gchar *ultra_stream_get_mac(UltraStream *device) {
    g_return_val_if_fail(device != NULL, NULL);
    g_return_val_if_fail(ULTRA_IS_STREAM(device), NULL);
    return device->priv->mac;
}
const gchar *ultra_stream_get_discription(UltraStream *device) {
    g_return_val_if_fail(device != NULL, NULL);
    g_return_val_if_fail(ULTRA_IS_STREAM(device), NULL);
    return device->priv->discription;
}
const gchar *ultra_stream_get_serial(UltraStream *device) {
    g_return_val_if_fail(device != NULL, NULL);
    g_return_val_if_fail(ULTRA_IS_STREAM(device), NULL);
    return device->priv->serial;
}

static gboolean ultra_stream_check_json_object(JsonObject *object) {
    gboolean serial  = FALSE;
    gboolean mac     = FALSE;
    gboolean product = FALSE;
    GList *  members = json_object_get_members(object);
    GList *  l       = NULL;
    for (l = members; l != NULL; l = l->next) {
        if (JSON_NODE_HOLDS_VALUE((JsonNode *)l->data)) {
            g_debug("check node %s", json_node_get_string((JsonNode *)l->data));
            if (0 == g_strcmp0("product", json_node_get_string((JsonNode *)l->data))) {
                product = TRUE;
            } else if (0 == g_strcmp0("mac", json_node_get_string((JsonNode *)l->data))) {
                mac = TRUE;
            } else if (0 == g_strcmp0("serial", json_node_get_string((JsonNode *)l->data))) {
                serial = TRUE;
            }
        }
    }
    g_list_free(members);
    return serial && mac && product;
}

UltraStream *ultra_stream_new_from_json(const gchar *json_body) {
    GError *    error  = NULL;
    UltraStream * device = NULL;
    JsonParser *parser = json_parser_new();
    json_parser_load_from_data(parser, json_body, -1, &error);
    if (error) {
        g_warning("JSon Parser(%s) decode error %d - %s", g_type_name(ULTRA_TYPE_STREAM), error->code, error->message);
        g_error_free(error);

    } else {
        JsonNode *jnode = json_parser_get_root(parser);
        if (JSON_NODE_HOLDS_OBJECT(jnode) && ultra_stream_check_json_object(json_node_get_object(jnode))) {
            device = ULTRA_STREAM(json_gobject_deserialize(ULTRA_TYPE_STREAM, jnode));
            if (device == NULL) {
                g_warning("Deserialize device failed");
            }
        }
    }
    g_object_unref(parser);
    return device;
}
