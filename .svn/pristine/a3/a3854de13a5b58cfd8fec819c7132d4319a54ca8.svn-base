/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktParamint32
 * @ingroup  MktParamint32
 * @{
 * @file  mkt-paramint32.h	PARAMINT32 interface model header
 * @brief This is PARAMINT32 interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef __MKT_PARAMINT32EN_H__
#define __MKT_PARAMINT32EN_H__

#include "mkt-model.h"


G_BEGIN_DECLS

#define MKT_TYPE_PARAMINT32                 (mkt_paramint32_get_type ())
#define MKT_PARAMINT32(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_PARAMINT32, MktParamint32))
#define MKT_IS_PARAMINT32(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_PARAMINT32))
#define MKT_PARAMINT32_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_PARAMINT32, MktParamint32Interface))



typedef struct _MktParamint32               MktParamint32; /* dummy object */
typedef struct _MktParamint32Interface      MktParamint32Interface;


struct _MktParamint32Interface
{
  GTypeInterface parent_iface;
  gint             (*value)                               ( MktParamint32 *self );
};


GType                    mkt_paramint32_get_type          ( void );

gint                     mkt_paramint32_value             ( MktParamint32 *paramint32 );

MktParamint32*           mkt_paramint32_get               ( const gchar *id, const gchar *path, const gchar *name );
gint                     mkt_paramint32_get_value         ( const gchar *id, const gchar *path, const gchar *name  );
void                     mkt_paramint32_set_value         ( MktParamint32 *paramint32, gint value );


G_END_DECLS

#endif /* _MKT_PARAMINT32EN_H_ */
/** @} */
