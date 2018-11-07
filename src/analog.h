/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * analog.h
 * Copyright (C) 2017 LAR
 *
 *
 */
#ifndef _ANALOG_H_
#define _ANALOG_H_

#include <glib-object.h>

#define TYPE_ANALOG (analog_get_type())
#define ANALOG(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_ANALOG, Analog))
#define ANALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_ANALOG, AnalogClass))
#define IS_ANALOG(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_ANALOG))
#define IS_ANALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_ANALOG))
#define ANALOG_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_ANALOG, AnalogClass))

typedef struct _AnalogClass   AnalogClass;
typedef struct _Analog        Analog;
typedef struct _AnalogPrivate AnalogPrivate;

struct _AnalogClass {
    GObjectClass parent_class;
};

struct _Analog {
    GObject        parent_instance;
    AnalogPrivate *priv;
};

GType analog_get_type(void) G_GNUC_CONST;


gdouble AnalogGetMin(Analog *analog);
gdouble AnalogGetMax(Analog *analog);
const gchar* AnalogGetBoolexpr(Analog *analog);
gboolean AnalogGetLifeZero(Analog *analog);


#endif /* _ANALOG_H_ */
