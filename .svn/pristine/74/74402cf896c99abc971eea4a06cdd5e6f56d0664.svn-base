/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mkt-sensor-data.c
 * Copyright (C) sascha 2013 <sascha@sascha-VirtualBox>
 * 
mkt-sensor-data.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * mkt-sensor-data.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mkt-user-model.h"
#include "market-time.h"
#include "mkt-sqlite.h"



enum
{
	PROP_0,
	PROP_USER_NAME,
	PROP_USER_PASSWORD,
	PROP_USER_ACTIVATED,
	PROP_USER_LEVEL,
};


struct _MktUserModelPrivate
{
	gchar      *name;
	gchar      *password;
	gboolean    activated;
	guint       level;
};


#define MKT_USER_MODEL_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), MKT_TYPE_USER_MODEL, MktUserModelPrivate))



static const gchar*
mkt_user_object_name                ( MktUser *user )
{
	g_return_val_if_fail(user != NULL ,NULL);
	g_return_val_if_fail(MKT_IS_USER_MODEL(user) , NULL );
	return MKT_USER_MODEL(user)->priv->name;
}

static const gchar*
mkt_user_object_password                ( MktUser *user )
{
	g_return_val_if_fail(user != NULL ,NULL);
	g_return_val_if_fail(MKT_IS_USER_MODEL(user) , NULL );
	return MKT_USER_MODEL(user)->priv->password;
}

static void
mkt_user_model_init_interface ( MktUserInterface *iface )
{
	iface->user_name            = mkt_user_object_name;
	iface->user_password        = mkt_user_object_password;

}



G_DEFINE_TYPE_WITH_CODE (MktUserModel, mkt_user_model, MKT_TYPE_PARAM_OBJECT,
		                                G_IMPLEMENT_INTERFACE (MKT_TYPE_USER,
		                                		mkt_user_model_init_interface)
		                                		MKT_CREATE_MODEL_FULL(mkt_connection_new_default_parameter(TRUE)))


static void
mkt_user_model_init (MktUserModel *mkt_user_model)
{
	MktUserModelPrivate *priv      = MKT_USER_MODEL_PRIVATE(mkt_user_model);

	mkt_user_model->priv           = priv;
	mkt_user_model->priv->name     = NULL;
	mkt_user_model->priv->password = NULL;
	/* TODO: Add initialization code here */
}

static void
mkt_user_model_finalize (GObject *model)
{
	MktUserModel *user = MKT_USER_MODEL(model);
	if(user->priv->name)          g_free(user->priv->name);
	if(user->priv->password)      g_free(user->priv->password);
	G_OBJECT_CLASS (mkt_user_model_parent_class)->finalize (model);
}




static void
mkt_user_model_set_property (GObject *model, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_USER_MODEL (model));
	MktUserModel *user = MKT_USER_MODEL(model);
	switch (prop_id)
	{
	case PROP_USER_NAME:
		if(user->priv->name)g_free(user->priv->name);
		user->priv->name = g_value_dup_string(value);
		break;
	case PROP_USER_PASSWORD:
		if(user->priv->password)g_free(user->priv->password);
		user->priv->password = g_value_dup_string(value);
		break;
	case PROP_USER_ACTIVATED:
		user->priv->activated = g_value_get_boolean(value);
		break;
	case PROP_USER_LEVEL:
		user->priv->level = g_value_get_uint(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (model, prop_id, pspec);
		break;
	}
}

static void
mkt_user_model_get_property (GObject *model, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_USER_MODEL (model));
	MktUserModel *stream = MKT_USER_MODEL(model);
	switch (prop_id)
	{

	case PROP_USER_NAME:
		g_value_set_string(value,stream->priv->name);
		break;
	case PROP_USER_PASSWORD:
		g_value_set_string(value,stream->priv->password);
		break;
	case PROP_USER_ACTIVATED:
		g_value_set_boolean(value,stream->priv->activated);
		break;
	case PROP_USER_LEVEL:
		g_value_set_uint(value,stream->priv->level);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (model, prop_id, pspec);
		break;
	}
}

static void actualizeTable (void)
{
	GObject*   model      = g_object_new   (MKT_TYPE_USER_MODEL, NULL);
	MktSqlite* connection = mkt_sqlite_new (model, "/usr/share/tera/db/device-parameter.database");

	mkt_sqlite_open (connection);

	mkt_sqlite_exec (
		connection,
		"%s",
		"DELETE FROM MktUserModel "
		"WHERE '/com/lar/tera/users/level3' = param_object_path "
		"OR " "'/com/lar/tera/users/level4' = param_object_path "
		"OR " "'/com/lar/tera/users/level5' = param_object_path;"
	);

	mkt_sqlite_exec (
		connection,
		"%s",
		"UPDATE MktUserModel "
		"SET user_password = NULL "
		"WHERE param_object_path = '/com/lar/tera/users/guest';"
	);

	mkt_sqlite_exec (
		connection,
		"%s",
		"UPDATE MktUserModel "
		"SET ref_id ="
		"(SELECT COUNT (*) FROM MktUserModel AS SUB WHERE SUB.ref_id <= MktUserModel.ref_id);"
	);

	// Durch das mit angegebene Attribut 'ref_id' mit den Werten 3 und 4 schlägt
	// das Hinzufügen fehl, wenn ein Datensatz oder Datensätze mit diesen Werten
	// für 'ref_id' schon vorhanden sind.
	mkt_sqlite_exec (
		connection,
		"%s",
		"INSERT INTO MktUserModel ("
			"ref_id, "
			"param_object_path, "
			"user_level, "
			"user_name, "
			"user_password"
		") VALUES ("
			"3, "
			"'/com/lar/tera/users/expert', "
			"3, "
			"'Expert Level', "
			"NULL"
		");"
	);

	mkt_sqlite_exec (
		connection,
		"%s",
		"INSERT INTO MktUserModel ("
			"ref_id, "
			"param_object_path, "
			"user_level, "
			"user_name, "
			"user_password"
		") VALUES ("
			"4, "
			"'/com/lar/tera/users/service', "
			"4, "
			"'Service', "
			"NULL"
		");"
	);

	mkt_sqlite_close (connection);
}

static void
mkt_user_model_class_init (MktUserModelClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//MktModelClass* parent_class = MKT_MODEL_CLASS (klass);
	g_type_class_add_private (klass, sizeof (MktUserModelPrivate));
	object_class->finalize     = mkt_user_model_finalize;
	object_class->set_property = mkt_user_model_set_property;
	object_class->get_property = mkt_user_model_get_property;
	g_object_class_override_property(object_class,PROP_USER_NAME,"user-name");
	g_object_class_override_property(object_class,PROP_USER_PASSWORD,"user-password");
	g_object_class_override_property(object_class,PROP_USER_ACTIVATED,"user-activated");
	g_object_class_override_property(object_class,PROP_USER_LEVEL,"user-level");

	actualizeTable ();
}

