/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * posparm.c
 * Copyright (C) LAR 2017 
 
 */


#include "posparm.h"

enum {
    PROP_0,
    PROP_ONLINE,
    PROP_DRAIN,
    PROP_CALIBRATION,
    PROP_DILUTION,
    PROP_SINGLE,
    PROP_CHECK,
};

struct _PosparmPrivate {
    gchar *online;
    gchar *drain;
    gchar *calibration;
    gchar *dilution;
    gchar *single;
    gchar *check;
};

#define POSPARM_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_POSPARM, PosparmPrivate))


G_DEFINE_TYPE(Posparm, posparm,G_TYPE_OBJECT)

static void posparm_init(Posparm *posparm) {
    PosparmPrivate *priv = POSPARM_PRIVATE(posparm);

    priv->online      = g_strdup("/com/lar/tera/vessels/2");
    priv->single      = g_strdup("/com/lar/tera/vessels/1");
    priv->drain       = g_strdup("/com/lar/tera/vessels/5");
    priv->calibration = g_strdup("/com/lar/tera/vessels/1");
    priv->dilution    = g_strdup("/com/lar/tera/vessels/6");
    priv->check       = g_strdup("/com/lar/tera/vessels/1");
    posparm->priv     = priv;
    /* TODO: Add initialization code here */
}

static void posparm_finalize(GObject *object) {
    Posparm *posparm = POSPARM(object);
    if (posparm->priv->online) g_free(posparm->priv->online);
    if (posparm->priv->drain) g_free(posparm->priv->drain);
    if (posparm->priv->calibration) g_free(posparm->priv->calibration);
    if (posparm->priv->dilution) g_free(posparm->priv->dilution);
    if (posparm->priv->single) g_free(posparm->priv->single);
    if (posparm->priv->check) g_free(posparm->priv->check);
    G_OBJECT_CLASS(posparm_parent_class)->finalize(object);
}

static void posparm_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_POSPARM(object));
    Posparm *posparm = POSPARM(object);
    switch (prop_id) {
    case PROP_ONLINE:
        if (posparm->priv->online) g_free(posparm->priv->online);
        posparm->priv->online = g_value_dup_string(value);
        break;
    case PROP_DRAIN:
        if (posparm->priv->drain) g_free(posparm->priv->drain);
        posparm->priv->drain = g_value_dup_string(value);
        break;
    case PROP_CALIBRATION:
        if (posparm->priv->calibration) g_free(posparm->priv->calibration);
        posparm->priv->calibration = g_value_dup_string(value);
        break;
    case PROP_DILUTION:
        if (posparm->priv->dilution) g_free(posparm->priv->dilution);
        posparm->priv->dilution = g_value_dup_string(value);
        break;
    case PROP_SINGLE:
        if (posparm->priv->single) g_free(posparm->priv->single);
        posparm->priv->single = g_value_dup_string(value);
        break;
    case PROP_CHECK:
        if (posparm->priv->check) g_free(posparm->priv->check);
        posparm->priv->check = g_value_dup_string(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void posparm_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_POSPARM(object));
    Posparm *posparm = POSPARM(object);
    switch (prop_id) {

    case PROP_ONLINE:
        g_value_set_string(value, posparm->priv->online);
        break;
    case PROP_DRAIN:
        g_value_set_string(value, posparm->priv->drain);
        break;
    case PROP_CALIBRATION:
        g_value_set_string(value, posparm->priv->calibration);
        break;
    case PROP_DILUTION:
        g_value_set_string(value, posparm->priv->dilution);
        break;
    case PROP_SINGLE:
        g_value_set_string(value, posparm->priv->single);
        break;
    case PROP_CHECK:
        g_value_set_string(value, posparm->priv->check);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void posparm_class_init(PosparmClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    g_type_class_add_private(klass, sizeof(PosparmPrivate));
    object_class->finalize     = posparm_finalize;
    object_class->set_property = posparm_set_property;
    object_class->get_property = posparm_get_property;

    g_object_class_install_property(object_class, PROP_ONLINE,
        g_param_spec_string("online", "sample vessel reference", "Set|Get posparm sample vessel reference", "/com/lar/tera/vessels/2", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_DRAIN,
        g_param_spec_string("drain", "drain vessel reference", "Set|Get posparm drain vessel reference", "/com/lar/tera/vessels/5", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_CALIBRATION, g_param_spec_string("calibration", "calibration vessel reference", "Set|Get posparm calibration vessel reference",
                                                                        "/com/lar/tera/vessels/1", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_SINGLE,
        g_param_spec_string("single", "single vessel reference", "Set|Get posparm single vessel reference", "/com/lar/tera/vessels/1", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_CHECK,
        g_param_spec_string("check", "check vessel reference", "Set|Get posparm check vessel reference", "/com/lar/tera/vessels/1", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_DILUTION, g_param_spec_string("dilution-vessel", "dilution vessel reference", "Set|Get posparm dilution vessel reference",
                                                                     "/com/lar/tera/vessels/4", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
}

/**
 * PosparmGetOnlineVessel:
 * @posparm: a #Posparm
 *
 * Get online vessel address  
 *
 * Returns:  address as string not free it.
 */
const gchar* PosparmGetOnlineVessel(Posparm *posparm){
    g_return_val_if_fail(posparm != NULL,NULL);
    g_return_val_if_fail(IS_POSPARM(posparm),NULL);
    return posparm->priv->online;
}
/**
 * PosparmGetDrainVessel:
 * @posparm: a #Posparm
 *
 * Get drain vessel address  
 *
 * Returns:  address as string not free it.
 */
const gchar* PosparmGetDrainVessel(Posparm *posparm){
    g_return_val_if_fail(posparm != NULL,NULL);
    g_return_val_if_fail(IS_POSPARM(posparm),NULL);
    return posparm->priv->drain;
}
/**
 * PosparmGetCalibrationVessel:
 * @posparm: a #Posparm
 *
 * Get calibration vessel address  
 *
 * Returns:  address as string not free it.
 */
const gchar* PosparmGetCalibrationVessel(Posparm *posparm){
    g_return_val_if_fail(posparm != NULL,NULL);
    g_return_val_if_fail(IS_POSPARM(posparm),NULL);
    return posparm->priv->calibration;
}

/**
 * PosparmGetSingleVessel:
 * @posparm: a #Posparm
 *
 * Get single vessel address  
 *
 * Returns:  address as string not free it.
 */
const gchar* PosparmGetSingleVessel(Posparm *posparm){
    g_return_val_if_fail(posparm != NULL,NULL);
    g_return_val_if_fail(IS_POSPARM(posparm),NULL);
    return posparm->priv->single;
}
/**
 * PosparmGetCheckVessel:
 * @posparm: a #Posparm
 *
 * Get check vessel address  
 *
 * Returns:  address as string not free it.
 */
const gchar* PosparmGetCheckVessel(Posparm *posparm){
    g_return_val_if_fail(posparm != NULL,NULL);
    g_return_val_if_fail(IS_POSPARM(posparm),NULL);
    return posparm->priv->check;
}

const gchar* PosparmGetDilutionVessel(Posparm *posparm){
    g_return_val_if_fail(posparm != NULL,NULL);
    g_return_val_if_fail(IS_POSPARM(posparm),NULL);
    return posparm->priv->dilution;
}
