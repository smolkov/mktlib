/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * file MktSqlite.h
 * Copyright (C) LAR 2017
 *

 */

#ifndef _MKT_SQLITE_H_
#define _MKT_SQLITE_H_

#include <glib-object.h>
#include <glib.h>
#include <sqlite3.h>

#include <gio/gio.h>

G_BEGIN_DECLS

#define MKT_TYPE_SQLITE (mkt_sqlite_conn_get_type())
#define MKT_SQLITE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), MKT_TYPE_SQLITE, MktSqlite))
#define MKT_SQLITE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), MKT_TYPE_SQLITE, MktSqliteClass))
#define MKT_IS_SQLITE(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), MKT_TYPE_SQLITE))
#define MKT_IS_SQLITE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), MKT_TYPE_SQLITE))
#define MKT_SQLITE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), MKT_TYPE_SQLITE, MktSqliteClass))

typedef struct _MktSqliteClass   MktSqliteClass;
typedef struct _MktSqlite        MktSqlite;
typedef struct _MktSqlitePrivate MktSqlitePrivate;

#define MKT_SQLITE_DB_DIR "db"
enum {
    MKT_SQLITE_PRIMERY_KEY = 1 << 13,
    MKT_SQLITE_TIMESTAMP   = 1 << 14,
    MKT_SQLITE_NOT_NULL    = 1 << 14,
};
struct _MktSqliteClass {
    GObjectClass parent_class;
};

struct _MktSqlite {
    GObject           parent_instance;
    MktSqlitePrivate *priv;
};

GType mkt_sqlite_conn_get_type(void);

MktSqlite *  mkt_sqlite_new(GObject *template, const gchar *database);
MktSqlite *  mkt_sqlite_new_full(GObject *template, const gchar *database, const gchar *tabel, const gchar *journal_mod);
gboolean     mkt_sqlite_open(MktSqlite *conn);
gboolean     mkt_sqlite_open_readonly(MktSqlite *conn);
void         mkt_sqlite_close(MktSqlite *conn);
gboolean     mkt_sqlite_create_table(MktSqlite *conn);
gboolean     mkt_sqlite_delete_table(MktSqlite *conn);
gboolean     mkt_sqlite_exec(MktSqlite *conn, const gchar *sql_format, ...) G_GNUC_PRINTF(2, 3);
guint64      mkt_sqlite_insert_object(MktSqlite *conn, GObject *object);
guint64      mkt_sqlite_insert_objects(MktSqlite *conn, GSList *objects);
GSList *     mkt_sqlite_select_objects(MktSqlite *conn, const gchar *match, ...) G_GNUC_PRINTF(2, 3);
GSList *     mkt_sqlite_select_objects_finish(GAsyncResult *result, GError **error);
gboolean     mkt_sqlite_select_objects_async(MktSqlite *conn, GCancellable *cancellable, GAsyncReadyCallback callback, gpointer user_data, const gchar *match, ...) G_GNUC_PRINTF(5, 6);
gboolean     mkt_sqlite_select_callback(MktSqlite *conn, int (*callback)(void *, int, char **, char **), gpointer data, const gchar *format, ...);
gboolean     mkt_sqlite_update_object_prop(MktSqlite *conn, GObject *object, const gchar *pname);
MktSqlite *  mkt_sqlite_clone(MktSqlite *conn);
const gchar *mkt_sqlite_get_table(MktSqlite *conn);
void         mkt_sqlite_delete_database(MktSqlite *conn);
gboolean     mkt_sqlite_check(MktSqlite *conn);
guint        mkt_sqlite3_exec(sqlite3 *db, const gchar *sql, sqlite3_callback cb, void *v, char **c);
guint        mkt_sqlite3_open_v2(const char *path, sqlite3 **db, int flag, const char *vfs);
guint        mkt_sqlite3_setup_wal(sqlite3 *db, int ms, void *data);
//
gchar *mkt_sqlite_now_utc();
gchar *mkt_sqlite_now_local();
gchar *mkt_sqlite_from_dt(GDateTime *dt);
GQuark mkt_sqlite_error_quark();
G_END_DECLS

#endif /* _MKT_SQLITE_H_ */
