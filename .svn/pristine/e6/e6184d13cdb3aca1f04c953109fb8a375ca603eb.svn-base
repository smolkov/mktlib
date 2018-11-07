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

#ifndef _MKT_STATUS_MODEL_H_
#define _MKT_STATUS_MODEL_H_

#include "mkt-status.h"
#include "mkt-param-object.h"



G_BEGIN_DECLS

#define MKT_TYPE_STATUS_MODEL             (mkt_status_model_get_type ())
#define MKT_STATUS_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_STATUS_MODEL, MktStatusModel))
#define MKT_STATUS_MODEL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), MKT_TYPE_STATUS_MODEL, MktStatusModelClass))
#define MKT_IS_STATUS_MODEL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_STATUS_MODEL))
#define MKT_IS_STATUS_MODEL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), MKT_TYPE_STATUS_MODEL))
#define MKT_STATUS_MODEL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), MKT_TYPE_STATUS_MODEL, MktStatusModelClass))

typedef struct _MktStatusModelClass        MktStatusModelClass;
typedef struct _MktStatusModel             MktStatusModel;
typedef struct _MktStatusModelPrivate      MktStatusModelPrivate;


struct _MktStatusModelClass
{
	MktModelClass                   parent_class;
};

struct _MktStatusModel
{
	MktModel                        parent_instance;
	MktStatusModelPrivate          *priv;
};

GType                              mkt_status_model_get_type                 (void) G_GNUC_CONST;

G_END_DECLS

#endif /* _MKT_STATUS_MODEL_H_ */
