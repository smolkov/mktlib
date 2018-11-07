/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup UltimateIntegration
 * @ingroup  UltimateIntegration
 * @{
 * @file  mkt-nitri_stream.h	NitriIntegration interface model header
 * @brief This is NitriIntegration interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */
#ifndef ULTIMATE_INTEGRATION_H_
#define ULTIMATE_INTEGRATION_H_


#include "mkt-model.h"

G_BEGIN_DECLS

#define ULTIMATE_TYPE_INTEGRATION                  (ultimate_integration_get_type ())
#define ULTIMATE_INTEGRATION(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj),ULTIMATE_TYPE_INTEGRATION, UltimateIntegration))
#define ULTIMATE_IS_INTEGRATION(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj),ULTIMATE_TYPE_INTEGRATION))
#define ULTIMATE_INTEGRATION_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE ((inst), ULTIMATE_TYPE_INTEGRATION, UltimateIntegrationInterface))


typedef struct _UltimateIntegrationInterface UltimateIntegrationInterface;
typedef struct _UltimateIntegration          UltimateIntegration;



struct _UltimateIntegrationInterface
{
	GTypeInterface         parent_iface;

	gdouble              (*justification_time)           ( UltimateIntegration *self );
	gdouble              (*start_threshold)              ( UltimateIntegration *self );
	gdouble              (*stop_threshold)               ( UltimateIntegration *self );
	gdouble              (*t_start_min)                  ( UltimateIntegration *self );
	gdouble              (*t_stop_max)                   ( UltimateIntegration *self );
	gdouble              (*t_stop_min)                   ( UltimateIntegration *self );
	guint                (*trigger)                      ( UltimateIntegration *self );

};


GType                     ultimate_integration_get_type                     ( void ) G_GNUC_CONST;


gdouble                   ultimate_integration_justification_time           ( UltimateIntegration *uintegration );
gdouble                   ultimate_integration_start_threshold              ( UltimateIntegration *uintegration );
gdouble                   ultimate_integration_stop_threshold               ( UltimateIntegration *uintegration );
gdouble                   ultimate_integration_last_zerro                   ( UltimateIntegration *uintegration );
gdouble                   ultimate_integration_last_zerro_cv                ( UltimateIntegration *uintegration );


gdouble                   ultimate_integration_t_start_min                  ( UltimateIntegration *uintegration );
gdouble                   ultimate_integration_t_stop_max                   ( UltimateIntegration *uintegration );
gdouble                   ultimate_integration_t_stop_min                   ( UltimateIntegration *uintegration );
guint                     ultimate_integration_trigger                      ( UltimateIntegration *uintegration );

//gboolean                  ultimate_transmit_results                    ( UltimateIntegration *uintegration );




G_END_DECLS

#endif /* ULTIMATE_INTEGRATION_H_ */
/** @} */
