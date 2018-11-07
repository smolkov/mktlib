/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktRelay
 * @{
 * @file  mkt-license.c	Relay model interface
 * @brief This is Relay model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */


#include "mkt-license.h"
#include "mkt-license-model.h"

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
mkt_license_base_init (gpointer g_iface)
{
	static gboolean is_license_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_license_initialized)
	{
		g_object_interface_install_property (g_iface,
				g_param_spec_string ("license-string",
						_("Relay string"),
						_("Set get license string parameter"),
						"--",
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE ));
		g_object_interface_install_property (g_iface,
				g_param_spec_boolean ("license-warning",
						_("Relay all warning errors"),
						_("Set get license all warning errors"),
						FALSE,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE  ));
		g_object_interface_install_property (g_iface,
				g_param_spec_boolean("license-critical",
						_("Relay all critical errors"),
						_("Set get license all critical errors"),
						FALSE,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE ));

		is_license_initialized = TRUE;
	}
	MUTEX_UNLOCK();
}


GType
mkt_license_get_type (void)
{
	static GType iface_type = 0;
	if (iface_type == 0)
	{
		static const GTypeInfo info = {
				sizeof (MktRelayInterface),
				(GBaseInitFunc) mkt_license_base_init,
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
			iface_type = g_type_register_static (G_TYPE_INTERFACE, "MktRelayInterface",&info, 0);
			g_type_interface_add_prerequisite (iface_type, MKT_TYPE_MODEL);
		}
		MUTEX_UNLOCK();
	}
	return iface_type;
}


const gchar*
mkt_license_string                       ( MktRelay *license )
{
	g_return_val_if_fail(license != NULL , NULL);
	g_return_val_if_fail(MKT_IS_RELAY(license) , NULL);
	if(MKT_RELAY_GET_INTERFACE(license)->license_string )
		return MKT_RELAY_GET_INTERFACE(license)->license_string(license);
	return NULL;
}



/** @} */
