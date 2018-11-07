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

#ifndef _MKT_VESSEL_MODEL_H_
#define _MKT_VESSEL_MODEL_H_

#include "mkt-model.h"
#include "mkt-vessel.h"
#include "mkt-param-object.h"


G_BEGIN_DECLS

#define MKT_TYPE_VESSEL_MODEL             (mkt_vessel_model_get_type ())
#define MKT_VESSEL_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_VESSEL_MODEL, MktVesselModel))
#define MKT_VESSEL_MODEL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass),  MKT_TYPE_VESSEL_MODEL, MktVesselModelClass))
#define MKT_IS_VESSEL_MODEL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_VESSEL_MODEL))
#define MKT_IS_VESSEL_MODEL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass),  MKT_TYPE_VESSEL_MODEL))
#define MKT_VESSEL_MODEL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj),  MKT_TYPE_VESSEL_MODEL, MktVesselModelClass))

typedef struct _MktVesselModelClass        MktVesselModelClass;
typedef struct _MktVesselModel             MktVesselModel;
typedef struct _MktVesselModelPrivate      MktVesselModelPrivate;


struct _MktVesselModelClass
{
	MktParamObjectClass parent_class;
};

struct _MktVesselModel
{
	MktParamObject           parent_instance;
	MktVesselModelPrivate  *priv;
};

GType                   mkt_vessel_model_get_type                 (void) G_GNUC_CONST;

G_END_DECLS

#endif /* _MKT_VESSEL_MODEL_H_ */
