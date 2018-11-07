/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * stream.c
 * Copyright (C) 2017 LAR
 */

#include "stream.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>
#include <string.h>

struct _StreamPrivate {
    gchar *     name;
    guint       number;
    Intervals * intervals;
    Statistics *statistics;
    Amount *    amount;
    gboolean     remote;
};

#define STREAM_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_STREAM, StreamPrivate))

enum { PROP_0, PROP_NAME, PROP_NUMBER, PROP_REMOTE, PROP_STATISTICS, PROP_INTERVALS, PROP_AMOUNT };

static JsonSerializableIface *stream_serializable_iface = NULL;

static void json_serializable_iface_init(gpointer g_iface);

G_DEFINE_TYPE_WITH_CODE(Stream, stream, G_TYPE_OBJECT, G_IMPLEMENT_INTERFACE(JSON_TYPE_SERIALIZABLE, json_serializable_iface_init));

static JsonNode *stream_serialize_property(JsonSerializable *serializable, const gchar *name, const GValue *value, GParamSpec *pspec) {
    // JsonNode *retval = NULL;
    return stream_serializable_iface->serialize_property(serializable, name, value, pspec);
}

static gboolean stream_deserialize_property(JsonSerializable *serializable, const gchar *property_name, GValue *value, GParamSpec *pspec, JsonNode *property_node) {
    return stream_serializable_iface->deserialize_property(serializable, property_name, value, pspec, property_node);
}

static void json_serializable_iface_init(gpointer g_iface) {
    JsonSerializableIface *iface = g_iface;
    stream_serializable_iface    = g_type_default_interface_peek(JSON_TYPE_SERIALIZABLE);
    iface->serialize_property    = stream_serialize_property;
    iface->deserialize_property  = stream_deserialize_property;
}

static void stream_init(Stream *stream) {
    StreamPrivate *priv      = STREAM_PRIVATE(stream);
    stream->priv             = priv;
    stream->priv->name       = g_strdup("none");
    stream->priv->number     = 1;
    stream->priv->remote     = TRUE;
    stream->priv->intervals  = INTERVALS(g_object_new(TYPE_INTERVALS, NULL));
    stream->priv->statistics = STATISTICS(g_object_new(TYPE_STATISTICS, NULL));
    stream->priv->amount     = AMOUNT(g_object_new(TYPE_AMOUNT, NULL));
}

static void stream_finalize(GObject *object) {
    Stream *stream = STREAM(object);
    if (stream->priv->name) g_free(stream->priv->name);
    if (stream->priv->statistics) g_object_unref(stream->priv->statistics);
    if (stream->priv->intervals) g_object_unref(stream->priv->intervals);
    if (stream->priv->amount) g_object_unref(stream->priv->amount);

    G_OBJECT_CLASS(stream_parent_class)->finalize(object);
}

static void stream_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_STREAM(object));
    Stream *stream = STREAM(object);
    switch (prop_id) {
    case PROP_NAME:
        if (stream->priv->name) g_free(stream->priv->name);
        stream->priv->name = g_value_dup_string(value);
        break;
    case PROP_NUMBER:
        stream->priv->number = g_value_get_uint(value);
        break;
    case PROP_REMOTE:
        stream->priv->remote = g_value_get_boolean(value);
        break;
    case PROP_STATISTICS:
        if (stream->priv->statistics) g_object_unref(stream->priv->statistics);
        stream->priv->statistics = g_value_dup_object(value);
        break;
    case PROP_INTERVALS:
        if (stream->priv->intervals) g_object_unref(stream->priv->intervals);
        stream->priv->intervals = g_value_dup_object(value);
        break;
    case PROP_AMOUNT:
        if (stream->priv->amount) g_object_unref(stream->priv->amount);
        stream->priv->amount = g_value_dup_object(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void stream_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_STREAM(object));
    Stream *stream = STREAM(object);

    switch (prop_id) {
    case PROP_NAME:
        g_value_set_string(value, stream->priv->name);
        break;
    case PROP_NUMBER:
        g_value_set_uint(value, stream->priv->number);
        break;
    case PROP_REMOTE:
        g_value_set_boolean(value, stream->priv->remote);
        break;
    case PROP_STATISTICS:
        g_value_set_object(value, stream->priv->statistics);
        break;
    case PROP_INTERVALS:
        g_value_set_object(value, stream->priv->intervals);
        break;
    case PROP_AMOUNT:
        g_value_set_object(value, stream->priv->amount);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void stream_class_init(StreamClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    g_type_class_add_private(klass, sizeof(StreamPrivate));

    object_class->finalize     = stream_finalize;
    object_class->set_property = stream_set_property;
    object_class->get_property = stream_get_property;
    g_object_class_install_property(object_class, PROP_NAME, g_param_spec_string("name", "name", "name", "none", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_NUMBER, g_param_spec_uint("number", "number", "stream number", 1, 6, 1, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_REMOTE, g_param_spec_boolean("remote", "remote", "remote control", FALSE, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_INTERVALS, g_param_spec_object("intervals", "measurement intervals", "intervals", TYPE_INTERVALS, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_STATISTICS, g_param_spec_object("statistics", "measurement statistics", "statistics", TYPE_STATISTICS, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_AMOUNT, g_param_spec_object("amount", "measurement amount", "amount", TYPE_AMOUNT, G_PARAM_READABLE | G_PARAM_WRITABLE));

    // g_object_class_install_property(
    //     object_class, ONLINE_STATISTIC, g_param_spec_object("online-statistic", "name", "name", TYPE_STATISTIC, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    // g_object_class_install_property(object_class, PROP_UPDATED,
    //     g_param_spec_int64("updated", "updated", "updated time in msec", 0, G_MAXINT64, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
}

/**
 * StreamGetName:
 * @stream: a #Stream
 *
 * Get stream name.
 *
 * Returns: stream name as string
 */
const gchar *StreamGetName(Stream *stream) {
    g_return_val_if_fail(stream != NULL, NULL);
    g_return_val_if_fail(IS_STREAM(stream), NULL);
    return stream->priv->name;
}

/**
 * StreamGetNumber:
 * @stream: a #Stream
 *
 * Get stream name.
 *
 * Returns: stream number as uint
 */
guint StreamGetNumber(Stream *stream) {
    g_return_val_if_fail(stream != NULL, 0);
    return stream->priv->number;
}
/**
 * StreamGetStatistics:
 * @stream: a #Stream
 *
 * Get stream statistic.
 *
 * Returns: stream #Statistic not free it.
 */
Statistics *StreamGetStatistics(Stream *stream) {
    g_return_val_if_fail(stream != NULL, NULL);
    g_return_val_if_fail(IS_STREAM(stream), NULL);
    return stream->priv->statistics;
}

/**
 * StreamGetIntervals:
 * @stream: a #Stream
 *
 * Get stream intervals.
 *
 * Returns: stream #Intervals not free it.
 */
Intervals *StreamGetIntervals(Stream *stream) {
    g_return_val_if_fail(stream != NULL, NULL);
    g_return_val_if_fail(IS_STREAM(stream), NULL);
    return stream->priv->intervals;
}
/**
 * StreamGetAmount:
 * @stream: a #Stream
 *
 * Get stream amount parameters.
 *
 * Returns: stream #Amount not free it.
 */
Amount *StreamGetAmount(Stream *stream) {
    g_return_val_if_fail(stream != NULL, NULL);
    g_return_val_if_fail(IS_STREAM(stream), NULL);
    return stream->priv->amount;
}

/**
 * StreamGetRemote:
 * @stream: a #Stream
 *
 * Get remote stream parameters.
 *
 * Returns: stream #Remote not free it.
 */
gboolean StreamGetRemote(Stream *stream) {
    g_return_val_if_fail(stream != NULL, FALSE);
    g_return_val_if_fail(IS_STREAM(stream), FALSE);
    return stream->priv->remote;
}
