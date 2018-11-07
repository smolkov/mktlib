/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * larintegration.h
 * Copyright (C) LAR 2017
 * 
 */

#ifndef _LAR_INTEGRATION_H_
#define _LAR_INTEGRATION_H_

#include <glib-object.h>



#define TYPE_LAR_INTEGRATION             (lar_integration_get_type ())
#define LAR_INTEGRATION(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_LAR_INTEGRATION, LarIntegration))
#define LAR_INTEGRATION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass),  TYPE_LAR_INTEGRATION, LarIntegrationClass))
#define IS_LAR_INTEGRATION(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_LAR_INTEGRATION))
#define IS_LAR_INTEGRATION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass),  TYPE_LAR_INTEGRATION))
#define LAR_INTEGRATION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj),  TYPE_LAR_INTEGRATION, LarIntegrationClass))

typedef struct _LarIntegrationClass     LarIntegrationClass;
typedef struct _LarIntegration          LarIntegration;


GType                   lar_integration_get_type                 (void) G_GNUC_CONST;


gdouble m_LarIntegrationJustificationTime(LarIntegration *integration);
gdouble m_LarIntegrationStartThreshold(LarIntegration *integration);
gdouble m_LarIntegrationStopThreshold(LarIntegration *integration);
gdouble m_LarIntegrationStartMin(LarIntegration *integration);
gdouble m_LarIntegrationStopMax(LarIntegration *integration);
gdouble m_LarIntegrationStopMin(LarIntegration *integration);

#endif /* _LAR_INTEGRATION_H_ */
