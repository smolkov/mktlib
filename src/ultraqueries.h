/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * ultraqueries.h
 * Copyright (C) LAR 2017
 *

 */

#ifndef _ULTRA_QUERIES_H_
#define _ULTRA_QUERIES_H_

#include "ultrameas.h"
#include "larresult.h"
#include "ultrameasurement.h"
#include "larintgrec.h"
#include <glib-object.h>
#include <gio/gio.h>


G_BEGIN_DECLS

#define TYPE_ULTRA_QUERIES (ultraqueries_get_type())
#define ULTRA_QUERIES(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_ULTRA_QUERIES, UltraQueries))
#define ULTRA_QUERIES_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_ULTRA_QUERIES, UltraQueriesClass))
#define IS_ULTRA_QUERIES(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_ULTRA_QUERIES))
#define IS_ULTRA_QUERIES_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_ULTRA_QUERIES))
#define ULTRA_QUERIES_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_ULTRA_QUERIES, UltraQueriesClass))

typedef struct _UltraQueriesClass   UltraQueriesClass;
typedef struct _UltraQueries        UltraQueries;
typedef struct _UltraQueriesPrivate UltraQueriesPrivate;


enum 
{
    ULTRA_DATA_DEBUG                 = 0,
    ULTRA_DATA_ONLINE                = 1,
    ULTRA_DATA_SINGLE                = 2,
    ULTRA_DATA_CHECK                 = 3,
};


struct _UltraQueriesClass {
    GObjectClass parent_class;
};

struct _UltraQueries {
    GObject              parent_instance;
    UltraQueriesPrivate *priv;
};

GType ultraqueries_get_type(void) G_GNUC_CONST;


UltraQueries *UltraQueriesNew(const gchar *path);
gboolean m_UltraQueriesCreateTables(UltraQueries *queries);
LarResult *m_UltraQueriesChannelLastResult(UltraQueries *queries, guint64 channel);
UltraMeasurement *m_UltraQueriesResultMeas(UltraQueries *queries, LarResult *result);
gboolean m_UltraQueriesInsertMeasurement(UltraQueries *queries, LarResult *result,UltraMeasurement *measurement);
gboolean m_UltraQueriesInsertResult(UltraQueries *queries, LarResult *result);
GSList *m_UltraQueriesChannelResultsInterval(UltraQueries *queries, guint64 channel,gint64 from,gint64 to);
GSList* m_UltraQueriesResultAsyncFinish(GAsyncResult *result, GError **error);
void m_UltraQueriesChannelResultsIntervalAsync(UltraQueries *queries, GCancellable *cancellable, GAsyncReadyCallback callback, gpointer user_data,guint64 channel,gint64 from,gint64 to );
LarDatas* m_UltraQueriesChannelDatasAsyncFinish(GAsyncResult *result, GError **error);
gboolean m_UltraQueriesChannelDatasAsync(UltraQueries *queries, GCancellable *cancellable, GAsyncReadyCallback callback, gpointer userData, gint64 from, gint64 to);
LarDatas *m_UltraQueriesChannelDatas(UltraQueries *queries, gint64 from, gint64 to);
void m_UltraQueriesSetStream(UltraQueries *queries, guint64 channel);
void m_UltraQueriesSetChannel(UltraQueries *queries, guint64 channel);
void m_UltraQueriesSetLimits(UltraQueries *queries, guint limit, guint offset);

void ultra_queries_result(UltraQueries *query);
void ultra_queries_measurement(UltraQueries *query);
void ultra_queries_online(UltraQueries *query);
void ultra_queries_check(UltraQueries *query);
void ultra_queries_offline(UltraQueries *query);
void ultra_queries_debug(UltraQueries *query);
void ultra_queries_stream(UltraQueries *query, guint64 stream);
void ultra_queries_channel(UltraQueries *query, guint64 channel);
void ultra_queries_interval(UltraQueries *query,gint64 from, gint64 to);



G_END_DECLS

#endif /* _ULTRA_QUERIES_H_ */
