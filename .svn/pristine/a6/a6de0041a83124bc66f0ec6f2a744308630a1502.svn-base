/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * temperatur.h
 * Copyright (C) 2017 LAR
 *
 *
 */
#ifndef _TEMPERATUR_H_
#define _TEMPERATUR_H_

#include <glib-object.h>

#define TYPE_TEMPERATUR (temperatur_get_type())
#define TEMPERATUR(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_TEMPERATUR, Temperatur))
#define TEMPERATUR_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_TEMPERATUR, TemperaturClass))
#define IS_TEMPERATUR(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_TEMPERATUR))
#define IS_TEMPERATUR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_TEMPERATUR))
#define TEMPERATUR_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_TEMPERATUR, TemperaturClass))

typedef struct _TemperaturClass   TemperaturClass;
typedef struct _Temperatur        Temperatur;
typedef struct _TemperaturPrivate TemperaturPrivate;

struct _TemperaturClass {
    GObjectClass parent_class;
};

struct _Temperatur {
    GObject         parent_instance;
    TemperaturPrivate *priv;
};

GType    temperatur_get_type(void) G_GNUC_CONST;

gboolean TemperaturIsCheckFurnace(Temperatur *temperatur);
gboolean TemperaturIsCheckHousing(Temperatur *temperatur);
gdouble  TemperaturGetSollFurnace(Temperatur *temperatur);
gdouble  TemperaturGetSollHousing(Temperatur *temperatur);
#endif /* _TEMPERATUR_H_ */
