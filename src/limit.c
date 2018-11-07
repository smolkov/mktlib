/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * limit.c
 * Copyright (C) LAR 2017
 *

 */

#include "limit.h"

enum {
    PROP_0,
    PROP_ACTIVATED,
    PROP_MIN,
    PROP_MAX,

};

struct _LimitPrivate {
    gboolean activated;
    gdouble  min;
    gdouble  max;
};

#define LIMIT_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_LIMIT, LimitPrivate))

G_DEFINE_TYPE(Limit, limit, G_TYPE_OBJECT)

static void limit_init(Limit *limit) {
    LimitPrivate *priv = LIMIT_PRIVATE(limit);
    priv->min          = 0.;
    priv->max          = 100.;
    priv->activated    = FALSE;
    limit->priv        = priv;

    /* TODO: Add initialization code here */
}

static void limit_finalize(GObject *object) {
    // Limit *limit = LIMIT(object);

    G_OBJECT_CLASS(limit_parent_class)->finalize(object);
}

static void limit_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_LIMIT(object));
    Limit *limit = LIMIT(object);
    switch (prop_id) {
    case PROP_ACTIVATED:
        limit->priv->activated = g_value_get_boolean(value);
        break;
    case PROP_MIN:
        limit->priv->min = g_value_get_double(value);
        break;
    case PROP_MAX:
        limit->priv->max = g_value_get_double(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void limit_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_LIMIT(object));
    Limit *limit = LIMIT(object);
    switch (prop_id) {
    case PROP_ACTIVATED:
        g_value_set_boolean(value, limit->priv->activated);
        break;
    case PROP_MIN:
        g_value_set_double(value, limit->priv->min);
        break;
    case PROP_MAX:
        g_value_set_double(value, limit->priv->max);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void limit_class_init(LimitClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // ModelClass* parent_class = MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(LimitPrivate));
    object_class->finalize     = limit_finalize;
    object_class->set_property = limit_set_property;
    object_class->get_property = limit_get_property;

    g_object_class_install_property(object_class, PROP_ACTIVATED, g_param_spec_boolean("activated", "is activated", "Set|Get activated property", FALSE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_MIN, g_param_spec_double("min", "minimum", "Set  minimal value in double property", 0., G_MAXDOUBLE, 0., G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_MAX, g_param_spec_double("max", "maximum", "maximal value", 0., G_MAXDOUBLE, 100., G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
}

/**
 * LimitGetMin:
 * @limit: a #Limit
 *
 * Get limit min value
 *
 * Return:  min limit value
 */
gdouble LimitGetMin(Limit *limit) {
    g_return_val_if_fail(limit != NULL, 0.0);
    g_return_val_if_fail(IS_LIMIT(limit), 0.0);
    return limit->priv->min;
}
/**
 * LimitGetMax:
 * @limit: a #Limit
 *
 * Get limit max value
 *
 * Return:  max limit value
 */
gdouble LimitGetMax(Limit *limit) {
    g_return_val_if_fail(limit != NULL, 0.0);
    g_return_val_if_fail(IS_LIMIT(limit), 0.0);
    return limit->priv->max;
}
/**
 * LimitGetActivated:
 * @limit: a #Limit
 *
 * Get is limit activated
 *
 * Return:  is limit activated
 */
gboolean LimitGetActivated(Limit *limit) {
    g_return_val_if_fail(limit != NULL,FALSE);
    g_return_val_if_fail(IS_LIMIT(limit), FALSE);
    return limit->priv->activated;
}

