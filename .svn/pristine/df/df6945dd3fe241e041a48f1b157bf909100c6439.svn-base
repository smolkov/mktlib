/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktRelay
 * @{
 * @file  mkt-relay.c	Relay model interface
 * @brief This is Relay model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */


#include "mkt-relay.h"
#include "mkt-relay-model.h"

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
mkt_relay_base_init (gpointer g_iface)
{
	static gboolean is_relay_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_relay_initialized)
	{
		g_object_interface_install_property (g_iface,
				g_param_spec_string ("relay-string",
						_("Relay string"),
						_("Set get relay string parameter"),
						"",
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE ));
		g_object_interface_install_property (g_iface,
				g_param_spec_boolean ("relay-warning",
						_("Relay all warning errors"),
						_("Set get relay all warning errors"),
						FALSE,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE  ));
		g_object_interface_install_property (g_iface,
				g_param_spec_boolean("relay-critical",
						_("Relay all critical errors"),
						_("Set get relay all critical errors"),
						FALSE,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE ));

		is_relay_initialized = TRUE;
		
	}
	MUTEX_UNLOCK();
}


GType
mkt_relay_get_type (void)
{
	static GType iface_type = 0;
	if (iface_type == 0)
	{
		static const GTypeInfo info = {
				sizeof (MktRelayInterface),
				(GBaseInitFunc) mkt_relay_base_init,
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
mkt_relay_string                       ( MktRelay *relay )
{
	g_return_val_if_fail(relay != NULL , NULL);
	g_return_val_if_fail(MKT_IS_RELAY(relay) , NULL);
	if(MKT_RELAY_GET_INTERFACE(relay)->relay_string )
		return MKT_RELAY_GET_INTERFACE(relay)->relay_string(relay);
	return NULL;
}



/** @} */
