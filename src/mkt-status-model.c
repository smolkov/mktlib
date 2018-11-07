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

#include "mkt-status-model.h"
#include "market-time.h"



enum
{
	PROP_0,
	PROP_STATUS_SIGNIFICATION,
	PROP_STATUS_DESCRIPTION,
	PROP_STATUS_ACTIVATED,
	PROP_STATUS_CHANGED

};


struct _MktStatusModelPrivate
{
	gchar      *signification;
	gchar      *description;
	gboolean    activated;
	gdouble     changed;
};


#define MKT_STATUS_MODEL_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), MKT_TYPE_STATUS_MODEL, MktStatusModelPrivate))



static const gchar*
mkt_status_object_signification                ( MktStatus *status )
{
	g_return_val_if_fail(status != NULL ,NULL);
	g_return_val_if_fail(MKT_IS_STATUS_MODEL(status) , NULL );
	return MKT_STATUS_MODEL(status)->priv->signification;
}

static const gchar*
mkt_status_object_description                ( MktStatus *status )
{
	g_return_val_if_fail(status != NULL ,NULL);
	g_return_val_if_fail(MKT_IS_STATUS_MODEL(status) , NULL );
	return MKT_STATUS_MODEL(status)->priv->description;
}

static gboolean
mkt_status_object_activated                ( MktStatus *status )
{
	g_return_val_if_fail(status != NULL ,FALSE);
	g_return_val_if_fail(MKT_IS_STATUS_MODEL(status) , FALSE );
	return MKT_STATUS_MODEL(status)->priv->activated;
}

static void
mkt_status_model_init_interface ( MktStatusInterface *iface )
{
	iface->status_signification          = mkt_status_object_signification;
	iface->status_description            = mkt_status_object_description;
	iface->is_active                     = mkt_status_object_activated;

}



G_DEFINE_TYPE_WITH_CODE (MktStatusModel, mkt_status_model, MKT_TYPE_MODEL,
		                                G_IMPLEMENT_INTERFACE (MKT_TYPE_STATUS,
		                                		mkt_status_model_init_interface)
		                                		MKT_CREATE_MODEL_FULL(mkt_connection_get_default(TRUE)))


static void
mkt_status_model_init (MktStatusModel *mkt_status_model)
{
	MktStatusModelPrivate *priv            = MKT_STATUS_MODEL_PRIVATE(mkt_status_model);

	mkt_status_model->priv                 = priv;
	mkt_status_model->priv->activated      = FALSE;
	mkt_status_model->priv->signification  = g_strdup("P");
	mkt_status_model->priv->description    = g_strdup("Pause");
	/* TODO: Add initialization code here */
}

static void
mkt_status_model_finalize (GObject *model)
{
	MktStatusModel *status = MKT_STATUS_MODEL(model);
	if(status->priv->signification)        g_free(status->priv->signification);
	if(status->priv->description)          g_free(status->priv->description);
	G_OBJECT_CLASS (mkt_status_model_parent_class)->finalize (model);
}




static void
mkt_status_model_set_property (GObject *model, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_STATUS_MODEL (model));
	MktStatusModel *status = MKT_STATUS_MODEL(model);
	switch (prop_id)
	{
	case PROP_STATUS_SIGNIFICATION:
		if(status->priv->signification) g_free(status->priv->signification);
		status->priv->signification   = g_value_dup_string(value);
		break;
	case PROP_STATUS_DESCRIPTION:
		if(status->priv->description)g_free(status->priv->description);
		status->priv->description = g_value_dup_string(value);
		break;
	case PROP_STATUS_ACTIVATED:
		status->priv->activated = g_value_get_boolean(value);
		break;
	case PROP_STATUS_CHANGED:
		status->priv->changed = g_value_get_double(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (model, prop_id, pspec);
		break;
	}
}

static void
mkt_status_model_get_property (GObject *model, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_STATUS_MODEL (model));
	MktStatusModel *stream = MKT_STATUS_MODEL(model);
	switch (prop_id)
	{

	case PROP_STATUS_SIGNIFICATION:
		g_value_set_string(value,stream->priv->signification);
		break;
	case PROP_STATUS_DESCRIPTION:
		g_value_set_string(value,stream->priv->description);
		break;
	case PROP_STATUS_ACTIVATED:
		g_value_set_boolean(value,stream->priv->activated);
		break;
	case PROP_STATUS_CHANGED:
		g_value_set_double(value,stream->priv->changed);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (model, prop_id, pspec);
		break;
	}
}

static void
mkt_status_model_class_init (MktStatusModelClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//MktModelClass* parent_class = MKT_MODEL_CLASS (klass);
	g_type_class_add_private (klass, sizeof (MktStatusModelPrivate));
	object_class->finalize     = mkt_status_model_finalize;
	object_class->set_property = mkt_status_model_set_property;
	object_class->get_property = mkt_status_model_get_property;
	g_object_class_override_property(object_class,PROP_STATUS_SIGNIFICATION,"status-signification");
	g_object_class_override_property(object_class,PROP_STATUS_DESCRIPTION,"status-description");
	g_object_class_override_property(object_class,PROP_STATUS_ACTIVATED,"status-active");
	g_object_class_override_property(object_class,PROP_STATUS_CHANGED,"status-changed");


}

