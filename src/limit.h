/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * limit.h
 * Copyright (C) LAR 2017
 *

 */

#ifndef _LIMIT_H_
#define _LIMIT_H_

#include <glib-object.h>


#define TYPE_LIMIT             (limit_get_type ())
#define LIMIT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_LIMIT, Limit))
#define LIMIT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_LIMIT, LimitClass))
#define IS_LIMIT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_LIMIT))
#define IS_LIMIT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_LIMIT))
#define LIMIT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_LIMIT, LimitClass))

typedef struct _LimitClass   LimitClass;
typedef struct _Limit        Limit;
typedef struct _LimitPrivate LimitPrivate;


struct _LimitClass
{
	GObjectClass parent_class;
};

struct _Limit
{
	GObject parent_instance;
	LimitPrivate *priv;
};

GType                   limit_get_type                 (void) G_GNUC_CONST;

gdouble LimitGetMin(Limit *limit);
gdouble LimitGetMax(Limit *limit);
gboolean LimitGetActivated(Limit *limit);

#endif /* _LIMIT_H_ */
