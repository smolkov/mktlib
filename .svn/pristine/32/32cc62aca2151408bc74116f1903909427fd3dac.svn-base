/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * larcalibration.c
 * Copyright (C) LAR 2017
 * autor Smolkov
 *
 */

#include "larcalibration.h"
#include "larsqlite.h"
enum
{
    PROP_0,
    PROP_PKEY,
    PROP_CALIBRATION_CHANNEL,
    PROP_CALIBRATION_STREAM,
    PROP_CALIBRATION_LEVEL,
    PROP_CALIBRATION_ACTIVATED,
    PROP_CALIBRATION_MAIN,
    PROP_CALIBRATION_SLOPE,
    PROP_CALIBRATION_INTERCEPT,
    PROP_CALIBRATION_DETERMINATION,
    PROP_CALIBRATION_CV,
    PROP_CALIBRATION_DEVIATION,
    PROP_CALIBRATION_CREATED,
    PROP_CALIBRATION_DATE,

};

struct _LarCalibrationPrivate
{
    guint64 key;
    guint level;
    guint64 channel;
    guint64 stream;
    gboolean activated;
    gboolean main;
    gdouble slope;
    gdouble intercept;
    gdouble determination;
    gdouble cv;
    gdouble deviation;
    gint64 created;
    gchar *date;
};

#define LAR_CALIBRATION_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_LAR_CALIBRATION, LarCalibrationPrivate))

G_DEFINE_TYPE(LarCalibration, lar_calibration, G_TYPE_OBJECT)

static void
lar_calibration_init(LarCalibration *lar_calibration)
{
    LarCalibrationPrivate *priv = LAR_CALIBRATION_PRIVATE(lar_calibration);
    priv->key = 0;
    priv->channel = 0;
    priv->stream = 0;
    priv->activated = FALSE;
    priv->main = FALSE;
    priv->slope = 0.;
    priv->intercept = 0.;
    priv->cv = 0.;
    priv->deviation = 0.;
    GDateTime *dt  = g_date_time_new_now_utc();
    priv->created  = g_date_time_to_unix(dt);
    g_date_time_unref(dt);
    priv->date = f_LarTimeNowSqliteFormat();
    
    lar_calibration->priv = priv;
}

static void
lar_calibration_finalize(GObject *object)
{
    LarCalibration *data = LAR_CALIBRATION(object);
    if (data->priv->date)
        g_free(data->priv->date);
    G_OBJECT_CLASS(lar_calibration_parent_class)->finalize(object);
}

static void
lar_calibration_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_LAR_CALIBRATION(object));
    LarCalibration *cal = LAR_CALIBRATION(object);
    switch (prop_id)
    {
    case PROP_PKEY:
        cal->priv->key = g_value_get_uint64(value);
        break;
    case PROP_CALIBRATION_LEVEL:
        cal->priv->level = g_value_get_uint(value);
        break;
    case PROP_CALIBRATION_CHANNEL:
        cal->priv->channel = g_value_get_uint64(value);
        break;
    case PROP_CALIBRATION_STREAM:
        cal->priv->stream = g_value_get_uint64(value);
        break;
    case PROP_CALIBRATION_ACTIVATED:
        cal->priv->activated = g_value_get_boolean(value);
        break;
    case PROP_CALIBRATION_MAIN:
        cal->priv->main = g_value_get_boolean(value);
        break;
    case PROP_CALIBRATION_SLOPE:
        cal->priv->slope = g_value_get_double(value);
        break;
    case PROP_CALIBRATION_INTERCEPT:
        cal->priv->intercept = g_value_get_double(value);
        break;
    case PROP_CALIBRATION_DETERMINATION:
        cal->priv->determination = g_value_get_double(value);
        break;
    case PROP_CALIBRATION_CV:
        cal->priv->cv = g_value_get_double(value);
        break;
    case PROP_CALIBRATION_DEVIATION:
        cal->priv->deviation = g_value_get_double(value);
        break;
    case PROP_CALIBRATION_CREATED:
        cal->priv->created = g_value_get_int64(value);
        break;
    case PROP_CALIBRATION_DATE:
        if (cal->priv->date)
            g_free(cal->priv->date);
        cal->priv->date = g_value_dup_string(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void
lar_calibration_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_LAR_CALIBRATION(object));
    LarCalibration *cal = LAR_CALIBRATION(object);
    switch (prop_id)
    {
    case PROP_PKEY:
        g_value_set_uint64(value, cal->priv->key);
        break;
    case PROP_CALIBRATION_LEVEL:
        g_value_set_uint(value, cal->priv->level);
        break;
    case PROP_CALIBRATION_CHANNEL:
        g_value_set_uint64(value, cal->priv->channel);
        break;
    case PROP_CALIBRATION_STREAM:
        g_value_set_uint64(value, cal->priv->stream);
        break;
    case PROP_CALIBRATION_ACTIVATED:
        g_value_set_boolean(value, cal->priv->activated);
        break;
    case PROP_CALIBRATION_MAIN:
        g_value_set_boolean(value, cal->priv->main);
        break;
    case PROP_CALIBRATION_SLOPE:
        g_value_set_double(value, cal->priv->slope);
        break;
    case PROP_CALIBRATION_INTERCEPT:
        g_value_set_double(value, cal->priv->intercept);
        break;
    case PROP_CALIBRATION_DETERMINATION:
        g_value_set_double(value, cal->priv->determination);
        break;
    case PROP_CALIBRATION_CV:
        g_value_set_double(value, cal->priv->cv);
        break;
    case PROP_CALIBRATION_DEVIATION:
        g_value_set_double(value, cal->priv->deviation);
        break;
    case PROP_CALIBRATION_CREATED:
        g_value_set_int64(value, cal->priv->created);
        break;
    case PROP_CALIBRATION_DATE:
        g_value_set_string(value, cal->priv->date);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void
lar_calibration_class_init(LarCalibrationClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    //MktModelClass* parent_class = MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(LarCalibrationPrivate));
    object_class->finalize = lar_calibration_finalize;
    object_class->set_property = lar_calibration_set_property;
    object_class->get_property = lar_calibration_get_property;
    g_object_class_install_property(
        object_class, PROP_PKEY, g_param_spec_uint64(SQLITE_PK_PROPERTY, "Calibration primery key", "Calibration primery key", 0, G_MAXUINT64, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | SQLITE_DB_PRIMERY_KEY));

    g_object_class_install_property(object_class, PROP_CALIBRATION_LEVEL,
                                    g_param_spec_uint("level",
                                                      "Calibration property",
                                                      "Set get measurement property",
                                                      0, 4, 0,
                                                      G_PARAM_READABLE | G_PARAM_WRITABLE));

    g_object_class_install_property(object_class, PROP_CALIBRATION_STREAM,
                                    g_param_spec_uint64("stream",
                                                        "Calibration stream reference",
                                                        "Calibration stream reference",
                                                        0, G_MAXUINT64, 0,
                                                        G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_CALIBRATION_CHANNEL,
                                    g_param_spec_uint64("channel",
                                                        "Calibration channel reference",
                                                        "Calibration channel reference",
                                                        0, G_MAXUINT64, 0,
                                                        G_PARAM_READABLE | G_PARAM_WRITABLE));

    g_object_class_install_property(object_class, PROP_CALIBRATION_ACTIVATED,
                                    g_param_spec_boolean("activated",
                                                         "Calibration is activated",
                                                         "Set|Get calibration is activated",
                                                         FALSE,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_CALIBRATION_MAIN,
                                    g_param_spec_boolean("main",
                                                         "Calibration is main",
                                                         "Set|Get calibration is main",
                                                         FALSE,
                                                         G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_CALIBRATION_SLOPE,
                                    g_param_spec_double("slope",
                                                        "Calibration slope property",
                                                        "Set slope property",
                                                        -G_MAXDOUBLE, G_MAXDOUBLE, 1.0,
                                                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_CALIBRATION_INTERCEPT,
                                    g_param_spec_double("intercept",
                                                        "Calibration intercept property",
                                                        "Set intercept property",
                                                        -G_MAXDOUBLE, G_MAXDOUBLE, 0.0,
                                                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_CALIBRATION_DETERMINATION,
                                    g_param_spec_double("determination",
                                                        "Calibration determination property",
                                                        "Set determination property",
                                                        -G_MAXDOUBLE, G_MAXDOUBLE, 0.0,
                                                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_CALIBRATION_CV,
                                    g_param_spec_double("cv",
                                                        "Calibration intercept property",
                                                        "Set intercept property",
                                                        -G_MAXDOUBLE, G_MAXDOUBLE, 0.0,
                                                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_CALIBRATION_DEVIATION,
                                    g_param_spec_double("deviation",
                                                        "Calibration determination property",
                                                        "Set determination property",
                                                        -G_MAXDOUBLE, G_MAXDOUBLE, 0.0,
                                                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_CALIBRATION_CREATED,
                                    g_param_spec_double("created",
                                                        "Calibration real time",
                                                        "Calibration real time",
                                                        0, G_MAXDOUBLE, 0,
                                                        G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_CALIBRATION_DATE,
                                    g_param_spec_string("date",
                                                        "Calibration date ISO 8601",
                                                        "Set|Get date RFC 3339 encoded string ISO 8601",
                                                        "%Y.%m.%dT%H:%M:%S",
                                                        G_PARAM_READABLE | G_PARAM_WRITABLE));
}


