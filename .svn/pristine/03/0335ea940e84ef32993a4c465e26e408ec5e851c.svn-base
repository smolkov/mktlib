/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * interval.c
 * Copyright (C) LAR 2017 
 
 */

#include "statistic.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>

enum
{
    PROP_0,
    PROP_STATISTIC_REPLICATES,
    PROP_STATISTIC_OUTLIERS,
    PROP_STATISTIC_MAX_CV,
    PROP_STATISTIC_THRESHOLD,
    PROP_STATISTIC_JUMP,
};

struct _StatisticClass
{
    GObjectClass parent_class;
};

struct _Statistic
{
    GObject parent_instance;
    guint replicates;
    guint outliers;
    gdouble max_cv;
    gdouble threshold;
    gdouble jump;
    guint number;
};

static JsonSerializableIface *statistic_serializable_iface = NULL;

static void json_statistic_iface_init(gpointer g_iface);

static JsonNode *statistic_serialize_property(JsonSerializable *serializable, const gchar *name, const GValue *value, GParamSpec *pspec)
{
    JsonNode *retval = NULL;
    retval = statistic_serializable_iface->serialize_property(serializable, name, value, pspec);
    return retval;
}

gboolean statistic_deserialize_property(JsonSerializable *serializable, const gchar *property_name, GValue *value, GParamSpec *pspec, JsonNode *property_node)
{
    // g_debug("deserialize device property %s", property_name);

    return statistic_serializable_iface->deserialize_property(serializable, property_name, value, pspec, property_node);
}

static void json_statistic_iface_init(gpointer g_iface)
{
    JsonSerializableIface *iface = g_iface;

    statistic_serializable_iface = g_type_default_interface_peek(JSON_TYPE_SERIALIZABLE);
    iface->serialize_property = statistic_serialize_property;
    iface->deserialize_property = statistic_deserialize_property;
}

G_DEFINE_TYPE_WITH_CODE(Statistic, statistic, G_TYPE_OBJECT, G_IMPLEMENT_INTERFACE(JSON_TYPE_SERIALIZABLE, json_statistic_iface_init));

static void statistic_init(Statistic *statistic)
{
    statistic->replicates = 1;
    statistic->outliers = 0;
    statistic->threshold = 0.0;
    statistic->jump = 0.0;
}

static void statistic_finalize(GObject *object)
{
    // Statistic *statistic = STATISTIC(object);
    G_OBJECT_CLASS(statistic_parent_class)->finalize(object);
}

static void statistic_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_STATISTIC(object));
    Statistic *statistic = STATISTIC(object);
    switch (prop_id)
    {
    case PROP_STATISTIC_REPLICATES:
        statistic->replicates = g_value_get_uint(value);
        break;
    case PROP_STATISTIC_OUTLIERS:
        statistic->outliers = g_value_get_uint(value);
        break;
    case PROP_STATISTIC_MAX_CV:
        statistic->max_cv = g_value_get_double(value);
        break;
    case PROP_STATISTIC_THRESHOLD:
        statistic->threshold = g_value_get_double(value);
        break;
    case PROP_STATISTIC_JUMP:
        statistic->jump = g_value_get_double(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void statistic_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_STATISTIC(object));
    Statistic *statistic = STATISTIC(object);
    switch (prop_id)
    {
    case PROP_STATISTIC_REPLICATES:
        g_value_set_uint(value, statistic->replicates);
        break;
    case PROP_STATISTIC_OUTLIERS:
        g_value_set_uint(value, statistic->outliers);
        break;
    case PROP_STATISTIC_MAX_CV:
        g_value_set_double(value, statistic->max_cv);
        break;
    case PROP_STATISTIC_THRESHOLD:
        g_value_set_double(value, statistic->threshold);
        break;
    case PROP_STATISTIC_JUMP:
        g_value_set_double(value, statistic->jump);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void statistic_class_init(StatisticClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // Class* parent_class = MODEL_CLASS (klass);
    object_class->finalize = statistic_finalize;
    object_class->set_property = statistic_set_property;
    object_class->get_property = statistic_get_property;

    g_object_class_install_property(object_class, PROP_STATISTIC_REPLICATES,
                                    g_param_spec_uint("replicates", "statistic replicates", "Set get statistic replicates", 1, 100, 1, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_STATISTIC_OUTLIERS, g_param_spec_uint("outliers", "statistic outlier", "Set get statistic outlier", 0, 30, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_STATISTIC_MAX_CV, g_param_spec_double("cv", "statistic max CV", "Set get statistic max cv", 0., 100., 2.5, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_STATISTIC_THRESHOLD, g_param_spec_double("threshold", "statistic threshold", "Set get statistic threshold", 0., 100., 0., G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_STATISTIC_JUMP, g_param_spec_double("jump", "statistic jump", "statistic jump", 0., 100., 0., G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
}

guint StatisticGetReplicates(Statistic *statistic)
{
    g_return_val_if_fail(statistic != NULL, 0);
    return statistic->replicates;
}
guint StatisticGetOutliers(Statistic *statistic)
{
    g_return_val_if_fail(statistic != NULL, 0);
    return statistic->outliers;
}
gdouble StatisticGetCV(Statistic *statistic)
{
    g_return_val_if_fail(statistic != NULL, 0);
    return statistic->max_cv;
}
gdouble StatisticGetThreshold(Statistic *statistic)
{
    g_return_val_if_fail(statistic != NULL, 0);
    return statistic->threshold;
}
gdouble StatisticGetJump(Statistic *statistic)
{
    g_return_val_if_fail(statistic != NULL, 0);
    return statistic->jump;
}

guint StatisticGetMeasurementNumber(Statistic *statistic)
{
    g_return_val_if_fail(statistic != NULL, 0);
    return statistic->number;
}

void StatisticSetMeasurementNumber(Statistic *statistic,guint number){
    g_return_if_fail(statistic!=NULL);
    statistic->number=number;
}
