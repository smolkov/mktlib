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

#include "mkt-cal-data-model.h"
#include "market-time.h"



enum
{
	PROP_0,
	PROP_MKT_CAL_DATA_POINT,
	PROP_MKT_CAL_DATA_MEASURED,
	PROP_MKT_CAL_DATA_TRIGGER,
	PROP_MKT_CAL_DATA_VALUE,
	PROP_MKT_CAL_DATA_OUTLIER,
	PROP_MKT_CAL_DATA_REPLICATE
};



struct _MktCalDataModelPrivate
{
	guint64     point;
	guint       trigger;
	gdouble     value;
	gboolean    outlier;
	guint       replicate;
};


#define MKT_CAL_DATA_MODEL_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), MKT_TYPE_CAL_DATA_MODEL, MktCalDataModelPrivate))



static gdouble
mkt_cal_data_model_value                          ( MktCalData *ucal )
{
	g_return_val_if_fail(ucal != NULL,0.);
	g_return_val_if_fail(MKT_IS_CAL_DATA(ucal),0.);
	return MKT_CAL_DATA_MODEL(ucal)->priv->value;
}

static gboolean
mkt_cal_data_model_average                                     ( MktCalData *ucal )
{
	g_return_val_if_fail(ucal != NULL,TRUE);
	g_return_val_if_fail(MKT_IS_CAL_DATA(ucal),TRUE);
	return MKT_CAL_DATA_MODEL(ucal)->priv->outlier;
}

static guint
mkt_cal_data_model_trigger                                    ( MktCalData *ucal )
{
	g_return_val_if_fail(ucal != NULL,0);
	g_return_val_if_fail(MKT_IS_CAL_DATA(ucal),0);
	return MKT_CAL_DATA_MODEL(ucal)->priv->trigger;
}

static guint
mkt_cal_data_model_replicate                                    ( MktCalData *ucal )
{
	g_return_val_if_fail(ucal != NULL,0);
	g_return_val_if_fail(MKT_IS_CAL_DATA(ucal),0);
	return MKT_CAL_DATA_MODEL(ucal)->priv->replicate;
}
static guint64
mkt_cal_data_model_point                                    ( MktCalData *ucal )
{
	g_return_val_if_fail(ucal != NULL,0);
	g_return_val_if_fail(MKT_IS_CAL_DATA(ucal),0);
	return MKT_CAL_DATA_MODEL(ucal)->priv->point;
}

static void
mkt_cal_data_model_init_stream_interface ( MktCalDataInterface *iface )
{
	iface->cal_data_value=mkt_cal_data_model_value;
	iface->cal_data_outlier=mkt_cal_data_model_average;
	iface->cal_data_trigger=mkt_cal_data_model_trigger;
	iface->cal_data_replicate=mkt_cal_data_model_replicate;
	iface->cal_data_point=mkt_cal_data_model_point;

}



G_DEFINE_TYPE_WITH_CODE (MktCalDataModel, mkt_cal_data_model, MKT_TYPE_MODEL,
		                                         G_IMPLEMENT_INTERFACE (MKT_TYPE_CAL_DATA,
		                                		 mkt_cal_data_model_init_stream_interface)
		                                		 MKT_CREATE_MODEL_FULL(mkt_connection_new_default_calibration(TRUE)))


static void
mkt_cal_data_model_init (MktCalDataModel *mkt_cal_data_model)
{
	MktCalDataModelPrivate *priv      = MKT_CAL_DATA_MODEL_PRIVATE(mkt_cal_data_model);
	priv->point              = 0;
	priv->value              = 0.;
	priv->outlier            = FALSE;

	mkt_cal_data_model->priv = priv;
	/* TODO: Add initialization code here */
}

static void
mkt_cal_data_model_finalize (GObject *object)
{
//	MktCalDataModel *data = MKT_STREAM_SIMPLE(object);

	G_OBJECT_CLASS (mkt_cal_data_model_parent_class)->finalize (object);
}




static void
mkt_cal_data_model_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_CAL_DATA_MODEL (object));
	MktCalDataModel *cal = MKT_CAL_DATA_MODEL(object);
	switch (prop_id)
	{

	case PROP_MKT_CAL_DATA_POINT:
		cal->priv->point = g_value_get_uint64(value);
		break;
	case PROP_MKT_CAL_DATA_TRIGGER:
		cal->priv->trigger = g_value_get_uint(value);
		break;
	case PROP_MKT_CAL_DATA_VALUE:
		cal->priv->value = g_value_get_double(value);
		break;
	case PROP_MKT_CAL_DATA_OUTLIER:
		cal->priv->outlier = g_value_get_boolean(value);
		break;
	case PROP_MKT_CAL_DATA_REPLICATE:
		cal->priv->replicate = g_value_get_uint(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_cal_data_model_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_CAL_DATA_MODEL (object));
	MktCalDataModel *cal = MKT_CAL_DATA_MODEL(object);
	switch (prop_id)
	{
	case PROP_MKT_CAL_DATA_POINT:
		g_value_set_uint64(value,cal->priv->point);
		break;
	case PROP_MKT_CAL_DATA_TRIGGER:
		g_value_set_uint(value,cal->priv->trigger);
		break;
	case PROP_MKT_CAL_DATA_VALUE:
		g_value_set_double(value,cal->priv->value);
		break;
	case PROP_MKT_CAL_DATA_OUTLIER:
		g_value_set_boolean(value,cal->priv->outlier);
		break;
	case PROP_MKT_CAL_DATA_REPLICATE:
		g_value_set_uint(value,cal->priv->replicate);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_cal_data_model_class_init (MktCalDataModelClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	g_type_class_add_private (klass, sizeof (MktCalDataModelPrivate));
	object_class->finalize     = mkt_cal_data_model_finalize;
	object_class->set_property = mkt_cal_data_model_set_property;
	object_class->get_property = mkt_cal_data_model_get_property;

	g_object_class_override_property(object_class,PROP_MKT_CAL_DATA_POINT,"cal-data-point");
	g_object_class_override_property(object_class,PROP_MKT_CAL_DATA_TRIGGER,"cal-data-trigger");
	g_object_class_override_property(object_class,PROP_MKT_CAL_DATA_VALUE,"cal-data-value");
	g_object_class_override_property(object_class,PROP_MKT_CAL_DATA_OUTLIER,"cal-data-outlier");
	g_object_class_override_property(object_class,PROP_MKT_CAL_DATA_REPLICATE,"cal-data-replicate");
}

