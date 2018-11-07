/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mkt-sql-connection.c
 * Copyright (C) sascha 2012 <sascha@sascha-desktop>
 *
mkt-sql-connection.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mkt-sql-connection.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MKT_SQLITE_CONNECTION_H_
#define _MKT_SQLITE_CONNECTION_H_

#include <gio/gio.h>
#include <glib-object.h>
#include <sqlite3.h>
#include "mkt-connection.h"
#include "mkt-decl.h"

G_BEGIN_DECLS

#define MKT_TYPE_SQLITE_CONNECTION (mkt_sqlite_connection_get_type())
#define MKT_SQLITE_CONNECTION(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), MKT_TYPE_SQLITE_CONNECTION, MktSQLiteConnection))
#define MKT_SQLITE_CONNECTION_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), MKT_TYPE_SQLITE_CONNECTION, MktSQLiteConnectionClass))
#define MKT_IS_SQLITE_CONNECTION(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), MKT_TYPE_SQLITE_CONNECTION))
#define MKT_IS_SQLITE_CONNECTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), MKT_TYPE_SQLITE_CONNECTION))
#define MKT_SQLITE_CONNECTION_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), MKT_TYPE_SQLITE_CONNECTION, MktSQLiteConnectionClass))

typedef struct _MktSQLiteConnectionClass   MktSQLiteConnectionClass;
typedef struct _MktSQLiteConnection        MktSQLiteConnection;
typedef struct _MktSQLiteConnectionPrivate MktSQLiteConnectionPrivate;

struct _MktSQLiteConnectionClass {
    MktConnectionClass parent_class;
};

struct _MktSQLiteConnection {
    MktConnection               parent_instance;
    MktSQLiteConnectionPrivate *priv;
};

GType mkt_sqlite_connection_get_type(void);

MktConnection *mkt_sqlite_connection_new(const gchar *id, const gchar *host, guint port, guint flag, const gchar *user, const gchar *password, const gchar *database);

// void mkt_sqlite_connection_commit(MktSQLiteConnection *connection, GCancellable *cancellable, GAsyncReadyCallback callback, gpointer user_data);
void mkt_sqlite_connection_update_table(MktSQLiteConnection *connection, GType object_type);
void mkt_sqlite_connection_check(MktSQLiteConnection *conn);
void mkt_sqlite_connection_close_after_use(MktSQLiteConnection *conn);
void mkt_sqlite_connection_exec(MktConnection *connection, const gchar *stmt );

G_END_DECLS

#endif /* _MKT_SQLITE_CONNECTION_H_ */
