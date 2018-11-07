/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * channel.c
 * Copyright (C) 2017 LAR
 */

#include "channel.h"

#include <glib/gprintf.h>
#include <math.h>

enum {
    PROP_0,
    PROP_NAME,
    PROP_UNIT,
    PROP_SENSOR,
    PROP_ACTIVATED,
    PROP_ACTIVATED_CAL,
    PROP_CHECK,
    PROP_NEED_CALIBRATION,
    PROP_TYPE,
    PROP_ANALOG,
    PROP_CHECK_ANALOG,
    PROP_MIN,
    PROP_MAX,
    PROP_FACTOR,
    PROP_LIMIT,
    PROP_CHECK_LIMIT,
    PROP_TRIGGER,
    PROP_REF
};

struct _ChannelPrivate {
    gchar *  name;
    gchar *  unit;
    gchar *  sensor;
    gboolean activatedCal;
    gboolean activated;
    gboolean check;
    gchar *  analog;
    gchar *  checkAnalog;
    gdouble  min;
    gdouble  max;
    gdouble  factor;
    Limit *  checkLimit;
    Limit *  limit;
    guint64  trigger;
    guint64  ref;
};

#define CHANNEL_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_CHANNEL, ChannelPrivate))

G_DEFINE_TYPE(Channel, channel, G_TYPE_OBJECT)

static void channel_init(Channel *channel) {
    ChannelPrivate *priv = CHANNEL_PRIVATE(channel);
    priv->name           = g_strdup("empty");
    priv->unit           = g_strdup("mg/l");
    priv->sensor         = g_strdup("empty");
    priv->analog         = g_strdup("empty");
    priv->checkAnalog    = g_strdup("empty");
    priv->min            = 0.;
    priv->max            = 100.;
    priv->activatedCal   = FALSE;
    priv->check          = FALSE;
    priv->factor         = 1.0;
    priv->trigger        = 0;
    priv->ref            = 0;
    priv->limit          = LIMIT(g_object_new(TYPE_LIMIT, NULL));
    priv->checkLimit     = LIMIT(g_object_new(TYPE_LIMIT, NULL));

    channel->priv = priv;
    /* TODO: Add initialization code here */
}

static void channel_finalize(GObject *object) {

    Channel *mchannel = CHANNEL(object);
    if (mchannel->priv->sensor) g_free(mchannel->priv->sensor);
    if (mchannel->priv->unit) g_free(mchannel->priv->unit);
    if (mchannel->priv->name) g_free(mchannel->priv->name);
    if (mchannel->priv->analog) g_free(mchannel->priv->analog);
    if (mchannel->priv->checkAnalog) g_free(mchannel->priv->checkAnalog);
    if (mchannel->priv->checkLimit) g_object_unref(mchannel->priv->checkLimit);
    if (mchannel->priv->limit) g_object_unref(mchannel->priv->limit);
    G_OBJECT_CLASS(channel_parent_class)->finalize(object);
}

static void channel_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_CHANNEL(object));
    Channel *channel = CHANNEL(object);
    switch (prop_id) {
    case PROP_UNIT:
        if (channel->priv->unit != NULL) g_free(channel->priv->unit);
        channel->priv->unit = g_value_dup_string(value);
        break;
    case PROP_SENSOR:
        if (channel->priv->sensor != NULL) g_free(channel->priv->sensor);
        channel->priv->sensor = g_value_dup_string(value);
        break;
    case PROP_NAME:
        if (channel->priv->name != NULL) g_free(channel->priv->name);
        channel->priv->name = g_value_dup_string(value);
        break;
    case PROP_ACTIVATED:
        channel->priv->activated = g_value_get_boolean(value);
        break;
    case PROP_ACTIVATED_CAL:
        channel->priv->activatedCal = g_value_get_boolean(value);
        break;
    case PROP_CHECK:
        channel->priv->check = g_value_get_boolean(value);
        break;
    case PROP_ANALOG:
        if (channel->priv->analog != NULL) g_free(channel->priv->analog);
        channel->priv->analog = g_value_dup_string(value);
        break;
    case PROP_CHECK_ANALOG:
        if (channel->priv->checkAnalog != NULL) g_free(channel->priv->checkAnalog);
        channel->priv->checkAnalog = g_value_dup_string(value);
        break;
    case PROP_MIN:
        channel->priv->min = g_value_get_double(value);
        break;
    case PROP_MAX:
        channel->priv->max = g_value_get_double(value);
        break;
    case PROP_FACTOR:
        channel->priv->factor = g_value_get_double(value);
        break;
    case PROP_LIMIT:
        if (channel->priv->limit != NULL) g_object_unref(channel->priv->limit);
        channel->priv->limit = g_value_dup_object(value);
        break;
    case PROP_CHECK_LIMIT:
        if (channel->priv->checkLimit != NULL) g_object_unref(channel->priv->checkLimit);
        channel->priv->checkLimit = g_value_dup_object(value);
        break;
    case PROP_TRIGGER:
        channel->priv->trigger = g_value_get_uint64(value);
        break;
    case PROP_REF:
        channel->priv->ref= g_value_get_uint64(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void channel_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_CHANNEL(object));
    Channel *channel = CHANNEL(object);
    switch (prop_id) {
    case PROP_NAME:
        g_value_set_string(value, channel->priv->name);
        break;
    case PROP_SENSOR:
        g_value_set_string(value, channel->priv->sensor);
        break;
    case PROP_UNIT:
        g_value_set_string(value, channel->priv->unit);
        break;
    case PROP_ANALOG:
        g_value_set_string(value, channel->priv->analog);
        break;
    case PROP_CHECK_ANALOG:
        g_value_set_string(value, channel->priv->checkAnalog);
        break;
    case PROP_ACTIVATED:
        g_value_set_boolean(value, channel->priv->activated);
        break;
    case PROP_ACTIVATED_CAL:
        g_value_set_boolean(value, channel->priv->activatedCal);
        break;
    case PROP_CHECK:
        g_value_set_boolean(value, channel->priv->check);
        break;
    case PROP_MIN:
        g_value_set_double(value, channel->priv->min);
        break;
    case PROP_MAX:
        g_value_set_double(value, channel->priv->max);
        break;
    case PROP_FACTOR:
        g_value_set_double(value, channel->priv->factor);
        break;
    case PROP_LIMIT:
        g_value_set_object(value, channel->priv->limit);
        break;
    case PROP_CHECK_LIMIT:
        g_value_set_object(value, channel->priv->checkLimit);
        break;
    case PROP_TRIGGER:
        g_value_set_uint64(value, channel->priv->trigger);
        break;
    case PROP_REF:
        g_value_set_uint64(value, channel->priv->ref);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void channel_class_init(ChannelClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // Class* parent_class = MODEL_CLASS (klass);

    g_type_class_add_private(klass, sizeof(ChannelPrivate));
    object_class->finalize     = channel_finalize;
    object_class->set_property = channel_set_property;
    object_class->get_property = channel_get_property;

    g_object_class_install_property(object_class, PROP_NAME, g_param_spec_string("name", "name", "Set get channel name property", "----", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_UNIT, g_param_spec_string("unit", "unit", "Set get channel unit property", "mg/l", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_SENSOR, g_param_spec_string("sensor", "sensor reference", "sensor reference", "/com/lar/tera/integrations/nil", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_ACTIVATED, g_param_spec_boolean("activated", "is activated", "Set get channel is activated", TRUE, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_CHECK, g_param_spec_boolean("check", "check is activated", "Set get channel check is activated", FALSE, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_ACTIVATED_CAL, g_param_spec_boolean("calibration", "Channel need calibration", "Channel need calibration", TRUE, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_ANALOG,
        g_param_spec_string("analog", "analog out reference", "Set get channel analog out hardware object name", "/analogs/1", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_CHECK_ANALOG, g_param_spec_string("checkAnalog", "check analog out reference", "Set get channel check analog out hardware object name",
                                                                         "/analogs/100", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_MIN,
        g_param_spec_double("min", "minimum scale limit", "Set get channel min scale value", -G_MAXDOUBLE, G_MAXDOUBLE, 0.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_MAX,
        g_param_spec_double("max", "maximum scale limit", "Set get channel max scale value", -G_MAXDOUBLE, G_MAXDOUBLE, 100.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_FACTOR, g_param_spec_double("factor", "secret factor", "Set get channel secret factor", 0.001, 100.0, 1.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_LIMIT, g_param_spec_object("limit", "channel limit", "channel limit", TYPE_LIMIT, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(
        object_class, PROP_CHECK_LIMIT, g_param_spec_object("checkLimit", "channel check limit", "channel check limit", TYPE_LIMIT, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(
        object_class, PROP_TRIGGER, g_param_spec_uint64("trigger", "trigger", "Set get channel signal trigger", 0, G_MAXUINT64, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_REF, g_param_spec_uint64("reference", "reference", "Set get channel reference", 0, G_MAXUINT64, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
}

const gchar *ChannelName(Channel *channel) {
    g_return_val_if_fail(channel != NULL, NULL);
    g_return_val_if_fail(IS_CHANNEL(channel), NULL);
    return channel->priv->name;
}

const gchar *ChannelUnit(Channel *channel) {
    g_return_val_if_fail(channel != NULL, NULL);
    g_return_val_if_fail(IS_CHANNEL(channel), NULL);
    return channel->priv->unit;
}
const gchar *ChannelSensor(Channel *channel) {
    g_return_val_if_fail(channel != NULL, NULL);
    g_return_val_if_fail(IS_CHANNEL(channel), NULL);
    return channel->priv->sensor;
}

const gchar *ChannelAnalog(Channel *channel) {
    g_return_val_if_fail(channel != NULL, NULL);
    g_return_val_if_fail(IS_CHANNEL(channel), NULL);
    return channel->priv->analog;
}
const gchar *ChannelCheckAnalog(Channel *channel) {
    g_return_val_if_fail(channel != NULL, NULL);
    g_return_val_if_fail(IS_CHANNEL(channel), NULL);
    return channel->priv->checkAnalog;
}

gdouble ChannelMin(Channel *channel) {
    g_return_val_if_fail(channel != NULL, 0.0);
    g_return_val_if_fail(IS_CHANNEL(channel), 0.0);
    return channel->priv->min;
}
gdouble ChannelMax(Channel *channel) {
    g_return_val_if_fail(channel != NULL, 101.99);
    g_return_val_if_fail(IS_CHANNEL(channel), 101.99);
    return channel->priv->max;
}
gdouble ChannelFactor(Channel *channel) {
    g_return_val_if_fail(channel != NULL, 1.0);
    g_return_val_if_fail(IS_CHANNEL(channel), 1.0);
    return channel->priv->factor;
}
gboolean ChannelActivated(Channel *channel) {
    g_return_val_if_fail(channel != NULL, FALSE);
    g_return_val_if_fail(IS_CHANNEL(channel), FALSE);
    return channel->priv->activated;
}
gboolean ChannelActivatedCal(Channel *channel) {
    g_return_val_if_fail(channel != NULL, FALSE);
    g_return_val_if_fail(IS_CHANNEL(channel), FALSE);
    return channel->priv->activatedCal;
}
gboolean ChannelCheck(Channel *channel) {
    g_return_val_if_fail(channel != NULL, FALSE);
    g_return_val_if_fail(IS_CHANNEL(channel), FALSE);
    return channel->priv->check;
}

Limit *ChannelGetLimit(Channel *channel) {
    g_return_val_if_fail(channel != NULL, NULL);
    g_return_val_if_fail(IS_CHANNEL(channel), NULL);
    return channel->priv->limit;
}
Limit *ChannelGetCheckLimit(Channel *channel) {
    g_return_val_if_fail(channel != NULL, NULL);
    g_return_val_if_fail(IS_CHANNEL(channel), NULL);
    return channel->priv->checkLimit;
}
guint64 ChannelTrigger(Channel *channel) {
    g_return_val_if_fail(channel != NULL,0);
    g_return_val_if_fail(IS_CHANNEL(channel), 0);
    return channel->priv->trigger;
}

guint64 ChannelReference(Channel *channel) {
    g_return_val_if_fail(channel != NULL,0);
    g_return_val_if_fail(IS_CHANNEL(channel), 0);
    return channel->priv->ref;
}
