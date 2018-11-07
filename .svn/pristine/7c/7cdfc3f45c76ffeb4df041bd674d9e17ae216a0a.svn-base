/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktChannel
 * @ingroup  MktChannel
 * @{
 * @file  mkt-channel.h	Channel interface model header
 * @brief This is Channel interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef __MKT_CHANNEL_H_
#define __MKT_CHANNEL_H_


#include "mkt-model.h"
#include "mkt-decl.h"
#include "mkt-measurement.h"

G_BEGIN_DECLS

#define MKT_TYPE_CHANNEL                  (mkt_channel_get_type ())
#define MKT_CHANNEL(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj),MKT_TYPE_CHANNEL, MktChannel))
#define MKT_IS_CHANNEL(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj),MKT_TYPE_CHANNEL))
#define MKT_CHANNEL_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_CHANNEL, MktChannelInterface))


typedef struct _MktChannelInterface MktChannelInterface;
typedef struct _MktChannel          MktChannel;



typedef enum
{
	MKT_CHANNEL_VALUE_REPLICATE,
	MKT_CHANNEL_VALUE_RESULT,

}MktChannelValueArt;


struct _MktChannelInterface
{
	GTypeInterface parent_iface;
	const gchar*            (*channel_name)                         ( MktChannel *self );
	const gchar*            (*channel_unit)                         ( MktChannel *self );
	const gchar*            (*channel_type)                         ( MktChannel *self );
	const gchar*            (*channel_analog_out)                   ( MktChannel *self );
	guint                   (*channel_measurement)                  ( MktChannel *self );
	guint                   (*channel_trigger)                      ( MktChannel *self );
	gdouble                 (*channel_min)                          ( MktChannel *self );
	gdouble                 (*channel_max)                          ( MktChannel *self );
	gdouble                 (*channel_factor)                       ( MktChannel *self );
	gdouble                 (*channel_result)                       ( MktChannel *self );
	gdouble                 (*channel_raw)                          ( MktChannel *self );
	gdouble                 (*channel_cv)                           ( MktChannel *self );
	gdouble                 (*channel_changed)                      ( MktChannel *self );
	void                    (*channel_add_value)                    ( MktChannel *self, gdouble value, guint sensor_trigger, guint type );
	GSList*                 (*channel_current_value)                ( MktChannel *self );
	MktMeasurement*         (*channel_current_result)               ( MktChannel *self );
	GSList*                 (*channel_current_replicates)           ( MktChannel *self );
};


GType                   mkt_channel_get_type                        ( void ) G_GNUC_CONST;
const gchar*            mkt_channel_type                            ( MktChannel *channel );
const gchar*            mkt_channel_name                            ( MktChannel *channel );
const gchar*            mkt_channel_unit                            ( MktChannel *channel );
const gchar*            mkt_channel_analog_out                      ( MktChannel *channel );
guint                   mkt_channel_measurement                     ( MktChannel *channel );
guint                   mkt_channel_trigger                         ( MktChannel *channel );
gdouble                 mkt_channel_min                             ( MktChannel *channel );
gdouble                 mkt_channel_max                             ( MktChannel *channel );
gdouble                 mkt_channel_factor                          ( MktChannel *channel );
gdouble                 mkt_channel_result                          ( MktChannel *channel );
gdouble                 mkt_channel_raw                             ( MktChannel *channel );
gdouble                 mkt_channel_cv                              ( MktChannel *channel );
void                    mkt_channel_add_value                       ( MktChannel *channel, gdouble value, guint sensor_trigger, guint type );
GSList*                 mkt_channel_current_value                   ( MktChannel *channel );
MktMeasurement*         mkt_channel_current_result                  ( MktChannel *channel );
GSList*                 mkt_channel_current_replicates              ( MktChannel *channel );
gdouble                 mkt_channel_changed                         ( MktChannel *channel );




G_END_DECLS

#endif /* __MKT_CHANNEL_H_ */

/** @} */
