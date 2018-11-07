/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktAnalog
 * @{
 * @file  mkt-analog.c	Analog model interface
 * @brief This is Analog model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */

#include "market-time.h"
#include "mkt-analog-model.h"
#include "mkt-analog.h"

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

static void mkt_analog_base_init(gpointer g_iface) {
    static gboolean is_analog_initialized = FALSE;
    MUTEX_LOCK();
    if (!is_analog_initialized) {

        g_object_interface_install_property(g_iface, g_param_spec_string("analog-channel", "Measurement daily start actions property",
                                                                         "Set get sensor measurement property", "/com/lar/streams1/channels/TC",
                                                                         G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));

        g_object_interface_install_property(g_iface,
                                            g_param_spec_double("analog-min", "Measurement daily start actions property", "Set get sensor measurement property",
                                                                -G_MAXDOUBLE, G_MAXDOUBLE, 0., G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));

        g_object_interface_install_property(g_iface,
                                            g_param_spec_double("analog-max", "Measurement daily start actions property", "Set get sensor measurement property",
                                                                -G_MAXDOUBLE, G_MAXDOUBLE, 20000., G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));

        g_object_interface_install_property(g_iface, g_param_spec_string("analog-life-zero-par", _("Life zero string"), _("Life zero string"), "",
                                                                         G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));

        g_object_interface_install_property(g_iface, g_param_spec_boolean("analog-life-zero", _("Analog life zero"), _("Analog life zero"), FALSE,
                                                                          G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));

        g_object_interface_install_property(g_iface, g_param_spec_double("analog-limit", "Measurement daily start actions property",
                                                                         "Set get sensor measurement property", -G_MAXDOUBLE, G_MAXDOUBLE, 0.,
                                                                         G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));

        is_analog_initialized = TRUE;
    }
    MUTEX_UNLOCK();
}

GType mkt_analog_get_type(void) {
    static GType iface_type = 0;
    if (iface_type == 0) {
        static const GTypeInfo info = {sizeof(MktAnalogInterface),
                                       (GBaseInitFunc)mkt_analog_base_init,
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
            iface_type = g_type_register_static(G_TYPE_INTERFACE, "MktAnalogInterface", &info, 0);
            g_type_interface_add_prerequisite(iface_type, MKT_TYPE_MODEL);
        }
        MUTEX_UNLOCK();
    }
    return iface_type;
}

gdouble mkt_analog_min(MktAnalog *analog) {
    g_return_val_if_fail(analog != NULL, 0.);
    g_return_val_if_fail(MKT_IS_ANALOG(analog), 0.);
    if (MKT_ANALOG_GET_INTERFACE(analog)->analog_min) return MKT_ANALOG_GET_INTERFACE(analog)->analog_min(analog);
    return 0.;
}
gdouble mkt_analog_max(MktAnalog *analog) {
    g_return_val_if_fail(analog != NULL, 0.);
    g_return_val_if_fail(MKT_IS_ANALOG(analog), 0.);
    if (MKT_ANALOG_GET_INTERFACE(analog)->analog_max) return MKT_ANALOG_GET_INTERFACE(analog)->analog_max(analog);
    return 0.;
}

gdouble mkt_analog_transmit_value(MktAnalog *analog, gdouble value, gdouble min, gdouble max) {
    g_return_val_if_fail(analog != NULL, FALSE);
    g_return_val_if_fail(MKT_IS_ANALOG(analog), FALSE);
    gdouble out_min = mkt_analog_min(analog);
    gdouble out_max = mkt_analog_max(analog);

    gdouble xt    = out_min - out_max;
    gdouble xb    = min - max;
    gdouble slope = xb == 0.0 ? 1.0 : (xt / xb);

    gdouble yt        = out_max * min - out_min * max;
    gdouble yb        = min - max;
    gdouble intercept = yb == 0.00 ? 0.0 : yt / yb;
    // g_debug("Intercept = %f , slope = %f",intercept,slope);
    gdouble result = value * slope + intercept;
    // g_debug("TEST_ RESULT = %f",result);
    return result;
}

/** @} */
