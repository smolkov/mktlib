/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * axis.c
 * Copyright (C) LAR 2017
 *
 */

#include "axis.h"


enum { PROP_0, PROP_AXIS_MAX, PROP_AXIS_HOLD, PROP_AXIS_CURRENT, PROP_AXIS_REVERCE };

G_DEFINE_TYPE(Axis, axis, G_TYPE_OBJECT)

static void axis_init(Axis *axis) {
}

static void axis_finalize(GObject *object) {
    G_OBJECT_CLASS(axis_parent_class)->finalize(object);
}

static void axis_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_AXIS(object));
    Axis *stream = AXIS(object);
    switch (prop_id) {
    case PROP_AXIS_MAX:
        stream->max = g_value_get_uint(value);
        break;
    case PROP_AXIS_HOLD:
        stream->hold = g_value_get_uint(value);
        break;
    case PROP_AXIS_CURRENT:
        stream->current = g_value_get_uint(value);
        break;
    case PROP_AXIS_REVERCE:
        stream->reverse = g_value_get_boolean(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void axis_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_AXIS(object));
    Axis *stream = AXIS(object);
    switch (prop_id) {
    case PROP_AXIS_MAX:
        g_value_set_uint(value, stream->max);
        break;
    case PROP_AXIS_HOLD:
        g_value_set_uint(value, stream->hold);
        break;
    case PROP_AXIS_CURRENT:
        g_value_set_uint(value, stream->current);
        break;
    case PROP_AXIS_REVERCE:
        g_value_set_boolean(value, stream->reverse);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void axis_class_init(AxisClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->finalize     = axis_finalize;
    object_class->set_property = axis_set_property;
    object_class->get_property = axis_get_property;

    g_object_class_install_property(object_class, PROP_AXIS_MAX,
        g_param_spec_uint("max", "max position parameter", "Set get axis max position parameter", 10, 20000, 1000, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_AXIS_HOLD,
        g_param_spec_uint("hold", "hold position parameter", "Set get axis hold position parameter", 10, 20000, 10, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_AXIS_CURRENT,
        g_param_spec_uint("current", "current parameter", "Set get axis current parameter", 100, 16000, 100, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_AXIS_REVERCE, g_param_spec_boolean("reverse", "is reverse", "Set get reverse parameter", FALSE, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
}




guint AxisGetMax(Axis *axis) {
    g_return_val_if_fail(axis!= NULL,0);
    g_return_val_if_fail(IS_AXIS(axis),0);
    return axis->max;
}
guint AxisGetHold(Axis *axis) {
    g_return_val_if_fail(axis!= NULL,0);
    g_return_val_if_fail(IS_AXIS(axis),0);
    return axis->hold;
}
guint AxisGetCurrent(Axis *axis) {
    g_return_val_if_fail(axis!= NULL,0);
    g_return_val_if_fail(IS_AXIS(axis),0);
    return axis->current;
}