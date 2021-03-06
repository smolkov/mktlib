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

#include "mkt-cal-point-model.h"
#include "market-time.h"
#include "mkt-cal-data-model.h"
#include "mkt-value.h"



enum
{
	PROP_0,
	PROP_MKT_CAL_POINT_CALIBRATION,
	PROP_MKT_CAL_POINT_DONE,
	PROP_MKT_CAL_POINTS_STARTED,
	PROP_MKT_CAL_POINT_SOLUTION,
	PROP_MKT_CAL_POINT_AVERAGE,
	PROP_MKT_CAL_POINT_CV
};



struct _MktCalPointModelPrivate
{
	guint64     calibration;
	gboolean    done;
	gdouble     solution;
	gdouble     average;
	gdouble     cv;
};


#define MKT_CAL_POINT_MODEL_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), MKT_TYPE_CAL_POINT_MODEL, MktCalPointModelPrivate))


static gboolean
mkt_cal_point_model_done                                    ( MktCalPoint *ucal )
{
	g_return_val_if_fail(ucal != NULL,0);
	g_return_val_if_fail(MKT_IS_CAL_POINT(ucal),0);
	return MKT_CAL_POINT_MODEL(ucal)->priv->done;
}


static gdouble
mkt_cal_point_model_solution                          ( MktCalPoint *ucal )
{
	g_return_val_if_fail(ucal != NULL,0.);
	g_return_val_if_fail(MKT_IS_CAL_POINT(ucal),0.);
	return MKT_CAL_POINT_MODEL(ucal)->priv->solution;
}

static gdouble
mkt_cal_point_model_average                                     ( MktCalPoint *ucal )
{
	g_return_val_if_fail(ucal != NULL,1.);
	g_return_val_if_fail(MKT_IS_CAL_POINT(ucal),1.);
	return MKT_CAL_POINT_MODEL(ucal)->priv->average;
}
static gdouble
mkt_cal_point_model_cv                                     ( MktCalPoint *ucal )
{
	g_return_val_if_fail(ucal != NULL,1.);
	g_return_val_if_fail(MKT_IS_CAL_POINT(ucal),1.);
	return MKT_CAL_POINT_MODEL(ucal)->priv->cv;
}

static guint64
mkt_cal_point_model_calibration                          ( MktCalPoint *ucal )
{
	g_return_val_if_fail(ucal != NULL,0);
	g_return_val_if_fail(MKT_IS_CAL_POINT(ucal),0);
	return MKT_CAL_POINT_MODEL(ucal)->priv->calibration;
}
/*
static GSList*
mkt_cal_point_model_data                                ( MktCalPoint *ucal )
{
	g_return_val_if_fail(ucal != NULL,NULL);
	g_return_val_if_fail(MKT_IS_CAL_POINT(ucal),NULL);
	if(MKT_CAL_POINT_MODEL(ucal)->priv->data)g_slist_free_full(MKT_CAL_POINT_MODEL(ucal)->priv->data,g_object_unref);
	MKT_CAL_POINT_MODEL(ucal)->priv->data = mkt_model_select(MKT_TYPE_CAL_DATA_MODEL,"select * from $tablename where cal_data_point = %"G_GUINT64_FORMAT" ORDER BY cal_data_replicate ASC",mkt_model_ref_id(MKT_IMODEL(ucal)));
	return MKT_CAL_POINT_MODEL(ucal)->priv->data;
}

static GSList*
mkt_cal_point_model_data_replicates                           ( MktCalPoint *ucal , guint count  )
{
	g_return_val_if_fail(ucal != NULL,NULL);
	g_return_val_if_fail(MKT_IS_CAL_POINT(ucal),NULL);
	if(MKT_CAL_POINT_MODEL(ucal)->priv->replicates_data)g_slist_free_full(MKT_CAL_POINT_MODEL(ucal)->priv->data,g_object_unref);
	MKT_CAL_POINT_MODEL(ucal)->priv->replicates_data = mkt_model_select(MKT_TYPE_CAL_DATA_MODEL,"select * from $tablename where cal_data_point = %"G_GUINT64_FORMAT" ORDER BY cal_data_replicate ASC LIMIT %d",mkt_model_ref_id(MKT_IMODEL(ucal)),count);
	return MKT_CAL_POINT_MODEL(ucal)->priv->replicates_data;
}

static MktCalData*
mkt_cal_point_model_replicate_data (MktCalPoint *ucal , guint replicate )
{
	g_return_val_if_fail(ucal != NULL,NULL);
	g_return_val_if_fail(MKT_IS_CAL_POINT(ucal),NULL);
	if(MKT_CAL_POINT_MODEL(ucal)->priv->replicate_model)g_object_unref(MKT_CAL_POINT_MODEL(ucal)->priv->replicate_model);
	MKT_CAL_POINT_MODEL(ucal)->priv->replicate_model = MKT_CAL_DATA(mkt_model_select_one(MKT_TYPE_CAL_DATA_MODEL,"select * from $tablename where cal_data_point = %"G_GUINT64_FORMAT" and cal_data_replicate = %u",
			mkt_model_ref_id(MKT_IMODEL(ucal)),replicate));
	return MKT_CAL_POINT_MODEL(ucal)->priv->replicate_model;
}

static gboolean
mkt_cal_point_model_create_replicate_data (MktCalPoint *ucal , guint replicate )
{
	g_return_val_if_fail(ucal != NULL,TRUE);
	g_return_val_if_fail(MKT_IS_CAL_POINT(ucal),TRUE);
	if(NULL == mkt_cal_point_model_replicate_data(ucal,replicate))
	{
		MktModel *new_dat_point = mkt_model_new(MKT_TYPE_CAL_DATA_MODEL,
				"cal-data-point",mkt_model_ref_id(MKT_IMODEL(ucal)),
				"cal-data-replicate",replicate,
				"cal-data-value",0.0,
				"cal-data-trigger",0,NULL);
		g_object_unref(new_dat_point);
	}
	return TRUE;
}


static gboolean
mkt_cal_point_model_remove_replicate_data (MktCalPoint *ucal , guint replicate )
{
	g_return_val_if_fail(ucal != NULL,TRUE);
	g_return_val_if_fail(MKT_IS_CAL_POINT(ucal),TRUE);
	if(MKT_CAL_POINT_MODEL(ucal)->priv->data)g_slist_free_full(MKT_CAL_POINT_MODEL(ucal)->priv->data,g_object_unref);
	MKT_CAL_POINT_MODEL(ucal)->priv->data = NULL;
	//g_debug("mkt_cal_point_model_remove_replicate_data = %d",replicate);
	mkt_model_exec( MKT_TYPE_CAL_DATA_MODEL, "delete from $tablename where cal_data_point = %"G_GUINT64_FORMAT" and cal_data_replicate > %u",mkt_model_ref_id(MKT_IMODEL(ucal)),replicate);
	return TRUE;
}
*/


static void
mkt_cal_point_model_init_stream_interface ( MktCalPointInterface *iface )
{
	iface->cal_point_done=mkt_cal_point_model_done;
	iface->cal_point_solution=mkt_cal_point_model_solution;
	iface->cal_point_average=mkt_cal_point_model_average;
	iface->cal_point_cv=mkt_cal_point_model_cv;
	iface->cal_point_calibration=mkt_cal_point_model_calibration;

}

G_DEFINE_TYPE_WITH_CODE (MktCalPointModel, mkt_cal_point_model, MKT_TYPE_MODEL,
		                                         G_IMPLEMENT_INTERFACE (MKT_TYPE_CAL_POINT,
		                                		 mkt_cal_point_model_init_stream_interface)
		                                		 MKT_CREATE_MODEL_FULL(mkt_connection_new_default_calibration(TRUE)))


static void
mkt_cal_point_model_init (MktCalPointModel *mkt_cal_point_model)
{
	MktCalPointModelPrivate *priv      = MKT_CAL_POINT_MODEL_PRIVATE(mkt_cal_point_model);
	priv->calibration         = 0;
	priv->done                = FALSE;
	priv->solution            = 0.;
	priv->average             = 0.;
	priv->cv                  = 0.;

	mkt_cal_point_model->priv = priv;
	/* TODO: Add initialization code here */
}

static void
mkt_cal_point_model_finalize (GObject *object)
{
	//MktCalPointModel *data = MKT_CAL_POINT_MODEL(object);
	G_OBJECT_CLASS (mkt_cal_point_model_parent_class)->finalize (object);
}



static void
mkt_cal_point_model_delete  (MktModel *model )
{
	GSList *list = mkt_model_select(MKT_TYPE_CAL_DATA_MODEL,"select * from $tablename where cal_data_point = %"G_GUINT64_FORMAT,mkt_model_ref_id(MKT_IMODEL(model)));
	mkt_slist_free_full(list,mkt_model_unref_and_delete);
	//mkt_cal_point_clean_data(MKT_CAL_POINT(point));
	MKT_MODEL_CLASS (mkt_cal_point_model_parent_class)->delete (model);
}


static void
mkt_cal_point_model_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_CAL_POINT_MODEL (object));
	MktCalPointModel *cal = MKT_CAL_POINT_MODEL(object);
	switch (prop_id)
	{
	case PROP_MKT_CAL_POINT_CALIBRATION:
		cal->priv->calibration = g_value_get_uint64(value);
		break;
	case PROP_MKT_CAL_POINT_DONE:
		cal->priv->done = g_value_get_boolean(value);
		break;
	case PROP_MKT_CAL_POINT_SOLUTION:
		cal->priv->solution = g_value_get_double(value);
		break;
	case PROP_MKT_CAL_POINT_AVERAGE:
		cal->priv->average = g_value_get_double(value);
		break;
	case PROP_MKT_CAL_POINT_CV:
		cal->priv->cv = g_value_get_double(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_cal_point_model_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_CAL_POINT_MODEL (object));
	MktCalPointModel *cal = MKT_CAL_POINT_MODEL(object);
	switch (prop_id)
	{
	case PROP_MKT_CAL_POINT_CALIBRATION:
		g_value_set_uint64(value,cal->priv->calibration);
		break;
	case PROP_MKT_CAL_POINT_DONE:
		g_value_set_boolean(value,cal->priv->done);
		break;
	case PROP_MKT_CAL_POINT_SOLUTION:
		g_value_set_double(value,cal->priv->solution);
		break;
	case PROP_MKT_CAL_POINT_AVERAGE:
		g_value_set_double(value,cal->priv->average);
		break;
	case PROP_MKT_CAL_POINT_CV:
		g_value_set_double(value,cal->priv->cv);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_cal_point_model_class_init (MktCalPointModelClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	MktModelClass *model_class = MKT_MODEL_CLASS(klass);
	g_type_class_add_private (klass, sizeof (MktCalPointModelPrivate));
	object_class->finalize     = mkt_cal_point_model_finalize;
	object_class->set_property = mkt_cal_point_model_set_property;
	object_class->get_property = mkt_cal_point_model_get_property;
	model_class->delete        = mkt_cal_point_model_delete;

	g_object_class_override_property(object_class,PROP_MKT_CAL_POINT_CALIBRATION,"cal-point-calibration");
	g_object_class_override_property(object_class,PROP_MKT_CAL_POINT_DONE,"cal-point-done");
	g_object_class_override_property(object_class,PROP_MKT_CAL_POINT_SOLUTION,"cal-point-solution");
	g_object_class_override_property(object_class,PROP_MKT_CAL_POINT_AVERAGE,"cal-point-average");
	g_object_class_override_property(object_class,PROP_MKT_CAL_POINT_CV,"cal-point-cv");

}

