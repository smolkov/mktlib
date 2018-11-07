/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * statistic.h
 * Copyright (C) LAR 2017 
 *

 */

#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include <glib-object.h>
#include <statistic.h>


#define TYPE_STATISTICS (statistics_get_type())
#define STATISTICS(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_STATISTICS, Statistics))
#define STATISTICS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_STATISTICS, StatisticsClass))
#define IS_STATISTICS(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_STATISTICS))
#define IS_STATISTICS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_STATISTICS))
#define STATISTICS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_STATISTICS, StatisticsClass))

typedef struct _StatisticsClass   StatisticsClass;
typedef struct _Statistics        Statistics;

GType statistics_get_type(void) G_GNUC_CONST;

Statistic *StatisticsGetOnline(Statistics *statistics);
Statistic *StatisticsGetSingle(Statistics *statistics);
Statistic *StatisticsGetCalibration(Statistics *statistics);
Statistic *StatisticsGetCheck(Statistics *statistics);

#endif /* _STATISTICS_H_ */
