/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktParam
 * @{
 * @file  mkt-param.c	Pc model interface
 * @brief This is PARAM model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */

#include "market-time.h"
#include "mkt-param.h"
#include <math.h>




#if GLIB_CHECK_VERSION(2,31,7)
static GRecMutex init_rmutex;
#define MUTEX_LOCK() g_rec_mutex_lock(&init_rmutex)
#define MUTEX_UNLOCK() g_rec_mutex_unlock(&init_rmutex)
#else
static GStaticRecMutex init_mutex = G_STATIC_REC_MUTEX_INIT;
#define MUTEX_LOCK() g_static_rec_mutex_lock(&init_mutex)
#define MUTEX_UNLOCK() g_static_rec_mutex_unlock(&init_mutex)
#endif


/* signals */
enum {
	DATE_CHANGED,
	LAST_SIGNAL
};

//static guint mkt_param_iface_signals[LAST_SIGNAL];

static void
mkt_param_base_init (gpointer g_iface)
{
	static gboolean is_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_initialized)
	{
		GParamSpec *pspec;
		pspec = g_param_spec_string("param-object-id",
				"Param object id",
				"Param object id",
				"com.lar.manager.id",
				G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  );
				g_object_interface_install_property (g_iface, pspec);

		pspec = g_param_spec_string("param-object-path",
				"Param object path",
				"Param object path",
				"/com/lar/manager/object",
				G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  );
		g_object_interface_install_property (g_iface, pspec);


		pspec = g_param_spec_string("param-db-reference",
				"Param db reference",
				"Param sqlite db referense",
				"Db.MktParamTable.ID",
				G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  );
		g_object_interface_install_property (g_iface, pspec);

		pspec = g_param_spec_boolean("param-activated",
				"Param activated ",
				"Set|Get activated",
				TRUE,
				G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  );
		g_object_interface_install_property (g_iface, pspec);

		pspec = g_param_spec_string("param-type",
				"Param type",
				"Set|Get type",
				"main",
				G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  );
		g_object_interface_install_property (g_iface, pspec);

		g_object_interface_install_property (g_iface,
				g_param_spec_double ("param-changed",
						"Param changed date in double",
						"Set changed date in double seconds.nano-seconds property",
						0.,G_MAXDOUBLE,0.,
						G_PARAM_READWRITE | MKT_MODEL_DB_PROP  ));
		pspec = g_param_spec_string ("param-name",
				"Param name prop",
				"Set|Get name property",
				"Param",
				G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP );
		g_object_interface_install_property (g_iface, pspec);

		pspec = g_param_spec_string ("param-description",
				"Param description prop",
				"Set|Get description property",
				"--",
				G_PARAM_READWRITE |  G_PARAM_CONSTRUCT |  MKT_MODEL_DB_PROP );
		g_object_interface_install_property (g_iface, pspec);

		pspec = g_param_spec_string ("param-value",
				"Param value variant",
				"Set|Get param value variant",
				"--",
				G_PARAM_READWRITE |  G_PARAM_CONSTRUCT |  MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE );
		g_object_interface_install_property (g_iface, pspec);
		is_initialized = TRUE;
	}

	MUTEX_UNLOCK();
}

GType
mkt_param_get_type (void)
{
	static GType iface_type = 0;
	if (iface_type == 0)
	{
		static const GTypeInfo info = {
				sizeof (MktParamInterface),
				(GBaseInitFunc) mkt_param_base_init,
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
			iface_type = g_type_register_static (G_TYPE_INTERFACE, "MktParamInterface",&info, 0);
		}
		MUTEX_UNLOCK();
	}
	return iface_type;
}

const gchar*
mkt_param_object_path                    ( MktParam *param )
{
	g_return_val_if_fail(param!=NULL,NULL);
	g_return_val_if_fail(MKT_IS_PARAM(param),NULL);
	if(MKT_PARAM_GET_INTERFACE(param)->object_path )
		return MKT_PARAM_GET_INTERFACE(param)->object_path(param);
	return NULL;
}



gboolean
mkt_param_activated                     ( MktParam *param )
{
	g_return_val_if_fail(param!=NULL,FALSE);
	g_return_val_if_fail(MKT_IS_PARAM(param),FALSE);
	if(MKT_PARAM_GET_INTERFACE(param)->activated )
		return MKT_PARAM_GET_INTERFACE(param)->activated(param);
	return FALSE;
}

const gchar*
mkt_param_type                  ( MktParam *param )
{
	g_return_val_if_fail(param!=NULL,NULL);
	g_return_val_if_fail(MKT_IS_PARAM(param),NULL);
	if(MKT_PARAM_GET_INTERFACE(param)->get_type )
		return MKT_PARAM_GET_INTERFACE(param)->get_type(param);
	return NULL;
}

gdouble
mkt_param_changed                       ( MktParam *param )
{
	g_return_val_if_fail(param!=NULL,0.);
	g_return_val_if_fail(MKT_IS_PARAM(param),0.);
	if(MKT_PARAM_GET_INTERFACE(param)->get_changed)
		return MKT_PARAM_GET_INTERFACE(param)->get_changed(param);
	return 0.0;
}

const gchar*
mkt_param_name                          ( MktParam *param )
{
	g_return_val_if_fail(param!=NULL,NULL);
	g_return_val_if_fail(MKT_IS_PARAM(param),NULL);
	if(MKT_PARAM_GET_INTERFACE(param)->get_name )
		return MKT_PARAM_GET_INTERFACE(param)->get_name(param);
	return NULL;
}

const gchar*
mkt_param_description                   ( MktParam *param )
{
	g_return_val_if_fail(param!=NULL,NULL);
	g_return_val_if_fail(MKT_IS_PARAM(param),NULL);
	if(MKT_PARAM_GET_INTERFACE(param)->get_description )
		return MKT_PARAM_GET_INTERFACE(param)->get_description(param);
	return NULL;
}

void
mkt_param_activate                      ( MktParam *param )
{
	g_return_if_fail(param!=NULL);
	g_return_if_fail(MKT_IS_PARAM(param));
	if(MKT_PARAM_GET_INTERFACE(param)->activate )
		MKT_PARAM_GET_INTERFACE(param)->activate(param);
}


const gchar*
mkt_param_value                          ( MktParam *param )
{
	g_return_val_if_fail(param!=NULL,NULL);
	g_return_val_if_fail(MKT_IS_PARAM(param),NULL);
	if(MKT_PARAM_GET_INTERFACE(param)->get_value )
		return MKT_PARAM_GET_INTERFACE(param)->get_value(param);
	return NULL;
}


void
mkt_param_update_changed                 ( MktParam *param )
{
	g_return_if_fail(param!=NULL);
	g_return_if_fail(MKT_IS_PARAM(param));
	g_object_set(param,"param-changed",market_db_time_now(),NULL);
}





/** @} */
