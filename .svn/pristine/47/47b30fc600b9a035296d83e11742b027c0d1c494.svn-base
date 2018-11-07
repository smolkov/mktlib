/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * streams.h
 * Copyright (C) 2017 LAR
 *
 *
 */

#ifndef _STREAM_H_
#define _STREAM_H_

#include <glib.h>
#include <intervals.h>
#include <statistics.h>
#include <amount.h>

#define TYPE_STREAM (stream_get_type())
#define STREAM(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_STREAM, Stream))
#define STREAM_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_STREAM, StreamClass))
#define IS_STREAM(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_STREAM))
#define IS_STREAM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_STREAM))
#define STREAM_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_STREAM, StreamClass))

typedef struct _StreamClass   StreamClass;
typedef struct _Stream        Stream;
typedef struct _StreamPrivate StreamPrivate;



struct _StreamClass {
    GObjectClass parent_class;
};

struct _Stream {
    GObject     parent_instance;
    StreamPrivate *priv;
};

GType stream_get_type(void) G_GNUC_CONST;

const gchar *StreamGetName(Stream *stream);
guint StreamGetNumber(Stream *stream);
Statistics *StreamGetStatistics(Stream *stream);
Intervals *StreamGetIntervals(Stream *stream);
Amount *StreamGetAmount(Stream *stream);
gboolean StreamGetRemote(Stream *stream);

#endif /* _STREAM_H_ */
