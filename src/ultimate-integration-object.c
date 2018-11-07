/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mkt-integration-data.c
 * Copyright (C) sascha 2013 <sascha@sascha-VirtualBox>
 * 
mkt-integration-data.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * mkt-integration-data.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ultimate-integration-object.h"


#include "../config.h"


#include <glib/gi18n-lib.h>


enum
{
	PROP_0,
	PROP_ULTIMATE_INTEGRATION_JUSTIFACATION_TIME,
	PROP_ULTIMATE_INTEGRATION_START_THRESHOLD,
	PROP_ULTIMATE_INTEGRATION_STOP_THRESHOLD,
	PROP_ULTIMATE_INTEGRATION_T_START_MIN,
	PROP_ULTIMATE_INTEGRATION_T_STOP_MAX,
	PROP_ULTIMATE_INTEGRATION_T_STOP_MIN,
	PROP_ULTIMATE_INTEGRATION_TRIGGER,
	PROP_ULTIMATE_INTEGRATION_ACTIVATED,
};


struct _UltimateIntegrationObjectPrivate
{
	gboolean             integration_runs;
	gdouble              justification_time;
	gdouble              start_threshold;
	gdouble              stop_threshold;
	gdouble              t_start_min;
	gdouble              t_stop_max;
	gdouble              t_stop_min;
	guint                trigger;
	gboolean             activated;
};


#define ULTIMATE_INTEGRATION_OBJECT_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), ULTIMATE_TYPE_INTEGRATION_OBJECT, UltimateIntegrationObjectPrivate))



static gdouble
ultimate_integration_object_justification_time                          ( UltimateIntegration *uintegration )
{
	g_return_val_if_fail(uintegration != NULL,0.);
	g_return_val_if_fail(ULTIMATE_IS_INTEGRATION(uintegration),0.);
	return ULTIMATE_INTEGRATION_OBJECT(uintegration)->priv->justification_time;
}
static gdouble
ultimate_integration_object_start_threshold                          ( UltimateIntegration *uintegration )
{
	g_return_val_if_fail(uintegration != NULL,0.);
	g_return_val_if_fail(ULTIMATE_IS_INTEGRATION(uintegration),0.);
	return ULTIMATE_INTEGRATION_OBJECT(uintegration)->priv->start_threshold;
}

static gdouble
ultimate_integration_object_stop_threshold                          ( UltimateIntegration *uintegration )
{
	g_return_val_if_fail(uintegration != NULL,0.);
	g_return_val_if_fail(ULTIMATE_IS_INTEGRATION(uintegration),0.);
	return  ULTIMATE_INTEGRATION_OBJECT(uintegration)->priv->stop_threshold;
}

static gdouble
ultimate_integration_object_t_start_min                          ( UltimateIntegration *uintegration )
{
	g_return_val_if_fail(uintegration != NULL,0.);
	g_return_val_if_fail(ULTIMATE_IS_INTEGRATION(uintegration),0.);
	return ULTIMATE_INTEGRATION_OBJECT(uintegration)->priv->t_start_min;
}

static gdouble
ultimate_integration_object_t_stop_max                          ( UltimateIntegration *uintegration )
{
	g_return_val_if_fail(uintegration != NULL,0.);
	g_return_val_if_fail(ULTIMATE_IS_INTEGRATION(uintegration),0.);
	return ULTIMATE_INTEGRATION_OBJECT(uintegration)->priv->t_stop_max;
}

static gdouble
ultimate_integration_object_t_stop_min                          ( UltimateIntegration *uintegration )
{
	g_return_val_if_fail(uintegration != NULL,0.);
	g_return_val_if_fail(ULTIMATE_IS_INTEGRATION(uintegration),0.);
	return ULTIMATE_INTEGRATION_OBJECT(uintegration)->priv->t_stop_min;
}

static guint
ultimate_integration_object_trigger                          ( UltimateIntegration *uintegration )
{
	g_return_val_if_fail(uintegration != NULL,0);
	g_return_val_if_fail(ULTIMATE_IS_INTEGRATION(uintegration),0);
	return ULTIMATE_INTEGRATION_OBJECT(uintegration)->priv->trigger;
}

static void
ultimate_integration_object_init_integration_interface ( UltimateIntegrationInterface *iface )
{
	iface->justification_time=ultimate_integration_object_justification_time;
	iface->start_threshold=ultimate_integration_object_start_threshold;
	iface->stop_threshold=ultimate_integration_object_stop_threshold;
	iface->t_start_min=ultimate_integration_object_t_start_min;
	iface->t_stop_max=ultimate_integration_object_t_stop_max;
	iface->t_stop_min=ultimate_integration_object_t_stop_min;
	iface->trigger=ultimate_integration_object_trigger;
}



G_DEFINE_TYPE_WITH_CODE (UltimateIntegrationObject, ultimate_integration_object, MKT_TYPE_PARAM_OBJECT,
		                                         G_IMPLEMENT_INTERFACE (ULTIMATE_TYPE_INTEGRATION,
		                                		 ultimate_integration_object_init_integration_interface)
		                                		 MKT_CREATE_MODEL_FULL(mkt_connection_new_default_parameter(TRUE)))




static void
ultimate_integration_object_init (UltimateIntegrationObject *ultimate_integration_object)
{
	UltimateIntegrationObjectPrivate *priv      = ULTIMATE_INTEGRATION_OBJECT_PRIVATE(ultimate_integration_object);
	priv->justification_time  = 10.0;
	priv->integration_runs    = FALSE;
	priv->start_threshold     = 0.002;
	priv->stop_threshold      = 0.003;
	priv->t_start_min         = 0.0;
	priv->t_stop_max          = 120.;
	priv->t_stop_min          = 80.;
	priv->trigger             = 0;

	ultimate_integration_object->priv = priv;
	/* TODO: Add initialization code here */
}

static void
ultimate_integration_object_finalize (GObject *object)
{
	//UltimateIntegrationObject *data = ULTIMATE_INTEGRATION_OBJECT(object);
	G_OBJECT_CLASS (ultimate_integration_object_parent_class)->finalize (object);
}




static void
ultimate_integration_object_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (ULTIMATE_IS_INTEGRATION_OBJECT (object));
	UltimateIntegrationObject *integration = ULTIMATE_INTEGRATION_OBJECT(object);
	switch (prop_id)
	{
	case PROP_ULTIMATE_INTEGRATION_JUSTIFACATION_TIME:
		integration->priv->justification_time = g_value_get_double(value);
		break;
	case PROP_ULTIMATE_INTEGRATION_START_THRESHOLD:
		integration->priv->start_threshold = g_value_get_double(value);
		break;
	case PROP_ULTIMATE_INTEGRATION_STOP_THRESHOLD:
		integration->priv->stop_threshold = g_value_get_double(value);
		break;
	case PROP_ULTIMATE_INTEGRATION_T_START_MIN:
		integration->priv->t_start_min = g_value_get_double(value);
		break;
	case PROP_ULTIMATE_INTEGRATION_T_STOP_MAX:
		integration->priv->t_stop_max = g_value_get_double(value);
		break;
	case PROP_ULTIMATE_INTEGRATION_T_STOP_MIN:
		integration->priv->t_stop_min = g_value_get_double(value);
		break;
	case PROP_ULTIMATE_INTEGRATION_TRIGGER:
		integration->priv->trigger = g_value_get_uint(value);
		break;
	case PROP_ULTIMATE_INTEGRATION_ACTIVATED:
		integration->priv->activated = g_value_get_boolean(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
ultimate_integration_object_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (ULTIMATE_IS_INTEGRATION_OBJECT (object));
	UltimateIntegrationObject *integration = ULTIMATE_INTEGRATION_OBJECT(object);
	switch (prop_id)
	{

	case PROP_ULTIMATE_INTEGRATION_JUSTIFACATION_TIME:
		g_value_set_double(value,integration->priv->justification_time);
		break;
	case PROP_ULTIMATE_INTEGRATION_START_THRESHOLD:
		g_value_set_double(value,integration->priv->start_threshold);
		break;

	case PROP_ULTIMATE_INTEGRATION_STOP_THRESHOLD:
		g_value_set_double(value,integration->priv->stop_threshold);
		break;
	case PROP_ULTIMATE_INTEGRATION_T_START_MIN:
		g_value_set_double(value,integration->priv->t_start_min);
		break;
	case PROP_ULTIMATE_INTEGRATION_T_STOP_MAX:
		g_value_set_double(value,integration->priv->t_stop_max);
		break;
	case PROP_ULTIMATE_INTEGRATION_T_STOP_MIN:
		g_value_set_double(value,integration->priv->t_stop_min);
		break;
	case PROP_ULTIMATE_INTEGRATION_TRIGGER:
		g_value_set_uint(value,integration->priv->trigger);
		break;
	case PROP_ULTIMATE_INTEGRATION_ACTIVATED:
		g_value_set_boolean(value,integration->priv->activated);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}




static void
ultimate_integration_object_class_init (UltimateIntegrationObjectClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	g_type_class_add_private (klass, sizeof (UltimateIntegrationObjectPrivate));
	object_class->finalize     = ultimate_integration_object_finalize;
	object_class->set_property = ultimate_integration_object_set_property;
	object_class->get_property = ultimate_integration_object_get_property;

	g_object_class_override_property(object_class,PROP_ULTIMATE_INTEGRATION_JUSTIFACATION_TIME,"justification-time");
	g_object_class_override_property(object_class,PROP_ULTIMATE_INTEGRATION_START_THRESHOLD,"start-threshold");
	g_object_class_override_property(object_class,PROP_ULTIMATE_INTEGRATION_STOP_THRESHOLD,"stop-threshold");
	g_object_class_override_property(object_class,PROP_ULTIMATE_INTEGRATION_T_START_MIN,"start-min-time");
	g_object_class_override_property(object_class,PROP_ULTIMATE_INTEGRATION_T_STOP_MAX,"stop-max-time");
	g_object_class_override_property(object_class,PROP_ULTIMATE_INTEGRATION_T_STOP_MIN,"stop-min-time");
	g_object_class_override_property(object_class,PROP_ULTIMATE_INTEGRATION_TRIGGER,"trigger");
	g_object_class_override_property(object_class,PROP_ULTIMATE_INTEGRATION_ACTIVATED,"activated");







}

/** @} */
