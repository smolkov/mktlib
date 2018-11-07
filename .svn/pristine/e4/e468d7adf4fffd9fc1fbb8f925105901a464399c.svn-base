/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * sigdata.c
 * Copyright (C) LAR 2017
 *

 */

#include "sigdata.h"

enum {
    PROP_0,
    PROP_SEC,
    PROP_VALUE,
};

struct _SigdataPrivate {
    gdouble  sec;
    gdouble  value;
};

#define SIGDATA_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_SIGDATA, SigdataPrivate))

G_DEFINE_TYPE(Sigdata, sigdata, G_TYPE_OBJECT)

static void sigdata_init(Sigdata *sigdata) {
    SigdataPrivate *priv = SIGDATA_PRIVATE(sigdata);
    priv->sec                    = 0.0;
    priv->value                  = 0.0;
    sigdata->priv       = priv;

    /* TODO: Add initialization code here */
}

static void sigdata_finalize(GObject *object) {
    // Sigdata *data = SIGDATA(object);

    G_OBJECT_CLASS(sigdata_parent_class)->finalize(object);
}

static void sigdata_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_SIGDATA(object));
    Sigdata *data = SIGDATA(object);
    switch (prop_id) {
    case PROP_SEC:
        data->priv->sec = g_value_get_double(value);
        break;
    case PROP_VALUE:
        data->priv->value = g_value_get_double(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void sigdata_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_SIGDATA(object));
    Sigdata *data = SIGDATA(object);
    switch (prop_id) {
    case PROP_SEC:
        g_value_set_double(value, data->priv->sec);
        break;
    case PROP_VALUE:
        g_value_set_double(value, data->priv->value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void sigdata_class_init(SigdataClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // ModelClass* parent_class = MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(SigdataPrivate));
    object_class->finalize     = sigdata_finalize;
    object_class->set_property = sigdata_set_property;
    object_class->get_property = sigdata_get_property;

    g_object_class_install_property(
        object_class, PROP_VALUE, g_param_spec_double("value", "Sigdata sigdata property", "Set get sensor sigdata property", -G_MAXDOUBLE, G_MAXDOUBLE, 0., G_PARAM_READABLE | G_PARAM_WRITABLE));

    g_object_class_install_property(object_class, PROP_SEC,
        g_param_spec_double("second", "Sigdata secind", "Set get sensor sigdata second", 0., G_MAXDOUBLE, 0., G_PARAM_READABLE | G_PARAM_WRITABLE));

}


/**
 * SigdataGetValue:
 * @sigdata: a #Sigdata
 *
 * sigdata result 
 *
 * Return:  sigdata result
 */
gdouble SigdataGetValue(Sigdata *sigdata) {
    g_return_val_if_fail(sigdata != NULL, 0.0);
    g_return_val_if_fail(IS_SIGDATA(sigdata), 0.0);
    return sigdata->priv->value;
}
/**
 * SigdataGetSecond:
 * @sigdata: a #Sigdata
 *
 * sigdata time in sec 
 *
 * Return:  second
 */
gdouble SigdataGetSecond(Sigdata *sigdata) {
    g_return_val_if_fail(sigdata != NULL, 0.0);
    g_return_val_if_fail(IS_SIGDATA(sigdata), 0.0);
    return sigdata->priv->sec;
}

