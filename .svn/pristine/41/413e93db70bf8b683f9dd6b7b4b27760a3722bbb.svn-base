/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mkt-sensor-data.c
 * Copyright (C) sascha 2013 <sascha@sascha-VirtualBox>
 *
mkt-sensor-data.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mkt-sensor-data.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "market-time.h"
#include "mkt-analog-model.h"

enum { PROP_0, PROP_ANALOG_CHANNEL, PROP_ANALOG_MIN, PROP_ANALOG_MAX, PROP_ANALOG_LIMIT, PROP_ANALOG_LIFE_ZERO, PROP_ANALOG_LIFE_ZERO_PAR };

struct _MktAnalogModelPrivate {
    gchar *  channel;
    gdouble  min;
    gdouble  max;
    gdouble  limit;
    gboolean life_zero;
    gchar *  life_zero_par;
};

#define MKT_ANALOG_MODEL_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), MKT_TYPE_ANALOG_MODEL, MktAnalogModelPrivate))

static gdouble mkt_analog_model_min(MktAnalog *analog) {
    g_return_val_if_fail(analog != NULL, 0.);
    g_return_val_if_fail(MKT_IS_ANALOG_MODEL(analog), 0.);
    return MKT_ANALOG_MODEL(analog)->priv->min;
}

static gdouble mkt_analog_model_max(MktAnalog *analog) {
    g_return_val_if_fail(analog != NULL, 0.);
    g_return_val_if_fail(MKT_IS_ANALOG_MODEL(analog), 0.);

    return MKT_ANALOG_MODEL(analog)->priv->max;
}

static gdouble mkt_analog_model_limit(MktAnalog *analog) {
    g_return_val_if_fail(analog != NULL, 0.);
    g_return_val_if_fail(MKT_IS_ANALOG_MODEL(analog), 0.);
    return MKT_ANALOG_MODEL(analog)->priv->limit;
}

static void mkt_analog_model_init_interface(MktAnalogInterface *iface) {
    iface->analog_min   = mkt_analog_model_min;
    iface->analog_max   = mkt_analog_model_max;
    iface->analog_limit = mkt_analog_model_limit;
}

G_DEFINE_TYPE_WITH_CODE(MktAnalogModel, mkt_analog_model, MKT_TYPE_PARAM_OBJECT,
    G_IMPLEMENT_INTERFACE(MKT_TYPE_ANALOG, mkt_analog_model_init_interface) MKT_CREATE_MODEL_FULL(mkt_connection_new_default_parameter(TRUE)))

static void mkt_analog_model_init(MktAnalogModel *mkt_analog_model) {
    MktAnalogModelPrivate *priv           = MKT_ANALOG_MODEL_PRIVATE(mkt_analog_model);
    mkt_analog_model->priv                = priv;
    mkt_analog_model->priv->channel       = g_strdup("/com/lar/streams1/channels/TC");
    mkt_analog_model->priv->life_zero_par = g_strdup("--");
    /* TODO: Add initialization code here */
}

static void mkt_analog_model_finalize(GObject *object) {
    MktAnalogModel *data = MKT_ANALOG_MODEL(object);
    if (data->priv->channel) g_free(data->priv->channel);
    if (data->priv->life_zero_par) g_free(data->priv->life_zero_par);
    G_OBJECT_CLASS(mkt_analog_model_parent_class)->finalize(object);
}

static void mkt_analog_model_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(MKT_IS_ANALOG_MODEL(object));
    MktAnalogModel *analog = MKT_ANALOG_MODEL(object);
    switch (prop_id) {
    case PROP_ANALOG_CHANNEL:
        if (analog->priv->channel) g_free(analog->priv->channel);
        analog->priv->channel = g_value_dup_string(value);
        break;
    case PROP_ANALOG_MIN:
        analog->priv->min = g_value_get_double(value);
        break;
    case PROP_ANALOG_MAX:
        analog->priv->max = g_value_get_double(value);
        break;
    case PROP_ANALOG_LIMIT:
        analog->priv->limit = g_value_get_double(value);
        break;
    case PROP_ANALOG_LIFE_ZERO:
        analog->priv->life_zero = g_value_get_boolean(value);
        break;
    case PROP_ANALOG_LIFE_ZERO_PAR:
        if (analog->priv->life_zero_par) g_free(analog->priv->life_zero_par);
        analog->priv->life_zero_par = g_value_dup_string(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void mkt_analog_model_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(MKT_IS_ANALOG_MODEL(object));
    MktAnalogModel *stream = MKT_ANALOG_MODEL(object);
    switch (prop_id) {
    case PROP_ANALOG_CHANNEL:
        g_value_set_string(value, stream->priv->channel);
        break;
    case PROP_ANALOG_MIN:
        g_value_set_double(value, stream->priv->min);
        break;
    case PROP_ANALOG_MAX:
        g_value_set_double(value, stream->priv->max);
        break;
    case PROP_ANALOG_LIMIT:
        g_value_set_double(value, stream->priv->limit);
        break;
    case PROP_ANALOG_LIFE_ZERO:
        g_value_set_boolean(value, stream->priv->life_zero);
        break;
    case PROP_ANALOG_LIFE_ZERO_PAR:
        g_value_set_string(value, stream->priv->life_zero_par);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void mkt_analog_model_class_init(MktAnalogModelClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // MktModelClass* parent_class = MKT_MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(MktAnalogModelPrivate));
    object_class->finalize     = mkt_analog_model_finalize;
    object_class->set_property = mkt_analog_model_set_property;
    object_class->get_property = mkt_analog_model_get_property;

    g_object_class_override_property(object_class, PROP_ANALOG_CHANNEL, "analog-channel");
    g_object_class_override_property(object_class, PROP_ANALOG_MIN, "analog-min");
    g_object_class_override_property(object_class, PROP_ANALOG_MAX, "analog-max");
    g_object_class_override_property(object_class, PROP_ANALOG_LIMIT, "analog-limit");
    g_object_class_override_property(object_class, PROP_ANALOG_LIFE_ZERO, "analog-life-zero");
    g_object_class_override_property(object_class, PROP_ANALOG_LIFE_ZERO_PAR, "analog-life-zero-par");
}
