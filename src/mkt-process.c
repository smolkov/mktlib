/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktProcess
 * @{
 * @file  mkt-process.c	Channel model interface
 * @brief This is Channel model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */

#include "mkt-process.h"
#include "market-time.h"
#include "mkt-process-model.h"
#include "mkt-stream-model.h"
#include "../config.h"
#include <glib/gi18n-lib.h>

#if GLIB_CHECK_VERSION(2,31,7)
static GRecMutex init_rmutex;
#define MUTEX_LOCK() g_rec_mutex_lock(&init_rmutex)
#define MUTEX_UNLOCK() g_rec_mutex_unlock(&init_rmutex)
#else
static GStaticRecMutex init_mutex = G_STATIC_REC_MUTEX_INIT;
#define MUTEX_LOCK() g_static_rec_mutex_lock(&init_mutex)
#define MUTEX_UNLOCK() g_static_rec_mutex_unlock(&init_mutex)
#endif

static void mkt_process_base_init(gpointer g_iface) {
	static gboolean is_process_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_process_initialized) {

		g_object_interface_install_property(g_iface,
				g_param_spec_string("process-path",
						"Process path",
						"Set get process path property",
						"----",
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT	| MKT_MODEL_DB_PROP ));

		g_object_interface_install_property(g_iface,
				g_param_spec_uint64("process-stream",
						"Process stream",
						"Set get process stream", 0,
						G_MAXUINT64, 0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT	| MKT_MODEL_DB_PROP));
		g_object_interface_install_property(g_iface,
				g_param_spec_uint("process-identification",
						"Process identification",
						"Process identification", 0,
						G_MAXUINT, 201,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT	| MKT_MODEL_DB_PROP));

		g_object_interface_install_property(g_iface,
				g_param_spec_string("process-name",
						"Process name",
						"Set get process name property",
						"Measurement",
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT	| MKT_MODEL_DB_PROP ));

		g_object_interface_install_property(g_iface,
				g_param_spec_string("process-description",
						"Process description",
						"Set get process description",
						"Measurement",
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT	| MKT_MODEL_DB_PROP ));

		g_object_interface_install_property(g_iface,
				g_param_spec_string("process-kind",
						"Process kind",
						"Set get process kind",
						"M",
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT	| MKT_MODEL_DB_PROP ));

		g_object_interface_install_property(g_iface,
				g_param_spec_double("process-start",
						"Process start time",
						"Set get process start",
						-G_MAXDOUBLE,G_MAXDOUBLE, 0.0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP ));

		g_object_interface_install_property(g_iface,
				g_param_spec_double("process-stop",
						"Process stop time",
						"Set get process stop time",
						-G_MAXDOUBLE,G_MAXDOUBLE, 0.0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT	| MKT_MODEL_DB_PROP ));

		g_object_interface_install_property(g_iface,
				g_param_spec_uint("process-run",
						"Channel measurement",
						"Set get process measurement property",
						0,G_MAXUINT32, 0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT	| MKT_MODEL_DB_PROP));

		g_object_interface_install_property(g_iface,
				g_param_spec_string("process-date",
						"Process date",
						"Set get process date",
						"1983.05.01T08:43:12:099898919",
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT	| MKT_MODEL_DB_PROP ));

		g_object_interface_install_property(g_iface,
				g_param_spec_uint("process-wait-action",
						"process wait action",
						"Set get process wait action",
						0,G_MAXUINT32, 0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT	| MKT_MODEL_DB_PROP));



		is_process_initialized = TRUE;
	}
	MUTEX_UNLOCK();
}

GType mkt_process_get_type(void) {
	static GType iface_type = 0;
	if (iface_type == 0) {
		static const GTypeInfo info = { sizeof(MktProcessInterface),
				(GBaseInitFunc) mkt_process_base_init, (GBaseFinalizeFunc) NULL,
				(GClassInitFunc) NULL,
				NULL,
				NULL, 0, 0, (GInstanceInitFunc) NULL, 0 };
		MUTEX_LOCK();
		if (iface_type == 0) {
			iface_type = g_type_register_static(G_TYPE_INTERFACE,
					"MktProcessInterface", &info, 0);
			g_type_interface_add_prerequisite(iface_type, MKT_TYPE_MODEL);
		}
		MUTEX_UNLOCK();
	}
	return iface_type;
}

const gchar*
mkt_process_name(MktProcess *process) {
	g_return_val_if_fail(process != NULL, NULL);
	g_return_val_if_fail(MKT_IS_PROCESS(process), NULL);
	if (MKT_PROCESS_GET_INTERFACE(process)->process_name)
		return MKT_PROCESS_GET_INTERFACE(process)->process_name(process);
	return NULL;
}
const gchar*
mkt_process_description(MktProcess *process) {
	g_return_val_if_fail(process != NULL, NULL);
	g_return_val_if_fail(MKT_IS_PROCESS(process), NULL);
	if (MKT_PROCESS_GET_INTERFACE(process)->process_description)
		return MKT_PROCESS_GET_INTERFACE(process)->process_description(process);
	return NULL;
}
const gchar*
mkt_process_kind(MktProcess *process) {
	g_return_val_if_fail(process != NULL, NULL);
	g_return_val_if_fail(MKT_IS_PROCESS(process), NULL);
	if (MKT_PROCESS_GET_INTERFACE(process)->process_kind)
		return MKT_PROCESS_GET_INTERFACE(process)->process_kind(process);
	return NULL;
}

guint
mkt_process_run(MktProcess *process) {
	g_return_val_if_fail(process != NULL, 0);
	g_return_val_if_fail(MKT_IS_PROCESS(process), 0);
	if (MKT_PROCESS_GET_INTERFACE(process)->process_run)
		return MKT_PROCESS_GET_INTERFACE(process)->process_run(process);
	return 0;
}

guint
mkt_process_identification ( MktProcess *process ){
	g_return_val_if_fail(process != NULL, 0);
	g_return_val_if_fail(MKT_IS_PROCESS(process), 0);
	if (MKT_PROCESS_GET_INTERFACE(process)->process_identification)
		return MKT_PROCESS_GET_INTERFACE(process)->process_identification(process);
	return 0;
}

guint
mkt_process_wait_action                     ( MktProcess *process )
{
	g_return_val_if_fail(process != NULL, 0);
	g_return_val_if_fail(MKT_IS_PROCESS(process), 0);
	if (MKT_PROCESS_GET_INTERFACE(process)->process_wait_action)
		return MKT_PROCESS_GET_INTERFACE(process)->process_wait_action(process);
	return 0;
}


guint64
mkt_process_stream(MktProcess *process) {
	g_return_val_if_fail(process != NULL, 0);
	g_return_val_if_fail(MKT_IS_PROCESS(process), 0);
	if (MKT_PROCESS_GET_INTERFACE(process)->process_stream)
		return MKT_PROCESS_GET_INTERFACE(process)->process_stream(process);
	return 0;
}

gdouble
mkt_process_start( MktProcess *process )
{
	g_return_val_if_fail(process != NULL , 0.0);
	g_return_val_if_fail(MKT_IS_PROCESS(process) ,  0.0);
	if(MKT_PROCESS_GET_INTERFACE(process)->process_start )
		return MKT_PROCESS_GET_INTERFACE(process)->process_start(process);
	return 0.0;
}
gdouble
mkt_process_stop( MktProcess *process )
{
	g_return_val_if_fail(process != NULL , 0.0);
	g_return_val_if_fail(MKT_IS_PROCESS(process) ,  0.0);
	if(MKT_PROCESS_GET_INTERFACE(process)->process_stop)
		return MKT_PROCESS_GET_INTERFACE(process)->process_stop(process);
	return 0.0;
}

void
mkt_process_update_start                    ( MktProcess *process )
{
	g_return_if_fail(process != NULL );
	g_return_if_fail(MKT_IS_PROCESS(process));
	gdouble time = market_db_time_now();
	const gchar * time_val = market_db_get_date_sqlite_format(time);
	g_object_set(process,"process-start",time,"process-stop",(time+10.0),"process-date",time_val,NULL);
}

void
mkt_process_update_stop                     ( MktProcess *process )
{
	g_return_if_fail(process != NULL );
	g_return_if_fail(MKT_IS_PROCESS(process));
	gdouble time = market_db_time_now();
	g_object_set(process,"process-stop",time,"process-run",0,NULL);
}


MktStream*
mkt_process_get_stream                      ( MktProcess *process )
{
	g_return_val_if_fail(process != NULL, NULL);
	g_return_val_if_fail(MKT_IS_PROCESS(process), NULL);
	MktStream *stream = MKT_STREAM(mkt_model_select_one(MKT_TYPE_STREAM_MODEL,"select * from $tablename where ref_id = %"G_GUINT64_FORMAT";",mkt_process_stream(process)));
	return stream;
}

void
mkt_process_run_analyse                     ( MktProcess *process )
{
	g_return_if_fail(process != NULL );
	g_return_if_fail(MKT_IS_PROCESS(process));
	g_object_set(process,"process-run",2,NULL);
}

void
mkt_process_run_prepare                     ( MktProcess *process )
{
	g_return_if_fail(process != NULL );
	g_return_if_fail(MKT_IS_PROCESS(process));
	g_object_set(process,"process-run",1,NULL);
}

/** @} */
