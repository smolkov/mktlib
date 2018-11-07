/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktRelay
 * @ingroup  MktRelay
 * @{
 * @file  mkt-relay.h	Relay interface model header
 * @brief This is Relay interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef MKT_RELAY_H_
#define MKT_RELAY_H_


#include "mkt-model.h"
#include "mkt-decl.h"

G_BEGIN_DECLS

#define MKT_TYPE_RELAY                  (mkt_relay_get_type ())
#define MKT_RELAY(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj),MKT_TYPE_RELAY, MktRelay))
#define MKT_IS_RELAY(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj),MKT_TYPE_RELAY))
#define MKT_RELAY_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_RELAY, MktRelayInterface))


typedef struct _MktRelayInterface        MktRelayInterface;
typedef struct _MktRelay                 MktRelay;




struct _MktRelayInterface
{
	GTypeInterface parent_iface;


	const gchar*            (*relay_string)                    ( MktRelay *self );

};


GType                   mkt_relay_get_type                     ( void ) G_GNUC_CONST;

const gchar*            mkt_relay_string                       ( MktRelay *relay );


G_END_DECLS

#endif /* MKT_RELAY_H_ */

/** @} */
