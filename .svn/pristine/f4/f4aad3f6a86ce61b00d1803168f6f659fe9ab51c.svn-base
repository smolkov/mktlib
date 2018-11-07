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

#include "mkt-limit-message.h"
#include "market-time.h"
#include "mkt-connection.h"

enum
{
	PROP_0,
	PROP_ACTIVATED,
	PROP_NUMBER,
	PROP_PENDING,
	PROP_MIN,
	PROP_MAX,
	PROP_NAME,

};


struct _MktLimitMessagePrivate
{
	gboolean     activated;
	guint        number;
	gint         pending;
	gdouble      min;
	gdouble      max;
	gchar       *name;
};


#define MKT_LIMIT_MESSAGE_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), MKT_TYPE_LIMIT_MESSAGE, MktLimitMessagePrivate))

static gboolean
mkt_limit_message_activated (MktLimit *data)
{
	g_return_val_if_fail(data != NULL , FALSE);
	g_return_val_if_fail(MKT_IS_LIMIT_MESSAGE(data) , FALSE);
	return MKT_LIMIT_MESSAGE(data)->priv->activated;
}

static guint
mkt_limit_message_number (MktLimit *data)
{
	g_return_val_if_fail(data != NULL , FALSE);
	g_return_val_if_fail(MKT_IS_LIMIT_MESSAGE(data) , FALSE);
	return MKT_LIMIT_MESSAGE(data)->priv->number;
}


static gint
mkt_limit_message_pending (MktLimit *data)
{
	g_return_val_if_fail(data != NULL , FALSE);
	g_return_val_if_fail(MKT_IS_LIMIT_MESSAGE(data) , FALSE);
	return MKT_LIMIT_MESSAGE(data)->priv->pending;
}


static gdouble
mkt_limit_message_min (MktLimit *data)
{
	g_return_val_if_fail(data != NULL ,  0.0);
	g_return_val_if_fail(MKT_IS_LIMIT_MESSAGE(data) ,  0.0);
	return MKT_LIMIT_MESSAGE(data)->priv->min;
}

static gdouble
mkt_limit_message_max (MktLimit *data)
{
	g_return_val_if_fail(data != NULL ,  0.0);
	g_return_val_if_fail(MKT_IS_LIMIT_MESSAGE(data) ,  0.0);
	return MKT_LIMIT_MESSAGE(data)->priv->max;
}

static const gchar*
mkt_limit_message_name (MktLimit *data)
{
	g_return_val_if_fail(data != NULL ,  NULL);
	g_return_val_if_fail(MKT_IS_LIMIT_MESSAGE(data) ,  NULL);
	return MKT_LIMIT_MESSAGE(data)->priv->name;
}


static void
mkt_limit_message_init_limit_interface ( MktLimitInterface *iface )
{
	iface->get_activated=mkt_limit_message_activated;
	iface->get_number=mkt_limit_message_number;
	iface->get_pending=mkt_limit_message_pending;
	iface->get_min=mkt_limit_message_min;
	iface->get_max=mkt_limit_message_max;
	iface->get_name=mkt_limit_message_name;
}


G_DEFINE_TYPE_WITH_CODE (MktLimitMessage, mkt_limit_message, MKT_TYPE_PARAM_OBJECT,
									    G_IMPLEMENT_INTERFACE (MKT_TYPE_LIMIT,
				                               mkt_limit_message_init_limit_interface)
				                               MKT_CREATE_MODEL_FULL(mkt_connection_get_default(TRUE)))

static void
mkt_limit_message_init (MktLimitMessage *mkt_limit_message)
{
	MktLimitMessagePrivate *priv = MKT_LIMIT_MESSAGE_PRIVATE(mkt_limit_message);
	priv->number      = 0;
	priv->min         = 0.;
	priv->max         = 1.;
	priv->pending     = 0;
	priv->name        = g_strdup("L");
	mkt_limit_message->priv   = priv;

	/* TODO: Add initialization code here */
}

static void
mkt_limit_message_finalize (GObject *object)
{
	//MktLimitMessage *data = MKT_LIMIT_MESSAGE(object);

	G_OBJECT_CLASS (mkt_limit_message_parent_class)->finalize (object);
}


static void
mkt_limit_message_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_LIMIT_MESSAGE (object));
	MktLimitMessage *data = MKT_LIMIT_MESSAGE(object);
	switch (prop_id)
	{
	case PROP_ACTIVATED:
		data->priv->activated = g_value_get_boolean(value);
		break;
	case PROP_NUMBER:
		data->priv->number = g_value_get_uint(value);
		break;
	case PROP_PENDING:
		data->priv->pending = g_value_get_int(value);
		break;
	case PROP_MIN:
		data->priv->min = g_value_get_double(value);
		break;
	case PROP_MAX:
		data->priv->max = g_value_get_double(value);
		break;
	case PROP_NAME:
		if(data->priv->name ) g_free(data->priv->name);
		data->priv->name = g_value_dup_string(value);
		break;

	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_limit_message_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_LIMIT_MESSAGE (object));
	MktLimitMessage *data = MKT_LIMIT_MESSAGE(object);
	switch (prop_id)
	{
	case PROP_ACTIVATED:
		g_value_set_boolean(value , data->priv->activated);
		break;
	case PROP_PENDING:
		g_value_set_int(value , data->priv->pending);
		break;
	case PROP_NUMBER:
		g_value_set_uint(value , data->priv->number);
		break;
	case PROP_MIN:
		g_value_set_double(value , data->priv->min);
		break;
	case PROP_MAX:
		g_value_set_double(value , data->priv->max);
		break;
	case PROP_NAME:
		g_value_set_string(value , data->priv->name);
		break;

	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_limit_message_class_init (MktLimitMessageClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//MktModelClass* parent_class = MKT_MODEL_CLASS (klass);
	g_type_class_add_private (klass, sizeof (MktLimitMessagePrivate));
	object_class->finalize = mkt_limit_message_finalize;
	object_class->set_property = mkt_limit_message_set_property;
	object_class->get_property = mkt_limit_message_get_property;

	g_object_class_override_property(object_class,PROP_ACTIVATED,"limit-activated");
	g_object_class_override_property(object_class,PROP_PENDING,"limit-pending");
	g_object_class_override_property(object_class,PROP_NUMBER,"limit-number");
	g_object_class_override_property(object_class,PROP_MIN,"limit-min");
	g_object_class_override_property(object_class,PROP_MAX,"limit-max");
	g_object_class_override_property(object_class,PROP_NAME,"limit-name");



}

