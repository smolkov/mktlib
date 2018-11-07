/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktCalibration
 * @ingroup  MktCalibration
 * @{
 * @file  mkt-nitri_calibration.h	NitriStream interface model header
 * @brief This is NitriStream interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */
#ifndef MKT_CALIBRATION_H_
#define MKT_CALIBRATION_H_


#include "mkt-model.h"
#include "mkt-stream.h"
#include "mkt-channel.h"
#include "mkt-process.h"
#include "mkt-cal-point.h"
#include "mkt-cal-data.h"

G_BEGIN_DECLS

#define MKT_TYPE_CALIBRATION                  (mkt_calibration_get_type ())
#define MKT_CALIBRATION(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj),MKT_TYPE_CALIBRATION, MktCalibration))
#define MKT_IS_CALIBRATION(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj),MKT_TYPE_CALIBRATION))
#define MKT_CALIBRATION_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_CALIBRATION, MktCalibrationInterface))


typedef struct _MktCalibrationInterface MktCalibrationInterface;
typedef struct _MktCalibration MktCalibration;


struct _MktCalibrationInterface
{
	GTypeInterface parent_iface;

	guint                (*calibration_identification)                              ( MktCalibration *self );
	guint                (*calibration_level)                                       ( MktCalibration *self );

	guint64              (*calibration_channel )                                    ( MktCalibration *self );
	guint64              (*calibration_stream )                                     ( MktCalibration *self );
	guint64              (*calibration_process )                                    ( MktCalibration *self );
    gboolean             (*calibration_activated)                                   ( MktCalibration *self );
    gboolean             (*calibration_main)                                        ( MktCalibration *self );
    gboolean             (*calibration_done)                                        ( MktCalibration *self );
    gdouble              (*calibration_slope)                                       ( MktCalibration *self );
    gdouble              (*calibration_intercept)                                   ( MktCalibration *self );
    gdouble              (*calibration_determination)                               ( MktCalibration *self );
    gdouble              (*calibration_cv)                                          ( MktCalibration *self );
    gdouble              (*calibration_deviation)                                   ( MktCalibration *self );
    gdouble              (*calibration_time )                                       ( MktCalibration *self );
	const gchar*         (*calibration_date )                                       ( MktCalibration *self );


    GSList*              (*calibration_points)   	                                ( MktCalibration *self );

};


GType                   mkt_calibration_get_type                                    ( void ) G_GNUC_CONST;

guint                   mkt_calibration_identification                              ( MktCalibration *calibration );
guint                   mkt_calibration_level                                       ( MktCalibration *calibration );
guint64                 mkt_calibration_channel                                     ( MktCalibration *calibration );
guint64                 mkt_calibration_stream                                      ( MktCalibration *calibration );
guint64                 mkt_calibration_process                                     ( MktCalibration *calibration );
gboolean                mkt_calibration_activated                                   ( MktCalibration *calibration );
gboolean                mkt_calibration_main                                        ( MktCalibration *calibration );
gboolean                mkt_calibration_done                                        ( MktCalibration *calibration );
gdouble                 mkt_calibration_slope                                       ( MktCalibration *calibration );
gdouble                 mkt_calibration_intercept                                   ( MktCalibration *calibration );
gdouble                 mkt_calibration_determination                               ( MktCalibration *calibration );
gdouble                 mkt_calibration_cv_parameter                                ( MktCalibration *calibration );
gdouble                 mkt_calibration_deviation                                   ( MktCalibration *calibration );

gdouble                 mkt_calibration_time                                        ( MktCalibration *calibration );
const gchar*            mkt_calibration_date                                        ( MktCalibration *calibration );
void                    mkt_calibration_update_time                                 ( MktCalibration *calibration );

GSList*                	mkt_calibration_points                                      ( MktCalibration *calibration );
GSList*                 mkt_calibration_next_points                                 ( MktCalibration *calibration );
MktCalPoint*            mkt_calibration_max_cv                                      ( MktCalibration *calibration );
gboolean              	mkt_calibration_point_add_data                              ( MktCalPoint *point, MktCalData  *data);
GSList*                	mkt_calibration_point_data                                  ( MktCalPoint *point);

void                    mkt_calibration_calculate                                   ( MktCalibration *calibration );
MktChannel*             mkt_calibration_channel_model                               ( MktCalibration *calibration );
MktStream*              mkt_calibration_stream_model                                ( MktCalibration *calibration );
MktProcess*             mkt_calibration_process_model                               ( MktCalibration *calibration );

MktCalibration*         mkt_calibration_activated_for_channel                       ( guint64 channel_id );
MktCalibration*         mkt_calibration_last_for_channel                            ( guint64 channel_id );
MktCalibration*         mkt_calibration_main_for_channel                            ( guint64 channel_id );

GSList*                 mkt_calibration_last_for_process                            ( guint64 process_id );

GSList*                 mkt_calibration_default_points                              ( guint64 channel_id );


MktCalibration*         mkt_calibration_from_data                                   ( MktCalData *data);
MktCalibration*         mkt_calibration_from_point                                  ( MktCalPoint *point );

gdouble                 mkt_calibration_max_deviation                               ( MktCalibration *calibration, gdouble grund_slope, gdouble grund_intercept );

G_END_DECLS

#endif /* MKT_CALIBRATION_H_ */


/** @} */
