/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mkt-measurement-data.c
 * Copyright (C) sascha 2013 <sascha@sascha-VirtualBox>
 * 
mkt-measurement-data.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * mkt-measurement-data.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MKT_PARAMDOUBLE_MODEL_H_
#define _MKT_PARAMDOUBLE_MODEL_H_


#include "mkt-param-object.h"
#include "mkt-paramdouble.h"


G_BEGIN_DECLS

#define MKT_TYPE_PARAMDOUBLE_MODEL             (mkt_paramdouble_model_get_type ())
#define MKT_PARAMDOUBLE_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_PARAMDOUBLE_MODEL, MktParamdoubleObject))
#define MKT_PARAMDOUBLE_MODEL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), MKT_TYPE_PARAMDOUBLE_MODEL, MktParamdoubleObjectClass))
#define MKT_IS_PARAMDOUBLE_MODEL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_PARAMDOUBLE_MODEL))
#define MKT_IS_PARAMDOUBLE_MODEL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), MKT_TYPE_PARAMDOUBLE_MODEL))
#define MKT_PARAMDOUBLE_MODEL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), MKT_TYPE_PARAMDOUBLE_MODEL, MktParamdoubleObjectClass))

typedef struct _MktParamdoubleObjectClass   MktParamdoubleObjectClass;
typedef struct _MktParamdoubleObject        MktParamdoubleObject;
typedef struct _MktParamdoubleObjectPrivate MktParamdoubleObjectPrivate;


struct _MktParamdoubleObjectClass
{
	MktParamObjectClass parent_class;
};

struct _MktParamdoubleObject
{
	MktParamObject               parent_instance;
	MktParamdoubleObjectPrivate *priv;
};

GType                   mkt_paramdouble_model_get_type                 (void) G_GNUC_CONST;

G_END_DECLS

#endif /* _MKT_PARAM1_OBJECT_H_ */
