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

#ifndef _MKT_CAL_DATA_MODEL_H_
#define _MKT_CAL_DATA_MODEL_H_

#include "mkt-model.h"
#include "mkt-cal-data.h"


G_BEGIN_DECLS

#define MKT_TYPE_CAL_DATA_MODEL             (mkt_cal_data_model_get_type ())
#define MKT_CAL_DATA_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_CAL_DATA_MODEL, MktCalDataModel))
#define MKT_CAL_DATA_MODEL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass),  MKT_TYPE_CAL_DATA_MODEL, MktCalDataModelClass))
#define MKT_IS_CAL_DATA_MODEL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_CAL_DATA_MODEL))
#define MKT_IS_CAL_DATA_MODEL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass),  MKT_TYPE_CAL_DATA_MODEL))
#define MKT_CAL_DATA_MODEL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj),  MKT_TYPE_CAL_DATA_MODEL, MktCalDataModelClass))

typedef struct _MktCalDataModelClass     MktCalDataModelClass;
typedef struct _MktCalDataModel          MktCalDataModel;
typedef struct _MktCalDataModelPrivate   MktCalDataModelPrivate;


struct _MktCalDataModelClass
{
	MktModelClass                           parent_class;
};

struct _MktCalDataModel
{
	MktModel                              parent_instance;
	MktCalDataModelPrivate     *priv;
};

GType                   mkt_cal_data_model_get_type                 (void) G_GNUC_CONST;

G_END_DECLS

#endif /* _MKT_CAL_DATA_MODEL_H_ */
