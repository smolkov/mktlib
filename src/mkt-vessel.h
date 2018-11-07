/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktVessel
 * @ingroup  MktVessel
 * @{
 * @file  mkt-vessel.h	Vessel interface model header
 * @brief This is Vessel interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef _MKT_VESSEL_H_
#define _MKT_VESSEL_H_


#include "mkt-model.h"
#include "mkt-decl.h"

G_BEGIN_DECLS

#define MKT_TYPE_VESSEL                  (mkt_vessel_get_type ())
#define MKT_VESSEL(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj),MKT_TYPE_VESSEL, MktVessel))
#define MKT_IS_VESSEL(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj),MKT_TYPE_VESSEL))
#define MKT_VESSEL_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_VESSEL, MktVesselInterface))


typedef struct _MktVesselInterface MktVesselInterface;
typedef struct _MktVessel          MktVessel;




struct _MktVesselInterface
{
	GTypeInterface parent_iface;

	guint                    (*vessel_xpos)                             ( MktVessel *self );
	guint                    (*vessel_ypos)                             ( MktVessel *self );
	guint                    (*vessel_y1pos)                            ( MktVessel *self );
	guint                    (*vessel_y2pos)                            ( MktVessel *self );
	guint                    (*vessel_y3pos)                            ( MktVessel *self );
	const gchar*             (*vessel_pump)                             ( MktVessel *self );

};


GType                        mkt_vessel_get_type                        (void) G_GNUC_CONST;

guint                        mkt_vessel_xpos                            ( MktVessel *vessel );
guint                        mkt_vessel_ypos                            ( MktVessel *vessel );
guint                        mkt_vessel_y1pos                           ( MktVessel *vessel );
guint                        mkt_vessel_y2pos                           ( MktVessel *vessel );
guint                        mkt_vessel_y3pos                           ( MktVessel *vessel );
const gchar*                 mkt_vessel_pump                            ( MktVessel *vessel );


G_END_DECLS

#endif /* MKT_VESSEL_H_ */

/** @} */
