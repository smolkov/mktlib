/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * axis.h
 * Copyright (C) LAR 2017
 *
 */

#ifndef _AXIS_H_
#define _AXIS_H_

#include <glib-object.h>

#define TYPE_AXIS (axis_get_type())
#define AXIS(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_AXIS, Axis))
#define AXIS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_AXIS, AxisClass))
#define IS_AXIS(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_AXIS))
#define IS_AXIS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_AXIS))
#define AXIS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_AXIS, AxisClass))

typedef struct _AxisClass AxisClass;
typedef struct _Axis      Axis;

struct _AxisClass {
    GObjectClass parent_class;
};

struct _Axis {
    GObject  parent_instance;
    guint    max;
    guint    hold;
    guint    current;
    gboolean reverse;
};

GType axis_get_type(void) G_GNUC_CONST;

guint AxisGetMax(Axis *axis);
guint AxisGetHold(Axis *axis);
guint AxisGetCurrent(Axis *axis);

#endif /* _AXIS_H_ */
