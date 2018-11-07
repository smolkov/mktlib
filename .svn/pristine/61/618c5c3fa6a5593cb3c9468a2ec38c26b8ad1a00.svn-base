/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * larmeasurement.c
 * Copyright (C) LAR 2017
 *

 */

#include "larmeasurement.h"
#include "larsqlite.h"
#include "value.h"

enum {
    PROP_0,
    PROP_KEY,
    PROP_RESULT,
    PROP_CHANGED,
    PROP_VALUE,
    PROP_VALUE_ROW,
    PROP_SIGNAL,
    PROP_REPLICATE,
    PROP_OUTLIER,
};


struct _LarMeasurementPrivate {
    guint64  key;
    gint64   changed;
    guint64  result;
    gdouble  value;
    gdouble  valueRaw;
    gint     type;
    guint    replicate;
    gboolean outlier;
};

#define LAR_MEASUREMENT_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_LAR_MEASUREMENT, LarMeasurementPrivate))

G_DEFINE_TYPE(LarMeasurement, lar_measurement, G_TYPE_OBJECT)

static void lar_measurement_init(LarMeasurement *measurement) {
    LarMeasurementPrivate *priv = LAR_MEASUREMENT_PRIVATE(measurement);
    GDateTime *dt  = g_date_time_new_now_utc();
    priv->key                = 0;
    priv->changed            = g_date_time_to_unix(dt);
    priv->value              = 0.0;
    priv->valueRaw           = 0.0;
    priv->result             = 0;
    priv->outlier            = FALSE;
    priv->replicate          = 1;
    measurement->priv = priv;
    g_date_time_unref(dt);

    /* TODO: Add initialization code here */
}

static void lar_measurement_finalize(GObject *object) {
    // LarMeasurement *data = LAR_MEASUREMENT(object);

    G_OBJECT_CLASS(lar_measurement_parent_class)->finalize(object);
}

static void lar_measurement_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_LAR_MEASUREMENT(object));
    LarMeasurement *data = LAR_MEASUREMENT(object);
    switch (prop_id) {
    case PROP_KEY:
        data->priv->key = g_value_get_uint64(value);
        break;
    case PROP_RESULT:
        data->priv->result = g_value_get_uint64(value);
        break;
    case PROP_CHANGED:
        data->priv->changed = g_value_get_int64(value);
        break;
    case PROP_VALUE:
        data->priv->value = g_value_get_double(value);
        break;
    case PROP_VALUE_ROW:
        data->priv->valueRaw = g_value_get_double(value);
        break;
    case PROP_OUTLIER:
        data->priv->outlier = g_value_get_boolean(value);
        break;
    case PROP_REPLICATE:
        data->priv->replicate = g_value_get_uint(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_measurement_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_LAR_MEASUREMENT(object));
    LarMeasurement *data = LAR_MEASUREMENT(object);
    switch (prop_id) {
    case PROP_KEY:
        g_value_set_uint64(value, data->priv->key);
        break;
    case PROP_RESULT:
        g_value_set_uint64(value, data->priv->result);
        break;
    case PROP_CHANGED:
        g_value_set_int64(value, data->priv->changed);
        break;
    case PROP_VALUE:
        g_value_set_double(value, data->priv->value);
        break;
    case PROP_VALUE_ROW:
        g_value_set_double(value, data->priv->valueRaw);
        break;
    case PROP_OUTLIER:
        g_value_set_boolean(value, data->priv->outlier);
        break;
    case PROP_REPLICATE:
        g_value_set_uint(value, data->priv->replicate);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_measurement_class_init(LarMeasurementClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // ModelClass* parent_class = MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(LarMeasurementPrivate));
    object_class->finalize     = lar_measurement_finalize;
    object_class->set_property = lar_measurement_set_property;
    object_class->get_property = lar_measurement_get_property;

    g_object_class_install_property(object_class, PROP_KEY, g_param_spec_uint64(SQLITE_PK_PROPERTY, "Key", "Key", 0, G_MAXUINT64, 0, G_PARAM_READWRITE| SQLITE_DB_PRIMERY_KEY));
    g_object_class_install_property(object_class, PROP_RESULT, g_param_spec_uint64("result", "result", "result", 0, G_MAXUINT64, 0, G_PARAM_READWRITE));
    g_object_class_install_property(
        object_class, PROP_CHANGED, g_param_spec_int64("changed", "Create measurement time", "Set get create measurement time", 0, G_MAXINT64, 0, G_PARAM_READWRITE));

    g_object_class_install_property(
        object_class, PROP_VALUE, g_param_spec_double("value", "LarMeasurement measurement property", "Set get sensor measurement property", 0., G_MAXDOUBLE, 0., G_PARAM_READABLE | G_PARAM_WRITABLE));

    g_object_class_install_property(object_class, PROP_VALUE_ROW,
        g_param_spec_double("valueRaw", "LarMeasurement measurement property", "Set get sensor measurement property", 0.0, G_MAXDOUBLE, 0., G_PARAM_READABLE | G_PARAM_WRITABLE));

    g_object_class_install_property(
        object_class, PROP_OUTLIER, g_param_spec_boolean("outlier", "LarMeasurement measurement outlier", "Set get sensor outlier property", FALSE, G_PARAM_READABLE | G_PARAM_WRITABLE));

    g_object_class_install_property(
        object_class, PROP_REPLICATE, g_param_spec_uint("replicate", "LarMeasurement type", "Set get measurement type property", 0, 255, 0, G_PARAM_READABLE | G_PARAM_WRITABLE));
}

/**
 * m_LarMeasurementGetResult:
 * @measurement: a #LarMeasurement
 *
 * result id .
 *
 * Return:  result id #guint64
 */
guint64 m_LarMeasurementGetResult(LarMeasurement *measurement) {
    g_return_val_if_fail(measurement != NULL, 0);
    g_return_val_if_fail(IS_LAR_MEASUREMENT(measurement), 0);
    return measurement->priv->result;
}

/**
 * m_LarMeasurementGetChanged:
 * @measurement: a #LarMeasurement
 *
 * changet time in mictoseconds.
 *
 * Return:  the number of microseconds since January 1, 1970 UTC.  #guint64
 */
gint64 m_LarMeasurementGetChanged(LarMeasurement *measurement) {
    g_return_val_if_fail(measurement != NULL, 0);
    g_return_val_if_fail(IS_LAR_MEASUREMENT(measurement), 0);
    return measurement->priv->changed;
}
/**
 * m_LarMeasurementGetValue:
 * @measurement: a #LarMeasurement
 *
 * measurement result
 *
 * Return:  measurement result
 */
gdouble m_LarMeasurementGetValue(LarMeasurement *measurement) {
    g_return_val_if_fail(measurement != NULL, 0.0);
    g_return_val_if_fail(IS_LAR_MEASUREMENT(measurement), 0.0);
    return measurement->priv->value;
}
/**
 * m_LarMeasurementGetRawValue:
 * @measurement: a #LarMeasurement
 *
 * measurement raw value
 *
 * Return:  measurement raw value
 */
gdouble m_LarMeasurementGetRawValue(LarMeasurement *measurement) {
    g_return_val_if_fail(measurement != NULL, 0.0);
    g_return_val_if_fail(IS_LAR_MEASUREMENT(measurement), 0.0);
    return measurement->priv->valueRaw;
}



/**
 * m_LarMeasurementGetReplicate:
 * @measurement: a #LarMeasurement
 *
 * replicate number
 *
 * Return:  replicate number
 */
guint m_LarMeasurementGetReplicate(LarMeasurement *measurement) {
    g_return_val_if_fail(measurement != NULL, 0);
    g_return_val_if_fail(IS_LAR_MEASUREMENT(measurement), 0);
    return measurement->priv->replicate;
}

/**
 * m_LarMeasurementGetOutlier:
 * @measurement: a #LarMeasurement
 *
 * is outlier
 *
 * Return: TRUE if data outlier.
 */
gboolean m_LarMeasurementGetOutlier(LarMeasurement *measurement) {
    g_return_val_if_fail(measurement != NULL, FALSE);
    g_return_val_if_fail(IS_LAR_MEASUREMENT(measurement), FALSE);
    return measurement->priv->outlier;
}
