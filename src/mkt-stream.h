/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktStream
 * @ingroup  MktStream
 * @{
 * @file  mkt-stream.h	Stream interface model header
 * @brief This is Stream interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef __MKT_STREAM_H_
#define __MKT_STREAM_H_


#include "mkt-model.h"

G_BEGIN_DECLS

#define MKT_TYPE_STREAM                  (mkt_stream_get_type())
#define MKT_STREAM(obj)                  (G_TYPE_CHECK_INSTANCE_CAST((obj),MKT_TYPE_STREAM, MktStream))
#define MKT_IS_STREAM(obj)               (G_TYPE_CHECK_INSTANCE_TYPE((obj),MKT_TYPE_STREAM))
#define MKT_STREAM_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE((inst), MKT_TYPE_STREAM, MktStreamInterface))


typedef struct _MktStreamInterface MktStreamInterface;
typedef struct _MktStream          MktStream;



struct _MktStreamInterface
{
	GTypeInterface parent_iface;

	GSList*                 (*channels)                            ( MktStream *self  );
};


GType                   mkt_stream_get_type                        (void) G_GNUC_CONST;

GSList*                 mkt_stream_channels                        ( MktStream *stream );






G_END_DECLS

#endif /* _MKT_STREAM_H_ */

/** @} */
