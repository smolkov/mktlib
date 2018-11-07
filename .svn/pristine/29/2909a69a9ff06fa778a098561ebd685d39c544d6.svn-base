/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * ultrameasurement.c
 * Copyright (C) LAR 2017
 *

 */

#include "ultrameasurement.h"
#include "larsqlite.h"
#include <glib-object.h>
#include <math.h>
#include <json-glib/json-glib.h>

// #include "value.h"


struct _UmeasArrayBoxed
{
    GSList *meas;
};

/*** implementation ***/

gpointer copyDatas(gconstpointer src, gpointer uresult)
{
    if (src != NULL && G_IS_OBJECT(src))
    {
        return g_object_ref(G_OBJECT(src));
    }
    return (gpointer)src;
}

UmeasArrayBoxed *UmeasArrayBoxedNew()
{
    UmeasArrayBoxed *new = g_slice_new(UmeasArrayBoxed);
    new->meas = NULL;
    return new;
}

UmeasArrayBoxed *UmeasArrayBoxedCopy(const UmeasArrayBoxed *src)
{
    UmeasArrayBoxed *copy = UmeasArrayBoxedNew();
    copy->meas = NULL;
    if (src != NULL && src->meas != NULL)
    {
        GSList *l = src->meas;
        for (; l != NULL; l = l->next)
        {
            if (IS_ULTRAMEAS(l->data))
            {
                copy->meas = g_slist_append(copy->meas, g_object_ref(l->data));
            }
        }
    }
    return copy;
}

void UmeasArrayBoxedFree(UmeasArrayBoxed *boxed)
{
    if (G_LIKELY(boxed))
    {
        if (boxed->meas != NULL)
        {
            if (boxed->meas->data != NULL && IS_ULTRAMEAS(boxed->meas->data))
                g_slist_free_full(boxed->meas, g_object_unref);
            else
                g_slist_free(boxed->meas);
        }
        g_slice_free(UmeasArrayBoxed, boxed);
    }
}

static JsonNode *umeasArrayBoxedSerialize(gconstpointer boxed)
{
    const UmeasArrayBoxed *umeasArray = boxed;
    JsonArray *array;
    JsonNode *node;

    if (boxed == NULL)
        return json_node_new(JSON_NODE_NULL);
    array = json_array_new();
    node = json_node_new(JSON_NODE_ARRAY);
    GSList *ld = NULL;
    for (ld = umeasArray->meas; ld != NULL; ld = ld->next)
    {
        if (IS_ULTRAMEAS(ld->data))
        {
            JsonNode *node = json_gobject_serialize(G_OBJECT(ld->data));
            if (node)
                json_array_add_element(array, node);
        }
    }
    json_node_take_array(node, array);
    return node;
}

static gpointer umeasArrayBoxedDeserialize(JsonNode *node)
{
    JsonArray *array;
    UmeasArrayBoxed *umeasArray;

    if (json_node_get_node_type(node) != JSON_NODE_ARRAY)
        return NULL;

    array = json_node_get_array(node);

    umeasArray = UmeasArrayBoxedNew();
    GList *elements = json_array_get_elements(array);
    GList *el = elements;
    for (; el != NULL; el = el->next)
    {
        if (JSON_NODE_HOLDS_OBJECT((JsonNode *)(el->data)))
        {
            GObject *object = json_gobject_deserialize(TYPE_ULTRAMEAS, (JsonNode *)(el->data));
            if (object && IS_ULTRAMEAS(object))
            {
                umeasArray->meas = g_slist_append(umeasArray->meas, object);
            }
        }
    }
    if(elements) g_list_free(elements);
    return umeasArray;
}

GType umeasarray_boxed_get_type(void)
{
    static GType b_type = 0;
    if (G_UNLIKELY(b_type == 0))
    {
        b_type = g_boxed_type_register_static("UmeasArrayBoxed", (GBoxedCopyFunc)UmeasArrayBoxedCopy, (GBoxedFreeFunc)UmeasArrayBoxedFree);
        json_boxed_register_serialize_func(b_type, JSON_NODE_ARRAY, umeasArrayBoxedSerialize);
        json_boxed_register_deserialize_func(b_type, JSON_NODE_ARRAY, umeasArrayBoxedDeserialize);
    }
    return b_type;
}
/*
 *
 * 
 * 
 */

gboolean UmeasArrayAddObject(UmeasArrayBoxed *boxed, Ultrameas *newMes)
{
    g_return_val_if_fail(boxed != NULL, FALSE);
    g_return_val_if_fail(newMes != NULL, FALSE);
    g_return_val_if_fail(IS_ULTRAMEAS(newMes), FALSE);
    boxed->meas = g_slist_append(boxed->meas, newMes);
    return TRUE;
}

GSList *UmeasArrayGetDatas(UmeasArrayBoxed *boxed)
{
    g_return_val_if_fail(boxed != NULL, NULL);
    return boxed->meas;
}

#define UMEASARRAY_TYPE_BOXED (umeasarray_boxed_get_type())

enum
{
    PROP_0,
    PROP_PKEY,
    PROP_CHANGED,
    PROP_VALUE,
    PROP_MAXCV,
    PROP_RESULT,
    PROP_CHANNEL,
    PROP_NUMBER,
    PROP_OUTLIERS,
    PROP_MEASUREMENTS,

};

struct _UltraMeasurementPrivate
{
    guint64 pkey;
    gint64 changed;
    gdouble value;
    gdouble maxcv;
    guint number;
    guint outliers;
    gdouble max_cv;
    guint64 result;
    guint64 channel;
    gboolean done;
    gdouble cv;
    UmeasArrayBoxed *measurements;
};

#define ULTRA_MEASUREMENT_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_ULTRA_MEASUREMENT, UltraMeasurementPrivate))

G_DEFINE_TYPE(UltraMeasurement, ultra_measurement, G_TYPE_OBJECT)

static void ultra_measurement_init(UltraMeasurement *ultra_measurement)
{
    UltraMeasurementPrivate *priv = ULTRA_MEASUREMENT_PRIVATE(ultra_measurement);
    GDateTime *dt  = g_date_time_new_now_utc();
    priv->measurements            = UmeasArrayBoxedNew();
    priv->changed                 = g_date_time_to_unix(dt);
    priv->outliers                = 0;
    priv->number                  = 0;
    ultra_measurement->priv       = priv;
    g_date_time_unref(dt);
}

static void ultra_measurement_finalize(GObject *object)
{
    UltraMeasurement *uresults = ULTRA_MEASUREMENT(object);
    if (uresults->priv->measurements)
        UmeasArrayBoxedFree(uresults->priv->measurements);
    G_OBJECT_CLASS(ultra_measurement_parent_class)->finalize(object);
}

static void ultra_measurement_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_ULTRA_MEASUREMENT(object));
    UltraMeasurement *uresult = ULTRA_MEASUREMENT(object);
    switch (prop_id)
    {
    case PROP_PKEY:
        uresult->priv->pkey = g_value_get_uint64(value);
        break;
    case PROP_CHANGED:
        uresult->priv->changed = g_value_get_int64(value);
        break;
    case PROP_VALUE:
        uresult->priv->value = g_value_get_double(value);
        break;
    case PROP_MAXCV:
        uresult->priv->maxcv = g_value_get_double(value);
        break;
    case PROP_RESULT:
        uresult->priv->result = g_value_get_uint64(value);
        break;
    case PROP_CHANNEL:
        uresult->priv->channel = g_value_get_uint64(value);
        break;
    case PROP_NUMBER:
        uresult->priv->number = g_value_get_uint(value);
        break;
    case PROP_OUTLIERS:
        uresult->priv->outliers = g_value_get_uint(value);
        break;
    case PROP_MEASUREMENTS:
        if (uresult->priv->measurements)
            UmeasArrayBoxedFree(uresult->priv->measurements);
        uresult->priv->measurements = UmeasArrayBoxedCopy(g_value_get_boxed(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void ultra_measurement_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_ULTRA_MEASUREMENT(object));
    UltraMeasurement *uresult = ULTRA_MEASUREMENT(object);
    switch (prop_id)
    {
    case PROP_PKEY:
        g_value_set_uint64(value, uresult->priv->pkey);
        break;
    case PROP_CHANGED:
        g_value_set_int64(value, uresult->priv->changed);
        break;
    case PROP_VALUE:
        g_value_set_double(value, uresult->priv->value);
        break;
    case PROP_MAXCV:
        g_value_set_double(value, uresult->priv->maxcv);
        break;
    case PROP_CHANNEL:
        g_value_set_uint64(value, uresult->priv->channel);
        break;
    case PROP_RESULT:
        g_value_set_uint64(value, uresult->priv->result);
        break;
    case PROP_NUMBER:
        g_value_set_uint(value, uresult->priv->number);
        break;
    case PROP_OUTLIERS:
        g_value_set_uint(value, uresult->priv->outliers);
        break;
    case PROP_MEASUREMENTS:
        g_value_set_boxed(value, uresult->priv->measurements);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void ultra_measurement_class_init(UltraMeasurementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // ModelClass* parent_class = MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(UltraMeasurementPrivate));
    object_class->finalize = ultra_measurement_finalize;
    object_class->set_property = ultra_measurement_set_property;
    object_class->get_property = ultra_measurement_get_property;
    g_object_class_install_property(
        object_class, PROP_PKEY, g_param_spec_uint64(SQLITE_PK_PROPERTY, "Result primery key", "Result primery key", 0, G_MAXUINT64, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | SQLITE_DB_PRIMERY_KEY));
    g_object_class_install_property(
        object_class, PROP_RESULT, g_param_spec_uint64("result", "Result stream reference", "Result stream reference", 0, G_MAXUINT64, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_CHANNEL, g_param_spec_uint64("channel", "Result channel reference", "Result channel reference", 0, G_MAXUINT64, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_NUMBER, g_param_spec_uint("number", "Result trigger", "Set get mesurement trigger property", 0, G_MAXINT32, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_OUTLIERS, g_param_spec_uint("outliers", "Outliers", "Set get outliers cout", 0, G_MAXINT32, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_MAXCV, g_param_spec_double("maxcv", "Max CV value", "Set get max cv value", 0., G_MAXDOUBLE, 2.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_CHANGED, g_param_spec_int64("changed", "Create result time", "Set get create result time", 0, G_MAXINT64, 0, G_PARAM_READWRITE));
    g_object_class_install_property(
        object_class, PROP_VALUE, g_param_spec_double("value", "Result property", "Set get result", 0., G_MAXDOUBLE, 0., G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_MEASUREMENTS, g_param_spec_boxed("measurements", "measurements replicates", "measurements replicates", UMEASARRAY_TYPE_BOXED, G_PARAM_READABLE | G_PARAM_WRITABLE));
}

/**
 * UltraMeasurementFromStatistic:
 * @statistic: a #Statistic
 *
 * result new measurement
 *
 * Return:  a new #UltraMeasurement object.
 */
UltraMeasurement * UltraMeasurementFromStatistic(guint64 channel, Statistic *statistic){
    g_return_val_if_fail(statistic != NULL, NULL);
    g_return_val_if_fail(IS_STATISTIC(statistic), NULL);
    UltraMeasurement *measurement = ULTRA_MEASUREMENT(g_object_new(TYPE_ULTRA_MEASUREMENT,"channel",channel,
    "number",StatisticGetMeasurementNumber(statistic),
    "outliers",StatisticGetOutliers(statistic),
    "maxcv",StatisticGetCV(statistic),NULL));
    return measurement;

}


/**
 * UltraMeasurementGetMeasurements:
 * @ultra_measurement: a #UltraMeasurement
 *
 * result measurements uresult
 *
 * Return:  GSList of a #Ultrameas objects.
 */
GSList *m_UltraMeasurementGetMeasurements(UltraMeasurement *ultraMeas)
{
    g_return_val_if_fail(ultraMeas != NULL, NULL);
    g_return_val_if_fail(IS_ULTRA_MEASUREMENT(ultraMeas), NULL);
    if (ultraMeas->priv->measurements == NULL)
        return NULL;
    return UmeasArrayGetDatas(ultraMeas->priv->measurements);
}

guint64 m_UltraMeasurementGetChannel(UltraMeasurement *ultraMeas)
{
    g_return_val_if_fail(ultraMeas != NULL, 0);
    g_return_val_if_fail(IS_ULTRA_MEASUREMENT(ultraMeas), 0);
    return ultraMeas->priv->channel;
}

guint64 m_UltraMeasurementGetResult(UltraMeasurement *ultraMeas)
{
    g_return_val_if_fail(ultraMeas != NULL, 0);
    g_return_val_if_fail(IS_ULTRA_MEASUREMENT(ultraMeas), 0);
    return ultraMeas->priv->result;
}

guint m_UltraMeasurementGetNumber(UltraMeasurement *ultraMeas)
{
    g_return_val_if_fail(ultraMeas != NULL, 0);
    g_return_val_if_fail(IS_ULTRA_MEASUREMENT(ultraMeas), 0);
    return ultraMeas->priv->number;
}
guint m_UltraMeasurementGetReplicates(UltraMeasurement *ultraMeas) {
    g_return_val_if_fail(ultraMeas != NULL, 0);
    g_return_val_if_fail(IS_ULTRA_MEASUREMENT(ultraMeas), 0);
    return g_slist_length(UmeasArrayGetDatas(ultraMeas->priv->measurements));
}

guint m_UltraMeasurementGetOutliers(UltraMeasurement *ultraMeas)
{
    g_return_val_if_fail(ultraMeas != NULL, 0);
    g_return_val_if_fail(IS_ULTRA_MEASUREMENT(ultraMeas), 0);
    return ultraMeas->priv->outliers;
}

gdouble m_UltraMeasurementGetValue(UltraMeasurement *ultraMeas)
{
    g_return_val_if_fail(ultraMeas != NULL, 0.0);
    g_return_val_if_fail(IS_ULTRA_MEASUREMENT(ultraMeas), 0.0);
    return ultraMeas->priv->value;
}

gdouble m_UltraMeasurementGetCv(UltraMeasurement *ultraMeas)
{
    g_return_val_if_fail(ultraMeas != NULL, 0.0);
    g_return_val_if_fail(IS_ULTRA_MEASUREMENT(ultraMeas), 0.0);
    return ultraMeas->priv->cv;
}

gdouble m_UltraMeasurementGetMaxCv(UltraMeasurement *ultraMeas){
    g_return_val_if_fail(ultraMeas != NULL, 0.0);
    g_return_val_if_fail(IS_ULTRA_MEASUREMENT(ultraMeas), 0.0);
    return ultraMeas->priv->maxcv;
}


/**
 * m_UltraMeasurementAddMeas:
 * @ultraMeas: a #UltraMeasurement
 * @meas: a #Ultrameas no add ref count.
 *
 * Return:  TRUE if done.
 */
gboolean m_UltraMeasurementAddMeas(UltraMeasurement *ultraMeas, Ultrameas *meas)
{
    g_return_val_if_fail(ultraMeas != NULL, FALSE);
    g_return_val_if_fail(IS_ULTRA_MEASUREMENT(ultraMeas), FALSE);
    if (UmeasArrayAddObject(ultraMeas->priv->measurements, meas))
    {
        g_object_notify(G_OBJECT(ultraMeas), "measurements");
        return TRUE;
    }
    return FALSE;
}

/**
 * m_UltraMeasurementCalculate:
 * @ultraMeas: a #UltraMeasurement
 *
 *
 * Return:  TRUE if calculation done.
 */
gboolean m_UltraMeasurementCalculate(UltraMeasurement *ultraMeas)
{
    g_return_val_if_fail(ultraMeas != NULL, FALSE);
    g_return_val_if_fail(IS_ULTRA_MEASUREMENT(ultraMeas), FALSE);
    guint replicates = m_UltraMeasurementGetReplicates(ultraMeas);
    gboolean done = FALSE;
    if(replicates == 1){
        GSList *first = UmeasArrayGetDatas(ultraMeas->priv->measurements);
        if (first->data && IS_ULTRAMEAS(first->data))
        {
            ultraMeas->priv->value = m_UltrameasGetValue(ULTRAMEAS(first->data));     
            g_object_notify(G_OBJECT(ultraMeas), "value");
        }
    }else {
        GSList *lm = UmeasArrayGetDatas(ultraMeas->priv->measurements);
        for (; lm != NULL; lm = lm->next)
        {
            if (lm->data && IS_ULTRAMEAS(lm->data))
            {
                m_UltrameasSetOutlier(ULTRAMEAS(lm->data),FALSE);
            }
        }
        gdouble raw = 0.;
        gdouble standardDeviation = 0.;
        gdouble max_d_sqr = 0., d_sqr = 0.;
        gdouble sum_d_sqr = 0.;
        guint currReplicate = 0;
        guint currOutlier = 0;
        while (!done)
        {
            currReplicate = 0;
            currOutlier = 0;
            // g_debug("START:Replicate %d",max_replicates);
            raw = 0.0;
            GSList *l = NULL;
            for (l = UmeasArrayGetDatas(ultraMeas->priv->measurements); l != NULL; l = l->next)
            {
                if (!m_UltrameasIsOutlier(ULTRAMEAS(l->data))) {
                    raw += m_UltrameasGetValue(ULTRAMEAS(l->data));
                    currReplicate++;
                } else {
                    currOutlier++;
                }
            }
            raw /= currReplicate;
            sum_d_sqr = 0.;
            for (l = UmeasArrayGetDatas(ultraMeas->priv->measurements); l != NULL; l = l->next)
                if (!m_UltrameasIsOutlier(ULTRAMEAS(l->data)))
                    sum_d_sqr += (m_UltrameasGetValue(ULTRAMEAS(l->data)) - raw) * (m_UltrameasGetValue(ULTRAMEAS(l->data)) - raw);
    
            standardDeviation = sqrt(sum_d_sqr / (currReplicate - 1));
            if (raw > 0.000000)
                ultraMeas->priv->cv = 100. * standardDeviation / fabs(raw);
    
            if( ultraMeas->priv->cv <= ultraMeas->priv->maxcv){
                done = TRUE;
                break;
            }
            if( currOutlier >= ultraMeas->priv->outliers || currOutlier >= replicates-1 ){
                break;
            }
            max_d_sqr = G_MINDOUBLE;
            Ultrameas *nextOutlier = NULL;
            for (l = UmeasArrayGetDatas(ultraMeas->priv->measurements); l != NULL; l = l->next)
            {
                if (!m_UltrameasIsOutlier(ULTRAMEAS(l->data)))
                {
                    gdouble val = m_UltrameasGetValue(ULTRAMEAS(l->data));
                    d_sqr = (val - raw) * (val - raw);
                    if (d_sqr > max_d_sqr || isnan(val) || isinf(val))
                    {
                        nextOutlier = ULTRAMEAS(l->data);
                        max_d_sqr = d_sqr;
                    }
                }
            }
            if (nextOutlier) { 
                m_UltrameasSetOutlier(nextOutlier,TRUE);
                // g_print("OUTLIER:%f\n",m_UltrameasGetValue(nextOutlier));
            }
        }
        ultraMeas->priv->value = raw;     
        g_object_notify(G_OBJECT(ultraMeas), "measurements");
        g_object_notify(G_OBJECT(ultraMeas), "value");
        // g_print("VAL:%f CV:%f\n",raw,ultraMeas->priv->cv);
    }
   
    return done;
}