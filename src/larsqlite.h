/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * file larsqlite.h
 * Copyright (C) LAR 2017
 *

 */

#ifndef _LAR_SQLITE_H_
#define _LAR_SQLITE_H_

#include <glib-object.h>
#include <sqlite3.h>
#include <glib.h>

#include <gio/gio.h>


G_BEGIN_DECLS

#define TYPE_LAR_SQLITE (sqlite_connection_get_type())
#define LAR_SQLITE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_LAR_SQLITE, LarSqlite))
#define LAR_SQLITE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_LAR_SQLITE, LarSqliteClass))
#define IS_LAR_SQLITE(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_LAR_SQLITE))
#define IS_LAR_SQLITE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_LAR_SQLITE))
#define LAR_SQLITE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_LAR_SQLITE, LarSqliteClass))

typedef struct _LarSqliteClass   LarSqliteClass;
typedef struct _LarSqlite        LarSqlite;
typedef struct _LarSqlitePrivate LarSqlitePrivate;

enum {
    SQLITE_DB_PRIMERY_KEY = 1 << 13,
    SQLITE_DB_TIMESTAMP   = 1 << 14,
    SQLITE_DB_NOT_NULL    = 1 << 14,
};
#define SQLITE_PK_PROPERTY "pkey"
struct _LarSqliteClass {
    GObjectClass parent_class;
};

struct _LarSqlite {
    GObject                  parent_instance;
    LarSqlitePrivate *priv;
};

GType sqlite_connection_get_type(void);

LarSqlite *LarSqliteNew(const gchar *database, guint flag);
gboolean m_LarSqliteCreateTable(LarSqlite *connection, const gchar *table, GObject *object);
guint64 m_LarSqliteInsertObject(LarSqlite *connection, const gchar *table, GObject *object);
guint64 m_LarSqliteInsertObjects(LarSqlite *connection, const gchar *table, GSList *objects);
GSList *m_LarSqliteSelectObjects(LarSqlite *connection,GType type, const gchar *format, ...) G_GNUC_PRINTF(3, 4);
GSList *m_LarSqliteSelectObjectsAsyncFinish(GAsyncResult *result, GError **error);
gboolean m_LarSqliteSelectObjectsAsync(LarSqlite *connection, GType type, GCancellable *cancellable, GAsyncReadyCallback callback, gpointer user_data, const gchar *format, ...)G_GNUC_PRINTF(6, 7);
void  m_LarSqliteJournalWal(LarSqlite *connection);
gboolean m_LarSqliteSelectCallback(LarSqlite *connection, int (*callback)(void *, int, char **, char **),gpointer data, const gchar *format, ...);
gboolean m_LarSqliteUpdateObjectProperty(LarSqlite *connection, const gchar *table,GObject *object, const gchar *pname);


//
gchar *f_LarTimeNowSqliteFormatUTC();
gchar *f_LarTimeNowSqliteFormat();
gchar *f_LarTimeSqliteFormat(GDateTime *dt);    

G_END_DECLS

#endif /* _LAR_SQLITE_H_ */
