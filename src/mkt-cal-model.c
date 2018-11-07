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

#include "market-time.h"
#include "mkt-model.h"
#include "mkt-connection.h"
#include "mkt-value.h"
#include "mkt-cal-model.h"
#include "mkt-calibration-model.h"
#include "mkt-cal-point-model.h"


enum
{
	PROP_0,
	PROP_MKT_CAL_CREATED,
	PROP_MKT_CAL_CALIBRATION,
	PROP_MKT_CAL_POINT,
	PROP_MKT_CAL_PROCESS,
	PROP_MKT_CAL_KIND,
	PROP_MKT_CAL_NUMBER,
	PROP_MKT_CAL_SLOPE,
	PROP_MKT_CAL_INTERCEPT,
	PROP_MKT_CAL_AIRFLOW_IN,
	PROP_MKT_CAL_AIRFLOW_OUT,
	PROP_MKT_CAL_ACTIVATED
};



struct _MktCalModelPrivate
{
	guint64        calibration;
	guint64        point;
	guint64        process;
	gdouble        slope;
	guint          kind;
	guint          number;
	gdouble        created;
	gdouble        intercept;
	gdouble        airflow_in;
	gdouble        airflow_out;
	gboolean       activated;
	MktModel      *calibration_object;
	MktModel      *process_model;
	MktCalPoint   *curr_point;
	MktCalPoint   *point_solution;
	GSList        *points;
};


#define MKT_CAL_MODEL_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), MKT_TYPE_CAL_MODEL, MktCalModelPrivate))


static MktModel*
mkt_cal_model_calibration                                     ( MktCal *cal )
{
	g_return_val_if_fail(cal != NULL,NULL);
	g_return_val_if_fail(MKT_IS_CAL(cal),NULL);
	if(MKT_CAL_MODEL(cal)->priv->calibration_object)g_object_unref (MKT_CAL_MODEL(cal)->priv->calibration_object);
	MKT_CAL_MODEL(cal)->priv->calibration_object = mkt_model_select_one(MKT_TYPE_CALIBRATION_MODEL,"select * from $tablename where ref_id = %"G_GUINT64_FORMAT,MKT_CAL_MODEL(cal)->priv->calibration);
	return MKT_CAL_MODEL(cal)->priv->calibration_object;
}

static MktCalPoint*
mkt_cal_model_solution_point                                     ( MktCal *cal , guint point )
{
	g_return_val_if_fail(cal != NULL,NULL);
	g_return_val_if_fail(MKT_IS_CAL(cal),NULL);
	g_return_val_if_fail(point>0,NULL);
	if(MKT_CAL_MODEL(cal)->priv->curr_point)g_object_unref (MKT_CAL_MODEL(cal)->priv->curr_point);
	/*TEST:g_debug("SQL :select * from $tablename where cal_point_cal = %"G_GUINT64_FORMAT" ORDER BY cal_point_solution LIMIT 1 OFFSET %d",
			mkt_model_ref_id(MKT_IMODEL(cal)),(point-1));*/

	MKT_CAL_MODEL(cal)->priv->curr_point = MKT_CAL_POINT(mkt_model_select_one(MKT_TYPE_CAL_POINT_MODEL,"select * from $tablename where cal_point_cal = %"G_GUINT64_FORMAT" ORDER BY cal_point_solution LIMIT 1 OFFSET %d",
			mkt_model_ref_id(MKT_IMODEL(cal)),(point-1)));
	//TEST:g_debug("MKT_CAL_MODEL(cal)->priv->curr_point = %p",MKT_CAL_MODEL(cal)->priv->curr_point);
	return MKT_CAL_MODEL(cal)->priv->curr_point;
}


static gboolean
mkt_cal_model_next_point  (  MktCal *cal  )
{
	MktCalModel *cal_model = MKT_CAL_MODEL(cal);
	MktCalPoint *point = MKT_CAL_POINT(mkt_model_select_one(MKT_TYPE_CAL_POINT_MODEL,"select * from $tablename where ref_id = %"G_GUINT64_FORMAT,cal_model->priv->point));
	if(point == NULL)return FALSE ;
	MktCalPoint *next_point  = MKT_CAL_POINT(mkt_model_select_one(MKT_TYPE_CAL_POINT_MODEL,"select * from $tablename where cal_point_cal = %"G_GUINT64_FORMAT" and cal_point_solution > %f ORDER BY cal_point_solution ASC LIMIT 1",
			mkt_model_ref_id(MKT_IMODEL(cal)),
			mkt_cal_point_solution(point)));
	if(next_point)
	{
		g_object_set(cal,"cal-acative-point",mkt_model_ref_id(MKT_IMODEL(next_point)),NULL);
		return TRUE;
	}
	else
	{
		g_object_set(cal,"cal-acative-point",0,NULL);
		g_object_unref(point);
	}
	return FALSE;
}

static gboolean
mkt_cal_model_pre_point  (  MktCal *cal  )
{
	MktCalModel *cal_model = MKT_CAL_MODEL(cal);
	MktCalPoint *point = MKT_CAL_POINT(mkt_model_select_one(MKT_TYPE_CAL_POINT_MODEL,"select * from $tablename where ref_id = %"G_GUINT64_FORMAT,cal_model->priv->point));
	if(point == NULL)return FALSE;
	MktCalPoint *pre_point  = MKT_CAL_POINT(mkt_model_select_one(MKT_TYPE_CAL_POINT_MODEL,"select * from $tablename where cal_point_cal = %"G_GUINT64_FORMAT" and cal_point_solution < %f ORDER BY cal_point_solution ASC LIMIT 1",
			mkt_model_ref_id(MKT_IMODEL(cal)),
			mkt_cal_point_solution(point)));
	if(pre_point)
	{
		g_object_set(cal,"cal-acative-point",mkt_model_ref_id(MKT_IMODEL(pre_point)),NULL);
		return TRUE;
	}
	else
	{
		g_object_set(cal,"cal-acative-point",0,NULL);
		g_object_unref(point);
	}
	return FALSE;
}

static gboolean
mkt_cal_model_have_next_point  (  MktCal *cal  )
{
	MktCalModel *cal_model = MKT_CAL_MODEL(cal);
	if(cal_model->priv->curr_point)g_object_unref (cal_model->priv->curr_point);
	cal_model->priv->curr_point = MKT_CAL_POINT(mkt_model_select_one(MKT_TYPE_CAL_POINT_MODEL,"select * from $tablename where ref_id = %"G_GUINT64_FORMAT,cal_model->priv->point));
	g_return_val_if_fail(cal_model->priv->curr_point != NULL,FALSE);
	MktCalPoint *next_point  = MKT_CAL_POINT(mkt_model_select_one(MKT_TYPE_CAL_POINT_MODEL,"select * from $tablename where cal_point_cal = %"G_GUINT64_FORMAT" and cal_point_solution > %f ORDER BY cal_point_solution ASC LIMIT 1",
			mkt_model_ref_id(MKT_IMODEL(cal)),
			mkt_cal_point_solution(cal_model->priv->curr_point)));
	if(next_point)
	{
		g_object_unref(next_point);
		return TRUE;
	}
	return FALSE;
}

static GSList*
mkt_cal_model_points                                     ( MktCal *cal )
{
	g_return_val_if_fail(cal != NULL,NULL);
	g_return_val_if_fail(MKT_IS_CAL(cal),NULL);
	if(MKT_CAL_MODEL(cal)->priv->points)mkt_slist_free_full(MKT_CAL_MODEL(cal)->priv->points,g_object_unref);
	MKT_CAL_MODEL(cal)->priv->points = mkt_model_select(MKT_TYPE_CAL_POINT_MODEL,"select * from $tablename where cal_point_cal = %"G_GUINT64_FORMAT" ORDER BY cal_point_solution ASC",mkt_model_ref_id(MKT_IMODEL(cal)));
	return MKT_CAL_MODEL(cal)->priv->points;
}


static gdouble
mkt_cal_model_slope                          ( MktCal *ucal )
{
	g_return_val_if_fail(ucal != NULL,0.);
	g_return_val_if_fail(MKT_IS_CAL(ucal),0.);
	return MKT_CAL_MODEL(ucal)->priv->slope;
}
static gdouble
mkt_cal_model_intercept                          ( MktCal *ucal )
{
	g_return_val_if_fail(ucal != NULL,0.);
	g_return_val_if_fail(MKT_IS_CAL(ucal),0.);
	return MKT_CAL_MODEL(ucal)->priv->intercept;
}


static gboolean
mkt_cal_model_delete_points ( MktCal *ucal  )
{
	g_return_val_if_fail(ucal != NULL,FALSE);
	g_return_val_if_fail(MKT_IS_CAL(ucal),FALSE);
	MktCalModel *cal_model = MKT_CAL_MODEL(ucal);
	if(cal_model->priv->curr_point)g_object_unref(cal_model->priv->curr_point);
	cal_model->priv->curr_point = NULL;
	mkt_cal_model_points(MKT_CAL(cal_model));
	if(cal_model->priv->points)mkt_slist_free_full(cal_model->priv->points,mkt_model_unref_and_delete);
	cal_model->priv->points = NULL;
	return TRUE;
}


static void
mkt_cal_model_init_stream_interface ( MktCalInterface *iface )
{
	iface->cal_calibration=mkt_cal_model_calibration;
	iface->cal_slope=mkt_cal_model_slope;
	iface->cal_slope=mkt_cal_model_slope;
	iface->cal_intercept=mkt_cal_model_intercept;
	iface->solution_point=mkt_cal_model_solution_point;
	iface->next_point=mkt_cal_model_next_point;
	iface->pre_point=mkt_cal_model_pre_point;
	iface->points=mkt_cal_model_points;
	iface->delete_points=mkt_cal_model_delete_points;
	iface->have_next_point=mkt_cal_model_have_next_point;
}



G_DEFINE_TYPE_WITH_CODE (MktCalModel, mkt_cal_model, MKT_TYPE_MODEL,
		                                         G_IMPLEMENT_INTERFACE (MKT_TYPE_CAL,
		                                		 mkt_cal_model_init_stream_interface)
		                                		 MKT_CREATE_MODEL_FULL(mkt_connection_new_default_calibration(TRUE)))


static void
mkt_cal_model_init (MktCalModel *mkt_cal_model)
{
	MktCalModelPrivate *priv  = MKT_CAL_MODEL_PRIVATE(mkt_cal_model);
	priv->calibration         = 0;
	priv->process             = 0;
	priv->point               = 0;
	priv->slope               = 0.;
	priv->intercept           = 0.;
	priv->calibration_object  = NULL;
	priv->curr_point          = NULL;
	priv->points              = NULL;
	priv->point_solution      = NULL;
	priv->process_model       = NULL;

	mkt_cal_model->priv = priv;
	/* TODO: Add initialization code here */
}

static void
mkt_cal_model_finalize (GObject *object)
{
	MktCalModel *data = MKT_CAL_MODEL(object);
	if(data->priv->calibration_object)g_object_unref(data->priv->calibration_object);
	if(data->priv->curr_point)        g_object_unref(data->priv->curr_point);
	if(data->priv->point_solution)    g_object_unref(data->priv->point_solution);
	if(data->priv->process_model)     g_object_unref(data->priv->process_model);
	if(data->priv->points)            g_slist_free_full(data->priv->points,g_object_unref);
	G_OBJECT_CLASS (mkt_cal_model_parent_class)->finalize (object);
}

static void
mkt_cal_model_delete  (MktModel *model )
{
	MktCalModel *cal_model = MKT_CAL_MODEL(model);
	if(cal_model->priv->curr_point)g_object_unref(cal_model->priv->curr_point);
	cal_model->priv->curr_point = NULL;
	mkt_cal_model_points(MKT_CAL(cal_model));
	if(cal_model->priv->points)mkt_slist_free_full(cal_model->priv->points,mkt_model_unref_and_delete);
	cal_model->priv->points = NULL;
	MKT_MODEL_CLASS (mkt_cal_model_parent_class)->delete (model);
}


static void
mkt_cal_model_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_CAL_MODEL (object));
	MktCalModel *cal = MKT_CAL_MODEL(object);
	switch (prop_id)
	{
	case PROP_MKT_CAL_CREATED:
		cal->priv->created = g_value_get_double(value);
		break;
	case PROP_MKT_CAL_CALIBRATION:
		cal->priv->calibration = g_value_get_uint64(value);
		break;
	case PROP_MKT_CAL_PROCESS:
		cal->priv->process = g_value_get_uint64(value);
		break;
	case PROP_MKT_CAL_POINT:
		cal->priv->point = g_value_get_uint64(value);
		break;
	case PROP_MKT_CAL_KIND:
		cal->priv->kind = g_value_get_uint(value);
		break;
	case PROP_MKT_CAL_NUMBER:
		cal->priv->number = g_value_get_uint(value);
		break;
	case PROP_MKT_CAL_SLOPE:
		cal->priv->slope = g_value_get_double(value);
		break;
	case PROP_MKT_CAL_INTERCEPT:
		cal->priv->intercept = g_value_get_double(value);
		break;
	case PROP_MKT_CAL_AIRFLOW_IN:
		cal->priv->airflow_in = g_value_get_double(value);
		break;
	case PROP_MKT_CAL_AIRFLOW_OUT:
		cal->priv->airflow_out = g_value_get_double(value);
		break;
	case PROP_MKT_CAL_ACTIVATED:
		cal->priv->activated = g_value_get_boolean(value);
		break;
	default:

		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_cal_model_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_CAL_MODEL (object));
	MktCalModel *cal = MKT_CAL_MODEL(object);
	switch (prop_id)
	{
	case PROP_MKT_CAL_CREATED:
		g_value_set_double(value,cal->priv->created);
		break;
	case PROP_MKT_CAL_CALIBRATION:
		g_value_set_uint64(value,cal->priv->calibration);
		break;
	case PROP_MKT_CAL_PROCESS:
		g_value_set_uint64(value,cal->priv->process);
		break;
	case PROP_MKT_CAL_POINT:
		g_value_set_uint64(value,cal->priv->point);
		break;
	case PROP_MKT_CAL_KIND:
		g_value_set_uint(value,cal->priv->kind);
		break;
	case PROP_MKT_CAL_NUMBER:
		g_value_set_uint(value,cal->priv->number);
		break;
	case PROP_MKT_CAL_SLOPE:
		g_value_set_double(value,cal->priv->slope);
		break;
	case PROP_MKT_CAL_INTERCEPT:
		g_value_set_double(value,cal->priv->intercept);
		break;
	case PROP_MKT_CAL_AIRFLOW_IN:
		g_value_set_double(value,cal->priv->airflow_in);
		break;
	case PROP_MKT_CAL_AIRFLOW_OUT:
		g_value_set_double(value,cal->priv->airflow_out);
		break;
	case PROP_MKT_CAL_ACTIVATED:
		g_value_set_boolean(value,cal->priv->activated);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static gboolean
mkt_cal_model_reset   ( MktCalModel *cal )
{
	GSList *points = mkt_cal_points(MKT_CAL(cal));
	if( points == NULL )
	{
		mkt_cal_add_point(MKT_CAL(cal),500.0);
		return TRUE;
	}
	else
	{
		GSList *l = NULL;
		for(l=points;l!=NULL;l=l->next)
		{
			mkt_cal_point_reset(MKT_CAL_POINT(l->data));
			//mkt_cal_point_clean_data(MKT_CAL_POINT(l->data));
		}
		g_object_set(cal,"cal-acative-point",mkt_model_ref_id(MKT_IMODEL(points->data)),NULL);
	}
	return TRUE;
}

static gboolean
mkt_cal_model_add_value   ( MktCalModel *cal , gdouble value , guint sensor_trigger )
{
	/*MktCalPoint *point          = mkt_cal_active_point(MKT_CAL(cal));
	MktModel *calibration       = mkt_cal_calibration(MKT_CAL(cal));
	if(point == NULL) return FALSE;
	if(calibration == NULL )return FALSE;
	mkt_cal_point_add_cal_value(point,calibration,value,sensor_trigger);
	if(mkt_cal_point_done(point))
		mkt_cal_calculate(MKT_CAL(cal));*/
	return TRUE;
}

static gboolean
mkt_cal_model_calculate   ( MktCalModel *cal )
{
	//if(!mkt_cal_is_done(MKT_CAL(cal))) return FALSE;
	GSList *points = mkt_cal_points(MKT_CAL(cal));
	GSList *l = NULL;
	gdouble sum11 = 0.;
	gdouble sum12 = 0.;
	gdouble sum1  = 0.;
	gdouble sum21 = 0.;
	gdouble sum22 = 0.;
	gdouble sum2  = 0.;
	gdouble a = 0.,b = 0.;
	if(g_slist_length(points)>1)
	{
		for(l=points;l!=NULL;l=l->next)
		{
			sum11+=1.;
			sum12+=mkt_cal_point_solution(MKT_CAL_POINT(l->data));
			sum1+=mkt_cal_point_average(MKT_CAL_POINT(l->data));
			sum21+=mkt_cal_point_solution(MKT_CAL_POINT(l->data));
			sum22+=mkt_cal_point_solution(MKT_CAL_POINT(l->data))*mkt_cal_point_solution(MKT_CAL_POINT(l->data));
			sum2+=mkt_cal_point_solution(MKT_CAL_POINT(l->data))*mkt_cal_point_average(MKT_CAL_POINT(l->data));
			a =  (sum12*sum2 - sum1*sum22)/(sum12*sum21 - sum11*sum22);
			b = -(sum11*sum2 - sum1*sum21)/(sum12*sum21 - sum11*sum22);
		}
	}
	else
	{
		a = 0;
		b = mkt_cal_point_average(MKT_CAL_POINT(points->data))/mkt_cal_point_solution(MKT_CAL_POINT(points->data));
	}
	g_object_set(cal,"cal-slope",b,"cal-intercept",a,NULL);
	return TRUE;
}



static void
mkt_cal_model_class_init (MktCalModelClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//MktModelClass* parent_class = MKT_MODEL_CLASS (klass);
	MktModelClass *model_class = MKT_MODEL_CLASS(klass);
	g_type_class_add_private (klass, sizeof (MktCalModelPrivate));
	object_class->finalize     = mkt_cal_model_finalize;
	object_class->set_property = mkt_cal_model_set_property;
	object_class->get_property = mkt_cal_model_get_property;
	model_class->delete        = mkt_cal_model_delete;
	klass->reset               = mkt_cal_model_reset;
	klass->add_value           = mkt_cal_model_add_value;
	klass->calculate           = mkt_cal_model_calculate;

	g_object_class_override_property(object_class,PROP_MKT_CAL_CREATED,"cal-created");
	g_object_class_override_property(object_class,PROP_MKT_CAL_KIND,"cal-kind");
	g_object_class_override_property(object_class,PROP_MKT_CAL_NUMBER,"cal-number");
	g_object_class_override_property(object_class,PROP_MKT_CAL_CALIBRATION,"cal-calibration");
	g_object_class_override_property(object_class,PROP_MKT_CAL_PROCESS,"cal-process");
	g_object_class_override_property(object_class,PROP_MKT_CAL_POINT,"cal-acative-point");
	g_object_class_override_property(object_class,PROP_MKT_CAL_SLOPE,"cal-slope");
	g_object_class_override_property(object_class,PROP_MKT_CAL_INTERCEPT,"cal-intercept");
	g_object_class_override_property(object_class,PROP_MKT_CAL_AIRFLOW_IN,"cal-airflow-in");
	g_object_class_override_property(object_class,PROP_MKT_CAL_AIRFLOW_OUT,"cal-airflow-out");

	g_object_class_override_property(object_class,PROP_MKT_CAL_ACTIVATED,"cal-activated");




}

