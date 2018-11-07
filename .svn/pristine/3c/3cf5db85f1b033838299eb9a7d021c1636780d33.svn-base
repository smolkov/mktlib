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

struct _LarStreamPrivate {
    gchar *name;
};

enum { PROP_0, PROP_NAME};

static JsonSerializableIface *serializable_iface = NULL;

static void json_serializable_iface_init(gpointer g_iface);

G_DEFINE_TYPE_WITH_CODE(LarStream, lar_stream, G_TYPE_OBJECT, G_IMPLEMENT_INTERFACE(JSON_TYPE_SERIALIZABLE, json_serializable_iface_init));

static JsonNode *lar_stream_serialize_property(JsonSerializable *serializable, const gchar *name, const GValue *value, GParamSpec *pspec) {
    JsonNode *retval = NULL;
    return serializable_iface->serialize_property(serializable, name, value, pspec);
}

static gboolean lar_stream_deserialize_property(
    JsonSerializable *serializable, const gchar *property_name, GValue *value, GParamSpec *pspec, JsonNode *property_node) {
    // g_debug("deserialize device property %s", property_name);
    g_return_val_if_fail(JSON_IS_SERIALIZABLE(serializable), FALSE);
    g_return_val_if_fail(property_name != NULL, FALSE);
    g_return_val_if_fail(value != NULL, FALSE);
    g_return_val_if_fail(pspec != NULL, FALSE);
    g_return_val_if_fail(property_node != NULL, FALSE);

    if (g_strcmp0(property_name, "name") == 0) {
        if (JSON_NODE_HOLDS_VALUE(property_node)) {
            g_value_set_string(value, json_node_get_string(property_node));
            return TRUE;
        } else if (JSON_NODE_HOLDS_OBJECT(property_node)) {
            g_warning("id from value deserialize not defined");
            return FALSE;
        }
    }
    return FALSE;
}

static void json_serializable_iface_init(gpointer g_iface) {
    JsonSerializableIface *iface = g_iface;

    serializable_iface          = g_type_default_interface_peek(JSON_TYPE_SERIALIZABLE);
    iface->serialize_property   = lar_stream_serialize_property;
    iface->deserialize_property = lar_stream_deserialize_property;
}

static void lar_stream_init(LarStream *lar_stream) {
    lar_stream->priv = G_TYPE_INSTANCE_GET_PRIVATE(lar_stream, LAR_TYPE_STREAM, LarStreamPrivate);

    /* TODO: Add initialization code here */
}

static void lar_stream_finalize(GObject *object) {
    LarStream *device = LAR_STREAM(object);
    if (device->priv->name) g_free(device->priv->name);

    G_OBJECT_CLASS(lar_stream_parent_class)->finalize(object);
}

static void lar_stream_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(LAR_IS_STREAM(object));
    LarStream *device = LAR_STREAM(object);
    switch (prop_id) {
    case PROP_NAME:
        if (device->priv->name) g_free(device->priv->name);
        device->priv->name= g_value_dup_string(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_stream_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(LAR_IS_STREAM(object));
    LarStream *device = LAR_STREAM(object);

    switch (prop_id) {
    case PROP_NAME:
        g_value_set_string(value, device->priv->name);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_stream_class_init(LarStreamClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    g_type_class_add_private(klass, sizeof(LarStreamPrivate));

    object_class->finalize     = lar_stream_finalize;
    object_class->set_property = lar_stream_set_property;
    object_class->get_property = lar_stream_get_property;
    g_object_class_install_property(
        object_class, PROP_NAME, g_param_spec_string("name", "name", "name", "none", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_UPDATED,
        g_param_spec_int64("updated", "updated", "updated time in msec", 0, G_MAXINT64, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
}


const gchar *lar_stream_get_name(LarStream *device) {
    g_return_val_if_fail(device != NULL, NULL);
    g_return_val_if_fail(LAR_IS_STREAM(device), NULL);
    return device->priv->name;
}

