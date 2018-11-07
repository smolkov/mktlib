/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * dilution.c
 * Copyright (C) LAR 2017
 *
 */

#include "dilution.h"

enum { PROP_0, PROP_VOLUME, PROP_TAKE_POS, PROP_PUSH_POS, PROP_REPEAT };

struct _DilutionClass {
    GObjectClass parent_class;
};

struct _Dilution {
    GObject parent_instance;
    gdouble volume;
    guint   takePos;
    guint   pushPos;
    guint   repeat;
};

G_DEFINE_TYPE(Dilution, dilution, G_TYPE_OBJECT)

static void dilution_init(Dilution *dilution) {
    // DilutionPrivate *priv = DILUTION_PRIVATE(dilution);
    // dilution              = priv;
    dilution->volume  = 1700.0;
    dilution->takePos = 1420;
    dilution->pushPos = 1020;
    dilution->repeat  = 3;
    /* TODO: Add initialization code here */
}

static void dilution_finalize(GObject *object) {
    // Dilution *data = DILUTION(object);
    G_OBJECT_CLASS(dilution_parent_class)->finalize(object);
}

static void dilution_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_DILUTION(object));
    Dilution *dilution = DILUTION(object);
    // g_debug("dilution_get_property %s",pspec->name);
    switch (prop_id) {
    case PROP_VOLUME:
        dilution->volume = g_value_get_double(value);
        break;
    case PROP_TAKE_POS:
        dilution->takePos = g_value_get_uint(value);
        break;
    case PROP_PUSH_POS:
        dilution->pushPos = g_value_get_uint(value);
        break;
    case PROP_REPEAT:
        dilution->repeat = g_value_get_uint(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void dilution_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_DILUTION(object));
    Dilution *dilution = DILUTION(object);
    switch (prop_id) {
    case PROP_VOLUME:
        g_value_set_double(value, dilution->volume);
        break;
    case PROP_TAKE_POS:
        g_value_set_uint(value, dilution->takePos);
        break;
    case PROP_PUSH_POS:
        g_value_set_uint(value, dilution->pushPos);
        break;
    case PROP_REPEAT:
        g_value_set_uint(value, dilution->repeat);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void dilution_class_init(DilutionClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // Class* parent_class = MODEL_CLASS (klass);
    object_class->finalize     = dilution_finalize;
    object_class->set_property = dilution_set_property;
    object_class->get_property = dilution_get_property;

    g_object_class_install_property(object_class, PROP_VOLUME,
        g_param_spec_double("volume", "vessel volume", "Set get dilution vessel volume", 10., 20000., 1700.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_TAKE_POS, g_param_spec_uint("takePos", "take position", "Set get dilution take position", 0, 2000, 1420, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_PUSH_POS, g_param_spec_uint("pushPos", "push position", "Set get dilution push position", 0, 2000, 1020, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_REPEAT, g_param_spec_uint("repeat", "repeat", "Set get dilution repeat", 1, 10, 3, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
}

gdouble DilutionGetVolume(Dilution *dilution) {
    g_return_val_if_fail(dilution != NULL, 0.0);
    g_return_val_if_fail(IS_DILUTION(dilution), 0.0);
    return dilution->volume;
}
guint DilutionGetTakePosition(Dilution *dilution) {
    g_return_val_if_fail(dilution != NULL, 0);
    g_return_val_if_fail(IS_DILUTION(dilution), 0);
    return dilution->takePos;
}
guint DilutionGetPushPosition(Dilution *dilution) {
    g_return_val_if_fail(dilution != NULL, 0);
    g_return_val_if_fail(IS_DILUTION(dilution), 0);
    return dilution->pushPos;
}
guint DilutionGetRepeat(Dilution *dilution) {
    g_return_val_if_fail(dilution != NULL, 0);
    g_return_val_if_fail(IS_DILUTION(dilution), 0);
    return dilution->repeat;
}