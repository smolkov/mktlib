/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * larmeasurement.h
 * Copyright (C) LAR 2017
 *

 */

#ifndef _LAR_MEASUREMENT_H_
#define _LAR_MEASUREMENT_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define TYPE_LAR_MEASUREMENT (lar_measurement_get_type())
#define LAR_MEASUREMENT(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_LAR_MEASUREMENT, LarMeasurement))
#define LAR_MEASUREMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_LAR_MEASUREMENT, LarMeasurementClass))
#define IS_LAR_MEASUREMENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_LAR_MEASUREMENT))
#define IS_LAR_MEASUREMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_LAR_MEASUREMENT))
#define LAR_MEASUREMENT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_LAR_MEASUREMENT, LarMeasurementClass))

typedef struct _LarMeasurementClass   LarMeasurementClass;
typedef struct _LarMeasurement        LarMeasurement;
typedef struct _LarMeasurementPrivate LarMeasurementPrivate;






       

struct _LarMeasurementClass {
    GObjectClass parent_class;
};

struct _LarMeasurement {
    GObject                 parent_instance;
    LarMeasurementPrivate *priv;
};

GType lar_measurement_get_type(void) G_GNUC_CONST;


guint64 m_LarMeasurementGetResult(LarMeasurement *measurement);
gint64 m_LarMeasurementGetChanged(LarMeasurement *measurement);
gdouble m_LarMeasurementGetValue(LarMeasurement *measurement);
gdouble m_LarMeasurementGetRawValue(LarMeasurement *measurement);
guint64 m_LarMeasurementGetSignal(LarMeasurement *measurement);
guint m_LarMeasurementGetReplicate(LarMeasurement *measurement);
gboolean m_LarMeasurementGetOutlier(LarMeasurement *measurement);

    G_END_DECLS

#endif /* _LAR_MEASUREMENT_H_ */
