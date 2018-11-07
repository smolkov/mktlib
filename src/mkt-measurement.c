/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktMeasurement
 * @{
 * @file  mkt-measurement.c	Measurement model interface
 * @brief This is Measurement model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */

#include "mkt-measurement.h"
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

static void mkt_measurement_base_init(gpointer g_iface) {
  static gboolean is_measurement_initialized = FALSE;
  MUTEX_LOCK();
  if (!is_measurement_initialized) {
    g_object_interface_install_property(
        g_iface, g_param_spec_uint(
                     "measurement-identification", "Measurement property",
                     "Set get measurement property", 0, G_MAXUINT, 201,
                     G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

    g_object_interface_install_property(
        g_iface, g_param_spec_uint64(
                     "measurement-stream", "Measurement stream reference",
                     "Measurement stream reference", 0, G_MAXUINT64, 0,
                     G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

    g_object_interface_install_property(
        g_iface, g_param_spec_uint64(
                     "measurement-process", "Measurement process reference",
                     "Measurement process reference", 0, G_MAXUINT64, 0,
                     G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

    g_object_interface_install_property(
        g_iface, g_param_spec_uint64(
                     "measurement-channel", "Measurement channel reference",
                     "Measurement channel reference", 0, G_MAXUINT64, 0,
                     G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

    g_object_interface_install_property(
        g_iface, g_param_spec_int(
                     "measurement-type", "Measurement type",
                     "Set get measurement type property", 0, 255, 0,
                     G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

    g_object_interface_install_property(
        g_iface, g_param_spec_double(
                     "measurement-changed", "Measurement measurement property",
                     "Set get sensor measurement property", 0., G_MAXDOUBLE, 0.,
                     G_PARAM_READWRITE | MKT_MODEL_DB_PROP));

    g_object_interface_install_property(
        g_iface, g_param_spec_double(
                     "measurement-value", "Measurement measurement property",
                     "Set get sensor measurement property", 0., G_MAXDOUBLE, 0.,
                     G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

    g_object_interface_install_property(
        g_iface,
        g_param_spec_double(
            "measurement-value-row", "Measurement measurement property",
            "Set get sensor measurement property", 0., G_MAXDOUBLE, 0.,
            G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

    g_object_interface_install_property(
        g_iface, g_param_spec_double(
                     "measurement-cv", "Measurement measurement property",
                     "Set get sensor measurement property", 0., G_MAXDOUBLE, 0.,
                     G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

    g_object_interface_install_property(
        g_iface, g_param_spec_uint(
                     "measurement-trigger", "Measurement measurement trigger",
                     "Set get mesurement trigger property", 0, G_MAXINT32, 0,
                     G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

    g_object_interface_install_property(
        g_iface, g_param_spec_uint(
                     "measurement-number", "Measurement measurement trigger",
                     "Set get mesurement trigger property", 0, G_MAXINT32, 0,
                     G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

    g_object_interface_install_property(
        g_iface, g_param_spec_uint(
                     "measurement-signal", "Measurement measurement trigger",
                     "Set get sensor trigger property", 0, G_MAXINT32, 0,
                     G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

    g_object_interface_install_property(
        g_iface, g_param_spec_boolean(
                     "measurement-outlier", "Measurement measurement outlier",
                     "Set get sensor outlier property", FALSE,
                     G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

    g_object_interface_install_property(
        g_iface, g_param_spec_uint(
                     "measurement-replicate", "Measurement type",
                     "Set get measurement type property", 0, 255, 0,
                     G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));
    g_object_interface_install_property(
        g_iface, g_param_spec_string("measurement-name", "Measurement name",
                                     "Measurement name", "noname",
                                     G_PARAM_READABLE | G_PARAM_WRITABLE |
                                         MKT_MODEL_DB_PROP));
    g_object_interface_install_property(
        g_iface, g_param_spec_string(
                     "measurement-unit", "Measurement measurement outlier",
                     "Set get sensor outlier property", "mg/l",
                     G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

    is_measurement_initialized = TRUE;
  }
  MUTEX_UNLOCK();
}

GType mkt_measurement_get_type(void) {
  static GType iface_type = 0;
  if (iface_type == 0) {
    static const GTypeInfo info = {sizeof(MktMeasurementInterface),
                                   (GBaseInitFunc)mkt_measurement_base_init,
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
                                          "MktMeasurementInterface", &info, 0);
    }
    MUTEX_UNLOCK();
  }
  return iface_type;
}

// const gchar *mkt_measurement_id_name(MktMeasurement *measurement) {
//   g_return_val_if_fail(measurement != NULL, NULL);
//   g_return_val_if_fail(MKT_IS_MEASUREMENT(measurement), NULL);
//   if (MKT_MEASUREMENT_GET_INTERFACE(measurement)->measurement_name)
//     return MKT_MEASUREMENT_GET_INTERFACE(measurement)
//         ->measurement_name(measurement);
//   return NULL;
// }

const gchar *mkt_measurement_name(MktMeasurement *measurement) {
  g_return_val_if_fail(measurement != NULL, NULL);
  g_return_val_if_fail(MKT_IS_MEASUREMENT(measurement), NULL);
  if (MKT_MEASUREMENT_GET_INTERFACE(measurement)->measurement_name)
    return MKT_MEASUREMENT_GET_INTERFACE(measurement)
        ->measurement_name(measurement);
  return NULL;
}

const gchar *mkt_measurement_unit(MktMeasurement *measurement) {
  g_return_val_if_fail(measurement != NULL, NULL);
  g_return_val_if_fail(MKT_IS_MEASUREMENT(measurement), NULL);
  if (MKT_MEASUREMENT_GET_INTERFACE(measurement)->measurement_unit)
    return MKT_MEASUREMENT_GET_INTERFACE(measurement)
        ->measurement_unit(measurement);
  return NULL;
}

guint mkt_measurement_identification(MktMeasurement *measurement) {
  g_return_val_if_fail(measurement != NULL, 0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT(measurement), 0);
  if (MKT_MEASUREMENT_GET_INTERFACE(measurement)->measurement_identification)
    return MKT_MEASUREMENT_GET_INTERFACE(measurement)
        ->measurement_identification(measurement);
  return 0;
}

gdouble mkt_measurement_changed(MktMeasurement *measurement) {
  g_return_val_if_fail(measurement != NULL, 0.0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT(measurement), 0.0);
  if (MKT_MEASUREMENT_GET_INTERFACE(measurement)->measurement_changed)
    return MKT_MEASUREMENT_GET_INTERFACE(measurement)
        ->measurement_changed(measurement);
  return 0.0;
}

gdouble mkt_measurement_value(MktMeasurement *measurement) {
  g_return_val_if_fail(measurement != NULL, 0.0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT(measurement), 0.0);
  if (MKT_MEASUREMENT_GET_INTERFACE(measurement)->measurement_value)
    return MKT_MEASUREMENT_GET_INTERFACE(measurement)
        ->measurement_value(measurement);
  return 0.0;
}

gdouble mkt_measurement_value_row(MktMeasurement *measurement) {
  g_return_val_if_fail(measurement != NULL, 0.0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT(measurement), 0.0);
  if (MKT_MEASUREMENT_GET_INTERFACE(measurement)->measurement_value_row)
    return MKT_MEASUREMENT_GET_INTERFACE(measurement)
        ->measurement_value_row(measurement);
  return 0.0;
}

gdouble mkt_measurement_cv(MktMeasurement *measurement) {
  g_return_val_if_fail(measurement != NULL, 0.0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT(measurement), 0.0);
  if (MKT_MEASUREMENT_GET_INTERFACE(measurement)->measurement_cv)
    return MKT_MEASUREMENT_GET_INTERFACE(measurement)
        ->measurement_cv(measurement);
  return 0.0;
}

guint mkt_measurement_trigger(MktMeasurement *measurement) {
  g_return_val_if_fail(measurement != NULL, 0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT(measurement), 0);
  if (MKT_MEASUREMENT_GET_INTERFACE(measurement)->measurement_trigger)
    return MKT_MEASUREMENT_GET_INTERFACE(measurement)
        ->measurement_trigger(measurement);
  return 0;
}

guint mkt_measurement_signal(MktMeasurement *measurement) {
  g_return_val_if_fail(measurement != NULL, 0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT(measurement), 0);
  if (MKT_MEASUREMENT_GET_INTERFACE(measurement)->measurement_signal)
    return MKT_MEASUREMENT_GET_INTERFACE(measurement)
        ->measurement_signal(measurement);
  return 0;
}

guint64 mkt_measurement_channel(MktMeasurement *measurement) {
  g_return_val_if_fail(measurement != NULL, 0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT(measurement), 0);
  if (MKT_MEASUREMENT_GET_INTERFACE(measurement)->measurement_channel)
    return MKT_MEASUREMENT_GET_INTERFACE(measurement)
        ->measurement_channel(measurement);
  return 0;
}

guint64 mkt_measurement_stream(MktMeasurement *measurement) {
  g_return_val_if_fail(measurement != NULL, 0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT(measurement), 0);
  if (MKT_MEASUREMENT_GET_INTERFACE(measurement)->measurement_stream)
    return MKT_MEASUREMENT_GET_INTERFACE(measurement)
        ->measurement_stream(measurement);
  return 0;
}

guint64 mkt_measurement_process(MktMeasurement *measurement) {
  g_return_val_if_fail(measurement != NULL, 0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT(measurement), 0);
  if (MKT_MEASUREMENT_GET_INTERFACE(measurement)->measurement_process)
    return MKT_MEASUREMENT_GET_INTERFACE(measurement)
        ->measurement_process(measurement);
  return 0;
}

gint mkt_measurement_type(MktMeasurement *measurement) {
  g_return_val_if_fail(measurement != NULL, 0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT(measurement), 0);
  if (MKT_MEASUREMENT_GET_INTERFACE(measurement)->measurement_type)
    return MKT_MEASUREMENT_GET_INTERFACE(measurement)
        ->measurement_type(measurement);
  return 0;
}
guint mkt_measurement_replicate(MktMeasurement *measurement) {
  g_return_val_if_fail(measurement != NULL, 0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT(measurement), 0);
  if (MKT_MEASUREMENT_GET_INTERFACE(measurement)->measurement_replicate)
    return MKT_MEASUREMENT_GET_INTERFACE(measurement)
        ->measurement_replicate(measurement);
  return 0;
}

gboolean mkt_measurement_outlier(MktMeasurement *measurement) {
  g_return_val_if_fail(measurement != NULL, FALSE);
  g_return_val_if_fail(MKT_IS_MEASUREMENT(measurement), FALSE);
  if (MKT_MEASUREMENT_GET_INTERFACE(measurement)->measurement_outlier)
    return MKT_MEASUREMENT_GET_INTERFACE(measurement)
        ->measurement_outlier(measurement);
  return 0;
}
