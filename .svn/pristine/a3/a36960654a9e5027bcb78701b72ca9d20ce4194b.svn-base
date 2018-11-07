/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktProcessModel
 * @ingroup  MktProcessModel
 * @{
 * @file  mkt-messung.h	Process interface model header
 * @brief This is Process interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef _MKT__H_PROCESS_MODEL
#define _MKT__H_PROCESS_MODEL

#include "mkt-model.h"
#include "mkt-param-object.h"
#include "mkt-process.h"


G_BEGIN_DECLS

#define MKT_TYPE_PROCESS_MODEL             (mkt_process_model_get_type ())
#define MKT_PROCESS_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_PROCESS_MODEL, MktProcessModel))
#define MKT_PROCESS_MODEL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), MKT_TYPE_PROCESS_MODEL, MktProcessModelClass))
#define MKT_IS_PROCESS_MODEL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_PROCESS_MODEL))
#define MKT_IS_PROCESS_MODEL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), MKT_TYPE_PROCESS_MODEL))
#define MKT_PROCESS_MODEL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), MKT_TYPE_PROCESS_MODEL, MktProcessModelClass))

typedef struct _MktProcessModelClass   MktProcessModelClass;
typedef struct _MktProcessModel        MktProcessModel;
typedef struct _MktProcessModelPrivate MktProcessModelPrivate;


struct _MktProcessModelClass
{
	MktModelClass                 parent_class;
};

struct _MktProcessModel
{
	MktModel                      parent_instance;
	MktProcessModelPrivate       *priv;
};

GType                             mkt_process_model_get_type                 (void) G_GNUC_CONST;

G_END_DECLS

#endif /* _MKT__H_PROCESS_MODEL */

/** @} */
