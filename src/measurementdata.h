/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * larmeasurement.h
 * Copyright (C) LAR 2017
 *

 */

#ifndef _LAR_MEASUREMENT_DATA_H_
#define _LAR_MEASUREMENT_DATA_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define TYPE_LAR_MEASUREMENT_DATA (measurement_data_get_type())
#define LAR_MEASUREMENT_DATA(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_LAR_MEASUREMENT_DATA, LarMeasurementData))
#define LAR_MEASUREMENT_DATA_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_LAR_MEASUREMENT_DATA, LarMeasurementDataClass))
#define IS_LAR_MEASUREMENT_DATA(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_LAR_MEASUREMENT_DATA))
#define IS_LAR_MEASUREMENT_DATA_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_LAR_MEASUREMENT_DATA))
#define LAR_MEASUREMENT_DATA_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_LAR_MEASUREMENT_DATA, LarMeasurementDataClass))

typedef struct _LarMeasurementDataClass   LarMeasurementDataClass;
typedef struct _LarMeasurementData        LarMeasurementData;
typedef struct _LarMeasurementDataPrivate LarMeasurementDataPrivate;

struct _LarMeasurementDataClass {
    GObjectClass parent_class;
};

struct _LarMeasurementData {
    GObject                 parent_instance;
    LarMeasurementDataPrivate *priv;
};

GType measurement_data_get_type(void) G_GNUC_CONST;


guint64 LarMeasurementDataGetResult(LarMeasurementData *measurement);
gint64 LarMeasurementDataGetChanged(LarMeasurementData *measurement);
gdouble LarMeasurementDataGetValue(LarMeasurementData *measurement);
gdouble LarMeasurementDataGetRawValue(LarMeasurementData *measurement);
guint64 LarMeasurementDataGetSignal(LarMeasurementData *measurement);
guint LarMeasurementDataGetReplicate(LarMeasurementData *measurement);
gboolean LarMeasurementDataGetOutlier(LarMeasurementData *measurement);

    G_END_DECLS

#endif /* _LAR_MEASUREMENT_DATA_H_ */
