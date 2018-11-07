/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktStream
 * @{
 * @file  mkt-stream.c	Stream model interface
 * @brief This is Stream model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */


#include "market-time.h"
#include "mkt-value.h"
#include "mkt-stream.h"
#include "../config.h"
#include <glib/gi18n-lib.h>
#include "mkt-channel-model.h"
#include "mkt-stream-model.h"


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
mkt_stream_base_init (gpointer g_iface)
{
	static gboolean is_stream_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_stream_initialized)
	{
		g_object_interface_install_property (g_iface,
				g_param_spec_string("stream-name",
						"Stream name",
						"Set get stream dbus interface name",
						"com.lar.Streams.ObjectManager.Stream1",
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT |  MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE ));

		is_stream_initialized = TRUE;
	}
	MUTEX_UNLOCK();
}

GType
mkt_stream_get_type (void)
{
	static GType iface_type = 0;
	if (iface_type == 0)
	{
		static const GTypeInfo info = {
				sizeof (MktStreamInterface),
				(GBaseInitFunc) mkt_stream_base_init,
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
			iface_type = g_type_register_static (G_TYPE_INTERFACE, "MktStreamInterface",&info, 0);
			g_type_interface_add_prerequisite (iface_type, MKT_TYPE_MODEL);
		}
		MUTEX_UNLOCK();
	}
	return iface_type;
}


GSList*
mkt_stream_channels                        ( MktStream *stream )
{
	g_return_val_if_fail(stream != NULL,NULL);
	g_return_val_if_fail(MKT_IS_STREAM(stream),NULL);
	if(MKT_STREAM_GET_INTERFACE(stream)->channels )
		return MKT_STREAM_GET_INTERFACE(stream)->channels(stream);
	return NULL;
}


/** @} */
