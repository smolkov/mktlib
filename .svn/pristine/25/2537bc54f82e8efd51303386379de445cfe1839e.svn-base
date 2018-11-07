/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * channel.h
 * Copyright (C) 2017 LAR
 *
 *
 */

#ifndef __H_CHANNEL
#define __H_CHANNEL

#include <glib-object.h>
#include <glib.h>
#include <limit.h>

#define TYPE_CHANNEL (channel_get_type())
#define CHANNEL(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_CHANNEL, Channel))
#define CHANNEL_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_CHANNEL, ChannelClass))
#define IS_CHANNEL(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_CHANNEL))
#define IS_CHANNEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_CHANNEL))
#define CHANNEL_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_CHANNEL, ChannelClass))

typedef struct _ChannelClass   ChannelClass;
typedef struct _Channel        Channel;
typedef struct _ChannelPrivate ChannelPrivate;

struct _ChannelClass {
    GObjectClass parent_class;
};

struct _Channel {
    GObject         parent_instance;
    ChannelPrivate *priv;
};

GType channel_get_type(void) G_GNUC_CONST;

const gchar *ChannelName(Channel *channel);
const gchar *ChannelUnit(Channel *channel);
const gchar *ChannelAnalog(Channel *channel);
const gchar *ChannelSensor(Channel *channel);
const gchar *ChannelCheckAnalog(Channel *channel);
gdouble ChannelMin(Channel *channel);
gdouble ChannelMax(Channel *channel);
gdouble ChannelFactor(Channel *channel);
gboolean ChannelActivated(Channel *channel);
gboolean ChannelActivatedCal(Channel *channel);
gboolean ChannelCheck(Channel *channel);
Limit *ChannelGetLimit(Channel *channel);
Limit *ChannelGetCheckLimit(Channel *channel);
guint64 ChannelTrigger(Channel *channel);
guint64 ChannelReference(Channel *channel);

#endif /* __H_CHANNEL */

