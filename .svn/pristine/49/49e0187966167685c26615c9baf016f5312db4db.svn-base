/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * integration.c
 * Copyright (C) LAR 2017
 *
 */

#include "integration.h"

enum {
    PROP_0,
    PROP_JUSTIFACATION_TIME,
    PROP_START_THRESHOLD,
    PROP_STOP_THRESHOLD,
    PROP_T_START_MIN,
    PROP_T_STOP_MAX,
    PROP_T_STOP_MIN,
};

struct _IntegrationClass {
    GObjectClass parent_class;
};

struct _Integration {
    GObject  parent_instance;
    gboolean integration_runs;
    gdouble  justification_time;
    gdouble  start_threshold;
    gdouble  stop_threshold;
    gdouble  t_start_min;
    gdouble  t_stop_max;
    gdouble  t_stop_min;
};


G_DEFINE_TYPE(Integration, integration, G_TYPE_OBJECT)

static void integration_init(Integration *integration) {
    integration->justification_time = 20.0;
    integration->integration_runs   = FALSE;
    integration->start_threshold    = 0.002;
    integration->stop_threshold     = 0.003;
    integration->t_start_min        = 0.0;
    integration->t_stop_max         = 120.;
    integration->t_stop_min         = 80.;
  

    /* TODO: Add initialization code here */
}

static void integration_finalize(GObject *object) {
    // Integration *data = INTEGRATION(object);
    G_OBJECT_CLASS(integration_parent_class)->finalize(object);
}

static void integration_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_INTEGRATION(object));
    Integration *integration = INTEGRATION(object);
    switch (prop_id) {
    case PROP_JUSTIFACATION_TIME:
        integration->justification_time = g_value_get_double(value);
        break;
    case PROP_START_THRESHOLD:
        integration->start_threshold = g_value_get_double(value);
        break;
    case PROP_STOP_THRESHOLD:
        integration->stop_threshold = g_value_get_double(value);
        break;
    case PROP_T_START_MIN:
        integration->t_start_min = g_value_get_double(value);
        break;
    case PROP_T_STOP_MAX:
        integration->t_stop_max = g_value_get_double(value);
        break;
    case PROP_T_STOP_MIN:
        integration->t_stop_min = g_value_get_double(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void integration_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_INTEGRATION(object));
    Integration *integration = INTEGRATION(object);
    switch (prop_id) {

    case PROP_JUSTIFACATION_TIME:
        g_value_set_double(value, integration->justification_time);
        break;
    case PROP_START_THRESHOLD:
        g_value_set_double(value, integration->start_threshold);
        break;

    case PROP_STOP_THRESHOLD:
        g_value_set_double(value, integration->stop_threshold);
        break;
    case PROP_T_START_MIN:
        g_value_set_double(value, integration->t_start_min);
        break;
    case PROP_T_STOP_MAX:
        g_value_set_double(value, integration->t_stop_max);
        break;
    case PROP_T_STOP_MIN:
        g_value_set_double(value, integration->t_stop_min);
        break;
    default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void integration_class_init(IntegrationClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->finalize     = integration_finalize;
    object_class->set_property = integration_set_property;
    object_class->get_property = integration_get_property;

    g_object_class_install_property(object_class, PROP_JUSTIFACATION_TIME,
        g_param_spec_double("justification", "Sensor justification time", "Set|Get sensor justification time", 10.0, 100.0, 20.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_START_THRESHOLD,
        g_param_spec_double("startThreshold", "start threshold", "Set|Get integration start threshold", -1.0, 1.0, 0.002, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_STOP_THRESHOLD,
        g_param_spec_double("stopThreshold", "stop threshold", "Set|Get integration stop threshold", -1.0, 1.0, 0.003, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_T_START_MIN, g_param_spec_double("startMin", "start min time", "Set|Get integration start min", 0.0, 500.0, 10.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_T_STOP_MAX, g_param_spec_double("stopMax", "stop max time", "Set|Get integration stop max", 10., 500.0, 120.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_T_STOP_MIN, g_param_spec_double("stopMin", "stop min time", "Set|Get integration stop min", 0.0, 500.0, 60.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
}

gdouble IntegrationJustificationTime(Integration *integration) {
    g_return_val_if_fail(integration != NULL, 0.);
    g_return_val_if_fail(IS_INTEGRATION(integration), 0.);
    return INTEGRATION(integration)->justification_time;
}
gdouble IntegrationStartThreshold(Integration *integration) {
    g_return_val_if_fail(integration != NULL, 0.);
    g_return_val_if_fail(IS_INTEGRATION(integration), 0.);
    return INTEGRATION(integration)->start_threshold;
}

gdouble IntegrationStopThreshold(Integration *integration) {
    g_return_val_if_fail(integration != NULL, 0.);
    g_return_val_if_fail(IS_INTEGRATION(integration), 0.);
    return INTEGRATION(integration)->stop_threshold;
}

gdouble IntegrationStartMin(Integration *integration) {
    g_return_val_if_fail(integration != NULL, 0.);
    g_return_val_if_fail(IS_INTEGRATION(integration), 0.);
    return INTEGRATION(integration)->t_start_min;
}

gdouble integrationStopMax(Integration *integration) {
    g_return_val_if_fail(integration != NULL, 0.);
    g_return_val_if_fail(IS_INTEGRATION(integration), 0.);
    return INTEGRATION(integration)->t_stop_max;
}

gdouble integrationStopMin(Integration *integration) {
    g_return_val_if_fail(integration != NULL, 0.);
    g_return_val_if_fail(IS_INTEGRATION(integration), 0.);
    return INTEGRATION(integration)->t_stop_min;
}

/** @} */
