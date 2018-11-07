/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * ultimate-sensor-data.c
 * Copyright (C) sascha 2013 <sascha@sascha-VirtualBox>
 *
ultimate-sensor-data.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ultimate-sensor-data.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ultimate-messparm-object.h"
#include "market-time.h"

enum {
    PROP_0,
    PROP_ULTIAMTE_MESSPARM_VOLUME,
    PROP_ULTIAMTE_MESSPARM_INJECTION,
    PROP_ULTIAMTE_MESSPARM_INJECTION_TIC,
    PROP_ULTIAMTE_MESSPARM_FILLING_TIME,
    PROP_ULTIAMTE_MESSPARM_DELAY,
    PROP_ULTIAMTE_MESSPARM_DELAY_TIC,
    PROP_ULTIAMTE_MESSPARM_PRE_RINSING,
    PROP_ULTIAMTE_MESSPARM_RINSING_COUNT,
    PROP_ULTIAMTE_MESSPARM_AFTER_RINSING,
    PROP_ULTIAMTE_MESSPARM_AFTER_RINSING_COUNT,
    PROP_ULTIAMTE_MESSPARM_CODO_INJECTION,
    PROP_ULTIAMTE_MESSPARM_NEED_STRIPPING,
    PROP_ULTIAMTE_MESSPARM_STRIPPING_TIME,
    PROP_ULTIAMTE_MESSPARM_DILUTION_TYPE,
    PROP_ULTIAMTE_MESSPARM_DILUTION_FACTOR,
    PROP_ULTIAMTE_MESSPARM_DILUTION_PUMP_TIME,
    PROP_ULTIAMTE_MESSPARM_DILUTION_WAIT_TIME,
    PROP_STATISTIC_DEVIATION,
    PROP_ULTIAMTE_MESSPARM_AUTOCAL_DEVIATION,
    PROP_PROCESS_RINSING_ON,
    PROP_PROCESS_RINSING_Y1,
    PROP_PROCESS_RINSING_Y2,
    PROP_PROCESS_RINSING_INJ_VOL,
    PROP_PROCESS_RINSING_INJ_REPLICATE,
    PROP_PROCESS_RINSING_WAIT,
    PROP_PROCESS_RINSING_WAIT_AFTER,

};

struct _UltimateMessparmObjectPrivate {
    guint    volume;
    guint    injection_volume;
    guint    injection_volume_tic;
    gdouble  sample_filling_time;
    gdouble  delay;
    gdouble  delay_tic;
    gboolean pre_rinsing;
    guint    rinsing_count;
    gboolean after_rinsing;
    guint    after_rinsing_count;
    gboolean need_stripping;
    gboolean codo_injection;
    gdouble  stripping_time;
    guint    dilution_type;
    gdouble  dilution_factor;
    gdouble  dilution_pump_time;
    gdouble  dilution_wait_time;
    gdouble  amount_deviation;
    gdouble  autocal_deviation;
    gboolean prinsing_on;
    guint    prinsing_Y1;
    guint    prinsing_Y2;
    guint    inj_rep;
    guint    inj_vol;
    gdouble  wait_inj;
    gdouble  wait_after;
};

#define ULTIMATE_MESSPARM_OBJECT_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), ULTIMATE_TYPE_MESSPARM_OBJECT, UltimateMessparmObjectPrivate))

static guint ultimate_messparm_object_volume(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, 0);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), 0);
    return ULTIMATE_MESSPARM_OBJECT(umessparm)->priv->volume;
}

static gdouble ultimate_messparm_object_sample_filling_time(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, 0.0);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), 0.0);
    return ULTIMATE_MESSPARM_OBJECT(umessparm)->priv->sample_filling_time;
}

static gdouble ultimate_messparm_object_delay(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, 0.0);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), 0.0);
    return ULTIMATE_MESSPARM_OBJECT(umessparm)->priv->delay;
}

static gboolean ultimate_messparm_object_pre_rinsing(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, FALSE);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), FALSE);
    return ULTIMATE_MESSPARM_OBJECT(umessparm)->priv->pre_rinsing;
}

static guint ultimate_messparm_object_rinsing_count(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, 0);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), 0);
    return ULTIMATE_MESSPARM_OBJECT(umessparm)->priv->rinsing_count;
}

static gboolean ultimate_messparm_object_after_rinsing(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, FALSE);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), FALSE);
    return ULTIMATE_MESSPARM_OBJECT(umessparm)->priv->after_rinsing;
}

static guint ultimate_messparm_object_after_rinsing_count(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, 0);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), 0);
    return ULTIMATE_MESSPARM_OBJECT(umessparm)->priv->after_rinsing_count;
}

static gdouble ultimate_messparm_object_stripping_time(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, 0.0);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), 0.0);
    return ULTIMATE_MESSPARM_OBJECT(umessparm)->priv->stripping_time;
}
static gdouble ultimate_messparm_object_dilution_factor(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, 0.0);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), 0.0);
    return ULTIMATE_MESSPARM_OBJECT(umessparm)->priv->dilution_factor;
}
static gdouble ultimate_messparm_object_dilution_pump_time(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, 0.0);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), 0.0);
    return ULTIMATE_MESSPARM_OBJECT(umessparm)->priv->dilution_pump_time;
}
static gdouble ultimate_messparm_object_dilution_wait_time(UltimateMessparm *umessparm) {
    g_return_val_if_fail(umessparm != NULL, 0.0);
    g_return_val_if_fail(ULTIMATE_IS_MESSPARM(umessparm), 0.0);
    return ULTIMATE_MESSPARM_OBJECT(umessparm)->priv->dilution_wait_time;
}

static void ultimate_messparm_object_init_interface(UltimateMessparmInterface *iface) {
    iface->volume              = ultimate_messparm_object_volume;
    iface->sample_filling_time = ultimate_messparm_object_sample_filling_time;
    iface->delay               = ultimate_messparm_object_delay;
    iface->pre_rinsing         = ultimate_messparm_object_pre_rinsing;
    iface->rinsing_count       = ultimate_messparm_object_rinsing_count;
    iface->after_rinsing       = ultimate_messparm_object_after_rinsing;
    iface->after_rinsing_count = ultimate_messparm_object_after_rinsing_count;
    iface->stripping_time      = ultimate_messparm_object_stripping_time;
    iface->dilution_factor     = ultimate_messparm_object_dilution_factor;
    iface->dilution_pump_time  = ultimate_messparm_object_dilution_pump_time;
    iface->dilution_wait_time  = ultimate_messparm_object_dilution_wait_time;
}

G_DEFINE_TYPE_WITH_CODE(UltimateMessparmObject, ultimate_messparm_object, MKT_TYPE_PARAM_OBJECT,
    G_IMPLEMENT_INTERFACE(ULTIMATE_TYPE_MESSPARM, ultimate_messparm_object_init_interface) MKT_CREATE_MODEL_FULL(mkt_connection_new_default_parameter(TRUE)))

static void ultimate_messparm_object_init(UltimateMessparmObject *ultimate_messparm_object) {
    UltimateMessparmObjectPrivate *priv = ULTIMATE_MESSPARM_OBJECT_PRIVATE(ultimate_messparm_object);
    ultimate_messparm_object->priv      = priv;
    /* TODO: Add initialization code here */
}

static void ultimate_messparm_object_finalize(GObject *object) {
    // MktMessparmObject *data = MKT_MESSPARM_DIFF_OBJECT(object);
    G_OBJECT_CLASS(ultimate_messparm_object_parent_class)->finalize(object);
}

static void ultimate_messparm_object_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(ULTIMATE_IS_MESSPARM_OBJECT(object));
    UltimateMessparmObject *messparm = ULTIMATE_MESSPARM_OBJECT(object);
    switch (prop_id) {
    case PROP_ULTIAMTE_MESSPARM_VOLUME:
        messparm->priv->volume = g_value_get_uint(value);
        break;
    case PROP_ULTIAMTE_MESSPARM_INJECTION:
        messparm->priv->injection_volume = g_value_get_uint(value);
        break;
    case PROP_ULTIAMTE_MESSPARM_INJECTION_TIC:
        messparm->priv->injection_volume_tic = g_value_get_uint(value);
        break;
    case PROP_ULTIAMTE_MESSPARM_FILLING_TIME:
        messparm->priv->sample_filling_time = g_value_get_double(value);
        break;
    case PROP_ULTIAMTE_MESSPARM_DELAY:
        messparm->priv->delay = g_value_get_double(value);
        break;
    case PROP_ULTIAMTE_MESSPARM_DELAY_TIC:
        messparm->priv->delay_tic = g_value_get_double(value);
        break;
    case PROP_ULTIAMTE_MESSPARM_PRE_RINSING:
        messparm->priv->pre_rinsing = g_value_get_boolean(value);
        break;
    case PROP_ULTIAMTE_MESSPARM_RINSING_COUNT:
        messparm->priv->rinsing_count = g_value_get_uint(value);
        break;
    case PROP_ULTIAMTE_MESSPARM_AFTER_RINSING:
        messparm->priv->after_rinsing = g_value_get_boolean(value);
        break;
    case PROP_ULTIAMTE_MESSPARM_AFTER_RINSING_COUNT:
        messparm->priv->after_rinsing_count = g_value_get_uint(value);
        break;
    case PROP_ULTIAMTE_MESSPARM_CODO_INJECTION:
        messparm->priv->codo_injection = g_value_get_boolean(value);
        break;
    case PROP_ULTIAMTE_MESSPARM_NEED_STRIPPING:
        messparm->priv->need_stripping = g_value_get_boolean(value);
        break;
    case PROP_ULTIAMTE_MESSPARM_STRIPPING_TIME:
        messparm->priv->stripping_time = g_value_get_double(value);
        break;
    case PROP_ULTIAMTE_MESSPARM_DILUTION_TYPE:
        messparm->priv->dilution_type = g_value_get_uint(value);
        break;
    case PROP_ULTIAMTE_MESSPARM_DILUTION_FACTOR:
        messparm->priv->dilution_factor = g_value_get_double(value);
        break;
    case PROP_ULTIAMTE_MESSPARM_DILUTION_PUMP_TIME:
        messparm->priv->dilution_pump_time = g_value_get_double(value);
        break;
    case PROP_ULTIAMTE_MESSPARM_DILUTION_WAIT_TIME:
        messparm->priv->dilution_wait_time = g_value_get_double(value);
        break;
    case PROP_STATISTIC_DEVIATION:
        messparm->priv->amount_deviation = g_value_get_double(value);
        break;
    case PROP_ULTIAMTE_MESSPARM_AUTOCAL_DEVIATION:
        messparm->priv->autocal_deviation = g_value_get_double(value);
        break;
    case PROP_PROCESS_RINSING_ON:
        messparm->priv->prinsing_on = g_value_get_boolean(value);
        break;
    case PROP_PROCESS_RINSING_Y1:
        messparm->priv->prinsing_Y1 = g_value_get_uint(value);
        break;
    case PROP_PROCESS_RINSING_Y2:
        messparm->priv->prinsing_Y2 = g_value_get_uint(value);
        break;
    case PROP_PROCESS_RINSING_INJ_VOL:
        messparm->priv->inj_vol = g_value_get_uint(value);
        break;
    case PROP_PROCESS_RINSING_INJ_REPLICATE:
        messparm->priv->inj_rep = g_value_get_uint(value);
        break;
    case PROP_PROCESS_RINSING_WAIT:
        messparm->priv->wait_inj = g_value_get_double(value);
        break;
    case PROP_PROCESS_RINSING_WAIT_AFTER:
        messparm->priv->wait_after = g_value_get_double(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void ultimate_messparm_object_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(ULTIMATE_IS_MESSPARM_OBJECT(object));
    UltimateMessparmObject *messparm = ULTIMATE_MESSPARM_OBJECT(object);
    switch (prop_id) {
    case PROP_ULTIAMTE_MESSPARM_VOLUME:
        g_value_set_uint(value, messparm->priv->volume);
        break;
    case PROP_ULTIAMTE_MESSPARM_INJECTION:
        g_value_set_uint(value, messparm->priv->injection_volume);
        break;
    case PROP_ULTIAMTE_MESSPARM_INJECTION_TIC:
        g_value_set_uint(value, messparm->priv->injection_volume_tic);
        break;
    case PROP_ULTIAMTE_MESSPARM_FILLING_TIME:
        g_value_set_double(value, messparm->priv->sample_filling_time);
        break;
    case PROP_ULTIAMTE_MESSPARM_DELAY:
        g_value_set_double(value, messparm->priv->delay);
        break;
    case PROP_ULTIAMTE_MESSPARM_DELAY_TIC:
        g_value_set_double(value, messparm->priv->delay_tic);
        break;
    case PROP_ULTIAMTE_MESSPARM_PRE_RINSING:
        g_value_set_boolean(value, messparm->priv->pre_rinsing);
        break;
    case PROP_ULTIAMTE_MESSPARM_RINSING_COUNT:
        g_value_set_uint(value, messparm->priv->rinsing_count);
        break;
    case PROP_ULTIAMTE_MESSPARM_AFTER_RINSING:
        g_value_set_boolean(value, messparm->priv->after_rinsing);
        break;
    case PROP_ULTIAMTE_MESSPARM_AFTER_RINSING_COUNT:
        g_value_set_uint(value, messparm->priv->after_rinsing_count);
        break;
    case PROP_ULTIAMTE_MESSPARM_CODO_INJECTION:
        g_value_set_boolean(value, messparm->priv->codo_injection);
        break;
    case PROP_ULTIAMTE_MESSPARM_NEED_STRIPPING:
        g_value_set_boolean(value, messparm->priv->need_stripping);
        break;
    case PROP_ULTIAMTE_MESSPARM_STRIPPING_TIME:
        g_value_set_double(value, messparm->priv->stripping_time);
        break;
    case PROP_ULTIAMTE_MESSPARM_DILUTION_TYPE:
        g_value_set_uint(value, messparm->priv->dilution_type);
        break;
    case PROP_ULTIAMTE_MESSPARM_DILUTION_FACTOR:
        g_value_set_double(value, messparm->priv->dilution_factor);
        break;
    case PROP_ULTIAMTE_MESSPARM_DILUTION_PUMP_TIME:
        g_value_set_double(value, messparm->priv->dilution_pump_time);
        break;
    case PROP_ULTIAMTE_MESSPARM_DILUTION_WAIT_TIME:
        g_value_set_double(value, messparm->priv->dilution_wait_time);
        break;
    case PROP_STATISTIC_DEVIATION:
        g_value_set_double(value, messparm->priv->amount_deviation);
        break;
    case PROP_ULTIAMTE_MESSPARM_AUTOCAL_DEVIATION:
        g_value_set_double(value, messparm->priv->autocal_deviation);
        break;
    case PROP_PROCESS_RINSING_ON:
        g_value_set_boolean(value, messparm->priv->prinsing_on);
        break;
    case PROP_PROCESS_RINSING_Y1:
        g_value_set_uint(value, messparm->priv->prinsing_Y1);
        break;
    case PROP_PROCESS_RINSING_Y2:
        g_value_set_uint(value, messparm->priv->prinsing_Y2);
        break;
    case PROP_PROCESS_RINSING_INJ_VOL:
        g_value_set_uint(value, messparm->priv->inj_vol);
        break;
    case PROP_PROCESS_RINSING_INJ_REPLICATE:
        g_value_set_uint(value, messparm->priv->inj_rep);
        break;
    case PROP_PROCESS_RINSING_WAIT:
        g_value_set_double(value, messparm->priv->wait_inj);
        break;
    case PROP_PROCESS_RINSING_WAIT_AFTER:
        g_value_set_double(value, messparm->priv->wait_after);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void ultimate_messparm_object_class_init(UltimateMessparmObjectClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // MktModelClass* parent_class = MKT_MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(UltimateMessparmObjectPrivate));
    object_class->finalize     = ultimate_messparm_object_finalize;
    object_class->set_property = ultimate_messparm_object_set_property;
    object_class->get_property = ultimate_messparm_object_get_property;

    g_object_class_override_property(object_class, PROP_ULTIAMTE_MESSPARM_VOLUME, "ultimate-sample-volume");
    g_object_class_override_property(object_class, PROP_ULTIAMTE_MESSPARM_INJECTION, "ultimate-injection-volume");
    g_object_class_override_property(object_class, PROP_ULTIAMTE_MESSPARM_INJECTION_TIC, "ultimate-injection-volume-tic");
    g_object_class_override_property(object_class, PROP_ULTIAMTE_MESSPARM_FILLING_TIME, "ultimate-sample-filling-time");
    g_object_class_override_property(object_class, PROP_ULTIAMTE_MESSPARM_DELAY, "ultimate-delay");
    g_object_class_override_property(object_class, PROP_ULTIAMTE_MESSPARM_DELAY_TIC, "ultimate-delay-tic");
    g_object_class_override_property(object_class, PROP_ULTIAMTE_MESSPARM_PRE_RINSING, "ultimate-is-pre-rinsing");
    g_object_class_override_property(object_class, PROP_ULTIAMTE_MESSPARM_RINSING_COUNT, "ultimate-rinsing-count");
    g_object_class_override_property(object_class, PROP_ULTIAMTE_MESSPARM_AFTER_RINSING, "ultimate-is-after-rinsing");
    g_object_class_override_property(object_class, PROP_ULTIAMTE_MESSPARM_AFTER_RINSING_COUNT, "ultimate-after-rinsing-count");
    g_object_class_override_property(object_class, PROP_ULTIAMTE_MESSPARM_CODO_INJECTION, "ultimate-codo-injection");
    g_object_class_override_property(object_class, PROP_ULTIAMTE_MESSPARM_NEED_STRIPPING, "ultimate-need-stripping");
    g_object_class_override_property(object_class, PROP_ULTIAMTE_MESSPARM_STRIPPING_TIME, "ultimate-stripping-time");
    g_object_class_override_property(object_class, PROP_ULTIAMTE_MESSPARM_DILUTION_TYPE, "ultimate-dilution-type");
    g_object_class_override_property(object_class, PROP_ULTIAMTE_MESSPARM_DILUTION_FACTOR, "ultimate-dilution-factor");
    g_object_class_override_property(object_class, PROP_ULTIAMTE_MESSPARM_DILUTION_PUMP_TIME, "ultimate-dilution-pump-time");
    g_object_class_override_property(object_class, PROP_ULTIAMTE_MESSPARM_DILUTION_WAIT_TIME, "ultimate-dilution-wait-time");
    g_object_class_override_property(object_class, PROP_STATISTIC_DEVIATION, "ultimate-allowed-deviation");
    g_object_class_override_property(object_class, PROP_ULTIAMTE_MESSPARM_AUTOCAL_DEVIATION, "ultimate-autocal-deviation");

    g_object_class_override_property(object_class, PROP_PROCESS_RINSING_ON, "process-rinsing");
    g_object_class_override_property(object_class, PROP_PROCESS_RINSING_Y1, "prinsing-y1-pos");
    g_object_class_override_property(object_class, PROP_PROCESS_RINSING_Y2, "prinsing-y2-pos");
    g_object_class_override_property(object_class, PROP_PROCESS_RINSING_INJ_VOL, "prinsing-injection-volume");
    g_object_class_override_property(object_class, PROP_PROCESS_RINSING_INJ_REPLICATE, "prinsing-injection-replicate");
    g_object_class_override_property(object_class, PROP_PROCESS_RINSING_WAIT, "prinsing-wait-time");
    g_object_class_override_property(object_class, PROP_PROCESS_RINSING_WAIT_AFTER, "prinsing-wait-after");
}
