/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * ultracalibration.c
 * Copyright (C) LAR 2017
 *

 */

#include "ultracalibration.h"
#include "larsqlite.h"
#include <glib-object.h>
#include <math.h>
#include <json-glib/json-glib.h>
#include "ultrapoint.h"
// #include "value.h"

typedef struct _UltrapointsArrayBoxed UltrapointsArrayBoxed;

struct _UltrapointsArrayBoxed
{
    GSList *points;
};

/*** implementation ***/

UltrapointsArrayBoxed *upointsArrayBoxedNew()
{
    UltrapointsArrayBoxed *new = g_slice_new(UltrapointsArrayBoxed);
    new->points = NULL;
    return new;
}

static UltrapointsArrayBoxed *upointsArrayBoxedCopy(const UltrapointsArrayBoxed *src)
{
    UltrapointsArrayBoxed *copy = upointsArrayBoxedNew();
    copy->points = NULL;
    if (src != NULL && src->points != NULL)
    {
        GSList *l = src->points;
        for (; l != NULL; l = l->next)
        {
            if (IS_ULTRA_POINT(l->data))
            {
                copy->points = g_slist_append(copy->points, g_object_ref(l->data));
            }
        }
    }
    // g_print("upointsArrayBoxedCopy %p\n",copy);
    return copy;
}

static void upointsArrayBoxedFree(UltrapointsArrayBoxed *boxed)
{
    if (G_LIKELY(boxed))
    {
        if (boxed->points != NULL)
        {
            if (boxed->points->data != NULL && IS_ULTRA_POINT(boxed->points->data))
                g_slist_free_full(boxed->points, g_object_unref);
            else
                g_slist_free(boxed->points);
        }
        g_slice_free(UltrapointsArrayBoxed, boxed);
    }
}

static JsonNode *upointsArrayBoxedSerialize(gconstpointer boxed)
{
    const UltrapointsArrayBoxed *upointsArray = boxed;
    JsonArray *array;
    JsonNode *node;

    if (boxed == NULL)
        return json_node_new(JSON_NODE_NULL);
    array = json_array_new();
    node = json_node_new(JSON_NODE_ARRAY);
    GSList *ld = NULL;
    for (ld = upointsArray->points; ld != NULL; ld = ld->next)
    {
        if (IS_ULTRA_POINT(ld->data))
        {
            JsonNode *node = json_gobject_serialize(G_OBJECT(ld->data));
            if (node)
                json_array_add_element(array, node);
        }
    }
    json_node_take_array(node, array);
    return node;
}

static gpointer upointsArrayBoxedDeserialize(JsonNode *node)
{
    JsonArray *array;
    UltrapointsArrayBoxed *upointsArray;

    if (json_node_get_node_type(node) != JSON_NODE_ARRAY)
        return NULL;

    array = json_node_get_array(node);

    upointsArray = upointsArrayBoxedNew();
    GList *elements = json_array_get_elements(array);
    GList *el = elements;
    for (; el != NULL; el = el->next)
    {
        if (JSON_NODE_HOLDS_OBJECT((JsonNode *)(el->data)))
        {
            GObject *object = json_gobject_deserialize(TYPE_ULTRA_POINT, (JsonNode *)(el->data));
            if (object &&IS_ULTRA_POINT(object))
            {
                upointsArray->points = g_slist_append(upointsArray->points, object);
            }
        }
    }
    if (elements)
        g_list_free(elements);
    return upointsArray;
}

GType upointsarray_boxed_get_type(void)
{
    static GType b_type = 0;
    if (G_UNLIKELY(b_type == 0))
    {
        b_type = g_boxed_type_register_static("UltrapointsArrayBoxed", (GBoxedCopyFunc)upointsArrayBoxedCopy, (GBoxedFreeFunc)upointsArrayBoxedFree);
        json_boxed_register_serialize_func(b_type, JSON_NODE_ARRAY, upointsArrayBoxedSerialize);
        json_boxed_register_deserialize_func(b_type, JSON_NODE_ARRAY, upointsArrayBoxedDeserialize);
    }
    return b_type;
}

gboolean upointsArrayAddObject(UltrapointsArrayBoxed *boxed, UltraPoint *newPoint)
{
    g_return_val_if_fail(boxed != NULL, FALSE);
    g_return_val_if_fail(newPoint!= NULL, FALSE);
    g_return_val_if_fail(IS_ULTRAMEAS(newPoint), FALSE);
    boxed->points = g_slist_append(boxed->points, newPoint);
    return TRUE;
}

static GSList *upointsArrayGetDatas(UltrapointsArrayBoxed *boxed)
{
    g_return_val_if_fail(boxed != NULL, NULL);
    return boxed->points;
}


static UltraPoint* upointsArrayGetLastPoint(UltrapointsArrayBoxed *boxed){
    if(boxed->points==NULL)return NULL;
    GSList *last = g_slist_last(boxed->points);
    UltraPoint *lastPoint=NULL;    
    if(last){
        lastPoint = ULTRA_POINT(last->data);
    }
    return lastPoint;
}
#define UPOINTS_TYPE_BOXED (upointsarray_boxed_get_type())

enum
{
    PROP_0,
    PROP_POINTS,

};

struct _UltraCalibrationPrivate
{
    Statistic *statistic;
    UltraPoint *currPoint;    
    UltrapointsArrayBoxed *points;
};

#define ULTRA_CALIBRATION_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_ULTRA_CALIBRATION, UltraCalibrationPrivate))

G_DEFINE_TYPE(UltraCalibration, ultra_calibration, TYPE_LAR_CALIBRATION)

static void ultra_calibration_init(UltraCalibration *ultra_calibration)
{
    UltraCalibrationPrivate *priv = ULTRA_CALIBRATION_PRIVATE(ultra_calibration);
    priv->points = upointsArrayBoxedNew();
    priv->currPoint = NULL;
    ultra_calibration->priv = priv;
}

static void ultra_calibration_finalize(GObject *object)
{
    UltraCalibration *calibration = ULTRA_CALIBRATION(object);
    if (calibration->priv->points)
        upointsArrayBoxedFree(calibration->priv->points);
    if (calibration->priv->statistic)
        g_object_unref(calibration->priv->statistic);

    G_OBJECT_CLASS(ultra_calibration_parent_class)->finalize(object);
}

static void ultra_calibration_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_ULTRA_CALIBRATION(object));
    UltraCalibration *calibration = ULTRA_CALIBRATION(object);
    switch (prop_id)
    {
    case PROP_POINTS:
        if (calibration->priv->points)
            upointsArrayBoxedFree(calibration->priv->points);
        calibration->priv->points = upointsArrayBoxedCopy(g_value_get_boxed(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void ultra_calibration_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_ULTRA_CALIBRATION(object));
    UltraCalibration *calibration = ULTRA_CALIBRATION(object);
    switch (prop_id)
    {
    case PROP_POINTS:
        g_value_set_boxed(value, calibration->priv->points);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void ultra_calibration_class_init(UltraCalibrationClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // ModelClass* parent_class = MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(UltraCalibrationPrivate));
    object_class->finalize = ultra_calibration_finalize;
    object_class->set_property = ultra_calibration_set_property;
    object_class->get_property = ultra_calibration_get_property;
    g_object_class_install_property(object_class, PROP_POINTS, g_param_spec_boxed("calibrations", "calibrations replicates", "calibrations replicates", UPOINTS_TYPE_BOXED, G_PARAM_READABLE | G_PARAM_WRITABLE));
}
/**
 * UltraCalibrationGetCalibration:
 * @ultra_calibration: a #UltraCalibration
 *
 * result calibrations calibration
 *
 * Return:  GSList of a #Ultrapoints objects.
 */
GSList *m_UltraCalibrationGetPoints(UltraCalibration *calibration)
{
    g_return_val_if_fail(calibration!= NULL, NULL);
    g_return_val_if_fail(IS_ULTRA_CALIBRATION(calibration), NULL);
    return upointsArrayGetDatas(calibration->priv->points);
}

/**
 * m_UltraCalibrationAddMeas:
 * @calibration: a #UltraCalibration
 * @points: a #Ultrapoints no add ref count.
 *
 * Return:  TRUE if done.
 */
gboolean m_UltraCalibrationAddMeas(UltraCalibration *calibration, Ultrameas *meas)
{
    g_return_val_if_fail(calibration!= NULL, FALSE);
    g_return_val_if_fail(IS_ULTRA_CALIBRATION(calibration), FALSE);
   return m_UltraPointAddMeas(upointsArrayGetLastPoint(calibration->priv->points), meas);
}

/**
 * m_UltraCalibrationCalculate:
 * @calibration: a #UltraCalibration
 *
 *
 * Return:  TRUE if calculation done.
 */
gboolean m_UltraCalibrationCalculateCurrentPoint(UltraCalibration *calibration)
{
    g_return_val_if_fail(calibration != NULL, FALSE);
    g_return_val_if_fail(IS_ULTRA_CALIBRATION(calibration), FALSE);
    return m_UltraPointCalculate(upointsArrayGetLastPoint(calibration->priv->points)); 
}
void m_UltraCalibrationCalculate(UltraCalibration *calibration)
{
    g_return_if_fail(calibration != NULL);
    g_return_if_fail(IS_ULTRA_CALIBRATION(calibration));
    GSList *points = upointsArrayGetDatas(calibration->priv->points);
	g_return_if_fail(points!=NULL);
    #define _AVERAGE  0.00000000000000001

	guint   c    = 0.;
	gdouble xm   = 0.;
	gdouble ym   = 0.;
	gdouble xx   = 0.;
	gdouble yy   = 0.;
	gdouble xym  = 0.;
	gdouble xx2  = 0.;
	gdouble yy2  = 0.;
	gdouble r    = 0.;
	gdouble rr   = 0.;
	gdouble b = 1.0,a=0.0;
	gdouble average  = 0.0;
	gdouble solution = 0.0;
	if(g_slist_length(points)>1)
	{
		GSList *l    = NULL;

		for(l=points;l!=NULL;l=l->next)
		{
			m_UltraPointCalculate(ULTRA_POINT(l->data));
			c++;
			average  = m_LarPointGetAverage(LAR_POINT(l->data))+_AVERAGE;
			solution = m_LarPointGetSolution(LAR_POINT(l->data));
			ym +=average;
			xm +=solution;
		}
		ym = ym/c;
		xm = xm/c;
		for(l=points;l!=NULL;l=l->next)
		{
			average  = m_LarPointGetAverage(LAR_POINT(l->data))+_AVERAGE;
			solution = m_LarPointGetSolution(LAR_POINT(l->data));
			xx += (solution-xm);
			yy += (average-ym);
			xym+= (solution-xm)*(average-ym);
			xx2+= (solution-xm)*(solution-xm);
			yy2+= (average-ym)*(average-ym);
		}
		//g_debug("TEST ---------------------------------------------------------MITTELWERT=%f",mittelwert);
		r  = xym/sqrt(xx2*yy2);
		rr = r*r;

		b = xym/xx2;
		a = ym-b*xm;
	}
	else
	{
		average  = m_LarPointGetAverage(LAR_POINT(points->data))+_AVERAGE;
		solution =m_LarPointGetSolution(LAR_POINT(points->data));
		a = 0;
		b = average/solution;
	}
	g_object_set(calibration,"slope",b,"intercept",a,"determination",rr,NULL);
   #undef _AVERAGE 
}