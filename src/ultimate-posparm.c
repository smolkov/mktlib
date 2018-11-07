/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup UltimatePosparm
 * @{
 * @file  mkt-nitri_posparm.c	NitriStream model interface
 * @brief This is NitriStream model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */


#include "ultimate-posparm.h"
#include "ultimate-posparm-object.h"
#include <market-time.h>

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
ultimate_posparm_base_init (gpointer g_iface)
{
	static gboolean is_ultimate_posparm_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_ultimate_posparm_initialized)
	{


		g_object_interface_install_property (g_iface,
				g_param_spec_string ("online-vessel",
						"sample vessel reference",
						"Set|Get posparm sample vessel reference",
						"/com/lar/tera/vessels/2",
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT |  MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property (g_iface,
				g_param_spec_string ("drain-vessel",
						"drain vessel reference",
						"Set|Get posparm drain vessel reference",
						"/com/lar/tera/vessels/5",
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT |  MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property (g_iface,
				g_param_spec_string ("calibration-vessel",
						"calibration vessel reference",
						"Set|Get posparm calibration vessel reference",
						"/com/lar/tera/vessels/1",
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT |  MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property (g_iface,
				g_param_spec_string ("single-vessel",
						"single vessel reference",
						"Set|Get posparm single vessel reference",
						"/com/lar/tera/vessels/1",
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT |  MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property (g_iface,
				g_param_spec_string ("check-vessel",
						"check vessel reference",
						"Set|Get posparm check vessel reference",
						"/com/lar/tera/vessels/1",
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT |  MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property (g_iface,
				g_param_spec_string ("dilution-vessel",
						"dilution vessel reference",
						"Set|Get posparm dilution vessel reference",
						"/com/lar/tera/vessels/4",
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT |  MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE));
		is_ultimate_posparm_initialized = TRUE;
	}
	MUTEX_UNLOCK();
}

GType
ultimate_posparm_get_type (void)
{
	static GType iface_type = 0;
	if (iface_type == 0)
	{
		static const GTypeInfo info = {
				sizeof (UltimatePosparmInterface),
				(GBaseInitFunc) ultimate_posparm_base_init,
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
			iface_type = g_type_register_static (G_TYPE_INTERFACE, "UltimatePosparmInterface",&info, 0);
		}
		MUTEX_UNLOCK();
	}
	return iface_type;
}


/** @} */
