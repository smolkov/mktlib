/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * relay.c
 * Copyright (C) LAR 2017
 *
 */

#include "relay.h"

enum {
    PROP_0,
    PROP_BOOLEXPR,
};

struct _RelayPrivate {
    gchar *boolexp;
};

#define RELAY_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_RELAY, RelayPrivate))

G_DEFINE_TYPE(Relay, relay, G_TYPE_OBJECT)

static void relay_init(Relay *relay) {
    RelayPrivate *priv   = RELAY_PRIVATE(relay);
    relay->priv          = priv;
    relay->priv->boolexp = g_strdup("");
    /* TODO: Add initialization code here */
}

static void relay_finalize(GObject *model) {
    Relay *data = RELAY(model);
    if (data->priv->boolexp) g_free(data->priv->boolexp);
    G_OBJECT_CLASS(relay_parent_class)->finalize(model);
}

static void relay_set_property(GObject *model, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_RELAY(model));
    Relay *relay = RELAY(model);
    switch (prop_id) {
    case PROP_BOOLEXPR:
        if (relay->priv->boolexp) g_free(relay->priv->boolexp);
        relay->priv->boolexp = g_value_dup_string(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(model, prop_id, pspec);
        break;
    }
}

static void relay_get_property(GObject *model, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_RELAY(model));
    Relay *stream = RELAY(model);
    switch (prop_id) {
    case PROP_BOOLEXPR:
        g_value_set_string(value, stream->priv->boolexp);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(model, prop_id, pspec);
        break;
    }
}

static void relay_class_init(RelayClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // Class* parent_class = MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(RelayPrivate));
    object_class->finalize     = relay_finalize;
    object_class->set_property = relay_set_property;
    object_class->get_property = relay_get_property;

    g_object_class_install_property(
        object_class, PROP_BOOLEXPR, g_param_spec_string("boolexpr", "bool expresion", "Set get relay bool expression", "", G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
}

const gchar* RelayGetBoolExpression(Relay *relay) {
    g_return_val_if_fail(relay != NULL, NULL);
    g_return_val_if_fail(IS_RELAY(relay), NULL);
    return relay->priv->boolexp;
}
