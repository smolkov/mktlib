/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktStatus
 * @ingroup  MktStatus
 * @{
 * @file  mkt-status.h	Status interface model header
 * @brief This is Status interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef MKT_STATUS_H_
#define MKT_STATUS_H_


#include "mkt-model.h"
#include "mkt-decl.h"

G_BEGIN_DECLS

#define MKT_TYPE_STATUS                  (mkt_status_get_type ())
#define MKT_STATUS(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj),MKT_TYPE_STATUS, MktStatus))
#define MKT_IS_STATUS(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj),MKT_TYPE_STATUS))
#define MKT_STATUS_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_STATUS, MktStatusInterface))


typedef struct _MktStatusInterface        MktStatusInterface;
typedef struct _MktStatus                 MktStatus;




struct _MktStatusInterface
{
	GTypeInterface parent_iface;


	const gchar*            (*status_signification)             ( MktStatus *self );
	const gchar*            (*status_description)               ( MktStatus *self );
	gboolean                (*is_active)                        ( MktStatus *self );


};


GType                   mkt_status_get_type                     ( void ) G_GNUC_CONST;

const gchar*            mkt_status_signification                ( MktStatus *status );
const gchar*            mkt_status_discription                  ( MktStatus *status );
gboolean                mkt_status_is_active                    ( MktStatus *status );
void                    mkt_status_switch                       ( const gchar *status, gboolean value );
void                    mkt_status_activate                     ( MktStatus *status  , gboolean value );

void                    mkt_status_create                       ( const gchar *status,const gchar *format,...  )G_GNUC_PRINTF (2, 3);
MktStatus*              mkt_status_new                          ( const gchar *status,const gchar *format,...  )G_GNUC_PRINTF (2, 3);
G_END_DECLS

#endif /* MKT_STATUS_H_ */

/** @} */
