/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * statistics.c
 * Copyright (C) LAR 2017
 */

#include "statistics.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>

enum {
    PROP_0,
    PROP_ONLINE,
    PROP_SINGLE,
    PROP_CALIBRATION,
    PROP_CHECK,
};

struct _StatisticsClass {
    GObjectClass parent_class;
};

struct _Statistics {
    GObject    parent_instance;
    Statistic *online;
    Statistic *single;
    Statistic *check;
    Statistic *calibration;
};

static JsonSerializableIface *statistics_serializable_iface = NULL;

static void json_statistics_iface_init(gpointer g_iface);

static JsonNode *statistics_serialize_property(JsonSerializable *serializable, const gchar *name, const GValue *value, GParamSpec *pspec) {
    JsonNode *retval = NULL;
    retval           = statistics_serializable_iface->serialize_property(serializable, name, value, pspec);
    return retval;
}

gboolean statistics_deserialize_property(JsonSerializable *serializable, const gchar *property_name, GValue *value, GParamSpec *pspec, JsonNode *property_node) {
    // g_debug("deserialize device property %s", property_name);

    return statistics_serializable_iface->deserialize_property(serializable, property_name, value, pspec, property_node);
}

static void json_statistics_iface_init(gpointer g_iface) {
    JsonSerializableIface *iface = g_iface;

    statistics_serializable_iface = g_type_default_interface_peek(JSON_TYPE_SERIALIZABLE);
    iface->serialize_property     = statistics_serialize_property;
    iface->deserialize_property   = statistics_deserialize_property;
}

G_DEFINE_TYPE_WITH_CODE(Statistics, statistics, G_TYPE_OBJECT, G_IMPLEMENT_INTERFACE(JSON_TYPE_SERIALIZABLE, json_statistics_iface_init));

static void statistics_init(Statistics *statistics) {
    statistics->online      = STATISTIC(g_object_new(TYPE_STATISTIC, NULL));
    statistics->single      = STATISTIC(g_object_new(TYPE_STATISTIC, NULL));
    statistics->calibration = STATISTIC(g_object_new(TYPE_STATISTIC, "replicates", 5, "outliers", 2, "cv", 2.0, NULL));
    statistics->check       = STATISTIC(g_object_new(TYPE_STATISTIC, NULL));
}

static void statistics_finalize(GObject *object) {
    Statistics *statistics = STATISTICS(object);
    if (statistics->online) g_object_unref(statistics->online);
    if (statistics->single) g_object_unref(statistics->single);
    if (statistics->calibration) g_object_unref(statistics->calibration);
    if (statistics->check) g_object_unref(statistics->check);
    G_OBJECT_CLASS(statistics_parent_class)->finalize(object);
}

static void statistics_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_STATISTICS(object));
    Statistics *statistics = STATISTICS(object);
    switch (prop_id) {
    case PROP_ONLINE:
        if (statistics->online) g_object_unref(statistics->online);
        statistics->online = g_value_dup_object(value);
        break;
    case PROP_SINGLE:
        if (statistics->single) g_object_unref(statistics->single);
        statistics->single = g_value_dup_object(value);
        break;
    case PROP_CALIBRATION:
        if (statistics->calibration) g_object_unref(statistics->calibration);
        statistics->calibration = g_value_dup_object(value);
        break;
    case PROP_CHECK:
        if (statistics->check) g_object_unref(statistics->check);
        statistics->check = g_value_dup_object(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void statistics_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_STATISTICS(object));
    Statistics *statistics = STATISTICS(object);
    switch (prop_id) {
    case PROP_ONLINE:
        g_value_set_object(value, statistics->online);
        break;
    case PROP_SINGLE:
        g_value_set_object(value, statistics->single);
        break;
    case PROP_CALIBRATION:
        g_value_set_object(value, statistics->calibration);
        break;
    case PROP_CHECK:
        g_value_set_object(value, statistics->check);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void statistics_class_init(StatisticsClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // Class* parent_class = MODEL_CLASS (klass);
    object_class->finalize     = statistics_finalize;
    object_class->set_property = statistics_set_property;
    object_class->get_property = statistics_get_property;

    g_object_class_install_property(
        object_class, PROP_ONLINE, g_param_spec_object("online", "online", "online statistic object", TYPE_STATISTIC, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_SINGLE, g_param_spec_object("single", "single statistic", "Single statistic", TYPE_STATISTIC, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_CALIBRATION, g_param_spec_object("calibration", "calibration statistic", "calibration statistic", TYPE_STATISTIC, G_PARAM_READWRITE ));
    g_object_class_install_property(
        object_class, PROP_CHECK, g_param_spec_object("check", "check statistic", "check statistic", TYPE_STATISTIC, G_PARAM_READWRITE ));
}

Statistic *StatisticsGetOnline(Statistics *statistics) {
    g_return_val_if_fail(statistics != NULL, NULL);
    return statistics->online;
}
Statistic *StatisticsGetSingle(Statistics *statistics) {
    g_return_val_if_fail(statistics != NULL, NULL);
    return statistics->single;
}
Statistic *StatisticsGetCalibration(Statistics *statistics) {
    g_return_val_if_fail(statistics != NULL, NULL);
    return statistics->calibration;
}
Statistic *StatisticsGetCheck(Statistics *statistics) {
    g_return_val_if_fail(statistics != NULL, NULL);
    return statistics->check;
}
