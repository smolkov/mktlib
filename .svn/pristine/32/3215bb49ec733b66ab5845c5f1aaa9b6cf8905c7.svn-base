/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * larresult.c
 * Copyright (C) LAR 2017
 *

 */

#include "larresult.h"
#include "larsqlite.h"
#include "value.h"

struct _LarDatas
{
    GArray *X;
    GArray *Y;
    gdouble minX;
    gdouble maxX;
    gdouble minY;
    gdouble maxY;
};

/*** implementation ***/

LarDatas *LarDatasNew()
{
    LarDatas *new = g_slice_new(LarDatas);
    new->minX = G_MAXDOUBLE;
    new->maxX = -G_MAXDOUBLE;
    new->minY = G_MAXDOUBLE;
    new->maxY = -G_MAXDOUBLE;
    new->X = g_array_new(FALSE, FALSE, sizeof(gdouble));
    new->Y = g_array_new(FALSE, FALSE, sizeof(gdouble));
    return new;
}

LarDatas *m_LarDatasCopy(const LarDatas *src)
{
    LarDatas *copy = LarDatasNew();
    if (src != NULL && src->X != NULL && src->Y != NULL)
    {
        copy->minX = src->minX;
        copy->maxX = src->maxX;
        copy->minY = src->minY;
        copy->maxY = src->maxY;
        copy->X = g_array_append_vals(copy->X, src->X->data, src->X->len);
        copy->Y = g_array_append_vals(copy->Y, src->Y->data, src->Y->len);
    }
    return copy;
}

LarDatas *m_LarDatasConvertStep(const LarDatas *src)
{
    LarDatas *copy = LarDatasNew();
    if (src != NULL && src->X != NULL && src->Y != NULL)
    {
        copy->minX = src->minX;
        copy->maxX = src->maxX;
        copy->minY = src->minY;
        copy->maxY = src->maxY;
        gint i = 0;
        gdouble pre = 0.0;
        for (i = 0; i < src->X->len && i < src->Y->len; i++)
        {
            gdouble valX = g_array_index(src->X, gdouble, i);
            gdouble valY = g_array_index(src->Y, gdouble, i);
            if (i > 0)
            {
                g_array_append_val(copy->X, valX);
                g_array_append_val(copy->Y, pre);
            }
            g_array_append_val(copy->X, valX);
            g_array_append_val(copy->Y,valY);
            pre = valY;
        }
    }
    return copy;
}

void m_LarDatasFree(LarDatas *boxed)
{
    if (G_LIKELY(boxed))
    {
        if (boxed->X != NULL)
        {
            g_array_free(boxed->X, TRUE);
        }
        if (boxed->Y != NULL)
        {
            g_array_free(boxed->Y, TRUE);
        }
        g_slice_free(LarDatas, boxed);
    }
}

GType lar_datas_get_type(void)
{
    static GType b_type = 0;

    if (G_UNLIKELY(b_type == 0))
    {
        b_type = g_boxed_type_register_static("LarDatas", (GBoxedCopyFunc)m_LarDatasCopy, (GBoxedFreeFunc)m_LarDatasFree);
    }

    return b_type;
}

guint m_LarDatasGetLen(LarDatas *boxed)
{
    g_return_val_if_fail(boxed != NULL, 0);
    g_return_val_if_fail(boxed->X != NULL, 0);
    g_return_val_if_fail(boxed->Y != NULL, 0);
    return MIN(boxed->X->len, boxed->Y->len);
}

gint m_LarDatasAddValues(LarDatas *boxed, gdouble x, gdouble y)
{
    g_return_val_if_fail(boxed != NULL, 0);
    g_return_val_if_fail(boxed->X != NULL, 0);
    g_return_val_if_fail(boxed->Y != NULL, 0);
    if (boxed->minX > x)
        boxed->minX = x;
    if (boxed->maxX < x)
        boxed->maxX = x;
    if (boxed->minY > y)
        boxed->minY = y;
    if (boxed->maxY < y)
        boxed->maxY = y;
    // g_debug("%f %f %f %f (%f:%f)",boxed->minX,boxed->maxX,boxed->minY,boxed->maxY,x,y);
    g_array_append_val(boxed->X, x);
    g_array_append_val(boxed->Y, y);
    // g_return_val_if_fail(object != NULL, FALSE);
    // g_return_val_if_fail(G_IS_OBJECT(object), FALSE);
    // boxed->datas = g_slist_append(boxed->datas, g_object_ref(object));
    return boxed->X->len - 1;
}

GArray *m_LarDatasGetX(LarDatas *boxed)
{
    g_return_val_if_fail(boxed != NULL, NULL);
    return boxed->X;
}
GArray *m_LarDatasGetY(LarDatas *boxed)
{
    g_return_val_if_fail(boxed != NULL, NULL);
    return boxed->Y;
}

gdouble m_LarDatasGetMinX(LarDatas *boxed)
{
    g_return_val_if_fail(boxed != NULL, 0.0);
    return boxed->minX;
}
gdouble m_LarDatasGetMaxX(LarDatas *boxed)
{
    g_return_val_if_fail(boxed != NULL, 0.0);
    return boxed->maxX;
}

gdouble m_LarDatasGetMinY(LarDatas *boxed)
{
    g_return_val_if_fail(boxed != NULL, 0.0);
    return boxed->minY;
}
gdouble m_LarDatasGetMaxY(LarDatas *boxed)
{
    g_return_val_if_fail(boxed != NULL, 0.0);
    return boxed->maxY;
}
enum
{
    PROP_0,
    PROP_PKEY,
    PROP_CHANGED,
    PROP_VALUE,
    PROP_CV,
    PROP_CHANNEL,
    PROP_STREAM,
    PROP_NUMBER,
    PROP_TYPE,
    PROP_PARAMETER,
    PROP_NAME,
    PROP_UNIT,
    PROP_CREATED
};

struct _LarResultPrivate
{
    guint64 pkey;
    gint64 changed;
    gdouble value;
    gdouble cv;
    guint number;
    guint64 channel;
    guint64 stream;
    gchar *parameter;
    gchar *name;
    gchar *unit;
    gchar *created;
};

#define LAR_RESULT_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_LAR_RESULT, LarResultPrivate))

G_DEFINE_TYPE(LarResult, result_data, G_TYPE_OBJECT)

static void result_data_init(LarResult *result_data)
{
    LarResultPrivate *priv = LAR_RESULT_PRIVATE(result_data);
    GDateTime *dt = g_date_time_new_now_utc();
    priv->changed = g_date_time_to_unix(dt);
    priv->value = 0.0;
    priv->cv = 0.0;
    priv->pkey = 0;
    priv->number = 0;
    priv->channel = 0;
    priv->stream = 1;
    priv->parameter = g_strdup("value");
    priv->name = g_strdup("noname");
    priv->unit = g_strdup("mg/l");
    priv->created = f_LarTimeNowSqliteFormat();
    g_date_time_unref(dt);
    result_data->priv = priv;

    /* TODO: Add initialization code here */
}

static void result_data_finalize(GObject *object)
{
    LarResult *data = LAR_RESULT(object);
    if (data->priv->name)
        g_free(data->priv->name);
    if (data->priv->parameter)
        g_free(data->priv->parameter);
    if (data->priv->unit)
        g_free(data->priv->unit);
    if (data->priv->created)
        g_free(data->priv->created);

    G_OBJECT_CLASS(result_data_parent_class)->finalize(object);
}

static void result_data_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_LAR_RESULT(object));
    LarResult *data = LAR_RESULT(object);
    switch (prop_id)
    {
    case PROP_PKEY:
        data->priv->pkey = g_value_get_uint64(value);
        break;
    case PROP_CHANGED:
        data->priv->changed = g_value_get_int64(value);
        break;
    case PROP_VALUE:
        data->priv->value = g_value_get_double(value);
        break;
    case PROP_CV:
        data->priv->cv = g_value_get_double(value);
        break;
    case PROP_NUMBER:
        data->priv->number = g_value_get_uint(value);
        break;
    case PROP_CHANNEL:
        data->priv->channel = g_value_get_uint64(value);
        break;
    case PROP_STREAM:
        data->priv->stream = g_value_get_uint64(value);
        break;
    case PROP_PARAMETER:
        if (data->priv->parameter)
            g_free(data->priv->parameter);
        data->priv->parameter = g_value_dup_string(value);
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
    case PROP_CREATED:
        if (data->priv->created)
            g_free(data->priv->created);
        data->priv->created = g_value_dup_string(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void result_data_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_LAR_RESULT(object));
    LarResult *data = LAR_RESULT(object);
    switch (prop_id)
    {
    case PROP_PKEY:
        g_value_set_uint64(value, data->priv->pkey);
        break;
    case PROP_CHANGED:
        g_value_set_int64(value, data->priv->changed);
        break;
    case PROP_VALUE:
        g_value_set_double(value, data->priv->value);
        break;
    case PROP_CV:
        g_value_set_double(value, data->priv->cv);
        break;
    case PROP_CHANNEL:
        g_value_set_uint64(value, data->priv->channel);
        break;
    case PROP_STREAM:
        g_value_set_uint64(value, data->priv->stream);
        break;
    case PROP_NUMBER:
        g_value_set_uint(value, data->priv->number);
        break;
    case PROP_PARAMETER:
        g_value_set_string(value, data->priv->parameter);
        break;
    case PROP_NAME:
        g_value_set_string(value, data->priv->name);
        break;
    case PROP_UNIT:
        g_value_set_string(value, data->priv->unit);
        break;
    case PROP_CREATED:
        g_value_set_string(value, data->priv->created);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void result_data_class_init(LarResultClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // ModelClass* parent_class = MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(LarResultPrivate));
    object_class->finalize = result_data_finalize;
    object_class->set_property = result_data_set_property;
    object_class->get_property = result_data_get_property;
    g_object_class_install_property(
        object_class, PROP_PKEY, g_param_spec_uint64(SQLITE_PK_PROPERTY, "Result primery key", "Result primery key", 0, G_MAXUINT64, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | SQLITE_DB_PRIMERY_KEY));

    g_object_class_install_property(
        object_class, PROP_STREAM, g_param_spec_uint64("stream", "Result stream reference", "Result stream reference", 0, G_MAXUINT64, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_CHANNEL, g_param_spec_uint64("channel", "Result channel reference", "Result channel reference", 0, G_MAXUINT64, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_NUMBER, g_param_spec_uint("number", "Result trigger", "Set get mesurement trigger property", 0, G_MAXINT32, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_CHANGED, g_param_spec_int64("changed", "Create result time", "Set get create result time", 0, G_MAXINT64, 0, G_PARAM_READWRITE));

    g_object_class_install_property(
        object_class, PROP_VALUE, g_param_spec_double("value", "Result result property", "Set get sensor result property", 0., G_MAXDOUBLE, 0., G_PARAM_READABLE | G_PARAM_WRITABLE));

    g_object_class_install_property(
        object_class, PROP_CV, g_param_spec_double("cv", "Result result property", "Set get sensor result property", 0., G_MAXDOUBLE, 0., G_PARAM_READABLE | G_PARAM_WRITABLE));

    g_object_class_install_property(
        object_class, PROP_PARAMETER, g_param_spec_string("parameter", "Result parameter", "Result real measurement parameter", "fsr", G_PARAM_READABLE | G_PARAM_WRITABLE));

    g_object_class_install_property(object_class, PROP_NAME, g_param_spec_string("name", "Result name", "Result name", "noname", G_PARAM_READABLE | G_PARAM_WRITABLE));

    g_object_class_install_property(object_class, PROP_UNIT, g_param_spec_string("unit", "Result result outlier", "Set get sensor outlier property", "mg/l", G_PARAM_READABLE | G_PARAM_WRITABLE));

    g_object_class_install_property(
        object_class, PROP_CREATED, g_param_spec_string("created", "Result created date", "Property created date", "%Y.%m.%dT%H:%M:%S", G_PARAM_READABLE | G_PARAM_WRITABLE));
}

/**
 * m_LarResultGetKey:
 * @result: a #LarResult
 *
 * primery key.
 *
 * Return:  primery key #guint64
 */
guint64 m_LarResultGetKey(LarResult *result)
{
    g_return_val_if_fail(result != NULL, 0);
    g_return_val_if_fail(IS_LAR_RESULT(result), 0);
    return result->priv->pkey;
}
/**
 * m_LarResultGetStream:
 * @result: a #LarResult
 *
 * stream number .
 *
 * Return:  stream number #guint64
 */
guint64 m_LarResultGetStream(LarResult *result)
{
    g_return_val_if_fail(result != NULL, 0);
    g_return_val_if_fail(IS_LAR_RESULT(result), 0);
    return result->priv->stream;
}

/**
 * m_LarResultGetChannel:
 * @result: a #LarResult
 *
 * channel number.
 *
 * Return:  channel number #guint64
 */
guint64 m_LarResultGetChannel(LarResult *result)
{
    g_return_val_if_fail(result != NULL, 0);
    g_return_val_if_fail(IS_LAR_RESULT(result), 0);
    return result->priv->channel;
}
/**
 * m_LarResultGetNumber:
 * @result: a #LarResult
 *
 * result number.
 *
 * Return:  result number #guint
 */
guint m_LarResultGetNumber(LarResult *result)
{
    g_return_val_if_fail(result != NULL, 0);
    g_return_val_if_fail(IS_LAR_RESULT(result), 0);
    return result->priv->number;
}
/**
 * m_LarResultGetChanged:
 * @result: a #LarResult
 *
 * changed time in mictoseconds.
 *
 * Return:  the number of microseconds since January 1, 1970 UTC.  #guint64
 */
guint64 m_LarResultGetChanged(LarResult *result)
{
    g_return_val_if_fail(result != NULL, 0);
    g_return_val_if_fail(IS_LAR_RESULT(result), 0);
    return result->priv->changed;
}
/**
 * m_LarResultGetValue:
 * @result: a #LarResult
 *
 * result result
 *
 * Return:  result result
 */
gdouble m_LarResultGetValue(LarResult *result)
{
    g_return_val_if_fail(result != NULL, 0.0);
    g_return_val_if_fail(IS_LAR_RESULT(result), 0.0);
    return result->priv->value;
}

/**
 * m_LarResultGetCV:
 * @result: a #LarResult
 *
 * result cv
 *
 * Return:  result cv
 */
gdouble m_LarResultGetCV(LarResult *result)
{
    g_return_val_if_fail(result != NULL, 0.0);
    g_return_val_if_fail(IS_LAR_RESULT(result), 0.0);
    return result->priv->cv;
}
/**
 * m_LarResultGetParameter:
 * @result: a #LarResult
 *
 * result real measurement parameter name
 *
 * Return: parameter name
 */
const gchar *m_LarResultGetParameter(LarResult *result)
{
    g_return_val_if_fail(result != NULL, NULL);
    g_return_val_if_fail(IS_LAR_RESULT(result), NULL);
    return result->priv->parameter;
}
/**
 * m_LarResultGetName:
 * @result: a #LarResult
 *
 * result name
 *
 * Return:  result name
 */
const gchar *m_LarResultGetName(LarResult *result)
{
    g_return_val_if_fail(result != NULL, NULL);
    g_return_val_if_fail(IS_LAR_RESULT(result), NULL);
    return result->priv->name;
}
/**
 * m_LarResultGetUnit:
 * @result: a #LarResult
 *
 * result unit
 *
 * Return:  result unit
 */
const gchar *m_LarResultGetUnit(LarResult *result)
{
    g_return_val_if_fail(result != NULL, NULL);
    g_return_val_if_fail(IS_LAR_RESULT(result), NULL);
    return result->priv->unit;
}
/**
 * m_LarResultGetCreated:
 * @result: a #LarResult
 *
 * result created date
 *
 * Return:  result created date
 */
const gchar *m_LarResultGetCreated(LarResult *result)
{
    g_return_val_if_fail(result != NULL, NULL);
    g_return_val_if_fail(IS_LAR_RESULT(result), NULL);
    return result->priv->created;
}
