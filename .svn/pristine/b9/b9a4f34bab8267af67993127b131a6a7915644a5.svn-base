/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * injection.c
 * Copyright (C) LAR 2017
 *
 */

#include "injection.h"
#include "axis.h"

enum {
    PROP_0,
    PROP_AIR,
    PROP_REST,
    PROP_FURNACE_AIR,
    PROP_DILUTION,
    PROP_RINSING,
    PROP_PUSH,
    PROP_SAMPLE_PULL,
    PROP_RINSING_PULL,
    PROP_RINSING_PUSH,
};



struct _InjectionClass {
    AxisClass parent_class;
};

struct _Injection {
    Axis     parent_instance;
    guint    air;
    guint    rest;
    guint    furnace_air;
    guint    dilution;
    gboolean rinsing;
    guint    push;
    guint    samplePull;
    guint    rinsingPull;
    guint    rinsingPush;
    
};






G_DEFINE_TYPE(Injection, injection, TYPE_AXIS)

static void injection_init(Injection *injection) {
    injection->air = 15;
    injection->rest = 100;
    /* TODO: Add initialization code here */
}

static void injection_finalize(GObject *object) {
    // Injection *data = INJECTION(object);
    G_OBJECT_CLASS(injection_parent_class)->finalize(object);
}

static void injection_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_INJECTION(object));
    Injection *injection = INJECTION(object);
    // g_debug("injection_get_property %s",pspec->name);
    switch (prop_id) {
    case PROP_AIR:
        injection->air = g_value_get_uint(value);
        break;
    case PROP_REST:
        injection->rest = g_value_get_uint(value);
        break;
    case PROP_FURNACE_AIR:
        injection->furnace_air = g_value_get_uint(value);
        break;
    case PROP_DILUTION:
        injection->dilution = g_value_get_uint(value);
        break;
    case PROP_RINSING:
        injection->rinsing = g_value_get_uint(value);
        break;
    case PROP_PUSH:
        injection->push = g_value_get_uint(value);
        break;
    case PROP_SAMPLE_PULL:
        injection->samplePull = g_value_get_uint(value);
        break;
    case PROP_RINSING_PULL:
        injection->rinsingPull = g_value_get_uint(value);
        break;
    case PROP_RINSING_PUSH:
        injection->rinsingPush = g_value_get_uint(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void injection_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_INJECTION(object));
    Injection *injection = INJECTION(object);
    switch (prop_id) {
    case PROP_AIR:
        g_value_set_uint(value, injection->air);
        break;
    case PROP_REST:
        g_value_set_uint(value, injection->rest);
        break;
    case PROP_FURNACE_AIR:
        g_value_set_uint(value, injection->furnace_air);
        break;
    case PROP_DILUTION:
        g_value_set_uint(value, injection->dilution);
        break;
    case PROP_RINSING:
        g_value_set_uint(value, injection->rinsing);
        break;
    case PROP_PUSH:
        g_value_set_uint(value, injection->push);
        break;
    case PROP_SAMPLE_PULL:
        g_value_set_uint(value, injection->samplePull);
        break;
    case PROP_RINSING_PULL:
        g_value_set_uint(value, injection->rinsingPull);
        break;
    case PROP_RINSING_PUSH:
        g_value_set_uint(value, injection->rinsingPush);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void injection_class_init(InjectionClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // Class* parent_class = MODEL_CLASS (klass);
    object_class->finalize     = injection_finalize;
    object_class->set_property = injection_set_property;
    object_class->get_property = injection_get_property;

    g_object_class_install_property(
        object_class, PROP_AIR, g_param_spec_uint("air", "air position", "Set get air position parameter", 0, 150, 100, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_REST, g_param_spec_uint("rest", "rest position", "Set get injection rest position parameter", 0, 200, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_FURNACE_AIR,
        g_param_spec_uint("furnaceAir", "furnacr air", "Set get injection current parameter", 0, 150, 50, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_DILUTION, g_param_spec_uint("dilution", "dilution position", "Set get injection dilution", 0, 600, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_RINSING, g_param_spec_uint("rinsing", "rinsing position", "Set get injection rinsing parameter", 100, 601, 580, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_PUSH, g_param_spec_uint("push", "push stepper", "Set get injection stepper parameter", 1, 30, 3, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_SAMPLE_PULL,
        g_param_spec_uint("samplePull", "sampling stepper", "Set get injection sampling stepper parameter", 1, 30, 2, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_RINSING_PULL,
        g_param_spec_uint("rinsingPull", "up stepper", "Set get injection up stepper parameter", 1, 30, 2, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_RINSING_PUSH,
        g_param_spec_uint("rinsingPush", "down stepper", "Set get injection down stepper parameter", 1, 30, 1, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
}



guint InjectionGetAir(Injection *injection) {
    g_return_val_if_fail(injection!= NULL,0);
    g_return_val_if_fail(IS_INJECTION(injection),0);
    return injection->air;
}
guint InjectionGetRest(Injection *injection) {
    g_return_val_if_fail(injection!= NULL,0);
    g_return_val_if_fail(IS_INJECTION(injection),0);
    return injection->rest;
}
guint InjectionGetFurnaceAir(Injection *injection) {
    g_return_val_if_fail(injection!= NULL,0);
    g_return_val_if_fail(IS_INJECTION(injection),0);
    return injection->furnace_air;
}
guint InjectionGetDilution(Injection *injection) {
    g_return_val_if_fail(injection!= NULL,0);
    g_return_val_if_fail(IS_INJECTION(injection),0);
    return injection->dilution;
}
guint InjectionGetRinsing(Injection *injection) {
    g_return_val_if_fail(injection!= NULL,0);
    g_return_val_if_fail(IS_INJECTION(injection),0);
    return injection->rinsing;
}
guint InjectionGetPush(Injection *injection) {
    g_return_val_if_fail(injection!= NULL,0);
    g_return_val_if_fail(IS_INJECTION(injection),0);
    return injection->push;
}
guint InjectionGetSamplePull(Injection *injection) {
    g_return_val_if_fail(injection!= NULL,0);
    g_return_val_if_fail(IS_INJECTION(injection),0);
    return injection->samplePull;
}
guint InjectionGetRinsingPull(Injection *injection) {
    g_return_val_if_fail(injection!= NULL,0);
    g_return_val_if_fail(IS_INJECTION(injection),0);
    return injection->rinsingPull;
}
guint InjectionGetRinsingPush(Injection *injection) {
    g_return_val_if_fail(injection!= NULL,0);
    g_return_val_if_fail(IS_INJECTION(injection),0);
    return injection->rinsingPush;
}