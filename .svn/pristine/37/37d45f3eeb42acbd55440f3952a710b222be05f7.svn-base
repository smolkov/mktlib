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

#include "mkt-vessel-model.h"
#include "market-time.h"



enum
{
	PROP_0,
	PROP_VESSEL_X,
	PROP_VESSEL_Y,
	PROP_VESSEL_Y1,
	PROP_VESSEL_Y2,
	PROP_VESSEL_Y3,
	PROP_VESSEL_PUMP,
	PROP_VESSEL_HAS_MOTOR


};


struct _MktVesselModelPrivate
{
	guint        X;
	guint        Y;
	guint        Y1;
	guint        Y2;
	guint        Y3;
	gchar*       pump;
	gboolean     has_motor;

// Only nitri parameter
};


#define MKT_VESSEL_MODEL_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), MKT_TYPE_VESSEL_MODEL, MktVesselModelPrivate))






static guint
mkt_vessel_model_xpos                      ( MktVessel *vessel )
{
	g_return_val_if_fail(vessel != NULL , 0);
	g_return_val_if_fail(MKT_IS_VESSEL_MODEL(vessel) , 0);
	return MKT_VESSEL_MODEL(vessel)->priv->X;
}
static guint
mkt_vessel_model_ypos                      ( MktVessel *vessel )
{
	g_return_val_if_fail(vessel != NULL , 0);
	g_return_val_if_fail(MKT_IS_VESSEL_MODEL(vessel) , 0);
	return MKT_VESSEL_MODEL(vessel)->priv->Y;
}
static guint
mkt_vessel_model_y1pos                      ( MktVessel *vessel )
{
	g_return_val_if_fail(vessel != NULL , 0);
	g_return_val_if_fail(MKT_IS_VESSEL_MODEL(vessel) , 0);
	return MKT_VESSEL_MODEL(vessel)->priv->Y1;
}
static guint
mkt_vessel_model_y2pos                      ( MktVessel *vessel )
{
	g_return_val_if_fail(vessel != NULL , 0);
	g_return_val_if_fail(MKT_IS_VESSEL_MODEL(vessel) , 0);
	return MKT_VESSEL_MODEL(vessel)->priv->Y2;
}
static guint
mkt_vessel_model_y3pos                      ( MktVessel *vessel )
{
	g_return_val_if_fail(vessel != NULL , 0);
	g_return_val_if_fail(MKT_IS_VESSEL_MODEL(vessel) , 0);
	return MKT_VESSEL_MODEL(vessel)->priv->Y3;
}
static const gchar*
mkt_vessel_model_pump                          ( MktVessel *vessel )
{
	g_return_val_if_fail(vessel != NULL , 0);
	g_return_val_if_fail(MKT_IS_VESSEL_MODEL(vessel) , 0);
	return MKT_VESSEL_MODEL(vessel)->priv->pump;
}



static void
mkt_vessel_model_init_stream_interface ( MktVesselInterface *iface )
{
	iface->vessel_xpos        = mkt_vessel_model_xpos;
	iface->vessel_ypos        = mkt_vessel_model_ypos;
	iface->vessel_y1pos       = mkt_vessel_model_y1pos;
	iface->vessel_y2pos       = mkt_vessel_model_y2pos;
	iface->vessel_y3pos       = mkt_vessel_model_y3pos;
	iface->vessel_pump        = mkt_vessel_model_pump;
}



G_DEFINE_TYPE_WITH_CODE (MktVesselModel, mkt_vessel_model, MKT_TYPE_PARAM_OBJECT,
		                                    G_IMPLEMENT_INTERFACE (MKT_TYPE_VESSEL,
		                                	mkt_vessel_model_init_stream_interface)
		                                	MKT_CREATE_MODEL_FULL(mkt_connection_new_default_parameter(TRUE)))


static void
mkt_vessel_model_init (MktVesselModel *mkt_vessel_model)
{
	MktVesselModelPrivate *priv      = MKT_VESSEL_MODEL_PRIVATE(mkt_vessel_model);
	priv->X           = 0;
	priv->Y1          = 0;
	priv->Y2          = 0;
	priv->Y3          = 0;
	priv->pump        = g_strdup("/com/lar/pumps/Pump1");
	mkt_vessel_model->priv   = priv;
	/* TODO: Add initialization code here */
}

static void
mkt_vessel_model_finalize (GObject *object)
{
	MktVesselModel *data = MKT_VESSEL_MODEL(object);
	if(data->priv->pump)g_free(data->priv->pump);
	G_OBJECT_CLASS (mkt_vessel_model_parent_class)->finalize (object);
}




static void
mkt_vessel_model_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_VESSEL_MODEL (object));
	MktVesselModel *stream = MKT_VESSEL_MODEL(object);
	switch (prop_id)
	{

	case PROP_VESSEL_X:
		stream->priv->X = g_value_get_uint(value);
		break;
	case PROP_VESSEL_Y:
		stream->priv->Y = g_value_get_uint(value);
		break;
	case PROP_VESSEL_Y1:
		stream->priv->Y1 = g_value_get_uint(value);
		break;
	case PROP_VESSEL_Y2:
		stream->priv->Y2 = g_value_get_uint(value);
		break;
	case PROP_VESSEL_Y3:
		stream->priv->Y3 = g_value_get_uint(value);
		break;
	case PROP_VESSEL_PUMP:
		if(stream->priv->pump)g_free(stream->priv->pump);
		stream->priv->pump = g_value_dup_string(value);
		break;
	case PROP_VESSEL_HAS_MOTOR:
		stream->priv->has_motor = g_value_get_boolean(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_vessel_model_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_VESSEL_MODEL (object));
	MktVesselModel *stream = MKT_VESSEL_MODEL(object);
	switch (prop_id)
	{
	case PROP_VESSEL_X:
		g_value_set_uint(value,stream->priv->X);
		break;
	case PROP_VESSEL_Y:
		g_value_set_uint(value,stream->priv->Y);
		break;
	case PROP_VESSEL_Y1:
		g_value_set_uint(value,stream->priv->Y1);
		break;
	case PROP_VESSEL_Y2:
		g_value_set_uint(value,stream->priv->Y2);
		break;
	case PROP_VESSEL_Y3:
		g_value_set_uint(value,stream->priv->Y3);
		break;
	case PROP_VESSEL_PUMP:
		g_value_set_string(value,stream->priv->pump );
		break;
	case PROP_VESSEL_HAS_MOTOR:
		g_value_set_boolean(value,stream->priv->has_motor);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_vessel_model_class_init (MktVesselModelClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//MktModelClass* parent_class = MKT_MODEL_CLASS (klass);
	g_type_class_add_private (klass, sizeof (MktVesselModelPrivate));
	object_class->finalize     = mkt_vessel_model_finalize;
	object_class->set_property = mkt_vessel_model_set_property;
	object_class->get_property = mkt_vessel_model_get_property;


	g_object_class_override_property(object_class,PROP_VESSEL_X,"vessel-x-pos");
	g_object_class_override_property(object_class,PROP_VESSEL_Y,"vessel-y-pos");
	g_object_class_override_property(object_class,PROP_VESSEL_Y1,"vessel-y1-pos");
	g_object_class_override_property(object_class,PROP_VESSEL_Y2,"vessel-y2-pos");
	g_object_class_override_property(object_class,PROP_VESSEL_Y3,"vessel-y3-pos");
	g_object_class_override_property(object_class,PROP_VESSEL_PUMP,"vessel-pump");
	g_object_class_override_property(object_class,PROP_VESSEL_HAS_MOTOR,"vessel-has-motor");


}

