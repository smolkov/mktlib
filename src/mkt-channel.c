/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktChannel
 * @{
 * @file  mkt-channel.c	Channel model interface
 * @brief This is Channel model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */


#include "mkt-channel.h"
#include "market-time.h"
#include "mkt-connection.h"
#include "mkt-stream-model.h"

#include "../config.h"
#include <glib/gi18n-lib.h>

#if GLIB_CHECK_VERSION(2,31,7)
static GRecMutex init_rmutex;
#define MUTEX_LOCK() g_rec_mutex_lock(&init_rmutex)
#define MUTEX_UNLOCK() g_rec_mutex_unlock(&init_rmutex)
#else
static GStaticRecMutex init_mutex = G_STATIC_REC_MUTEX_INIT;
#define MUTEX_LOCK() g_static_rec_mutex_lock(&init_mutex)
#define MUTEX_UNLOCK() g_static_rec_mutex_unlock(&init_mutex)
#endif

static void
mkt_channel_base_init (gpointer g_iface)
{
	static gboolean is_channel_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_channel_initialized)
	{
		g_object_interface_install_property (g_iface,
				g_param_spec_uint64("channel-stream",
						"stream id",
						"Set get channel measurement property",
						0,G_MAXUINT64,0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT  | MKT_MODEL_DB_PROP ));

		g_object_interface_install_property (g_iface,
				g_param_spec_string  ("channel-name",
						"name",
						"Set get channel name property",
						"----",
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property (g_iface,
				g_param_spec_string  ("channel-unit",
						 "unit",
						"Set get channel unit property",
						"mg/l",
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property (g_iface,
				g_param_spec_string("channel-sensor",
						"sensor reference",
						"sensor reference",
						"/com/lar/tera/integrations/nil",
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE ));

		g_object_interface_install_property (g_iface,
				g_param_spec_boolean("channel-activated",
						"is activated",
						"Set get channel is activated",
						TRUE,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP ));
		g_object_interface_install_property (g_iface,
				g_param_spec_boolean("channel-activated-cal",
						"is activated calibration",
						"Set get channel is activated calibration",
						FALSE,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP ));

		g_object_interface_install_property (g_iface,
				g_param_spec_boolean("channel-allow",
						"is allow",
						"Set get channel is allow",
						TRUE,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP ));

		g_object_interface_install_property (g_iface,
				g_param_spec_boolean("channel-check",
						"check is activated",
						"Set get channel check is activated",
						FALSE,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE  ));

		g_object_interface_install_property (g_iface,
				g_param_spec_boolean("need-calibration",
						"Channel need calibration",
						"Channel need calibration",
						TRUE,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP ));

		g_object_interface_install_property (g_iface,
				g_param_spec_string ("channel-analog-out",
						"analog out reference",
						"Set get channel analog out hardware object name",
						"/analogs/1",
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property (g_iface,
				g_param_spec_string ("channel-check-analog-out",
						"check analog out reference",
					  "Set get channel check analog out hardware object name",
						"/analogs/100",
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property (g_iface,
				g_param_spec_string("channel-type",
						"Channel Type",
						"Channel Type",
						"none",
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT   | MKT_MODEL_DB_PROP ));

		g_object_interface_install_property (g_iface,
				g_param_spec_uint("channel-measurement",
						"Channel measurement",
						"Set get channel measurement property",
						0,G_MAXUINT32,0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT  | MKT_MODEL_DB_PROP ));

		g_object_interface_install_property (g_iface,
				g_param_spec_uint("channel-trigger",
						"Channel measurement",
						"Set get channel measurement property",
						0,G_MAXUINT32,0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT  | MKT_MODEL_DB_PROP ));
		g_object_interface_install_property (g_iface,
				g_param_spec_double("channel-min",
						"minimum scale limit",
						"Set get channel min scale value",
						-G_MAXDOUBLE,G_MAXDOUBLE,0.0,
						G_PARAM_READABLE | G_PARAM_WRITABLE| G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP |MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property (g_iface,
				g_param_spec_double("channel-max",
						"maximum scale limit",
						"Set get channel max scale value",
						-G_MAXDOUBLE,G_MAXDOUBLE,100.0,
						G_PARAM_READABLE | G_PARAM_WRITABLE| G_PARAM_CONSTRUCT  | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE ));

		g_object_interface_install_property (g_iface,
				g_param_spec_double("channel-factor",
						"secret factor",
						"Set get channel secret factor",
						0.001,100.0,1.0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT |  MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property (g_iface,
				g_param_spec_double ("channel-result",
						"Channel channel property",
						"Set get sensor channel property",
						0.,G_MAXDOUBLE,0.,
						G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP ));

		g_object_interface_install_property (g_iface,
				g_param_spec_double ("channel-raw",
						"Channel channel property",
						"Set get sensor channel property",
						0.,G_MAXDOUBLE,0.,
						G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP  ));

		g_object_interface_install_property (g_iface,
				g_param_spec_double ("channel-cv",
						"Channel channel property",
						"Set get sensor channel property",
						0.,G_MAXDOUBLE,0.,
						G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP  ));
		g_object_interface_install_property (g_iface,
				g_param_spec_double ("channel-changed",
						"Measurement measurement property",
						"Set get sensor measurement property",
						0.,G_MAXDOUBLE,0.,
						G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP  ));

		is_channel_initialized = TRUE;
	}
	MUTEX_UNLOCK();
}

GType
mkt_channel_get_type (void)
{
	static GType iface_type = 0;
	if (iface_type == 0)
	{
		static const GTypeInfo info = {
				sizeof (MktChannelInterface),
				(GBaseInitFunc) mkt_channel_base_init,
				(GBaseFinalizeFunc) NULL,
				(GClassInitFunc) NULL,
				NULL,
				NULL,
				0,
				0,
				(GInstanceInitFunc) NULL,
				0
		};
		MUTEX_LOCK();
		if (iface_type == 0)
		{
			iface_type = g_type_register_static (G_TYPE_INTERFACE, "MktChannelInterface",&info, 0);
			g_type_interface_add_prerequisite (iface_type, MKT_TYPE_MODEL);
		}
		MUTEX_UNLOCK();
	}
	return iface_type;
}
const gchar*
mkt_channel_name                         ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , NULL);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) , NULL);
	if(MKT_CHANNEL_GET_INTERFACE(channel)->channel_name )
		return MKT_CHANNEL_GET_INTERFACE(channel)->channel_name(channel);
	return NULL;

}

const gchar*
mkt_channel_unit                         ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , NULL);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) , NULL);
	if(MKT_CHANNEL_GET_INTERFACE(channel)->channel_unit )
		return MKT_CHANNEL_GET_INTERFACE(channel)->channel_unit(channel);
	return NULL;

}

const gchar *
mkt_channel_type                         ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , NULL);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) , NULL);
	if(MKT_CHANNEL_GET_INTERFACE(channel)->channel_type )
		return MKT_CHANNEL_GET_INTERFACE(channel)->channel_type(channel);
	return NULL;
}

const gchar*
mkt_channel_analog_out                       ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , NULL);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) , NULL);
	if(MKT_CHANNEL_GET_INTERFACE(channel)->channel_analog_out )
		return MKT_CHANNEL_GET_INTERFACE(channel)->channel_analog_out(channel);
	return NULL;
}

guint
mkt_channel_measurement                     ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , 0);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) , 0);
	if(MKT_CHANNEL_GET_INTERFACE(channel)->channel_measurement )
		return MKT_CHANNEL_GET_INTERFACE(channel)->channel_measurement(channel);
	return 0;
}

guint
mkt_channel_trigger                         ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , 0);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) , 0);
	if(MKT_CHANNEL_GET_INTERFACE(channel)->channel_trigger )
		return MKT_CHANNEL_GET_INTERFACE(channel)->channel_trigger(channel);
	return 0;
}

gdouble
mkt_channel_min                             ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , 0.0);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) ,  0.0);
	if(MKT_CHANNEL_GET_INTERFACE(channel)->channel_min )
		return MKT_CHANNEL_GET_INTERFACE(channel)->channel_min(channel);
	return 0.0;
}
gdouble
mkt_channel_max                             ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , 0.0);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) ,  0.0);
	if(MKT_CHANNEL_GET_INTERFACE(channel)->channel_max )
		return MKT_CHANNEL_GET_INTERFACE(channel)->channel_max(channel);
	return 0.0;
}
gdouble
mkt_channel_factor                          ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , 0.0);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) ,  0.0);
	if(MKT_CHANNEL_GET_INTERFACE(channel)->channel_factor )
		return MKT_CHANNEL_GET_INTERFACE(channel)->channel_factor(channel);
	return 0.0;
}

gdouble
mkt_channel_result                       ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , 0.0);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) ,  0.0);
	if(MKT_CHANNEL_GET_INTERFACE(channel)->channel_result )
		return MKT_CHANNEL_GET_INTERFACE(channel)->channel_result(channel);
	return 0.0;
}
gdouble
mkt_channel_raw                          ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , 0.0);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) ,  0.0);
	if(MKT_CHANNEL_GET_INTERFACE(channel)->channel_raw )
		return MKT_CHANNEL_GET_INTERFACE(channel)->channel_raw(channel);
	return 0.0;
}
gdouble
mkt_channel_cv                           ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , 0.0);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) ,  0.0);
	if(MKT_CHANNEL_GET_INTERFACE(channel)->channel_cv )
		return MKT_CHANNEL_GET_INTERFACE(channel)->channel_cv(channel);
	return 0.0;
}

// Analog autput number

void
mkt_channel_add_value                    ( MktChannel *channel, gdouble value, guint sensor_trigger , guint type )
{
	g_return_if_fail(channel!=NULL);
	g_return_if_fail(MKT_IS_CHANNEL(channel));
	g_object_set(channel,"channel-wait",FALSE,NULL);
	if(MKT_CHANNEL_GET_INTERFACE(channel)->channel_add_value )
		MKT_CHANNEL_GET_INTERFACE(channel)->channel_add_value(channel,value,sensor_trigger,type);
}

GSList*
mkt_channel_current_value                   ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , NULL);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) , NULL);
	if(MKT_CHANNEL_GET_INTERFACE(channel)->channel_current_value )
		return MKT_CHANNEL_GET_INTERFACE(channel)->channel_current_value(channel);
	return NULL;
}

MktMeasurement*
mkt_channel_current_result                  ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , NULL);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) , NULL);
	if(MKT_CHANNEL_GET_INTERFACE(channel)->channel_current_result )
		return MKT_CHANNEL_GET_INTERFACE(channel)->channel_current_result(channel);
	return NULL;
}

GSList*
mkt_channel_current_replicates              ( MktChannel *channel )
{
	g_return_val_if_fail(channel != NULL , NULL);
	g_return_val_if_fail(MKT_IS_CHANNEL(channel) , NULL);
	if(MKT_CHANNEL_GET_INTERFACE(channel)->channel_current_replicates )
		return MKT_CHANNEL_GET_INTERFACE(channel)->channel_current_replicates(channel);
	return NULL;
}


gdouble
mkt_channel_changed (MktChannel *measurement)
{
	g_return_val_if_fail(measurement != NULL ,  0.0);
	g_return_val_if_fail(MKT_IS_CHANNEL(measurement) ,  0.0);
	if(MKT_CHANNEL_GET_INTERFACE(measurement)->channel_changed )
		return MKT_CHANNEL_GET_INTERFACE(measurement)->channel_changed(measurement);
	return 0.0;
}



/** @} */
