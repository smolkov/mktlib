/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * temperatur.c
 * Copyright (C) 2017 LAR
 *
 *
 */

#include "temperatur.h"

enum { PROP_0, PROP_CHECK_FURNACE, PROP_CHECK_HOUSING, PROP_HOUSING_SOLL, PROP_FURNACE_SOLL};

struct _TemperaturPrivate {
    gboolean check_furnace;
    gboolean check_housing;
    gdouble  furnace_soll;
    gdouble  housing_soll;
};

#define TEMPERATUR_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_TEMPERATUR, TemperaturPrivate))

G_DEFINE_TYPE(Temperatur, temperatur, G_TYPE_OBJECT)

static void temperatur_init(Temperatur *temperatur) {
    TemperaturPrivate *priv         = TEMPERATUR_PRIVATE(temperatur);
    temperatur->priv                = priv;
    temperatur->priv->check_furnace = TRUE;
    temperatur->priv->check_housing = TRUE;
    temperatur->priv->furnace_soll  = 95.0;
    temperatur->priv->housing_soll  = 5.0;
    /* TODO: Add initialization code here */
}

static void temperatur_finalize(GObject *object) {
    // Temperatur *data = TEMPERATUR(object);
    G_OBJECT_CLASS(temperatur_parent_class)->finalize(object);
}

static void temperatur_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_TEMPERATUR(object));
    Temperatur *temperatur = TEMPERATUR(object);
    switch (prop_id) {
    case PROP_CHECK_FURNACE:
        temperatur->priv->check_furnace = g_value_get_boolean(value);
        break;
    case PROP_CHECK_HOUSING:
        temperatur->priv->check_housing=  g_value_get_boolean(value);
        break;
    case PROP_FURNACE_SOLL:
        temperatur->priv->furnace_soll= g_value_get_double(value);
        break;
    case PROP_HOUSING_SOLL:
        temperatur->priv->housing_soll= g_value_get_double(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void temperatur_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_TEMPERATUR(object));
    Temperatur *temperatur = TEMPERATUR(object);
    switch (prop_id) {
    case PROP_CHECK_FURNACE:
        g_value_set_boolean(value, temperatur->priv->check_furnace);
        break;
    case PROP_CHECK_HOUSING:
        g_value_set_boolean(value, temperatur->priv->check_housing);
        break;
    case PROP_FURNACE_SOLL:
        g_value_set_double(value, temperatur->priv->furnace_soll);
        break;
    case PROP_HOUSING_SOLL:
        g_value_set_double(value, temperatur->priv->housing_soll);
        break;
   default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void temperatur_class_init(TemperaturClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // Class* parent_class = MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(TemperaturPrivate));
    object_class->finalize     = temperatur_finalize;
    object_class->set_property = temperatur_set_property;
    object_class->get_property = temperatur_get_property;

    g_object_class_install_property(object_class, PROP_CHECK_FURNACE, g_param_spec_boolean("check-furnace", "on furnace watch", "On furnace watch",FALSE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_CHECK_HOUSING, g_param_spec_boolean("check-housing", "watch temperatur in device", "Watch temperatur in device",FALSE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_FURNACE_SOLL, g_param_spec_double("furnace-soll", "max temperaturn in furnace", "max temperaturn in furnace", -G_MAXDOUBLE, G_MAXDOUBLE, 95., G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_HOUSING_SOLL, g_param_spec_double("housing-soll", "min temperaturn in analyzer", "min temperaturn in analyzer", -G_MAXDOUBLE, G_MAXDOUBLE, 5., G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
}

/**
 * TemperaturIsCheckFurnace:
 * @temperatur: a #Temperatur
 *
 * Get is critical
 *
 * Return: TRUE if watch temperatur on furnace activated
 */
gboolean TemperaturIsCheckFurnace(Temperatur *temperatur) {
    g_return_val_if_fail(temperatur != NULL, FALSE);
    g_return_val_if_fail(IS_TEMPERATUR(temperatur), FALSE);
    return temperatur->priv->check_furnace;
}

/**
 * TemperaturIsCheckHousing:
 * @temperatur: a #Temperatur
 *
 * Get
 *
 * Return:
 */
gboolean TemperaturIsCheckHousing(Temperatur *temperatur) {
    g_return_val_if_fail(temperatur != NULL, FALSE);
    g_return_val_if_fail(IS_TEMPERATUR(temperatur), FALSE);
    return temperatur->priv->check_housing;
}
/**
 * TemperaturGetSollFurnace:
 * @temperatur: a #Temperatur
 *
 * Get
 *
 * Return: min termperatur
 */
gdouble TemperaturGetSollFurnace(Temperatur *temperatur) {
    g_return_val_if_fail(temperatur != NULL, 0.0);
    g_return_val_if_fail(IS_TEMPERATUR(temperatur), 0.0);
    return temperatur->priv->furnace_soll;
}
/**
 * TemperaturGetSollHousing:
 * @temperatur: a #Temperatur
 * Get
 *
 * Return: max termperatur
 */
gdouble TemperaturGetSollHousing(Temperatur *temperatur) {
    g_return_val_if_fail(temperatur != NULL, 0.0);
    g_return_val_if_fail(IS_TEMPERATUR(temperatur), 0.0);
    return temperatur->priv->housing_soll;
}