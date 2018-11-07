/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktCalibration
 * @{
 * @file  mkt-nitri_calibration.c	NitriCalibration model interface
 * @brief This is NitriCalibration model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */


#include "mkt-calibration.h"
#include "market-time.h"
#include "mkt-value.h"
#include "mkt-connection.h"
#include "mkt-calibration-model.h"
#include <math.h>
#include "mkt-cal-point-model.h"
#include "mkt-cal-data-model.h"
#include "mkt-point-model.h"
#include "mkt-channel-model.h"
#include "mkt-stream-model.h"
#include "mkt-process-model.h"

#include "../config.h"
#include <glib/gi18n-lib.h>

#if GLIB_CHECK_VERSION(2,31,7)
static GRecMutex init_rmutex;
#define MUTEX_LOCK() g_rec_mutex_lock(&init_rmutex)
#define MUTEX_UNLOCK() g_rec_mutex_unlock(&init_rmutex)
#else
static GStaticRecMutex init_mutex = G_STATIC_REC_MUTEX_INIT;
#define MUTEX_LOCK() g_static_rec_mutex_lock(&init_mutex)
#define MUTEX_UNLOCK() g_static_rec_mutex_unlock(&init_mutex)
#endif


static void
mkt_calibration_base_init (gpointer g_iface)
{
	static gboolean is_mkt_calibration_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_mkt_calibration_initialized)
	{
		g_object_interface_install_property (g_iface,
				g_param_spec_uint ("calibration-identification",
						"Calibration property",
						"Set get measurement property",
						0,G_MAXUINT,301,
						G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

		g_object_interface_install_property (g_iface,
				g_param_spec_uint ("calibration-level",
						"Calibration property",
						"Set get measurement property",
						0,4,0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

		g_object_interface_install_property (g_iface,
				g_param_spec_uint64 ("calibration-process",
						"Calibration calibration solution",
						"Set|Get calibration solution",
						0,G_MAXUINT64,0,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT   |  MKT_MODEL_DB_PROP));

		g_object_interface_install_property (g_iface,
				g_param_spec_uint64("calibration-stream",
						"Calibration stream reference",
						"Calibration stream reference",
						0,G_MAXUINT64,0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP ));
		g_object_interface_install_property (g_iface,
				g_param_spec_uint64("calibration-channel",
						"Calibration channel reference",
						"Calibration channel reference",
						0,G_MAXUINT64,0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP ));

		g_object_interface_install_property (g_iface,
				g_param_spec_boolean  ("calibration-activated",
						"Calibration is activated",
						"Set|Get calibration is activated",
						FALSE,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));

		g_object_interface_install_property (g_iface,
				g_param_spec_boolean  ("calibration-main",
						"Calibration is main",
						"Set|Get calibration is main",
						FALSE,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));

		g_object_interface_install_property (g_iface,
				g_param_spec_boolean  ("calibration-done",
						"Calibration is done",
						"Set|Get calibration is done",
						FALSE,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));

		g_object_interface_install_property (g_iface,
				g_param_spec_double ("calibration-slope",
						"Calibration slope property",
						"Set slope property",
						-G_MAXDOUBLE,G_MAXDOUBLE,1.0,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  ));

		g_object_interface_install_property (g_iface,
				g_param_spec_double ("calibration-intercept",
						"Calibration intercept property",
						"Set intercept property",
						-G_MAXDOUBLE,G_MAXDOUBLE,0.0,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  ));

		g_object_interface_install_property (g_iface,
				g_param_spec_double ("calibration-determination",
						"Calibration determination property",
						"Set determination property",
						-G_MAXDOUBLE,G_MAXDOUBLE,0.0,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  ));

		g_object_interface_install_property (g_iface,
				g_param_spec_double ("calibration-cv",
						"Calibration intercept property",
						"Set intercept property",
						-G_MAXDOUBLE,G_MAXDOUBLE,0.0,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  ));

		g_object_interface_install_property (g_iface,
				g_param_spec_double ("calibration-deviation",
						"Calibration determination property",
						"Set determination property",
						-G_MAXDOUBLE,G_MAXDOUBLE,0.0,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  ));

		g_object_interface_install_property (g_iface,
				g_param_spec_double("calibration-changed",
						"Calibration real time",
						"Calibration real time",
						0,G_MAXDOUBLE,0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP ));

		g_object_interface_install_property (g_iface,
				g_param_spec_string  ("calibration-date",
						"Calibration date ISO 8601",
						"Set|Get date RFC 3339 encoded string ISO 8601",
						"YYYY-MM-DDTHH:MM:SSZ",
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));


		is_mkt_calibration_initialized = TRUE;
	}
	MUTEX_UNLOCK();
}

GType
mkt_calibration_get_type (void)
{
	static GType iface_type = 0;
	if (iface_type == 0)
	{
		static const GTypeInfo info = {
				sizeof (MktCalibrationInterface),
				(GBaseInitFunc) mkt_calibration_base_init,
				(GBaseFinalizeFunc) NULL,
				(GClassInitFunc) NULL,
				NULL,
				NULL,
				0,
				0,
				(GInstanceInitFunc) NULL,
				0
		};
		MUTEX_LOCK();
		if (iface_type == 0)
		{
			iface_type = g_type_register_static (G_TYPE_INTERFACE, "MktCalibrationInterface",&info, 0);

		}
		MUTEX_UNLOCK();
	}
	return iface_type;
}


guint
mkt_calibration_identification                  ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL , 0);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration) , 0);
	if(MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_identification )
		return MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_identification(calibration);
	return 0;
}

guint
mkt_calibration_level                                       ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL , 0);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration) , 0);
	if(MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_level )
		return MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_level(calibration);
	return 0;
}

guint64
mkt_calibration_channel                         ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL , 0);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration) , 0);
	if(MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_channel )
		return MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_channel(calibration);
	return 0;
}

guint64
mkt_calibration_stream                         ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL , 0);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration) , 0);
	if(MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_stream )
		return MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_stream(calibration);
	return 0;
}

guint64
mkt_calibration_process                         ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL , 0);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration) , 0);
	if(MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_process )
		return MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_process(calibration);
	return 0;
}

gboolean
mkt_calibration_activated                                   ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,FALSE);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),FALSE);
	if(MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_activated)
		return MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_activated(calibration);
	return FALSE;
}

gboolean
mkt_calibration_main                                        ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,FALSE);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),FALSE);
	if(MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_main)
		return MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_main(calibration);
	return FALSE;
}

gboolean
mkt_calibration_done                                        ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,FALSE);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),FALSE);
	if(MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_done)
		return MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_done(calibration);
	return FALSE;
}


gdouble
mkt_calibration_slope                                       ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,1.0);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),1.0);
	if(MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_slope)
		return MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_slope(calibration);
	return 1.0;
}



gdouble
mkt_calibration_intercept                                   ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,0.0);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),0.0);
	if(MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_intercept)
		return MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_intercept(calibration);
	return 0.0;
}

gdouble
mkt_calibration_determination                                       ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,0.0);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),0.0);
	if(MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_determination)
		return MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_determination(calibration);
	return 0.0;
}

gdouble
mkt_calibration_cv_parameter                                ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,0.0);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),0.0);
	if(MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_cv)
		return MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_cv(calibration);
	return 0.0;
}

gdouble
mkt_calibration_deviation                                   ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,0.0);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),0.0);
	if(MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_deviation)
		return MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_deviation(calibration);
	return 0.0;
}

gdouble
mkt_calibration_time                                        ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL , 0.);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration) , 0.);
	if(MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_time )
		return MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_time(calibration);
	return 0.;
}

const gchar*
mkt_calibration_date                                        ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,NULL);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),NULL);
	if(MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_date)
		return MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_date(calibration);
	return NULL;
}

void
mkt_calibration_update_time                                 ( MktCalibration *calibration )
{
	g_return_if_fail(calibration!=NULL);
	g_return_if_fail(MKT_IS_CALIBRATION(calibration));
	gdouble time = market_db_time_now();
	const gchar * time_val = market_db_get_date_sqlite_format(time);
	g_object_set(calibration,"calibration-changed",time,"calibration-date",time_val,NULL);
}


GSList*
mkt_calibration_points                                      ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,NULL);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),NULL);
	if(MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_points)
		return MKT_CALIBRATION_GET_INTERFACE(calibration)->calibration_points(calibration);
	return NULL;
}

GSList*
mkt_calibration_next_points                               ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,NULL);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),NULL);
	GSList *ponts = mkt_model_select(MKT_TYPE_CAL_POINT_MODEL,"select * from $tablename where cal_point_calibration = %"G_GUINT64_FORMAT" and cal_point_done = 0 ORDER BY cal_point_solution ASC",mkt_model_ref_id(MKT_IMODEL(calibration)));
	return ponts;
}

MktCalPoint*
mkt_calibration_max_cv                                      ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,NULL);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),NULL);
	GSList *points = mkt_model_select(MKT_TYPE_CAL_POINT_MODEL,"select * from $tablename where cal_point_calibration = %"G_GUINT64_FORMAT,mkt_model_ref_id(MKT_IMODEL(calibration)));
	MktCalPoint *point = NULL;
	GSList *lp = NULL;
	for(lp=points;lp!=NULL;lp=lp->next)
	{
		if(point == NULL)
		{
			point = (MKT_CAL_POINT(g_object_ref(lp->data)));
		}
		else if(mkt_cal_point_cv(MKT_CAL_POINT(lp->data))>mkt_cal_point_cv(MKT_CAL_POINT(point)))
		{
			g_object_unref(point);
			point = (MKT_CAL_POINT(g_object_ref(lp->data)));
		}
	}
	g_slist_free_full(points,g_object_unref);
	return point;
}

gboolean
mkt_calibration_point_add_data                              ( MktCalPoint *point, MktCalData  *data)
{
	g_return_val_if_fail(point != NULL,FALSE);
	g_return_val_if_fail(MKT_IS_CAL_POINT(point),FALSE);
	g_return_val_if_fail(data != NULL,FALSE);
	g_return_val_if_fail(MKT_IS_CAL_DATA(data),FALSE);
	g_object_set(data,"cal-data-point",mkt_model_ref_id(MKT_IMODEL(point)),NULL);
	return TRUE;
}

GSList*
mkt_calibration_point_data                                  ( MktCalPoint *point)
{
	g_return_val_if_fail(point != NULL,NULL);
	g_return_val_if_fail(MKT_IS_CAL_POINT(point),NULL);
	GSList *data = mkt_model_select(MKT_TYPE_CAL_DATA_MODEL,"select * from $tablename where cal_data_point = %"G_GUINT64_FORMAT,mkt_model_ref_id(MKT_IMODEL(point)));
	return data;
}

#define _AVERAGE  0.00000000000000001
void
mkt_calibration_calculate                                   ( MktCalibration *calibration )
{
	GSList *points = mkt_calibration_points(calibration);
	g_return_if_fail(points!=NULL);

	guint   c    = 0.;
	gdouble xm   = 0.;
	gdouble ym   = 0.;
	gdouble xx   = 0.;
	gdouble yy   = 0.;
	gdouble xym  = 0.;
	gdouble xx2  = 0.;
	gdouble yy2  = 0.;
	gdouble r    = 0.;
	gdouble rr   = 0.;
	gdouble b = 1.0,a=0.0;
	gdouble average  = 0.0;
	gdouble solution = 0.0;
	if(g_slist_length(points)>1)
	{
		GSList *l    = NULL;

		for(l=points;l!=NULL;l=l->next)
		{
			mkt_cal_point_calculate(MKT_CAL_POINT(l->data));
			c++;
			average  = mkt_cal_point_average(MKT_CAL_POINT(l->data))+_AVERAGE;
			solution = mkt_cal_point_solution(MKT_CAL_POINT(l->data));
			ym +=average;
			xm +=solution;
		}
		ym = ym/c;
		xm = xm/c;
		for(l=points;l!=NULL;l=l->next)
		{
			average  = mkt_cal_point_average(MKT_CAL_POINT(l->data))+_AVERAGE;
			solution = mkt_cal_point_solution(MKT_CAL_POINT(l->data));
			xx += (solution-xm);
			yy += (average-ym);
			xym+= (solution-xm)*(average-ym);
			xx2+= (solution-xm)*(solution-xm);
			yy2+= (average-ym)*(average-ym);
		}
		//g_debug("TEST ---------------------------------------------------------MITTELWERT=%f",mittelwert);
		r  = xym/sqrt(xx2*yy2);
		rr = r*r;

		b = xym/xx2;
		a = ym-b*xm;
	}
	else
	{
		average  = mkt_cal_point_average(MKT_CAL_POINT(points->data))+_AVERAGE;
		solution = mkt_cal_point_solution(MKT_CAL_POINT(points->data));
		a = 0;
		b = average/solution;
	}
	g_object_set(calibration,"calibration-slope",b,"calibration-intercept",a,"calibration-determination",rr,NULL);
	return ;
}

MktChannel*
mkt_calibration_channel_model                               ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,NULL);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),NULL);
	return MKT_CHANNEL(mkt_model_select_one(MKT_TYPE_CHANNEL_MODEL,"select * from $tablename where ref_id = %"G_GUINT64_FORMAT" ORDER BY ref_id DESC LIMIT 1",mkt_calibration_channel(calibration)));
}

MktStream*
mkt_calibration_stream_model                                ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,NULL);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),NULL);
	return MKT_STREAM(mkt_model_select_one(MKT_TYPE_STREAM_MODEL,"select * from $tablename where ref_id = %"G_GUINT64_FORMAT" ORDER BY ref_id DESC LIMIT 1",mkt_calibration_stream(calibration)));

}

MktProcess*
mkt_calibration_process_model                               ( MktCalibration *calibration )
{
	g_return_val_if_fail(calibration != NULL,NULL);
	g_return_val_if_fail(MKT_IS_CALIBRATION(calibration),NULL);
	return MKT_PROCESS(mkt_model_select_one(MKT_TYPE_PROCESS_MODEL,"select * from $tablename where ref_id = %"G_GUINT64_FORMAT" ORDER BY ref_id DESC LIMIT 1",mkt_calibration_process(calibration)));

}

MktCalibration*
mkt_calibration_activated_for_channel                          ( guint64 channel_id )
{
	MktCalibration *calibration = MKT_CALIBRATION(mkt_model_select_one(MKT_TYPE_CALIBRATION_MODEL,"select * from $tablename where calibration_channel = %"G_GUINT64_FORMAT" and calibration_activated = 1 ORDER BY ref_id DESC LIMIT 1",channel_id));
	return calibration;
}

MktCalibration*
mkt_calibration_last_for_channel                          ( guint64 channel_id )
{
	MktCalibration *calibration = MKT_CALIBRATION(mkt_model_select_one(MKT_TYPE_CALIBRATION_MODEL,"select * from $tablename where calibration_channel = %"G_GUINT64_FORMAT" ORDER BY ref_id DESC LIMIT 1",channel_id));
	return calibration;
}

MktCalibration*
mkt_calibration_main_for_channel                            ( guint64 channel_id )
{
	MktCalibration *calibration = MKT_CALIBRATION(mkt_model_select_one(MKT_TYPE_CALIBRATION_MODEL,"select * from $tablename where calibration_channel = %"G_GUINT64_FORMAT" and calibration_main = 1 ORDER BY ref_id DESC LIMIT 1",channel_id));
	return calibration;
}



GSList*
mkt_calibration_last_for_process                          ( guint64 process_id )
{
	GSList *calibrations = mkt_model_select(MKT_TYPE_CALIBRATION_MODEL,"select * from $tablename where calibration_process = %"G_GUINT64_FORMAT,process_id);
	return calibrations;
}

GSList*
mkt_calibration_default_points                                      ( guint64 channel_id )
{
	GSList *points = mkt_model_select(MKT_TYPE_POINT_MODEL,"select * from $tablename where point_ref =%"G_GUINT64_FORMAT" ORDER BY point_solution ASC",channel_id);
	return points;
}


MktCalibration*
mkt_calibration_from_data                                   ( MktCalData *data)
{
	g_return_val_if_fail(data != NULL,NULL);
	g_return_val_if_fail(MKT_IS_CAL_DATA(data),NULL);
	MktCalibration* calibration = NULL;
	MktCalPoint *point = MKT_CAL_POINT(mkt_model_select_one(MKT_TYPE_CAL_POINT_MODEL,"select * from $tablename where ref_id = %"G_GUINT64_FORMAT" ORDER BY ref_id DESC LIMIT 1",mkt_cal_data_point(data)));
	if(point)
	{

		calibration = MKT_CALIBRATION(mkt_model_select_one(MKT_TYPE_CALIBRATION_MODEL,"select * from $tablename where ref_id = %"G_GUINT64_FORMAT" ORDER BY ref_id DESC LIMIT 1",mkt_cal_point_calibration(point)));
		g_object_unref(point);
	}
	return calibration;
}

MktCalibration*
mkt_calibration_from_point                                  ( MktCalPoint *point )
{
	g_return_val_if_fail(point != NULL,NULL);
	g_return_val_if_fail(MKT_IS_CAL_POINT(point),NULL);
	MktCalibration* calibration = NULL;
	calibration = MKT_CALIBRATION(mkt_model_select_one(MKT_TYPE_CALIBRATION_MODEL,"select * from $tablename where ref_id = %"G_GUINT64_FORMAT" ORDER BY ref_id DESC LIMIT 1",mkt_cal_point_calibration(point)));
	return calibration;
}


gdouble
mkt_calibration_max_deviation                               ( MktCalibration *calibration, gdouble grund_slope, gdouble grund_intercept )
{
	gdouble max_deviation = -1.0;
	GSList *points = mkt_calibration_points(calibration);
	GSList *l = NULL;
	for(l=points;l!=NULL;l=l->next)
	{
		MktCalPoint *p = MKT_CAL_POINT(l->data);
		if(mkt_cal_point_solution(p) > 0.0)
		{
			gdouble result = (mkt_cal_point_average(p)-grund_intercept)/grund_slope;
			gdouble defference = (gdouble) ABS( 100.-((result/mkt_cal_point_solution(p))*100.));
			if(max_deviation<defference)max_deviation = defference;
		}

	}
	if(max_deviation>100.0)max_deviation = 100.0;
	return max_deviation;
}


/** @} */
