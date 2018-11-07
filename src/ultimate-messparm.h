/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup UltimateMessparm
 * @defgroup UltimateMessparm
 * @ingroup  UltimateMessparm
 * @{
 * @file  mkt-nitri_messparm.h	NitriMessparm interface model header
 * @brief This is NitriMessparm interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */
#ifndef ULTIMATE_MESSPARM_H_
#define ULTIMATE_MESSPARM_H_



#include "mkt-model.h"
#include "mkt-decl.h"

G_BEGIN_DECLS

#define ULTIMATE_TYPE_MESSPARM                  (ultimate_messparm_get_type ())
#define ULTIMATE_MESSPARM(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj),ULTIMATE_TYPE_MESSPARM, UltimateMessparm))
#define ULTIMATE_IS_MESSPARM(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj),ULTIMATE_TYPE_MESSPARM))
#define ULTIMATE_MESSPARM_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE ((inst), ULTIMATE_TYPE_MESSPARM, UltimateMessparmInterface))


typedef struct _UltimateMessparmInterface UltimateMessparmInterface;
typedef struct _UltimateMessparm UltimateMessparm;


struct _UltimateMessparmInterface
{
	GTypeInterface parent_iface;
	guint                  (*volume)                                                       ( UltimateMessparm *self );
	gdouble                (*sample_filling_time)                                          ( UltimateMessparm *self );
	gdouble                (*delay)                                                        ( UltimateMessparm *self );
	gboolean               (*pre_rinsing)                                                  ( UltimateMessparm *self );
	guint                  (*rinsing_count)                                                ( UltimateMessparm *self );
	gboolean               (*after_rinsing)                                                ( UltimateMessparm *self );
	guint                  (*after_rinsing_count)                                          ( UltimateMessparm *self );
	gdouble                (*stripping_time)                                               ( UltimateMessparm *self );
	gdouble                (*dilution_factor)                                              ( UltimateMessparm *self );
	gdouble                (*dilution_pump_time)                                           ( UltimateMessparm *self );
	gdouble                (*dilution_wait_time)                                           ( UltimateMessparm *self );

};

GType                        ultimate_messparm_get_type                                    ( void ) G_GNUC_CONST;


guint                        ultimate_messparm_volume                                      ( UltimateMessparm *umessparm );
gdouble                      ultimate_messparm_sample_filling_time                         ( UltimateMessparm *umessparm );
gdouble                      ultimate_messparm_delay                                       ( UltimateMessparm *umessparm );
gboolean                     ultimate_messparm_pre_rinsing                                 ( UltimateMessparm *umessparm );
guint                        ultimate_messparm_rinsing_count                               ( UltimateMessparm *umessparm );
gboolean                     ultimate_messparm_after_rinsing                               ( UltimateMessparm *umessparm );
guint                        ultimate_messparm_after_rinsing_count                         ( UltimateMessparm *umessparm );


gdouble                      ultimate_messparm_stripping_time                              ( UltimateMessparm *umessparm );
gdouble                      ultimate_messparm_dilution_factor                             ( UltimateMessparm *umessparm );
gdouble                      ultimate_messparm_dilution_pump_time                          ( UltimateMessparm *umessparm );
gdouble                      ultimate_messparm_dilution_wait_time                          ( UltimateMessparm *umessparm );


G_END_DECLS

#endif /* ULTIMATE_MESSPARM_H_ */
