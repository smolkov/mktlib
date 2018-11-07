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

#include "mkt-stream-model.h"

#include "mkt-stream.h"
#include "market-time.h"
#include "mkt-channel.h"
#include "mkt-calibration.h"
#include "mkt-channel-model.h"




enum
{
	PROP_0,
	PROP_STREAM_NAME
};


struct _MktStreamModelPrivate
{
	gchar         *name;
	GSList        *channels;
};

#define MKT_STREAM_MODEL_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), MKT_TYPE_STREAM_MODEL, MktStreamModelPrivate))

static GSList*
mkt_stream_model_channels ( MktStream * stream )
{
	if(MKT_STREAM_MODEL(stream)->priv->channels)g_slist_free_full(MKT_STREAM_MODEL(stream)->priv->channels,g_object_unref);
	MKT_STREAM_MODEL(stream)->priv->channels =  mkt_model_select(MKT_TYPE_CHANNEL_MODEL,"select * from $tablename where stream_stream = %"G_GUINT64_FORMAT,
			mkt_model_ref_id(MKT_IMODEL(stream)));
	return MKT_STREAM_MODEL(stream)->priv->channels;
}

static void
mkt_stream_model_init_stream_interface ( MktStreamInterface *iface )
{
	iface->channels                 = mkt_stream_model_channels;
}


G_DEFINE_TYPE_WITH_CODE (MktStreamModel, mkt_stream_model, MKT_TYPE_PARAM_OBJECT,
		                                G_IMPLEMENT_INTERFACE (MKT_TYPE_STREAM,
		                                		mkt_stream_model_init_stream_interface)
		                                		MKT_CREATE_MODEL_FULL(mkt_connection_new_default_parameter(TRUE)))


static void
mkt_stream_model_init (MktStreamModel *mkt_stream_model)
{
	MktStreamModelPrivate *priv      = MKT_STREAM_MODEL_PRIVATE(mkt_stream_model);
	priv->channels                   = NULL;
	priv->name                       = NULL;
	mkt_stream_model->priv           = priv;
	/* TODO: Add initialization code here */
}


static void
mkt_stream_model_finalize (GObject *object)
{
	//g_debug("mkt_stream_model_finalize");
	MktStreamModel *stream = MKT_STREAM_MODEL(object);
	if(stream->priv->channels)       g_slist_free_full(MKT_STREAM_MODEL(stream)->priv->channels,g_object_unref);
	if(stream->priv->name)  g_free(stream->priv->name);
	//g_debug("mkt_stream_model_finalize end");
	G_OBJECT_CLASS (mkt_stream_model_parent_class)->finalize (object);
}


static void
mkt_stream_model_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_STREAM_MODEL (object));
	MktStreamModel *stream = MKT_STREAM_MODEL(object);
	//g_debug("Set stream property %s",pspec->name);
	switch (prop_id)
	{
	case PROP_STREAM_NAME:
		if(stream->priv->name)         g_free(stream->priv->name);
		stream->priv->name = g_value_dup_string(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_stream_model_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_STREAM_MODEL (object));
	MktStreamModel *stream = MKT_STREAM_MODEL(object);
	switch (prop_id)
	{
	case PROP_STREAM_NAME:
		g_value_set_string(value,stream->priv->name);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_stream_model_class_init (MktStreamModelClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//MktModelClass* parent_class = MKT_MODEL_CLASS (klass);
	g_type_class_add_private (klass, sizeof (MktStreamModelPrivate));
	object_class->finalize     = mkt_stream_model_finalize;
	object_class->set_property = mkt_stream_model_set_property;
	object_class->get_property = mkt_stream_model_get_property;

	g_object_class_override_property(object_class,PROP_STREAM_NAME,"stream-name");

}

const gchar *mkt_stream_model_get_name(MktStreamModel *stream){
	g_return_val_if_fail(stream!= NULL,NULL);
	g_return_val_if_fail(MKT_IS_STREAM_MODEL(stream),NULL);
	return stream->priv->name;

}
