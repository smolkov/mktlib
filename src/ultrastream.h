/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * ultrastreams.h
 * Copyright (C) 2017 LAR
 *
 *
 */

#ifndef _ULTRA_STREAM_H_
#define _ULTRA_STREAM_H_

#include <channel.h>
#include <posparm.h>
#include <glib.h>
#include <stream.h>

#define TYPE_ULTRA_STREAM (ultra_stream_get_type())
#define ULTRA_STREAM(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_ULTRA_STREAM, UltraStream))
#define ULTRA_STREAM_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_ULTRA_STREAM, UltraStreamClass))
#define IS_ULTRA_STREAM(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_ULTRA_STREAM))
#define IS_ULTRA_STREAM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_ULTRA_STREAM))
#define ULTRA_STREAM_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_ULTRA_STREAM, UltraStreamClass))

typedef struct _UltraStreamClass UltraStreamClass;
typedef struct _UltraStream      UltraStream;

GType ultra_stream_get_type(void) G_GNUC_CONST;

guint UltraStreamGetVolume(UltraStream *ultraStream);
guint UltraStreamGetInjectionVolume(UltraStream *ultraStream);
guint UltraStreamGetInjectionVolumeTic(UltraStream *ultraStream);
gdouble UltraStreamGetFillingTime(UltraStream *ultraStream);
gdouble UltraStreamGetDelay(UltraStream *ultraStream);
gdouble UltraStreamGetDelayTIC(UltraStream *ultraStream);
gboolean UltraStreamGetPreRinsing(UltraStream *ultraStream);
guint UltraStreamGetRinsingCount(UltraStream *ultraStream);
gboolean UltraStreamGetAfterRinsing(UltraStream *ultraStream);
guint UltraStreamGetAfterCount(UltraStream *ultraStream);
gboolean UltraStreamGetCODoInjection(UltraStream *ultraStream);
gboolean UltraStreamGetNeedStripping(UltraStream *ultraStream);
gdouble UltraStreamGetStrippingTime(UltraStream *ultraStream);
guint UltraStreamGetDilutionType(UltraStream *ultraStream);
gdouble UltraStreamGetDilutionFactor(UltraStream *ultraStream);
gdouble UltraStreamGetDilutionPumpTime(UltraStream *ultraStream);
gdouble UltraStreamGetDilutionWaitTime(UltraStream *ultraStream);
gdouble UltraStreamGetAmountDeviation(UltraStream *ultraStream);
gdouble UltraStreamGetAutocalDeviation(UltraStream *ultraStream);
gboolean UltraStreamGetPRinsingON(UltraStream *ultraStream);
guint UltraStreamGetPRinsingY1(UltraStream *ultraStream);
guint UltraStreamGetPRinsingY2(UltraStream *ultraStream);
guint UltraStreamGetPRinsingInjVol(UltraStream *ultraStream);
guint UltraStreamGetPRinsingInjReplicates(UltraStream *ultraStream);
gdouble UltraStreamGetPrinsingWaitAfter(UltraStream *ultraStream);

Channel* UltraStreamGetTocChannel(UltraStream *ultraStream);
Channel* UltraStreamGetTCChannel(UltraStream *ultraStream);
Channel* UltraStreamGetTicChannel(UltraStream *ultraStream);
Channel* UltraStreamGetTnbChannel(UltraStream *ultraStream);
Channel* UltraStreamGetCODoChannel(UltraStream *ultraStream);
Posparm* UltraStreamGetVesselsPos(UltraStream *ultraStream);



#endif /* _ULTRA_STREAM_H_ */
