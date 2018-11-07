/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * vessel.c
 * Copyright (C) LAR 2017
 *
 */

#include "vessel.h"

enum {
    PROP_0,
    PROP_VESSEL_X,
    PROP_VESSEL_Y,
    PROP_VESSEL_Y1,
    PROP_VESSEL_Y2,
    PROP_VESSEL_Y3,
    PROP_VESSEL_HAS_MOTOR

};

struct _VesselPrivate {
    gint    X;
    gint    Y;
    gint    Y1;
    gint    Y2;
    gint    Y3;
    gboolean hasMotor;

    // Only nitri parameter
};

struct _VesselClass {
    GObjectClass parent_class;
};

struct _Vessel {
    GObject        parent_instance;
    VesselPrivate *priv;
};

#define VESSEL_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_VESSEL, VesselPrivate))

G_DEFINE_TYPE(Vessel, vessel, G_TYPE_OBJECT)

static void vessel_init(Vessel *vessel) {
    VesselPrivate *priv = VESSEL_PRIVATE(vessel);
    priv->X             = 0;
    priv->Y1            = 0;
    priv->Y2            = 0;
    priv->Y3            = 0;
    vessel->priv        = priv;
    /* TODO: Add initialization code here */
}

static void vessel_finalize(GObject *object) {
    // Vessel *data = VESSEL(object);
    G_OBJECT_CLASS(vessel_parent_class)->finalize(object);
}

static void vessel_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_VESSEL(object));
    Vessel *vessel = VESSEL(object);
    switch (prop_id) {

    case PROP_VESSEL_X:
        vessel->priv->X = g_value_get_int(value);
        break;
    case PROP_VESSEL_Y:
        vessel->priv->Y = g_value_get_int(value);
        break;
    case PROP_VESSEL_Y1:
        vessel->priv->Y1 = g_value_get_int(value);
        break;
    case PROP_VESSEL_Y2:
        vessel->priv->Y2 = g_value_get_int(value);
        break;
    case PROP_VESSEL_Y3:
        vessel->priv->Y3 = g_value_get_int(value);
        break;
    case PROP_VESSEL_HAS_MOTOR:
        vessel->priv->hasMotor = g_value_get_boolean(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void vessel_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_VESSEL(object));
    Vessel *vessel = VESSEL(object);
    switch (prop_id) {
    case PROP_VESSEL_X:
        g_value_set_int(value, vessel->priv->X);
        break;
    case PROP_VESSEL_Y:
        g_value_set_int(value, vessel->priv->Y);
        break;
    case PROP_VESSEL_Y1:
        g_value_set_int(value, vessel->priv->Y1);
        break;
    case PROP_VESSEL_Y2:
        g_value_set_int(value, vessel->priv->Y2);
        break;
    case PROP_VESSEL_Y3:
        g_value_set_int(value, vessel->priv->Y3);
        break;
    case PROP_VESSEL_HAS_MOTOR:
        g_value_set_boolean(value, vessel->priv->hasMotor);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void vessel_class_init(VesselClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // Class* parent_class = MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(VesselPrivate));
    object_class->finalize     = vessel_finalize;
    object_class->set_property = vessel_set_property;
    object_class->get_property = vessel_get_property;

    g_object_class_install_property(object_class, PROP_VESSEL_X, g_param_spec_int("xpos", "X-Axis position", "Set get X-Axis position", 1, 16000, 1200,
                                                                G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT ));

    g_object_class_install_property(object_class, PROP_VESSEL_Y, g_param_spec_int("ypos", "Y-Axis injection position", "Set get Y-Axis injection position", 10, 16000, 1250,
                                                                G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT ));

    g_object_class_install_property(object_class, PROP_VESSEL_Y1, g_param_spec_int("y1pos", "Y-Axis needle position", "Set get Y-Axis needle position", 0, 16000, 0,
                                                                 G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT ));

    g_object_class_install_property(object_class, PROP_VESSEL_Y2, g_param_spec_int("y2pos", "Y-Axis 2 position", "Set get Y-Axis 2 position", 0, 16000, 0,
                                                                 G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT ));

    g_object_class_install_property(object_class, PROP_VESSEL_Y3, g_param_spec_int("y3pos", "Y-Axis 3 position", "Set get Y-Axis 3 position", 0, 16000, 0,
                                                                 G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT ));

    g_object_class_install_property(object_class, PROP_VESSEL_HAS_MOTOR, g_param_spec_boolean("hasmotor", "with motor", "Set get with motor", FALSE,
                                                 G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT ));
}


/**
 * VesselGetXPos:
 * @vessel: a #Vessel
 *
 * Get vessel x position.
 *
 * Returns: x position as int
 */

gint VesselGetXPos(Vessel *vessel){
    g_return_val_if_fail(vessel != NULL,0);
    g_return_val_if_fail(IS_VESSEL(vessel),0);
    return vessel->priv->X;
}
/**
 * VesselGetYPos:
 * @vessel: a #Vessel
 *
 * Get vessel y position.
 *
 * Returns: y position as int
 */

gint VesselGetYPos(Vessel *vessel){
    g_return_val_if_fail(vessel != NULL,0);
    g_return_val_if_fail(IS_VESSEL(vessel),0);
    return vessel->priv->Y;
}
/**
 * VesselGetY1Pos:
 * @vessel: a #Vessel
 *
 * Get vessel y1 position.
 *
 * Returns: y1 position as int
 */

gint VesselGetY1Pos(Vessel *vessel){
    g_return_val_if_fail(vessel != NULL,0);
    g_return_val_if_fail(IS_VESSEL(vessel),0);
    return vessel->priv->Y1;
}
/**
 * VesselGetY2Pos:
 * @vessel: a #Vessel
 *
 * Get vessel y2 position.
 *
 * Returns: y2 position as int
 */

gint VesselGetY2Pos(Vessel *vessel){
    g_return_val_if_fail(vessel != NULL,0);
    g_return_val_if_fail(IS_VESSEL(vessel),0);
    return vessel->priv->Y2;
}
/**
 * VesselGetY3Pos:
 * @vessel: a #Vessel
 *
 * Get vessel y3 position.
 *
 * Returns: y3 position as int
 */

gint VesselGetY3Pos(Vessel *vessel){
    g_return_val_if_fail(vessel != NULL,0);
    g_return_val_if_fail(IS_VESSEL(vessel),0);
    return vessel->priv->Y3;
}

/**
 * VesselGetHasMotor:
 * @vessel: a #Vessel
 *
 * Get vessel has motor.
 *
 * Returns: TRUE if vessel with motor
 */

 gboolean VesselGetHasMotor(Vessel *vessel){
    g_return_val_if_fail(vessel != NULL,FALSE);
    g_return_val_if_fail(IS_VESSEL(vessel),FALSE);
    return vessel->priv->hasMotor;
}

