/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * integration.h
 * Copyright (C) LAR 2017
 * 
 */

#ifndef _INTEGRATION_H_
#define _INTEGRATION_H_

#include <glib-object.h>



#define TYPE_INTEGRATION             (integration_get_type ())
#define INTEGRATION(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_INTEGRATION, Integration))
#define INTEGRATION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass),  TYPE_INTEGRATION, IntegrationClass))
#define IS_INTEGRATION(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_INTEGRATION))
#define IS_INTEGRATION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass),  TYPE_INTEGRATION))
#define INTEGRATION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj),  TYPE_INTEGRATION, IntegrationClass))

typedef struct _IntegrationClass     IntegrationClass;
typedef struct _Integration          Integration;


GType                   integration_get_type                 (void) G_GNUC_CONST;


gdouble IntegrationJustificationTime(Integration *integration);
gdouble IntegrationStartThreshold(Integration *integration);
gdouble IntegrationStopThreshold(Integration *integration);
gdouble IntegrationStartMin(Integration *integration);
gdouble integrationStopMax(Integration *integration);
gdouble integrationStopMin(Integration *integration);

#endif /* _INTEGRATION_H_ */
