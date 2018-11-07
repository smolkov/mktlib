/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * ultrastream.c
 * Copyright (C) 2017 LAR
 */

#include "ultrastream.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>
#include <string.h>

struct _UltraStreamClass
{
    StreamClass parent_class;
};

struct _UltraStream
{
    Stream parent_instance;
    guint volume;
    guint injectionVolume;
    guint injectionVolumeTic;
    gdouble fillingTime;
    gdouble delay;
    gdouble delayTic;
    gboolean preRinsing;
    guint rinsingCount;
    gboolean afterRinsing;
    guint afterCount;
    gboolean needStripping;
    gboolean codoInjection;
    gdouble strippingTime;
    gboolean isDilution;
    guint dilutionType;
    gdouble dilutionFactor;
    gdouble dilutionPumpTime;
    gdouble dilutionWaitTime;
    gdouble amountDeviation;
    gdouble autocalDeviation;
    gboolean prinsingON;
    guint prinsingY1;
    guint prinsingY2;
    guint injRep;
    guint injVol;
    gdouble waitAfter;
    Channel *TOC;
    Channel *TC;
    Channel *TIC;
    Channel *TNb;
    Channel *CODo;
    Posparm *pos;
};

enum
{
    PROP_0,
    PROP_VOLUME,
    PROP_INJECTION_VOL,
    PROP_INJECTION_VOL_TIC,
    PROP_FILLING_TIME,
    PROP_DELAY,
    PROP_DELAY_TIC,
    PROP_PRE_RINSING,
    PROP_RINSING_COUNT,
    PROP_AFTER_RINSING,
    PROP_AFTER_COUNT,
    PROP_NEED_STREEPING,
    PROP_CODO_INJ,
    PROP_STRIPPING_TIME,
    PROP_IS_DILUTION,
    PROP_DILUTION_TYPE,
    PROP_DILUTION_FACTOR,
    PROP_DILUTION_PUMP_TIME,
    PROP_DILUTION_WAIT_TIME,
    PROP_AMOUT_DEVIATION,
    PROP_AUTOCAL_DEVIATION,
    PROP_PRINSING_ON,
    PROP_PRINSING_Y1,
    PROP_PRINSING_Y2,
    PROP_INJ_REPLICATE,
    PROP_INJ_VOLUME,
    PROP_WAIT_AFTER,
    PROP_CHANNEL_TOC,
    PROP_CHANNEL_TC,
    PROP_CHANNEL_TIC,
    PROP_CHANNEL_TNB,
    PROP_CHANNEL_CODo,
    PROP_VESSELS_POS,
};

static JsonSerializableIface *ultra_stream_serializable_iface = NULL;

static void json_serializable_iface_init(gpointer object_class);

G_DEFINE_TYPE_WITH_CODE(UltraStream, ultra_stream, TYPE_STREAM, G_IMPLEMENT_INTERFACE(JSON_TYPE_SERIALIZABLE, json_serializable_iface_init));

static JsonNode *ultra_stream_serialize_property(JsonSerializable *serializable, const gchar *name, const GValue *value, GParamSpec *pspec)
{
    // JsonNode *retval = NULL;
    return ultra_stream_serializable_iface->serialize_property(serializable, name, value, pspec);
}

static gboolean ultra_stream_deserialize_property(JsonSerializable *serializable, const gchar *property_name, GValue *value, GParamSpec *pspec, JsonNode *property_node)
{
    return ultra_stream_serializable_iface->deserialize_property(serializable, property_name, value, pspec, property_node);
}

static void json_serializable_iface_init(gpointer object_class)
{
    JsonSerializableIface *iface = object_class;
    ultra_stream_serializable_iface = g_type_default_interface_peek(JSON_TYPE_SERIALIZABLE);
    iface->serialize_property = ultra_stream_serialize_property;
    iface->deserialize_property = ultra_stream_deserialize_property;
}

static void ultra_stream_init(UltraStream *ultraStream)
{
    ultraStream->volume = 100;
    ultraStream->injectionVolume = 100;
    ultraStream->injectionVolumeTic = 300;
    ultraStream->fillingTime = 1.0;
    ultraStream->delay = 0.0;
    ultraStream->delayTic = 0.0;
    ultraStream->TOC = CHANNEL(g_object_new(TYPE_CHANNEL, "name", "TOC", "sensor", "/com/lar/sensors/signal/inst_1_1", NULL));
    ultraStream->TC = CHANNEL(g_object_new(TYPE_CHANNEL, "name", "TC", "sensor", "/com/lar/sensors/signal/inst_1_1", NULL));
    ultraStream->TIC = CHANNEL(g_object_new(TYPE_CHANNEL, "name", "TIC", "sensor", "/com/lar/sensors/signal/inst_1_1", NULL));
    ultraStream->TNb = CHANNEL(g_object_new(TYPE_CHANNEL, "name", "TNb", "sensor", "/com/lar/sensors/signal/inst_3_1", NULL));
    ultraStream->CODo = CHANNEL(g_object_new(TYPE_CHANNEL, "name", "CODo", "sensor", "/com/lar/sensors/signal/inst_4_1", NULL));
    ultraStream->pos = POSPARM(g_object_new(TYPE_POSPARM, NULL));
}

static void ultra_stream_finalize(GObject *object)
{
    UltraStream *ultraStream = ULTRA_STREAM(object);
    if (ultraStream->TOC)
        g_object_unref(ultraStream->TOC);
    if (ultraStream->TC)
        g_object_unref(ultraStream->TC);
    if (ultraStream->TIC)
        g_object_unref(ultraStream->TIC);
    if (ultraStream->TNb)
        g_object_unref(ultraStream->TNb);
    if (ultraStream->CODo)
        g_object_unref(ultraStream->CODo);
    if (ultraStream->pos)
        g_object_unref(ultraStream->pos);

    G_OBJECT_CLASS(ultra_stream_parent_class)->finalize(object);
}

static void ultra_stream_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_ULTRA_STREAM(object));
    UltraStream *ultraStream = ULTRA_STREAM(object);
    switch (prop_id)
    {
    case PROP_VOLUME:
        ultraStream->volume = g_value_get_uint(value);
        break;
    case PROP_INJECTION_VOL:
        ultraStream->injectionVolume = g_value_get_uint(value);
        break;
    case PROP_INJECTION_VOL_TIC:
        ultraStream->injectionVolumeTic = g_value_get_uint(value);
        break;
    case PROP_FILLING_TIME:
        ultraStream->fillingTime = g_value_get_double(value);
        break;
    case PROP_DELAY:
        ultraStream->delay = g_value_get_double(value);
        break;
    case PROP_DELAY_TIC:
        ultraStream->delayTic = g_value_get_double(value);
        break;
    case PROP_PRE_RINSING:
        ultraStream->preRinsing = g_value_get_boolean(value);
        break;
    case PROP_RINSING_COUNT:
        ultraStream->rinsingCount = g_value_get_uint(value);
        break;
    case PROP_AFTER_RINSING:
        ultraStream->afterRinsing = g_value_get_boolean(value);
        break;
    case PROP_AFTER_COUNT:
        ultraStream->afterCount = g_value_get_uint(value);
        break;
    case PROP_CODO_INJ:
        ultraStream->codoInjection = g_value_get_boolean(value);
        break;
    case PROP_NEED_STREEPING:
        ultraStream->needStripping = g_value_get_boolean(value);
        break;
    case PROP_STRIPPING_TIME:
        ultraStream->strippingTime = g_value_get_double(value);
        break;
    case PROP_IS_DILUTION:
        ultraStream->isDilution = g_value_get_boolean(value);
        break;
    case PROP_DILUTION_TYPE:
        ultraStream->dilutionType = g_value_get_uint(value);
        break;
    case PROP_DILUTION_PUMP_TIME:
        ultraStream->dilutionPumpTime = g_value_get_double(value);
        break;
    case PROP_DILUTION_WAIT_TIME:
        ultraStream->dilutionWaitTime = g_value_get_double(value);
        break;
    case PROP_DILUTION_FACTOR:
        ultraStream->dilutionFactor = g_value_get_double(value);
        break;
    case PROP_AMOUT_DEVIATION:
        ultraStream->amountDeviation = g_value_get_double(value);
        break;
    case PROP_AUTOCAL_DEVIATION:
        ultraStream->autocalDeviation = g_value_get_double(value);
        break;
    case PROP_PRINSING_ON:
        ultraStream->prinsingON = g_value_get_boolean(value);
        break;
    case PROP_PRINSING_Y1:
        ultraStream->prinsingY1 = g_value_get_uint(value);
        break;
    case PROP_PRINSING_Y2:
        ultraStream->prinsingY2 = g_value_get_uint(value);
        break;
    case PROP_INJ_VOLUME:
        ultraStream->injVol = g_value_get_uint(value);
        break;
    case PROP_INJ_REPLICATE:
        ultraStream->injRep = g_value_get_uint(value);
        break;
    case PROP_WAIT_AFTER:
        ultraStream->waitAfter = g_value_get_double(value);
        break;
    case PROP_CHANNEL_TOC:
        if (ultraStream->TOC)
            g_object_unref(ultraStream->TOC);
        ultraStream->TOC = g_value_dup_object(value);
        break;
    case PROP_CHANNEL_TC:
        if (ultraStream->TC)
            g_object_unref(ultraStream->TC);
        ultraStream->TC = g_value_dup_object(value);
        break;
    case PROP_CHANNEL_TIC:
        if (ultraStream->TIC)
            g_object_unref(ultraStream->TIC);
        ultraStream->TIC = g_value_dup_object(value);
        break;
    case PROP_CHANNEL_TNB:
        if (ultraStream->TNb)
            g_object_unref(ultraStream->TNb);
        ultraStream->TNb = g_value_dup_object(value);
        break;
    case PROP_CHANNEL_CODo:
        if (ultraStream->CODo)
            g_object_unref(ultraStream->CODo);
        ultraStream->CODo = g_value_dup_object(value);
        break;
    case PROP_VESSELS_POS:
        if (ultraStream->pos)
            g_object_unref(ultraStream->pos);
        ultraStream->pos = g_value_dup_object(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}
static void ultra_stream_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_ULTRA_STREAM(object));
    UltraStream *ultra_stream = ULTRA_STREAM(object);

    switch (prop_id)
    {
    case PROP_VOLUME:
        g_value_set_uint(value, ultra_stream->volume);
        break;
    case PROP_INJECTION_VOL:
        g_value_set_uint(value, ultra_stream->injectionVolume);
        break;
    case PROP_INJECTION_VOL_TIC:
        g_value_set_uint(value, ultra_stream->injectionVolumeTic);
        break;
    case PROP_FILLING_TIME:
        g_value_set_double(value, ultra_stream->fillingTime);
        break;
    case PROP_DELAY:
        g_value_set_double(value, ultra_stream->delay);
        break;
    case PROP_DELAY_TIC:
        g_value_set_double(value, ultra_stream->delayTic);
        break;
    case PROP_PRE_RINSING:
        g_value_set_boolean(value, ultra_stream->preRinsing);
        break;
    case PROP_RINSING_COUNT:
        g_value_set_uint(value, ultra_stream->rinsingCount);
        break;
    case PROP_AFTER_RINSING:
        g_value_set_boolean(value, ultra_stream->afterRinsing);
        break;
    case PROP_AFTER_COUNT:
        g_value_set_uint(value, ultra_stream->afterCount);
        break;
    case PROP_CODO_INJ:
        g_value_set_boolean(value, ultra_stream->codoInjection);
        break;
    case PROP_NEED_STREEPING:
        g_value_set_boolean(value, ultra_stream->needStripping);
        break;
    case PROP_STRIPPING_TIME:
        g_value_set_double(value, ultra_stream->strippingTime);
        break;
    case PROP_IS_DILUTION:
        g_value_set_boolean(value, ultra_stream->isDilution);
        break;
    case PROP_DILUTION_TYPE:
        g_value_set_uint(value, ultra_stream->dilutionType);
        break;
    case PROP_DILUTION_PUMP_TIME:
        g_value_set_double(value, ultra_stream->dilutionPumpTime);
        break;
    case PROP_DILUTION_WAIT_TIME:
        g_value_set_double(value, ultra_stream->dilutionWaitTime);
        break;
    case PROP_DILUTION_FACTOR:
        g_value_set_double(value, ultra_stream->dilutionFactor);
        break;
    case PROP_AMOUT_DEVIATION:
        g_value_set_double(value, ultra_stream->amountDeviation);
        break;
    case PROP_AUTOCAL_DEVIATION:
        g_value_set_double(value, ultra_stream->autocalDeviation);
        break;
    case PROP_PRINSING_ON:
        g_value_set_boolean(value, ultra_stream->prinsingON);
        break;
    case PROP_PRINSING_Y1:
        g_value_set_uint(value, ultra_stream->prinsingY1);
        break;
    case PROP_PRINSING_Y2:
        g_value_set_uint(value, ultra_stream->prinsingY2);
        break;
    case PROP_INJ_VOLUME:
        g_value_set_uint(value, ultra_stream->injVol);
        break;
    case PROP_INJ_REPLICATE:
        g_value_set_uint(value, ultra_stream->injRep);
        break;
    case PROP_WAIT_AFTER:
        g_value_set_double(value, ultra_stream->waitAfter);
        break;
    case PROP_CHANNEL_TOC:
        g_value_set_object(value, ultra_stream->TOC);
        break;
    case PROP_CHANNEL_TC:
        g_value_set_object(value, ultra_stream->TC);
        break;
    case PROP_CHANNEL_TIC:
        g_value_set_object(value, ultra_stream->TIC);
        break;
    case PROP_CHANNEL_TNB:
        g_value_set_object(value, ultra_stream->TNb);
        break;
    case PROP_CHANNEL_CODo:
        g_value_set_object(value, ultra_stream->CODo);
        break;
    case PROP_VESSELS_POS:
        g_value_set_object(value, ultra_stream->pos);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void ultra_stream_class_init(UltraStreamClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->finalize = ultra_stream_finalize;
    object_class->set_property = ultra_stream_set_property;
    object_class->get_property = ultra_stream_get_property;

    g_object_class_install_property(
        object_class, PROP_VOLUME, g_param_spec_uint("volume", "sample volume", "Set get sample volume", 0, 650, 100, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_INJECTION_VOL,
                                    g_param_spec_uint("injectionVolume", "injection volume furnace", "Set get injection volume furnace", 0, 650, 100, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_INJECTION_VOL_TIC,
                                    g_param_spec_uint("injectionVolumeTic", "injection volume TIC", "Set get injection volume TIC", 0, 650, 300, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_FILLING_TIME,
                                    g_param_spec_double("fillingTime", "sample filling time", "Set|Get Sample filling time", 0.0, 20000.0, 1.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_DELAY, g_param_spec_double("delay", "TC delay time", "Set|Get TC delay time", 0.0, 20000.0, 0.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_DELAY_TIC, g_param_spec_double("delayTic", "TIC delay time", "Set|Get TC delay time", 0.0, 20000.0, 50.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_PRE_RINSING, g_param_spec_boolean("preRinsing", "need pre rinsing system ?", "Set need pre rinsing system", FALSE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_RINSING_COUNT, g_param_spec_uint("rinsingCount", "rinsing count", "Set Rinsing count", 1, 20, 1, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_AFTER_RINSING, g_param_spec_boolean("afterRinsing", "need after rinsing system?", "Set need after rinsing system", FALSE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_AFTER_COUNT, g_param_spec_uint("afterCount", "rinsing count", "Set Rinsing count", 1, 20, 1, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_CODO_INJ, g_param_spec_boolean("codoInjection", "codo injection system", "Set is codo injection system", FALSE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_NEED_STREEPING, g_param_spec_boolean("needStripping", "stripping", "Set need pre rinsing system", FALSE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_STRIPPING_TIME,
                                    g_param_spec_double("strippingTime", "stripping time", "Set|Get Stripping time", 0.0, 20000.0, 60.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_IS_DILUTION, g_param_spec_boolean("isDilution", "is dilution", "Set is dilurion", FALSE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_DILUTION_TYPE, g_param_spec_uint("dilutionType", "is dilution", "Set|Get is dilution", 0, 2, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_DILUTION_FACTOR,
                                    g_param_spec_double("dilutionFactor", "dilution factor", "Set|Get Dilution factor", 1.0, 20.0, 5.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_DILUTION_PUMP_TIME,
                                    g_param_spec_double("dilutionPumpTime", "dilution pump time", "Set|Get dilution pump time", 3.0, 600.0, 10.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_DILUTION_WAIT_TIME,
                                    g_param_spec_double("dilutionWaitTime", "dilution wait time", "Set|Get dilution wait time", 3.0, 2000.0, 10.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_AMOUT_DEVIATION,
                                    g_param_spec_double("amountDeviation", "allowed deviation", "Set|Get allowed deviation", 1.0, 100.0, 10.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_AUTOCAL_DEVIATION,
                                    g_param_spec_double("autocalDeviation", "auto calibration deviation", "Set|Get auto calibration deviation", 0.1, 100.0, 5.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    // Option Process rinsing ultra
    g_object_class_install_property(
        object_class, PROP_PRINSING_ON, g_param_spec_boolean("prinsingON", "extra rinsing option", "On|Off extra rinsing option", FALSE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_PRINSING_Y1,
                                    g_param_spec_uint("prinsingY1", "process rinsing Y1 position", "Set|Get process rinsing Y1 position", 600, 1720, 720, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_PRINSING_Y2,
                                    g_param_spec_uint("prinsingY2", "process rinsing Y2 position", "Set|Get process rinsing Y2 position", 600, 1720, 1100, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_INJ_VOLUME,
                                    g_param_spec_uint("injVol", "injection volume process rinsing", "Set get injection volume for process rinsing", 200, 600, 200, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_INJ_REPLICATE,
                                    g_param_spec_uint("injRep", "process rinsing injection replicates", "Set get injection replicates for process rinsing", 1, 5, 1, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_WAIT_AFTER,
                                    g_param_spec_double("waitAfter", "Wait time after rinsing process", "Set|Get wait time after rinsing process", 0.0, 180.0, 80.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_CHANNEL_TOC, g_param_spec_object("toc", "TOC channel", "TOC channel", TYPE_CHANNEL, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_CHANNEL_TC, g_param_spec_object("tc", "TC channel", "TC channel", TYPE_CHANNEL, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_CHANNEL_TIC, g_param_spec_object("tic", "TIC channel", "TIC channel", TYPE_CHANNEL, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_CHANNEL_TNB, g_param_spec_object("tnb", "TNb channel", "TNb channel", TYPE_CHANNEL, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_CHANNEL_CODo, g_param_spec_object("codo", "CODo channel", "CODo channel", TYPE_CHANNEL, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_VESSELS_POS, g_param_spec_object("position", "Vessels positions", "Vessels positions", TYPE_POSPARM, G_PARAM_READABLE | G_PARAM_WRITABLE));

    // g_object_class_install_property(
    //     object_class, ONLINE_STATISTIC, g_param_spec_object("online-statistic", "name", "name", TYPE_STATISTIC, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    // g_object_class_install_property(object_class, PROP_UPDATED,
    //     g_param_spec_int64("updated", "updated", "updated time in msec", 0, G_MAXINT64, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
}
/**
 * UltraStreamGetVolume:
 * @ultra_stream: a #UltraStream
 *
 * Get ultra volume.
 *
 * Return:  volume
 */
guint UltraStreamGetVolume(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, 0);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), 0);
    return ultraStream->volume;
}

/**
 * UltraStreamGetInjVol:
 * @ultra_stream: a #UltraStream
 *
 * Get ultra furnace injection volume.
 *
 * Return:  volume in m/l
 */
guint UltraStreamGetInjectionVolume(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, 0);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), 0);
    return ultraStream->injectionVolume;
}

/**
 * UltraStreamGetInjectionVolumeTic:
 * @ultra_stream: a #UltraStream
 *
 * Get ultra TIC-port injection volume.
 *
 * Return:  volume in m/l
 */
guint UltraStreamGetInjectionVolumeTic(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, 0);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), 0);
    return ultraStream->injectionVolumeTic;
}

/**
 * UltraStreamGetInjectionVolumeTic:
 * @ultra_stream: a #UltraStream
 *
 * Get ultra vessel filling time
 *
 * Return:  sec
 */
gdouble UltraStreamGetFillingTime(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, 0.0);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), 0.0);
    return ultraStream->fillingTime;
}

/**
 * UltraStreamGetDelay:
 * @ultra_stream: a #UltraStream
 *
 * Get ultra TC measurement delay
 *
 * Return:  interval in sec
 */
gdouble UltraStreamGetDelay(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, 0.0);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), 0.0);
    return ultraStream->delay;
}

/**
 * UltraStreamGetDelay:
 * @ultra_stream: a #UltraStream
 *
 * Get ultra TIC measurement delay
 *
 * Return:  interval in sec
 */
gdouble UltraStreamGetDelayTIC(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, 0.0);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), 0.0);
    return ultraStream->delayTic;
}

/**
 * UltraStreamGetDelay:
 * @ultra_stream: a #UltraStream
 *
 * Get ultra pre rinsing parameter
 *
 * Return:  TRUE if pre rinsing activated
 */
gboolean UltraStreamGetPreRinsing(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, FALSE);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), FALSE);
    return ultraStream->preRinsing;
}

/**
 * UltraStreamGetRinsingCount:
 * @ultra_stream: a #UltraStream
 *
 * Get pre rinsing replicates
 *
 * Return:  replicates.
 */
guint UltraStreamGetRinsingCount(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, 1);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), 1);
    return ultraStream->rinsingCount;
}

/**
 * UltraStreamGetAfterRinsing:
 * @ultra_stream: a #UltraStream
 *
 * Get ultra pre rinsing parameter
 *
 * Return:  TRUE if after rinsing activated
 */
gboolean UltraStreamGetAfterRinsing(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, FALSE);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), FALSE);
    return ultraStream->afterRinsing;
}

/**
 * UltraStreamGetAfterCount:
 * @ultra_stream: a #UltraStream
 *
 * Get after rinsing replicates
 *
 * Return:  replicates.
 */
guint UltraStreamGetAfterCount(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, 1);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), 1);
    return ultraStream->afterCount;
}

/**
 * UltraStreamGetAfterRinsing:
 * @ultra_stream: a #UltraStream
 *
 * Is CODo injection activated
 *
 * Return:  TRUE if CODo injection activated
 */
gboolean UltraStreamGetCODoInjection(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, FALSE);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), FALSE);
    return ultraStream->codoInjection;
}

/**
 * UltraStreamGetNeedStripping:
 * @ultra_stream: a #UltraStream
 *
 * Need stripping
 *
 * Return:  TRUE if stripping activated
 */
gboolean UltraStreamGetNeedStripping(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, FALSE);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), FALSE);
    return ultraStream->needStripping;
}

/**
 * UltraStreamGetStrippingTime:
 * @ultra_stream: a #UltraStream
 *
 * Get stripping time in second
 *
 * Return:   sec
 */
gdouble UltraStreamGetStrippingTime(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, 0.0);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), 0.0);
    return ultraStream->strippingTime;
}

/**
 * UltraStreamGetDilutionType:
 * @ultra_stream: a #UltraStream
 *
 * Get dilution type
 *
 * Return:  dilution type.
 */
guint UltraStreamGetDilutionType(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, 0);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), 0);
    return ultraStream->dilutionType;
}
/**
 * UltraStreamGetDilutionFactor:
 * @ultra_stream: a #UltraStream
 *
 * Get dilution factor
 *
 * Return:   factor
 */
gdouble UltraStreamGetDilutionFactor(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, 0.0);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), 0.0);
    return ultraStream->dilutionFactor;
}

/**
 * UltraStreamGetDilutionPumpTime:
 * @ultra_stream: a #UltraStream
 *
 * Get dilution pump time
 *
 * Return:   sec
 */
gdouble UltraStreamGetDilutionPumpTime(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, 0.0);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), 0.0);
    return ultraStream->dilutionPumpTime;
}
/**
 * UltraStreamGetDilutionWaitTime:
 * @ultra_stream: a #UltraStream
 *
 * Get dilution wait time
 *
 * Return:   sec
 */
gdouble UltraStreamGetDilutionWaitTime(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, 0.0);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), 0.0);
    return ultraStream->dilutionWaitTime;
}
/**
 * UltraStreamGetAmountDeviation:
 * @ultra_stream: a #UltraStream
 *
 * Get amount deviation
 *
 * Return:   deviation
 */
gdouble UltraStreamGetAmountDeviation(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, 0.0);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), 0.0);
    return ultraStream->amountDeviation;
}
/**
 * UltraStreamGetAutocalDeviation:
 * @ultra_stream: a #UltraStream
 *
 * Get autocal deviation
 *
 * Return:   deviation
 */
gdouble UltraStreamGetAutocalDeviation(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, 0.0);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), 0.0);
    return ultraStream->autocalDeviation;
}

/**
 * UltraStreamGetPRinsingON:
 * @ultra_stream: a #UltraStream
 *
 * Get process rinsing
 *
 * Return:  TRUE if process rinsing activated
 */
gboolean UltraStreamGetPRinsingON(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, FALSE);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), FALSE);
    return ultraStream->prinsingON;
}

/**
 * UltraStreamGetPRinsingY1:
 * @ultra_stream: a #UltraStream
 *
 * process rinsing Y1  position
 *
 * Return:  position
 */
guint UltraStreamGetPRinsingY1(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, 0);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), 0);
    return ultraStream->prinsingY1;
}

/**
 * UltraStreamGetPRinsingY2:
 * @ultra_stream: a #UltraStream
 *
 * process rinsing Y2  position
 *
 * Return:  position
 */
guint UltraStreamGetPRinsingY2(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, 0);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), 0);
    return ultraStream->prinsingY2;
}

/**
 * UltraStreamGetPRinsingInjVol:
 * @ultra_stream: a #UltraStream
 *
 * process rinsing injection volume
 *
 * Return:  volume
 */
guint UltraStreamGetPRinsingInjVol(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, 0);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), 0);
    return ultraStream->injVol;
}
/**
 * UltraStreamGetPRinsingInjReplicates:
 * @ultra_stream: a #UltraStream
 *
 * process rinsing injection replicates
 *
 * Return:  replicates
 */
guint UltraStreamGetPRinsingInjReplicates(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, 0);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), 0);
    return ultraStream->injRep;
}

/**
 * UltraStreamGetPrinsingWaitAfter:
 * @ultra_stream: a #UltraStream
 *
 * process rinsing  wait after time
 *
 * Return:   sec
 */
gdouble UltraStreamGetPrinsingWaitAfter(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, 0.0);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), 0.0);
    return ultraStream->waitAfter;
}
/**
 * UltraStreamGetTocChannel:
 * @ultra_stream: a #UltraStream
 *
 * TOC channel
 *
 * Return:   channel #Channel (not free )
 */
Channel *UltraStreamGetTocChannel(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, NULL);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), NULL);
    return ultraStream->TOC;
}
/**
 * UltraStreamGetTCChannel:
 * @ultra_stream: a #UltraStream
 *
 * TC channel
 *
 * Return:   channel #Channel (not free )
 */
Channel *UltraStreamGetTCChannel(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, NULL);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), NULL);
    return ultraStream->TC;
}
/**
 * UltraStreamGetTicChannel:
 * @ultra_stream: a #UltraStream
 *
 * TIC channel
 *
 * Return:   channel #Channel (not free )
 */
Channel *UltraStreamGetTicChannel(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, NULL);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), NULL);
    return ultraStream->TIC;
}
/**
 * UltraStreamGetTnbChannel:
 * @ultra_stream: a #UltraStream
 *
 * TNb channel
 *
 * Return:   channel #Channel (not free)
 */
Channel *UltraStreamGetTnbChannel(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, NULL);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), NULL);
    return ultraStream->TNb;
}
/**
 * UltraStreamGetCODoChannel:
 * @ultra_stream: a #UltraStream
 *
 * CODo channel
 *
 * Return:   channel #Channel (not free )
 */
Channel *UltraStreamGetCODoChannel(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, NULL);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), NULL);
    return ultraStream->CODo;
}
/**
 * UltraStreamGetVesselsPos:
 * @ultra_stream: a #UltraStream
 *
 * Vessels positions 
 *
 * Return:   pos #Posparm (not free )
 */
Posparm *UltraStreamGetVesselsPos(UltraStream *ultraStream)
{
    g_return_val_if_fail(ultraStream != NULL, NULL);
    g_return_val_if_fail(IS_ULTRA_STREAM(ultraStream), NULL);
    return ultraStream->pos;
}
