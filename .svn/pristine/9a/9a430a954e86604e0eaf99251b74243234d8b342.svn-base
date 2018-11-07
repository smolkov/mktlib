/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * measurement.c
 * Copyright (C) LAR 2017
 *

 */

#include "measurementdata.h"
#include "value.h"
#include "larsqlite.h"

enum {
    PROP_0,
    PROP_KEY,
    PROP_CHANGED,
    PROP_VALUE,
    PROP_VALUE_ROW,
    PROP_RESULT,
    PROP_SIGNAL,
    PROP_TYPE,
    PROP_REPLICATE,
    PROP_OUTLIER,
};

struct _LarMeasurementDataPrivate {
    guint64  key;
    gint64   changed;
    gdouble  value;
    gdouble  valueRaw;
    guint64  result;
    guint64  signalData;
    gint     type;
    guint    replicate;
    gboolean outlier;
};

#define LAR_MEASUREMENT_DATA_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_LAR_MEASUREMENT_DATA, LarMeasurementDataPrivate))

G_DEFINE_TYPE(LarMeasurementData, measurement_data, G_TYPE_OBJECT)

static void measurement_data_init(LarMeasurementData *measurement_data) {
    LarMeasurementDataPrivate *priv = LAR_MEASUREMENT_DATA_PRIVATE(measurement_data);
    priv->changed                = g_get_real_time();
    priv->value                  = 0.0;
    priv->signalData             = 0;
    priv->result                 = 0;
    priv->type                   = 0;
    priv->outlier                = FALSE;
    priv->replicate              = 1;
    measurement_data->priv       = priv;

    /* TODO: Add initialization code here */
}

static void measurement_data_finalize(GObject *object) {
    // LarMeasurementData *data = LAR_MEASUREMENT_DATA(object);

    G_OBJECT_CLASS(measurement_data_parent_class)->finalize(object);
}

static void measurement_data_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_LAR_MEASUREMENT_DATA(object));
    LarMeasurementData *data = LAR_MEASUREMENT_DATA(object);
    switch (prop_id) {
    case PROP_CHANGED:
        data->priv->changed = g_value_get_int64(value);
        break;
    case PROP_VALUE:
        data->priv->value = g_value_get_double(value);
        break;
    case PROP_VALUE_ROW:
        data->priv->valueRaw = g_value_get_double(value);
        break;
    case PROP_SIGNAL:
        data->priv->signalData = g_value_get_uint64(value);
        break;
    case PROP_RESULT:
        data->priv->result = g_value_get_uint64(value);
        break;
    case PROP_TYPE:
        data->priv->type = g_value_get_int(value);
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

static void measurement_data_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_LAR_MEASUREMENT_DATA(object));
    LarMeasurementData *data = LAR_MEASUREMENT_DATA(object);
    switch (prop_id) {
    case PROP_CHANGED:
        g_value_set_int64(value, data->priv->changed);
        break;
    case PROP_VALUE:
        g_value_set_double(value, data->priv->value);
        break;
    case PROP_VALUE_ROW:
        g_value_set_double(value, data->priv->valueRaw);
        break;
    case PROP_SIGNAL:
        g_value_set_uint64(value, data->priv->signalData);
        break;
    case PROP_RESULT:
        g_value_set_uint64(value, data->priv->result);
        break;
    case PROP_TYPE:
        g_value_set_int(value, data->priv->type);
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

static void measurement_data_class_init(LarMeasurementDataClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // ModelClass* parent_class = MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(LarMeasurementDataPrivate));
    object_class->finalize     = measurement_data_finalize;
    object_class->set_property = measurement_data_set_property;
    object_class->get_property = measurement_data_get_property;
    g_object_class_install_property(object_class, PROP_KEY, g_param_spec_uint64("key", "Key", "Key", 0, G_MAXUINT64, 0, G_PARAM_READWRITE| SQLITE_DB_PRIMERY_KEY));
    
    g_object_class_install_property(
        object_class, PROP_RESULT, g_param_spec_uint64("result", "LarMeasurement stream reference", "LarMeasurement stream reference", 0, G_MAXUINT64, 0, G_PARAM_READABLE | G_PARAM_WRITABLE));

    g_object_class_install_property(object_class, PROP_TYPE, g_param_spec_int("type", "LarMeasurement type", "Set get measurement type property", 0, 255, 0, G_PARAM_READABLE | G_PARAM_WRITABLE));

    g_object_class_install_property(
        object_class, PROP_CHANGED, g_param_spec_int64("changed", "Create measurement time", "Set get create measurement time", 0, G_MAXINT64, g_get_real_time(), G_PARAM_READWRITE));

    g_object_class_install_property(
        object_class, PROP_VALUE, g_param_spec_double("value", "LarMeasurement measurement property", "Set get sensor measurement property", 0., G_MAXDOUBLE, 0., G_PARAM_READABLE | G_PARAM_WRITABLE));

    g_object_class_install_property(object_class, PROP_VALUE_ROW,
        g_param_spec_double("valueRaw", "LarMeasurement measurement property", "Set get sensor measurement property", 0., G_MAXDOUBLE, 0., G_PARAM_READABLE | G_PARAM_WRITABLE));

    g_object_class_install_property(
        object_class, PROP_SIGNAL, g_param_spec_uint64("signalData", "LarMeasurement measurement trigger", "Set get sensor trigger property", 0, G_MAXUINT64, 0, G_PARAM_READABLE | G_PARAM_WRITABLE));

    g_object_class_install_property(
        object_class, PROP_OUTLIER, g_param_spec_boolean("outlier", "LarMeasurement measurement outlier", "Set get sensor outlier property", FALSE, G_PARAM_READABLE | G_PARAM_WRITABLE));

    g_object_class_install_property(
        object_class, PROP_REPLICATE, g_param_spec_uint("replicate", "LarMeasurement type", "Set get measurement type property", 0, 255, 0, G_PARAM_READABLE | G_PARAM_WRITABLE));

}

/**
 * LarMeasurementDataGetResult:
 * @measurement: a #LarMeasurementData
 *
 * result id .
 *
 * Return:  result id #guint64
 */
guint64 LarMeasurementDataGetResult(LarMeasurementData *measurement) {
    g_return_val_if_fail(measurement != NULL, 0);
    g_return_val_if_fail(IS_LAR_MEASUREMENT_DATA(measurement), 0);
    return measurement->priv->result;
}

/**
 * LarMeasurementDataGetChanged:
 * @measurement: a #LarMeasurementData
 *
 * changet time in mictoseconds.
 *
 * Return:  the number of microseconds since January 1, 1970 UTC.  #guint64
 */
gint64 LarMeasurementDataGetChanged(LarMeasurementData *measurement) {
    g_return_val_if_fail(measurement != NULL, 0);
    g_return_val_if_fail(IS_LAR_MEASUREMENT_DATA(measurement), 0);
    return measurement->priv->changed;
}
/**
 * LarMeasurementDataGetValue:
 * @measurement: a #LarMeasurementData
 *
 * measurement result 
 *
 * Return:  measurement result
 */
gdouble LarMeasurementDataGetValue(LarMeasurementData *measurement) {
    g_return_val_if_fail(measurement != NULL, 0.0);
    g_return_val_if_fail(IS_LAR_MEASUREMENT_DATA(measurement), 0.0);
    return measurement->priv->value;
}
/**
 * LarMeasurementDataGetRawValue:
 * @measurement: a #LarMeasurementData
 *
 * measurement raw value 
 *
 * Return:  measurement raw value 
 */
gdouble LarMeasurementDataGetRawValue(LarMeasurementData *measurement) {
    g_return_val_if_fail(measurement != NULL, 0.0);
    g_return_val_if_fail(IS_LAR_MEASUREMENT_DATA(measurement), 0.0);
    return measurement->priv->valueRaw;
}


/**
 * LarMeasurementDataGetSignal:
 * @measurement: a #LarMeasurementData
 *
 * signal id .
 *
 * Return:  signal id #guint64
 */
guint64 LarMeasurementDataGetSignal(LarMeasurementData *measurement) {
    g_return_val_if_fail(measurement != NULL, 0);
    g_return_val_if_fail(IS_LAR_MEASUREMENT_DATA(measurement), 0);
    return measurement->priv->signalData;
}


/**
 * LarMeasurementDataGetReplicate:
 * @measurement: a #LarMeasurementData
 *
 * replicate number
 *
 * Return:  replicate number
 */
guint LarMeasurementDataGetReplicate(LarMeasurementData *measurement) {
    g_return_val_if_fail(measurement != NULL, 0);
    g_return_val_if_fail(IS_LAR_MEASUREMENT_DATA(measurement), 0);
    return measurement->priv->replicate;
}

/**
 * LarMeasurementDataGetOutlier:
 * @measurement: a #LarMeasurementData
 *
 * is outlier
 *
 * Return: TRUE if data outlier.
 */
gboolean LarMeasurementDataGetOutlier(LarMeasurementData *measurement) {
    g_return_val_if_fail(measurement != NULL, FALSE);
    g_return_val_if_fail(IS_LAR_MEASUREMENT_DATA(measurement), FALSE);
    return measurement->priv->outlier;
}