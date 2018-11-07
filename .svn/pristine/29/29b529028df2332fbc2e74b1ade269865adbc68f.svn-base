/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * airflow.c
 * Copyright (C) 2017 LAR
 *
 *
 */

#include "airflow.h"

enum { PROP_0, PROP_CRITICAL, PROP_SOLL, PROP_CORRELATION, PROP_DEVIATION, PROP_ADJUSTMENT, PROP_TIMEOUT, PROP_THRESHOLD };

struct _AirflowPrivate {
    gboolean critical;
    gdouble  soll;
    gdouble  correlation;
    gdouble  deviation;
    gdouble  adjustment;
    gdouble  timeout;
    gdouble  threshold;
};

#define AIRFLOW_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_AIRFLOW, AirflowPrivate))

G_DEFINE_TYPE(Airflow, airflow, G_TYPE_OBJECT)

static void airflow_init(Airflow *airflow) {
    AirflowPrivate *priv       = AIRFLOW_PRIVATE(airflow);
    airflow->priv              = priv;
    airflow->priv->critical    = TRUE;
    airflow->priv->soll        = 30.0;
    airflow->priv->correlation = 10.0;
    airflow->priv->deviation   = 5.0;
    airflow->priv->adjustment  = 0.0;
    airflow->priv->timeout     = 2.0;
    airflow->priv->threshold   = 70.0;
    /* TODO: Add initialization code here */
}

static void airflow_finalize(GObject *object) {
    // Airflow *data = AIRFLOW(object);
    G_OBJECT_CLASS(airflow_parent_class)->finalize(object);
}

static void airflow_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_AIRFLOW(object));
    Airflow *airflow = AIRFLOW(object);
    switch (prop_id) {
    case PROP_CRITICAL:
        airflow->priv->critical = g_value_get_boolean(value);
        break;
    case PROP_SOLL:
        airflow->priv->soll = g_value_get_double(value);
        break;
    case PROP_CORRELATION:
        airflow->priv->correlation = g_value_get_double(value);
        break;
    case PROP_DEVIATION:
        airflow->priv->deviation = g_value_get_double(value);
        break;
    case PROP_ADJUSTMENT:
        airflow->priv->adjustment = g_value_get_double(value);
        break;
    case PROP_TIMEOUT:
        airflow->priv->timeout = g_value_get_double(value);
        break;
    case PROP_THRESHOLD:
        airflow->priv->threshold = g_value_get_double(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void airflow_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_AIRFLOW(object));
    Airflow *airflow = AIRFLOW(object);
    switch (prop_id) {
    case PROP_CRITICAL:
        g_value_set_boolean(value, airflow->priv->critical);
        break;
    case PROP_SOLL:
        g_value_set_double(value, airflow->priv->soll);
        break;
    case PROP_CORRELATION:
        g_value_set_double(value, airflow->priv->correlation);
        break;
    case PROP_DEVIATION:
        g_value_set_double(value, airflow->priv->deviation);
        break;
    case PROP_ADJUSTMENT:
        g_value_set_double(value, airflow->priv->adjustment);
        break;
    case PROP_TIMEOUT:
        g_value_set_double(value, airflow->priv->timeout);
        break;
    case PROP_THRESHOLD:
        g_value_set_double(value, airflow->priv->threshold);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void airflow_class_init(AirflowClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // Class* parent_class = MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(AirflowPrivate));
    object_class->finalize     = airflow_finalize;
    object_class->set_property = airflow_set_property;
    object_class->get_property = airflow_get_property;

    g_object_class_install_property(object_class, PROP_CRITICAL, g_param_spec_boolean("critical", "is critical", "Airflow is critical", TRUE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_SOLL, g_param_spec_double("soll", "soll value", "Airflow soll value", 0.0, G_MAXDOUBLE, 30., G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_CORRELATION, g_param_spec_double("correlation", "correlation", "Airflow correlation", -G_MAXDOUBLE, G_MAXDOUBLE, 10., G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_DEVIATION, g_param_spec_double("deviation", "deviation", "Airflow deviation", -G_MAXDOUBLE, G_MAXDOUBLE, 5., G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_ADJUSTMENT, g_param_spec_double("adjustment", "adjustment", "Airflow deviation", -G_MAXDOUBLE, G_MAXDOUBLE, 0., G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_TIMEOUT, g_param_spec_double("timeout", "timeout", "Injection analyze time", -G_MAXDOUBLE, G_MAXDOUBLE, 2., G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_THRESHOLD, g_param_spec_double("threshold", "threshold", "Injection analyze threshold", -G_MAXDOUBLE, G_MAXDOUBLE,70., G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
}

/**
 * AirflowGetCritical:
 * @airflow: a #Airflow
 *
 * Get is critical
 *
 * Return: Critical error if flow out of range
 */
gboolean AirflowGetCritical(Airflow *airflow) {
    g_return_val_if_fail(airflow != NULL, FALSE);
    g_return_val_if_fail(IS_AIRFLOW(airflow), FALSE);
    return airflow->priv->critical;
}

/**
 * AirflowGetSoll:
 * @airflow: a #Airflow
 *
 * Get
 *
 * Return:
 */
gdouble AirflowGetSoll(Airflow *airflow) {
    g_return_val_if_fail(airflow != NULL, 0.0);
    g_return_val_if_fail(IS_AIRFLOW(airflow), 0.0);
    return airflow->priv->soll;
}
/**
 * AirflowGetCorrelation:
 * @airflow: a #Airflow
 *
 * Get
 *
 * Return: correlation
 */
gdouble AirflowGetMaxCorrelation(Airflow *airflow) {
    g_return_val_if_fail(airflow != NULL, 0.0);
    g_return_val_if_fail(IS_AIRFLOW(airflow), 0.0);
    return airflow->priv->correlation;
}
/**
 * AirflowGetDeviation:
 * @airflow: a #Airflow
 *
 * Get deviation
 *
 * Return: deviation value
 */
gdouble AirflowGetDeviation(Airflow *airflow) {
    g_return_val_if_fail(airflow != NULL, 0.0);
    g_return_val_if_fail(IS_AIRFLOW(airflow), 0.0);
    return airflow->priv->deviation;
}
/**
 * AirflowGetAdjustment:
 * @airflow: a #Airflow
 *
 * Get adjustement
 *
 * Return:
 */
gdouble AirflowGetAdjustment(Airflow *airflow) {
    g_return_val_if_fail(airflow != NULL, 0.0);
    g_return_val_if_fail(IS_AIRFLOW(airflow), 0.0);
    return airflow->priv->adjustment;
}
/**
 * AirflowGetTimeout:
 * @airflow: a #Airflow
 *
 * Get timeout
 *
 * Return: timeout
 */
gdouble AirflowGetTimeout(Airflow *airflow) {
    g_return_val_if_fail(airflow != NULL, 0.0);
    g_return_val_if_fail(IS_AIRFLOW(airflow), 0.0);
    return airflow->priv->timeout;
}
/**
 * AirflowGetThreshold:
 * @airflow: a #Airflow
 *
 * Get threshold
 *
 * Return: threshold
 */
gdouble AirflowGetThreshold(Airflow *airflow) {
    g_return_val_if_fail(airflow != NULL, 0.0);
    g_return_val_if_fail(IS_AIRFLOW(airflow), 0.0);
    return airflow->priv->threshold;
}
