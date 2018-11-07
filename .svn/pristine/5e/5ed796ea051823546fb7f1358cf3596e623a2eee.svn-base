/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktAxis
 * @{
 * @file  mkt-axis.c	Axis model interface
 * @brief This is Axis model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */

#include "market-time.h"
#include "mkt-axis.h"

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

static void mkt_axis_base_init(gpointer g_iface) {
    static gboolean is_axis_initialized = FALSE;
    MUTEX_LOCK();
    if (!is_axis_initialized) {
        g_object_interface_install_property(g_iface,
                                            g_param_spec_uint("axis-max", _("Axis max position parameter"), _("Set get axis max position parameter"), 10, 20000,
                                                              1000, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));

        g_object_interface_install_property(g_iface,
                                            g_param_spec_uint("axis-hold", _("Axis hold position parameter"), _("Set get axis hold position parameter"), 10,
                                                              20000, 10, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));

        g_object_interface_install_property(g_iface,
                                            g_param_spec_uint("axis-current", _("Axis current parameter"), _("Set get axis current parameter"), 100, 16000, 100,
                                                              G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));

        g_object_interface_install_property(g_iface, g_param_spec_boolean("axis-reverse", _("Axis reverse parameter"), _("Set get reverse parameter"), FALSE,
                                                                          G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));

        is_axis_initialized = TRUE;
    }
    MUTEX_UNLOCK();
}

GType mkt_axis_get_type(void) {
    static GType iface_type = 0;
    if (iface_type == 0) {
        static const GTypeInfo info = {sizeof(MktAxisInterface),
                                       (GBaseInitFunc)mkt_axis_base_init,
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
            iface_type = g_type_register_static(G_TYPE_INTERFACE, "MktAxisInterface", &info, 0);
            g_type_interface_add_prerequisite(iface_type, MKT_TYPE_MODEL);
        }
        MUTEX_UNLOCK();
    }
    return iface_type;
}

/** @} */
