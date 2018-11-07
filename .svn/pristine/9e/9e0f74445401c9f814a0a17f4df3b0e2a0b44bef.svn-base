/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktCalPoint
 * @ingroup  MktCalPoint
 * @{
 * @file  mkt-nitri_cal_point.h	NitriStream interface model header
 * @brief This is NitriStream interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */
#ifndef _MKT_CAL_POINT_H_
#define _MKT_CAL_POINT_H_


#include "mkt-model.h"
#include "mkt-cal-data.h"


G_BEGIN_DECLS

#define MKT_TYPE_CAL_POINT                  (mkt_cal_point_get_type ())
#define MKT_CAL_POINT(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj),MKT_TYPE_CAL_POINT, MktCalPoint))
#define MKT_IS_CAL_POINT(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj),MKT_TYPE_CAL_POINT))
#define MKT_CAL_POINT_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_CAL_POINT, MktCalPointInterface))


typedef struct _MktCalPointInterface  MktCalPointInterface;
typedef struct _MktCalPoint           MktCalPoint;


struct _MktCalPointInterface
{
	GTypeInterface parent_iface;

	gboolean            (*cal_point_done)	                                      ( MktCalPoint *self );
	gdouble             (*cal_point_average)                                      ( MktCalPoint *self );
    gdouble             (*cal_point_solution)                                     ( MktCalPoint *self );
    gdouble             (*cal_point_cv)                                           ( MktCalPoint *self );
    guint64             (*cal_point_calibration)                                  ( MktCalPoint *self );
};


GType                   mkt_cal_point_get_type                                    ( void ) G_GNUC_CONST;


gboolean                mkt_cal_point_done                                        ( MktCalPoint *cal_point );
gdouble              	mkt_cal_point_average                                     ( MktCalPoint *cal_point );
gdouble              	mkt_cal_point_solution                                    ( MktCalPoint *cal_point );
gdouble              	mkt_cal_point_cv                                          ( MktCalPoint *cal_point );
void                    mkt_cal_point_calculate                                   ( MktCalPoint *cal_point  );
guint64              	mkt_cal_point_calibration                                 ( MktCalPoint *cal_point );

G_END_DECLS

#endif /* _MKT_CAL_POINT_H_ */
