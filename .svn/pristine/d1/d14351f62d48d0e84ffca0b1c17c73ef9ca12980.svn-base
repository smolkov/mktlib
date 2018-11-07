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

#include "mkt-connection.h"
#include <glib.h>
#include <glib/gprintf.h>
#include <glib/gthread.h>
#include <string.h>
#include <gio/gio.h>
#include "mkt-utils.h"
#include "mkt-SQLite-connection.h"

#define CONNECTION_MAX_TYPES 100

struct _MktConnectionPrivate {
    gchar *host;
    gchar *db_name;
    gchar *user;
    gchar *password;
    guint  port;
    guint  flag;
};

#define MKT_CONNECTION_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), MKT_TYPE_CONNECTION, MktConnectionPrivate))

G_DEFINE_TYPE(MktConnection, mkt_connection, G_TYPE_OBJECT);

enum {
    PROP_MKT_CONNECTION_O,
    PROP_MKT_CONNECTION_HOST,
    PROP_MKT_CONNECTION_USER,
    PROP_MKT_CONNECTION_PASSWORD,
    PROP_MKT_CONNECTION_PORT,
    PROP_MKT_CONNECTION_FLAG,
    PROP_MKT_CONNECTION_DB_NAME,
    PROP_MKT_CONNECTION_DB_SYNCHRONIZED
};

void mkt_connection_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    MktConnection *connection = MKT_CONNECTION(object);
    switch (prop_id) {
        case PROP_MKT_CONNECTION_HOST:
            if (connection->priv->host) g_free(connection->priv->host);
            connection->priv->host = g_value_dup_string(value);
            break;
        case PROP_MKT_CONNECTION_USER:
            if (connection->priv->user) g_free(connection->priv->user);
            connection->priv->user = g_value_dup_string(value);
            break;
        case PROP_MKT_CONNECTION_PASSWORD:
            if (connection->priv->password) g_free(connection->priv->password);
            connection->priv->password = g_value_dup_string(value);
            break;
        case PROP_MKT_CONNECTION_PORT:
            if (g_value_get_uint(value) > 0) connection->priv->port = g_value_get_uint(value);
            break;
        case PROP_MKT_CONNECTION_FLAG:
            connection->priv->flag = g_value_get_uint(value);
            break;
        case PROP_MKT_CONNECTION_DB_NAME:
            if (connection->priv->db_name) g_free(connection->priv->db_name);
            connection->priv->db_name = g_value_dup_string(value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}

void mkt_connection_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    MktConnection *connection = MKT_CONNECTION(object);
    switch (prop_id) {
        case PROP_MKT_CONNECTION_HOST:
            g_value_set_string(value, connection->priv->host);
            break;
        case PROP_MKT_CONNECTION_USER:
            g_value_set_string(value, connection->priv->user);
            break;
        case PROP_MKT_CONNECTION_PASSWORD:
            g_value_set_string(value, connection->priv->password);
            break;
        case PROP_MKT_CONNECTION_PORT:
            g_value_set_uint(value, connection->priv->port);
            break;
        case PROP_MKT_CONNECTION_FLAG:
            g_value_set_uint(value, connection->priv->flag);
            break;
        case PROP_MKT_CONNECTION_DB_NAME:
            g_value_set_string(value, connection->priv->db_name);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}

static void mkt_connection_init(MktConnection *object) {
    /* TODO: Add initialization code here */
    object->priv           = G_TYPE_INSTANCE_GET_PRIVATE(object, MKT_TYPE_CONNECTION, MktConnectionPrivate);
    object->priv->host     = g_strdup_printf("%s/.ims_db", "/usr/share");
    object->priv->user     = g_strdup("root");
    object->priv->password = g_strdup("root");
    object->priv->port     = 0;
    object->priv->flag     = 0;
    object->priv->db_name  = g_strdup("main");
}

static void mkt_connection_finalize(GObject *object) {
    /* TODO: Add deinitalization code here */
    MktConnection *connection = MKT_CONNECTION(object);
    if (connection->priv->host) g_free(connection->priv->host);
    if (connection->priv->user) g_free(connection->priv->user);
    if (connection->priv->password) g_free(connection->priv->password);
    if (connection->priv->db_name) g_free(connection->priv->db_name);
    G_OBJECT_CLASS(mkt_connection_parent_class)->finalize(object);
}

static void mkt_connection_class_init(MktConnectionClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // GObjectClass* parent_class = G_OBJECT_CLASS (klass);

    g_type_class_add_private(klass, sizeof(MktConnectionPrivate));

    GParamSpec *pspec;

    object_class->finalize     = mkt_connection_finalize;
    object_class->set_property = mkt_connection_set_property;
    object_class->get_property = mkt_connection_get_property;

    klass->open           = NULL;
    klass->close          = NULL;
    klass->ping           = NULL;
    klass->realize        = NULL;
    klass->send           = NULL;
    klass->send_no_select = NULL;
    klass->show_status    = NULL;

    klass->create_table = NULL;
    klass->value_type   = NULL;

    pspec = g_param_spec_string("host", "connection construct prop", "Set connection's host", NULL, G_PARAM_READWRITE);
    g_object_class_install_property(object_class, PROP_MKT_CONNECTION_HOST, pspec);
    pspec = g_param_spec_string("user", "connection construct prop", "Set connection's user", NULL, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
    g_object_class_install_property(object_class, PROP_MKT_CONNECTION_USER, pspec);
    pspec = g_param_spec_string("password", "connection construct prop", "Set connection's password", "root", G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
    g_object_class_install_property(object_class, PROP_MKT_CONNECTION_PASSWORD, pspec);
    pspec = g_param_spec_uint("port", "connection construct prop", "Set connection's port", 0, G_MAXUINT16, 3306, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
    g_object_class_install_property(object_class, PROP_MKT_CONNECTION_PORT, pspec);
    pspec = g_param_spec_uint("flag", "connection construct prop", "Set connection's port", 0, G_MAXUINT16, 0, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
    g_object_class_install_property(object_class, PROP_MKT_CONNECTION_FLAG, pspec);
    pspec = g_param_spec_string("database", "connection construct prop", "Set connection's database", "mysql", G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
    g_object_class_install_property(object_class, PROP_MKT_CONNECTION_DB_NAME, pspec);
}

const gchar *mkt_connection_get_data_base(MktConnection *connection) {
    g_return_val_if_fail(connection != NULL, NULL);
    g_return_val_if_fail(MKT_IS_CONNECTION(connection), NULL);
    return connection->priv->db_name;
}

guint64 mkt_connection_send_no_select(MktConnection *connection, const gchar *stmt) {
    g_return_val_if_fail(connection != NULL, 0);
    g_return_val_if_fail(stmt != NULL, 0);
    g_return_val_if_fail(MKT_IS_CONNECTION(connection), 0);
    if (MKT_CONNECTION_GET_CLASS(connection)->send_no_select) return MKT_CONNECTION_GET_CLASS(connection)->send_no_select(connection, stmt);
    return 0;
}

gboolean mkt_connection_delete_table(MktConnection *connection, const gchar *table) {
    g_return_val_if_fail(connection != NULL, 0);
    g_return_val_if_fail(MKT_IS_CONNECTION(connection), 0);
    if (MKT_CONNECTION_GET_CLASS(connection)->delete_table) return MKT_CONNECTION_GET_CLASS(connection)->delete_table(connection, table);
    return FALSE;
}

GSList *mkt_connection_send(MktConnection *connection, GType otype, const gchar *stmt) {
    g_return_val_if_fail(connection != NULL, NULL);
    g_return_val_if_fail(stmt != NULL, NULL);
    g_return_val_if_fail(MKT_IS_CONNECTION(connection), NULL);
    if (MKT_CONNECTION_GET_CLASS(connection)->send) {
        return MKT_CONNECTION_GET_CLASS(connection)->send(connection, otype, stmt);
    }
    return NULL;
}

const gchar *mkt_connection_create_table_sql(MktConnection *connection) {
    g_return_val_if_fail(connection != NULL, NULL);
    g_return_val_if_fail(MKT_IS_CONNECTION(connection), NULL);
    if (MKT_CONNECTION_GET_CLASS(connection)->create_table) return MKT_CONNECTION_GET_CLASS(connection)->create_table(connection);
    return "NULL";
}

gchar *mkt_connection_convert_value_type_sql(MktConnection *connection, GType type, guint flag) {
    g_return_val_if_fail(connection != NULL, NULL);
    g_return_val_if_fail(MKT_IS_CONNECTION(connection), NULL);
    if (MKT_CONNECTION_GET_CLASS(connection)->value_type) return MKT_CONNECTION_GET_CLASS(connection)->value_type(connection, type, flag);
    return NULL;
}

const gchar *mkt_connection_get_provider(MktConnection *connection) {
    g_return_val_if_fail(connection != NULL, NULL);
    g_return_val_if_fail(MKT_IS_CONNECTION(connection), NULL);
    if (MKT_CONNECTION_GET_CLASS(connection)->provider) return MKT_CONNECTION_GET_CLASS(connection)->provider(connection);
    return NULL;
}

guint mkt_connection_get_flag(MktConnection *connection) {
    g_return_val_if_fail(connection != NULL, 0);
    g_return_val_if_fail(MKT_IS_CONNECTION(connection), 0);
    return connection->priv->flag;
}

const gchar *mkt_connection_get_host(MktConnection *connection) {
    g_return_val_if_fail(connection != NULL, NULL);
    g_return_val_if_fail(MKT_IS_CONNECTION(connection), NULL);
    return connection->priv->host;
}

/*static GType __connection_default_type = MKT_TYPE_SQL_CONNECTION;

void  set_connection_set_default_type  ( GType type )
{
        if(!g_type_is_a (type,MKT_TYPE_CONNECTION))
        {
                __connection_default_type  = type;
        }
}*/

#define DB_DIR "tera/db"

MktConnection *mkt_connection_default() {
    gchar *host_address = g_build_path("/", "/usr/share", DB_DIR, NULL);
    gchar *db_address   = g_build_path("/", "/usr/share", DB_DIR, "device.database", NULL);
    mkt_make_dir(host_address);
    MktConnection *conn = MKT_CONNECTION(g_object_new(MKT_TYPE_SQLITE_CONNECTION, "host", host_address, "database", db_address, "flag", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL));
    g_free(host_address);
    g_free(db_address);
    return conn;
}
MktConnection *mkt_connection_errors() {
    gchar *host_address = g_build_path("/", "/usr/share", DB_DIR, NULL);
    gchar *db_address   = g_build_path("/", "/usr/share", DB_DIR, "errors.sqlite", NULL);
    mkt_make_dir(host_address);
    MktConnection *conn = MKT_CONNECTION(g_object_new(MKT_TYPE_SQLITE_CONNECTION, "host", host_address, "database", db_address, "flag", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL));
    g_free(host_address);
    g_free(db_address);
    return conn;
}

MktConnection *mkt_connection_parameter() {
    MktConnection *connection   = NULL;
    gchar *        host_address = g_build_path("/", "/usr/share", DB_DIR, NULL);
    gchar *        db_address   = g_build_path("/", "/usr/share", DB_DIR, "device-parameter.database", NULL);
    mkt_make_dir(host_address);
    connection = MKT_CONNECTION(g_object_new(MKT_TYPE_SQLITE_CONNECTION, "host", host_address, "database", db_address, "flag", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL));
    mkt_sqlite_connection_close_after_use(MKT_SQLITE_CONNECTION(connection));
    g_free(host_address);
    g_free(db_address);
    return connection;
}

MktConnection *mkt_connection_special() {
    gchar *host_address = g_build_path("/", "/usr/share", DB_DIR, NULL);
    gchar *db_address   = g_build_path("/", "/usr/share", DB_DIR, "device-special-parameter.database", NULL);
    mkt_make_dir(host_address);
    MktConnection *connection = MKT_CONNECTION(g_object_new(MKT_TYPE_SQLITE_CONNECTION, "host", host_address, "database", db_address, "flag", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL));
    g_free(host_address);
    g_free(db_address);
    return connection;
}

MktConnection *mkt_connection_measurement() {
    gchar *host_address = g_build_path("/", "/usr/share", DB_DIR, NULL);
    gchar *db_address   = g_build_path("/", "/usr/share", DB_DIR, "device-measurement.database", NULL);
    mkt_make_dir(host_address);
    MktConnection *connection = MKT_CONNECTION(g_object_new(MKT_TYPE_SQLITE_CONNECTION, "host", host_address, "database", db_address, "flag", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL));
    g_free(host_address);
    g_free(db_address);
    return connection;
}

MktConnection *mkt_connection_calibration() {
    gchar *host_address = g_build_path("/", "/usr/share", DB_DIR, NULL);
    gchar *db_address   = g_build_path("/", "/usr/share", DB_DIR, "device-calibration.database", NULL);
    mkt_make_dir(host_address);
    MktConnection *connection = MKT_CONNECTION(g_object_new(MKT_TYPE_SQLITE_CONNECTION, "host", host_address, "database", db_address, "flag", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL));
    g_free(host_address);
    g_free(db_address);
    return connection;
}
MktConnection *mkt_connection_process() {
    gchar *host_address = g_build_path("/", "/usr/share", DB_DIR, NULL);
    gchar *db_address   = g_build_path("/", "/usr/share", DB_DIR, "device-process.database", NULL);
    mkt_make_dir(host_address);
    MktConnection *connection = MKT_CONNECTION(g_object_new(MKT_TYPE_SQLITE_CONNECTION, "host", host_address, "database", db_address, "flag", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL));
    g_free(host_address);
    g_free(db_address);
    return connection;
}

MktConnection *mkt_connection_get_default(gboolean open) {
    static MktConnection *connection = NULL;
    if (connection != NULL) return connection;
    connection = mkt_connection_default();
    return connection;
}

MktConnection *mkt_connection_get_errors(gboolean open) {
    static MktConnection *err_connection = NULL;
    if (err_connection != NULL) return err_connection;
    err_connection = mkt_connection_errors();
    return err_connection;
}

MktConnection *mkt_connection_new_default_parameter(gboolean open) {
    static MktConnection *default_parameter = NULL;
    if (default_parameter != NULL) return default_parameter;
    default_parameter = mkt_connection_parameter();
    return default_parameter;
}

MktConnection *mkt_connection_new_special_parameter(gboolean open) {
    static MktConnection *connection = NULL;
    if (connection != NULL) return connection;
    connection = mkt_connection_special();
    return connection;
}

MktConnection *mkt_connection_new_default_measurement(gboolean open) {
    static MktConnection *connection = NULL;
    if (connection != NULL) return connection;
    connection = mkt_connection_measurement();
    return connection;
}

MktConnection *mkt_connection_new_default_calibration(gboolean open) {
    static MktConnection *connection = NULL;
    if (connection != NULL) return connection;
    connection = mkt_connection_calibration();
    return connection;
}

MktConnection *mkt_connection_new_default_process(gboolean open) {
    static MktConnection *connection = NULL;
    if (connection != NULL) return connection;
    connection = mkt_connection_process();
    return connection;
}

MktConnection *mkt_connection_new(const gchar *host, const gchar *database, gboolean open) {
    MktConnection *connection = NULL;
    connection                = MKT_CONNECTION(g_object_new(MKT_TYPE_SQLITE_CONNECTION, "host", host, "database", database, "flag", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL));
    // mkt_sqlite_connection_journal_wal(MKT_SQLITE_CONNECTION(connection));
    return connection;
}
