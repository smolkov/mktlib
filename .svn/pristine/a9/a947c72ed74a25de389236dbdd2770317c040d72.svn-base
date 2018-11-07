/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mkt-data.c
 * Copyright (C) 2015 LAR
 *
 */

#include "mkt-data.h"
#include "market-time.h"

#if GLIB_CHECK_VERSION(2, 31, 7)
static GRecMutex init_rmutex;
#define MUTEX_LOCK() g_rec_mutex_lock(&init_rmutex)
#define MUTEX_UNLOCK() g_rec_mutex_unlock(&init_rmutex)
#else
static GStaticRecMutex init_mutex = G_STATIC_REC_MUTEX_INIT;
#define MUTEX_LOCK() g_static_rec_mutex_lock(&init_mutex)
#define MUTEX_UNLOCK() g_static_rec_mutex_unlock(&init_mutex)
#endif

static void mkt_data_base_init(gpointer g_iface) {
    static gboolean is_data_initialized = FALSE;
    MUTEX_LOCK();
    if (!is_data_initialized) {
        g_object_interface_install_property(
            g_iface, g_param_spec_uint64("data-creator", "Data data property", "Set get sensor data property", 0, G_MAXUINT64, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));
        g_object_interface_install_property(g_iface,
                                            g_param_spec_double("data-changed", "Data data property", "Set get sensor data property", 0., G_MAXDOUBLE, 0., G_PARAM_READWRITE | MKT_MODEL_DB_PROP));
        g_object_interface_install_property(g_iface,
                                            g_param_spec_double("data-value", "Data data property", "Set get sensor data property", 0., G_MAXDOUBLE, 0., G_PARAM_READWRITE | MKT_MODEL_DB_PROP));
        g_object_interface_install_property(
            g_iface, g_param_spec_int("data-type", "Data data property", "Set get sensor data property", 0, G_MAXINT32, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));
        g_object_interface_install_property(
            g_iface, g_param_spec_uint("data-trigger", "Data data trigger", "Set get sensor trigger property", 0, G_MAXUINT32, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

        is_data_initialized = TRUE;
    }
    MUTEX_UNLOCK();
}

GType mkt_data_get_type(void) {
    static GType iface_type = 0;
    if (iface_type == 0) {
        static const GTypeInfo info = {sizeof(MktDataInterface), (GBaseInitFunc)mkt_data_base_init, (GBaseFinalizeFunc)NULL, (GClassInitFunc)NULL, NULL, NULL, 0, 0, (GInstanceInitFunc)NULL, 0};
        MUTEX_LOCK();
        if (iface_type == 0) {
            iface_type = g_type_register_static(G_TYPE_INTERFACE, "MktDataInterface", &info, 0);
        }
        MUTEX_UNLOCK();
    }
    return iface_type;
}

gdouble mkt_data_changed(MktData *data) {
    g_return_val_if_fail(data != NULL, 0.0);
    g_return_val_if_fail(MKT_IS_DATA(data), 0.0);
    if (MKT_DATA_GET_INTERFACE(data)->data_changed) return MKT_DATA_GET_INTERFACE(data)->data_changed(data);
    return 0.0;
}

gdouble mkt_data_value(MktData *data) {
    g_return_val_if_fail(data != NULL, 0.0);
    g_return_val_if_fail(MKT_IS_DATA(data), 0.0);
    if (MKT_DATA_GET_INTERFACE(data)->data_value) return MKT_DATA_GET_INTERFACE(data)->data_value(data);
    return 0.0;
}

guint mkt_data_trigger(MktData *data) {
    g_return_val_if_fail(data != NULL, 0);
    g_return_val_if_fail(MKT_IS_DATA(data), 0);
    if (MKT_DATA_GET_INTERFACE(data)->data_trigger) return MKT_DATA_GET_INTERFACE(data)->data_trigger(data);
    return 0;
}

gint mkt_data_type(MktData *data) {
    g_return_val_if_fail(data != NULL, 0);
    g_return_val_if_fail(MKT_IS_DATA(data), 0);
    if (MKT_DATA_GET_INTERFACE(data)->data_type) return MKT_DATA_GET_INTERFACE(data)->data_type(data);
    return 0;
}





