/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * larintegrations.h
 * Copyright (C) LAR 2017 
 *

 */

#ifndef _LAR_INTEGRATIONS_H_
#define _LAR_INTEGRATIONS_H_

#include <glib-object.h>
#include <larintegration.h>


#define TYPE_LAR_INTEGRATIONS (integrations_get_type())
#define LAR_INTEGRATIONS(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_LAR_INTEGRATIONS, LarIntegrations))
#define LAR_INTEGRATIONS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_LAR_INTEGRATIONS, LarIntegrationsClass))
#define IS_LAR_INTEGRATIONS(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_LAR_INTEGRATIONS))
#define IS_LAR_INTEGRATIONS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_LAR_INTEGRATIONS))
#define LAR_INTEGRATIONS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_LAR_INTEGRATIONS, LarIntegrationsClass))

typedef struct _LarIntegrationsClass   LarIntegrationsClass;
typedef struct _LarIntegrations        LarIntegrations;

GType integrations_get_type(void) G_GNUC_CONST;


LarIntegration *m_LarIntegrationsGetNDir1TC1(LarIntegrations *integrations);
LarIntegration *m_LarIntegrationsGetNDir1TC2(LarIntegrations *integrations);
LarIntegration *m_LarIntegrationsGetNDir1TC3(LarIntegrations *integrations);
LarIntegration *m_LarIntegrationsGetNDir1TIC1(LarIntegrations *integrations);
LarIntegration *m_LarIntegrationsGetNDir1TIC2(LarIntegrations *integrations);
LarIntegration *m_LarIntegrationsGetNDir1TIC3(LarIntegrations *integrations);
LarIntegration *m_LarIntegrationsGetNDir2TC1(LarIntegrations *integrations);
LarIntegration *m_LarIntegrationsGetNDir2TC2(LarIntegrations *integrations);
LarIntegration *m_LarIntegrationsGetNDir2TC3(LarIntegrations *integrations);
LarIntegration *m_LarIntegrationsGetNDir2TIC1(LarIntegrations *integrations);
LarIntegration *m_LarIntegrationsGetNDir2TIC2(LarIntegrations *integrations);
LarIntegration *m_LarIntegrationsGetNDir2TIC3(LarIntegrations *integrations);
LarIntegration *m_LarIntegrationsGetTNbTC(LarIntegrations *integrations);
LarIntegration *m_LarIntegrationsGetTNbTIC(LarIntegrations *integrations);
LarIntegration *m_LarIntegrationsGetCODo(LarIntegrations *integrations);

#endif /* _LAR_INTEGRATIONS_H_ */
