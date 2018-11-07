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

#include "mkt-paramint32-model.h"
#include "market-time.h"
#include "../config.h"
#include <glib/gi18n-lib.h>

enum
{
	PROP_0,
	PROP_VALUE,
};


struct _MktParamint32ObjectPrivate
{
	gint       value;
};


#define MKT_PARAMINT32_MODEL_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), MKT_TYPE_PARAMINT32_MODEL, MktParamint32ObjectPrivate))


static gint
mkt_paramint32_model_value (MktParamint32 *param)
{
	g_return_val_if_fail(param != NULL ,  0);
	g_return_val_if_fail(MKT_IS_PARAMINT32_MODEL(param) ,  0);
	return MKT_PARAMINT32_MODEL(param)->priv->value;
}

static void
mkt_paramint32_model_init_param_interface ( MktParamint32Interface *iface )
{
	iface->value=mkt_paramint32_model_value;
}



G_DEFINE_TYPE_WITH_CODE (MktParamint32Object, mkt_paramint32_model, MKT_TYPE_PARAM_OBJECT,
										G_IMPLEMENT_INTERFACE (MKT_TYPE_PARAMINT32,
						                       mkt_paramint32_model_init_param_interface)
						                       MKT_CREATE_MODEL_FULL(mkt_connection_new_special_parameter(TRUE)))

static void
mkt_paramint32_model_init (MktParamint32Object *mkt_paramint32)
{
	MktParamint32ObjectPrivate *priv      = MKT_PARAMINT32_MODEL_PRIVATE(mkt_paramint32);
	priv->value            = 0;
	mkt_paramint32->priv   = priv;

	/* TODO: Add initialization code here */
}

static void
mkt_paramint32_model_finalize (GObject *object)
{
	//MktParamint32Object *data = MKT_PARAMINT32_MODEL(object);
	G_OBJECT_CLASS (mkt_paramint32_model_parent_class)->finalize (object);
}


static void
mkt_paramint32_model_delete   (MktModel *model)
{
	MKT_MODEL_CLASS (mkt_paramint32_model_parent_class)->delete (model);
}

static void
mkt_paramint32_model_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_PARAMINT32_MODEL (object));
	MktParamint32Object *data = MKT_PARAMINT32_MODEL(object);
	switch (prop_id)
	{
	case PROP_VALUE:
		data->priv->value = g_value_get_int(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_paramint32_model_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_PARAMINT32_MODEL (object));
	MktParamint32Object *data = MKT_PARAMINT32_MODEL(object);
	switch (prop_id)
	{
	case PROP_VALUE:
		g_value_set_int(value , data->priv->value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_paramint32_model_class_init (MktParamint32ObjectClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	MktModelClass* parent_class = MKT_MODEL_CLASS (klass);
	g_type_class_add_private (klass, sizeof (MktParamint32ObjectPrivate));
	object_class->finalize     = mkt_paramint32_model_finalize;
	object_class->set_property = mkt_paramint32_model_set_property;
	object_class->get_property = mkt_paramint32_model_get_property;
	parent_class->delete       = mkt_paramint32_model_delete;
	g_object_class_override_property(object_class,PROP_VALUE,"value");
}

