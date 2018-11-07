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
#include "mkt-axis-model.h"

#include "../config.h"
#include <glib/gi18n-lib.h>

enum { PROP_0, PROP_AXIS_MAX, PROP_AXIS_HOLD, PROP_AXIS_CURRENT, PROP_AXIS_REVERCE };

struct _MktAxisModelPrivate {
    guint    max;
    guint    hold;
    guint    current;
    gboolean reverse;

    // Only nitri parameter
};

#define MKT_AXIS_MODEL_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), MKT_TYPE_AXIS_MODEL, MktAxisModelPrivate))

static void mkt_axis_model_init_stream_interface(MktAxisInterface *iface) {
}

G_DEFINE_TYPE_WITH_CODE(MktAxisModel, mkt_axis_model, MKT_TYPE_PARAM_OBJECT, G_IMPLEMENT_INTERFACE(MKT_TYPE_AXIS, mkt_axis_model_init_stream_interface)
                                                                                 MKT_CREATE_MODEL_FULL(mkt_connection_new_default_parameter(TRUE)))

static void mkt_axis_model_init(MktAxisModel *mkt_axis_model) {
    MktAxisModelPrivate *priv = MKT_AXIS_MODEL_PRIVATE(mkt_axis_model);
    mkt_axis_model->priv      = priv;
    /* TODO: Add initialization code here */
}

static void mkt_axis_model_finalize(GObject *object) {
    // MktAxisModel *data = MKT_AXIS_MODEL(object);
    G_OBJECT_CLASS(mkt_axis_model_parent_class)->finalize(object);
}

static void mkt_axis_model_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(MKT_IS_AXIS_MODEL(object));
    MktAxisModel *stream = MKT_AXIS_MODEL(object);
    // g_debug("mkt_axis_model_get_property %s",pspec->name);
    switch (prop_id) {
    case PROP_AXIS_MAX:
        stream->priv->max = g_value_get_uint(value);
        break;
    case PROP_AXIS_HOLD:
        stream->priv->hold = g_value_get_uint(value);
        break;
    case PROP_AXIS_CURRENT:
        stream->priv->current = g_value_get_uint(value);
        break;
    case PROP_AXIS_REVERCE:
        stream->priv->reverse = g_value_get_boolean(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void mkt_axis_model_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(MKT_IS_AXIS_MODEL(object));
    MktAxisModel *stream = MKT_AXIS_MODEL(object);
    switch (prop_id) {
    case PROP_AXIS_MAX:
        g_value_set_uint(value, stream->priv->max);
        break;
    case PROP_AXIS_HOLD:
        g_value_set_uint(value, stream->priv->hold);
        break;
    case PROP_AXIS_CURRENT:
        g_value_set_uint(value, stream->priv->current);
        break;
    case PROP_AXIS_REVERCE:
        g_value_set_boolean(value, stream->priv->reverse);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void mkt_axis_model_class_init(MktAxisModelClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // MktModelClass* parent_class = MKT_MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(MktAxisModelPrivate));
    object_class->finalize     = mkt_axis_model_finalize;
    object_class->set_property = mkt_axis_model_set_property;
    object_class->get_property = mkt_axis_model_get_property;

    g_object_class_override_property(object_class, PROP_AXIS_MAX, "axis-max");
    g_object_class_override_property(object_class, PROP_AXIS_HOLD, "axis-hold");
    g_object_class_override_property(object_class, PROP_AXIS_CURRENT, "axis-current");
    g_object_class_override_property(object_class, PROP_AXIS_REVERCE, "axis-reverse");
}
