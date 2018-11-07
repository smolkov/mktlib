/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktChannelModel
 * @ingroup  MktChannelModel
 * @{
 * @file  mkt-channel.h	Channel interface model header
 * @brief This is Channel interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef _MKT__H_CHANNEL_MODEL
#define _MKT__H_CHANNEL_MODEL

#include "mkt-model.h"
#include "mkt-channel.h"
#include "mkt-param-object.h"


G_BEGIN_DECLS

#define MKT_TYPE_CHANNEL_MODEL             (mkt_channel_model_get_type ())
#define MKT_CHANNEL_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_CHANNEL_MODEL, MktChannelModel))
#define MKT_CHANNEL_MODEL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), MKT_TYPE_CHANNEL_MODEL, MktChannelModelClass))
#define MKT_IS_CHANNEL_MODEL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_CHANNEL_MODEL))
#define MKT_IS_CHANNEL_MODEL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), MKT_TYPE_CHANNEL_MODEL))
#define MKT_CHANNEL_MODEL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), MKT_TYPE_CHANNEL_MODEL, MktChannelModelClass))

typedef struct _MktChannelModelClass   MktChannelModelClass;
typedef struct _MktChannelModel        MktChannelModel;
typedef struct _MktChannelModelPrivate MktChannelModelPrivate;


struct _MktChannelModelClass
{
	MktParamObjectClass           parent_class;
};

struct _MktChannelModel
{
	MktParamObject                parent_instance;
	MktChannelModelPrivate       *priv;
};

GType                   mkt_channel_model_get_type                 (void) G_GNUC_CONST;

G_END_DECLS

#endif /* _MKT__H_CHANNEL_MODEL */

/** @} */
