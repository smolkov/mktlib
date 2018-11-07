/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktStatus
 * @{
 * @file  mkt-status.c	Status model interface
 * @brief This is Status model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */


#include "mkt-status.h"
#include "mkt-status-model.h"

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
mkt_status_base_init (gpointer g_iface)
{
	static gboolean is_status_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_status_initialized)
	{
		g_object_interface_install_property (g_iface,
				g_param_spec_string ("status-signification",
						"Status signification",
						"Set get status signification parameter",
						"P",
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  ));
		g_object_interface_install_property (g_iface,
				g_param_spec_double("status-changed",
						"Status last changed",
						"Status last changed",
						0.0,G_MAXDOUBLE,0.0,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  ));
		g_object_interface_install_property (g_iface,
				g_param_spec_boolean ("status-active",
						"Status is active",
						"Status is active",
						FALSE,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  ));
		g_object_interface_install_property (g_iface,
				g_param_spec_string ("status-description",
						"Status discription",
						"Status discription",
						"Pause",
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  ));

		is_status_initialized = TRUE;
	}
	MUTEX_UNLOCK();
}


GType
mkt_status_get_type (void)
{
	static GType iface_type = 0;
	if (iface_type == 0)
	{
		static const GTypeInfo info = {
				sizeof (MktStatusInterface),
				(GBaseInitFunc) mkt_status_base_init,
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
			iface_type = g_type_register_static (G_TYPE_INTERFACE, "MktStatusInterface",&info, 0);
			g_type_interface_add_prerequisite (iface_type, MKT_TYPE_MODEL);
		}
		MUTEX_UNLOCK();
	}
	return iface_type;
}


const gchar*
mkt_status_discription                       ( MktStatus *status )
{
	g_return_val_if_fail(status != NULL , NULL);
	g_return_val_if_fail(MKT_IS_STATUS(status) , NULL);
	if(MKT_STATUS_GET_INTERFACE(status)->status_description )
		return MKT_STATUS_GET_INTERFACE(status)->status_description(status);
	return NULL;
}


const gchar*
mkt_status_signification                      ( MktStatus *status )
{
	g_return_val_if_fail(status != NULL , "E");
	g_return_val_if_fail(MKT_IS_STATUS(status) , "E");
	if(MKT_STATUS_GET_INTERFACE(status)->status_signification )
		return MKT_STATUS_GET_INTERFACE(status)->status_signification(status);
	return "E";
}

gboolean
mkt_status_is_active                    ( MktStatus *status )
{
	g_return_val_if_fail(status != NULL , FALSE);
	g_return_val_if_fail(MKT_IS_STATUS(status) , FALSE);
	if(MKT_STATUS_GET_INTERFACE(status)->is_active )
		return MKT_STATUS_GET_INTERFACE(status)->is_active(status);
	return FALSE;
}


void
mkt_status_activate                     ( MktStatus *status  , gboolean value )
{
	g_return_if_fail(status != NULL );
	g_return_if_fail(MKT_IS_STATUS(status));
	g_object_set(status,"status-active",value,NULL);
}


void
mkt_status_switch                       ( const gchar *status, gboolean value )
{
	MktStatus *model = MKT_STATUS(mkt_model_select_one(MKT_TYPE_STATUS_MODEL,"select * from $tablename where status_signification = '%s'",status));
	if(model)
	{
		g_object_set(model,"status-active",value,NULL);
		g_object_unref(model);
	}
}

void
mkt_status_create                       ( const gchar *status ,const gchar *format,...  )
{
	va_list args;
	gchar  *description;
	va_start (args, format);
	description =g_strdup_vprintf (format, args);
	va_end (args);

	MktStatus *model = MKT_STATUS(mkt_model_select_one(MKT_TYPE_STATUS_MODEL,"select * from $tablename where status_signification = '%s'",status));
	if(model == NULL)
	{
		model = MKT_STATUS(mkt_model_new(MKT_TYPE_STATUS_MODEL,"status-signification",status,"status-description",description,"status-active",FALSE,NULL));
		g_object_unref(model);
	}
	else
	{
		g_object_set(model,"status-description",description,"status-active",FALSE,NULL);
		g_object_unref(model);
	}
	g_free(description);
}


MktStatus*
mkt_status_new                          ( const gchar *status,const gchar *format,...  )
{
	va_list args;
	gchar  *description;
	va_start (args, format);
	description =g_strdup_vprintf (format, args);
	va_end (args);

	MktStatus *model = MKT_STATUS(mkt_model_select_one(MKT_TYPE_STATUS_MODEL,"select * from $tablename where status_signification = '%s'",status));
	if(model == NULL)
	{
		model = MKT_STATUS(mkt_model_new(MKT_TYPE_STATUS_MODEL,"status-signification",status,"status-description",description,"status-active",FALSE,NULL));
	}
	else
	{
		g_object_set(model,"status-description",description,"status-active",FALSE,NULL);
	}
	g_free(description);
	return model;
}


/** @} */
