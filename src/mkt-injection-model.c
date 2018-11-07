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

#include "mkt-injection-model.h"
#include "market-time.h"

#include "../config.h"
#include <glib/gi18n-lib.h>

enum
{
	PROP_0,
	PROP_INJECTION_AIR,
	PROP_INJECTION_REST,
	PROP_INJECTION_FURNACE_AIR,
	PROP_INJECTION_DILUTION,
	PROP_INJECTION_RINSING,
	PROP_INJECTION_INJECTION_PAR,
	PROP_INJECTION_SAMPLE_PAR,
	PROP_INJECTION_RINSING_UP_PAR,
	PROP_INJECTION_RINSING_DOWND_PAR,
};

struct _MktInjectionModelPrivate
{
	guint        air;
	guint        rest;
	guint        furnace_air;
	guint        dilution;
	gboolean     rinsing;
	guint        injection_stepper_par;
	guint        sample_stepper_par;
	guint        rinsing_up_stepper_par;
	guint        rinsing_down_stepper_par;

// Only nitri parameter
};


#define MKT_INJECTION_MODEL_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), MKT_TYPE_INJECTION_MODEL, MktInjectionModelPrivate))





static void
mkt_injection_model_init_stream_interface ( MktInjectionInterface *iface )
{
}



G_DEFINE_TYPE_WITH_CODE (MktInjectionModel, mkt_injection_model, MKT_TYPE_PARAM_OBJECT,
		                                    G_IMPLEMENT_INTERFACE (MKT_TYPE_INJECTION,
		                                	mkt_injection_model_init_stream_interface)
		                                	MKT_CREATE_MODEL_FULL(mkt_connection_new_default_parameter(TRUE)))


static void
mkt_injection_model_init (MktInjectionModel *mkt_injection_model)
{
	MktInjectionModelPrivate *priv      = MKT_INJECTION_MODEL_PRIVATE(mkt_injection_model);
	mkt_injection_model->priv   = priv;
	/* TODO: Add initialization code here */
}

static void
mkt_injection_model_finalize (GObject *object)
{
	//MktInjectionModel *data = MKT_INJECTION_MODEL(object);
	G_OBJECT_CLASS (mkt_injection_model_parent_class)->finalize (object);
}




static void
mkt_injection_model_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_INJECTION_MODEL (object));
	MktInjectionModel *stream = MKT_INJECTION_MODEL(object);
	//g_debug("mkt_injection_model_get_property %s",pspec->name);
	switch (prop_id)
	{
	case PROP_INJECTION_AIR:
		stream->priv->air = g_value_get_uint(value);
		break;
	case PROP_INJECTION_REST:
		stream->priv->rest = g_value_get_uint(value);
		break;
	case PROP_INJECTION_FURNACE_AIR:
		stream->priv->furnace_air = g_value_get_uint(value);
		break;
	case PROP_INJECTION_DILUTION:
		stream->priv->dilution = g_value_get_uint(value);
		break;
	case PROP_INJECTION_RINSING:
		stream->priv->rinsing = g_value_get_uint(value);
		break;
	case PROP_INJECTION_INJECTION_PAR:
		stream->priv->injection_stepper_par = g_value_get_uint(value);
		break;
	case PROP_INJECTION_SAMPLE_PAR:
		stream->priv->sample_stepper_par = g_value_get_uint(value);
		break;
	case PROP_INJECTION_RINSING_UP_PAR:
		stream->priv->rinsing_up_stepper_par = g_value_get_uint(value);
		break;
	case PROP_INJECTION_RINSING_DOWND_PAR:
		stream->priv->rinsing_down_stepper_par = g_value_get_uint(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}


static void
mkt_injection_model_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_INJECTION_MODEL (object));
	MktInjectionModel *stream = MKT_INJECTION_MODEL(object);
	switch (prop_id)
	{
	case PROP_INJECTION_AIR:
		g_value_set_uint(value,stream->priv->air);
		break;
	case PROP_INJECTION_REST:
		g_value_set_uint(value,stream->priv->rest);
		break;
	case PROP_INJECTION_FURNACE_AIR:
		g_value_set_uint(value,stream->priv->furnace_air);
		break;
	case PROP_INJECTION_DILUTION:
		g_value_set_uint(value,stream->priv->dilution);
		break;
	case PROP_INJECTION_RINSING:
		g_value_set_uint(value,stream->priv->rinsing);
		break;
	case PROP_INJECTION_INJECTION_PAR:
		g_value_set_uint(value,stream->priv->injection_stepper_par);
		break;
	case PROP_INJECTION_SAMPLE_PAR:
		g_value_set_uint(value,stream->priv->sample_stepper_par);
		break;
	case PROP_INJECTION_RINSING_UP_PAR:
		g_value_set_uint(value,stream->priv->rinsing_up_stepper_par);
		break;
	case PROP_INJECTION_RINSING_DOWND_PAR:
		g_value_set_uint(value,stream->priv->rinsing_down_stepper_par);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_injection_model_class_init (MktInjectionModelClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//MktModelClass* parent_class = MKT_MODEL_CLASS (klass);
	g_type_class_add_private (klass, sizeof (MktInjectionModelPrivate));
	object_class->finalize     = mkt_injection_model_finalize;
	object_class->set_property = mkt_injection_model_set_property;
	object_class->get_property = mkt_injection_model_get_property;

	g_object_class_override_property(object_class,PROP_INJECTION_AIR,"injection-air");
	g_object_class_override_property(object_class,PROP_INJECTION_REST,"injection-rest");
	g_object_class_override_property(object_class,PROP_INJECTION_FURNACE_AIR,"injection-furnace-air");
	g_object_class_override_property(object_class,PROP_INJECTION_DILUTION,"injection-dilution");
	g_object_class_override_property(object_class,PROP_INJECTION_RINSING,"injection-rinsing");

	g_object_class_override_property(object_class,PROP_INJECTION_INJECTION_PAR,"injection-stepper-parameter");
	g_object_class_override_property(object_class,PROP_INJECTION_SAMPLE_PAR,"sample-stepper-parameter");
	g_object_class_override_property(object_class,PROP_INJECTION_RINSING_UP_PAR,"rinsing-up-stepper-parameter");
	g_object_class_override_property(object_class,PROP_INJECTION_RINSING_DOWND_PAR,"rinsing-down-stepper-parameter");

}

