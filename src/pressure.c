/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * pressure.c
 * Copyright (C) 2017 LAR
 *
 *
 */

#include "pressure.h"

enum { PROP_0, PROP_CRITICAL, PROP_SOLL };

struct _PressurePrivate {
    gboolean critical;
    gdouble  soll;
};

#define PRESSURE_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_PRESSURE, PressurePrivate))

G_DEFINE_TYPE(Pressure, pressure, G_TYPE_OBJECT)

static void pressure_init(Pressure *pressure) {
    PressurePrivate *priv       = PRESSURE_PRIVATE(pressure);
    pressure->priv              = priv;
    pressure->priv->critical    = TRUE;
    pressure->priv->soll        = 600.0;
    /* TODO: Add initialization code here */
}

static void pressure_finalize(GObject *object) {
    // Pressure *data = PRESSURE(object);
    G_OBJECT_CLASS(pressure_parent_class)->finalize(object);
}

static void pressure_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_PRESSURE(object));
    Pressure *pressure = PRESSURE(object);
    switch (prop_id) {
    case PROP_CRITICAL:
        pressure->priv->critical = g_value_get_boolean(value);
        break;
    case PROP_SOLL:
        pressure->priv->soll = g_value_get_double(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void pressure_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_PRESSURE(object));
    Pressure *pressure = PRESSURE(object);
    switch (prop_id) {
    case PROP_CRITICAL:
        g_value_set_boolean(value, pressure->priv->critical);
        break;
    case PROP_SOLL:
        g_value_set_double(value, pressure->priv->soll);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void pressure_class_init(PressureClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // Class* parent_class = MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(PressurePrivate));
    object_class->finalize     = pressure_finalize;
    object_class->set_property = pressure_set_property;
    object_class->get_property = pressure_get_property;

    g_object_class_install_property(object_class, PROP_CRITICAL, g_param_spec_boolean("critical", "is critical", "Pressure is critical", TRUE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_SOLL, g_param_spec_double("soll", "soll value", "Pressure soll value", 0.0,1000.0, 600., G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
}

/**
 * PressureGetCritical:
 * @pressure: a #Pressure
 *
 * Get is critical
 *
 * Return: Critical error if flow out of range
 */
gboolean PressureGetCritical(Pressure *pressure) {
    g_return_val_if_fail(pressure != NULL, FALSE);
    g_return_val_if_fail(IS_PRESSURE(pressure), FALSE);
    return pressure->priv->critical;
}

/**
 * PressureGetSoll:
 * @pressure: a #Pressure
 *
 * Get
 *
 * Return:
 */
gdouble PressureGetSoll(Pressure *pressure) {
    g_return_val_if_fail(pressure != NULL, 0.0);
    g_return_val_if_fail(IS_PRESSURE(pressure), 0.0);
    return pressure->priv->soll;
}
