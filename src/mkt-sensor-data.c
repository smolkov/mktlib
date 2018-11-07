
#include "mkt-sensor-data.h"
#include <fcntl.h>
#include <glib/gstdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "market-time.h"

enum { PROP_0, PROP_CHANGED, PROP_VALUE, PROP_TRIGGER, PROP_STATE };

struct _MktSensorDataPrivate {
  gdouble changed;
  gdouble value;
  guint trigger;
  gint state;
};

GQuark mkt_sensor_data_quark() {
  static GQuark error;
  if (!error)
    error = g_quark_from_static_string("data-error");
  return error;
}

#define MKT_SENSOR_DATA_PRIVATE(o)                                             \
  (G_TYPE_INSTANCE_GET_PRIVATE((o), MKT_TYPE_SENSOR_DATA, MktSensorDataPrivate))

G_DEFINE_TYPE(MktSensorData, mkt_sensor_data, G_TYPE_OBJECT)

static void mkt_sensor_data_init(MktSensorData *mkt_sensor_data) {
  MktSensorDataPrivate *priv = MKT_SENSOR_DATA_PRIVATE(mkt_sensor_data);
  priv->changed = market_db_time_now();
  priv->value = 0.0;
  priv->trigger = 0;
  priv->state = 0;
  mkt_sensor_data->priv = priv;
  /* TODO: Add initialization code here */
}

static void mkt_sensor_data_finalize(GObject *object) {
  // MktSensorData *data = MKT_SENSOR_DATA(object);
  G_OBJECT_CLASS(mkt_sensor_data_parent_class)->finalize(object);
}

static void mkt_sensor_data_set_property(GObject *object, guint prop_id,
                                         const GValue *value,
                                         GParamSpec *pspec) {
  g_return_if_fail(MKT_IS_SENSOR_DATA(object));
  MktSensorData *data = MKT_SENSOR_DATA(object);
  switch (prop_id) {
  case PROP_CHANGED:
    data->priv->changed = g_value_get_double(value);
    break;
  case PROP_VALUE:
    data->priv->value = g_value_get_double(value);
    break;
  case PROP_TRIGGER:
    data->priv->trigger = g_value_get_uint(value);
    break;
  case PROP_STATE:
    data->priv->state = g_value_get_uint(value);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void mkt_sensor_data_get_property(GObject *object, guint prop_id,
                                         GValue *value, GParamSpec *pspec) {
  g_return_if_fail(MKT_IS_SENSOR_DATA(object));
  MktSensorData *data = MKT_SENSOR_DATA(object);
  switch (prop_id) {
  case PROP_CHANGED:
    g_value_set_double(value, data->priv->changed);
    break;
  case PROP_VALUE:
    g_value_set_double(value, data->priv->value);
    break;
  case PROP_TRIGGER:
    g_value_set_uint(value, data->priv->trigger);
    break;
  case PROP_STATE:
    g_value_set_uint(value, data->priv->state);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void mkt_sensor_data_class_init(MktSensorDataClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  // MktModelClass* parent_class = MKT_MODEL_CLASS (klass);
  g_type_class_add_private(klass, sizeof(MktSensorDataPrivate));
  object_class->finalize = mkt_sensor_data_finalize;
  object_class->set_property = mkt_sensor_data_set_property;
  object_class->get_property = mkt_sensor_data_get_property;

  g_object_class_install_property(
      object_class, PROP_CHANGED,
      g_param_spec_double("changed", "changed", "Set get sensor data property",
                          0.0, G_MAXDOUBLE, market_db_time_now(),
                          G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
  g_object_class_install_property(
      object_class, PROP_VALUE,
      g_param_spec_double("value", "Data data property",
                          "Set get sensor data property", -G_MAXDOUBLE,
                          G_MAXDOUBLE, 0.,
                          G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
  g_object_class_install_property(
      object_class, PROP_TRIGGER,
      g_param_spec_uint("trigger", "Data data property",
                        "Set get sensor data property", 0, G_MAXINT32, 0,
                        G_PARAM_READABLE | G_PARAM_WRITABLE |
                            G_PARAM_CONSTRUCT));
  g_object_class_install_property(
      object_class, PROP_STATE,
      g_param_spec_uint("state", "Data data state",
                        "Set get sensor state property", 0, G_MAXUINT32, 0,
                        G_PARAM_READABLE | G_PARAM_WRITABLE |
                            G_PARAM_CONSTRUCT));
}
typedef struct {
  gdouble from;
  gdouble to;
  guint64 channel;
  guint trigger;
  guint records;
  guint offset;
  guint limit;
} Query;

Query *new_query() {
  Query *query = g_new(Query, 1);
  query->channel = 0;
  query->trigger = 0;
  query->offset = 0;
  query->limit = 0;
  query->records = 0;
  query->from = 0.0;
  query->to = 0.0;
  return query;
}

void free_query(gpointer query) {
  if (query)
    g_free(query);
}

gchar *query_gen_where(Query *q) {
  GString *where = g_string_new("");
  GList *wchl = NULL;
  if (q->trigger > 0) {
    wchl = g_list_append(wchl, g_strdup_printf("trigger = %u", q->trigger));
  }
  if (q->from > 0.0) {
    wchl = g_list_append(wchl, g_strdup_printf("changed > %f", q->from));
  }
  if (q->to > 0.0) {
    wchl = g_list_append(wchl, g_strdup_printf("to < %f", q->to));
  }
  if (wchl != NULL) {
    g_string_append(where, "where ");
    GList *l = wchl;
    for (l = wchl; l != NULL; l = l->next) {
      g_string_append_printf(where, "%s%s", l->prev != NULL ? " and " : "",
                             ((const gchar *)l->data));
    }
    g_list_free_full(wchl, g_free);
  }
  g_string_append(where, ";");
  gchar *ret = where->str;
  g_string_free(where, FALSE);
  return ret;
}

#define TABLE "datas"
#define JOURNAL "WAL"

gchar *mkt_sensor_data_dir() {
  gchar *dbdir =
      g_build_path("/", g_get_home_dir(), MKT_SQLITE_DB_DIR, "channels", NULL);
  return dbdir;
}

MktSqlite *mkt_sensor_data_conn(guint64 channel) {
  gchar *fname = g_strdup_printf(
      "channel%" G_GUINT64_FORMAT "-sensor-data.sqlite", channel);
  gchar *dbdir = mkt_sensor_data_dir();
  gchar *dbpath = g_build_filename(dbdir, fname, NULL);
  g_free(fname);
  g_free(dbdir);
  GObject *model = g_object_new(MKT_TYPE_SENSOR_DATA, NULL);
  MktSqlite *conn = mkt_sqlite_new_full(model, dbpath, TABLE, JOURNAL);
  g_free(dbpath);
  g_object_unref(model);
  return conn;
}

MktSqlite *mkt_sensor_data_conn_history(guint64 channel) {
  gchar *fname = g_strdup_printf(
      "channel%" G_GUINT64_FORMAT "-sensor-data-history.sqlite", channel);
  gchar *dbdir = mkt_sensor_data_dir();
  gchar *dbpath = g_build_filename(dbdir, fname, NULL);
  g_free(fname);
  g_free(dbdir);
  GObject *model = g_object_new(MKT_TYPE_SENSOR_DATA, NULL);
  MktSqlite *conn = mkt_sqlite_new_full(model, dbpath, TABLE, JOURNAL);
  g_free(dbpath);
  g_object_unref(model);
  return conn;
}

void mkt_sensor_data_clean(MktSqlite *conn) {
  if (!mkt_sqlite_open(conn)) {
    mkt_sqlite_delete_database(conn);
    if (!mkt_sqlite_open(conn)) {
      return;
    }
  }
  mkt_sqlite_delete_table(conn);
  mkt_sqlite_create_table(conn);
}

MktSensorData *mkt_sensor_data_new(gdouble value, guint trigger, guint state) {
  return MKT_SENSOR_DATA(g_object_new(MKT_TYPE_SENSOR_DATA, "value", value,
                                      "trigger", trigger, "state", state,
                                      NULL));
}

void mkt_sensot_data_insert(MktSqlite *conn, gdouble value, guint trigger,
                            guint state) {
  MktSensorData *data = mkt_sensor_data_new(value, trigger, state);
  mkt_sqlite_insert_object(conn, G_OBJECT(data));
}

static void sqliteAppendThread(GTask *task, gpointer source_object,
                               gpointer task_data, GCancellable *cancellable) {
  MktSqlite *conn = MKT_SQLITE(source_object);
  GSList *objs = (GSList *)g_task_get_task_data(task);
  if (objs == NULL || !mkt_sqlite_open(conn)) {
    g_task_return_boolean(task, FALSE);
    return;
  }
  mkt_sqlite_create_table(conn);
  guint64 last_id = mkt_sqlite_insert_objects(conn, objs);
  mkt_sqlite_close(conn);
  g_task_return_boolean(task, last_id != 0);
  return;
}
void unref_objects_and_free(gpointer data) {
  if (data != NULL)
    g_slist_free_full((GSList *)data, g_object_unref);
}

gpointer copy_data(gpointer src, gpointer data) { return g_object_ref(src); }

gboolean mkt_sensor_data_append_sync(guint64 channel, GSList *datas) {

  MktSqlite *conn = mkt_sensor_data_conn(channel);
  guint64 last_id = mkt_sqlite_insert_objects(conn, datas);
  mkt_sqlite_close(conn);
  return last_id!=0;
}

void mkt_sensor_data_append(guint64 channel, GSList *datas,
                            GCancellable *cancel, GAsyncReadyCallback callback,
                            gpointer user_data) {
  g_return_if_fail(datas != NULL);
  GSList *to_transmit = NULL;
  GSList *l = NULL;
  for (l = datas; l != NULL; l = l->next) {
    if (l->data && MKT_IS_SENSOR_DATA(l->data)) {
      to_transmit = g_slist_append(to_transmit, g_object_ref(l->data));
    } else {
      g_warning("wrong sensor data");
    }
  }
  MktSqlite *conn = mkt_sensor_data_conn(channel);
  GTask *task = g_task_new(conn, cancel, callback, user_data);
  g_task_set_task_data(task, to_transmit, unref_objects_and_free);
  g_task_run_in_thread(task, sqliteAppendThread);
  g_object_unref(conn);
  g_object_unref(task);
}

gdouble mkt_sensor_data_changed(MktSensorData *data) {
  g_return_val_if_fail(data != NULL, 0.0);
  g_return_val_if_fail(MKT_IS_SENSOR_DATA(data), 0.0);
  return data->priv->changed;
}

gdouble mkt_sensor_data_value(MktSensorData *data) {
  g_return_val_if_fail(data != NULL, 0.0);
  g_return_val_if_fail(MKT_IS_SENSOR_DATA(data), 0.0);
  return data->priv->value;
}

guint mkt_sensor_data_state(MktSensorData *data) {
  g_return_val_if_fail(data != NULL, 0);
  g_return_val_if_fail(MKT_IS_SENSOR_DATA(data), 0);
  return data->priv->state;
}

guint mkt_sensor_data_trigger(MktSensorData *data) {
  g_return_val_if_fail(data != NULL, 0);
  g_return_val_if_fail(MKT_IS_SENSOR_DATA(data), 0);
  return data->priv->trigger;
}

void mkt_sensor_data_append_history(guint64 channel, GSList *datas,
                                    GCancellable *cancel,
                                    GAsyncReadyCallback callback,
                                    gpointer user_data) {
  g_return_if_fail(datas != NULL);
  GSList *to_transmit = NULL;
  GSList *l = NULL;
  for (l = datas; l != NULL; l = l->next) {
    if (l->data != NULL && MKT_IS_SENSOR_DATA(l->data)) {
      to_transmit = g_slist_append(to_transmit, g_object_ref(l->data));
    } else {
      g_warning("wrong sensor data");
    }
  }
  MktSqlite *conn = mkt_sensor_data_conn_history(channel);
  GTask *task = g_task_new(conn, cancel, callback, user_data);
  g_task_set_task_data(task, to_transmit, unref_objects_and_free);
  g_task_run_in_thread(task, sqliteAppendThread);
  g_object_unref(conn);
  g_object_unref(task);
}

static void sqliteSelectQueryThread(GTask *task, gpointer source_object,
                                    gpointer task_data,
                                    GCancellable *cancellable) {
  MktSqlite *conn = MKT_SQLITE(source_object);
  if (g_task_return_error_if_cancelled(task)) {
    return;
  }
  if (!mkt_sqlite_open(conn)) {
    g_task_return_error(
        task, g_error_new(mkt_sensor_data_quark(), 100, "Open database error"));
    return;
  }
  Query *q = g_task_get_task_data(task);
  GSList *datas = NULL;
  if (q != NULL && q->trigger > 0) {
    datas = mkt_sqlite_select_objects(
        conn, "where trigger =  %u ORDER BY changed ASC", q->trigger);
  } else if (q != NULL && q->offset > 0) {
    datas = mkt_sqlite_select_objects(
        conn, "ORDER BY changed ASC LIMIT %u OFFSET %u;",
        q->limit > 0 ? q->limit : 4000, q->offset);
  } else {
    datas =
        mkt_sqlite_select_objects(conn, "ORDER BY changed ASC LIMIT 10000;");
  }

  mkt_sqlite_close(conn);
  if (g_task_return_error_if_cancelled(task)) {
    g_slist_free_full(datas, g_object_unref);
    return;
  }
  g_task_return_pointer(task, datas, unref_objects_and_free);
  return;
}

void mkt_sensor_data_select(guint64 channel, GCancellable *cancel,
                            GAsyncReadyCallback callback, gpointer user_data) {
  MktSqlite *conn = mkt_sensor_data_conn(channel);
  GTask *task = g_task_new(conn, cancel, callback, user_data);
  Query *q = new_query();
  g_task_set_task_data(task, q, free_query);
  g_task_run_in_thread(task, sqliteSelectQueryThread);
  g_object_unref(task);
  g_object_unref(conn);
}

void mkt_sensor_data_select_offset(guint64 channel, guint offset,
                                   GCancellable *cancel,
                                   GAsyncReadyCallback callback,
                                   gpointer user_data) {
  MktSqlite *conn = mkt_sensor_data_conn(channel);
  GTask *task = g_task_new(conn, cancel, callback, user_data);
  Query *q = new_query();
  q->offset = offset;
  q->limit = 4000;
  g_task_set_task_data(task, q, free_query);
  g_task_run_in_thread(task, sqliteSelectQueryThread);
  g_object_unref(task);
  g_object_unref(conn);
}

void mkt_sensor_data_select_trigger(guint64 channel, guint trigger,
                                    GCancellable *cancel,
                                    GAsyncReadyCallback callback,
                                    gpointer user_data) {
  MktSqlite *conn = mkt_sensor_data_conn_history(channel);
  GTask *task = g_task_new(conn, cancel, callback, user_data);
  Query *q = new_query();
  q->trigger = trigger;
  g_task_set_task_data(task, q, free_query);
  g_task_run_in_thread(task, sqliteSelectQueryThread);
  g_object_unref(task);
  g_object_unref(conn);
}

static void sqliteCleanThread(GTask *task, gpointer source_object,
                              gpointer task_data, GCancellable *cancellable) {
  MktSqlite *conn = MKT_SQLITE(source_object);
  if (g_task_return_error_if_cancelled(task)) {
    return;
  }
  if (!mkt_sqlite_open(conn)) {
    mkt_sqlite_delete_database(conn);
    if (!mkt_sqlite_open(conn)) {
      g_task_return_error(task, g_error_new(mkt_sensor_data_quark(), 100,
                                            "Open database error"));
      return;
    }
    g_task_return_boolean(task, FALSE);
  }
  mkt_sqlite_create_table(conn);
  Query *q = g_task_get_task_data(task);
  GSList *datas =
      mkt_sqlite_select_objects(conn, "ORDER BY changed DESC LIMIT 1");
  guint trigger = 0;
  if (datas) {
    if (MKT_IS_SENSOR_DATA(datas->data))
      trigger = mkt_sensor_data_trigger(MKT_SENSOR_DATA(datas->data));
    g_slist_free_full(datas, g_object_unref);
  }
  if (trigger > q->records) {
    mkt_sqlite_exec(conn, "delete from %s where trigger < %u;", TABLE,
                    trigger - q->records);
  }
  mkt_sqlite_close(conn);
  if (g_task_return_error_if_cancelled(task)) {
    return;
  }
  g_task_return_boolean(task, TRUE);
}

void mkt_sensor_data_clean_history(guint64 channel, guint records,
                                   GCancellable *cancel,
                                   GAsyncReadyCallback callback,
                                   gpointer user_data) {
  MktSqlite *conn = mkt_sensor_data_conn_history(channel);
  GTask *task = g_task_new(conn, cancel, callback, user_data);
  Query *q = new_query();
  q->records = records;
  g_task_set_task_data(task, q, free_query);
  g_task_run_in_thread(task, sqliteCleanThread);
  g_object_unref(task);
  g_object_unref(conn);
}

void mkt_sensor_data_check() {
  gchar *sdir = mkt_sensor_data_dir();
  if (!g_file_test(sdir, G_FILE_TEST_IS_DIR)) {
    g_mkdir_with_parents(sdir, 0777);
    return;
  } else {
    g_chmod(sdir, 0777);
  }
  GDir *dir = NULL;
  GError *error = NULL;
  dir = g_dir_open(sdir, 0, &error);
  if (dir == NULL || error != NULL) {
    g_warning("check sensor database failed - %s", error->message);
    return;
  }
  const gchar *name = NULL;
  while ((name = g_dir_read_name(dir))) {
    if (g_str_has_suffix(name, ".sqlite")) {
      gchar *dbpath = g_build_path("/", sdir, name, NULL);
      if (g_file_test(dbpath, G_FILE_TEST_IS_DIR)) {
        GObject *model = g_object_new(MKT_TYPE_SENSOR_DATA, NULL);
        MktSqlite *conn = mkt_sqlite_new_full(model, dbpath, TABLE, JOURNAL);
        mkt_sqlite_check(conn);
      }
      g_free(dbpath);
    }
  }
  g_dir_close(dir);
  g_free(sdir);
}
void mkt_sensor_data_all_clean() {
  gchar *sdir = mkt_sensor_data_dir();
  if (!g_file_test(sdir, G_FILE_TEST_IS_DIR)) {
    g_mkdir_with_parents(sdir, 0777);
    return;
  } else {
    g_chmod(sdir, 0777);
  }
  GDir *dir = NULL;
  GError *error = NULL;
  dir = g_dir_open(sdir, 0, &error);
  if (dir == NULL || error != NULL) {
    g_warning("check sensor database failed - %s", error->message);
    return;
  }
  const gchar *name = NULL;
  while ((name = g_dir_read_name(dir))) {
    if (g_str_has_suffix(name, ".sqlite")) {
      gchar *dbpath = g_build_path("/", sdir, name, NULL);
      if (g_file_test(dbpath, G_FILE_TEST_IS_REGULAR)) {
        GObject *model = g_object_new(MKT_TYPE_SENSOR_DATA, NULL);
        MktSqlite *conn = mkt_sqlite_new_full(model, dbpath, TABLE, JOURNAL);
        mkt_sqlite_delete_table(conn);
        mkt_sqlite_check(conn);
      }
      g_free(dbpath);
    }
  }
  g_dir_close(dir);
  g_free(sdir);
}
#undef TABLE
#undef JOURNAL
