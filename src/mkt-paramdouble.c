/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktParamdouble
 * @{
 * @file  mkt-paramdouble.c	Pc model interface
 * @brief This is PARAMDOUBLE model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */

#include "mkt-paramdouble.h"
#include "mkt-paramdouble-model.h"
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
mkt_paramdouble_base_init (gpointer g_iface)
{
	static gboolean is_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_initialized)
	{
		g_object_interface_install_property (g_iface,
				g_param_spec_double ("value",
					 	_("value"),
						_("value"),
						-G_MAXDOUBLE,G_MAXDOUBLE,0.0,
						G_PARAM_READWRITE | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE ));

		is_initialized = TRUE;
	}

	MUTEX_UNLOCK();
}

GType
mkt_paramdouble_get_type (void)
{
	static GType iface_type = 0;
	if (iface_type == 0)
	{
		static const GTypeInfo info = {
				sizeof (MktParamdoubleInterface),
				(GBaseInitFunc) mkt_paramdouble_base_init,
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
			iface_type = g_type_register_static (G_TYPE_INTERFACE, "MktParamdoubleInterface",&info, 0);
		}
		MUTEX_UNLOCK();
	}
	return iface_type;
}


gdouble
mkt_paramdouble_value                     ( MktParamdouble *paramdouble )
{
	g_return_val_if_fail(paramdouble!=NULL,0.);
	g_return_val_if_fail(MKT_IS_PARAMDOUBLE(paramdouble),0.);
	if(MKT_PARAMDOUBLE_GET_INTERFACE(paramdouble)->get_value )
		return MKT_PARAMDOUBLE_GET_INTERFACE(paramdouble)->get_value(paramdouble);
	return 0.0;
}

MktParamdouble*
mkt_paramdouble_get               ( const gchar *id, const gchar *path,  const gchar *name )
{
	g_return_val_if_fail(id!=NULL,NULL);
	g_return_val_if_fail(path!=NULL,NULL);
	g_return_val_if_fail(name!=NULL,NULL);
	MktParamdouble * param = MKT_PARAMDOUBLE(mkt_model_select_one(MKT_TYPE_PARAMDOUBLE_MODEL,"select * from $tablename where param_activated = 1 and param_object_id = '%s' and param_object_path = '%s' and param_name = '%s'",id,path,name));
	return param;
}

gdouble
mkt_paramdouble_get_value         ( const gchar *id, const gchar *path, const gchar *name  )
{
	MktParamdouble *param = mkt_paramdouble_get(id,path,name);
	if(param == NULL)
	{
		g_warning("param %s - %s - %s not found",id,path,name);
		return 0.0;
	}
	gdouble value = mkt_paramdouble_value(param);
	g_object_unref(param);
	return value;
}

void
mkt_paramdouble_set_value         ( MktParamdouble *paramdouble, gdouble value )
{
	g_return_if_fail(paramdouble!=NULL);
	g_return_if_fail(MKT_IS_PARAMDOUBLE(paramdouble));
	g_object_set(paramdouble,"value",value,NULL);
}


/** @} */
