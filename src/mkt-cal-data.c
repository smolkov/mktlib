/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktCalData
 * @{
 * @file  mkt-nitri_cal_data.c	NitriCalData model interface
 * @brief This is NitriCalData model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */


#include "mkt-cal-data.h"
#include "market-time.h"

#include <math.h>

#define GETTEXT_PACKAGE "mkt-library"
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
mkt_cal_data_base_init (gpointer g_iface)
{
	static gboolean is_mkt_cal_data_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_mkt_cal_data_initialized)
	{
		g_object_interface_install_property (g_iface,
				g_param_spec_uint64 ("cal-data-point",
						"Cal data point address",
						"Set|Get data point address",
						0,G_MAXUINT64,0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

		g_object_interface_install_property (g_iface,
				g_param_spec_uint ("cal-data-trigger",
						"Cal data point trigger",
						"Set|Get data point trigger",
						0,G_MAXUINT32,0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

		g_object_interface_install_property (g_iface,
				g_param_spec_double  ("cal-data-value",
						"Cal data value",
						"Set|Get cal data value",
						0.0,G_MAXDOUBLE,0.0,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));

		g_object_interface_install_property (g_iface,
				g_param_spec_boolean  ("cal-data-outlier",
						"Cal data average value",
						"Set|Get cal data average value",
						FALSE,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));

		g_object_interface_install_property (g_iface,
				g_param_spec_uint ("cal-data-replicate",
						"Cal data point replicate",
						"Set|Get data point replicate",
						0,G_MAXUINT32,0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));
		is_mkt_cal_data_initialized = TRUE;
	}
	MUTEX_UNLOCK();
}

GType
mkt_cal_data_get_type (void)
{
	static GType iface_type = 0;
	if (iface_type == 0)
	{
		static const GTypeInfo info = {
				sizeof (MktCalDataInterface),
				(GBaseInitFunc) mkt_cal_data_base_init,
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
			iface_type = g_type_register_static (G_TYPE_INTERFACE, "MktCalDataInterface",&info, 0);
		}
		MUTEX_UNLOCK();
	}
	return iface_type;
}


gdouble
mkt_cal_data_value                                        ( MktCalData *ucal_data )
{
	g_return_val_if_fail(ucal_data != NULL,0);
	g_return_val_if_fail(MKT_IS_CAL_DATA(ucal_data),0);
	if(MKT_CAL_DATA_GET_INTERFACE(ucal_data)->cal_data_value)
		return MKT_CAL_DATA_GET_INTERFACE(ucal_data)->cal_data_value(ucal_data);
	return 0;
}

gboolean
mkt_cal_data_outlier                                      ( MktCalData *ucal_data )
{
	g_return_val_if_fail(ucal_data != NULL,TRUE);
	g_return_val_if_fail(MKT_IS_CAL_DATA(ucal_data),TRUE);
	if(MKT_CAL_DATA_GET_INTERFACE(ucal_data)->cal_data_outlier)
		return MKT_CAL_DATA_GET_INTERFACE(ucal_data)->cal_data_outlier(ucal_data);
	return TRUE;
}

guint
mkt_cal_data_trigger                                      ( MktCalData *ucal_data )
{
	g_return_val_if_fail(ucal_data != NULL,0);
	g_return_val_if_fail(MKT_IS_CAL_DATA(ucal_data),0);
	if(MKT_CAL_DATA_GET_INTERFACE(ucal_data)->cal_data_trigger)
		return MKT_CAL_DATA_GET_INTERFACE(ucal_data)->cal_data_trigger(ucal_data);
	return 0;
}

guint
mkt_cal_data_replicate                                    ( MktCalData *ucal_data )
{
	g_return_val_if_fail(ucal_data != NULL,0);
	g_return_val_if_fail(MKT_IS_CAL_DATA(ucal_data),0);
	if(MKT_CAL_DATA_GET_INTERFACE(ucal_data)->cal_data_replicate)
		return MKT_CAL_DATA_GET_INTERFACE(ucal_data)->cal_data_replicate(ucal_data);
	return 0;
}


guint64
mkt_cal_data_point                                        ( MktCalData *ucal_data )
{
	g_return_val_if_fail(ucal_data != NULL,0);
	g_return_val_if_fail(MKT_IS_CAL_DATA(ucal_data),0);
	if(MKT_CAL_DATA_GET_INTERFACE(ucal_data)->cal_data_point)
		return MKT_CAL_DATA_GET_INTERFACE(ucal_data)->cal_data_point(ucal_data);
	return 0;
}

