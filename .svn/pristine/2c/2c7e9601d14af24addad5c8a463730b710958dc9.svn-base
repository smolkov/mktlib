/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * intervals.h
 * Copyright (C) LAR 2017 
 *

 */

#ifndef _INTERVALS_H_
#define _INTERVALS_H_

#include <glib-object.h>
#include <interval.h>


#define TYPE_INTERVALS (intervals_get_type())
#define INTERVALS(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_INTERVALS, Intervals))
#define INTERVALS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_INTERVALS, IntervalsClass))
#define IS_INTERVALS(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_INTERVALS))
#define IS_INTERVALS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_INTERVALS))
#define INTERVALS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_INTERVALS, IntervalsClass))

typedef struct _IntervalsClass   IntervalsClass;
typedef struct _Intervals        Intervals;

GType intervals_get_type(void) G_GNUC_CONST;


Interval* IntervalsGetOnline(Intervals *intervals);
Interval* IntervalsGetCalibration(Intervals *intervals);
Interval* IntervalsGetCheck(Intervals *intervals);


#endif /* _INTERVALS_H_ */
