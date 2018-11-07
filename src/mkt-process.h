/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktProcess
 * @ingroup  MktProcess
 * @{
 * @file  mkt-process.h	Channel interface model header
 * @brief This is Channel interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef __MKT_PROCESS_H_
#define __MKT_PROCESS_H_


#include "mkt-model.h"
#include "mkt-decl.h"
#include "mkt-measurement.h"
#include "mkt-stream.h"

G_BEGIN_DECLS

#define MKT_TYPE_PROCESS                  (mkt_process_get_type ())
#define MKT_PROCESS(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj),MKT_TYPE_PROCESS, MktProcess))
#define MKT_IS_PROCESS(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj),MKT_TYPE_PROCESS))
#define MKT_PROCESS_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_PROCESS, MktProcessInterface))


typedef struct _MktProcessInterface MktProcessInterface;
typedef struct _MktProcess          MktProcess;


struct _MktProcessInterface
{
	GTypeInterface parent_iface;
	const gchar*            (*process_name)                         ( MktProcess *self );
	const gchar*            (*process_description)                  ( MktProcess *self );
	const gchar*            (*process_kind)                         ( MktProcess *self );
	guint64                 (*process_stream)                       ( MktProcess *self );
	guint                   (*process_run)                          ( MktProcess *self );
	guint                   (*process_identification)               ( MktProcess *self );
	guint                   (*process_wait_action)                  ( MktProcess *self );
	gdouble                 (*process_start)                        ( MktProcess *self );
	gdouble                 (*process_stop)                         ( MktProcess *self );
};


GType                   mkt_process_get_type                        ( void ) G_GNUC_CONST;
const gchar*            mkt_process_name                            ( MktProcess *process );
const gchar*            mkt_process_description                     ( MktProcess *process );
const gchar*            mkt_process_kind                            ( MktProcess *process );
guint                   mkt_process_run                             ( MktProcess *process );
guint                   mkt_process_identification                  ( MktProcess *process );
guint                   mkt_process_wait_action                     ( MktProcess *process );

guint64                 mkt_process_stream                          ( MktProcess *process );
gdouble                 mkt_process_start                           ( MktProcess *process );
gdouble                 mkt_process_stop                            ( MktProcess *process );
void                    mkt_process_update_start                    ( MktProcess *process );
void                    mkt_process_update_stop                     ( MktProcess *process );

MktStream*              mkt_process_get_stream                      ( MktProcess *process );
void                    mkt_process_run_analyse                     ( MktProcess *process );
void                    mkt_process_run_prepare                     ( MktProcess *process );




G_END_DECLS

#endif /* __MKT_PROCESS_H_ */

/** @} */
