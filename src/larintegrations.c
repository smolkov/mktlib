/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * larintegrations.c
 * Copyright (C) LAR 2017
 *

 */

#include "larintegrations.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>

enum {
    PROP_0,
    PROP_NDIR1_TC1,
    PROP_NDIR1_TC2,
    PROP_NDIR1_TC3,
    PROP_NDIR1_TIC1,
    PROP_NDIR1_TIC2,
    PROP_NDIR1_TIC3,
    PROP_NDIR2_TC1,
    PROP_NDIR2_TC2,
    PROP_NDIR2_TC3,
    PROP_NDIR2_TIC1,
    PROP_NDIR2_TIC2,
    PROP_NDIR2_TIC3,
    PROP_TNB_TC,
    PROP_TNB_TIC,
    PROP_CODO,
};

struct _LarIntegrationsClass {
    GObjectClass parent_class;
};

struct _LarIntegrations {
    GObject      parent_instance;
    LarIntegration *ndir1Tc1;
    LarIntegration *ndir1Tc2;
    LarIntegration *ndir1Tc3;
    LarIntegration *ndir1Tic1;
    LarIntegration *ndir1Tic2;
    LarIntegration *ndir1Tic3;
    LarIntegration *ndir2Tc1;
    LarIntegration *ndir2Tc2;
    LarIntegration *ndir2Tc3;
    LarIntegration *ndir2Tic1;
    LarIntegration *ndir2Tic2;
    LarIntegration *ndir2Tic3;
    LarIntegration *tnbTc;
    LarIntegration *tnbTic;
    LarIntegration *codo;
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

G_DEFINE_TYPE_WITH_CODE(LarIntegrations, integrations, G_TYPE_OBJECT, G_IMPLEMENT_INTERFACE(JSON_TYPE_SERIALIZABLE, json_integrations_iface_init));

static void integrations_init(LarIntegrations *integrations) {
    integrations->ndir1Tc1 = LAR_INTEGRATION(g_object_new(TYPE_LAR_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
    integrations->ndir1Tc2 = LAR_INTEGRATION(g_object_new(TYPE_LAR_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
    integrations->ndir1Tc3 = LAR_INTEGRATION(g_object_new(TYPE_LAR_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
    integrations->ndir1Tic1 = LAR_INTEGRATION(g_object_new(TYPE_LAR_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
    integrations->ndir1Tic2 = LAR_INTEGRATION(g_object_new(TYPE_LAR_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
    integrations->ndir1Tic3 = LAR_INTEGRATION(g_object_new(TYPE_LAR_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
    integrations->ndir2Tc1 = LAR_INTEGRATION(g_object_new(TYPE_LAR_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
    integrations->ndir2Tc2 = LAR_INTEGRATION(g_object_new(TYPE_LAR_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
    integrations->ndir2Tc3 = LAR_INTEGRATION(g_object_new(TYPE_LAR_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
    integrations->ndir2Tic1 = LAR_INTEGRATION(g_object_new(TYPE_LAR_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
    integrations->ndir2Tic2 = LAR_INTEGRATION(g_object_new(TYPE_LAR_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
    integrations->ndir2Tic3 = LAR_INTEGRATION(g_object_new(TYPE_LAR_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
    integrations->tnbTc = LAR_INTEGRATION(g_object_new(TYPE_LAR_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
    integrations->tnbTic = LAR_INTEGRATION(g_object_new(TYPE_LAR_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
    integrations->codo = LAR_INTEGRATION(g_object_new(TYPE_LAR_INTEGRATION, "startThreshold", 0.002, "stopThreshold", 0.003, "startMin", 10.0, "stopMin", 80.0, "stopMax", 120.0, NULL));
}

static void integrations_finalize(GObject *object) {
    LarIntegrations *integrations = LAR_INTEGRATIONS(object);
    if (integrations->ndir1Tc1) g_object_unref(integrations->ndir1Tc1);
    if (integrations->ndir1Tc2) g_object_unref(integrations->ndir1Tc2);
    if (integrations->ndir1Tc3) g_object_unref(integrations->ndir1Tc3);
    if (integrations->ndir1Tic1) g_object_unref(integrations->ndir1Tic1);
    if (integrations->ndir1Tic2) g_object_unref(integrations->ndir1Tic2);
    if (integrations->ndir1Tic3) g_object_unref(integrations->ndir1Tic3);
    if (integrations->ndir2Tc1) g_object_unref(integrations->ndir2Tc1);
    if (integrations->ndir2Tc2) g_object_unref(integrations->ndir2Tc2);
    if (integrations->ndir2Tc3) g_object_unref(integrations->ndir2Tc3);
    if (integrations->ndir2Tic1) g_object_unref(integrations->ndir2Tic1);
    if (integrations->ndir2Tic2) g_object_unref(integrations->ndir2Tic2);
    if (integrations->ndir2Tic3) g_object_unref(integrations->ndir2Tic3);
    if (integrations->tnbTc) g_object_unref(integrations->tnbTc);
    if (integrations->tnbTic) g_object_unref(integrations->tnbTic);
    if (integrations->codo) g_object_unref(integrations->codo);
    G_OBJECT_CLASS(integrations_parent_class)->finalize(object);
}

static void integrations_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_LAR_INTEGRATIONS(object));
    LarIntegrations *integrations = LAR_INTEGRATIONS(object);
    switch (prop_id) {
    case PROP_NDIR1_TC1:
        if (integrations->ndir1Tc1) g_object_unref(integrations->ndir1Tc1);
        integrations->ndir1Tc1= g_value_dup_object(value);
        break;
      case PROP_NDIR1_TC2:
        if (integrations->ndir1Tc2) g_object_unref(integrations->ndir1Tc2);
        integrations->ndir1Tc2 = g_value_dup_object(value);
        break;
   case PROP_NDIR1_TC3:
        if (integrations->ndir1Tc3) g_object_unref(integrations->ndir1Tc3);
        integrations->ndir1Tc3 = g_value_dup_object(value);
        break;
   case PROP_NDIR1_TIC1:
        if (integrations->ndir1Tic1) g_object_unref(integrations->ndir1Tic1);
        integrations->ndir1Tic1 = g_value_dup_object(value);
        break;
   case PROP_NDIR1_TIC2:
        if (integrations->ndir1Tic2) g_object_unref(integrations->ndir1Tic2);
        integrations->ndir1Tic2= g_value_dup_object(value);
        break;
  case PROP_NDIR1_TIC3:
        if (integrations->ndir1Tic3) g_object_unref(integrations->ndir1Tic3);
        integrations->ndir1Tic3= g_value_dup_object(value);
        break;
   case PROP_NDIR2_TC1:
        if (integrations->ndir2Tc1) g_object_unref(integrations->ndir2Tc1);
        integrations->ndir2Tc1= g_value_dup_object(value);
        break;
   case PROP_NDIR2_TC2:
        if (integrations->ndir2Tc2) g_object_unref(integrations->ndir2Tc2);
        integrations->ndir2Tc2= g_value_dup_object(value);
        break;
   case PROP_NDIR2_TC3:
        if (integrations->ndir2Tc3) g_object_unref(integrations->ndir2Tc3);
        integrations->ndir2Tc3= g_value_dup_object(value);
        break;
    case PROP_NDIR2_TIC1:
        if (integrations->ndir2Tic1) g_object_unref(integrations->ndir2Tic1);
        integrations->ndir2Tic1= g_value_dup_object(value);
        break;
    case PROP_NDIR2_TIC2:
        if (integrations->ndir2Tic2) g_object_unref(integrations->ndir2Tic2);
        integrations->ndir2Tic2= g_value_dup_object(value);
        break;
    case PROP_NDIR2_TIC3:
        if (integrations->ndir2Tic3) g_object_unref(integrations->ndir2Tic3);
        integrations->ndir2Tic3= g_value_dup_object(value);
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
    g_return_if_fail(IS_LAR_INTEGRATIONS(object));
    LarIntegrations *integrations = LAR_INTEGRATIONS(object);
    switch (prop_id) {
    case PROP_NDIR1_TC1:
        g_value_set_object(value, integrations->ndir1Tc1);
        break;
    case PROP_NDIR1_TC2:
        g_value_set_object(value, integrations->ndir1Tc2);
        break;
    case PROP_NDIR1_TC3:
        g_value_set_object(value, integrations->ndir1Tc3);
        break;
    case PROP_NDIR1_TIC1:
        g_value_set_object(value, integrations->ndir1Tic1);
        break;
    case PROP_NDIR1_TIC2:
        g_value_set_object(value, integrations->ndir1Tic2);
        break;
    case PROP_NDIR1_TIC3:
        g_value_set_object(value, integrations->ndir1Tic3);
        break;
    case PROP_NDIR2_TC1:
        g_value_set_object(value, integrations->ndir2Tc1);
        break;
    case PROP_NDIR2_TC2:
        g_value_set_object(value, integrations->ndir2Tc2);
        break;
    case PROP_NDIR2_TC3:
        g_value_set_object(value, integrations->ndir2Tc3);
        break;
    case PROP_NDIR2_TIC1:
        g_value_set_object(value, integrations->ndir2Tic1);
        break;
    case PROP_NDIR2_TIC2:
        g_value_set_object(value, integrations->ndir2Tic2);
        break;
    case PROP_NDIR2_TIC3:
        g_value_set_object(value, integrations->ndir2Tic3);
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

static void integrations_class_init(LarIntegrationsClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // Class* parent_class = MODEL_CLASS (klass);
    object_class->finalize     = integrations_finalize;
    object_class->set_property = integrations_set_property;
    object_class->get_property = integrations_get_property;

    g_object_class_install_property(
        object_class, PROP_NDIR1_TC1, g_param_spec_object("ndir1Tc1", "NDIR 1 TC 1", "NDIR1 TC integration", TYPE_LAR_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_NDIR1_TC2, g_param_spec_object("ndir1Tc2", "NDIR 1 TC 2", "NDIR1 TC integration", TYPE_LAR_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_NDIR1_TC3, g_param_spec_object("ndir1Tc3", "NDIR 1 TC 3", "NDIR1 TC integration", TYPE_LAR_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_NDIR1_TIC1, g_param_spec_object("ndir1Tic1", "NDIR 1 TIC 1", "NDIR1 TIC integration", TYPE_LAR_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_NDIR1_TIC2, g_param_spec_object("ndir1Tic2", "NDIR 1 TIC 2", "NDIR1 TIC integration", TYPE_LAR_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_NDIR1_TIC3, g_param_spec_object("ndir1Tic3", "NDIR 1 TIC 3", "NDIR1 TIC integration", TYPE_LAR_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_NDIR2_TC1, g_param_spec_object("ndir2Tc1", "NDIR 2 TC 1", "NDIR2 TC integration", TYPE_LAR_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_NDIR2_TC2, g_param_spec_object("ndir2Tc2", "NDIR 2 TC 2", "NDIR2 TC integration", TYPE_LAR_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_NDIR2_TC3, g_param_spec_object("ndir2Tc3", "NDIR 2 TC 3", "NDIR2 TC integration", TYPE_LAR_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_NDIR2_TIC1, g_param_spec_object("ndir2Tic1", "NDIR 2 TIC 1", "NDIR2 TIC integration", TYPE_LAR_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_NDIR2_TIC2, g_param_spec_object("ndir2Tic2", "NDIR 2 TIC 2", "NDIR2 TIC integration", TYPE_LAR_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_NDIR2_TIC3, g_param_spec_object("ndir2Tic3", "NDIR 2 TIC 3", "NDIR2 TIC integration", TYPE_LAR_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_TNB_TC, g_param_spec_object("tnbTc", "TNb TC", "TNb TC integration", TYPE_LAR_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_TNB_TIC, g_param_spec_object("tnbTic", "TNb TIC", "TNb TIC integration", TYPE_LAR_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
    g_object_class_install_property(
        object_class, PROP_CODO, g_param_spec_object("codo", "CODo TC", "CODo TC integration", TYPE_LAR_INTEGRATION, G_PARAM_READABLE | G_PARAM_WRITABLE ));
}

LarIntegration *m_LarIntegrationsGetNDir1TC1(LarIntegrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_LAR_INTEGRATIONS(integrations),NULL);
    return integrations->ndir1Tc1;
}
LarIntegration *m_LarIntegrationsGetNDir1TC2(LarIntegrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_LAR_INTEGRATIONS(integrations),NULL);
    return integrations->ndir1Tc2;
}
LarIntegration *m_LarIntegrationsGetNDir1TC3(LarIntegrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_LAR_INTEGRATIONS(integrations),NULL);
    return integrations->ndir1Tc3;
}
LarIntegration *m_LarIntegrationsGetNDir1TIC1(LarIntegrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_LAR_INTEGRATIONS(integrations),NULL);
    return integrations->ndir1Tic1;
}
LarIntegration *m_LarIntegrationsGetNDir1TIC2(LarIntegrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_LAR_INTEGRATIONS(integrations),NULL);
    return integrations->ndir1Tic2;
}
LarIntegration *m_LarIntegrationsGetNDir1TIC3(LarIntegrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_LAR_INTEGRATIONS(integrations),NULL);
    return integrations->ndir1Tic3;
}
LarIntegration *m_LarIntegrationsGetNDir2TC1(LarIntegrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_LAR_INTEGRATIONS(integrations),NULL);
    return integrations->ndir2Tc1;
}
LarIntegration *m_LarIntegrationsGetNDir2TC2(LarIntegrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_LAR_INTEGRATIONS(integrations),NULL);
    return integrations->ndir2Tc2;
}
LarIntegration *m_LarIntegrationsGetNDir2TC3(LarIntegrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_LAR_INTEGRATIONS(integrations),NULL);
    return integrations->ndir2Tc3;
}
LarIntegration *m_LarIntegrationsGetNDir2TIC1(LarIntegrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_LAR_INTEGRATIONS(integrations),NULL);
    return integrations->ndir2Tic1;
}
LarIntegration *m_LarIntegrationsGetNDir2TIC2(LarIntegrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_LAR_INTEGRATIONS(integrations),NULL);
    return integrations->ndir2Tic2;
}
LarIntegration *m_LarIntegrationsGetNDir2TIC3(LarIntegrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_LAR_INTEGRATIONS(integrations),NULL);
    return integrations->ndir2Tic3;
}
LarIntegration *m_LarIntegrationsGetTNbTC(LarIntegrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_LAR_INTEGRATIONS(integrations),NULL);
    return integrations->tnbTc;
}
LarIntegration *m_LarIntegrationsGetTNbTIC(LarIntegrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_LAR_INTEGRATIONS(integrations),NULL);
    return integrations->tnbTic;
}
LarIntegration *m_LarIntegrationsGetCODo(LarIntegrations *integrations) {
    g_return_val_if_fail(integrations != NULL, NULL);
    g_return_val_if_fail(IS_LAR_INTEGRATIONS(integrations),NULL);
    return integrations->codo;
}

