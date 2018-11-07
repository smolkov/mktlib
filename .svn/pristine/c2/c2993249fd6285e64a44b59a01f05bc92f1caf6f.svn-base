/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktCategory
 * @{
 * @file  mkt-category.c	Category model interface
 * @brief This is Category model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */


#include "mkt-category.h"
#include "market-time.h"
#include "mkt-value.h"



#include "../config.h"
#include <glib/gi18n-lib.h>
#include "mkt-statistic-model.h"

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
mkt_category_base_init (gpointer g_iface)
{
	static gboolean is_category_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_category_initialized)
	{

		g_object_interface_install_property (g_iface,
				g_param_spec_double ("category-interval",
						"daily start actions",
						"Set get measurement daily start actions property",
						-10.0,G_MAXDOUBLE,1800.,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE ));
		g_object_interface_install_property (g_iface,
				g_param_spec_boolean ("remote-control",
						"remote control",
						"Set get remote control",
						FALSE,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE ));
		g_object_interface_install_property (g_iface,
				g_param_spec_boolean ("category-online",
						"is online",
						"Set get sensor measurement property",
						FALSE,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE  ));
		g_object_interface_install_property (g_iface,
				g_param_spec_uint("category-runs",
						"Category statistic counter",
						"Set Get statistic counter",
						0,G_MAXUINT32,0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP ));

		g_object_interface_install_property (g_iface,
				g_param_spec_double ("category-starttime",
						"Measurement daily start actions property",
						"Set get measurement daily start actions property",
						-10.0,G_MAXDOUBLE,0.0,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));

		is_category_initialized = TRUE;
	}
	MUTEX_UNLOCK();
}

GType
mkt_category_get_type (void)
{
	static GType iface_type = 0;
	if (iface_type == 0)
	{
		static const GTypeInfo info = {
				sizeof (MktCategoryInterface),
				(GBaseInitFunc) mkt_category_base_init,
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
			iface_type = g_type_register_static (G_TYPE_INTERFACE, "MktCategoryInterface",&info, 0);
			g_type_interface_add_prerequisite (iface_type, MKT_TYPE_MODEL);
		}
		MUTEX_UNLOCK();
	}
	return iface_type;
}


gdouble
mkt_category_interval                              ( MktCategory *category )
{
	g_return_val_if_fail(category != NULL,0.);
	g_return_val_if_fail(MKT_IS_CATEGORY(category),0.);
	if(MKT_CATEGORY_GET_INTERFACE(category)->interval)
		return MKT_CATEGORY_GET_INTERFACE(category)->interval(category);
	return 0.0;
}


/** @} */
