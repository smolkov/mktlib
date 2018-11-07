/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup UltimateIntegration
 * @{
 * @file  mkt-nitri_integration.c	NitriIntegration model interface
 * @brief This is NitriIntegration model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */

#include "ultimate-integration.h"
#include "market-time.h"


#include <math.h>

#define GETTEXT_PACKAGE "ultimate-library"
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
ultimate_integration_base_init (gpointer g_iface)
{
	static gboolean is_ultimate_integration_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_ultimate_integration_initialized)
	{

		g_object_interface_install_property (g_iface,
				g_param_spec_double  ("justification-time",
						"Sensor justification time",
						"Set|Get sensor justification time",
						10.0,100.0,20.0,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property (g_iface,
				g_param_spec_double  ("start-threshold",
						"start threshold",
						"Set|Get integration start threshold",
						-1.0,1.0,0.002,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property (g_iface,
				g_param_spec_double  ("stop-threshold",
						"stop threshold",
						"Set|Get integration stop threshold",
						-1.0,1.0,0.003,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property (g_iface,
				g_param_spec_double  ("start-min-time",
						"start min time",
						"Set|Get integration start min",
						0.0,500.0,10.0,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property (g_iface,
				g_param_spec_double  ("stop-max-time",
						"stop max time",
						"Set|Get integration stop max",
						10.,500.0,120.0,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property (g_iface,
				g_param_spec_double  ("stop-min-time",
						"stop min time",
						"Set|Get integration stop min",
						0.0,500.0,60.0,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property (g_iface,
				g_param_spec_uint("trigger",
						"Signal trigger",
						"Signal trigger",
						0,G_MAXUINT32,0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT  | MKT_MODEL_DB_PROP ));
		g_object_interface_install_property (g_iface,
				g_param_spec_boolean("activated",
						"Is activated",
						"Is activated",
						FALSE,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT  | MKT_MODEL_DB_PROP ));

		is_ultimate_integration_initialized = TRUE;


	}
	MUTEX_UNLOCK();
}

GType
ultimate_integration_get_type (void)
{
	static GType iface_type = 0;
	if (iface_type == 0)
	{
		static const GTypeInfo info = {
				sizeof (UltimateIntegrationInterface),
				(GBaseInitFunc) ultimate_integration_base_init,
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
			iface_type = g_type_register_static (G_TYPE_INTERFACE, "UltimateIntegrationInterface",&info, 0);
		}
		MUTEX_UNLOCK();
	}
	return iface_type;
}


gdouble
ultimate_integration_justification_time           ( UltimateIntegration *uintegration )
{
	g_return_val_if_fail(uintegration != NULL,0.);
	g_return_val_if_fail(ULTIMATE_IS_INTEGRATION(uintegration),0.);
	if(ULTIMATE_INTEGRATION_GET_INTERFACE(uintegration)->justification_time)
		return ULTIMATE_INTEGRATION_GET_INTERFACE(uintegration)->justification_time(uintegration);
	return 0.0;
}


gdouble
ultimate_integration_start_threshold              ( UltimateIntegration *uintegration )
{
	g_return_val_if_fail(uintegration != NULL,0.);
	g_return_val_if_fail(ULTIMATE_IS_INTEGRATION(uintegration),0.);
	if(ULTIMATE_INTEGRATION_GET_INTERFACE(uintegration)->start_threshold)
		return ULTIMATE_INTEGRATION_GET_INTERFACE(uintegration)->start_threshold(uintegration);
	return 0.0;
}

gdouble
ultimate_integration_stop_threshold               ( UltimateIntegration *uintegration )
{
	g_return_val_if_fail(uintegration != NULL,0.);
	g_return_val_if_fail(ULTIMATE_IS_INTEGRATION(uintegration),0.);
	if(ULTIMATE_INTEGRATION_GET_INTERFACE(uintegration)->start_threshold)
		return ULTIMATE_INTEGRATION_GET_INTERFACE(uintegration)->start_threshold(uintegration);
	return 0.0;
}


gdouble
ultimate_integration_t_start_min                  ( UltimateIntegration *uintegration )
{
	g_return_val_if_fail(uintegration != NULL,0.);
	g_return_val_if_fail(ULTIMATE_IS_INTEGRATION(uintegration),0.);
	if(ULTIMATE_INTEGRATION_GET_INTERFACE(uintegration)->t_start_min)
		return ULTIMATE_INTEGRATION_GET_INTERFACE(uintegration)->t_start_min(uintegration);
	return 0.0;
}

gdouble
ultimate_integration_t_stop_max                   ( UltimateIntegration *uintegration )
{
	g_return_val_if_fail(uintegration != NULL,0.);
	g_return_val_if_fail(ULTIMATE_IS_INTEGRATION(uintegration),0.);
	if(ULTIMATE_INTEGRATION_GET_INTERFACE(uintegration)->t_stop_max)
		return ULTIMATE_INTEGRATION_GET_INTERFACE(uintegration)->t_stop_max(uintegration);
	return 0.0;
}
gdouble
ultimate_integration_t_stop_min                   ( UltimateIntegration *uintegration )
{
	g_return_val_if_fail(uintegration != NULL,0.);
	g_return_val_if_fail(ULTIMATE_IS_INTEGRATION(uintegration),0.);
	if(ULTIMATE_INTEGRATION_GET_INTERFACE(uintegration)->t_stop_min)
		return ULTIMATE_INTEGRATION_GET_INTERFACE(uintegration)->t_stop_min(uintegration);
	return 0.0;
}

guint
ultimate_integration_trigger                      ( UltimateIntegration *uintegration )
{
	g_return_val_if_fail(uintegration != NULL,0);
	g_return_val_if_fail(ULTIMATE_IS_INTEGRATION(uintegration),0);
	if(ULTIMATE_INTEGRATION_GET_INTERFACE(uintegration)->trigger)
		return ULTIMATE_INTEGRATION_GET_INTERFACE(uintegration)->trigger(uintegration);
	return 0;
}


/** @} */
