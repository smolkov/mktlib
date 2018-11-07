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

#include "ultimate-posparm-object.h"
#include "market-time.h"



enum
{
	PROP_0,
	PROP_ULTIMATE_POSPARM_ONLINE,
	PROP_ULTIMATE_POSPARM_DRAIN,
	PROP_ULTIMATE_POSPARM_CALIBRATION,
	PROP_ULTIMATE_POSPARM_DILUTION,
	PROP_ULTIMATE_POSPARM_SINGLE,
	PROP_ULTIMATE_POSPARM_CHECK,
};




struct _UltimatePosparmObjectPrivate
{
	gchar          *online;
	gchar          *drain;
	gchar          *calibration;
	gchar          *dilution;
	gchar          *single;
	gchar          *check;
};



#define ULTIMATE_POSPARM_OBJECT_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), ULTIMATE_TYPE_POSPARM_OBJECT, UltimatePosparmObjectPrivate))



static void
ultimate_posparm_object_init_posparm_interface ( UltimatePosparmInterface *iface )
{

}



G_DEFINE_TYPE_WITH_CODE (UltimatePosparmObject, ultimate_posparm_object, MKT_TYPE_PARAM_OBJECT,
		                                         G_IMPLEMENT_INTERFACE  (ULTIMATE_TYPE_POSPARM,
		                                		 ultimate_posparm_object_init_posparm_interface)
		                                		 MKT_CREATE_MODEL_FULL(mkt_connection_new_default_parameter(TRUE)))


static void
ultimate_posparm_object_init (UltimatePosparmObject *ultimate_posparm_object)
{
	UltimatePosparmObjectPrivate *priv      = ULTIMATE_POSPARM_OBJECT_PRIVATE(ultimate_posparm_object);

	priv->online              = NULL;
	priv->single              = NULL;
	priv->drain               = NULL;
	priv->calibration         = NULL;
	priv->dilution            = NULL;
	priv->single              = NULL;
	priv->check               = NULL;
	ultimate_posparm_object->priv = priv;
	/* TODO: Add initialization code here */
}

static void
ultimate_posparm_object_finalize (GObject *object)
{
	UltimatePosparmObject *posparm = ULTIMATE_POSPARM_OBJECT(object);
	if(posparm->priv->online)      g_free(posparm->priv->online);
	if(posparm->priv->drain)       g_free(posparm->priv->drain);
	if(posparm->priv->calibration) g_free(posparm->priv->calibration);
	if(posparm->priv->dilution)    g_free(posparm->priv->dilution);
	if(posparm->priv->single)      g_free(posparm->priv->single);
	if(posparm->priv->check)       g_free(posparm->priv->check);
	G_OBJECT_CLASS (ultimate_posparm_object_parent_class)->finalize (object);
}


static void
ultimate_posparm_object_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (ULTIMATE_IS_POSPARM_OBJECT (object));
	UltimatePosparmObject *posparm = ULTIMATE_POSPARM_OBJECT(object);
	switch (prop_id)
	{
	case PROP_ULTIMATE_POSPARM_ONLINE:
		if(posparm->priv->online)      g_free(posparm->priv->online);
		posparm->priv->online      =   g_value_dup_string(value);
		break;
	case PROP_ULTIMATE_POSPARM_DRAIN:
		if(posparm->priv->drain)       g_free(posparm->priv->drain);
		posparm->priv->drain       =   g_value_dup_string(value);
		break;
	case PROP_ULTIMATE_POSPARM_CALIBRATION:
		if(posparm->priv->calibration) g_free(posparm->priv->calibration);
		posparm->priv->calibration =   g_value_dup_string(value);
		break;
	case PROP_ULTIMATE_POSPARM_DILUTION:
		if(posparm->priv->dilution)    g_free(posparm->priv->dilution);
		posparm->priv->dilution    =   g_value_dup_string(value);
		break;
	case PROP_ULTIMATE_POSPARM_SINGLE:
		if(posparm->priv->single)    g_free(posparm->priv->single);
		posparm->priv->single    =   g_value_dup_string(value);
		break;
	case PROP_ULTIMATE_POSPARM_CHECK:
		if(posparm->priv->check)    g_free(posparm->priv->check);
		posparm->priv->check    =   g_value_dup_string(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
ultimate_posparm_object_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (ULTIMATE_IS_POSPARM_OBJECT (object));
	UltimatePosparmObject *posparm = ULTIMATE_POSPARM_OBJECT(object);
	switch (prop_id)
	{

	case PROP_ULTIMATE_POSPARM_ONLINE:
		g_value_set_string(value,posparm->priv->online);
		break;
	case PROP_ULTIMATE_POSPARM_DRAIN:
		g_value_set_string(value,posparm->priv->drain);
		break;
	case PROP_ULTIMATE_POSPARM_CALIBRATION:
		g_value_set_string(value,posparm->priv->calibration);
		break;
	case PROP_ULTIMATE_POSPARM_DILUTION:
		g_value_set_string(value,posparm->priv->dilution);
		break;
	case PROP_ULTIMATE_POSPARM_SINGLE:
		g_value_set_string(value,posparm->priv->single);
		break;
	case PROP_ULTIMATE_POSPARM_CHECK:
		g_value_set_string(value,posparm->priv->check);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
ultimate_posparm_object_class_init (UltimatePosparmObjectClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//MktModelClass* parent_class = MKT_MODEL_CLASS (klass);
	g_type_class_add_private (klass, sizeof (UltimatePosparmObjectPrivate));
	object_class->finalize     = ultimate_posparm_object_finalize;
	object_class->set_property = ultimate_posparm_object_set_property;
	object_class->get_property = ultimate_posparm_object_get_property;

	g_object_class_override_property(object_class,PROP_ULTIMATE_POSPARM_ONLINE,"online-vessel");
	g_object_class_override_property(object_class,PROP_ULTIMATE_POSPARM_DRAIN,"drain-vessel");
	g_object_class_override_property(object_class,PROP_ULTIMATE_POSPARM_CALIBRATION,"calibration-vessel");
	g_object_class_override_property(object_class,PROP_ULTIMATE_POSPARM_DILUTION,"dilution-vessel");
	g_object_class_override_property(object_class,PROP_ULTIMATE_POSPARM_SINGLE,"single-vessel");
	g_object_class_override_property(object_class,PROP_ULTIMATE_POSPARM_CHECK,"check-vessel");
}

/** @} */
