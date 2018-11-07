/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktParamint32
 * @{
 * @file  mkt-paramint32.c	Pc model interface
 * @brief This is PARAMINT32 model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */

#include "mkt-paramint32.h"
#include "mkt-paramint32-model.h"
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
mkt_paramint32_base_init (gpointer g_iface)
{
	static gboolean is_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_initialized)
	{
		g_object_interface_install_property (g_iface,
				g_param_spec_int ("value",
					 	_("value"),
						_("value "),
						G_MININT32,G_MAXINT32,0,
						G_PARAM_READWRITE | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE ));

		is_initialized = TRUE;
	}

	MUTEX_UNLOCK();
}

GType
mkt_paramint32_get_type (void)
{
	static GType iface_type = 0;
	if (iface_type == 0)
	{
		static const GTypeInfo info = {
				sizeof (MktParamint32Interface),
				(GBaseInitFunc) mkt_paramint32_base_init,
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
			iface_type = g_type_register_static (G_TYPE_INTERFACE, "MktParamint32Interface",&info, 0);
		}
	}
	return iface_type;
	MUTEX_UNLOCK();
}


gint
mkt_paramint32_value                     ( MktParamint32 *paramint32 )
{
	g_return_val_if_fail(paramint32!=NULL,0);
	g_return_val_if_fail(MKT_IS_PARAMINT32(paramint32),0);
	if(MKT_PARAMINT32_GET_INTERFACE(paramint32)->value )
		return MKT_PARAMINT32_GET_INTERFACE(paramint32)->value(paramint32);
	return FALSE;
}



MktParamint32*
mkt_paramint32_get               ( const gchar *id, const gchar *path,  const gchar *name )
{
	g_return_val_if_fail(id!=NULL,NULL);
	g_return_val_if_fail(path!=NULL,NULL);
	g_return_val_if_fail(name!=NULL,NULL);
	MktParamint32 * param = MKT_PARAMINT32(mkt_model_select_one(MKT_TYPE_PARAMINT32_MODEL,"select * from $tablename where param_activated = 1 and param_object_id = '%s' and param_object_path = '%s' and param_name = '%s'",id,path,name));
	return param;
}

gint
mkt_paramint32_get_value         ( const gchar *id, const gchar *path, const gchar *name  )
{
	MktParamint32 *param = mkt_paramint32_get(id,path,name);
	if(param == NULL)
	{
		g_warning("param %s - %s - %s not found",id,path,name);
		return 0;
	}
	gint value = mkt_paramint32_value(param);
	g_object_unref(param);
	return value;
}

void
mkt_paramint32_set_value         ( MktParamint32 *paramint32, gint value )
{
	g_return_if_fail(paramint32!=NULL);
	g_return_if_fail(MKT_IS_PARAMINT32(paramint32));
	g_object_set(paramint32,"value",value,NULL);
}


/** @} */
