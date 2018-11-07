/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktStatistic
 * @{
 * @file  mkt-statistic.c	Statistic model interface
 * @brief This is Statistic model interface description.
 *
 *
 *  Copyright (C) LAR 2015
 *
 * @author A.Smolkov <asmolkov@lar.com>
 *
 */


#include "mkt-statistic.h"
#include "market-time.h"
#include "mkt-value.h"


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
mkt_statistic_base_init (gpointer g_iface)
{
	static gboolean is_statistic_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_statistic_initialized)
	{
		g_object_interface_install_property (g_iface,
				g_param_spec_uint("statistic-replicates",
						_("statistic replicates"),
						_("Set get statistic replicates"),
						1,100,1,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));
		g_object_interface_install_property (g_iface,
				g_param_spec_uint("statistic-outliers",
						_("statistic outlier"),
						_("Set get statistic outlier"),
						0,30,0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE ));
		g_object_interface_install_property (g_iface,
				g_param_spec_double ("statistic-max-cv",
						_("statistic max CV"),
						_("Set get statistic max cv"),
						0.,100.,2.5,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE ));
		g_object_interface_install_property (g_iface,
				g_param_spec_double ("statistic-threshold",
						_("statistic threshold"),
						_("Set get statistic threshold"),
						0.,100.,0.,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE ));
		g_object_interface_install_property (g_iface,
				g_param_spec_double ("statistic-jump",
						_("statistic jump"),
						_("statistic jump"),
						0.,100.,0.,
						G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE ));

		g_object_interface_install_property (g_iface,
				g_param_spec_uint ("statistic-amount-counter",
						_("statistic amount counter"),
						_("Set|Get statistic amount counter"),
						0,5,0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property (g_iface,
				g_param_spec_double ("statistic-amount-percentage",
						_("statistic amount percentage"),
						_("Set|Get statistic amount percentage"),
						20.0,50.0,20.0,
						G_PARAM_READABLE | G_PARAM_WRITABLE  | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE));

		is_statistic_initialized = TRUE;
	}
	MUTEX_UNLOCK();
}

GType
mkt_statistic_get_type (void)
{
	static GType iface_type = 0;
	if (iface_type == 0)
	{
		static const GTypeInfo info = {
				sizeof (MktStatisticInterface),
				(GBaseInitFunc) mkt_statistic_base_init,
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
			iface_type = g_type_register_static (G_TYPE_INTERFACE, "MktStatisticInterface",&info, 0);
			g_type_interface_add_prerequisite (iface_type, MKT_TYPE_MODEL);
		}
		MUTEX_UNLOCK();
	}
	return iface_type;
}

guint
mkt_statistic_replicates                      ( MktStatistic *statistic )
{
	g_return_val_if_fail(statistic != NULL , 0);
	g_return_val_if_fail(MKT_IS_STATISTIC(statistic) , 0);
	if(MKT_STATISTIC_GET_INTERFACE(statistic)->statistic_replicates )
		return MKT_STATISTIC_GET_INTERFACE(statistic)->statistic_replicates(statistic);
	return 0;
}

guint
mkt_statistic_outliers                        ( MktStatistic *statistic )
{
	g_return_val_if_fail(statistic != NULL , 0);
	g_return_val_if_fail(MKT_IS_STATISTIC(statistic) , 0);
	if(MKT_STATISTIC_GET_INTERFACE(statistic)->statistic_outliers )
		return MKT_STATISTIC_GET_INTERFACE(statistic)->statistic_outliers(statistic);
	return 0;
}

gdouble
mkt_statistic_max_cv                          ( MktStatistic *statistic )
{
	g_return_val_if_fail(statistic != NULL , 0.);
	g_return_val_if_fail(MKT_IS_STATISTIC(statistic) , 0.);
	if(MKT_STATISTIC_GET_INTERFACE(statistic)->statistic_max_cv )
		return MKT_STATISTIC_GET_INTERFACE(statistic)->statistic_max_cv(statistic);
	return 0.;
}

gdouble
mkt_statistic_threshold                       ( MktStatistic *statistic )
{
	g_return_val_if_fail(statistic != NULL , 0.);
	g_return_val_if_fail(MKT_IS_STATISTIC(statistic) , 0.);
	if(MKT_STATISTIC_GET_INTERFACE(statistic)->statistic_threshold )
		return MKT_STATISTIC_GET_INTERFACE(statistic)->statistic_threshold(statistic);
	return 0.;
}

/** @} */
