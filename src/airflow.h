/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * airflow.h
 * Copyright (C) 2017 LAR
 *
 *
 */
#ifndef _AIRFLOW_H_
#define _AIRFLOW_H_

#include <glib-object.h>

#define TYPE_AIRFLOW (airflow_get_type())
#define AIRFLOW(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_AIRFLOW, Airflow))
#define AIRFLOW_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_AIRFLOW, AirflowClass))
#define IS_AIRFLOW(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_AIRFLOW))
#define IS_AIRFLOW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_AIRFLOW))
#define AIRFLOW_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_AIRFLOW, AirflowClass))

typedef struct _AirflowClass   AirflowClass;
typedef struct _Airflow        Airflow;
typedef struct _AirflowPrivate AirflowPrivate;

struct _AirflowClass {
    GObjectClass parent_class;
};

struct _Airflow {
    GObject         parent_instance;
    AirflowPrivate *priv;
};

GType    airflow_get_type(void) G_GNUC_CONST;

gboolean AirflowGetCritical(Airflow *airflow);
gdouble AirflowGetSoll(Airflow *airflow);
gdouble AirflowGetMaxCorrelation(Airflow *airflow);
gdouble AirflowGetDeviation(Airflow *airflow);
gdouble AirflowGetAdjustment(Airflow *airflow);
gdouble AirflowGetTimeout(Airflow *airflow);
gdouble AirflowGetThreshold(Airflow *airflow);

#endif /* _AIRFLOW_H_ */
