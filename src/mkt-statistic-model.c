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
#include "mkt-statistic-model.h"
#include "mkt-value.h"



enum
{
	PROP_0,
	PROP_STATISTIC_REPLICATES,
	PROP_STATISTIC_OUTLIERS,
	PROP_STATISTIC_MAX_CV,
	PROP_STATISTIC_THRESHOLD,
	PROP_STATISTIC_JUMP,
	PROP_STATISTIC_AMOUNT_COUNTER,
	PROP_STATISTIC_AMOUNT_PERCENTAGE,
};


struct _MktStatisticModelPrivate
{
	guint          replicates;
	guint          outlier;
	gdouble        max_cv;
	gdouble        threshold;
	gdouble        jump;
	guint          amount_counter;
	gdouble        amount_percentage;
// Only nitri parameter
};


#define MKT_STATISTIC_MODEL_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), MKT_TYPE_STATISTIC_MODEL, MktStatisticModelPrivate))





static guint
mkt_stream_object_replicates                      ( MktStatistic *statistic )
{
	g_return_val_if_fail(statistic != NULL , 0);
	g_return_val_if_fail(MKT_IS_STATISTIC_MODEL(statistic) , 0);
	return MKT_STATISTIC_MODEL(statistic)->priv->replicates;
}

static guint
mkt_stream_object_outliers                        ( MktStatistic *statistic )
{
	g_return_val_if_fail(statistic != NULL , 0);
	g_return_val_if_fail(MKT_IS_STATISTIC_MODEL(statistic) , 0);
	return MKT_STATISTIC_MODEL(statistic)->priv->outlier;
}
static gdouble
mkt_stream_object_max_cv                    ( MktStatistic *statistic )
{
	g_return_val_if_fail(statistic != NULL , 0.);
	g_return_val_if_fail(MKT_IS_STATISTIC_MODEL(statistic) , 0.);
	return MKT_STATISTIC_MODEL(statistic)->priv->max_cv;
}
static gdouble
mkt_stream_object_threshold                    ( MktStatistic *statistic )
{
	g_return_val_if_fail(statistic != NULL , 0.);
	g_return_val_if_fail(MKT_IS_STATISTIC_MODEL(statistic) , 0.);
	return MKT_STATISTIC_MODEL(statistic)->priv->threshold;
}


static void
mkt_strean_object_init_statistic_interface ( MktStatisticInterface *iface )
{
	iface->statistic_replicates      =mkt_stream_object_replicates;
	iface->statistic_outliers        =mkt_stream_object_outliers;
	iface->statistic_max_cv          =mkt_stream_object_max_cv;
	iface->statistic_threshold       =mkt_stream_object_threshold;


}



G_DEFINE_TYPE_WITH_CODE (MktStatisticModel, mkt_statistic_model, MKT_TYPE_PARAM_OBJECT,
		                                  		G_IMPLEMENT_INTERFACE (MKT_TYPE_STATISTIC,
		                                		mkt_strean_object_init_statistic_interface)
		                                		MKT_CREATE_MODEL_FULL(mkt_connection_new_default_parameter(TRUE)))


static void
mkt_statistic_model_init (MktStatisticModel *mkt_statistic_model)
{
	MktStatisticModelPrivate *priv      = MKT_STATISTIC_MODEL_PRIVATE(mkt_statistic_model);
	mkt_statistic_model->priv           = priv;
	/* TODO: Add initialization code here */
}

static void
mkt_statistic_model_finalize (GObject *object)
{
	//MktStatisticModel *statistic = MKT_STATISTIC_MODEL(object);
	G_OBJECT_CLASS (mkt_statistic_model_parent_class)->finalize (object);
}

static void
mkt_statistic_model_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_STATISTIC_MODEL (object));
	MktStatisticModel *stream = MKT_STATISTIC_MODEL(object);
	switch (prop_id)
	{
	case PROP_STATISTIC_REPLICATES:
		stream->priv->replicates = g_value_get_uint(value);
		break;
	case PROP_STATISTIC_OUTLIERS:
		stream->priv->outlier = g_value_get_uint(value);
		break;
	case PROP_STATISTIC_MAX_CV:
		stream->priv->max_cv  = g_value_get_double(value);
		break;
	case PROP_STATISTIC_THRESHOLD:
		stream->priv->threshold = g_value_get_double(value);
		break;
	case PROP_STATISTIC_JUMP:
		stream->priv->jump = g_value_get_double(value);
		break;
	case PROP_STATISTIC_AMOUNT_COUNTER:
		stream->priv->amount_counter = g_value_get_uint(value);
		break;
	case PROP_STATISTIC_AMOUNT_PERCENTAGE:
		stream->priv->amount_percentage = g_value_get_double(value);
		break;

	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_statistic_model_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_STATISTIC_MODEL (object));
	MktStatisticModel *stream = MKT_STATISTIC_MODEL(object);
	switch (prop_id)
	{
	case PROP_STATISTIC_REPLICATES:
		g_value_set_uint(value,stream->priv->replicates);
		break;
	case PROP_STATISTIC_OUTLIERS:
		g_value_set_uint(value,stream->priv->outlier);
		break;
	case PROP_STATISTIC_MAX_CV:
		g_value_set_double(value,stream->priv->max_cv);
		break;
	case PROP_STATISTIC_THRESHOLD:
		g_value_set_double(value,stream->priv->threshold);
		break;
	case PROP_STATISTIC_JUMP:
		g_value_set_double(value,stream->priv->threshold);
		break;
	case PROP_STATISTIC_AMOUNT_COUNTER:
		g_value_set_uint(value,stream->priv->amount_counter);
		break;
	case PROP_STATISTIC_AMOUNT_PERCENTAGE:
		g_value_set_double(value,stream->priv->amount_percentage);
		break;

	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_statistic_model_class_init (MktStatisticModelClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//MktModelClass* parent_class = MKT_MODEL_CLASS (klass);
	g_type_class_add_private (klass, sizeof (MktStatisticModelPrivate));
	object_class->finalize     = mkt_statistic_model_finalize;
	object_class->set_property = mkt_statistic_model_set_property;
	object_class->get_property = mkt_statistic_model_get_property;

	g_object_class_override_property(object_class,PROP_STATISTIC_REPLICATES,"statistic-replicates");
	g_object_class_override_property(object_class,PROP_STATISTIC_OUTLIERS,"statistic-outliers");
	g_object_class_override_property(object_class,PROP_STATISTIC_MAX_CV,"statistic-max-cv");
	g_object_class_override_property(object_class,PROP_STATISTIC_THRESHOLD,"statistic-threshold");
	g_object_class_override_property(object_class,PROP_STATISTIC_JUMP,"statistic-jump");
	g_object_class_override_property(object_class,PROP_STATISTIC_AMOUNT_COUNTER,"statistic-amount-counter");
	g_object_class_override_property(object_class,PROP_STATISTIC_AMOUNT_PERCENTAGE,"statistic-amount-percentage");



}

