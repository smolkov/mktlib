/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktCal
 * @ingroup  MktCal
 * @{
 * @file  mkt-nitri_cal.h	NitriStream interface model header
 * @brief This is NitriStream interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */
#ifndef _MKT_CAL_H_
#define _MKT_CAL_H_


#include <mkt-model.h>
#include "mkt-cal-point.h"

G_BEGIN_DECLS

#define MKT_TYPE_CAL                  (mkt_cal_get_type ())
#define MKT_CAL(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj),MKT_TYPE_CAL, MktCal))
#define MKT_IS_CAL(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj),MKT_TYPE_CAL))
#define MKT_CAL_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_CAL, MktCalInterface))


typedef struct _MktCalInterface MktCalInterface;
typedef struct _MktCal MktCal;



enum
{
	MKT_CAL_SAVED_TYPE,
	MKT_CAL_LAST_TYPE,
	MKT_CAL_ACTIVATED_TYPE,
};

struct _MktCalInterface
{
	GTypeInterface parent_iface;

	MktModel*            (*cal_calibration)	                                     ( MktCal *self );
	MktCalPoint*         (*solution_point)	                                     ( MktCal *self, guint solution );
	guint64              (*cal_process)	                                         ( MktCal *self );
	GSList*              (*points)                                               ( MktCal *self );
	GSList*              (*point_data)                                           ( MktCal *self );
	gboolean             (*next_point)                                           ( MktCal *self );
	gboolean             (*pre_point)                                            ( MktCal *self );
	gboolean             (*have_next_point)                                      ( MktCal *self );
	gboolean             (*delete_points)                                        ( MktCal *self );
    gdouble              (*cal_slope)                                            ( MktCal *self );
    gdouble              (*cal_intercept)                                        ( MktCal *self );
};


GType                   mkt_cal_get_type                                         ( void ) G_GNUC_CONST;
MktModel*               mkt_cal_calibration                                      ( MktCal *cal );

MktCalPoint*            mkt_cal_solution_point                                   ( MktCal *cal , guint siolution );
gboolean                mkt_cal_next_point                                       ( MktCal *cal );
gboolean                mkt_cal_pre_point                                        ( MktCal *cal );
gboolean                mkt_cal_have_next_point                                  ( MktCal *cal );


void                    mkt_cal_update                                           ( MktCal *cal );

GSList*                 mkt_cal_points                                           ( MktCal *cal );


gdouble             	mkt_cal_slope                                            ( MktCal *cal );
gdouble              	mkt_cal_intercept                                        ( MktCal *cal );
guint64                 mkt_cal_process                                          ( MktCal *cal );
gboolean                mkt_cal_add_value                                        ( MktCal *cal, gdouble value, guint sensor_trigger );
gboolean                mkt_cal_calculate                                        ( MktCal *cal );
gboolean                mkt_cal_reset                                            ( MktCal *cal );

gboolean                mkt_cal_delete_points                                    ( MktCal *cal );
gboolean                mkt_cal_replace_points                                   ( MktCal *src , MktCal *dest  );
gboolean                mkt_cal_add_point                                        ( MktCal *cal , gdouble solution );

G_END_DECLS

#endif /* MKT_CAL_H_ */


/** @} */
