/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * ultrapoint.h
 * Copyright (C) LAR 2017
 *

 */

#ifndef _ULTRA_POINT_H_
#define _ULTRA_POINT_H_

#include <glib.h>
#include "larpoint.h"
#include "ultrameas.h"
G_BEGIN_DECLS

#define TYPE_ULTRA_POINT (ultra_point_get_type())
#define ULTRA_POINT(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_ULTRA_POINT, UltraPoint))
#define ULTRA_POINT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_ULTRA_POINT, UltraPointClass))
#define IS_ULTRA_POINT(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_ULTRA_POINT))
#define IS_ULTRA_POINT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_ULTRA_POINT))
#define ULTRA_POINT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_ULTRA_POINT, UltraPointClass))

typedef struct _UltraPointClass UltraPointClass;
typedef struct _UltraPoint UltraPoint;
typedef struct _UltraPointPrivate UltraPointPrivate;

struct _UltraPointClass
{
    LarPointClass parent_class;
};

struct _UltraPoint
{
    LarPoint parent_instance;
    UltraPointPrivate *priv;
};

GType ultra_point_get_type(void) G_GNUC_CONST;

GSList *m_UltraPointGetPoints(UltraPoint *upoint);
gboolean m_UltraPointAddMeas(UltraPoint *upoint, Ultrameas *meas);
gboolean m_UltraPointCalculate(UltraPoint *upoint);

G_END_DECLS

#endif /* _ULTRA_POINT_H_ */