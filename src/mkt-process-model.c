/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktProcessModel
 * @{
 * @file  mkt-process.c	Process model model interface
 * @brief This is Process model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */



#include "mkt-process-model.h"

#include <math.h>
#include <glib/gprintf.h>
#include "market-time.h"
#include "mkt-model.h"
#include "mkt-measurement-data.h"
#include "mkt-error.h"
#include "mkt-limit.h"
#include "market-time.h"
#include "mkt-analog-model.h"
#include "mkt-stream.h"
#include "mkt-param.h"
#include "mkt-param-object.h"
#include "mkt-value.h"



enum
{
	PROP_0,
	PROP_PROCESS_STREAM,
	PROP_PROCESS_INDENTIFICATION,
	PROP_PROCESS_PATH,
	PROP_PROCESS_NAME,
	PROP_PROCESS_DESCRIPTION,
	PROP_PROCESS_KIND,
	PROP_PROCESS_DATE,
	PROP_PROCESS_START,
	PROP_PROCESS_STOP,
	PROP_PROCESS_RUN,
	PROP_PROCESS_WAIT_ACTION,
};


struct _MktProcessModelPrivate
{
	guint64                stream;
	gchar*                 path;
	gchar*                 name;
	gchar*                 description;
	gchar*                 kind;
	gchar*                 date;
	gdouble                stop;
	gdouble                start;
	guint                  run;
	guint                  wait_action;
	guint                  indentification;
};


#define MKT_PROCESS_MODEL_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), MKT_TYPE_PROCESS_MODEL, MktProcessModelPrivate))

static guint64
mkt_process_model_stream                       ( MktProcess *process )
{
	g_return_val_if_fail(process != NULL , 0);
	g_return_val_if_fail(MKT_IS_PROCESS(process) , 0);
	return MKT_PROCESS_MODEL(process)->priv->stream;
}

static const gchar*
mkt_process_model_name                     (  MktProcess *process)
{
	g_return_val_if_fail(process != NULL , NULL);
	g_return_val_if_fail(MKT_IS_PROCESS(process) , NULL);
	return MKT_PROCESS_MODEL(process)->priv->name;
}

static const gchar*
mkt_process_model_kind                         ( MktProcess *process )
{
	g_return_val_if_fail(process != NULL , NULL);
	g_return_val_if_fail(MKT_IS_PROCESS(process) , NULL);
	return MKT_PROCESS_MODEL(process)->priv->kind;

}

static const gchar*
mkt_process_model_description                       ( MktProcess *process )
{
	g_return_val_if_fail(process != NULL , NULL);
	g_return_val_if_fail(MKT_IS_PROCESS(process) , NULL);
	return MKT_PROCESS_MODEL(process)->priv->description;
}

static gdouble
mkt_process_model_start                    ( MktProcess *process )
{
	g_return_val_if_fail(process != NULL , 0);
	g_return_val_if_fail(MKT_IS_PROCESS(process) , 0);
	return MKT_PROCESS_MODEL(process)->priv->start;
}
static gdouble
mkt_process_model_stop                     ( MktProcess *process )
{
	g_return_val_if_fail(process != NULL , 0);
	g_return_val_if_fail(MKT_IS_PROCESS(process) , 0);
	return MKT_PROCESS_MODEL(process)->priv->stop;
}
static guint
mkt_process_model_run                       ( MktProcess *process )
{
	g_return_val_if_fail(process != NULL , 0);
	g_return_val_if_fail(MKT_IS_PROCESS(process) , 0);
	return MKT_PROCESS_MODEL(process)->priv->run;
}
static guint
mkt_process_model_identification                       ( MktProcess *process )
{
	g_return_val_if_fail(process != NULL , 0);
	g_return_val_if_fail(MKT_IS_PROCESS(process) , 0);
	return MKT_PROCESS_MODEL(process)->priv->indentification;
}
static guint
mkt_process_model_wait_action                       ( MktProcess *process )
{
	g_return_val_if_fail(process != NULL , 0);
	g_return_val_if_fail(MKT_IS_PROCESS(process) , 0);
	return MKT_PROCESS_MODEL(process)->priv->wait_action;
}
static void
mkt_process_model_init_interface ( MktProcessInterface *iface )
{
	iface->process_name=mkt_process_model_name;
	iface->process_stream=mkt_process_model_stream;
	iface->process_description=mkt_process_model_description;
	iface->process_kind=mkt_process_model_kind;
	iface->process_start=mkt_process_model_start;
	iface->process_stop=mkt_process_model_stop;
	iface->process_run=mkt_process_model_run;
	iface->process_identification=mkt_process_model_identification;
	iface->process_wait_action=mkt_process_model_wait_action;

}


G_DEFINE_TYPE_WITH_CODE (MktProcessModel, mkt_process_model, MKT_TYPE_MODEL,
		                                  G_IMPLEMENT_INTERFACE (MKT_TYPE_PROCESS,
				                          mkt_process_model_init_interface)
				                          MKT_CREATE_MODEL_FULL(mkt_connection_new_default_process(TRUE)))


static void
mkt_process_model_init (MktProcessModel *mkt_process_model)
{
	MktProcessModelPrivate *priv      = MKT_PROCESS_MODEL_PRIVATE(mkt_process_model);
	priv->path                = NULL;
	priv->name                = g_strdup("Measurement");
	priv->description         = g_strdup("Measurement");
	priv->kind                = g_strdup("M");
	priv->date                = NULL;
	priv->indentification     = 0;
	priv->run                 = 0;
	priv->wait_action         = 0;

	mkt_process_model->priv   = priv;
	/* TODO: Add initialization code here */
}

static void
mkt_process_model_finalize (GObject *object)
{

	MktProcessModel *mprocess = MKT_PROCESS_MODEL(object);
	if(mprocess->priv->path)               g_free(mprocess->priv->path);
	if(mprocess->priv->name)               g_free(mprocess->priv->name);
	if(mprocess->priv->description)        g_free(mprocess->priv->description);
	if(mprocess->priv->kind)               g_free(mprocess->priv->kind);
	if(mprocess->priv->date)               g_free(mprocess->priv->date);
	G_OBJECT_CLASS (mkt_process_model_parent_class)->finalize (object);
}


static void
mkt_process_model_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_PROCESS_MODEL (object));
	MktProcessModel *process = MKT_PROCESS_MODEL(object);
	switch (prop_id)
	{
	case PROP_PROCESS_STREAM:
		process->priv->stream = g_value_get_uint64(value);
		break;
	case PROP_PROCESS_PATH:
		if(process->priv->path)g_free(process->priv->path);
		process->priv->path = g_value_dup_string(value);
		break;
	case PROP_PROCESS_NAME:
		if(process->priv->name)g_free(process->priv->name);
		process->priv->name = g_value_dup_string(value);
		break;
	case PROP_PROCESS_DESCRIPTION:
		if(process->priv->description!= NULL)g_free(process->priv->description);
		process->priv->description = g_value_dup_string(value);
		break;
	case PROP_PROCESS_KIND:
		if(process->priv->kind!= NULL)g_free(process->priv->kind);
		process->priv->kind = g_value_dup_string(value);
		break;
	case PROP_PROCESS_START:
		process->priv->start = g_value_get_double(value);
		break;
	case PROP_PROCESS_STOP:
		process->priv->stop = g_value_get_double(value);
		break;
	case PROP_PROCESS_RUN:
		process->priv->run = g_value_get_uint(value);
		break;
	case PROP_PROCESS_INDENTIFICATION:
		process->priv->indentification = g_value_get_uint(value);
		break;
	case PROP_PROCESS_WAIT_ACTION:
		process->priv->wait_action = g_value_get_uint(value);
		break;
	case PROP_PROCESS_DATE:
		if(process->priv->date!= NULL)g_free(process->priv->date);
		process->priv->date = g_value_dup_string(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_process_model_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_PROCESS_MODEL (object));
	MktProcessModel *process = MKT_PROCESS_MODEL(object);
	switch (prop_id)
	{
	case PROP_PROCESS_STREAM:
		g_value_set_uint64(value , process->priv->stream);
		break;
	case PROP_PROCESS_PATH:
		g_value_set_string(value , process->priv->path);
		break;
	case PROP_PROCESS_NAME:
		g_value_set_string(value , process->priv->name);
		break;
	case PROP_PROCESS_DESCRIPTION:
		g_value_set_string(value , process->priv->description);
		break;
	case PROP_PROCESS_KIND:
		g_value_set_string(value , process->priv->kind);
		break;
	case PROP_PROCESS_START:
		g_value_set_double(value , process->priv->start);
		break;
	case PROP_PROCESS_STOP:
		g_value_set_double(value , process->priv->stop);
		break;
	case PROP_PROCESS_RUN:
		g_value_set_uint(value ,  process->priv->run);
		break;
	case PROP_PROCESS_INDENTIFICATION:
		g_value_set_uint(value ,  process->priv->indentification);
		break;
	case PROP_PROCESS_WAIT_ACTION:
		g_value_set_uint(value ,  process->priv->wait_action);
		break;
	case PROP_PROCESS_DATE:
		g_value_set_string(value , process->priv->date);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_process_model_class_init (MktProcessModelClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//MktModelClass* parent_class = MKT_MODEL_CLASS (klass);

	g_type_class_add_private (klass, sizeof (MktProcessModelPrivate));
	object_class->finalize     = mkt_process_model_finalize;
	object_class->set_property = mkt_process_model_set_property;
	object_class->get_property = mkt_process_model_get_property;

	g_object_class_override_property(object_class,PROP_PROCESS_PATH,"process-path");
	g_object_class_override_property(object_class,PROP_PROCESS_STREAM,"process-stream");
	g_object_class_override_property(object_class,PROP_PROCESS_INDENTIFICATION,"process-identification");
	g_object_class_override_property(object_class,PROP_PROCESS_NAME,"process-name");
	g_object_class_override_property(object_class,PROP_PROCESS_DESCRIPTION,"process-description");
	g_object_class_override_property(object_class,PROP_PROCESS_KIND,"process-kind");
	g_object_class_override_property(object_class,PROP_PROCESS_START,"process-start");
	g_object_class_override_property(object_class,PROP_PROCESS_STOP,"process-stop");
	g_object_class_override_property(object_class,PROP_PROCESS_RUN,"process-run");
	g_object_class_override_property(object_class,PROP_PROCESS_WAIT_ACTION,"process-wait-action");
	g_object_class_override_property(object_class,PROP_PROCESS_DATE,"process-date");



}

/** @} */
