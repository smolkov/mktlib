/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup UltimateMessparm
 * @{
 * @file  mkt-nitri_messparm.c	NitriMessparm model interface
 * @brief This is NitriMessparm model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */

#include "ultimate-messparm.h"
#include <math.h>

#define GETTEXT_PACKAGE "ultimate-library"
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

static void ultimate_messparm_base_init(gpointer g_iface) {
    static gboolean is_ultimate_messparm_initialized = FALSE;
    MUTEX_LOCK();
    if (!is_ultimate_messparm_initialized) {

        g_object_interface_install_property(g_iface, g_param_spec_uint("ultimate-sample-volume", "sample volume", "Set get sample volume", 0, 650, 100,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

        g_object_interface_install_property(g_iface, g_param_spec_uint("ultimate-injection-volume", "injection volume furnace", "Set get injection volume furnace", 0, 650, 100,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

        g_object_interface_install_property(g_iface, g_param_spec_uint("ultimate-injection-volume-tic", "injection volume TIC", "Set get injection volume TIC", 0, 650, 300,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

        g_object_interface_install_property(g_iface, g_param_spec_double("ultimate-sample-filling-time", "sample filling time", "Set|Get Sample filling time", 0.0, 20000.0, 1.0,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

        g_object_interface_install_property(g_iface, g_param_spec_double("ultimate-delay", "TC delay time", "Set|Get TC delay time", 0.0, 20000.0, 0.0,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

        g_object_interface_install_property(g_iface, g_param_spec_double("ultimate-delay-tic", "TIC delay time", "Set|Get TC delay time", 0.0, 20000.0, 50.0,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

        g_object_interface_install_property(g_iface, g_param_spec_boolean("ultimate-is-pre-rinsing", "need pre rinsing system ?", "Set need pre rinsing system", FALSE,
                                                         G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

        g_object_interface_install_property(g_iface, g_param_spec_uint("ultimate-rinsing-count", "rinsing count", "Set Rinsing count", 1, 20, 1,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

        g_object_interface_install_property(g_iface, g_param_spec_boolean("ultimate-is-after-rinsing", "need after rinsing system?", "Set need after rinsing system", FALSE,
                                                         G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

        g_object_interface_install_property(g_iface, g_param_spec_uint("ultimate-after-rinsing-count", "rinsing count", "Set Rinsing count", 1, 20, 1,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

        g_object_interface_install_property(
            g_iface, g_param_spec_boolean("ultimate-codo-injection", "codo injection system", "Set is codo injection system", FALSE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP));
        g_object_interface_install_property(g_iface,
            g_param_spec_boolean("ultimate-need-stripping", "stripping", "Set need pre rinsing system", FALSE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

        g_object_interface_install_property(g_iface, g_param_spec_double("ultimate-stripping-time", "stripping time", "Set|Get Stripping time", 0.0, 20000.0, 60.0,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

        g_object_interface_install_property(g_iface, g_param_spec_uint("ultimate-dilution-type", "is dilution", "Set|Get is dilution", 0, 2, 0,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

        g_object_interface_install_property(g_iface, g_param_spec_double("ultimate-dilution-factor", "dilution factor", "Set|Get Dilution factor", 1.0, 20.0, 5.0,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

        g_object_interface_install_property(g_iface, g_param_spec_double("ultimate-dilution-pump-time", "dilution pump time", "Set|Get dilution pump time", 3.0, 600.0, 10.0,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));
        g_object_interface_install_property(g_iface, g_param_spec_double("ultimate-dilution-wait-time", "dilution wait time", "Set|Get dilution wait time", 3.0, 2000.0, 10.0,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

        g_object_interface_install_property(g_iface, g_param_spec_double("ultimate-allowed-deviation", "allowed deviation", "Set|Get allowed deviation", 1.0, 100.0, 10.0,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));
        g_object_interface_install_property(g_iface, g_param_spec_double("ultimate-autocal-deviation", "auto calibration deviation", "Set|Get auto calibration deviation", 0.1, 100.0, 5.0,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));
        // Option Process rinsing ultra
        g_object_interface_install_property(g_iface,
            g_param_spec_boolean("process-rinsing", "extra rinsing option", "On|Off extra rinsing option", FALSE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));
        g_object_interface_install_property(g_iface, g_param_spec_uint("prinsing-y1-pos", "process rinsing Y1 position", "Set|Get process rinsing Y1 position", 600, 1720, 720,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));
        g_object_interface_install_property(g_iface, g_param_spec_uint("prinsing-injection-volume", "injection volume process rinsing", "Set get injection volume for process rinsing", 200, 600, 200,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

        g_object_interface_install_property(g_iface, g_param_spec_uint("prinsing-injection-replicate", "process rinsing injection replicates", "Set get injection replicates for process rinsing", 1, 5,
                                                         1, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));
        g_object_interface_install_property(g_iface, g_param_spec_double("prinsing-wait-time", "Wait time between injections", "Set|Get wait time between injections", 0.0, 20.0, 1.0,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

        g_object_interface_install_property(g_iface, g_param_spec_uint("prinsing-y2-pos", "process rinsing Y2 position", "Set|Get process rinsing Y2 position", 600, 1720, 1100,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));
        g_object_interface_install_property(g_iface, g_param_spec_double("prinsing-wait-after", "Wait time after rinsing process", "Set|Get wait time after rinsing process", 0.0, 180.0, 80.0,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

        is_ultimate_messparm_initialized = TRUE;
    }
    MUTEX_UNLOCK();
}

GType ultimate_messparm_get_type(void) {
    static GType iface_type = 0;
    if (iface_type == 0) {
        static const GTypeInfo info = {
            sizeof(UltimateMessparmInterface), (GBaseInitFunc)ultimate_messparm_base_init, (GBaseFinalizeFunc)NULL, (GClassInitFunc)NULL, NULL, NULL, 0, 0, (GInstanceInitFunc)NULL, 0};
        MUTEX_LOCK();
        if (iface_type == 0) {
            iface_type = g_type_register_static(G_TYPE_INTERFACE, "UltimateMessparmInterface", &info, 0);
        }
        MUTEX_UNLOCK();
    }
    return iface_type;
}

guint ultimate_messparm_volume(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, 0);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), 0);

    if (ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->volume) return ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->volume(umessparm);
    return 0;
}

gdouble ultimate_messparm_sample_filling_time(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, 0.);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), 0.);
    if (ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->sample_filling_time) return ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->sample_filling_time(umessparm);
    return 0.0;
}

gdouble ultimate_messparm_delay(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, 0.);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), 0.);
    if (ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->delay) return ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->delay(umessparm);
    return 0.0;
}

gboolean ultimate_messparm_pre_rinsing(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, FALSE);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), FALSE);
    if (ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->pre_rinsing) return ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->pre_rinsing(umessparm);
    return FALSE;
}

guint ultimate_messparm_rinsing_count(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, 0.0);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), 0.0);
    if (ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->rinsing_count) return ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->rinsing_count(umessparm);
    return 0;
}

gboolean ultimate_messparm_after_rinsing(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, FALSE);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), FALSE);
    if (ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->after_rinsing) return ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->after_rinsing(umessparm);
    return FALSE;
}

guint ultimate_messparm_after_rinsing_count(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, 0.0);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), 0.0);
    if (ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->after_rinsing_count) return ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->after_rinsing_count(umessparm);
    return 0;
}

gdouble ultimate_messparm_stripping_time(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, 0.0);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), 0.0);
    if (ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->stripping_time) return ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->stripping_time(umessparm);
    return 0.0;
}

gdouble ultimate_messparm_dilution_factor(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, 0.0);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), 0.0);
    if (ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->dilution_factor) return ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->dilution_factor(umessparm);
    return 0.0;
}

gdouble ultimate_messparm_dilution_pump_time(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, 0.0);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), 0.0);
    if (ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->dilution_pump_time) return ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->dilution_pump_time(umessparm);
    return 0.0;
}

gdouble ultimate_messparm_dilution_wait_time(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, 0.0);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), 0.0);
    if (ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->dilution_wait_time) return ULTIMATE_MESSPARM_GET_INTERFACE(umessparm)->dilution_wait_time(umessparm);
    return 0.0;
}





/** @} */
