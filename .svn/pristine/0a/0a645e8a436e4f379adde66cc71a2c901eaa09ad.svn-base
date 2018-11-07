/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktInjection
 * @{
 * @file  mkt-injection.c	Injection model interface
 * @brief This is Injection model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */

#include "mkt-injection.h"
#include "market-time.h"

#include "../config.h"
#include <glib/gi18n-lib.h>

#if GLIB_CHECK_VERSION(2, 31, 7)
static GRecMutex init_rmutex;
#define MUTEX_LOCK() g_rec_mutex_lock(&init_rmutex)
#define MUTEX_UNLOCK() g_rec_mutex_unlock(&init_rmutex)
#else
static GStaticRecMutex init_mutex = G_STATIC_REC_MUTEX_INIT;
#define MUTEX_LOCK() g_static_rec_mutex_lock(&init_mutex)
#define MUTEX_UNLOCK() g_static_rec_mutex_unlock(&init_mutex)
#endif

static void mkt_injection_base_init(gpointer g_iface) {
	static gboolean is_injection_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_injection_initialized) {
		g_object_interface_install_property(
			g_iface, g_param_spec_uint("injection-air",
        "Injection air position",
				"Set get air position parameter", 0, 150, 100,
				G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT |
				MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property(
			g_iface,
			g_param_spec_uint("injection-rest",
        "rest position",
				"Set get injection rest position parameter", 0, 200, 0,
				G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT |
				MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property(
			g_iface, g_param_spec_uint("injection-furnace-air",
        "Injection current",
				"Set get injection current parameter", 0, 150, 50,
				G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT |
				MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));
		g_object_interface_install_property(
			g_iface, g_param_spec_uint("injection-dilution",
        "injection dilution",
				"Set get injection dilution", 0, 600, 0,
				G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT |
				MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));
		g_object_interface_install_property(
			g_iface, g_param_spec_uint("injection-rinsing",
        "injection rinsing",
				"Set get injection rinsing parameter", 100, 601, 580,
				G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT |
				MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property(
			g_iface,
			g_param_spec_uint("injection-stepper-parameter",
        "injection stepper",
				"Set get injection stepper parameter", 1, 30, 3,
				G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT |
				MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property(
			g_iface,g_param_spec_uint("sample-stepper-parameter",
				"injection sampling stepper",
				"Set get injection sampling stepper parameter", 1, 30, 2,
				G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT |
				MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property(
			g_iface,g_param_spec_uint("rinsing-up-stepper-parameter",
        "injection up stepper",
				"Set get injection up stepper parameter", 1, 30, 2,
				G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT |
				MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

		g_object_interface_install_property(
			g_iface, g_param_spec_uint("rinsing-down-stepper-parameter",
				"injection down",
				"Set get injection down stepper parameter", 1, 30, 1,
				G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT |
				MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

		is_injection_initialized = TRUE;
	}
	MUTEX_UNLOCK();
}

GType mkt_injection_get_type(void) {
	static GType iface_type = 0;
	if (iface_type == 0) {
		static const GTypeInfo info = {sizeof(MktInjectionInterface),
			                       (GBaseInitFunc)mkt_injection_base_init,
			                       (GBaseFinalizeFunc)NULL,
			                       (GClassInitFunc)NULL,
			                       NULL,
			                       NULL,
			                       0,
			                       0,
			                       (GInstanceInitFunc)NULL,
			                       0};
		MUTEX_LOCK();
		if (iface_type == 0) {
			iface_type = g_type_register_static(G_TYPE_INTERFACE,
			                                    "MktInjectionInterface", &info, 0);
			g_type_interface_add_prerequisite(iface_type, MKT_TYPE_MODEL);
		}
		MUTEX_UNLOCK();
	}
	return iface_type;
}

/** @} */
