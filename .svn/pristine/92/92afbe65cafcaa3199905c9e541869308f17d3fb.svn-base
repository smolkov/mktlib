/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktParamuint32
 * @{
 * @file  mkt-paramuint32.c	Pc model interface
 * @brief This is PARAMUINT32 model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */

#include "mkt-paramuint32.h"
#include "mkt-paramuint32-model.h"

#include <math.h>


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
mkt_paramuint32_base_init (gpointer g_iface)
{
	static gboolean is_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_initialized)
	{
		g_object_interface_install_property (g_iface,
				g_param_spec_uint ("value",
					 	_("value"),
						_("value "),
						0,G_MAXUINT32,0,
						G_PARAM_READWRITE | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE ));

		is_initialized = TRUE;
	}

	MUTEX_UNLOCK();
}

GType
mkt_paramuint32_get_type (void)
{
	static GType iface_type = 0;
	if (iface_type == 0)
	{
		static const GTypeInfo info = {
				sizeof (MktParamuint32Interface),
				(GBaseInitFunc) mkt_paramuint32_base_init,
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
			iface_type = g_type_register_static (G_TYPE_INTERFACE, "MktParamuint32Interface",&info, 0);
		}
		MUTEX_UNLOCK();
	}
	return iface_type;
}


guint
mkt_paramuint32_value                     ( MktParamuint32 *paramuint32 )
{
	g_return_val_if_fail(paramuint32!=NULL,0);
	g_return_val_if_fail(MKT_IS_PARAMUINT32(paramuint32),0);
	if(MKT_PARAMUINT32_GET_INTERFACE(paramuint32)->value )
		return MKT_PARAMUINT32_GET_INTERFACE(paramuint32)->value(paramuint32);
	return FALSE;
}

MktParamuint32*
mkt_paramuint32_get               ( const gchar *id, const gchar *path,  const gchar *name )
{
	g_return_val_if_fail(id!=NULL,NULL);
	g_return_val_if_fail(path!=NULL,NULL);
	g_return_val_if_fail(name!=NULL,NULL);
	MktParamuint32 * param = MKT_PARAMUINT32(mkt_model_select_one(MKT_TYPE_PARAMUINT32_MODEL,"select * from $tablename where param_activated = 1 and param_object_id = '%s' and param_object_path = '%s' and param_name = '%s'",id,path,name));
	return param;
}

/** @} */
