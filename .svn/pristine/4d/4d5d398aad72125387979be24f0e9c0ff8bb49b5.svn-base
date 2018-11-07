/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktParamboolean
 * @{
 * @file  mkt-paramboolean.c	Pc model interface
 * @brief This is PARAMBOOLEAN model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */

#include "mkt-paramboolean.h"
#include "mkt-paramboolean-model.h"
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
mkt_paramboolean_base_init (gpointer g_iface)
{
	static gboolean is_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_initialized)
	{
		g_object_interface_install_property (g_iface,
				g_param_spec_boolean ("value",
					 	_("value"),
						_("value "),
						FALSE,
						G_PARAM_READWRITE | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE ));

		is_initialized = TRUE;
	}

	MUTEX_UNLOCK();
}

GType
mkt_paramboolean_get_type (void)
{
	static GType iface_type = 0;
	if (iface_type == 0)
	{
		static const GTypeInfo info = {
				sizeof (MktParambooleanInterface),
				(GBaseInitFunc) mkt_paramboolean_base_init,
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
			iface_type = g_type_register_static (G_TYPE_INTERFACE, "MktParambooleanInterface",&info, 0);
		}
		MUTEX_UNLOCK();
	}
	return iface_type;
}


gboolean
mkt_paramboolean_value                     ( MktParamboolean *paramboolean )
{
	g_return_val_if_fail(paramboolean!=NULL,FALSE);
	g_return_val_if_fail(MKT_IS_PARAMBOOLEAN(paramboolean),FALSE);
	if(MKT_PARAMBOOLEAN_GET_INTERFACE(paramboolean)->get_value )
		return MKT_PARAMBOOLEAN_GET_INTERFACE(paramboolean)->get_value(paramboolean);
	return FALSE;
}

MktParamboolean*
mkt_paramboolean_get               ( const gchar *id, const gchar *path,  const gchar *name )
{
	g_return_val_if_fail(id!=NULL,NULL);
	g_return_val_if_fail(path!=NULL,NULL);
	g_return_val_if_fail(name!=NULL,NULL);
	MktParamboolean * param = MKT_PARAMBOOLEAN(mkt_model_select_one(MKT_TYPE_PARAMBOOLEAN_MODEL,"select * from $tablename where param_activated = 1 and param_object_id = '%s' and param_object_path = '%s' and param_name = '%s'",id,path,name));
	return param;
}

gboolean
mkt_paramboolean_get_value         ( const gchar *id, const gchar *path, const gchar *name  )
{
	MktParamboolean *param = mkt_paramboolean_get(id,path,name);
	if(param == NULL)
	{
		g_warning("param %s - %s - %s not found",id,path,name);
		return FALSE;
	}
	gboolean value = mkt_paramboolean_value(param);
	g_object_unref(param);
	return value;
}



/** @} */
