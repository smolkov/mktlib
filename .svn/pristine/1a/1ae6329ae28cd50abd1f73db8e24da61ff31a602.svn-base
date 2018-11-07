/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktCalibrationModel
 * @{
 * @file  mkt-calibration-object.c	default calibration model object
 * @brief Vollständige Beschreibung .... <<
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */

#include "mkt-calibration-model.h"
#include "market-time.h"
#include "mkt-value.h"
#include "mkt-stream-model.h"
#include "mkt-channel-model.h"
#include "mkt-cal-point-model.h"
#include "mkt-cal-data-model.h"


enum
{
	PROP_0,
	PROP_MKT_CALIBRATION_IDENTIFICATION,
	PROP_MKT_CALIBRATION_LEVEL,
	PROP_MKT_CALIBRATION_CHANNEL,
	PROP_MKT_CALIBRATION_STREAM,
	PROP_MKT_CALIBRATION_PROCESS,
	PROP_MKT_CALIBRATION_STARTED,
	PROP_MKT_CALIBRATION_ACTIVATED,
	PROP_MKT_CALIBRATION_MAIN,
	PROP_MKT_CALIBRATION_DONE,
	PROP_MKT_CALIBRATION_SLOPE,
	PROP_MKT_CALIBRATION_INTERCEPT,
	PROP_MKT_CALIBRATION_DETERMINATION,
	PROP_MKT_CALIBRATION_CV,
	PROP_MKT_CALIBRATION_DEVIATION,
	PROP_MKT_CALIBRATION_TIME,
	PROP_MKT_CALIBRATION_DATE,

};



struct _MktCalibrationModelPrivate
{
	guint            identification;
	guint            level;

	guint64          channel;
	guint64          stream;
	guint64          process;
	gboolean         activated;
	gboolean         main;
	gboolean         done;
	gdouble          slope;
	gdouble          intercept;
	gdouble          determination;
	gdouble          cv;
	gdouble          deviation;
	gdouble          time;
	gchar           *date;
};


#define MKT_CALIBRATION_MODEL_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), MKT_TYPE_CALIBRATION_MODEL, MktCalibrationModelPrivate))


static guint
mkt_calibration_model_identification (MktCalibration *data)
{
	g_return_val_if_fail(data != NULL , 0);
	g_return_val_if_fail(MKT_IS_CALIBRATION_MODEL(data) , 0);
	return MKT_CALIBRATION_MODEL(data)->priv->identification;
}
static guint
mkt_calibration_model_level (MktCalibration *data)
{
	g_return_val_if_fail(data != NULL , 0);
	g_return_val_if_fail(MKT_IS_CALIBRATION_MODEL(data) , 0);
	return MKT_CALIBRATION_MODEL(data)->priv->level;
}
static guint64
mkt_calibration_model_channel (MktCalibration *data)
{
	g_return_val_if_fail(data != NULL , 0);
	g_return_val_if_fail(MKT_IS_CALIBRATION_MODEL(data) , 0);
	return MKT_CALIBRATION_MODEL(data)->priv->channel;
}

static guint64
mkt_calibration_model_stream (MktCalibration *data)
{
	g_return_val_if_fail(data != NULL , 0);
	g_return_val_if_fail(MKT_IS_CALIBRATION_MODEL(data) , 0);
	return MKT_CALIBRATION_MODEL(data)->priv->stream;
}
static guint64
mkt_calibration_model_process (MktCalibration *data)
{
	g_return_val_if_fail(data != NULL , 0);
	g_return_val_if_fail(MKT_IS_CALIBRATION_MODEL(data) , 0);
	return MKT_CALIBRATION_MODEL(data)->priv->process;
}


static gboolean
mkt_calibration_model_activated                               ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,FALSE);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),FALSE);
	return MKT_CALIBRATION_MODEL(calibration)->priv->activated;
}

static gboolean
mkt_calibration_model_main                               ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,FALSE);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),FALSE);
	return MKT_CALIBRATION_MODEL(calibration)->priv->main;
}

static gboolean
mkt_calibration_model_done                               ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,FALSE);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),FALSE);
	return MKT_CALIBRATION_MODEL(calibration)->priv->done;
}

static gdouble
mkt_calibration_model_slope                               ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,0.0);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),0.0);
	return MKT_CALIBRATION_MODEL(calibration)->priv->slope;
}
static gdouble
mkt_calibration_model_intercept                               ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,0.0);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),0.0);
	return MKT_CALIBRATION_MODEL(calibration)->priv->intercept;
}
static gdouble
mkt_calibration_model_determination                               ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,0.0);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),0.0);
	return MKT_CALIBRATION_MODEL(calibration)->priv->determination;
}
static gdouble
mkt_calibration_model_cv_parameter                               ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,0.0);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),0.0);
	return MKT_CALIBRATION_MODEL(calibration)->priv->cv;
}
static gdouble
mkt_calibration_model_deviation                               ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,0.0);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),0.0);
	return MKT_CALIBRATION_MODEL(calibration)->priv->deviation;
}
static gdouble
mkt_calibration_model_time (MktCalibration *data)
{
	g_return_val_if_fail(data != NULL , 0.);
	g_return_val_if_fail(MKT_IS_CALIBRATION_MODEL(data) , 0.);
	return MKT_CALIBRATION_MODEL(data)->priv->time;
}
static const gchar*
mkt_calibration_model_date (MktCalibration *data)
{
	g_return_val_if_fail(data != NULL , NULL);
	g_return_val_if_fail(MKT_IS_CALIBRATION_MODEL(data) , NULL);
	return MKT_CALIBRATION_MODEL(data)->priv->date;
}
static GSList*
mkt_calibration_model_points                               ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,FALSE);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),FALSE);
	GSList *ponts = mkt_model_select(MKT_TYPE_CAL_POINT_MODEL,"select * from $tablename where cal_point_calibration = %"G_GUINT64_FORMAT" ORDER BY cal_point_solution ASC",mkt_model_ref_id(MKT_IMODEL(calibration)));
	return ponts;
}


static void
mkt_calibration_model_init_stream_interface ( MktCalibrationInterface *iface )
{
	iface->calibration_identification=mkt_calibration_model_identification;
	iface->calibration_level=mkt_calibration_model_level;
	iface->calibration_channel=mkt_calibration_model_channel;
	iface->calibration_stream=mkt_calibration_model_stream;
	iface->calibration_process=mkt_calibration_model_process;

	iface->calibration_activated=mkt_calibration_model_activated;
	iface->calibration_main=mkt_calibration_model_main;
	iface->calibration_done=mkt_calibration_model_done;
	iface->calibration_slope=mkt_calibration_model_slope;
	iface->calibration_cv=mkt_calibration_model_cv_parameter;
	iface->calibration_deviation=mkt_calibration_model_deviation;
	iface->calibration_time=mkt_calibration_model_time;
	iface->calibration_date=mkt_calibration_model_date;

	iface->calibration_intercept=mkt_calibration_model_intercept;
	iface->calibration_determination=mkt_calibration_model_determination;

	iface->calibration_points=mkt_calibration_model_points;
}



G_DEFINE_TYPE_WITH_CODE (MktCalibrationModel, mkt_calibration_model, MKT_TYPE_MODEL,
		                                         G_IMPLEMENT_INTERFACE (MKT_TYPE_CALIBRATION,
		                                		 mkt_calibration_model_init_stream_interface)
		                                		 MKT_CREATE_MODEL_FULL(mkt_connection_new_default_calibration(TRUE)))



static void
mkt_calibration_model_init (MktCalibrationModel *mkt_calibration_model)
{
	MktCalibrationModelPrivate *priv      = MKT_CALIBRATION_MODEL_PRIVATE(mkt_calibration_model);
	priv->identification      = 0;
	priv->channel             = 0;
	priv->stream              = 0;
	priv->process             = 0;
	priv->activated           = FALSE;
	priv->main                = FALSE;
	priv->slope               = 0.;
	priv->intercept           = 0.;
	priv->cv                  = 0.;
	priv->deviation           = 0.;
	priv->time                = 0.;
	priv->date                = NULL;

	mkt_calibration_model->priv = priv;
	/* TODO: Add initialization code here */
}

static void
mkt_calibration_model_finalize (GObject *object)
{
	MktCalibrationModel *data = MKT_CALIBRATION_MODEL(object);
	if(data->priv->date)g_free(data->priv->date);
	G_OBJECT_CLASS (mkt_calibration_model_parent_class)->finalize (object);
}

static void
mkt_calibration_model_constructed (MktModel *model )
{
	if(MKT_MODEL_CLASS (mkt_calibration_model_parent_class)->initialized)
		MKT_MODEL_CLASS (mkt_calibration_model_parent_class)->initialized(model);

}

static void
mkt_calibration_model_delete  (MktModel *model )
{
	MktCalibrationModel *cal = MKT_CALIBRATION_MODEL(model);
	GSList *list = mkt_model_select(MKT_TYPE_CAL_POINT_MODEL,"select * from $tablename where cal_point_cal = %"G_GUINT64_FORMAT,mkt_model_ref_id(MKT_IMODEL(cal)));
	mkt_slist_free_full(list,mkt_model_unref_and_delete);
	MKT_MODEL_CLASS (mkt_calibration_model_parent_class)->delete (model);
}


static void
mkt_calibration_model_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_CALIBRATION_MODEL (object));
	MktCalibrationModel *cal = MKT_CALIBRATION_MODEL(object);
	switch (prop_id)
	{
	case PROP_MKT_CALIBRATION_IDENTIFICATION:
		cal->priv->identification = g_value_get_uint(value);
		break;
	case PROP_MKT_CALIBRATION_LEVEL:
		cal->priv->level = g_value_get_uint(value);
		break;
	case PROP_MKT_CALIBRATION_CHANNEL:
		cal->priv->channel = g_value_get_uint64(value);
		break;
	case PROP_MKT_CALIBRATION_STREAM:
		cal->priv->stream = g_value_get_uint64(value);
		break;
	case PROP_MKT_CALIBRATION_PROCESS:
		cal->priv->process = g_value_get_uint64(value);
		break;
	case PROP_MKT_CALIBRATION_ACTIVATED:
		cal->priv->activated = g_value_get_boolean(value);
		break;
	case PROP_MKT_CALIBRATION_MAIN:
		cal->priv->main = g_value_get_boolean(value);
		break;
	case PROP_MKT_CALIBRATION_DONE:
		cal->priv->done = g_value_get_boolean(value);
		break;
	case PROP_MKT_CALIBRATION_SLOPE:
		cal->priv->slope = g_value_get_double(value);
		break;
	case PROP_MKT_CALIBRATION_INTERCEPT:
		cal->priv->intercept = g_value_get_double(value);
		break;
	case PROP_MKT_CALIBRATION_DETERMINATION:
		cal->priv->determination = g_value_get_double(value);
		break;
	case PROP_MKT_CALIBRATION_CV:
		cal->priv->cv = g_value_get_double(value);
		break;
	case PROP_MKT_CALIBRATION_DEVIATION:
		cal->priv->deviation = g_value_get_double(value);
		break;
	case PROP_MKT_CALIBRATION_TIME:
		cal->priv->time = g_value_get_double(value);
		break;
	case PROP_MKT_CALIBRATION_DATE:
		if(cal->priv->date)g_free(cal->priv->date);
		cal->priv->date = g_value_dup_string(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_calibration_model_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_CALIBRATION_MODEL (object));
	MktCalibrationModel *cal = MKT_CALIBRATION_MODEL(object);
	switch (prop_id)
	{
	case PROP_MKT_CALIBRATION_IDENTIFICATION:
		g_value_set_uint(value , cal->priv->identification);
		break;
	case PROP_MKT_CALIBRATION_LEVEL:
		g_value_set_uint(value , cal->priv->level);
		break;
	case PROP_MKT_CALIBRATION_CHANNEL:
		g_value_set_uint64(value , cal->priv->channel);
		break;
	case PROP_MKT_CALIBRATION_STREAM:
		g_value_set_uint64(value , cal->priv->stream);
		break;
	case PROP_MKT_CALIBRATION_PROCESS:
		g_value_set_uint64(value , cal->priv->process);
		break;
	case PROP_MKT_CALIBRATION_ACTIVATED:
		g_value_set_boolean(value,cal->priv->activated);
		break;
	case PROP_MKT_CALIBRATION_MAIN:
		g_value_set_boolean(value,cal->priv->main);
		break;
	case PROP_MKT_CALIBRATION_DONE:
		g_value_set_boolean(value,cal->priv->done);
		break;
	case PROP_MKT_CALIBRATION_SLOPE:
		g_value_set_double(value,cal->priv->slope);
		break;
	case PROP_MKT_CALIBRATION_INTERCEPT:
		g_value_set_double(value,cal->priv->intercept);
		break;
	case PROP_MKT_CALIBRATION_DETERMINATION:
		g_value_set_double(value,cal->priv->determination);
		break;
	case PROP_MKT_CALIBRATION_CV:
		g_value_set_double(value,cal->priv->cv);
		break;
	case PROP_MKT_CALIBRATION_DEVIATION:
		g_value_set_double(value,cal->priv->deviation);
		break;
	case PROP_MKT_CALIBRATION_TIME:
		g_value_set_double(value , cal->priv->time);
		break;
	case PROP_MKT_CALIBRATION_DATE:
		g_value_set_string(value , cal->priv->date);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_calibration_model_class_init (MktCalibrationModelClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//MktModelClass* parent_class = MKT_MODEL_CLASS (klass);
	MktModelClass *model_class = MKT_MODEL_CLASS(klass);
	g_type_class_add_private (klass, sizeof (MktCalibrationModelPrivate));
	object_class->finalize     = mkt_calibration_model_finalize;
	object_class->set_property = mkt_calibration_model_set_property;
	object_class->get_property = mkt_calibration_model_get_property;
	model_class->delete        = mkt_calibration_model_delete;
	model_class->initialized   = mkt_calibration_model_constructed;
	g_object_class_override_property(object_class,PROP_MKT_CALIBRATION_IDENTIFICATION,"calibration-identification");
	g_object_class_override_property(object_class,PROP_MKT_CALIBRATION_LEVEL,"calibration-level");

	g_object_class_override_property(object_class,PROP_MKT_CALIBRATION_CHANNEL,"calibration-channel");
	g_object_class_override_property(object_class,PROP_MKT_CALIBRATION_STREAM,"calibration-stream");
	g_object_class_override_property(object_class,PROP_MKT_CALIBRATION_PROCESS,"calibration-process");
	g_object_class_override_property(object_class,PROP_MKT_CALIBRATION_ACTIVATED,"calibration-activated");
	g_object_class_override_property(object_class,PROP_MKT_CALIBRATION_MAIN,"calibration-main");
	g_object_class_override_property(object_class,PROP_MKT_CALIBRATION_DONE,"calibration-done");
	g_object_class_override_property(object_class,PROP_MKT_CALIBRATION_SLOPE,"calibration-slope");
	g_object_class_override_property(object_class,PROP_MKT_CALIBRATION_INTERCEPT,"calibration-intercept");
	g_object_class_override_property(object_class,PROP_MKT_CALIBRATION_DETERMINATION,"calibration-determination");
	g_object_class_override_property(object_class,PROP_MKT_CALIBRATION_CV,"calibration-cv");
	g_object_class_override_property(object_class,PROP_MKT_CALIBRATION_DEVIATION,"calibration-deviation");
	g_object_class_override_property(object_class,PROP_MKT_CALIBRATION_TIME,"calibration-changed");
	g_object_class_override_property(object_class,PROP_MKT_CALIBRATION_DATE,"calibration-date");


}

/** @} */
