/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * sensor.h
 * Copyright (C) 2017 LAR
 *
 *
 */
#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <glib-object.h>

#define TYPE_SENSOR (sensor_get_type())
#define SENSOR(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_SENSOR, Sensor))
#define SENSOR_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_SENSOR, SensorClass))
#define IS_SENSOR(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_SENSOR))
#define IS_SENSOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_SENSOR))
#define SENSOR_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_SENSOR, SensorClass))

typedef struct _SensorClass   SensorClass;
typedef struct _Sensor        Sensor;
typedef struct _SensorPrivate SensorPrivate;

struct _SensorClass {
    GObjectClass parent_class;
};

struct _Sensor {
    GObject        parent_instance;
    SensorPrivate *priv;
};

GType sensor_get_type(void) G_GNUC_CONST;


gdouble SensorGetMinRange1(Sensor *sensor);
gdouble SensorGetMaxRange1(Sensor *sensor);
guint SensorGetModel(Sensor *sensor);
guint SensorGetState(Sensor *sensor);

#endif /* _SENSOR_H_ */
