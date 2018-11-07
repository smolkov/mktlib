/*
 * device.c
 * Copyright (C) 2017 LAR
 *
 */

#include "device.h"
#include "vessel.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>
#include <string.h>

struct _DevicePrivate {
    gchar *  id;
    gchar *  serial;
    gchar *  product;
    gchar *  discription;
    gint64   created;
    gint64   updated;
    gboolean autostart;
    guint    run_counter;
};

enum { PROP_0, PROP_ID, PROP_PRODUCT, PROP_MAC, PROP_SERIAL, PROP_DISCRIPTION, PROP_CREATED, PROP_UPDATED, PROP_AUTOSTART, PROP_RUN_COUNTER };

G_DEFINE_TYPE(Device, device, G_TYPE_OBJECT);

static void device_init(Device *device) {
    device->priv = G_TYPE_INSTANCE_GET_PRIVATE(device, TYPE_DEVICE, DevicePrivate);

    /* TODO: Add initialization code here */
}

static void device_finalize(GObject *object) {
    Device *device = DEVICE(object);
    if (device->priv->id) g_free(device->priv->id);
    if (device->priv->product) g_free(device->priv->product);
    if (device->priv->serial) g_free(device->priv->serial);
    if (device->priv->discription) g_free(device->priv->discription);

    G_OBJECT_CLASS(device_parent_class)->finalize(object);
}

static void device_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_DEVICE(object));
    Device *device = DEVICE(object);
    switch (prop_id) {
        case PROP_ID:
            if (device->priv->id) g_free(device->priv->id);
            device->priv->id = g_value_dup_string(value);
            break;
        case PROP_PRODUCT:
            if (device->priv->product) g_free(device->priv->product);
            device->priv->product = g_value_dup_string(value);
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
        case PROP_AUTOSTART:
            device->priv->autostart = g_value_get_boolean(value);
            break;
        case PROP_RUN_COUNTER:
            device->priv->run_counter = g_value_get_uint(value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}

static void device_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_DEVICE(object));
    Device *device = DEVICE(object);

    switch (prop_id) {
        case PROP_ID:
            g_value_set_string(value, device->priv->id);
            break;
        case PROP_PRODUCT:
            g_value_set_string(value, device->priv->product);
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
        case PROP_AUTOSTART:
            g_value_set_boolean(value, device->priv->autostart);
            break;
        case PROP_RUN_COUNTER:
            g_value_set_uint(value, device->priv->run_counter);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}

static void device_class_init(DeviceClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    g_type_class_add_private(klass, sizeof(DevicePrivate));

    object_class->finalize     = device_finalize;
    object_class->set_property = device_set_property;
    object_class->get_property = device_get_property;
    g_object_class_install_property(object_class, PROP_ID, g_param_spec_string("id", "id", "id", "none", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_PRODUCT, g_param_spec_string("product", "product", "product", "none", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_SERIAL, g_param_spec_string("serial", "serial number", "serial", "-", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_DISCRIPTION, g_param_spec_string("discription", "discription", "discription", "-", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_CREATED,
                                    g_param_spec_int64("created", "created", "created time in msec", 0, G_MAXINT64, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_UPDATED,
                                    g_param_spec_int64("updated", "updated", "updated time in msec", 0, G_MAXINT64, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_AUTOSTART, g_param_spec_boolean("autostart", "autostart", "autostart device", FALSE, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_RUN_COUNTER,
                                    g_param_spec_uint("counter", "run counter", "run counter", 0, G_MAXUINT32, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
}

void device_print_test(Device *device) {
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
    g_print("  discription:%s\n", device->priv->discription);
}

/**
 * device_get_id:
 * @device: a #Device
 *
 * Get device serial number.
 *
 * Returns: serial number as string
 */

const gchar *device_get_id(Device *device) {
    g_return_val_if_fail(device != NULL, NULL);
    g_return_val_if_fail(IS_DEVICE(device), NULL);
    return device->priv->id;
}

/**
 * device_get_product:
 * @device: a #Device
 *
 * Get device product identification.
 *
 * Returns: product name as string
 */
const gchar *device_get_product(Device *device) {
    g_return_val_if_fail(device != NULL, NULL);
    g_return_val_if_fail(IS_DEVICE(device), NULL);
    return device->priv->product;
}

const gchar *device_get_discription(Device *device) {
    g_return_val_if_fail(device != NULL, NULL);
    g_return_val_if_fail(IS_DEVICE(device), NULL);
    return device->priv->discription;
}

const gchar *device_get_serial(Device *device) {
    g_return_val_if_fail(device != NULL, NULL);
    g_return_val_if_fail(IS_DEVICE(device), NULL);
    return device->priv->serial;
}
gint64 device_get_created(Device *device) {
    g_return_val_if_fail(device != NULL, 0);
    g_return_val_if_fail(IS_DEVICE(device), 0);
    return device->priv->created;
}
gint64 device_get_updated(Device *device) {
    g_return_val_if_fail(device != NULL, 0);
    g_return_val_if_fail(IS_DEVICE(device), 0);
    return device->priv->updated;
}

static gboolean device_check_json_object(JsonObject *object) {
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

Device *device_new_from_json(const gchar *json_body) {
    GError *    error  = NULL;
    Device *    device = NULL;
    JsonParser *parser = json_parser_new();
    json_parser_load_from_data(parser, json_body, -1, &error);
    if (error) {
        g_warning("JSon Parser(%s) decode error %d - %s", g_type_name(TYPE_DEVICE), error->code, error->message);
        g_error_free(error);

    } else {
        JsonNode *jnode = json_parser_get_root(parser);
        if (JSON_NODE_HOLDS_OBJECT(jnode) && device_check_json_object(json_node_get_object(jnode))) {
            device = DEVICE(json_gobject_deserialize(TYPE_DEVICE, jnode));
            if (device == NULL) {
                g_warning("Deserialize device failed");
            }
        }
    }
    g_object_unref(parser);
    return device;
}
gboolean device_get_autostart(Device *device) {
    g_return_val_if_fail(device != NULL, FALSE);
    g_return_val_if_fail(IS_DEVICE(device), FALSE);
    return device->priv->autostart;
}

void device_check_serial(Device *device, const gchar *old_serian) {
    g_return_if_fail(device != NULL);
    g_return_if_fail(IS_DEVICE(device));
    if (g_utf8_strlen(device->priv->serial, 10) < 4) {
        g_object_set(device, "serial", old_serian, NULL);
    }
}
guint device_get_run_counter(Device *device) {
    g_return_val_if_fail(device != NULL, 0);
    g_return_val_if_fail(IS_DEVICE(device), 0);
    return device->priv->run_counter;
}
void device_up_run_counter(Device *device) {
    g_return_if_fail(device != NULL);
    g_return_if_fail(IS_DEVICE(device));
    g_object_set(device,"counter",device->priv->run_counter+1,NULL);
}
