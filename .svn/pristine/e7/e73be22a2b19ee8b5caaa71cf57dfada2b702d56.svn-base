/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * statistic.h
 * Copyright (C) LAR 2017 
 *

 */

#ifndef _STATISTIC_H_
#define _STATISTIC_H_

#include <glib-object.h>


#define TYPE_STATISTIC (statistic_get_type())
#define STATISTIC(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_STATISTIC, Statistic))
#define STATISTIC_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_STATISTIC, StatisticClass))
#define IS_STATISTIC(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_STATISTIC))
#define IS_STATISTIC_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_STATISTIC))
#define STATISTIC_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_STATISTIC, StatisticClass))

typedef struct _StatisticClass   StatisticClass;
typedef struct _Statistic        Statistic;

GType statistic_get_type(void) G_GNUC_CONST;


guint StatisticGetReplicates(Statistic *statistic);
guint StatisticGetOutliers(Statistic *statistic);
gdouble StatisticGetCV(Statistic *statistic);
gdouble StatisticGetThreshold(Statistic *statistic);
gdouble StatisticGetJump(Statistic *statistic);
guint StatisticGetMeasurementNumber(Statistic *statistic);
void StatisticSetMeasurementNumber(Statistic *statistic,guint number);
#endif /* _STATISTIC_H_ */
