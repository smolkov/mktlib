/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * humidity.h
 * Copyright (C) 2017 LAR
 *
 *
 */
#ifndef _HUMIDITY_H_
#define _HUMIDITY_H_

#include <glib-object.h>

#define TYPE_HUMIDITY (humidity_get_type())
#define HUMIDITY(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_HUMIDITY, Humidity))
#define HUMIDITY_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_HUMIDITY, HumidityClass))
#define IS_HUMIDITY(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_HUMIDITY))
#define IS_HUMIDITY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_HUMIDITY))
#define HUMIDITY_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_HUMIDITY, HumidityClass))

typedef struct _HumidityClass   HumidityClass;
typedef struct _Humidity        Humidity;
typedef struct _HumidityPrivate HumidityPrivate;

struct _HumidityClass {
    GObjectClass parent_class;
};

struct _Humidity {
    GObject         parent_instance;
    HumidityPrivate *priv;
};

GType    humidity_get_type(void) G_GNUC_CONST;

gboolean HumidityGetCritical(Humidity *humidity);
gdouble HumidityGetSoll(Humidity *humidity);

#endif /* _HUMIDITY_H_ */
