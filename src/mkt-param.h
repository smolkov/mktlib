/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktParam
 * @ingroup  MktParam
 * @{
 * @file  mkt-param.h	PARAM interface model header
 * @brief This is PARAM interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef __MKT_PARAMEN_H__
#define __MKT_PARAMEN_H__

#include "mkt-model.h"


G_BEGIN_DECLS

#define MKT_TYPE_PARAM                 (mkt_param_get_type ())
#define MKT_PARAM(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_PARAM, MktParam))
#define MKT_IS_PARAM(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_PARAM))
#define MKT_PARAM_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_PARAM, MktParamInterface))



typedef struct _MktParam               MktParam; /* dummy object */
typedef struct _MktParamInterface      MktParamInterface;

enum
{
	MKT_PARAM_SYSTEM,
	MKT_PARAM_LIMIT_ON_CHANNEL,
	MKT_PARAM_LIMIT_MEASUREMENT_SWITCH,
	MKT_PARAM_MEASUREMENT,
	MKT_PARAM_CALIBRATE,
	MKT_PARAM_SERVICE_PARAM,
	MKT_PARAM_OPERATOR,
	MKT_PARAM_UNKNOWN,
	MKT_PARAM_MESSAGE_LAST,
};


struct _MktParamInterface
{
  GTypeInterface parent_iface;
  const gchar*     (*object_path)                         ( MktParam *self );
  gboolean         (*activated)                           ( MktParam *self );
  const gchar*     (*get_type)                            ( MktParam *self );
  gdouble          (*get_changed)                         ( MktParam *self );
  const gchar*     (*get_name)                            ( MktParam *self );
  const gchar*     (*get_description)                     ( MktParam *self );
  void             (*activate)                            ( MktParam *self );
  const gchar*     (*get_value)                           ( MktParam *self );

};


GType                           mkt_param_get_type        ( void );

const gchar*     mkt_param_object_path                    ( MktParam *param );

gboolean         mkt_param_activated                      ( MktParam *param );
const gchar*     mkt_param_type                           ( MktParam *param );
gdouble          mkt_param_changed                        ( MktParam *param );
const gchar*     mkt_param_name                           ( MktParam *param );
const gchar*     mkt_param_description                    ( MktParam *param );
void             mkt_param_activate                       ( MktParam *param );
const gchar*     mkt_param_value                          ( MktParam *param );
void             mkt_param_update_changed                 ( MktParam *param );

G_END_DECLS

#endif /* _MKT_PARAMEN_H_ */
/** @} */
