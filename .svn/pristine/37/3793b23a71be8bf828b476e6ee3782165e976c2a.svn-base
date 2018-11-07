/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * integrations.h
 * Copyright (C) LAR 2017 
 *

 */

#ifndef _INTEGRATIONS_H_
#define _INTEGRATIONS_H_

#include <glib-object.h>
#include <integration.h>


#define TYPE_INTEGRATIONS (integrations_get_type())
#define INTEGRATIONS(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_INTEGRATIONS, Integrations))
#define INTEGRATIONS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_INTEGRATIONS, IntegrationsClass))
#define IS_INTEGRATIONS(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_INTEGRATIONS))
#define IS_INTEGRATIONS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_INTEGRATIONS))
#define INTEGRATIONS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_INTEGRATIONS, IntegrationsClass))

typedef struct _IntegrationsClass   IntegrationsClass;
typedef struct _Integrations        Integrations;

GType integrations_get_type(void) G_GNUC_CONST;


Integration *IntegrationsGetNDir1TC(Integrations *integrations);
Integration *IntegrationsGetNDir1TIC(Integrations *integrations);
Integration *IntegrationsGetNDir2TC(Integrations *integrations);
Integration *IntegrationsGetNDir2TIC(Integrations *integrations);
Integration *IntegrationsGetTNbTC(Integrations *integrations);
Integration *IntegrationsGetTNbTIC(Integrations *integrations);
Integration *IntegrationsGetCODo(Integrations *integrations);

#endif /* _INTEGRATIONS_H_ */
