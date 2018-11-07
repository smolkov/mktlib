/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * ultrameas.c
 * Copyright (C) LAR 2017
 *

 */

// #include "larsqlite.h"
#include "ultrameas.h"
// #include "value.h"

enum
{
    PROP_0,
    PROP_CHANGED,
    PROP_VALUE,
    PROP_OUTLIER,
    PROP_LAR_INTEGRATION,
};

struct _UltrameasPrivate
{
    gint64 changed;
    gdouble value;
    gboolean outlier;
    LarIntgrec *integration;
};

#define ULTRAMEAS_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_ULTRAMEAS, UltrameasPrivate))

G_DEFINE_TYPE(Ultrameas, ultrameas, G_TYPE_OBJECT)

static void ultrameas_init(Ultrameas *ultrameas)
{
    UltrameasPrivate *priv = ULTRAMEAS_PRIVATE(ultrameas);
    GDateTime *dt = g_date_time_new_now_utc();
    priv->changed = g_date_time_to_unix(dt);
    g_date_time_unref(dt);
    priv->value = 0.0;
    priv->outlier = FALSE;
    priv->integration = NULL;
    ultrameas->priv = priv;
}

static void ultrameas_finalize(GObject *object)
{

    Ultrameas *umeas = ULTRAMEAS(object);
    if (umeas->priv->integration)
        g_object_unref(umeas->priv->integration);
    G_OBJECT_CLASS(ultrameas_parent_class)->finalize(object);
}

static void ultrameas_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_ULTRAMEAS(object));
    Ultrameas *umeas = ULTRAMEAS(object);
    switch (prop_id)
    {
    case PROP_CHANGED:
        umeas->priv->changed = g_value_get_int64(value);
        break;
    case PROP_VALUE:
        umeas->priv->value = g_value_get_double(value);
        break;
    case PROP_OUTLIER:
        umeas->priv->outlier = g_value_get_boolean(value);
        break;
    case PROP_LAR_INTEGRATION:
        if (umeas->priv->integration)
            g_object_unref(umeas->priv->integration);
        umeas->priv->integration = g_value_dup_object(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void ultrameas_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_ULTRAMEAS(object));
    Ultrameas *umeas = ULTRAMEAS(object);
    switch (prop_id)
    {
    case PROP_CHANGED:
        g_value_set_int64(value, umeas->priv->changed);
        break;
    case PROP_VALUE:
        g_value_set_double(value, umeas->priv->value);
        break;
    case PROP_OUTLIER:
        g_value_set_boolean(value, umeas->priv->outlier);
        break;
    case PROP_LAR_INTEGRATION:
        g_value_set_object(value, umeas->priv->integration);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void ultrameas_class_init(UltrameasClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // ModelClass* parent_class = MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(UltrameasPrivate));
    object_class->finalize = ultrameas_finalize;
    object_class->set_property = ultrameas_set_property;
    object_class->get_property = ultrameas_get_property;
    g_object_class_install_property(
        object_class, PROP_CHANGED, g_param_spec_int64("changed", "Create measurement time", "Set get create measurement time", 0, G_MAXINT64, 0, G_PARAM_READWRITE));
    g_object_class_install_property(
        object_class, PROP_VALUE, g_param_spec_double("value", "LarMeasurement measurement property", "Set get sensor measurement property", 0., G_MAXDOUBLE, 0., G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(
        object_class, PROP_OUTLIER, g_param_spec_boolean("outlier", "LarMeasurement measurement outlier", "Set get sensor outlier property", FALSE, G_PARAM_READABLE | G_PARAM_WRITABLE));

    g_object_class_install_property(object_class, PROP_LAR_INTEGRATION, g_param_spec_object("integration", "Ultra integration object", "LarIntegration object", TYPE_LAR_INTGREC, G_PARAM_READWRITE));
}

Ultrameas *UltrameasNew(LarIntgrec *rec, gdouble offset, gdouble slope, gdouble factor)
{
    m_LarIntgrecCalculateZero(rec);
    if(slope == 0.00)slope = 1.0;
    gdouble value = (m_LarIntgrecCalculateIntegration(rec) - offset) / slope;
    if (factor == 0.0)
        factor = 1.0;
    
    if(value<0.0)value=0.0;
    Ultrameas *meas = ULTRAMEAS(g_object_new(TYPE_ULTRAMEAS, "integration", rec, "value", value * factor, NULL));
    return meas;
}

Ultrameas *UltrameasNewFromValue(gdouble value, gdouble factor)
{
    if (factor == 0.0)
        factor = 1.0;
    Ultrameas *meas = ULTRAMEAS(g_object_new(TYPE_ULTRAMEAS, "value", value * factor, NULL));
    return meas;
}

/**
 * m_UltrameasGetChanged:
 * @ultrameas: a #Ultrameas
 *
 * result in microseconds 
 *
 * Return:  create time in micro seconds .
 */
gint64 m_UltrameasGetCreated(Ultrameas *ultrameas)
{
    g_return_val_if_fail(ultrameas != NULL, 0);
    g_return_val_if_fail(IS_ULTRAMEAS(ultrameas), 0);
    return ultrameas->priv->changed;
}

/**
 * m_UltrameasGetOutlier:
 * @ultrameas: a #Ultrameas
 *
 * is outlier
 *
 * Return:  TRUE is outlier.
 */
gboolean m_UltrameasIsOutlier(Ultrameas *ultrameas)
{
    g_return_val_if_fail(ultrameas != NULL, FALSE);
    g_return_val_if_fail(IS_ULTRAMEAS(ultrameas), FALSE);
    return ultrameas->priv->outlier;
}
/**
 * m_UltrameasSetOutlier:
 * @ultrameas: a #Ultrameas
 * @isOutlier: gboolean
 * is outlier
 *
 * Return:  set outlier flag.
 */
void m_UltrameasSetOutlier(Ultrameas *ultrameas, gboolean isOutlier)
{
    g_return_if_fail(ultrameas != NULL);
    g_return_if_fail(IS_ULTRAMEAS(ultrameas));
    ultrameas->priv->outlier = isOutlier;
}

/**
 * m_UltrameasGetLarIntegration:
 * @ultrameas: a #Ultrameas
 *
 * result integrations umeas
 *
 * Return:  integrationsumeas
 */
gdouble m_UltrameasGetValue(Ultrameas *ultrameas)
{
    g_return_val_if_fail(ultrameas != NULL, 0.0);
    g_return_val_if_fail(IS_ULTRAMEAS(ultrameas), 0.0);
    return ultrameas->priv->value;
}
/**
 * m_UltrameasGetLarIntegration:
 * @ultrameas: a #Ultrameas
 *
 * result integrations umeas
 *
 * Return:  integrationsumeas
 */
LarIntgrec *m_UltrameasGetLarIntegration(Ultrameas *ultrameas)
{
    g_return_val_if_fail(ultrameas != NULL, 0);
    g_return_val_if_fail(IS_ULTRAMEAS(ultrameas), 0);
    return ultrameas->priv->integration;
}
