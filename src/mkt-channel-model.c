/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktChannelModel
 * @{
 * @file  mkt-channel.c	Channel model model interface
 * @brief This is Channel model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */



#include "mkt-channel-model.h"

#include <math.h>
#include <glib/gprintf.h>
#include "market-time.h"
#include "mkt-model.h"
#include "mkt-measurement-data.h"
#include "mkt-error.h"
#include "mkt-limit.h"
#include "market-time.h"
#include "mkt-analog-model.h"
#include "mkt-stream.h"
#include "mkt-param.h"
#include "mkt-param-object.h"
#include "mkt-value.h"



enum
{
	PROP_0,
	PROP_CHANNEL_STREAM,
	PROP_CHANNEL_NAME_CHANNEL,
	PROP_CHANNEL_UNIT,
	PROP_CHANNEL_SENSOR,
	PROP_CHANNEL_ACTIVATED,
	PROP_CHANNEL_ACTIVATED_CAL,
	PROP_CHANNEL_ALLOW,
	PROP_CHANNEL_CHECK,
	PROP_CHANNEL_NEED_CALIBRATION,
	PROP_CHANNEL_TYPE,
	PROP_CHANNEL_ANALOG_OUT,
	PROP_CHANNEL_CHECK_ANALOG_OUT,
	PROP_CHANNEL_MEASUREMENT,
	PROP_CHANNEL_TRIGGER,
	PROP_CHANNEL_MIN,
	PROP_CHANNEL_MAX,
	PROP_CHANNEL_RAW_VALUE,
	PROP_CHANNEL_RESULT,
	PROP_CHANNEL_CV,
	PROP_CHANNEL_FACTOR,
	PROP_CHANNEL_CHANGED
};


struct _MktChannelModelPrivate
{
	guint64                channel_stream;
	gchar*                 channel_name;
	gchar*                 unit;
	gchar*                 sensor;
	gboolean               activated_cal;
	gboolean               activated;
	gboolean               allow;
	gboolean               check;
	gboolean               need_calibration;
	gchar*                 type;
	gchar*                 analog_out;
	gchar*                 check_analog_out;
	guint                  measurement;
	guint                  trigger;
	guint                  signal_trigger;
	gdouble                min;
	gdouble                max;
	gdouble                factor;
	gdouble                result;
	gdouble                raw;
	gdouble                cv;
	gdouble                changed;
	GSList                *current_value;
	MktMeasurement        *current_result;
	GSList                *current_replicates;
};


#define MKT_CHANNEL_MODEL_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), MKT_TYPE_CHANNEL_MODEL, MktChannelModelPrivate))



static const gchar*
mkt_channel_model_name                     (  MktChannel *channel)
{
	g_return_val_if_fail(channel != NULL , NULL);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) , NULL);
	return MKT_CHANNEL_MODEL(channel)->priv->channel_name;
}

static const gchar*
mkt_channel_model_unit                         ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , NULL);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) , NULL);
	return MKT_CHANNEL_MODEL(channel)->priv->unit;

}

static const gchar*
mkt_channel_model_analog_out                       ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , NULL);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) , NULL);
	return MKT_CHANNEL_MODEL(channel)->priv->analog_out;
}

static gdouble
mkt_channel_model_min                    ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , 0);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) , 0);
	return MKT_CHANNEL_MODEL(channel)->priv->min;
}
static gdouble
mkt_channel_model_max                     ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , 0);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) , 0);
	return MKT_CHANNEL_MODEL(channel)->priv->max;
}
static guint
mkt_channel_model_trigger                       ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , 0);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) , 0);
	return MKT_CHANNEL_MODEL(channel)->priv->trigger;
}

static gdouble
mkt_channel_model_result                       ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , 0.0);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) ,  0.0);
	return MKT_CHANNEL_MODEL(channel)->priv->result;
}
static gdouble
mkt_channel_model_raw                          ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , 0.0);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) ,  0.0);
	return MKT_CHANNEL_MODEL(channel)->priv->raw;
}
static gdouble
mkt_channel_model_cv                           ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , 0.0);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) ,  0.0);
	return MKT_CHANNEL_MODEL(channel)->priv->cv;
}

static gdouble
mkt_channel_model_changed                           ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , 0.0);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) ,  0.0);
	return MKT_CHANNEL_MODEL(channel)->priv->changed;
}


static void
mkt_channel_model_add_value                    ( MktChannel *channel, gdouble value, guint sensor_trigger , guint type )
{
	g_return_if_fail(channel!=NULL);
	g_return_if_fail(MKT_IS_CHANNEL(channel));
	MktChannelModel *mchannel = MKT_CHANNEL_MODEL(channel);
	mktMPSet(MKT_MODEL(mchannel),"channel-raw",value);
	if(mchannel->priv->measurement==0)
	{
		mktMPSet(channel,"channel-measurement",1);
	}
	MktModel *model = mkt_model_new(MKT_TYPE_MEASUREMENT_DATA,
			"measurement-errors","",
			"measurement-limits","",
			"measurement-channel",mkt_model_ref_id(MKT_IMODEL(channel)),
			"measurement-type",type,
			"measurement-changed",market_db_time_now(),
			"measurement-value",mchannel->priv->raw,
			"measurement-trigger",mchannel->priv->measurement,
			"measurement-signal",mchannel->priv->trigger,NULL);
	g_object_unref(model);
}

static GSList*
mkt_channel_model_current_value (  MktChannel *channel )
{
	MktChannelModel *mchannel = MKT_CHANNEL_MODEL(channel);
	if(mchannel->priv->current_value)      g_slist_free_full(mchannel->priv->current_value,g_object_unref);
	guint64 ref_id = mkt_model_ref_id(MKT_IMODEL(mchannel));
	mchannel->priv->current_value = mkt_model_select(MKT_TYPE_MEASUREMENT_DATA,
			"select * from $tablename where measurement_channel = %"G_GUINT64_FORMAT" and measurement_trigger = %u",ref_id,mchannel->priv->measurement);
	return mchannel->priv->current_value;
}

static MktMeasurement*
mkt_channel_model_current_result (  MktChannel *channel )
{
	MktChannelModel *mchannel = MKT_CHANNEL_MODEL(channel);
	if(mchannel->priv->current_result)      g_object_unref(mchannel->priv->current_result);
	guint64 ref_id = mkt_model_ref_id(MKT_IMODEL(mchannel));
	mchannel->priv->current_result = MKT_MEASUREMENT(mkt_model_select_one(MKT_TYPE_MEASUREMENT_DATA,
			"select * from $tablename where measurement_channel = %"G_GUINT64_FORMAT" and measurement_trigger = %u and measurement_type=1 LIMIT 1",ref_id,mchannel->priv->measurement));
	return mchannel->priv->current_result;
}

static GSList*
mkt_channel_model_current_replicates (  MktChannel *channel )
{
	MktChannelModel *mchannel = MKT_CHANNEL_MODEL(channel);
	if(mchannel->priv->current_replicates)      g_slist_free_full(mchannel->priv->current_replicates,g_object_unref);
	guint64 ref_id = mkt_model_ref_id(MKT_IMODEL(mchannel));
	mchannel->priv->current_replicates = mkt_model_select(MKT_TYPE_MEASUREMENT_DATA,
			"select * from $tablename where measurement_channel = %"G_GUINT64_FORMAT" and measurement_trigger = %u and measurement_type=0",ref_id,mchannel->priv->measurement);
	return mchannel->priv->current_replicates;
}

static void
mkt_channel_model_init_interface ( MktChannelInterface *iface )
{
	iface->channel_name=mkt_channel_model_name;
	iface->channel_unit=mkt_channel_model_unit;
	iface->channel_analog_out=mkt_channel_model_analog_out;
	iface->channel_measurement=mkt_channel_measurement;
	iface->channel_trigger=mkt_channel_model_trigger;
	iface->channel_min=mkt_channel_model_min;
	iface->channel_max=mkt_channel_model_max;
	iface->channel_result=mkt_channel_model_result;
	iface->channel_raw=mkt_channel_model_raw;
	iface->channel_cv=mkt_channel_model_cv;
	iface->channel_add_value=mkt_channel_model_add_value;
	iface->channel_changed=mkt_channel_model_changed;
	iface->channel_current_value=mkt_channel_model_current_value;
	iface->channel_current_result=mkt_channel_model_current_result;
	iface->channel_current_replicates=mkt_channel_model_current_replicates;
}


G_DEFINE_TYPE_WITH_CODE (MktChannelModel, mkt_channel_model, MKT_TYPE_PARAM_OBJECT,
		                                  G_IMPLEMENT_INTERFACE (MKT_TYPE_CHANNEL,
				                          mkt_channel_model_init_interface)
				                          MKT_CREATE_MODEL_FULL(mkt_connection_new_default_parameter(TRUE)))


static void
mkt_channel_model_init (MktChannelModel *mkt_channel_model)
{
	MktChannelModelPrivate *priv      = MKT_CHANNEL_MODEL_PRIVATE(mkt_channel_model);
	priv->channel_name        = NULL;
	priv->unit                = NULL;
	priv->sensor              = NULL;
	priv->analog_out          = NULL;
	priv->check_analog_out    = NULL;
	priv->type                = NULL;
	priv->current_value       = NULL;
	priv->current_result      = NULL;
	priv->current_replicates  = NULL;
	priv->cv=0.0;
	priv->min=0.;
	priv->max=100.;
	priv->result=0.0;
	priv->raw=0.0;
	priv->trigger  = 1;
	priv->measurement=0;
	priv->need_calibration = FALSE;
	mkt_channel_model->priv   = priv;
	/* TODO: Add initialization code here */
}

static void
mkt_channel_model_finalize (GObject *object)
{

	MktChannelModel *mchannel = MKT_CHANNEL_MODEL(object);
	if(mchannel->priv->sensor)               g_free(mchannel->priv->sensor);
	if(mchannel->priv->unit)               g_free(mchannel->priv->unit);
	if(mchannel->priv->channel_name)       g_free(mchannel->priv->channel_name);
	if(mchannel->priv->type)               g_free(mchannel->priv->type);
	if(mchannel->priv->analog_out)         g_free(mchannel->priv->analog_out);
	if(mchannel->priv->check_analog_out)   g_free(mchannel->priv->check_analog_out);
	if(mchannel->priv->current_value)      g_slist_free_full(mchannel->priv->current_value,g_object_unref);
	if(mchannel->priv->current_result)     g_object_unref(mchannel->priv->current_result);
	if(mchannel->priv->current_replicates) g_slist_free_full(mchannel->priv->current_replicates,g_object_unref);
	G_OBJECT_CLASS (mkt_channel_model_parent_class)->finalize (object);
}


static void
mkt_channel_model_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_CHANNEL_MODEL (object));
	MktChannelModel *channel = MKT_CHANNEL_MODEL(object);
	switch (prop_id)
	{
	case PROP_CHANNEL_STREAM:
		channel->priv->channel_stream = g_value_get_uint64(value);
		break;
	case PROP_CHANNEL_TYPE:
		if(channel->priv->type)g_free(channel->priv->type);
		channel->priv->type = g_value_dup_string(value);
		break;
	case PROP_CHANNEL_UNIT:
		if(channel->priv->unit!= NULL)g_free(channel->priv->unit);
		channel->priv->unit = g_value_dup_string(value);
		break;
	case PROP_CHANNEL_SENSOR:
		if(channel->priv->sensor!= NULL)g_free(channel->priv->sensor);
		channel->priv->sensor = g_value_dup_string(value);
		break;
	case PROP_CHANNEL_NAME_CHANNEL:
		if(channel->priv->channel_name!= NULL)g_free(channel->priv->channel_name);
		channel->priv->channel_name = g_value_dup_string(value);
		break;
	case PROP_CHANNEL_ACTIVATED:
		channel->priv->activated = g_value_get_boolean(value);
		break;
	case PROP_CHANNEL_ACTIVATED_CAL:
		channel->priv->activated_cal = g_value_get_boolean(value);
		break;
	case PROP_CHANNEL_ALLOW:
		channel->priv->allow = g_value_get_boolean(value);
		break;
	case PROP_CHANNEL_CHECK:
		channel->priv->check = g_value_get_boolean(value);
		break;
	case PROP_CHANNEL_NEED_CALIBRATION:
		channel->priv->need_calibration = g_value_get_boolean(value);
		break;
	case PROP_CHANNEL_ANALOG_OUT:
		if(channel->priv->analog_out!= NULL)g_free(channel->priv->analog_out);
		channel->priv->analog_out = g_value_dup_string(value);
		break;
	case PROP_CHANNEL_CHECK_ANALOG_OUT:
		if(channel->priv->check_analog_out!= NULL)g_free(channel->priv->check_analog_out);
		channel->priv->check_analog_out = g_value_dup_string(value);
		break;
	case PROP_CHANNEL_MEASUREMENT:
		channel->priv->measurement = g_value_get_uint(value);
		break;
	case PROP_CHANNEL_TRIGGER:
		channel->priv->trigger = g_value_get_uint(value);
		break;
	case PROP_CHANNEL_MIN:
		channel->priv->min = g_value_get_double(value);
		break;
	case PROP_CHANNEL_MAX:
		channel->priv->max = g_value_get_double(value);
		break;
	case PROP_CHANNEL_RESULT:
		channel->priv->result = g_value_get_double(value);
		break;
	case PROP_CHANNEL_RAW_VALUE:
		channel->priv->raw = g_value_get_double(value);
		break;
	case PROP_CHANNEL_CV:
		channel->priv->cv = g_value_get_double(value);
		break;
	case PROP_CHANNEL_CHANGED:
		channel->priv->changed = g_value_get_double(value);
		break;
	case PROP_CHANNEL_FACTOR:
		channel->priv->factor = g_value_get_double(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_channel_model_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_CHANNEL_MODEL (object));
	MktChannelModel *channel = MKT_CHANNEL_MODEL(object);
	switch (prop_id)
	{
	case PROP_CHANNEL_STREAM:
		g_value_set_uint64(value , channel->priv->channel_stream);
		break;
	case PROP_CHANNEL_TYPE:
		g_value_set_string(value , channel->priv->type);
		break;
	case PROP_CHANNEL_NAME_CHANNEL:
		g_value_set_string(value , channel->priv->channel_name);
		break;
	case PROP_CHANNEL_SENSOR:
		g_value_set_string(value , channel->priv->sensor);
		break;
	case PROP_CHANNEL_UNIT:
		g_value_set_string(value , channel->priv->unit);
		break;
	case PROP_CHANNEL_ANALOG_OUT:
		g_value_set_string(value , channel->priv->analog_out);
		break;
	case PROP_CHANNEL_CHECK_ANALOG_OUT:
		g_value_set_string(value , channel->priv->check_analog_out);
		break;
	case PROP_CHANNEL_ACTIVATED:
		g_value_set_boolean(value , channel->priv->activated);
		break;
	case PROP_CHANNEL_ACTIVATED_CAL:
		g_value_set_boolean(value , channel->priv->activated_cal);
		break;
	case PROP_CHANNEL_ALLOW:
		g_value_set_boolean(value , channel->priv->allow);
		break;
	case PROP_CHANNEL_CHECK:
		g_value_set_boolean(value , channel->priv->check);
		break;
	case PROP_CHANNEL_NEED_CALIBRATION:
		g_value_set_boolean(value , channel->priv->need_calibration);
		break;
	case PROP_CHANNEL_MEASUREMENT:
		g_value_set_uint(value , channel->priv->measurement);
		break;
	case PROP_CHANNEL_TRIGGER:
		g_value_set_uint(value , channel->priv->trigger);
		break;
	case PROP_CHANNEL_MIN:
		g_value_set_double(value , channel->priv->min);
		break;
	case PROP_CHANNEL_MAX:
		g_value_set_double(value , channel->priv->max);
		break;
	case PROP_CHANNEL_RESULT:
		g_value_set_double(value , channel->priv->result);
		break;
	case PROP_CHANNEL_RAW_VALUE:
		g_value_set_double(value , channel->priv->raw);
		break;
	case PROP_CHANNEL_CV:
		g_value_set_double(value , channel->priv->cv);
		break;
	case PROP_CHANNEL_CHANGED:
		g_value_set_double(value , channel->priv->changed);
		break;
	case PROP_CHANNEL_FACTOR:
		g_value_set_double(value,channel->priv->factor);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_channel_model_class_init (MktChannelModelClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//MktModelClass* parent_class = MKT_MODEL_CLASS (klass);

	g_type_class_add_private (klass, sizeof (MktChannelModelPrivate));
	object_class->finalize     = mkt_channel_model_finalize;
	object_class->set_property = mkt_channel_model_set_property;
	object_class->get_property = mkt_channel_model_get_property;

	g_object_class_override_property(object_class,PROP_CHANNEL_STREAM,"channel-stream");
	g_object_class_override_property(object_class,PROP_CHANNEL_TYPE,"channel-type");
	g_object_class_override_property(object_class,PROP_CHANNEL_UNIT,"channel-unit");
	g_object_class_override_property(object_class,PROP_CHANNEL_SENSOR,"channel-sensor");
	g_object_class_override_property(object_class,PROP_CHANNEL_ANALOG_OUT,"channel-analog-out");
	g_object_class_override_property(object_class,PROP_CHANNEL_CHECK_ANALOG_OUT,"channel-check-analog-out");

	g_object_class_override_property(object_class,PROP_CHANNEL_ACTIVATED,"channel-activated");
	g_object_class_override_property(object_class,PROP_CHANNEL_ACTIVATED_CAL,"channel-activated-cal");

	g_object_class_override_property(object_class,PROP_CHANNEL_ALLOW,"channel-allow");
	g_object_class_override_property(object_class,PROP_CHANNEL_CHECK,"channel-check");
	g_object_class_override_property(object_class,PROP_CHANNEL_NEED_CALIBRATION,"need-calibration");
	g_object_class_override_property(object_class,PROP_CHANNEL_NAME_CHANNEL,"channel-name");
	g_object_class_override_property(object_class,PROP_CHANNEL_MEASUREMENT,"channel-measurement");
	g_object_class_override_property(object_class,PROP_CHANNEL_TRIGGER,"channel-trigger");

	g_object_class_override_property(object_class,PROP_CHANNEL_MIN,"channel-min");
	g_object_class_override_property(object_class,PROP_CHANNEL_MAX,"channel-max");
	g_object_class_override_property(object_class,PROP_CHANNEL_RESULT,"channel-result");
	g_object_class_override_property(object_class,PROP_CHANNEL_RAW_VALUE,"channel-raw");
	g_object_class_override_property(object_class,PROP_CHANNEL_CV,"channel-cv");
	g_object_class_override_property(object_class,PROP_CHANNEL_CHANGED,"channel-changed");
	g_object_class_override_property(object_class,PROP_CHANNEL_FACTOR,"channel-factor");
}

/** @} */
