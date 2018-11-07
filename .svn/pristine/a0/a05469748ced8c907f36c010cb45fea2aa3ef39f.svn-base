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


#include "market-time.h"
#include "mkt-category-model.h"
#include "mkt-model.h"
#include "mkt-statistic-model.h"


#define GETTEXT_PACKAGE "mkt"
#include <glib/gi18n-lib.h>



enum
{
	PROP_0,
	PROP_REMOTE_CONTROL,
	PROP_INTERVAL,
	PROP_ONLINE,
	PROP_RUN_COUNTER,
	PROP_RUN_STARTTIME

};


struct _MktCategoryModelPrivate
{
	guint            run_counter;
	gboolean         remote_control;
	gdouble          interval;
	gboolean         category_online;
	gdouble          category_starttime;


};


#define MKT_CATEGORY_MODEL_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), MKT_TYPE_CATEGORY_MODEL, MktCategoryModelPrivate))

static gdouble
mkt_category_model_interval                              ( MktCategory *category )
{
	g_return_val_if_fail(category != NULL,0.);
	g_return_val_if_fail(MKT_IS_CATEGORY_MODEL(category),0.);
	return MKT_CATEGORY_MODEL(category)->priv->interval;
}
static void
mkt_category_model_init_interface ( MktCategoryInterface *iface )
{
	iface->interval =mkt_category_model_interval;
}

G_DEFINE_TYPE_WITH_CODE (MktCategoryModel, mkt_category_model, MKT_TYPE_PARAM_OBJECT,
											   G_IMPLEMENT_INTERFACE (MKT_TYPE_CATEGORY,
				                               mkt_category_model_init_interface)
											   MKT_CREATE_MODEL_FULL(mkt_connection_new_default_parameter(TRUE)))


static void
mkt_category_model_init (MktCategoryModel *mkt_category_model)
{
	MktCategoryModelPrivate *priv      = MKT_CATEGORY_MODEL_PRIVATE(mkt_category_model);
	priv->interval                      = 0.0;
	priv->remote_control                = FALSE;
	priv->category_online               = FALSE;
	priv->category_starttime            = 0.0;
	mkt_category_model->priv            = priv;
	/* TODO: Add initialization code here */
}

static void
mkt_category_model_finalize (GObject *object)
{
	//MktCategoryModel *data = MKT_CATEGORY_MODEL(object);
	G_OBJECT_CLASS (mkt_category_model_parent_class)->finalize (object);
}


static void
mkt_category_model_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_CATEGORY_MODEL (object));
	MktCategoryModel *data = MKT_CATEGORY_MODEL(object);
	switch (prop_id)
	{

	case PROP_REMOTE_CONTROL:
		data->priv->remote_control = g_value_get_boolean(value);
		break;
	case PROP_INTERVAL:
		data->priv->interval = g_value_get_double(value);
		break;
	case PROP_ONLINE:
		data->priv->category_online = g_value_get_boolean(value);
		break;
	case PROP_RUN_COUNTER:
		data->priv->run_counter = g_value_get_uint(value);
		break;
	case PROP_RUN_STARTTIME:
		data->priv->category_starttime = g_value_get_double(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_category_model_get_property ( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec )
{
	g_return_if_fail (MKT_IS_CATEGORY_MODEL (object));
	MktCategoryModel *data = MKT_CATEGORY_MODEL(object);
	switch (prop_id)
	{
	case PROP_REMOTE_CONTROL:
		g_value_set_boolean(value , data->priv->remote_control);
		break;
	case PROP_INTERVAL:
		g_value_set_double(value , data->priv->interval);
		break;
	case PROP_ONLINE:
		g_value_set_boolean(value , data->priv->category_online);
		break;
	case PROP_RUN_COUNTER:
		g_value_set_uint(value , data->priv->run_counter);
		break;
	case PROP_RUN_STARTTIME:
		g_value_set_double(value , data->priv->category_starttime);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_category_model_class_init (MktCategoryModelClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//MktModelClass* parent_class = MKT_MODEL_CLASS (klass);
	g_type_class_add_private (klass, sizeof (MktCategoryModelPrivate));
	object_class->finalize = mkt_category_model_finalize;
	object_class->set_property = mkt_category_model_set_property;
	object_class->get_property = mkt_category_model_get_property;

	g_object_class_override_property(object_class,PROP_INTERVAL,"category-interval");
	g_object_class_override_property(object_class,PROP_REMOTE_CONTROL,"remote-control");
	g_object_class_override_property(object_class,PROP_ONLINE,"category-online");
	g_object_class_override_property(object_class,PROP_RUN_COUNTER,"category-runs");
	g_object_class_override_property(object_class,PROP_RUN_STARTTIME,"category-starttime");
}

