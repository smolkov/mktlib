/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktParamboolean
 * @ingroup  MktParamboolean
 * @{
 * @file  mkt-paramboolean.h	PARAMBOOLEAN interface model header
 * @brief This is PARAMBOOLEAN interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef __MKT_PARAMBOOLEANEN_H__
#define __MKT_PARAMBOOLEANEN_H__

#include "mkt-model.h"


G_BEGIN_DECLS

#define MKT_TYPE_PARAMBOOLEAN                 (mkt_paramboolean_get_type ())
#define MKT_PARAMBOOLEAN(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_PARAMBOOLEAN, MktParamboolean))
#define MKT_IS_PARAMBOOLEAN(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_PARAMBOOLEAN))
#define MKT_PARAMBOOLEAN_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_PARAMBOOLEAN, MktParambooleanInterface))



typedef struct _MktParamboolean               MktParamboolean; /* dummy object */
typedef struct _MktParambooleanInterface      MktParambooleanInterface;


struct _MktParambooleanInterface
{
  GTypeInterface parent_iface;
  gboolean              (*get_value)                        ( MktParamboolean *self );
};


GType                     mkt_paramboolean_get_type          ( void );

gboolean                  mkt_paramboolean_value             ( MktParamboolean *paramboolean );

MktParamboolean*          mkt_paramboolean_get               ( const gchar *id, const gchar *path, const gchar *name );
gboolean                  mkt_paramboolean_get_value         ( const gchar *id, const gchar *path, const gchar *name  );


G_END_DECLS

#endif /* _MKT_PARAMBOOLEANEN_H_ */
/** @} */
