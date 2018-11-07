/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * pressure.h
 * Copyright (C) 2017 LAR
 *
 *
 */
#ifndef _PRESSURE_H_
#define _PRESSURE_H_

#include <glib-object.h>

#define TYPE_PRESSURE (pressure_get_type())
#define PRESSURE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_PRESSURE, Pressure))
#define PRESSURE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_PRESSURE, PressureClass))
#define IS_PRESSURE(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_PRESSURE))
#define IS_PRESSURE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_PRESSURE))
#define PRESSURE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_PRESSURE, PressureClass))

typedef struct _PressureClass   PressureClass;
typedef struct _Pressure        Pressure;
typedef struct _PressurePrivate PressurePrivate;

struct _PressureClass {
    GObjectClass parent_class;
};

struct _Pressure {
    GObject         parent_instance;
    PressurePrivate *priv;
};

GType    pressure_get_type(void) G_GNUC_CONST;

gboolean PressureGetCritical(Pressure *pressure);
gdouble PressureGetSoll(Pressure *pressure);

#endif /* _PRESSURE_H_ */
