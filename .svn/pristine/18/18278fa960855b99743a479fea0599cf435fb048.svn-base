/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktUser
 * @ingroup  MktUser
 * @{
 * @file  mkt-user.h	User interface model header
 * @brief This is User interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef MKT_USER_H_
#define MKT_USER_H_


#include "mkt-model.h"
#include "mkt-decl.h"

G_BEGIN_DECLS

#define MKT_TYPE_USER                  (mkt_user_get_type ())
#define MKT_USER(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj),MKT_TYPE_USER, MktUser))
#define MKT_IS_USER(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj),MKT_TYPE_USER))
#define MKT_USER_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_USER, MktUserInterface))


typedef struct _MktUserInterface        MktUserInterface;
typedef struct _MktUser                 MktUser;




struct _MktUserInterface
{
	GTypeInterface parent_iface;


	const gchar*            (*user_name)                      ( MktUser *self );
	const gchar*            (*user_password)                  ( MktUser *self );


};


GType                   mkt_user_get_type                     ( void ) G_GNUC_CONST;

const gchar*            mkt_user_name                         ( MktUser *user );
const gchar*            mkt_user_password                     ( MktUser *user );


G_END_DECLS

#endif /* MKT_USER_H_ */

/** @} */
