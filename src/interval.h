/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * interval.h
 * Copyright (C) LAR 2017 
 *

 */

#ifndef _INTERVAL_H_
#define _INTERVAL_H_

#include <glib-object.h>


#define TYPE_INTERVAL (interval_get_type())
#define INTERVAL(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_INTERVAL, Interval))
#define INTERVAL_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_INTERVAL, IntervalClass))
#define IS_INTERVAL(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_INTERVAL))
#define IS_INTERVAL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_INTERVAL))
#define INTERVAL_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_INTERVAL, IntervalClass))

typedef struct _IntervalClass   IntervalClass;
typedef struct _Interval        Interval;

GType interval_get_type(void) G_GNUC_CONST;


gint64 IntervalGetInterval(Interval *interval);
gboolean IntervalGetActivated(Interval *interval);


#endif /* _INTERVAL_H_ */
