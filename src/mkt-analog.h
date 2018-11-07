/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktAnalog
 * @ingroup  MktAnalog
 * @{
 * @file  mkt-analog.h	Analog interface model header
 * @brief This is Analog interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef MKT_ANALOG_H_
#define MKT_ANALOG_H_


#include "mkt-model.h"
#include "mkt-decl.h"

G_BEGIN_DECLS

#define MKT_TYPE_ANALOG                  (mkt_analog_get_type ())
#define MKT_ANALOG(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj),MKT_TYPE_ANALOG, MktAnalog))
#define MKT_IS_ANALOG(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj),MKT_TYPE_ANALOG))
#define MKT_ANALOG_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_ANALOG, MktAnalogInterface))


typedef struct _MktAnalogInterface        MktAnalogInterface;
typedef struct _MktAnalog                 MktAnalog;




struct _MktAnalogInterface
{
	GTypeInterface parent_iface;


	gdouble                 (*analog_min)                       ( MktAnalog *self );
	gdouble                 (*analog_max)                       ( MktAnalog *self );
	gdouble                 (*analog_limit)                     ( MktAnalog *self );

};


GType                   mkt_analog_get_type                        (void) G_GNUC_CONST;



gdouble                 mkt_analog_min                          ( MktAnalog *analog );
gdouble                 mkt_analog_max                          ( MktAnalog *analog );
gdouble                 mkt_analog_limit                        ( MktAnalog *analog );
gdouble                 mkt_analog_transmit_value               ( MktAnalog *analog, gdouble value , gdouble min , gdouble max );



MktAnalog*              mkt_analog_get_object                      ( guint64 id );


G_END_DECLS

#endif /* MKT_ANALOG_H_ */

/** @} */
