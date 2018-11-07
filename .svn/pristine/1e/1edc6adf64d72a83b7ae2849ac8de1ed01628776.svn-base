/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * analog.c
 * Copyright (C) 2017 LAR
 *
 *
 */

#include "analog.h"

enum { PROP_0, PROP_MIN, PROP_MAX, PROP_LIFE_ZERO, PROP_BOOLEXPR };

struct _AnalogPrivate {
    gdouble  min;
    gdouble  max;
    gboolean lifeZero;
    gchar *  boolexpr;
};

#define ANALOG_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_ANALOG, AnalogPrivate))


G_DEFINE_TYPE(Analog, analog, G_TYPE_OBJECT)

static void analog_init(Analog *analog) {
    AnalogPrivate *priv    = ANALOG_PRIVATE(analog);
    priv->boolexpr			= g_strdup("CRIT");

    analog->priv			= priv;
    /* TODO: Add initialization code here */
}

static void analog_finalize(GObject *object) {
    Analog *data = ANALOG(object);
    if (data->priv->boolexpr) g_free(data->priv->boolexpr);
    G_OBJECT_CLASS(analog_parent_class)->finalize(object);
}

static void analog_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_ANALOG(object));
    Analog *analog = ANALOG(object);
    switch (prop_id) {
    case PROP_MIN:
        analog->priv->min = g_value_get_double(value);
        break;
    case PROP_MAX:
        analog->priv->max = g_value_get_double(value);
        break;
    case PROP_LIFE_ZERO:
        analog->priv->lifeZero = g_value_get_boolean(value);
        break;
    case PROP_BOOLEXPR:
        if (analog->priv->boolexpr) g_free(analog->priv->boolexpr);
        analog->priv->boolexpr = g_value_dup_string(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void analog_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_ANALOG(object));
    Analog *stream = ANALOG(object);
    switch (prop_id) {
    case PROP_MIN:
        g_value_set_double(value, stream->priv->min);
        break;
    case PROP_MAX:
        g_value_set_double(value, stream->priv->max);
        break;
    case PROP_LIFE_ZERO:
        g_value_set_boolean(value, stream->priv->lifeZero);
        break;
    case PROP_BOOLEXPR:
        g_value_set_string(value, stream->priv->boolexpr);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void analog_class_init(AnalogClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // Class* parent_class = MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(AnalogPrivate));
    object_class->finalize     = analog_finalize;
    object_class->set_property = analog_set_property;
    object_class->get_property = analog_get_property;

    g_object_class_install_property(object_class, PROP_MIN, g_param_spec_double("min", "Min", "min scale limit", -G_MAXDOUBLE, G_MAXDOUBLE, 0., G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_MAX, g_param_spec_double("max", "max", "max scale limit", -G_MAXDOUBLE, G_MAXDOUBLE, 20000., G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_BOOLEXPR, g_param_spec_string("boolexpr", "life zero string", "Life zero bool expressions", "", G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_LIFE_ZERO, g_param_spec_boolean("lifezero", "is life zero", "Life zero", FALSE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
}


/**
 * AnalogGetMin:
 * @analog: a #Analog
 *
 * Get min limit
 *
 * Return:  limit
 */
gdouble AnalogGetMin(Analog *analog) {
    g_return_val_if_fail(analog != NULL, 0.0);
    g_return_val_if_fail(IS_ANALOG(analog), 0.0);
    return analog->priv->min;
}
/**
 * AnalogGetMax:
 * @analog: a #Analog
 *
 * Get max limit
 *
 * Return: limit
 */
gdouble AnalogGetMax(Analog *analog) {
    g_return_val_if_fail(analog != NULL, 20000.0);
    g_return_val_if_fail(IS_ANALOG(analog), 20000.0);
    return analog->priv->max;
}
/**
 * AnalogGetBoolexpr:
 * @analog: a #Analog
 *
 * Get bool expression
 *
 * Return: bool expression
 */
const gchar* AnalogGetBoolexpr(Analog *analog) {
    g_return_val_if_fail(analog != NULL, NULL);
    g_return_val_if_fail(IS_ANALOG(analog), NULL);
    return analog->priv->boolexpr;
}

/**
 * AnalogGetLifeZero:
 * @analog: a #Analog
 *
 * Get life zero
 *
 * Return: TRUE if life zero is activ
 */
gboolean AnalogGetLifeZero(Analog *analog) {
    g_return_val_if_fail(analog != NULL, FALSE);
    g_return_val_if_fail(IS_ANALOG(analog),FALSE);
    return analog->priv->lifeZero;
}
