/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktInjection
 * @ingroup  MktInjection
 * @{
 * @file  mkt-injection.h	Injection interface model header
 * @brief This is Injection interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef _MKT_INJECTION_H_
#define _MKT_INJECTION_H_


#include "mkt-model.h"
#include "mkt-decl.h"

G_BEGIN_DECLS

#define MKT_TYPE_INJECTION                  (mkt_injection_get_type ())
#define MKT_INJECTION(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj),MKT_TYPE_INJECTION, MktInjection))
#define MKT_IS_INJECTION(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj),MKT_TYPE_INJECTION))
#define MKT_INJECTION_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_INJECTION, MktInjectionInterface))


typedef struct _MktInjectionInterface MktInjectionInterface;
typedef struct _MktInjection          MktInjection;




struct _MktInjectionInterface
{
	GTypeInterface parent_iface;

};

GType                        mkt_injection_get_type                        (void) G_GNUC_CONST;


G_END_DECLS

#endif /* MKT_INJECTION_H_ */

/** @} */
