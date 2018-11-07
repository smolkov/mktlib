/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * larpoint.h
 * Copyright (C) LAR 2017
 * autor Smolkov
 */

#include "larpoint.h"

enum
{
    PROP_0,
    PROP_CAL_POINT_SOLUTION,
    PROP_CAL_POINT_AVERAGE,
    PROP_CAL_POINT_CV
};

struct _LarPointPrivate
{
    gdouble solution;
    gdouble average;
    gdouble cv;
};

#define LAR_POINT_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_LAR_POINT, LarPointPrivate))

G_DEFINE_TYPE(LarPoint, lar_point, G_TYPE_OBJECT)

static void
lar_point_init(LarPoint *lar_point)
{
    LarPointPrivate *priv = LAR_POINT_PRIVATE(lar_point);
    priv->solution = 0.;
    priv->average = 0.;
    priv->cv = 0.;
    lar_point->priv = priv;
    /* TODO: Add initialization code here */
}

static void
lar_point_finalize(GObject *object)
{
    //LarPoint *data = LAR_POINT(object);
    G_OBJECT_CLASS(lar_point_parent_class)->finalize(object);
}

static void
lar_point_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_LAR_POINT(object));
    LarPoint *cal = LAR_POINT(object);
    switch (prop_id)
    {
    case PROP_CAL_POINT_SOLUTION:
        cal->priv->solution = g_value_get_double(value);
        break;
    case PROP_CAL_POINT_AVERAGE:
        cal->priv->average = g_value_get_double(value);
        break;
    case PROP_CAL_POINT_CV:
        cal->priv->cv = g_value_get_double(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void
lar_point_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_LAR_POINT(object));
    LarPoint *cal = LAR_POINT(object);
    switch (prop_id)
    {
    case PROP_CAL_POINT_SOLUTION:
        g_value_set_double(value, cal->priv->solution);
        break;
    case PROP_CAL_POINT_AVERAGE:
        g_value_set_double(value, cal->priv->average);
        break;
    case PROP_CAL_POINT_CV:
        g_value_set_double(value, cal->priv->cv);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void
lar_point_class_init(LarPointClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    g_type_class_add_private(klass, sizeof(LarPointPrivate));
    object_class->finalize = lar_point_finalize;
    object_class->set_property = lar_point_set_property;
    object_class->get_property = lar_point_get_property;

    g_object_class_install_property(object_class, PROP_CAL_POINT_SOLUTION,
                                        g_param_spec_double("solution",
                                                            "solution value",
                                                            "Set|Get cal point solution value",
                                                            0.000001, G_MAXDOUBLE, 500.0,
                                                            G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class,PROP_CAL_POINT_AVERAGE,
                                        g_param_spec_double("average",
                                                            "average value",
                                                            "Set|Get cal point average value",
                                                            0.0, G_MAXDOUBLE, 0.0,
                                                            G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_CAL_POINT_CV,
                                        g_param_spec_double("cv",
                                                            "cv value",
                                                            "Set|Get cal point cv value",
                                                            0.0, G_MAXDOUBLE, 0.0,
                                                            G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
}


gdouble m_LarPointGetSolution(LarPoint *point){
    g_return_val_if_fail(point!=NULL,0.0);
    g_return_val_if_fail(IS_LAR_POINT(point),0.0);
    return point->priv->solution;
}

gboolean m_LarPointGetAverage(LarPoint *point ){
    g_return_val_if_fail(point!=NULL,0.0);
    g_return_val_if_fail(IS_LAR_POINT(point),0.0);
    return point->priv->average;
}
gboolean m_LarPointGetCv(LarPoint *point ){
    g_return_val_if_fail(point!=NULL,0.0);
    g_return_val_if_fail(IS_LAR_POINT(point),0.0);
    return point->priv->cv;
}