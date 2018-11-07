/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * larpoint.h
 * Copyright (C) LAR 2017
 * autor Smolkov
 */

#ifndef _LAR_POINT_H_
#define _LAR_POINT_H_

#include <glib-object.h>


G_BEGIN_DECLS

#define TYPE_LAR_POINT             (lar_point_get_type ())
#define LAR_POINT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_LAR_POINT, LarPoint))
#define LAR_POINT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass),  TYPE_LAR_POINT, LarPointClass))
#define IS_LAR_POINT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_LAR_POINT))
#define IS_LAR_POINT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass),  TYPE_LAR_POINT))
#define LAR_POINT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj),  TYPE_LAR_POINT, LarPointClass))

typedef struct _LarPointClass     LarPointClass;
typedef struct _LarPoint          LarPoint;
typedef struct _LarPointPrivate   LarPointPrivate;


struct _LarPointClass
{
	GObjectClass      parent_class;
};

struct _LarPoint
{
	GObject           parent_instance;
	LarPointPrivate  *priv;
};

GType                   lar_point_get_type                 (void) G_GNUC_CONST;
gdouble m_LarPointGetSolution(LarPoint *point);
gboolean m_LarPointGetAverage(LarPoint *point );
gboolean m_LarPointGetCv(LarPoint *point);
G_END_DECLS

#endif /* _LAR_POINT_H_ */
