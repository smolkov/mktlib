/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * sensor.c
 * Copyright (C) 2017 LAR
 *
 *
 */

#include "sensor.h"

enum
{
    PROP_0,
    PROP_MODEL,
    PROP_STATE,
    PROP_MIN1,
    PROP_MAX1,
    PROP_MIN2,
    PROP_MAX2,
    PROP_MIN3,
    PROP_MAX3,
};

struct _SensorPrivate
{
    gdouble min1;
    gdouble max1;
    gdouble min2;
    gdouble max2;
    gdouble min3;
    gdouble max3;
    guint   state;
    guint model;
};

#define SENSOR_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_SENSOR, SensorPrivate))

G_DEFINE_TYPE(Sensor, sensor, G_TYPE_OBJECT)

static void sensor_init(Sensor *sensor)
{
    SensorPrivate *priv = SENSOR_PRIVATE(sensor);
    sensor->priv = priv;
    /* TODO: Add initialization code here */
}

static void sensor_finalize(GObject *object)
{
    // Sensor *data = SENSOR(object);
    G_OBJECT_CLASS(sensor_parent_class)->finalize(object);
}

static void sensor_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_SENSOR(object));
    Sensor *sensor = SENSOR(object);
    switch (prop_id)
    {
    case PROP_MODEL:
        sensor->priv->model= g_value_get_uint(value);
        break;
    case PROP_STATE:
        sensor->priv->state= g_value_get_uint(value);
        break;
    case PROP_MIN1:
        sensor->priv->min1 = g_value_get_double(value);
        break;
    case PROP_MAX1:
        sensor->priv->max1 = g_value_get_double(value);
        break;
    case PROP_MIN2:
        sensor->priv->min2 = g_value_get_double(value);
        break;
    case PROP_MAX2:
        sensor->priv->max2 = g_value_get_double(value);
        break;
    case PROP_MIN3:
        sensor->priv->min3 = g_value_get_double(value);
        break;
    case PROP_MAX3:
        sensor->priv->max3 = g_value_get_double(value);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void sensor_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_SENSOR(object));
    Sensor *stream = SENSOR(object);
    switch (prop_id)
    {
    case PROP_MODEL:
        g_value_set_uint(value, stream->priv->model);
        break;
    case PROP_STATE:
        g_value_set_uint(value, stream->priv->state);
        break;
    case PROP_MIN1:
        g_value_set_double(value, stream->priv->min1);
        break;
    case PROP_MAX1:
        g_value_set_double(value, stream->priv->max1);
        break;
    case PROP_MIN2:
        g_value_set_double(value, stream->priv->min2);
        break;
    case PROP_MAX2:
        g_value_set_double(value, stream->priv->max2);
        break;
    case PROP_MIN3:
        g_value_set_double(value, stream->priv->min3);
        break;
    case PROP_MAX3:
        g_value_set_double(value, stream->priv->max3);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void sensor_class_init(SensorClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // Class* parent_class = MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(SensorPrivate));
    object_class->finalize = sensor_finalize;
    object_class->set_property = sensor_set_property;
    object_class->get_property = sensor_get_property;

    g_object_class_install_property(object_class, PROP_MODEL, g_param_spec_uint("model", "model", "Sensor model number", 0, 11, 0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_STATE, g_param_spec_uint("state", "state", "Sensor state", 0, 5, 0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_MIN1, g_param_spec_double("min-one", "Min", "min scale limit range one", -G_MAXDOUBLE, G_MAXDOUBLE, 0.0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_MAX1, g_param_spec_double("max-one", "max", "min scale limit range", -G_MAXDOUBLE, G_MAXDOUBLE, 1.0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_MIN2, g_param_spec_double("min-two", "Min", "min scale limit range one", -G_MAXDOUBLE, G_MAXDOUBLE, 0.0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_MAX2, g_param_spec_double("max-two", "max", "min scale limit range", -G_MAXDOUBLE, G_MAXDOUBLE, 1.0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_MIN3, g_param_spec_double("min-three", "Min", "min scale limit range", -G_MAXDOUBLE, G_MAXDOUBLE, 0.0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_MAX3, g_param_spec_double("max-three", "max", "min scale limit range", -G_MAXDOUBLE, G_MAXDOUBLE, 1.0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
}

/**
 * SensorGetMinRange1:
 * @sensor: a #Sensor
 *
 * Get min limit
 *
 * Return:  limit
 */
gdouble SensorGetMinRange1(Sensor *sensor)
{
    g_return_val_if_fail(sensor != NULL, 0.0);
    g_return_val_if_fail(IS_SENSOR(sensor), 0.0);
    return sensor->priv->min1;
}
/**
 * SensorGetMaxRange1:
 * @sensor: a #Sensor
 *
 * Get max limit
 *
 * Return: max scaling limit
 *   default 1.0
 */
gdouble SensorGetMaxRange1(Sensor *sensor)
{
    g_return_val_if_fail(sensor != NULL,1.0);
    g_return_val_if_fail(IS_SENSOR(sensor),1.0);
    return sensor->priv->max1;
}
/**
 * SensorGetModel:
 * @sensor: a #Sensor
 *
 * Get sensor model number
 *
 * Return: 0..11
 *  default 0; - sensor not set.
 */
guint SensorGetModel(Sensor *sensor)
{
    g_return_val_if_fail(sensor != NULL,0);
    g_return_val_if_fail(IS_SENSOR(sensor),0);
    return sensor->priv->model;
}

/**
 * SensorGetState:
 * @sensor: a #Sensor
 *
 * Get sensor state number
 *
 * Return: 0..11
 *  default 0; - sensor not set.
 */
guint SensorGetState(Sensor *sensor)
{
    g_return_val_if_fail(sensor != NULL,0);
    g_return_val_if_fail(IS_SENSOR(sensor),0);
    return sensor->priv->state;
}
