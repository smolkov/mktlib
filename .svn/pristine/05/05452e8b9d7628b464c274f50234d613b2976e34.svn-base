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

#ifndef _MKT_PARAMUINT32_MODEL_H_
#define _MKT_PARAMUINT32_MODEL_H_


#include "mkt-param-object.h"
#include "mkt-paramuint32.h"


G_BEGIN_DECLS

#define MKT_TYPE_PARAMUINT32_MODEL             (mkt_paramuint32_model_get_type ())
#define MKT_PARAMUINT32_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_PARAMUINT32_MODEL, MktParamuint32Object))
#define MKT_PARAMUINT32_MODEL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), MKT_TYPE_PARAMUINT32_MODEL, MktParamuint32ObjectClass))
#define MKT_IS_PARAMUINT32_MODEL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_PARAMUINT32_MODEL))
#define MKT_IS_PARAMUINT32_MODEL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), MKT_TYPE_PARAMUINT32_MODEL))
#define MKT_PARAMUINT32_MODEL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), MKT_TYPE_PARAMUINT32_MODEL, MktParamuint32ObjectClass))

typedef struct _MktParamuint32ObjectClass   MktParamuint32ObjectClass;
typedef struct _MktParamuint32Object        MktParamuint32Object;
typedef struct _MktParamuint32ObjectPrivate MktParamuint32ObjectPrivate;


struct _MktParamuint32ObjectClass
{
	MktParamObjectClass          parent_class;
};

struct _MktParamuint32Object
{
	MktParamObject               parent_instance;
	MktParamuint32ObjectPrivate  *priv;
};

GType                   mkt_paramuint32_model_get_type                 (void) G_GNUC_CONST;

G_END_DECLS

#endif /* _MKT_PARAM1_OBJECT_H_ */
