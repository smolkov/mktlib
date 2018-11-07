/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * interval.c
 * Copyright (C) LAR 2017

 */

#include "interval.h"
#include "lartime.h"

enum {
    PROP_0,
    PROP_TIMER,
    PROP_ACTIVATED,
};

struct _IntervalClass {
    GObjectClass parent_class;
};

struct _Interval {
    GObject  parent_instance;
    gint64  interval;   //in seconds 
    gboolean activated;
};


G_DEFINE_TYPE(Interval, interval, G_TYPE_OBJECT);

static void interval_init(Interval *interval) {
    interval->interval  = LARTIME_SPAN_HOUR * 12.0;
    interval->activated = FALSE;
}

static void interval_finalize(GObject *object) {
    // Interval *interval = INTERVAL(object);
    G_OBJECT_CLASS(interval_parent_class)->finalize(object);
}

static void interval_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_INTERVAL(object));
    Interval *interval = INTERVAL(object);
    switch (prop_id) {
    case PROP_TIMER:
        interval->interval = g_value_get_int64(value);
        break;
    case PROP_ACTIVATED:
        interval->activated = g_value_get_boolean(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void interval_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_INTERVAL(object));
    Interval *interval = INTERVAL(object);
    switch (prop_id) {
    case PROP_TIMER:
        g_value_set_int64(value, interval->interval);
        break;
    case PROP_ACTIVATED:
        g_value_set_boolean(value, interval->activated);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void interval_class_init(IntervalClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // Class* parent_class = MODEL_CLASS (klass);
    object_class->finalize     = interval_finalize;
    object_class->set_property = interval_set_property;
    object_class->get_property = interval_get_property;

    g_object_class_install_property(object_class, PROP_TIMER,
        g_param_spec_int64("interval", "interval time", "Set get interval time", 0, G_MAXINT64,(gint64)(G_TIME_SPAN_HOUR * 2) , G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_ACTIVATED, g_param_spec_boolean("activated", "is activated", "Set get is activated", FALSE, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
}

gint64 IntervalGetInterval(Interval *interval) {
    g_return_val_if_fail(interval != NULL, 0);
    return interval->interval;
}
gboolean IntervalGetActivated(Interval *interval) {
    g_return_val_if_fail(interval != NULL, FALSE);
    return interval->activated;
}
