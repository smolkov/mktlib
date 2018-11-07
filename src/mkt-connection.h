/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mkt-connection.c
 * Copyright (C) sascha 2012 <sascha@sascha-desktop>
 *
mkt-connection.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mkt-connection.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MKT_CONNECTION_H_
#define _MKT_CONNECTION_H_

#include <glib.h>
#include "mkt-data-model.h"

G_BEGIN_DECLS

#define MKT_TYPE_CONNECTION (mkt_connection_get_type())
#define MKT_CONNECTION(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), MKT_TYPE_CONNECTION, MktConnection))
#define MKT_CONNECTION_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), MKT_TYPE_CONNECTION, MktConnectionClass))
#define MKT_IS_CONNECTION(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), MKT_TYPE_CONNECTION))
#define MKT_IS_CONNECTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), MKT_TYPE_CONNECTION))
#define MKT_CONNECTION_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), MKT_TYPE_CONNECTION, MktConnectionClass))

typedef struct _MktConnection        MktConnection;
typedef struct _MktConnectionClass   MktConnectionClass;
typedef struct _MktConnectionPrivate MktConnectionPrivate;

typedef enum {
    MKT_CONNECTION_PROVIDER_TYPE_MYSQL,
    MKT_CONNECTION_PROVIDER_TYPE_SQLite,
} MktConnectionProviderType;

struct _MktConnectionClass {
    GObjectClass parent_class;
    gboolean (*open)(MktConnection *connection);
    gboolean (*close)(MktConnection *connection);
    gboolean (*ping)(MktConnection *connection);
    gboolean (*realize)(MktConnection *connection);
    GSList *(*send)(MktConnection *connection, GType otype, const gchar *stmt);
    guint64 (*send_no_select)(MktConnection *connection, const gchar *stmt);
    gboolean (*check_table)(MktConnection *connection, const gchar *table, const gchar *check);
    gboolean (*delete_table)(MktConnection *connection, const gchar *table);

    MktDataModel *(*show_status)(MktConnection *connection);
    const gchar *(*create_table)(MktConnection *connection);
    gchar *(*value_type)(MktConnection *connection, GType type, guint flag);
    const gchar *(*provider)(MktConnection *connection);
    // Connection functions
    const gchar *(*timestamp_now)(MktConnection *connection);
};

struct _MktConnection {
    GObject               parent_instance;
    MktConnectionPrivate *priv;
};

GType          mkt_connection_get_type(void) G_GNUC_CONST;
void           mkt_connection_set_default_type(GType type);
MktConnection *mkt_connection_get_default(gboolean open);
MktConnection *mkt_connection_get_errors(gboolean open);
MktConnection *mkt_connection_new_default_parameter(gboolean open);
MktConnection *mkt_connection_new_special_parameter(gboolean open);
MktConnection *mkt_connection_new_default_measurement(gboolean open);
MktConnection *mkt_connection_new_default_calibration(gboolean open);
MktConnection *mkt_connection_new_default_process(gboolean open);
MktConnection *mkt_connection_new(const gchar *host, const gchar *database, gboolean open);
GSList *       mkt_connection_send(MktConnection *connection, GType type, const gchar *stmt);
guint64        mkt_connection_send_no_select(MktConnection *connection, const gchar *stmt);
gboolean       mkt_connection_delete_table(MktConnection *connection, const gchar *table);
const gchar *  mkt_connection_get_data_base(MktConnection *connection);
const gchar *  mkt_connection_create_table_sql(MktConnection *connection);
gchar *        mkt_connection_convert_value_type_sql(MktConnection *connection, GType type, guint flag);
const gchar *  mkt_connection_get_provider(MktConnection *connection);
guint          mkt_connection_get_flag(MktConnection *connection);
const gchar *  mkt_connection_get_host(MktConnection *connection);
MktConnection *mkt_connection_default();
MktConnection *mkt_connection_errors();
MktConnection *mkt_connection_parameter();
MktConnection *mkt_connection_special();
MktConnection *mkt_connection_measurement();
MktConnection *mkt_connection_calibration();
MktConnection *mkt_connection_process();
G_END_DECLS

#endif /* _MKT_CONNECTION_H_ */
