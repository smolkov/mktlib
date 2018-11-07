/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * ultrameasurement.h
 * Copyright (C) LAR 2017
 *

 */

#ifndef _ULTRA_MEASUREMENT_H_
#define _ULTRA_MEASUREMENT_H_

#include <glib.h>
#include "ultrameas.h"
#include "statistic.h"

G_BEGIN_DECLS

typedef struct _UmeasArrayBoxed UmeasArrayBoxed;

#define UMEASARRAY_TYPE_BOXED (umeasarray_boxed_get_type())

GType umeasarray_boxed_get_type(void) G_GNUC_CONST;

void UmeasArrayBoxedFree(UmeasArrayBoxed *boxed);
UmeasArrayBoxed *UmeasArrayBoxedCopy(const UmeasArrayBoxed *src);
UmeasArrayBoxed *UmeasArrayBoxedNew();
gboolean UmeasArrayAddObject(UmeasArrayBoxed *boxed, Ultrameas *newMes);
GSList *UmeasArrayGetDatas(UmeasArrayBoxed *boxed);

#define TYPE_ULTRA_MEASUREMENT (ultra_measurement_get_type())
#define ULTRA_MEASUREMENT(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_ULTRA_MEASUREMENT, UltraMeasurement))
#define ULTRA_MEASUREMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_ULTRA_MEASUREMENT, UltraMeasurementClass))
#define IS_ULTRA_MEASUREMENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_ULTRA_MEASUREMENT))
#define IS_ULTRA_MEASUREMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_ULTRA_MEASUREMENT))
#define ULTRA_MEASUREMENT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_ULTRA_MEASUREMENT, UltraMeasurementClass))

typedef struct _UltraMeasurementClass UltraMeasurementClass;
typedef struct _UltraMeasurement UltraMeasurement;
typedef struct _UltraMeasurementPrivate UltraMeasurementPrivate;

struct _UltraMeasurementClass
{
    GObjectClass parent_class;
};

struct _UltraMeasurement
{
    GObject parent_instance;
    UltraMeasurementPrivate *priv;
};

GType ultra_measurement_get_type(void) G_GNUC_CONST;

UltraMeasurement * UltraMeasurementFromStatistic(guint64,Statistic *statistic);

GSList *m_UltraMeasurementGetMeasurements(UltraMeasurement *ultraMeas);
guint64 m_UltraMeasurementGetChannel(UltraMeasurement *ultraMeas);
guint64 m_UltraMeasurementGetResult(UltraMeasurement *ultraMeas);
guint m_UltraMeasurementGetNumber(UltraMeasurement *ultraMeas);
guint m_UltraMeasurementGetReplicates(UltraMeasurement *ultraMeas);
guint m_UltraMeasurementGetOutliers(UltraMeasurement *ultraMeas);
gdouble m_UltraMeasurementGetValue(UltraMeasurement *ultraMeas);
gdouble m_UltraMeasurementGetCv(UltraMeasurement *ultraMeas);
gdouble m_UltraMeasurementGetMaxCv(UltraMeasurement *ultraMeas);
gboolean m_UltraMeasurementAddMeas(UltraMeasurement *ultraMeas, Ultrameas *meas);
gboolean m_UltraMeasurementCalculate(UltraMeasurement *ultraMeas);


G_END_DECLS

#endif /* _ULTRA_MEASUREMENT_H_ */