/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktParamdouble
 * @ingroup  MktParamdouble
 * @{
 * @file  mkt-paramdouble.h	PARAMDOUBLE interface model header
 * @brief This is PARAMDOUBLE interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef __MKT_PARAMDOUBLEEN_H__
#define __MKT_PARAMDOUBLEEN_H__

#include "mkt-model.h"


G_BEGIN_DECLS

#define MKT_TYPE_PARAMDOUBLE                 (mkt_paramdouble_get_type ())
#define MKT_PARAMDOUBLE(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_PARAMDOUBLE, MktParamdouble))
#define MKT_IS_PARAMDOUBLE(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_PARAMDOUBLE))
#define MKT_PARAMDOUBLE_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_PARAMDOUBLE, MktParamdoubleInterface))



typedef struct _MktParamdouble               MktParamdouble; /* dummy object */
typedef struct _MktParamdoubleInterface      MktParamdoubleInterface;


struct _MktParamdoubleInterface
{
  GTypeInterface parent_iface;
  gdouble              (*get_value)                        ( MktParamdouble *self );
};


GType                    mkt_paramdouble_get_type          ( void );

gdouble                  mkt_paramdouble_value             ( MktParamdouble *paramdouble );

MktParamdouble*          mkt_paramdouble_get               ( const gchar *id, const gchar *path, const gchar *name );
gdouble                  mkt_paramdouble_get_value         ( const gchar *id, const gchar *path, const gchar *name  );
void                     mkt_paramdouble_set_value         ( MktParamdouble *paramdouble, gdouble value );


G_END_DECLS

#endif /* _MKT_PARAMDOUBLEEN_H_ */
/** @} */
