/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * ultrapoint.c
 * Copyright (C) LAR 2017
 *

 */

#include "ultrapoint.h"
#include "ultrameasurement.h"
#include "larsqlite.h"
#include <glib-object.h>
#include <math.h>
#include <json-glib/json-glib.h>

// #include "value.h"

typedef struct _UltrapointsArrayBoxed UltrapointsArrayBoxed;

struct _UltrapointsArrayBoxed
{
    GSList *meas;
};

enum
{
    PROP_0,
    PROP_MEAS,

};

struct _UltraPointPrivate
{
    UmeasArrayBoxed *meas;
    Statistic *statistic;
};

#define ULTRA_POINT_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_ULTRA_POINT, UltraPointPrivate))

G_DEFINE_TYPE(UltraPoint, ultra_point, TYPE_LAR_POINT)

static void ultra_point_init(UltraPoint *ultra_point)
{
    UltraPointPrivate *priv = ULTRA_POINT_PRIVATE(ultra_point);
    priv->meas = UmeasArrayBoxedNew();
    ultra_point->priv = priv;
}

static void ultra_point_finalize(GObject *object)
{
    UltraPoint *upoint = ULTRA_POINT(object);
    if (upoint->priv->meas)
        UmeasArrayBoxedFree(upoint->priv->meas);
    G_OBJECT_CLASS(ultra_point_parent_class)->finalize(object);
}

static void ultra_point_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_ULTRA_POINT(object));
    UltraPoint *upoint = ULTRA_POINT(object);
    switch (prop_id)
    {
    case PROP_MEAS:
        if (upoint->priv->meas)
            UmeasArrayBoxedFree(upoint->priv->meas);
        upoint->priv->meas = UmeasArrayBoxedCopy(g_value_get_boxed(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void ultra_point_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_ULTRA_POINT(object));
    UltraPoint *upoint = ULTRA_POINT(object);
    switch (prop_id)
    {
    case PROP_MEAS:
        g_value_set_boxed(value, upoint->priv->meas);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void ultra_point_class_init(UltraPointClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // ModelClass* parent_class = MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(UltraPointPrivate));
    object_class->finalize = ultra_point_finalize;
    object_class->set_property = ultra_point_set_property;
    object_class->get_property = ultra_point_get_property;

    g_object_class_install_property(object_class, PROP_MEAS, g_param_spec_boxed("measurements", "points replicates", "points replicates", UMEASARRAY_TYPE_BOXED, G_PARAM_READABLE | G_PARAM_WRITABLE));
}

/**
 * m_UltraPointGetMeasurements:
 * @ultra_point: a #UltraPoint
 *
 * result points upoint
 *
 * Return:  GSList of a #Ultrapoints objects.
 */
GSList *m_UltraPointGetMeasurements(UltraPoint *ultraPoint)
{
    g_return_val_if_fail(ultraPoint != NULL, NULL);
    g_return_val_if_fail(IS_ULTRA_POINT(ultraPoint), NULL);
    if (ultraPoint->priv->meas == NULL)
        return NULL;
    return UmeasArrayGetDatas(ultraPoint->priv->meas);
}

/**
 * m_UltraPointAddMeas:
 * @ultraPoint: a #UltraPoint
 * @points: a #Ultrapoints no add ref count.
 *
 * Return:  TRUE if done.
 */
gboolean m_UltraPointAddMeas(UltraPoint *ultraPoint, Ultrameas *meas)
{
    g_return_val_if_fail(ultraPoint != NULL, FALSE);
    g_return_val_if_fail(IS_ULTRA_POINT(ultraPoint), FALSE);
    if (UmeasArrayAddObject(ultraPoint->priv->meas, meas))
    {
        g_object_notify(G_OBJECT(ultraPoint), "measurements");
        return TRUE;
    }
    return FALSE;
}

/**
 * m_UltraPointCalculate:
 * @ultraPoint: a #UltraPoint
 * @maxcv: max cv
 * @outlies: outliers 
 *
 * Return:  TRUE if calculation done.
 */
gboolean m_UltraPointCalculate(UltraPoint *ultraPoint)
{
    g_return_val_if_fail(ultraPoint != NULL, FALSE);
    g_return_val_if_fail(IS_ULTRA_POINT(ultraPoint), FALSE);
    g_return_val_if_fail(ultraPoint->priv->statistic != NULL, FALSE);
    guint replicates = g_slist_length(UmeasArrayGetDatas(ultraPoint->priv->meas));
    guint outliers = StatisticGetOutliers(ultraPoint->priv->statistic); 
    gdouble maxcv = StatisticGetCV(ultraPoint->priv->statistic);
    gdouble cv = 0.0;
    gdouble value = 0.0;
    gboolean done = FALSE;
    if (replicates == 1) {
        GSList *first = UmeasArrayGetDatas(ultraPoint->priv->meas);
        if (first->data && IS_ULTRAMEAS(first->data))
        {
            value = m_UltrameasGetValue(ULTRAMEAS(first->data));
            g_object_set(G_OBJECT(ultraPoint), "average",value,NULL);
            g_object_set(G_OBJECT(ultraPoint), "cv",0.0,NULL);
        }
    } else {
        GSList *lm = UmeasArrayGetDatas(ultraPoint->priv->meas);
        for (; lm != NULL; lm = lm->next)
        {
            if (lm->data && IS_ULTRAMEAS(lm->data))
            {
                m_UltrameasSetOutlier(ULTRAMEAS(lm->data), FALSE);
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
            for (l = UmeasArrayGetDatas(ultraPoint->priv->meas); l != NULL; l = l->next)
            {
                if (!m_UltrameasIsOutlier(ULTRAMEAS(l->data)))
                {
                    raw += m_UltrameasGetValue(ULTRAMEAS(l->data));
                    currReplicate++;
                }
                else
                {
                    currOutlier++;
                }
            }
            raw /= currReplicate;
            sum_d_sqr = 0.;
            for (l = UmeasArrayGetDatas(ultraPoint->priv->meas); l != NULL; l = l->next)
                if (!m_UltrameasIsOutlier(ULTRAMEAS(l->data)))
                    sum_d_sqr += (m_UltrameasGetValue(ULTRAMEAS(l->data)) - raw) * (m_UltrameasGetValue(ULTRAMEAS(l->data)) - raw);

            standardDeviation = sqrt(sum_d_sqr / (currReplicate - 1));
            if (raw > 0.000000)
                cv = 100. * standardDeviation / fabs(raw);

            if (cv <= maxcv)
            {
                done = TRUE;
                break;
            }
            if (currOutlier >= outliers || currOutlier >= replicates - 1)
            {
                break;
            }
            max_d_sqr = G_MINDOUBLE;
            Ultrameas *nextOutlier = NULL;
            for (l = UmeasArrayGetDatas(ultraPoint->priv->meas); l != NULL; l = l->next) {
                if (!m_UltrameasIsOutlier(ULTRAMEAS(l->data))){
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
                m_UltrameasSetOutlier(nextOutlier, TRUE);
            }
        }
        g_object_set(ultraPoint,"average",raw,NULL);
        g_object_set(ultraPoint,"cv",cv,NULL);
    }
    return done;
}