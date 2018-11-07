/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * lar-sensor-data.c
 * Copyright (C) sascha 2013 <sascha@sascha-VirtualBox>
 *
lar-sensor-data.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lar-sensor-data.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "lar-statistic-model.h"

enum {
    PROP_0,
    PROP_STATISTIC_REPLICATES,
    PROP_STATISTIC_OUTLIERS,
    PROP_STATISTIC_MAX_CV,
    PROP_STATISTIC_THRESHOLD,
    PROP_STATISTIC_JUMP,
    PROP_STATISTIC_AMOUNT_COUNTER,
    PROP_STATISTIC_AMOUNT_PERCENTAGE,
};

struct _LarStatisticModelPrivate {
    guint   replicates;
    guint   outlier;
    gdouble max_cv;
    gdouble threshold;
    gdouble jump;
    guint   amount_counter;
    gdouble amount_percentage;
    // Only nitri parameter
};

#define LAR_STATISTIC_MODEL_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), LAR_TYPE_STATISTIC_MODEL, LarStatisticModelPrivate))

static guint lar_stream_object_replicates(LarStatistic *statistic) {
    g_return_val_if_fail(statistic != NULL, 0);
    g_return_val_if_fail(LAR_IS_STATISTIC_MODEL(statistic), 0);
    return LAR_STATISTIC_MODEL(statistic)->priv->replicates;
}

static guint lar_stream_object_outliers(LarStatistic *statistic) {
    g_return_val_if_fail(statistic != NULL, 0);
    g_return_val_if_fail(LAR_IS_STATISTIC_MODEL(statistic), 0);
    return LAR_STATISTIC_MODEL(statistic)->priv->outlier;
}
static gdouble lar_stream_object_max_cv(LarStatistic *statistic) {
    g_return_val_if_fail(statistic != NULL, 0.);
    g_return_val_if_fail(LAR_IS_STATISTIC_MODEL(statistic), 0.);
    return LAR_STATISTIC_MODEL(statistic)->priv->max_cv;
}
static gdouble lar_stream_object_threshold(LarStatistic *statistic) {
    g_return_val_if_fail(statistic != NULL, 0.);
    g_return_val_if_fail(LAR_IS_STATISTIC_MODEL(statistic), 0.);
    return LAR_STATISTIC_MODEL(statistic)->priv->threshold;
}

static void lar_strean_object_init_statistic_interface(LarStatisticInterface *iface) {
    iface->statistic_replicates = lar_stream_object_replicates;
    iface->statistic_outliers   = lar_stream_object_outliers;
    iface->statistic_max_cv     = lar_stream_object_max_cv;
    iface->statistic_threshold  = lar_stream_object_threshold;
}

G_DEFINE_TYPE_WITH_CODE(LarStatisticModel, lar_statistic_model, LAR_TYPE_PARAM_OBJECT,
    G_IMPLEMENT_INTERFACE(LAR_TYPE_STATISTIC, lar_strean_object_init_statistic_interface) LAR_CREATE_MODEL_FULL(lar_connection_new_default_parameter(TRUE)))

static void lar_statistic_model_init(LarStatisticModel *lar_statistic_model) {
    LarStatisticModelPrivate *priv = LAR_STATISTIC_MODEL_PRIVATE(lar_statistic_model);
    lar_statistic_model->priv      = priv;
    /* TODO: Add initialization code here */
}

static void lar_statistic_model_finalize(GObject *object) {
    // LarStatisticModel *statistic = LAR_STATISTIC_MODEL(object);
    G_OBJECT_CLASS(lar_statistic_model_parent_class)->finalize(object);
}

static void lar_statistic_model_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(LAR_IS_STATISTIC_MODEL(object));
    LarStatisticModel *stream = LAR_STATISTIC_MODEL(object);
    switch (prop_id) {
    case PROP_STATISTIC_REPLICATES:
        stream->priv->replicates = g_value_get_uint(value);
        break;
    case PROP_STATISTIC_OUTLIERS:
        stream->priv->outlier = g_value_get_uint(value);
        break;
    case PROP_STATISTIC_MAX_CV:
        stream->priv->max_cv = g_value_get_double(value);
        break;
    case PROP_STATISTIC_THRESHOLD:
        stream->priv->threshold = g_value_get_double(value);
        break;
    case PROP_STATISTIC_JUMP:
        stream->priv->jump = g_value_get_double(value);
        break;
    case PROP_STATISTIC_AMOUNT_COUNTER:
        stream->priv->amount_counter = g_value_get_uint(value);
        break;
    case PROP_STATISTIC_AMOUNT_PERCENTAGE:
        stream->priv->amount_percentage = g_value_get_double(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_statistic_model_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(LAR_IS_STATISTIC_MODEL(object));
    LarStatisticModel *stream = LAR_STATISTIC_MODEL(object);
    switch (prop_id) {
    case PROP_STATISTIC_REPLICATES:
        g_value_set_uint(value, stream->priv->replicates);
        break;
    case PROP_STATISTIC_OUTLIERS:
        g_value_set_uint(value, stream->priv->outlier);
        break;
    case PROP_STATISTIC_MAX_CV:
        g_value_set_double(value, stream->priv->max_cv);
        break;
    case PROP_STATISTIC_THRESHOLD:
        g_value_set_double(value, stream->priv->threshold);
        break;
    case PROP_STATISTIC_JUMP:
        g_value_set_double(value, stream->priv->threshold);
        break;
    case PROP_STATISTIC_AMOUNT_COUNTER:
        g_value_set_uint(value, stream->priv->amount_counter);
        break;
    case PROP_STATISTIC_AMOUNT_PERCENTAGE:
        g_value_set_double(value, stream->priv->amount_percentage);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_statistic_model_class_init(LarStatisticModelClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // LarModelClass* parent_class = LAR_MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(LarStatisticModelPrivate));
    object_class->finalize     = lar_statistic_model_finalize;
    object_class->set_property = lar_statistic_model_set_property;
    object_class->get_property = lar_statistic_model_get_property;

    g_object_interface_install_property(g_iface, g_param_spec_uint("statistic-replicates", _("statistic replicates"), _("Set get statistic replicates"), 1, 100, 1,
                                                     G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));
    g_object_interface_install_property(g_iface, g_param_spec_uint("statistic-outliers", _("statistic outlier"), _("Set get statistic outlier"), 0, 30, 0,
                                                     G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));
    g_object_interface_install_property(g_iface, g_param_spec_double("statistic-max-cv", _("statistic max CV"), _("Set get statistic max cv"), 0., 100., 2.5,
                                                     G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));
    g_object_interface_install_property(g_iface, g_param_spec_double("statistic-threshold", _("statistic threshold"), _("Set get statistic threshold"), 0., 100., 0.,
                                                     G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));
    g_object_interface_install_property(
        g_iface, g_param_spec_double("statistic-jump", _("statistic jump"), _("statistic jump"), 0., 100., 0., G_PARAM_READWRITE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

    g_object_interface_install_property(g_iface, g_param_spec_uint("statistic-amount-counter", _("statistic amount counter"), _("Set|Get statistic amount counter"), 0, 5, 0,
                                                     G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

    g_object_interface_install_property(g_iface, g_param_spec_double("statistic-amount-percentage", _("statistic amount percentage"), _("Set|Get statistic amount percentage"), 20.0, 50.0, 20.0,
                                                     G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

    g_object_class_override_property(object_class, PROP_STATISTIC_REPLICATES, "statistic-replicates");
    g_object_class_override_property(object_class, PROP_STATISTIC_OUTLIERS, "statistic-outliers");
    g_object_class_override_property(object_class, PROP_STATISTIC_MAX_CV, "statistic-max-cv");
    g_object_class_override_property(object_class, PROP_STATISTIC_THRESHOLD, "statistic-threshold");
    g_object_class_override_property(object_class, PROP_STATISTIC_THRESHOLD, "statistic-jump");
    g_object_class_override_property(object_class, PROP_STATISTIC_AMOUNT_COUNTER, "statistic-amount-counter");
    g_object_class_override_property(object_class, PROP_STATISTIC_AMOUNT_PERCENTAGE, "statistic-amount-percentage");
}
