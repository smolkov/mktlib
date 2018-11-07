/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mkt-measurement-data.c
 * Copyright (C) sascha 2013 <sascha@sascha-VirtualBox>
 *
mkt-measurement-data.c is free software: you can redistribute it and/or modify
it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mkt-measurement-data.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mkt-measurement-data.h"
#include "market-time.h"
#include "mkt-connection.h"

enum {
  PROP_0,
  PROP_NUMBER,
  PROP_IDENTIFICATION,
  PROP_CHANGED,
  PROP_VALUE,
  PROP_VALUE_ROW,
  PROP_CV,
  PROP_MEASUREMENT_NUMBER,
  PROP_TRIGGER,
  PROP_SIGNAL,
  PROP_TIME,
  PROP_CHANNEL,
  PROP_STREAM,
  PROP_PROCESS,
  PROP_TYPE,
  PROP_REPLICATE,
  PROP_OUTLIER,
  PROP_LIMIT,
  PROP_ERROR,
  PROP_NAME,
  PROP_UNIT,
};

struct _MktMeasurementDataPrivate {
  guint identification;
  gdouble changed;
  gdouble value;
  gdouble value_row;
  gdouble cv;
  guint number;
  guint trigger;
  guint signal;
  guint64 channel;
  guint64 stream;
  guint64 process;
  gint type;
  guint replicate;
  gboolean outlier;
  gchar *name;
  gchar *unit;
};

#define MKT_MEASUREMENT_DATA_PRIVATE(o)                                        \
  (G_TYPE_INSTANCE_GET_PRIVATE((o), MKT_TYPE_MEASUREMENT_DATA,                 \
                               MktMeasurementDataPrivate))

static guint mkt_measurement_data_identification(MktMeasurement *data) {
  g_return_val_if_fail(data != NULL, 0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT_DATA(data), 0);
  return MKT_MEASUREMENT_DATA(data)->priv->identification;
}

static gdouble mkt_measurement_data_changed(MktMeasurement *data) {
  g_return_val_if_fail(data != NULL, 0.0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT_DATA(data), 0.0);
  return MKT_MEASUREMENT_DATA(data)->priv->changed;
}

static gdouble mkt_measurement_data_value(MktMeasurement *data) {
  g_return_val_if_fail(data != NULL, 0.0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT_DATA(data), 0.0);
  return MKT_MEASUREMENT_DATA(data)->priv->value;
}

static gdouble mkt_measurement_data_value_row(MktMeasurement *data) {
  g_return_val_if_fail(data != NULL, 0.0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT_DATA(data), 0.0);
  return MKT_MEASUREMENT_DATA(data)->priv->value_row;
}
static gdouble mkt_measurement_data_cv(MktMeasurement *data) {
  g_return_val_if_fail(data != NULL, 0.0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT_DATA(data), 0.0);
  return MKT_MEASUREMENT_DATA(data)->priv->cv;
}

static guint mkt_measurement_data_trigger(MktMeasurement *data) {
  g_return_val_if_fail(data != NULL, 0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT_DATA(data), 0);
  return MKT_MEASUREMENT_DATA(data)->priv->trigger;
}

static guint mkt_measurement_data_replicate(MktMeasurement *data) {
  g_return_val_if_fail(data != NULL, 0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT_DATA(data), 0);
  return MKT_MEASUREMENT_DATA(data)->priv->replicate;
}
static guint mkt_measurement_data_signal(MktMeasurement *data) {
  g_return_val_if_fail(data != NULL, 0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT_DATA(data), 0);
  return MKT_MEASUREMENT_DATA(data)->priv->signal;
}
static guint64 mkt_measurement_data_channel(MktMeasurement *data) {
  g_return_val_if_fail(data != NULL, 0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT_DATA(data), 0);
  return MKT_MEASUREMENT_DATA(data)->priv->channel;
}

static guint64 mkt_measurement_data_stream(MktMeasurement *data) {
  g_return_val_if_fail(data != NULL, 0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT_DATA(data), 0);
  return MKT_MEASUREMENT_DATA(data)->priv->stream;
}
static guint64 mkt_measurement_data_process(MktMeasurement *data) {
  g_return_val_if_fail(data != NULL, 0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT_DATA(data), 0);
  return MKT_MEASUREMENT_DATA(data)->priv->process;
}

static gint mkt_measurement_data_type(MktMeasurement *data) {
  g_return_val_if_fail(data != NULL, 0);
  g_return_val_if_fail(MKT_IS_MEASUREMENT_DATA(data), 0);
  return MKT_MEASUREMENT_DATA(data)->priv->type;
}

static gboolean mkt_measurement_data_outlier(MktMeasurement *data) {
  g_return_val_if_fail(data != NULL, FALSE);
  g_return_val_if_fail(MKT_IS_MEASUREMENT_DATA(data), FALSE);
  return MKT_MEASUREMENT_DATA(data)->priv->outlier;
}
static const gchar *mkt_measurement_data_name(MktMeasurement *data) {
  g_return_val_if_fail(data != NULL, NULL);
  g_return_val_if_fail(MKT_IS_MEASUREMENT_DATA(data), NULL);
  return MKT_MEASUREMENT_DATA(data)->priv->name;
}
static const gchar *mkt_measurement_data_unit(MktMeasurement *data) {
  g_return_val_if_fail(data != NULL, NULL);
  g_return_val_if_fail(MKT_IS_MEASUREMENT_DATA(data), NULL);
  return MKT_MEASUREMENT_DATA(data)->priv->unit;
}

static void
mkt_measurement_data_init_interface(MktMeasurementInterface *iface) {
  iface->measurement_identification = mkt_measurement_data_identification;
  iface->measurement_changed = mkt_measurement_data_changed;
  iface->measurement_value = mkt_measurement_data_value;
  iface->measurement_value_row = mkt_measurement_data_value_row;

  iface->measurement_cv = mkt_measurement_data_cv;
  iface->measurement_trigger = mkt_measurement_data_trigger;
  iface->measurement_signal = mkt_measurement_data_signal;
  iface->measurement_channel = mkt_measurement_data_channel;
  iface->measurement_stream = mkt_measurement_data_stream;
  iface->measurement_process = mkt_measurement_data_process;
  iface->measurement_type = mkt_measurement_data_type;
  iface->measurement_outlier = mkt_measurement_data_outlier;
  iface->measurement_replicate = mkt_measurement_data_replicate;

  iface->measurement_name = mkt_measurement_data_name;
  iface->measurement_unit = mkt_measurement_data_unit;
}

G_DEFINE_TYPE_WITH_CODE(
    MktMeasurementData, mkt_measurement_data, MKT_TYPE_MODEL,
    G_IMPLEMENT_INTERFACE(MKT_TYPE_MEASUREMENT,
                          mkt_measurement_data_init_interface)
        MKT_CREATE_MODEL_FULL(mkt_connection_new_default_measurement(TRUE)))

static void
mkt_measurement_data_init(MktMeasurementData *mkt_measurement_data) {
  MktMeasurementDataPrivate *priv =
      MKT_MEASUREMENT_DATA_PRIVATE(mkt_measurement_data);
  priv->identification = 0;
  priv->changed = market_db_time_now();
  priv->value = 0.0;
  priv->cv = 0.0;
  priv->number = 0;
  priv->trigger = 0;
  priv->signal = 0;
  priv->channel = 0;
  priv->stream = 1;
  priv->process = 1;

  priv->type = 0;
  priv->outlier = FALSE;
  priv->replicate = 1;
  priv->name = g_strdup("noname");
  priv->unit = g_strdup("mg/l");
  mkt_measurement_data->priv = priv;

  /* TODO: Add initialization code here */
}

static void mkt_measurement_data_finalize(GObject *object) {
  MktMeasurementData *data = MKT_MEASUREMENT_DATA(object);
  if (data->priv->name)
    g_free(data->priv->name);
  if (data->priv->unit)
    g_free(data->priv->unit);

  G_OBJECT_CLASS(mkt_measurement_data_parent_class)->finalize(object);
}

static void mkt_measurement_data_set_property(GObject *object, guint prop_id,
                                              const GValue *value,
                                              GParamSpec *pspec) {
  g_return_if_fail(MKT_IS_MEASUREMENT_DATA(object));
  MktMeasurementData *data = MKT_MEASUREMENT_DATA(object);
  switch (prop_id) {
  case PROP_IDENTIFICATION:
    data->priv->identification = g_value_get_uint(value);
    break;
  case PROP_CHANGED:
    data->priv->changed = g_value_get_double(value);
    break;
  case PROP_VALUE:
    data->priv->value = g_value_get_double(value);
    break;
  case PROP_VALUE_ROW:
    data->priv->value_row = g_value_get_double(value);
    break;
  case PROP_CV:
    data->priv->cv = g_value_get_double(value);
    break;
  case PROP_MEASUREMENT_NUMBER:
    data->priv->number = g_value_get_uint(value);
    break;
  case PROP_TRIGGER:
    data->priv->trigger = g_value_get_uint(value);
    break;
  case PROP_SIGNAL:
    data->priv->signal = g_value_get_uint(value);
    break;
  case PROP_CHANNEL:
    data->priv->channel = g_value_get_uint64(value);
    break;
  case PROP_STREAM:
    data->priv->stream = g_value_get_uint64(value);
    break;
  case PROP_PROCESS:
    data->priv->process = g_value_get_uint64(value);
    break;
  case PROP_TYPE:
    data->priv->type = g_value_get_int(value);
    break;
  case PROP_OUTLIER:
    data->priv->outlier = g_value_get_boolean(value);
    break;
  case PROP_REPLICATE:
    data->priv->replicate = g_value_get_uint(value);
    break;
  case PROP_NAME:
    if (data->priv->name)
      g_free(data->priv->name);
    data->priv->name = g_value_dup_string(value);
    break;
  case PROP_UNIT:
    if (data->priv->unit)
      g_free(data->priv->unit);
    data->priv->unit = g_value_dup_string(value);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void mkt_measurement_data_get_property(GObject *object, guint prop_id,
                                              GValue *value,
                                              GParamSpec *pspec) {
  g_return_if_fail(MKT_IS_MEASUREMENT_DATA(object));
  MktMeasurementData *data = MKT_MEASUREMENT_DATA(object);
  switch (prop_id) {
  case PROP_IDENTIFICATION:
    g_value_set_uint(value, data->priv->identification);
    break;
  case PROP_CHANGED:
    g_value_set_double(value, data->priv->changed);
    break;
  case PROP_VALUE:
    g_value_set_double(value, data->priv->value);
    break;
  case PROP_VALUE_ROW:
    g_value_set_double(value, data->priv->value_row);
    break;
  case PROP_CV:
    g_value_set_double(value, data->priv->cv);
    break;
  case PROP_MEASUREMENT_NUMBER:
    g_value_set_uint(value, data->priv->number);
    break;
  case PROP_TRIGGER:
    g_value_set_uint(value, data->priv->trigger);
    break;
  case PROP_SIGNAL:
    g_value_set_uint(value, data->priv->signal);
    break;
  case PROP_CHANNEL:
    g_value_set_uint64(value, data->priv->channel);
    break;
  case PROP_PROCESS:
    g_value_set_uint64(value, data->priv->process);
    break;
  case PROP_STREAM:
    g_value_set_uint64(value, data->priv->stream);
    break;
  case PROP_TYPE:
    g_value_set_int(value, data->priv->type);
    break;
  case PROP_OUTLIER:
    g_value_set_boolean(value, data->priv->outlier);
    break;
  case PROP_REPLICATE:
    g_value_set_uint(value, data->priv->replicate);
    break;
  case PROP_NAME:
    g_value_set_string(value, data->priv->name);
    break;
  case PROP_UNIT:
    g_value_set_string(value, data->priv->unit);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void mkt_measurement_data_class_init(MktMeasurementDataClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  // MktModelClass* parent_class = MKT_MODEL_CLASS (klass);
  g_type_class_add_private(klass, sizeof(MktMeasurementDataPrivate));
  object_class->finalize = mkt_measurement_data_finalize;
  object_class->set_property = mkt_measurement_data_set_property;
  object_class->get_property = mkt_measurement_data_get_property;

  g_object_class_override_property(object_class, PROP_IDENTIFICATION,
                                   "measurement-identification");
  g_object_class_override_property(object_class, PROP_CHANGED,
                                   "measurement-changed");
  g_object_class_override_property(object_class, PROP_CHANNEL,
                                   "measurement-channel");
  g_object_class_override_property(object_class, PROP_STREAM,
                                   "measurement-stream");
  g_object_class_override_property(object_class, PROP_PROCESS,
                                   "measurement-process");
  g_object_class_override_property(object_class, PROP_TYPE, "measurement-type");
  g_object_class_override_property(object_class, PROP_VALUE,
                                   "measurement-value");
  g_object_class_override_property(object_class, PROP_VALUE_ROW,
                                   "measurement-value-row");
  g_object_class_override_property(object_class, PROP_CV, "measurement-cv");
  g_object_class_override_property(object_class, PROP_MEASUREMENT_NUMBER,
                                   "measurement-number");
  g_object_class_override_property(object_class, PROP_TRIGGER,
                                   "measurement-trigger");
  g_object_class_override_property(object_class, PROP_SIGNAL,
                                   "measurement-signal");
  g_object_class_override_property(object_class, PROP_OUTLIER,
                                   "measurement-outlier");
  g_object_class_override_property(object_class, PROP_REPLICATE,
                                   "measurement-replicate");
  g_object_class_override_property(object_class, PROP_NAME, "measurement-name");
  g_object_class_override_property(object_class, PROP_UNIT, "measurement-unit");
}
