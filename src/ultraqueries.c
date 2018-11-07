/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * ultraqueries.c
 * Copyright (C) LAR 2017
 *
 */

#include "ultraqueries.h"
#include "larsqlite.h"
#include <errno.h>
#include <glib/gstdio.h>
#include <glib.h>
// #include "value.h"

enum
{
    PROP_0,
    PROP_PATH,
};

// GType ultra_data_cathegory_type(void)
// {
//     static GType the_type = 0;

//     if (the_type == 0)
//     {
//         static const GEnumValue values[] =
//             {
//                 ULTRA_DATA_DEBUG,
//                 "debug_results",
//                 "debug_measurement",
//                 ULTRA_DATA_SINGLE,
//                 "single_results",
//                 "single_measurement",
//                 ULTRA_DATA_ONLINE,
//                 "online_results",
//                 "online_measurements",
//                 ULTRA_DATA_CHECK,
//                 "check_results",
//                 "check_measurements",
//             };

//         the_type = g_flags_register_static(
//             g_intern_static_string("UltraTable"),
//             values);
//     }
//     return the_type;
// }

typedef enum {
    MEASUREMENT,
    RESULTS
} UltraValues;

enum
{
    DEBUG,
    SINGLE,
    ONLINE,
    CHECK
};
typedef struct
{
    GType t;
    const gchar *n[4];

} QueryTable;

static const char *measDBPath = "/usr/share/tera/db/measurements.sqlite";

static QueryTable QTABLE[RESULTS + 1] = {
    {0,
     {"debug_measurement",
      "single_measurement",
      "online_measurement",
      "check_measurement"}},
    {0,
     {"debug_results",
      "single_results",
      "online_results",
      "check_results"}}};

struct _UltraQueriesPrivate
{
    LarSqlite *connection;
    gchar *path;
    guint64 stream;
    guint64 channel;
    gchar *lim;
    GType object_type;
    guint op : 4;
    guint vt : 2;
    gchar *table;
    GString *sql_par;
};
static GQuark errorQueryQuark(void)
{
    static GQuark error;
    if (!error)
        error = g_quark_from_static_string("query-results");
    return error;
}
#define ULTRA_QUERIES_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_ULTRA_QUERIES, UltraQueriesPrivate))

G_DEFINE_TYPE(UltraQueries, ultraqueries, G_TYPE_OBJECT)

static void reconfigure(UltraQueries *q)
{
    static gboolean init = 0;
    if (init == 0)
    {
        QTABLE[MEASUREMENT].t = TYPE_LAR_RESULT;
        QTABLE[RESULTS].t = TYPE_ULTRA_MEASUREMENT;
    }
    if (q->priv->table != NULL)
        g_free(q->priv->table);
    q->priv->table = g_strdup(QTABLE[q->priv->vt].n[q->priv->op]);
    q->priv->object_type = QTABLE[q->priv->vt].t;
}

static void ultraqueries_init(UltraQueries *ultraqueries)
{
    UltraQueriesPrivate *priv = ULTRA_QUERIES_PRIVATE(ultraqueries);
    priv->path = NULL;
    priv->lim = g_strdup(";");
    priv->sql_par = g_string_new("");
    priv->vt = RESULTS;
    priv->op = ONLINE;
    ultraqueries->priv = priv;
    reconfigure(ultraqueries);
}
static void ultraqueries_constructed(GObject *object)
{
    UltraQueries *uqueries = ULTRA_QUERIES(object);
    uqueries->priv->connection = LarSqliteNew(uqueries->priv->path, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    // char mode[] = "0777";
    // gint64 i;
    // i = g_ascii_strtoll(mode, NULL, 8);
    // if (g_chmod(uqueries->priv->path, i) < 0)
    // {
    //     g_critical("error in chmod(%s, %s) - %d (%s)\n", uqueries->priv->path, mode, errno, g_strerror(errno));
    // }
}

static void ultraqueries_finalize(GObject *object)
{
    UltraQueries *uqueries = ULTRA_QUERIES(object);
    if (uqueries->priv->connection)
        g_object_unref(uqueries->priv->connection);
    if (uqueries->priv->path)
        g_free(uqueries->priv->path);
    if (uqueries->priv->lim)
        g_free(uqueries->priv->lim);
    if (uqueries->priv->table)
        g_free(uqueries->priv->table);
    if (uqueries->priv->sql_par)
        g_string_free(uqueries->priv->sql_par, TRUE);
    G_OBJECT_CLASS(ultraqueries_parent_class)->finalize(object);
}

static void ultraqueries_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_ULTRA_QUERIES(object));
    UltraQueries *uqueries = ULTRA_QUERIES(object);
    switch (prop_id)
    {
    case PROP_PATH:
        if (uqueries->priv->path)
            g_free(uqueries->priv->path);
        uqueries->priv->path = g_value_dup_string(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void ultraqueries_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_ULTRA_QUERIES(object));
    UltraQueries *uqueries = ULTRA_QUERIES(object);
    switch (prop_id)
    {
    case PROP_PATH:
        g_value_set_string(value, uqueries->priv->path);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void ultraqueries_class_init(UltraQueriesClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // ModelClass* parent_class = MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(UltraQueriesPrivate));
    object_class->finalize = ultraqueries_finalize;
    object_class->set_property = ultraqueries_set_property;
    object_class->get_property = ultraqueries_get_property;
    object_class->constructed = ultraqueries_constructed;
    g_object_class_install_property(object_class, PROP_PATH, g_param_spec_string("path", "Ultra measurements database path", "database path", measDBPath, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

/**
 * UltraQueriesNew:
 *
 * Ultra queries object.
 *
 * Return: a new instance of #UltraQueries
 */
UltraQueries *UltraQueriesNew(const gchar *path)
{
    UltraQueries *query = ULTRA_QUERIES(g_object_new(TYPE_ULTRA_QUERIES, "path", path != NULL ? path : measDBPath, NULL));
    return query;
}

/**
 * UltraQueriesNewData:
 *
 * Ultra queries object.
 *
 * Return: a new instance of #UltraQueries
 */
UltraQueries *UltraQueriesNewResults()
{
    UltraQueries *query = ULTRA_QUERIES(g_object_new(TYPE_ULTRA_QUERIES, "path", measDBPath, NULL));
    query->priv->vt = RESULTS;
    query->priv->op = ONLINE;
    reconfigure(query);
    return query;
}

/**
 * m_UltraQueriesCreateTables:
 *
 * Create measurement and result table if not exist
 *
 * Return: true if ok.
 */

gboolean m_UltraQueriesCreateTables(UltraQueries *queries)
{
    g_return_val_if_fail(queries != NULL, FALSE);
    g_return_val_if_fail(IS_ULTRA_QUERIES(queries), FALSE);
    GObject *result = g_object_new(TYPE_LAR_RESULT, NULL);
    GObject *meas = g_object_new(TYPE_ULTRA_MEASUREMENT, NULL);
    m_LarSqliteCreateTable(queries->priv->connection, QTABLE[RESULTS].n[SINGLE], result);
    m_LarSqliteCreateTable(queries->priv->connection, QTABLE[RESULTS].n[ONLINE], result);
    m_LarSqliteCreateTable(queries->priv->connection, QTABLE[RESULTS].n[CHECK], result);
    m_LarSqliteCreateTable(queries->priv->connection, QTABLE[RESULTS].n[DEBUG], result);
    m_LarSqliteCreateTable(queries->priv->connection, QTABLE[MEASUREMENT].n[SINGLE], meas);
    m_LarSqliteCreateTable(queries->priv->connection, QTABLE[MEASUREMENT].n[ONLINE], meas);
    m_LarSqliteCreateTable(queries->priv->connection, QTABLE[MEASUREMENT].n[CHECK], meas);
    m_LarSqliteCreateTable(queries->priv->connection, QTABLE[MEASUREMENT].n[DEBUG], meas);
    g_object_unref(result);
    g_object_unref(meas);
    return TRUE;
}
/**
 * m_UltraQueriesQueriesChannelLastResult:
 * @queries: a #UltraQueries
 * @channel a channel number
 * Last completed result
 *
 * Return: a new instance of #LarResult
 */
LarResult *m_UltraQueriesChannelLastResult(UltraQueries *queries, guint64 channel)
{
    g_return_val_if_fail(queries != NULL, NULL);
    g_return_val_if_fail(IS_ULTRA_QUERIES(queries), NULL);
    ultra_queries_result(queries);
    GSList *l = m_LarSqliteSelectObjects(queries->priv->connection, TYPE_LAR_RESULT, "select * from %s where channel = %" G_GUINT64_FORMAT " ORDER BY %s DESC LIMIT 1", queries->priv->table, channel, SQLITE_PK_PROPERTY);
    LarResult *res = NULL;
    if (l != NULL && l->data != NULL && IS_LAR_RESULT(l->data))
    {
        res = g_object_ref(l->data);
        g_slist_free_full(l, g_object_unref);
    }
    return res;
}

/**
 * m_UltraQueriesQueriesResultReplicates:
 * @queries: a #UltraQueries
 * Result measurement object.
 *
 * Return: a new #UltraMeasurement
 */

UltraMeasurement *m_UltraQueriesResultMeas(UltraQueries *queries, LarResult *result)
{
    g_return_val_if_fail(queries != NULL, NULL);
    g_return_val_if_fail(result != NULL, NULL);
    g_return_val_if_fail(IS_ULTRA_QUERIES(queries), NULL);
    ultra_queries_measurement(queries);
    guint64 resId = m_LarResultGetKey(result);
    GSList *ml = m_LarSqliteSelectObjects(queries->priv->connection, TYPE_ULTRA_MEASUREMENT, "select * from %s where result = %" G_GUINT64_FORMAT " ORDER BY %s DESC LIMIT 1", queries->priv->table, resId, SQLITE_PK_PROPERTY);
    UltraMeasurement *measurement = NULL;
    if (ml != NULL && ml->data != NULL && IS_LAR_RESULT(ml->data))
    {
        measurement = g_object_ref(ml->data);
        g_slist_free_full(ml, g_object_unref);
    }
    return measurement;
}

/**
 * m_UltraQueriesInsertMeasurement:
 * @queries: a #UltraQueries
 * @result: a #LarResult
 * @measurement: a #Ultrameas
 * Insert new measurement.
 *
 * Return: TRUE if ok.
 */
gboolean m_UltraQueriesInsertMeasurement(UltraQueries *queries, LarResult *result, UltraMeasurement *measurement)
{
    g_return_val_if_fail(queries != NULL, FALSE);
    g_return_val_if_fail(result != NULL, FALSE);
    g_return_val_if_fail(measurement != NULL, FALSE);
    g_return_val_if_fail(IS_ULTRA_QUERIES(queries), FALSE);
    ultra_queries_measurement(queries);
    // g_debug("Insert measurement.. activ table %s",queries->priv->table);
    guint64 resId = m_LarResultGetKey(result);
    g_object_set(measurement, "result", resId, NULL);
    guint64 id = m_LarSqliteInsertObject(queries->priv->connection, queries->priv->table, G_OBJECT(measurement));
    return id != 0;
}

/**
 * m_UltraQueriesInsertResult:
 * @queries: a #UltraQueries
 * @result: a #LarResult
 * @measurement: a #Ultrameas
 * Insert new measurement.
 *
 * Return: TRUE if ok.
 */
gboolean m_UltraQueriesInsertResult(UltraQueries *queries, LarResult *result)
{
    g_return_val_if_fail(queries != NULL, FALSE);
    g_return_val_if_fail(result != NULL, FALSE);
    g_return_val_if_fail(IS_ULTRA_QUERIES(queries), FALSE);
    ultra_queries_result(queries);
    guint64 id = m_LarSqliteInsertObject(queries->priv->connection, queries->priv->table, G_OBJECT(result));
    return id != 0;
}

/**
 * m_UltraQueriesInsertResult:
 * @queries: a #UltraQueries
 * @channel: a #guint64
 * @from: a #gint64 unix time in microseconds.
 * @to: a #gint64 unix time in microseconds.
 * Select results . 
 *
 * Return: GSList of a #LarResult objects.
 */
GSList *m_UltraQueriesChannelResultsInterval(UltraQueries *queries, guint64 channel, gint64 from, gint64 to)
{
    g_return_val_if_fail(queries != NULL, NULL);
    g_return_val_if_fail(IS_ULTRA_QUERIES(queries), NULL);
    ultra_queries_result(queries);
    GSList *results = m_LarSqliteSelectObjects(queries->priv->connection, TYPE_LAR_RESULT, "select * from %s where channel = %" G_GUINT64_FORMAT " and changed >= %" G_GINT64_FORMAT " and changed <= %" G_GINT64_FORMAT " ORDER BY changed ASC%s", queries->priv->table, channel, from, to, queries->priv->lim);
    return results;
}

static void queriesSelectResultThread(GTask *task, gpointer source_object, gpointer task_data, GCancellable *cancellable)
{
    UltraQueries *queries = ULTRA_QUERIES(source_object);
    gchar *sql = (gchar *)g_task_get_task_data(task);
    if (sql == NULL)
    {
        g_task_return_error(task, g_error_new(errorQueryQuark(), 10, "sql string should not be nil"));
        return;
    }
    if (g_task_return_error_if_cancelled(task))
        return;
    GSList *results = m_LarSqliteSelectObjects(queries->priv->connection, TYPE_LAR_RESULT, "%s;", sql);
    if (g_task_return_error_if_cancelled(task))
    {
        if (results)
        {
            g_slist_free_full(results, g_object_unref);
        }
        return;
    }
    g_task_return_pointer(task, results, (GDestroyNotify)g_object_unref);
    return;
}

GSList *m_UltraQueriesResultAsyncFinish(GAsyncResult *result, GError **error)
{
    return (GSList *)g_task_propagate_pointer(G_TASK(result), error);
}
/**
 * m_UltraQueriesInsertResult:
 * @queries: a #UltraQueries
 * @cancellable: a #GCallback
 * @callback: a #GAsyncReadyCallback
 * @gpointer: a user data pointer
 * @channel: a #guint64
 * @from: a #gint64 unix time in microseconds.
 * @to: a #gint64 unix time in microseconds.
 * Select async results. 
 */
void m_UltraQueriesChannelResultsIntervalAsync(UltraQueries *queries, GCancellable *cancellable, GAsyncReadyCallback callback, gpointer userData, guint64 channel, gint64 from, gint64 to)
{
    g_return_if_fail(queries != NULL);
    g_return_if_fail(IS_ULTRA_QUERIES(queries));
    ultra_queries_result(queries);
    gchar *sql = g_strdup_printf("select changed,value from %s where channel = %" G_GUINT64_FORMAT " and changed >= %" G_GINT64_FORMAT " and changed <= %" G_GINT64_FORMAT " ORDER BY changed ASC%s", queries->priv->table, queries->priv->channel, from, to, queries->priv->lim);
    GTask *task = g_task_new(queries, cancellable, callback, userData);
    g_task_set_task_data(task, sql, g_free);
    g_task_run_in_thread(task, queriesSelectResultThread);
    g_object_unref(task);
}

void m_UltraQueriesSetStream(UltraQueries *queries, guint64 channel)
{
    g_return_if_fail(queries != NULL);
    g_return_if_fail(IS_ULTRA_QUERIES(queries));
    queries->priv->stream = channel;
}

void m_UltraQueriesSetChannel(UltraQueries *queries, guint64 channel)
{
    g_return_if_fail(queries != NULL);
    g_return_if_fail(IS_ULTRA_QUERIES(queries));
    queries->priv->channel = channel;
}

void m_UltraQueriesSetLimits(UltraQueries *queries, guint limit, guint offset)
{
    g_return_if_fail(queries != NULL);
    g_return_if_fail(IS_ULTRA_QUERIES(queries));
    g_free(queries->priv->lim);
    if (limit > 0 && offset > 0)
    {

        queries->priv->lim = g_strdup_printf("LIMIT %d OFFSET %d;", limit, offset);
    }
    else if (limit > 0)
    {
        queries->priv->lim = g_strdup_printf("LIMIT %d;", limit);
    }
    else
    {
        queries->priv->lim = g_strdup(";");
    }
}
static int ultraQueriesSelectLarDatas(void *data, int argc, char **argv, char **azColName)
{
    LarDatas *datas = (LarDatas *)data;
    if (datas == NULL)
        return -1;
    if (argc < 2)
        return -1;

    gdouble X = g_ascii_strtod(argv[0], NULL);
    gdouble Y = g_ascii_strtod(argv[1], NULL);
    m_LarDatasAddValues(datas, X, Y);
    return 0;
}

LarDatas *m_UltraQueriesChannelDatas(UltraQueries *queries, gint64 from, gint64 to)
{
    g_return_val_if_fail(queries != NULL, NULL);
    g_return_val_if_fail(IS_ULTRA_QUERIES(queries), NULL);
    g_return_val_if_fail(queries->priv->channel > 0, NULL);
    LarDatas *datas = LarDatasNew();
    if (!m_LarSqliteSelectCallback(queries->priv->connection, ultraQueriesSelectLarDatas, datas, "select changed,value from %s where channel = %" G_GUINT64_FORMAT " and changed >= %" G_GINT64_FORMAT " and changed <= %" G_GINT64_FORMAT " ORDER BY changed ASC%s", queries->priv->table, queries->priv->channel, from, to, queries->priv->lim))
    {
        m_LarDatasFree(datas);
        return NULL;
    }
    return datas;
}
static void queriesSelectDatasThread(GTask *task, gpointer source_object, gpointer task_data, GCancellable *cancellable)
{
    UltraQueries *queries = ULTRA_QUERIES(source_object);
    gchar *sql = (gchar *)g_task_get_task_data(task);
    if (sql == NULL)
    {
        g_task_return_error(task, g_error_new(errorQueryQuark(), 10, "sql string should not be nil"));
        return;
    }
    if (g_task_return_error_if_cancelled(task))
        return;
    LarDatas *datas = LarDatasNew();
    if (!m_LarSqliteSelectCallback(queries->priv->connection, ultraQueriesSelectLarDatas, datas, "%s", sql))
    {
        m_LarDatasFree(datas);
        return;
    }
    if (g_task_return_error_if_cancelled(task))
    {
        if (datas)
        {
            m_LarDatasFree(datas);
        }
        return;
    }
    g_task_return_pointer(task, datas, (GDestroyNotify)m_LarDatasFree);
    return;
}

LarDatas *m_UltraQueriesChannelDatasAsyncFinish(GAsyncResult *result, GError **error) { return (LarDatas *)g_task_propagate_pointer(G_TASK(result), error); }

gboolean m_UltraQueriesChannelDatasAsync(UltraQueries *queries, GCancellable *cancellable, GAsyncReadyCallback callback, gpointer userData, gint64 from, gint64 to)
{
    g_return_val_if_fail(queries != NULL, FALSE);
    g_return_val_if_fail(IS_ULTRA_QUERIES(queries), FALSE);
    g_return_val_if_fail(queries->priv->channel > 0, FALSE);
    gchar *sql = g_strdup_printf("select changed,value from %s where channel = %" G_GUINT64_FORMAT " and changed >= %" G_GINT64_FORMAT " and changed <= %" G_GINT64_FORMAT " ORDER BY changed ASC%s", queries->priv->table, queries->priv->channel, from, to, queries->priv->lim);
    GTask *task = g_task_new(queries, cancellable, callback, userData);
    g_task_set_task_data(task, sql, g_free);
    g_task_run_in_thread(task, queriesSelectDatasThread);
    g_object_unref(task);
    return TRUE;
}

void ultra_queries_result(UltraQueries *query)
{
    g_return_if_fail(IS_ULTRA_QUERIES(query));
    query->priv->vt = RESULTS;
    reconfigure(query);
}
void ultra_queries_measurement(UltraQueries *query)
{
    g_return_if_fail(IS_ULTRA_QUERIES(query));
    query->priv->vt = MEASUREMENT;
    reconfigure(query);
}
void ultra_queries_online(UltraQueries *query)
{
    g_return_if_fail(IS_ULTRA_QUERIES(query));
    query->priv->op = ONLINE;
    reconfigure(query);
}
void ultra_queries_offline(UltraQueries *query)
{
    g_return_if_fail(IS_ULTRA_QUERIES(query));
    query->priv->op = SINGLE;
    reconfigure(query);
}
void ultra_queries_check(UltraQueries *query)
{
    g_return_if_fail(IS_ULTRA_QUERIES(query));
    query->priv->op = CHECK;
    reconfigure(query);
}
void ultra_queries_debug(UltraQueries *query)
{
    g_return_if_fail(IS_ULTRA_QUERIES(query));
    query->priv->op = DEBUG;
    reconfigure(query);
}
void ultra_queries_stream(UltraQueries *query, guint64 stream)
{
    g_return_if_fail(IS_ULTRA_QUERIES(query));
    query->priv->stream = stream;
    g_return_if_fail(g_strrstr(query->priv->sql_par->str, "stream") == NULL);
    if (query->priv->sql_par->len > 1)
        g_string_append_printf(query->priv->sql_par, "where stream =%" G_GUINT64_FORMAT, stream);
    else
        g_string_append_printf(query->priv->sql_par, " and stream =%" G_GUINT64_FORMAT, stream);
}
void ultra_queries_channel(UltraQueries *query, guint64 channel)
{
    g_return_if_fail(IS_ULTRA_QUERIES(query));
    query->priv->channel = channel;
    g_return_if_fail(g_strrstr(query->priv->sql_par->str, "channel") == NULL);
    if (query->priv->sql_par->len > 1)
        g_string_append_printf(query->priv->sql_par, "where channel=%" G_GUINT64_FORMAT, channel);
    else
        g_string_append_printf(query->priv->sql_par, " and channel=%" G_GUINT64_FORMAT, channel);
}
void ultra_queries_interval(UltraQueries *query, gint64 from, gint64 to)
{
    g_return_if_fail(IS_ULTRA_QUERIES(query));
    g_return_if_fail(g_strrstr(query->priv->sql_par->str, "changed") == NULL);
    if (query->priv->sql_par->len > 1)
        g_string_append_printf(query->priv->sql_par, " changed >= %" G_GINT64_FORMAT " and changed <= %" G_GINT64_FORMAT, from, to);
    else
        g_string_append_printf(query->priv->sql_par, "where changed >= %" G_GINT64_FORMAT " and changed <= %" G_GINT64_FORMAT, from, to);
}