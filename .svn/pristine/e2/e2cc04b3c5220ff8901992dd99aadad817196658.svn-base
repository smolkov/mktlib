/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktLimit
 * @ingroup  MktLimit
 * @{
 * @file  mkt-limit.h	LIMIT interface model header
 * @brief This is LIMIT interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef _MKT_LIMITEN_H_
#define _MKT_LIMITEN_H_

#include "mkt-model.h"


G_BEGIN_DECLS

#define MKT_TYPE_LIMIT                 (mkt_limit_get_type ())
#define MKT_LIMIT(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_LIMIT, MktLimit))
#define MKT_IS_LIMIT(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_LIMIT))
#define MKT_LIMIT_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_LIMIT, MktLimitInterface))



typedef struct _MktLimit               MktLimit; /* dummy object */
typedef struct _MktLimitInterface      MktLimitInterface;


enum {
	MKT_LIMIT_UNKNOWN,
	MKT_LIMIT_ON_CHANNEL,
	MKT_LIMIT_MEASUREMENT_SWITCH,
};

struct _MktLimitInterface
{
  GTypeInterface     parent_iface;
  gboolean         (*get_activated)                       ( MktLimit *self );
  guint            (*get_number)                          ( MktLimit *self );
  gint             (*get_pending)                         ( MktLimit *self );
  gdouble          (*get_min)                             ( MktLimit *self );
  gdouble          (*get_max)                             ( MktLimit *self );
  const gchar*     (*get_name)                            ( MktLimit *self );

};


GType            mkt_limit_get_type                       ( void );

gboolean         mkt_limit_activated                      ( MktLimit *limit );
guint            mkt_limit_number                         ( MktLimit *limit );
gint             mkt_limit_pending                        ( MktLimit *limit );
gdouble          mkt_limit_min                            ( MktLimit *limit );
gdouble          mkt_limit_max                            ( MktLimit *limit );
const gchar*     mkt_limit_name                           ( MktLimit *limit );

gchar*           mkt_limit_dup                            ( void );
gchar            mkt_limit_check_result                   ( MktLimit *limit , gdouble result);





G_END_DECLS

#endif /* _MKT_LIMITEN_H_ */
/** @} */
