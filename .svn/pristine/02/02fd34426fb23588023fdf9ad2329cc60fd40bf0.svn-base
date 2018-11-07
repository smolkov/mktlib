/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * integrations.c
 * Copyright (C) LAR 2017
 *

 */

#include "integrations.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>

enum {
    PROP_0,
    PROP_NDIR1_TC,
    PROP_NDIR1_TIC,
    PROP_NDIR2_TC,
    PROP_NDIR2_TIC,
    PROP_TNB_TC,
    PROP_TNB_TIC,
    PROP_CODO,
};

struct _IntegrationsClass {
    GObjectClass parent_class;
};

struct _Integrations {
    GObject      parent_instance;
    Integration *ndir1Tc;
    Integration *ndir1Tic;
    Integration *ndir2Tc;
    Integration *ndir2Tic;
    Integration *tnbTc;
    Integration *tnbTic;
    Integration *codo;
};

static JsonSerializableIface *integrations_serializable_iface = NULL;

static void json_integrations_iface_init(gpointer g_iface);

static JsonNode *integrations_serialize_property(JsonSerializable *serializable, const gchar *name, const GValue *value, GParamSpec *pspec) {
    JsonNode *retval = NULL;
    retval           = integrations_serializable_iface->serialize_property(serializable, name, value, pspec);
    return retval;
}

gboolean integrations_deserialize_property(JsonSerializable *serializable, const gchar *property_name, GValue *value, GParamSpec *pspec, JsonNode *property_node) {
    // g_debug("deserialize device property %s", property_name);

    return integrations_serializable_iface->deserialize_property(serializable, property_name, value, pspec, property_node);
}

static void json_integrations_iface_init(gpointer g_iface) {
    JsonSerializableIface *iface = g_iface;

    integrations_serializable_iface = g_type_default_interface_peek(JSON_TYPE_SERIALIZABLE);
    iface->serialize_property       = integrations_serialize_property;
    iface->deserialize_property     = integrations_deserialize_property;
}

G_DEFINE_TYPE_WITH_CODE(Integrations, integrations, G_TYPE_OBJECT, G_IMPLEMENT_INTERFACE(JSON_TYPE_SERIALIZABLE, json_integrations_iface_init));

static void integrations_init(Integrations *integrations) {
    integrations->ndir1Tc = INTEGRATION(g_object_new(TYPE_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
    integrations->ndir1Tic = INTEGRATION(g_object_new(TYPE_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
    integrations->ndir2Tc = INTEGRATION(g_object_new(TYPE_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
    integrations->ndir2Tic = INTEGRATION(g_object_new(TYPE_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
    integrations->tnbTc = INTEGRATION(g_object_new(TYPE_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
    integrations->tnbTic = INTEGRATION(g_object_new(TYPE_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
    integrations->codo = INTEGRATION(g_object_new(TYPE_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
}

static void integrations_finalize(GObject *object) {
    Integrations *integrations = INTEGRATIONS(object);
    if (integrations->ndir1Tc) g_object_unref(integrations->ndir1Tc);
    if (integrations->ndir1Tic) g_object_unref(integrations->ndir1Tic);
    if (integrations->ndir2Tc) g_object_unref(integrations->ndir2Tc);
    if (integrations->ndir2Tic) g_object_unref(integrations->ndir2Tic);
    if (integrations->tnbTc) g_object_unref(integrations->tnbTc);
    if (integrations->tnbTic) g_object_unref(integrations->tnbTic);
    if (integrations->codo) g_object_unref(integrations->codo);
    G_OBJECT_CLASS(integrations_parent_class)->finalize(object);
}

static void integrations_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_INTEGRATIONS(object));
    Integrations *integrations = INTEGRATIONS(object);
    switch (prop_id) {
    case PROP_NDIR1_TC:
        if (integrations->ndir1Tc) g_object_unref(integrations->ndir1Tc);
        integrations->ndir1Tc = g_value_dup_object(value);
        break;
    case PROP_NDIR1_TIC:
        if (integrations->ndir1Tic) g_object_unref(integrations->ndir1Tic);
        integrations->ndir1Tic = g_value_dup_object(value);
        break;
    case PROP_NDIR2_TC:
        if (integrations->ndir2Tc) g_object_unref(integrations->ndir2Tc);
        integrations->ndir2Tc = g_value_dup_object(value);
        break;
    case PROP_NDIR2_TIC:
        if (integrations->ndir2Tic) g_object_unref(integrations->ndir2Tic);
        integrations->ndir2Tic = g_value_dup_object(value);
        break;
        case PROP_TNB_TC:
        if (integrations->tnbTc) g_object_unref(integrations->tnbTc);
        integrations->tnbTc = g_value_dup_object(value);
        break;
        case PROP_TNB_TIC:
        if (integrations->tnbTic) g_object_unref(integrations->tnbTic);
        integrations->tnbTic = g_value_dup_object(value);
        break;
        case PROP_CODO:
        if (integrations->codo) g_object_unref(integrations->codo);
        integrations->codo = g_value_dup_object(value);
        break;
   
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void integrations_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_INTEGRATIONS(object));
    Integrations *integrations = INTEGRATIONS(object);
    switch (prop_id) {
    case PROP_NDIR1_TC:
        g_value_set_object(value, integrations->ndir1Tc);
        break;
    case PROP_NDIR1_TIC:
        g_value_set_object(value, integrations->ndir1Tic);
        break;
    case PROP_NDIR2_TC:
        g_value_set_object(value, integrations->ndir2Tc);
        break;
    case PROP_NDIR2_TIC:
        g_value_set_object(value, integrations->ndir2Tic);
        break;
    case PROP_TNB_TC:
        g_value_set_object(value, integrations->tnbTc);
        break;
    case PROP_TNB_TIC:
        g_value_set_object(value, integrations->tnbTic);
        break;
    case PROP_CODO:
        g_value_set_object(value, integrations->codo);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void integrations_class_init(IntegrationsClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // Class* parent_class = MODEL_CLASS (klass);
    object_class->finalize     = integrations_finalize;
    object_class->set_property = integrations_set_property;
    object_class->get_property = integrations_get_property;

    g_object_class_install_property(
        object_class, PROP_NDIR1_TC, g_param_spec_object("ndir1Tc", "NDIR 1 TC", "NDIR1 TC integration", TYPE_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_NDIR1_TIC, g_param_spec_object("ndir1Tic", "NDIR 1 TIC", "NDIR1 TIC integration", TYPE_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_NDIR2_TC, g_param_spec_object("ndir2Tc", "NDIR 2 TC", "NDIR2 TC integration", TYPE_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_NDIR2_TIC, g_param_spec_object("ndir2Tic", "NDIR 2 TIC", "NDIR2 TIC integration", TYPE_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_TNB_TC, g_param_spec_object("tnbTc", "TNb TC", "TNb TC integration", TYPE_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_TNB_TIC, g_param_spec_object("tnbTic", "TNb TIC", "TNb TIC integration", TYPE_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_CODO, g_param_spec_object("codo", "CODo TC", "CODo TC integration", TYPE_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
}

Integration *IntegrationsGetNDir1TC(Integrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_INTEGRATIONS(integrations),NULL);
    return integrations->ndir1Tc;
}
Integration *IntegrationsGetNDir1TIC(Integrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_INTEGRATIONS(integrations),NULL);
    return integrations->ndir1Tic;
}
Integration *IntegrationsGetNDir2TC(Integrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_INTEGRATIONS(integrations),NULL);
    return integrations->ndir2Tc;
}
Integration *IntegrationsGetNDir2TIC(Integrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_INTEGRATIONS(integrations),NULL);
    return integrations->ndir2Tic;
}
Integration *IntegrationsGetTNbTC(Integrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_INTEGRATIONS(integrations),NULL);
    return integrations->tnbTc;
}
Integration *IntegrationsGetTNbTIC(Integrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_INTEGRATIONS(integrations),NULL);
    return integrations->tnbTic;
}
Integration *IntegrationsGetCODo(Integrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_INTEGRATIONS(integrations),NULL);
    return integrations->codo;
}

