/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktParamuint32
 * @ingroup  MktParamuint32
 * @{
 * @file  mkt-paramuint32.h	PARAMUINT32 interface model header
 * @brief This is PARAMUINT32 interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef __MKT_PARAMUINT32EN_H__
#define __MKT_PARAMUINT32EN_H__

#include "mkt-model.h"


G_BEGIN_DECLS

#define MKT_TYPE_PARAMUINT32                 (mkt_paramuint32_get_type ())
#define MKT_PARAMUINT32(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_PARAMUINT32, MktParamuint32))
#define MKT_IS_PARAMUINT32(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_PARAMUINT32))
#define MKT_PARAMUINT32_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_PARAMUINT32, MktParamuint32Interface))



typedef struct _MktParamuint32               MktParamuint32; /* dummy object */
typedef struct _MktParamuint32Interface      MktParamuint32Interface;


struct _MktParamuint32Interface
{
  GTypeInterface parent_iface;
  guint             (*value)                               ( MktParamuint32 *self );
};


GType                    mkt_paramuint32_get_type          ( void );

guint                    mkt_paramuint32_value             ( MktParamuint32 *paramuint32 );
MktParamuint32*          mkt_paramuint32_get               ( const gchar *id, const gchar *path, const gchar *name );


G_END_DECLS

#endif /* _MKT_PARAMUINT32EN_H_ */
/** @} */
