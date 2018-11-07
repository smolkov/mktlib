/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * humidity.c
 * Copyright (C) 2017 LAR
 *
 *
 */

#include "humidity.h"

enum { PROP_0, PROP_CRITICAL, PROP_SOLL };

struct _HumidityPrivate {
    gboolean critical;
    gdouble  soll;
};

#define HUMIDITY_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_HUMIDITY, HumidityPrivate))

G_DEFINE_TYPE(Humidity, humidity, G_TYPE_OBJECT)

static void humidity_init(Humidity *humidity) {
    HumidityPrivate *priv       = HUMIDITY_PRIVATE(humidity);
    humidity->priv              = priv;
    humidity->priv->critical    = TRUE;
    humidity->priv->soll        = 60.0;
    /* TODO: Add initialization code here */
}

static void humidity_finalize(GObject *object) {
    // Humidity *data = HUMIDITY(object);
    G_OBJECT_CLASS(humidity_parent_class)->finalize(object);
}

static void humidity_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_HUMIDITY(object));
    Humidity *humidity = HUMIDITY(object);
    switch (prop_id) {
    case PROP_CRITICAL:
        humidity->priv->critical = g_value_get_boolean(value);
        break;
    case PROP_SOLL:
        humidity->priv->soll = g_value_get_double(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void humidity_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_HUMIDITY(object));
    Humidity *humidity = HUMIDITY(object);
    switch (prop_id) {
    case PROP_CRITICAL:
        g_value_set_boolean(value, humidity->priv->critical);
        break;
    case PROP_SOLL:
        g_value_set_double(value, humidity->priv->soll);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void humidity_class_init(HumidityClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // Class* parent_class = MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(HumidityPrivate));
    object_class->finalize     = humidity_finalize;
    object_class->set_property = humidity_set_property;
    object_class->get_property = humidity_get_property;

    g_object_class_install_property(object_class, PROP_CRITICAL, g_param_spec_boolean("critical", "is critical", "Humidity is critical", TRUE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_SOLL, g_param_spec_double("soll", "soll value", "Humidity soll value", 0.0,100.0, 60., G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
}

/**
 * HumidityGetCritical:
 * @humidity: a #Humidity
 *
 * Get is critical
 *
 * Return: Critical error if flow out of range
 */
gboolean HumidityGetCritical(Humidity *humidity) {
    g_return_val_if_fail(humidity != NULL, FALSE);
    g_return_val_if_fail(IS_HUMIDITY(humidity), FALSE);
    return humidity->priv->critical;
}

/**
 * HumidityGetSoll:
 * @humidity: a #Humidity
 *
 * Get
 *
 * Return:
 */
gdouble HumidityGetSoll(Humidity *humidity) {
    g_return_val_if_fail(humidity != NULL, 0.0);
    g_return_val_if_fail(IS_HUMIDITY(humidity), 0.0);
    return humidity->priv->soll;
}
