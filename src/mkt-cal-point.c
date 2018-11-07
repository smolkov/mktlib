/*
 * @ingroup MktCalPoint
 * @{
 * @file  mkt-nitri_cal_point.c	NitriCalPoint model interface
 * @brief This is NitriCalPoint model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */


#include "mkt-cal-point.h"
#include <market-time.h>
#include <mkt-model.h>
#include <mkt-connection.h>
#include <mkt-calibration.h>
#include "mkt-cal-data.h"
#include "mkt-cal-data-model.h"
#include "mkt-cal.h"
#include <math.h>
#include <mkt-value.h>

#define GETTEXT_PACKAGE "mkt-library"
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
mkt_cal_point_base_init (gpointer g_iface)
{
	static gboolean is_mkt_cal_point_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_mkt_cal_point_initialized)
	{

		g_object_interface_install_property (g_iface,
				g_param_spec_uint64 ("cal-point-calibration",
						"Cal point calibration address",
						"Set|Get cal point calibration address",
						0,G_MAXUINT64,0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));
		g_object_interface_install_property (g_iface,
				g_param_spec_boolean ("cal-point-done",
						"Cal point calibration done",
						"Set|Get cal point calibration done",
						FALSE,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));
		g_object_interface_install_property (g_iface,
				g_param_spec_double  ("cal-point-solution",
						"solution value",
						"Set|Get cal point solution value",
						0.01,G_MAXDOUBLE,500.0,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));

		g_object_interface_install_property (g_iface,
				g_param_spec_double  ("cal-point-average",
						"average value",
						"Set|Get cal point average value",
						0.0,G_MAXDOUBLE,0.0,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));

		g_object_interface_install_property (g_iface,
				g_param_spec_double  ("cal-point-cv",
						"cv value",
						"Set|Get cal point cv value",
						0.0,G_MAXDOUBLE,0.0,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));


		is_mkt_cal_point_initialized = TRUE;
	}
	MUTEX_UNLOCK();
}

GType
mkt_cal_point_get_type (void)
{
	static GType iface_type = 0;
	if (iface_type == 0)
	{
		static const GTypeInfo info = {
				sizeof (MktCalPointInterface),
				(GBaseInitFunc) mkt_cal_point_base_init,
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
			iface_type = g_type_register_static (G_TYPE_INTERFACE, "MktCalPointInterface",&info, 0);
		}
		MUTEX_UNLOCK();
	}
	return iface_type;
}


gboolean
mkt_cal_point_done                                      ( MktCalPoint *ucal_point )
{
	g_return_val_if_fail(ucal_point != NULL,FALSE);
	g_return_val_if_fail(MKT_IS_CAL_POINT(ucal_point),FALSE);
	if(MKT_CAL_POINT_GET_INTERFACE(ucal_point)->cal_point_done)
		return MKT_CAL_POINT_GET_INTERFACE(ucal_point)->cal_point_done(ucal_point);
	return 0;

}
gdouble
mkt_cal_point_average                                     ( MktCalPoint *ucal_point )
{
	g_return_val_if_fail(ucal_point != NULL,0);
	g_return_val_if_fail(MKT_IS_CAL_POINT(ucal_point),0);
	if(MKT_CAL_POINT_GET_INTERFACE(ucal_point)->cal_point_average)
		return MKT_CAL_POINT_GET_INTERFACE(ucal_point)->cal_point_average(ucal_point);
	return 0;
}


gdouble
mkt_cal_point_solution                                    ( MktCalPoint *ucal_point )
{
	g_return_val_if_fail(ucal_point != NULL,0);
	g_return_val_if_fail(MKT_IS_CAL_POINT(ucal_point),0);
	if(MKT_CAL_POINT_GET_INTERFACE(ucal_point)->cal_point_solution)
		return MKT_CAL_POINT_GET_INTERFACE(ucal_point)->cal_point_solution(ucal_point);
	return 0;
}

gdouble
mkt_cal_point_cv                                          ( MktCalPoint *ucal_point )
{
	g_return_val_if_fail(ucal_point != NULL,0.);
	g_return_val_if_fail(MKT_IS_CAL_POINT(ucal_point),0.);
	if(MKT_CAL_POINT_GET_INTERFACE(ucal_point)->cal_point_cv)
		return MKT_CAL_POINT_GET_INTERFACE(ucal_point)->cal_point_cv(ucal_point);
	return 0.;
}

guint64
mkt_cal_point_calibration                                 ( MktCalPoint *cal_point )
{
	g_return_val_if_fail(cal_point != NULL,0);
	g_return_val_if_fail(MKT_IS_CAL_POINT(cal_point),0);
	if(MKT_CAL_POINT_GET_INTERFACE(cal_point)->cal_point_calibration)
		return MKT_CAL_POINT_GET_INTERFACE(cal_point)->cal_point_calibration(cal_point);
	return 0;
}


void
mkt_cal_point_calculate                                   ( MktCalPoint *cal_point  )
{
	g_return_if_fail(cal_point!=NULL);

	GSList *data = mkt_model_select(MKT_TYPE_CAL_DATA_MODEL,"select * from $tablename where cal_data_point = %"G_GUINT64_FORMAT,mkt_model_ref_id(MKT_IMODEL(cal_point)));
	gdouble   standardDeviation = 0.;
	gdouble   sum_d_sqr=0.;
	gdouble   cv = 0.0;
	gdouble raw = 0.;
	GSList *l = NULL;
	guint     curr_replicate=0;
	for(l=data;l!=NULL;l=l->next)
	{
		if ( !mkt_cal_data_outlier(MKT_CAL_DATA(l->data)) )
		{
			raw +=  mkt_cal_data_value(MKT_CAL_DATA(l->data));
			curr_replicate++;
		}
	}
	raw /=  curr_replicate;
	sum_d_sqr = 0.;
	for(l=data;l!=NULL;l=l->next)
		if ( !mkt_cal_data_outlier(MKT_CAL_DATA(l->data))  )
			sum_d_sqr += ( mkt_cal_data_value(MKT_CAL_DATA(l->data))- raw)*( mkt_cal_data_value(MKT_CAL_DATA(l->data))- raw);
	standardDeviation = sqrt( sum_d_sqr / ((curr_replicate-1)>0?(curr_replicate-1):1) );
	if(raw > 0.000000)
		cv= 100. * standardDeviation / fabs( raw);
	g_object_set(cal_point,"cal-point-cv",cv,"cal-point-average",raw,NULL);
}
