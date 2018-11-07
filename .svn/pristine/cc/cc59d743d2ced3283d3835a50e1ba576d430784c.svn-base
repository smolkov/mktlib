/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktMeasurement
 * @ingroup  MktMeasurement
 * @{
 * @file  mkt-measurement.h	Measurement interface model header
 * @brief This is Measurement interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */
#ifndef MKT_MEASUREMENT_H_
#define MKT_MEASUREMENT_H_


#include "mkt-model.h"


G_BEGIN_DECLS

#define MKT_TYPE_MEASUREMENT                  (mkt_measurement_get_type ())
#define MKT_MEASUREMENT(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj),MKT_TYPE_MEASUREMENT, MktMeasurement))
#define MKT_IS_MEASUREMENT(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj),MKT_TYPE_MEASUREMENT))
#define MKT_MEASUREMENT_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_MEASUREMENT, MktMeasurementInterface))


typedef struct _MktMeasurementInterface   MktMeasurementInterface;
typedef struct _MktMeasurement            MktMeasurement;



struct _MktMeasurementInterface
{
	GTypeInterface            parent_iface;
	const gchar*            (*measurement_name)                         ( MktMeasurement *self );
	const gchar*            (*measurement_unit)                         ( MktMeasurement *self );
	guint                   (*measurement_identification)               ( MktMeasurement *self );
	gint                    (*measurement_type)                         ( MktMeasurement *self );
	gdouble                 (*measurement_changed)                      ( MktMeasurement *self );
	gdouble                 (*measurement_value)                        ( MktMeasurement *self );
	gdouble                 (*measurement_value_row)                    ( MktMeasurement *self );
	gdouble                 (*measurement_cv)                           ( MktMeasurement *self );
	guint                   (*measurement_replicate)                    ( MktMeasurement *self );

	gboolean                (*measurement_outlier)                      ( MktMeasurement *self );
	guint                   (*measurement_trigger)                      ( MktMeasurement *self );
	guint                   (*measurement_signal )                      ( MktMeasurement *self );
	guint64                 (*measurement_channel )                     ( MktMeasurement *self );
	guint64                 (*measurement_stream )                      ( MktMeasurement *self );
	guint64                 (*measurement_process )                     ( MktMeasurement *self );
};


GType                   mkt_measurement_get_type                        (void) G_GNUC_CONST;

const gchar*            mkt_measurement_name                            ( MktMeasurement *measurement );
const gchar*            mkt_measurement_unit                            ( MktMeasurement *measurement );
guint                   mkt_measurement_identification                  ( MktMeasurement *measurement );
gdouble                 mkt_measurement_changed                         ( MktMeasurement *measurement );
gdouble                 mkt_measurement_value                           ( MktMeasurement *measurement );
gdouble                 mkt_measurement_value_row                       ( MktMeasurement *measurement );
gdouble                 mkt_measurement_cv                              ( MktMeasurement *measurement );
guint                   mkt_measurement_replicate                       ( MktMeasurement *measurement );
gboolean                mkt_measurement_outlier                         ( MktMeasurement *measurement );
guint                   mkt_measurement_trigger                         ( MktMeasurement *measurement );
guint                   mkt_measurement_signal                          ( MktMeasurement *measurement );

guint64                 mkt_measurement_channel                         ( MktMeasurement *measurement );
gint                    mkt_measurement_type                            ( MktMeasurement *measurement );
guint64                 mkt_measurement_stream                          ( MktMeasurement *measurement );
guint64                 mkt_measurement_process                         ( MktMeasurement *measurement );


void                    mkt_measurement_insert                          ( MktMeasurement *measurement );

G_END_DECLS

#endif /* MKT_MEASUREMENT_H_ */

/** @} */
