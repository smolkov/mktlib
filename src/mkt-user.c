/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktUser
 * @{
 * @file  mkt-user.c	User model interface
 * @brief This is User model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */


#include "mkt-user.h"
#include "mkt-user-model.h"

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
mkt_user_base_init (gpointer g_iface)
{
	static gboolean is_user_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_user_initialized)
	{

		g_object_interface_install_property (g_iface,
				g_param_spec_string ("user-name",
						"User name",
						"Set get user name parameter",
						"guest",
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  ));
		g_object_interface_install_property (g_iface,
				g_param_spec_string ("user-password",
						_("User password"),
						_("Set get user password parameter"),
						"***",
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  | MKT_MODEL_DB_LOG_CHANGE  ));
//		g_object_interface_install_property (g_iface,
//				g_param_spec_boolean ("user-autologin",
//						_("User is auto login"),
//						_("Set get user is auto login"),
//						FALSE,
//						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  ));
		g_object_interface_install_property (g_iface,
				g_param_spec_uint ("user-level",
						"User level",
						"Set get user level",
						1,5,1,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  ));

		g_object_interface_install_property (g_iface,
				g_param_spec_boolean ("user-activated",
						_("User is activated"),
						_("User is activated"),
						FALSE,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE ));

		is_user_initialized = TRUE;
	}
	MUTEX_UNLOCK();
}


GType
mkt_user_get_type (void)
{
	static GType iface_type = 0;
	if (iface_type == 0)
	{
		static const GTypeInfo info = {
				sizeof (MktUserInterface),
				(GBaseInitFunc) mkt_user_base_init,
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
			iface_type = g_type_register_static (G_TYPE_INTERFACE, "MktUserInterface",&info, 0);
			g_type_interface_add_prerequisite (iface_type, MKT_TYPE_MODEL);
		}
		MUTEX_UNLOCK();
	}
	return iface_type;
}


const gchar*
mkt_user_name                       ( MktUser *user )
{
	g_return_val_if_fail(user != NULL , NULL);
	g_return_val_if_fail(MKT_IS_USER(user) , NULL);
	if(MKT_USER_GET_INTERFACE(user)->user_name )
		return MKT_USER_GET_INTERFACE(user)->user_name(user);
	return NULL;
}


const gchar*
mkt_user_password                       ( MktUser *user )
{
	g_return_val_if_fail(user != NULL , NULL);
	g_return_val_if_fail(MKT_IS_USER(user) , NULL);
	if(MKT_USER_GET_INTERFACE(user)->user_password )
		return MKT_USER_GET_INTERFACE(user)->user_password(user);
	return NULL;
}


/** @} */
