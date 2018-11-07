/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mkt-measurement-data.c
 * Copyright (C) sascha 2013 <sascha@sascha-VirtualBox>
 * 
mkt-measurement-data.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * mkt-measurement-data.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mkt-param-object.h"
#include "market-time.h"
#include "mkt-connection.h"
#include "mkt-value.h"
#include "mkt-model.h"

enum
{
	PROP_0,
	PROP_OBJECT_ID,
	PROP_OBJECT_PATH,
	PROP_OBJECT_REFERENCE,
	PROP_ACTIVATED,
	PROP_CHANGED,
	PROP_TYPE,
	PROP_NAME,
	PROP_VALUE,
	PROP_DESCRIPTION
};


struct _MktParamObjectPrivate
{
	gchar      *object_id;
	gchar      *object_path;
	gchar      *db_reference;
	gboolean    activated;
	gdouble     changed;
	gchar      *type;
	gchar      *name;
	gchar      *description;
	gchar      *value;
};


#define MKT_PARAM_OBJECT_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), MKT_TYPE_PARAM_OBJECT, MktParamObjectPrivate))

static const  gchar*
mkt_param_object_type ( MktParam *param )
{
	g_return_val_if_fail(param != NULL , 0);
	g_return_val_if_fail(MKT_IS_PARAM_OBJECT(param) , 0);
	return MKT_PARAM_OBJECT(param)->priv->type;
}

static gdouble
mkt_param_object_changed (MktParam *param)
{
	g_return_val_if_fail(param != NULL ,  0.0);
	g_return_val_if_fail(MKT_IS_PARAM_OBJECT(param) ,  0.0);
	return MKT_PARAM_OBJECT(param)->priv->changed;
}

static const gchar*
mkt_param_object_object_path (MktParam *param)
{
	g_return_val_if_fail(param != NULL , NULL);
	g_return_val_if_fail(MKT_IS_PARAM_OBJECT(param) , NULL);
	return MKT_PARAM_OBJECT(param)->priv->object_path;
}

static gboolean
mkt_param_object_activated (MktParam *param)
{
	g_return_val_if_fail(param != NULL , FALSE);
	g_return_val_if_fail(MKT_IS_PARAM_OBJECT(param) , FALSE);
	return MKT_PARAM_OBJECT(param)->priv->activated;
}

static const gchar*
mkt_param_object_name (MktParam *param)
{
	g_return_val_if_fail(param != NULL , NULL);
	g_return_val_if_fail(MKT_IS_PARAM_OBJECT(param) , NULL);
	return MKT_PARAM_OBJECT(param)->priv->name;
}

static const gchar*
mkt_param_object_description (MktParam *param)
{
	g_return_val_if_fail(param != NULL , NULL);
	g_return_val_if_fail(MKT_IS_PARAM_OBJECT(param) , NULL);
	return MKT_PARAM_OBJECT(param)->priv->description;
}




static void
mkt_param_object_activate (MktParam *param)
{
	g_return_if_fail(param!=NULL);
	g_return_if_fail(MKT_IS_PARAM_OBJECT(param));

	/*GType type = G_OBJECT_TYPE(G_OBJECT(param));
	GSList *params = mkt_model_select(type,"select * from $tablename");
	// Deactivate all siblings parameter
	GSList *l = NULL;
	for(l=params;l!=NULL;l=l->next)
	{
		if(l->data && MKT_IS_PARAM_OBJECT(l->data))
		{
			if(mkt_param_activated(MKT_PARAM(l->data)))mktMPSet(l->data,"param-activated",FALSE);
		}
	}
	if(params)mkt_slist_free_full(params,g_object_unref);*/
	mktMPSet(param,"param-activated",TRUE);
}

static const gchar*
mkt_param_object_value (MktParam *param)
{
	g_return_val_if_fail(param != NULL , NULL);
	g_return_val_if_fail(MKT_IS_PARAM_OBJECT(param) , NULL);
	return MKT_PARAM_OBJECT(param)->priv->value;
}

static void
mkt_param_object_init_param_interface ( MktParamInterface *iface )
{
	iface->object_path=mkt_param_object_object_path;
	iface->get_changed=mkt_param_object_changed;
	iface->get_type=mkt_param_object_type;
	iface->activated=mkt_param_object_activated;
	iface->get_name=mkt_param_object_name;
	iface->get_description=mkt_param_object_description;
	iface->activate = mkt_param_object_activate;
	iface->get_value = mkt_param_object_value;
}



G_DEFINE_TYPE_WITH_CODE (MktParamObject, mkt_param_object, MKT_TYPE_MODEL,
										G_IMPLEMENT_INTERFACE (MKT_TYPE_PARAM,
						                       mkt_param_object_init_param_interface)
						                       MKT_CREATE_MODEL_FULL(mkt_connection_new_default_parameter(TRUE)))

static void
mkt_param_object_init (MktParamObject *mkt_param_object)
{
	MktParamObjectPrivate *priv      = MKT_PARAM_OBJECT_PRIVATE(mkt_param_object);
	priv->object_id   = NULL;
	priv->object_path = NULL;
	priv->db_reference= NULL;
	priv->activated   = FALSE;
	priv->name        = NULL;
	priv->description = NULL;
	priv->changed     = market_db_time_now();
	priv->type        = NULL;
	priv->value       = g_strdup(" ");
	mkt_param_object->priv   = priv;

	/* TODO: Add initialization code here */
}

static void
mkt_param_object_finalize (GObject *object)
{
	MktParamObject *data = MKT_PARAM_OBJECT(object);
	if(data->priv->object_id)    g_free(data->priv->object_id);
	if(data->priv->db_reference) g_free(data->priv->db_reference);
	if(data->priv->description)  g_free(data->priv->description);
	if(data->priv->name)         g_free(data->priv->name);
	if(data->priv->type)         g_free(data->priv->type);
	if(data->priv->object_path)  g_free(data->priv->object_path);
	if(data->priv->value)        g_free(data->priv->value);
	G_OBJECT_CLASS (mkt_param_object_parent_class)->finalize (object);
}


static void
mkt_param_object_delete   (MktModel *model)
{
	MKT_MODEL_CLASS (mkt_param_object_parent_class)->delete (model);
}

static void
mkt_param_object_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_PARAM_OBJECT (object));
	MktParamObject *data = MKT_PARAM_OBJECT(object);
	switch (prop_id)
	{
	case PROP_OBJECT_ID:
		if(data->priv->object_id!= NULL)g_free(data->priv->object_id);
		data->priv->object_id = g_value_dup_string(value);
		break;
	case PROP_OBJECT_PATH:
		if(data->priv->object_path!= NULL)g_free(data->priv->object_path);
		data->priv->object_path = g_value_dup_string(value);
		break;
	case PROP_OBJECT_REFERENCE:
		if(data->priv->db_reference!= NULL)g_free(data->priv->db_reference);
		data->priv->db_reference = g_value_dup_string(value);
		break;
	case PROP_ACTIVATED:
		data->priv->activated = g_value_get_boolean(value);
		break;
	case PROP_TYPE:
		if(data->priv->type!= NULL)g_free(data->priv->type);
		data->priv->type = g_value_dup_string(value);
		break;
	case PROP_CHANGED:
		data->priv->changed = g_value_get_double(value);
		break;
	case PROP_NAME:
		if(data->priv->name!= NULL)g_free(data->priv->name);
		data->priv->name = g_value_dup_string(value);
		break;
	case PROP_VALUE:
		if(data->priv->value!= NULL)g_free(data->priv->value);
		data->priv->value = g_value_dup_string(value);
		break;
	case PROP_DESCRIPTION:
		if(data->priv->description!= NULL)g_free(data->priv->description);
		data->priv->description = g_value_dup_string(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_param_object_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_PARAM_OBJECT (object));
	MktParamObject *data = MKT_PARAM_OBJECT(object);
	switch (prop_id)
	{
	case PROP_OBJECT_ID:
		g_value_set_string(value , data->priv->object_id);
		break;
	case PROP_OBJECT_PATH:
		g_value_set_string(value , data->priv->object_path);
		break;
	case PROP_OBJECT_REFERENCE:
		g_value_set_string(value , data->priv->db_reference);
		break;
	case PROP_ACTIVATED:
		g_value_set_boolean(value , data->priv->activated);
		break;
	case PROP_TYPE:
		g_value_set_string(value , data->priv->type);
		break;
	case PROP_CHANGED:
		g_value_set_double(value , data->priv->changed);
		break;
	case PROP_NAME:
		g_value_set_string(value , data->priv->name);
		break;
	case PROP_VALUE:
		g_value_set_string(value , data->priv->value);
		break;
	case PROP_DESCRIPTION:
		g_value_set_string(value , data->priv->description);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_param_object_class_init (MktParamObjectClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	MktModelClass* parent_class = MKT_MODEL_CLASS (klass);
	g_type_class_add_private (klass, sizeof (MktParamObjectPrivate));
	object_class->finalize     = mkt_param_object_finalize;
	object_class->set_property = mkt_param_object_set_property;
	object_class->get_property = mkt_param_object_get_property;
	parent_class->delete       = mkt_param_object_delete;
	g_object_class_override_property(object_class,PROP_OBJECT_ID,"param-object-id");
	g_object_class_override_property(object_class,PROP_OBJECT_PATH,"param-object-path");
	g_object_class_override_property(object_class,PROP_OBJECT_REFERENCE,"param-db-reference");
	g_object_class_override_property(object_class,PROP_ACTIVATED,"param-activated");
	g_object_class_override_property(object_class,PROP_TYPE,"param-type");
	g_object_class_override_property(object_class,PROP_CHANGED,"param-changed");
	g_object_class_override_property(object_class,PROP_NAME,"param-name");
	g_object_class_override_property(object_class,PROP_DESCRIPTION,"param-description");
	g_object_class_override_property(object_class,PROP_VALUE,"param-value");
}

