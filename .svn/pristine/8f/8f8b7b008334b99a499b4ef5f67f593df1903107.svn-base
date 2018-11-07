/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktCal
 * @{
 * @file  mkt-nitri_cal.c	NitriCal model interface
 * @brief This is NitriCal model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */


#include "mkt-cal.h"
#include "mkt-cal-model.h"
#include "mkt-calibration.h"
#include "mkt-cal-point-model.h"
#include "mkt-cal-data-model.h"

#include <market-time.h>
#include <mkt-value.h>
#include <math.h>
#include <mkt-connection.h>

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
mkt_cal_base_init (gpointer g_iface)
{
	static gboolean is_mkt_cal_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_mkt_cal_initialized)
	{
		g_object_interface_install_property (g_iface,
				g_param_spec_double ("cal-created",
						"Cal intercept property",
						"Set intercept property",
						-G_MAXDOUBLE,G_MAXDOUBLE,0.0,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  ));
		g_object_interface_install_property (g_iface,
				g_param_spec_uint64("cal-calibration",
						"Cal cal calibration parent",
						"Set|Get calibration parent address",
						0,G_MAXUINT64,0,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));
		g_object_interface_install_property (g_iface,
				g_param_spec_uint64("cal-process",
						"Cal process id",
						"Set|Get calibration process id",
						0,G_MAXUINT64,0,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));
		g_object_interface_install_property (g_iface,
				g_param_spec_uint64 ("cal-acative-point",
						"Cal cal vessel address",
						"Set|Get cal stream address",
						0,G_MAXUINT64,0,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));
		g_object_interface_install_property (g_iface,
				g_param_spec_double ("cal-slope",
						"Cal slope property",
						"Set slope property",
						-G_MAXDOUBLE,G_MAXDOUBLE,1.0,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  ));

		g_object_interface_install_property (g_iface,
				g_param_spec_double ("cal-airflow-in",
						"Cal slope property",
						"Set slope property",
						-G_MAXDOUBLE,G_MAXDOUBLE,30.0,
				         G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  ));

		g_object_interface_install_property (g_iface,
				g_param_spec_double ("cal-airflow-out",
						"Cal slope property",
						"Set slope property",
						-G_MAXDOUBLE,G_MAXDOUBLE,30.0,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  ));

		g_object_interface_install_property (g_iface,
				g_param_spec_double ("cal-intercept",
						"Cal intercept property",
						"Set intercept property",
						-G_MAXDOUBLE,G_MAXDOUBLE,0.0,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP  ));
		g_object_interface_install_property (g_iface,
				g_param_spec_uint("cal-kind",
						"Cal cal calibration kind",
						"Set|Get calibration kind",
						0,10,0,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));
		g_object_interface_install_property (g_iface,
				g_param_spec_boolean("cal-activated",
						"Cal cal calibration activated",
						"Set|Get calibration activated",
						FALSE,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));
		g_object_interface_install_property (g_iface,
				g_param_spec_uint("cal-number",
						"Cal cal calibration number",
						"Set|Get calibration number",
						0,G_MAXUINT32,0,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));


		is_mkt_cal_initialized = TRUE;
	}
	MUTEX_UNLOCK();
}

GType
mkt_cal_get_type (void)
{
	static GType iface_type = 0;
	if (iface_type == 0)
	{
		static const GTypeInfo info = {
				sizeof (MktCalInterface),
				(GBaseInitFunc) mkt_cal_base_init,
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
			iface_type = g_type_register_static (G_TYPE_INTERFACE, "MktCalInterface",&info, 0);
		}
		MUTEX_UNLOCK();
	}
	return iface_type;
}

MktModel*
mkt_cal_calibration                                 ( MktCal *cal )
{
	g_return_val_if_fail(cal != NULL,NULL);
	g_return_val_if_fail(MKT_IS_CAL(cal),NULL);
	if(MKT_CAL_GET_INTERFACE(cal)->cal_calibration)
		return MKT_CAL_GET_INTERFACE(cal)->cal_calibration(cal);
	return NULL;
}


MktCalPoint*
mkt_cal_solution_point                                 ( MktCal *cal , guint solution)
{
	g_return_val_if_fail(cal != NULL,NULL);
	g_return_val_if_fail(MKT_IS_CAL(cal),NULL);
	if(MKT_CAL_GET_INTERFACE(cal)->solution_point)
		return MKT_CAL_GET_INTERFACE(cal)->solution_point(cal,solution);
	return NULL;
}

gboolean
mkt_cal_next_point                                       ( MktCal *cal )
{
	g_return_val_if_fail(cal != NULL,FALSE);
	g_return_val_if_fail(MKT_IS_CAL(cal),FALSE);
	if(MKT_CAL_GET_INTERFACE(cal)->next_point)
		return MKT_CAL_GET_INTERFACE(cal)->next_point(cal);
	return FALSE;
}

gboolean
mkt_cal_have_next_point                                  ( MktCal *cal )
{
	g_return_val_if_fail(cal != NULL,FALSE);
	g_return_val_if_fail(MKT_IS_CAL(cal),FALSE);
	if(MKT_CAL_GET_INTERFACE(cal)->have_next_point)
		return MKT_CAL_GET_INTERFACE(cal)->have_next_point(cal);
	return FALSE;
}


gboolean
mkt_cal_pre_point                                        ( MktCal *cal )
{
	g_return_val_if_fail(cal != NULL,FALSE);
	g_return_val_if_fail(MKT_IS_CAL(cal),FALSE);
	if(MKT_CAL_GET_INTERFACE(cal)->pre_point)
		return MKT_CAL_GET_INTERFACE(cal)->pre_point(cal);
	return FALSE;
}



GSList*
mkt_cal_points                                           ( MktCal *cal )
{
	g_return_val_if_fail(cal != NULL,NULL);
	g_return_val_if_fail(MKT_IS_CAL(cal),NULL);
	if(MKT_CAL_GET_INTERFACE(cal)->points)
		return MKT_CAL_GET_INTERFACE(cal)->points(cal);
	return NULL;
}


gdouble
mkt_cal_slope                               ( MktCal *cal )
{
	g_return_val_if_fail(cal != NULL,0.);
	g_return_val_if_fail(MKT_IS_CAL(cal),0.);
	if(MKT_CAL_GET_INTERFACE(cal)->cal_slope)
		return MKT_CAL_GET_INTERFACE(cal)->cal_slope(cal);
	return 0.0;
}

gdouble
mkt_cal_intercept                                        ( MktCal *cal )
{
	g_return_val_if_fail(cal != NULL,0.);
	g_return_val_if_fail(MKT_IS_CAL(cal),0.);
	if(MKT_CAL_GET_INTERFACE(cal)->cal_intercept)
		return MKT_CAL_GET_INTERFACE(cal)->cal_intercept(cal);
	return 0.0;
}

guint64
mkt_cal_process                                          ( MktCal *cal )
{
	g_return_val_if_fail(cal != NULL,0);
	g_return_val_if_fail(MKT_IS_CAL(cal),0);
	if(MKT_CAL_GET_INTERFACE(cal)->cal_process)
		return MKT_CAL_GET_INTERFACE(cal)->cal_process(cal);
	return 0;
}


gboolean
mkt_cal_reset                           ( MktCal *cal )
{
	g_return_val_if_fail(cal != NULL,FALSE);
	g_return_val_if_fail(MKT_IS_CAL(cal),FALSE);
	if(MKT_CAL_MODEL_GET_CLASS(cal)->reset)
		return MKT_CAL_MODEL_GET_CLASS(cal)->reset(MKT_CAL_MODEL(cal));
	return FALSE;
}



gboolean
mkt_cal_delete_points                                    ( MktCal *cal )
{
	g_return_val_if_fail(cal != NULL,FALSE);
	g_return_val_if_fail(MKT_IS_CAL(cal),FALSE);
	if(MKT_CAL_GET_INTERFACE(cal)->delete_points)
		return MKT_CAL_GET_INTERFACE(cal)->delete_points(cal);
	return FALSE;
}

void
mkt_cal_update                                           ( MktCal *cal )
{
	g_return_if_fail(cal != NULL);
	g_return_if_fail(MKT_IS_CAL(cal));
	g_object_set(cal,"cal-created",market_db_time_now(),NULL);

}

gboolean
mkt_cal_replace_points                                          ( MktCal *src , MktCal *dest  )
{
	g_return_val_if_fail(src != NULL,FALSE);
	g_return_val_if_fail(MKT_IS_CAL(src),FALSE);
	g_return_val_if_fail(dest != NULL,FALSE);
	g_return_val_if_fail(MKT_IS_CAL(dest),FALSE);
	mkt_cal_delete_points(dest);
	GSList *points = mkt_cal_points(src);
	if(points)
	{
		GSList *l = NULL;
		for(l=points;l!=NULL;l=l->next)
		{
			MktModel *new_point = mkt_model_new(MKT_TYPE_CAL_POINT_MODEL,
					"cal-point-cal", mkt_model_ref_id(MKT_IMODEL(dest)),
					"cal-point-solution",mkt_cal_point_solution(MKT_CAL_POINT(l->data)),
					"cal-point-average",mkt_cal_point_average(MKT_CAL_POINT(l->data)),
					"cal-point-cv",mkt_cal_point_cv(MKT_CAL_POINT(l->data)),
					"cal-point-done",mkt_cal_point_done(MKT_CAL_POINT(l->data)),
					NULL);
			mkt_cal_point_replace_data(MKT_CAL_POINT(l->data),MKT_CAL_POINT(new_point));
			g_object_unref(new_point);
		}
	}
	return TRUE;
}

gboolean
mkt_cal_add_point                                         ( MktCal *cal , gdouble solution)
{
	g_return_val_if_fail(cal != NULL,FALSE);
	g_return_val_if_fail(MKT_IS_CAL(cal),FALSE);
	MktModel *new_point = mkt_model_new(MKT_TYPE_CAL_POINT_MODEL,"cal-point-cal", mkt_model_ref_id(MKT_IMODEL(cal)),
							 "cal-point-solution",solution,NULL);
	g_object_unref(new_point);
	return TRUE;
}




/** @} */

