/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktPoint
 * @{
 * @file  mkt-nitri_point.c	NitriPoint model interface
 * @brief This is NitriPoint model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */


#include "mkt-point.h"
#include <market-time.h>
#include <mkt-model.h>
#include <mkt-connection.h>

#include <math.h>
#include <mkt-value.h>

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
mkt_point_base_init (gpointer g_iface)
{
	static gboolean is_mkt_point_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_mkt_point_initialized)
	{


		g_object_interface_install_property (g_iface,
				g_param_spec_uint64 ("point-ref",
						"Point reference object",
						"Set|Get Point reference object",
						0,G_MAXUINT64,0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

		g_object_interface_install_property (g_iface,
				g_param_spec_double  ("point-solution",
						"Point solution value",
						"Set|Get point solution value",
						0.01,G_MAXDOUBLE,500.0,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));

		g_object_interface_install_property (g_iface,
				g_param_spec_double  ("point-value",
						"Point average value",
						"Set|Get point average value",
						0.0,G_MAXDOUBLE,0.0,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));
		g_object_interface_install_property (g_iface,
				g_param_spec_double  ("point-cv",
						"Point cv value",
						"Set|Get point cv value",
						0.0,G_MAXDOUBLE,0.0,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));

		is_mkt_point_initialized = TRUE;
	}
	MUTEX_UNLOCK();
}

GType
mkt_point_get_type (void)
{
	static GType iface_type = 0;
	if (iface_type == 0)
	{
		static const GTypeInfo info = {
				sizeof (MktPointInterface),
				(GBaseInitFunc) mkt_point_base_init,
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
			iface_type = g_type_register_static (G_TYPE_INTERFACE, "MktPointInterface",&info, 0);
		}
		MUTEX_UNLOCK();
	}
	return iface_type;
}


gdouble
mkt_point_average                                     ( MktPoint *upoint )
{
	g_return_val_if_fail(upoint != NULL,0);
	g_return_val_if_fail(MKT_IS_POINT(upoint),0);
	if(MKT_POINT_GET_INTERFACE(upoint)->point_average)
		return MKT_POINT_GET_INTERFACE(upoint)->point_average(upoint);
	return 0;
}


gdouble
mkt_point_solution                                    ( MktPoint *upoint )
{
	g_return_val_if_fail(upoint != NULL,0);
	g_return_val_if_fail(MKT_IS_POINT(upoint),0);
	if(MKT_POINT_GET_INTERFACE(upoint)->point_solution)
		return MKT_POINT_GET_INTERFACE(upoint)->point_solution(upoint);
	return 0;
}

gdouble
mkt_point_cv                                          ( MktPoint *upoint )
{
	g_return_val_if_fail(upoint != NULL,0);
	g_return_val_if_fail(MKT_IS_POINT(upoint),0);
	if(MKT_POINT_GET_INTERFACE(upoint)->point_cv)
		return MKT_POINT_GET_INTERFACE(upoint)->point_cv(upoint);
	return 0;
}
