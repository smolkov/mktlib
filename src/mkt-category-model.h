/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mkt-sensor-data.c
 * Copyright (C) sascha 2013 <sascha@sascha-VirtualBox>
 * 
mkt-sensor-data.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * mkt-sensor-data.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MKT_CATEGORY_MODEL_H_
#define _MKT_CATEGORY_MODEL_H_

#include "mkt-model.h"
#include "mkt-category.h"
#include "mkt-param-object.h"


#define MKT_TYPE_CATEGORY_MODEL             (mkt_category_model_get_type ())
#define MKT_CATEGORY_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_CATEGORY_MODEL,  MktCategoryModel))
#define MKT_CATEGORY_MODEL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass),  MKT_TYPE_CATEGORY_MODEL,  MktCategoryModelClass))
#define MKT_IS_CATEGORY_MODEL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_CATEGORY_MODEL))
#define MKT_IS_CATEGORY_MODEL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass),  MKT_TYPE_CATEGORY_MODEL))
#define MKT_CATEGORY_MODEL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj),  MKT_TYPE_CATEGORY_MODEL,  MktCategoryModelClass))

typedef struct _MktCategoryModelClass MktCategoryModelClass;
typedef struct _MktCategoryModel MktCategoryModel;
typedef struct _MktCategoryModelPrivate MktCategoryModelPrivate;


struct _MktCategoryModelClass
{
	MktParamObjectClass                parent_class;
};

struct _MktCategoryModel
{
	MktParamObject                     parent_instance;
	MktCategoryModelPrivate           *priv;
};

GType                   mkt_category_model_get_type                 (void) G_GNUC_CONST;


#endif /* _MKT_CATEGORY_MODEL_H_ */
