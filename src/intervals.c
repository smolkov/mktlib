/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * intervals.c
 * Copyright (C) LAR 2017 
 *

 */


#include "lartime.h"
#include "intervals.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>

enum {
    PROP_0,
    PROP_ONLINE,
    PROP_CALIBRATION,
    PROP_CHECK,
};

struct _IntervalsClass {
    GObjectClass parent_class;
};

struct _Intervals {
    GObject    parent_instance;
    Interval *online;
    Interval *check;
    Interval *calibration;
};

static JsonSerializableIface *intervals_serializable_iface = NULL;

static void json_intervals_iface_init(gpointer g_iface);

static JsonNode *intervals_serialize_property(JsonSerializable *serializable, const gchar *name, const GValue *value, GParamSpec *pspec) {
    JsonNode *retval = NULL;
    retval           = intervals_serializable_iface->serialize_property(serializable, name, value, pspec);
    return retval;
}

gboolean intervals_deserialize_property(JsonSerializable *serializable, const gchar *property_name, GValue *value, GParamSpec *pspec, JsonNode *property_node) {
    // g_debug("deserialize device property %s", property_name);

    return intervals_serializable_iface->deserialize_property(serializable, property_name, value, pspec, property_node);
}

static void json_intervals_iface_init(gpointer g_iface) {
    JsonSerializableIface *iface = g_iface;

    intervals_serializable_iface = g_type_default_interface_peek(JSON_TYPE_SERIALIZABLE);
    iface->serialize_property     = intervals_serialize_property;
    iface->deserialize_property   = intervals_deserialize_property;
}

G_DEFINE_TYPE_WITH_CODE(Intervals, intervals, G_TYPE_OBJECT, G_IMPLEMENT_INTERFACE(JSON_TYPE_SERIALIZABLE, json_intervals_iface_init));

static void intervals_init(Intervals *intervals) {
    intervals->online      = INTERVAL(g_object_new(TYPE_INTERVAL, "interval", G_TIME_SPAN_MINUTE * 30, "activated",TRUE, NULL));
    intervals->calibration = INTERVAL(g_object_new(TYPE_INTERVAL, "interval", G_TIME_SPAN_HOUR * 12, "activated",FALSE, NULL));
    intervals->check       = INTERVAL(g_object_new(TYPE_INTERVAL, "interval", G_TIME_SPAN_HOUR * 4, "activated",FALSE, NULL));
}

static void intervals_finalize(GObject *object) {
    Intervals *intervals = INTERVALS(object);
    if (intervals->online) g_object_unref(intervals->online);
    if (intervals->calibration) g_object_unref(intervals->calibration);
    if (intervals->check) g_object_unref(intervals->check);
    G_OBJECT_CLASS(intervals_parent_class)->finalize(object);
}

static void intervals_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_INTERVALS(object));
    Intervals *intervals = INTERVALS(object);
    switch (prop_id) {
    case PROP_ONLINE:
        if (intervals->online) g_object_unref(intervals->online);
        intervals->online = g_value_dup_object(value);
        break;
    case PROP_CALIBRATION:
        if (intervals->calibration) g_object_unref(intervals->calibration);
        intervals->calibration = g_value_dup_object(value);
        break;
    case PROP_CHECK:
        if (intervals->check) g_object_unref(intervals->check);
        intervals->check = g_value_dup_object(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void intervals_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_INTERVALS(object));
    Intervals *intervals = INTERVALS(object);
    switch (prop_id) {
    case PROP_ONLINE:
        g_value_set_object(value, intervals->online);
        break;
    case PROP_CALIBRATION:
        g_value_set_object(value, intervals->calibration);
        break;
    case PROP_CHECK:
        g_value_set_object(value, intervals->check);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void intervals_class_init(IntervalsClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // Class* parent_class = MODEL_CLASS (klass);
    object_class->finalize     = intervals_finalize;
    object_class->set_property = intervals_set_property;
    object_class->get_property = intervals_get_property;

    g_object_class_install_property(
        object_class, PROP_ONLINE, g_param_spec_object("online", "online", "online interval object", TYPE_INTERVAL, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(
        object_class, PROP_CALIBRATION, g_param_spec_object("calibration", "calibration interval", "calibration interval", TYPE_INTERVAL, G_PARAM_READWRITE));
    g_object_class_install_property(
        object_class, PROP_CHECK, g_param_spec_object("check", "check interval", "check interval", TYPE_INTERVAL, G_PARAM_READWRITE));
}

Interval *IntervalsGetOnline(Intervals *intervals) {
    g_return_val_if_fail(intervals != NULL, NULL);
    return intervals->online;
}
Interval *IntervalsGetCalibration(Intervals *intervals) {
    g_return_val_if_fail(intervals != NULL, NULL);
    return intervals->calibration;
}
Interval *IntervalsGetCheck(Intervals *intervals) {
    g_return_val_if_fail(intervals != NULL, NULL);
    return intervals->check;
}


