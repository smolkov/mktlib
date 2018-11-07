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

#include "mkt-error-message.h"
#include "market-time.h"
#include "mkt-connection.h"

enum
{
	PROP_0,
	PROP_NUMBER,
	PROP_PENDING,
	PROP_TYPE,
	PROP_TIMESTAMP,
	PROP_CHANGED,
	PROP_DESCRIPTION,
	PROP_SERVICE,
};


struct _MktErrorMessagePrivate
{
	gint       number;
	gboolean   pending;
	gdouble    changed;
	gchar     *time;
	gint       type;
	gchar     *description;
	gchar     *service;
};


#define MKT_ERROR_MESSAGE_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), MKT_TYPE_ERROR_MESSAGE, MktErrorMessagePrivate))

static gint
mkt_error_message_number ( MktError *data )
{
	g_return_val_if_fail(data != NULL , 0);
	g_return_val_if_fail(MKT_IS_ERROR_MESSAGE(data) , 0);

	return MKT_ERROR_MESSAGE(data)->priv->number;
}

static gboolean
mkt_error_message_pending (MktError *data)
{
	g_return_val_if_fail(data != NULL , FALSE);
	g_return_val_if_fail(MKT_IS_ERROR_MESSAGE(data) , FALSE);
	return MKT_ERROR_MESSAGE(data)->priv->pending;
}

static gdouble
mkt_error_message_changed (MktError *data)
{
	g_return_val_if_fail(data != NULL ,  0.0);
	g_return_val_if_fail(MKT_IS_ERROR_MESSAGE(data) ,  0.0);
	return MKT_ERROR_MESSAGE(data)->priv->changed;
}

static gchar
mkt_error_message_type ( MktError *data )
{
	g_return_val_if_fail(data != NULL , 0);
	g_return_val_if_fail(MKT_IS_ERROR_MESSAGE(data) , 0);
	return MKT_ERROR_MESSAGE(data)->priv->type;
}

static gchar*
mkt_error_message_description ( MktError *data )
{
	g_return_val_if_fail(data != NULL , 0);
	g_return_val_if_fail(MKT_IS_ERROR_MESSAGE(data) , 0);
	return MKT_ERROR_MESSAGE(data)->priv->description;
}

static void
mkt_error_message_init_interface ( MktErrorInterface *iface )
{
	iface->get_number=mkt_error_message_number;
	iface->get_pending=mkt_error_message_pending;
	iface->get_changed=mkt_error_message_changed;
	iface->get_type=mkt_error_message_type;
	//iface->get_error_no=mkt_error_message_get_text_number;
	iface->description=mkt_error_message_description;
}


G_DEFINE_TYPE_WITH_CODE (MktErrorMessage, mkt_error_message, MKT_TYPE_MODEL,
		                                G_IMPLEMENT_INTERFACE (MKT_TYPE_ERROR,
				                               mkt_error_message_init_interface)
				                               MKT_CREATE_MODEL_FULL(mkt_connection_get_errors(TRUE)))

static void
mkt_error_message_init (MktErrorMessage *mkt_error_message)
{
	MktErrorMessagePrivate *priv      = MKT_ERROR_MESSAGE_PRIVATE(mkt_error_message);
	priv->number      = 0;
	priv->pending     = FALSE;
	priv->changed     = market_db_time_now();
	priv->type        = 0;
	priv->description = g_strdup("Set error description here");
	priv->service     = g_strdup("com.lar.tera.general");
	mkt_error_message->priv   = priv;

	/* TODO: Add initialization code here */
}

static void
mkt_error_message_finalize (GObject *object)
{
	MktErrorMessage *data = MKT_ERROR_MESSAGE(object);
	if(data->priv->description)  g_free(data->priv->description);
	if(data->priv->service)      g_free(data->priv->service);
	G_OBJECT_CLASS (mkt_error_message_parent_class)->finalize (object);
}


static void
mkt_error_message_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_ERROR_MESSAGE (object));
	MktErrorMessage *data = MKT_ERROR_MESSAGE(object);
	switch (prop_id)
	{
	case PROP_NUMBER:
		data->priv->number = g_value_get_int(value);
		break;
	case PROP_PENDING:
		data->priv->pending = g_value_get_boolean(value);
		break;
	case PROP_CHANGED:
		data->priv->changed = g_value_get_double(value);
		break;
	case PROP_TYPE:
	#if GLIB_CHECK_VERSION(2,31,7)
		data->priv->type = g_value_get_schar(value);
	#else
		data->priv->type = g_value_get_schar(value);
	#endif
		break;
	case PROP_DESCRIPTION:
		if(data->priv->description!= NULL)g_free(data->priv->description);
		data->priv->description = g_value_dup_string(value);
		break;
	case PROP_SERVICE:
		if(data->priv->service!= NULL)g_free(data->priv->service);
		data->priv->service = g_value_dup_string(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_error_message_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_ERROR_MESSAGE (object));
	MktErrorMessage *data = MKT_ERROR_MESSAGE(object);
	switch (prop_id)
	{
	case PROP_NUMBER:
		g_value_set_int(value , data->priv->number);
		break;
	case PROP_PENDING:
		g_value_set_boolean(value , data->priv->pending);
		break;
	case PROP_CHANGED:
		g_value_set_double(value , data->priv->changed);
		break;
	case PROP_TYPE:
	#if GLIB_CHECK_VERSION(2,31,7)
		g_value_set_schar(value , data->priv->type);
	#else
		g_value_set_schar(value , data->priv->type);
	#endif
		break;
	case PROP_DESCRIPTION:
		g_value_set_string(value , data->priv->description);
		break;
	case PROP_SERVICE:
		g_value_set_string(value , data->priv->service);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_error_message_class_init (MktErrorMessageClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//MktModelClass* parent_class = MKT_MODEL_CLASS (klass);
	g_type_class_add_private (klass, sizeof (MktErrorMessagePrivate));
	object_class->finalize = mkt_error_message_finalize;
	object_class->set_property = mkt_error_message_set_property;
	object_class->get_property = mkt_error_message_get_property;

	g_object_class_override_property(object_class,PROP_NUMBER,"error-number");
	g_object_class_override_property(object_class,PROP_PENDING,"error-pending");
	g_object_class_override_property(object_class,PROP_CHANGED,"error-changed");
	g_object_class_override_property(object_class,PROP_TYPE,"error-type");
	g_object_class_override_property(object_class,PROP_DESCRIPTION,"error-description");
	g_object_class_override_property(object_class,PROP_SERVICE,"error-service");

}

