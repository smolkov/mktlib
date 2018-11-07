/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * larsqlite.c
 * Copyright (C) LAR 2017
 *

 */

#include "larsqlite.h"
#include <fcntl.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>
#include <locale.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include "value.h"

#define MAX_BUSY_RETRY 10000

enum { PROP_0, SQLITE_FLAG, SQLITE_DATABASE };

const gchar *sqlParserRps(const gchar *pname) {
    static gchar ret[1024] = "";
    memset(ret, 0, sizeof(ret));
    strncpy(ret, pname, sizeof(ret));
    gchar *p = (gchar *)ret;
    while (*p != '\0') {
        if (*p == '-') *p = '_';
        p++;
    }
    return ret;
}

gchar *sqlParserDupRps(const gchar *pname) {
    gchar *ret = g_strdup(pname);
    gchar *p   = (gchar *)ret;
    while (*p != '\0') {
        if (*p == '-') *p = '_';
        p++;
    }
    return ret;
}

gchar *m_Larf_LarTimeNowSqliteFormatUTC() {
    GDateTime *dt  = g_date_time_new_now_utc();
    gchar *    res = g_date_time_format(dt, "%Y.%m.%dT%H:%M:%S");
    g_date_time_unref(dt);
    return res;
}
gchar *f_LarTimeNowSqliteFormat() {
    GTimeZone *tz  = g_time_zone_new_local();
    GDateTime *dt  = g_date_time_new_now(tz);
    gchar *    res = g_date_time_format(dt, "%Y.%m.%dT%H:%M:%S");
    g_date_time_unref(dt);
    g_time_zone_unref(tz);
    return res;
}
gchar *f_LarTimeSqliteFormat(GDateTime *dt) {
    gchar *res = g_date_time_format(dt, "%Y.%m.%dT%H:%M:%S");
    return res;
}

GType SqliteConvertToGType(guint SqliteType) {
    // TEST:g_debug("MySQL type %d",MySQLType);
    switch (SqliteType) {
        case 1:
            return G_TYPE_INT;
        case 2:
            return G_TYPE_STRING;  // MySQL name TINYINT
        case 3:
            return G_TYPE_STRING;  // MySQL name INT
        case 4:
            return G_TYPE_DOUBLE;
        case 5:
            return G_TYPE_STRING;  // MySQL name BIGINT
        default:
            return G_TYPE_NONE;
    }
};

gchar *SqliteConvertGType(GType type, guint flag) {
    GString *type_string = g_string_new("");
    if (G_TYPE_IS_BOXED(type)) {
        type_string = g_string_append(type_string, "TEXT");
    } else if (G_TYPE_IS_OBJECT(type)) {
        type_string = g_string_append(type_string, "TEXT");
    } else if (!(flag & SQLITE_DB_PRIMERY_KEY)) {
        switch (type) {
            case G_TYPE_BOOLEAN:
                type_string = g_string_append(type_string, "TINYINT");
                break;
            case G_TYPE_CHAR:
                type_string = g_string_append(type_string, "SMALLINT");
                break;
            case G_TYPE_UCHAR:
                type_string = g_string_append(type_string, "SMALLINT UNSIGNED");
                break;
            case G_TYPE_INT:
                type_string = g_string_append(type_string, "INTEGER");
                break;
            case G_TYPE_UINT:
                type_string = g_string_append(type_string, "INT UNSIGNED");
                break;
            case G_TYPE_INT64:
                type_string = g_string_append(type_string, "INTEGER");
                break;
            case G_TYPE_LONG:
                type_string = g_string_append(type_string, "INTEGER");
                break;
            case G_TYPE_ULONG:
                type_string = g_string_append(type_string, "INTEGER");
                break;
            case G_TYPE_UINT64:
                type_string = g_string_append(type_string, "INTEGER");
                break;
            case G_TYPE_DOUBLE:
                type_string = g_string_append(type_string, "DOUBLE");
                break;
            // case G_TYPE_ARRAY:
            // type_string = g_string_append(type_string, "TEXT");
            // break;
            case G_TYPE_STRING:
                if (flag & SQLITE_DB_TIMESTAMP) {
                    type_string = g_string_append(type_string, "TIMESTAMP");
                } else {
                    type_string = g_string_append(type_string, "TEXT");
                    break;
                }
                break;
            default:
                // g_debug("DEFAULT TYPE TYPE NAME %s",g_type_name(type));
                g_warning("MYSQL types : unnown GType %" G_GSIZE_FORMAT ":%s ", type, g_type_name(type));
                type_string = g_string_append(type_string, "TEXT");
                break;
        }
    } else if (flag & SQLITE_DB_PRIMERY_KEY)
        type_string = g_string_append(type_string, "INTEGER PRIMARY KEY AUTOINCREMENT");
    if (flag & SQLITE_DB_NOT_NULL) type_string = g_string_append(type_string, " NOT NULL");
    gchar *ret = g_strdup(type_string->str);
    g_string_free(type_string, TRUE);
    return ret;
}

gchar *CreateTableSQL(GObject *object, const gchar *table) {
    GString *    string = g_string_new("");
    guint        len    = 0;
    GParamSpec **pspec  = g_object_class_list_properties(G_OBJECT_GET_CLASS(object), &len);
    int          i      = 0;
    for (i = 0; i < len; i++) {
        gchar *type = SqliteConvertGType(pspec[i]->value_type, pspec[i]->flags);
        g_string_append_printf(string, "%s%s %s", i > 0 ? "," : "", sqlParserRps(pspec[i]->name), type);
        if (type) g_free(type);
    }
    if (pspec != NULL) g_free(pspec);
    gchar *result = g_strdup_printf("CREATE TABLE IF NOT EXISTS %s (%s);", table, string->str);
    g_string_free(string, TRUE);
    // g_debug("CREATE_TABLE:%s",result);
    return result;
}

static gchar *getObjectValue(GObject *object, GParamSpec *pspec) {
    gchar * ret = NULL;
    GValue *val = value_new(pspec->value_type);
    g_object_get_property(object, pspec->name, val);
    if (pspec->value_type == G_TYPE_STRING) {
        char *escaped = sqlite3_mprintf("%q", g_value_get_string(val));
        ret           = g_strdup_printf("'%s'", escaped);
        sqlite3_free(escaped);
    } else if (G_TYPE_IS_OBJECT(pspec->value_type)) {
        GObject *vo = g_value_get_object(val);
        if (vo == NULL) {
            ret = g_strdup_printf("NULL");
        } else {
            gchar *data;
            gsize  len;
            data          = json_gobject_to_data(vo, &len);
            char *escaped = sqlite3_mprintf("%q", data);
            ret           = g_strdup_printf("'%s'", escaped);
            sqlite3_free(escaped);
            g_free(data);
        }
    } else if (G_TYPE_IS_BOXED(pspec->value_type)) {
        JsonNode *     node      = json_boxed_serialize(pspec->value_type, g_value_get_boxed(val));
        JsonGenerator *generator = json_generator_new();
        json_generator_set_root(generator, node);
        gsize  len     = 0;
        gchar *data    = json_generator_to_data(generator, &len);
        char * escaped = sqlite3_mprintf("%q", data);
        ret            = g_strdup_printf("'%s'", escaped);
        sqlite3_free(escaped);
        g_free(data);
        json_node_free(node);
        g_object_unref(generator);
    } else {
        ret = value_stringify(val);
    }
    value_free(val);
    return ret;
}
GValue *getPspecValue(GParamSpec *pspec, const gchar *sqliteVal) {
    if (sqliteVal == NULL) return NULL;
    GValue *val = value_new(pspec->value_type);
    if (G_TYPE_IS_OBJECT(pspec->value_type)) {
        GError * error  = NULL;
        GObject *object = json_gobject_from_data(pspec->value_type, sqliteVal, g_utf8_strlen(sqliteVal, -1), &error);
        if (error) {
            value_free(val);
            g_error_free(error);
            if (object) g_object_unref(object);
            return NULL;
        }
        g_value_take_object(val, object);
        return val;
    } else if (G_TYPE_IS_BOXED(pspec->value_type)) {
        JsonParser *parser;
        parser = json_parser_new();
        g_assert(JSON_IS_PARSER(parser));
        if (!json_parser_load_from_data(parser, sqliteVal, -1, NULL)) {
            return NULL;
        }
        gpointer boxed = json_boxed_deserialize(pspec->value_type, json_parser_get_root(parser));
        if (boxed) {
            g_value_set_boxed(val, boxed);
            g_boxed_free(pspec->value_type, boxed);
        }
        g_object_unref(parser);
        return val;  // FIXME: create GArray value with .
    } else {
        if (!set_gvalue_from_string(val, sqliteVal))  // FIX: need string escape.
        {
            value_free(val);
            return NULL;
        }
        return val;
    }
    return NULL;
}

gchar *InsertSQLString(GObject *object, const gchar *table) {
    guint        len    = 0;
    GParamSpec **pspec  = g_object_class_list_properties(G_OBJECT_GET_CLASS(object), &len);
    int          i      = 0;
    gint         vs     = 0;
    GString *    names  = g_string_new("");
    GString *    values = g_string_new("");
    for (i = 0; i < len; i++) {
        if (!(pspec[i]->flags & SQLITE_DB_PRIMERY_KEY)) {
            gchar *value_str = getObjectValue(object, pspec[i]);
            g_string_append_printf(names, "%s%s", vs > 0 ? "," : "", sqlParserRps(pspec[i]->name));
            g_string_append_printf(values, "%s%s", vs > 0 ? "," : "", value_str);
            g_free(value_str);
            vs++;
        }
    }
    gchar *result = g_strdup_printf("INSERT INTO %s (%s) VALUES(%s);", table, names->str, values->str);
    g_string_free(names, TRUE);
    g_string_free(values, TRUE);
    if (pspec) g_free(pspec);
    return result;
}

struct _LarSqlitePrivate {
    sqlite3 *mySQL;
    gchar *  table;

    gchar *dbpath;
    guint  flag;

    gboolean isInit;
    GObject *model;
    GMutex   m;
};

// static const char *larDBPath = "/usr/share/tera/db"

static void lock(LarSqlite *connection) { g_mutex_lock(&connection->priv->m); }
static void unlock(LarSqlite *connection) { g_mutex_unlock(&connection->priv->m); }

#define LAR_SQLITE_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_LAR_SQLITE, LarSqlitePrivate))

G_DEFINE_TYPE(LarSqlite, sqlite_connection, G_TYPE_OBJECT);

static void sqlite_connection_init(LarSqlite *object) {
    object->priv        = G_TYPE_INSTANCE_GET_PRIVATE(object, TYPE_LAR_SQLITE, LarSqlitePrivate);
    object->priv->mySQL = NULL;
}

static void sqlite_connection_finalize(GObject *object) {
    /* TODO: Add deinitalization code here */
    // g_debug("SQLite connection finalize");
    LarSqlite *connection = LAR_SQLITE(object);
    if (connection->priv->mySQL) sqlite3_close(connection->priv->mySQL);
    connection->priv->mySQL = NULL;
    if (connection->priv->dbpath) g_free(connection->priv->dbpath);
    G_OBJECT_CLASS(sqlite_connection_parent_class)->finalize(object);
}

void sqlite_connection_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    LarSqlite *connection = LAR_SQLITE(object);
    switch (prop_id) {
        case SQLITE_FLAG:
            connection->priv->flag = g_value_get_uint(value);
            break;
        case SQLITE_DATABASE:
            if (connection->priv->dbpath) g_free(connection->priv->dbpath);
            connection->priv->dbpath = g_value_dup_string(value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}

void sqlite_connection_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    LarSqlite *connection = LAR_SQLITE(object);
    switch (prop_id) {
        case SQLITE_FLAG:
            g_value_set_uint(value, connection->priv->flag);
            break;
        case SQLITE_DATABASE:
            g_value_set_string(value, connection->priv->dbpath);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}

void sqlite_connection_constructed(GObject *object) {
    LarSqlite *connection = LAR_SQLITE(object);
    if (!g_file_test(connection->priv->dbpath, G_FILE_TEST_EXISTS)) {
        gchar *dir = g_path_get_dirname(connection->priv->dbpath);
        g_mkdir_with_parents(dir, 0777);
        if (dir) g_free(dir);
    }
}

static void sqlite_connection_class_init(LarSqliteClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // ConnectionClass *parent_class = CONNECTION_CLASS(klass);
    g_type_class_add_private(klass, sizeof(LarSqlitePrivate));
    object_class->finalize     = sqlite_connection_finalize;
    object_class->set_property = sqlite_connection_set_property;
    object_class->get_property = sqlite_connection_get_property;
    object_class->constructed  = sqlite_connection_constructed;
    g_object_class_install_property(object_class, SQLITE_FLAG,
                                    g_param_spec_uint("flag", "connection construct prop", "Set connection's port", 0, G_MAXUINT16, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                                                      G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE));
    g_object_class_install_property(
        object_class, SQLITE_DATABASE,
        g_param_spec_string("database", "connection construct prop", "Set connection's database", "db.sqlite", G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE));
}

static int LarSqliteBusyHandler(void *data, int retry) {
    if (retry < MAX_BUSY_RETRY) {
        sqlite3_sleep(100);
        return 1;
    }
    return 0;
}

static gboolean LarSqliteOpen(LarSqlite *connection) {
    lock(connection);

    if (connection->priv->mySQL) {
        if (connection->priv->mySQL) sqlite3_close(connection->priv->mySQL);
        connection->priv->mySQL = NULL;
    }
    gboolean ret = FALSE;
    gint     rc  = sqlite3_open_v2(connection->priv->dbpath, &connection->priv->mySQL, connection->priv->flag, NULL);
    if (SQLITE_OK != rc) {
        g_critical("SQLite-connection: connect to data base %s failed - %s", connection->priv->dbpath, sqlite3_errmsg(connection->priv->mySQL));
        // sqlite3_close(connection->priv->mySQL);
        connection->priv->mySQL = NULL;
        unlock(connection);
    } else {
        if (!connection->priv->isInit) {
            sqlite3_exec(connection->priv->mySQL, "PRAGMA journal_mode=WAL", NULL, NULL, NULL);
            sqlite3_exec(connection->priv->mySQL, "PRAGMA synchronous=OFF", NULL, NULL, NULL);
            connection->priv->isInit = TRUE;
        }
        sqlite3_busy_handler(connection->priv->mySQL, LarSqliteBusyHandler, connection);
        ret = TRUE;
    }
    unlock(connection);
    return ret;
}

static void LarSqliteClose(LarSqlite *connection) {
    lock(connection);
    if (connection->priv->mySQL) {
        if (connection->priv->mySQL) sqlite3_close(connection->priv->mySQL);
        connection->priv->mySQL = NULL;
    }
    unlock(connection);
}

void m_LarSqliteJournalWal(LarSqlite *connection) {
    g_return_val_if_fail(connection != NULL, FALSE);
    g_return_val_if_fail(IS_LAR_SQLITE(connection), FALSE);
    if (!LarSqliteOpen(connection)) {
//        return FALSE;
// Gerhard: Meldung: warning: 'return' with a value, in function returning void [enabled by default]
        return;
    }
    lock(connection);
}

typedef struct {
    GSList *   last;
    GSList *   start;
    LarSqlite *connection;
    GObject *  model;

} sqLiteCallbackObjects;

static int sqliteConnectionLoadModelsCallback(void *data, int argc, char **argv, char **azColName) {
    //	g_debug("connection_load_models_callback");
    sqLiteCallbackObjects *cbo = (sqLiteCallbackObjects *)data;
    if (cbo == NULL) return -1;
    if (argc <= 0) return -1;

    GParameter *param = calloc(sizeof(GParameter), argc);
    gint        npar  = 0;
    int         i;
    for (i = 0; i < argc; i++) {
        GParamSpec *pspec = g_object_class_find_property(G_OBJECT_GET_CLASS(cbo->model), azColName[i]);
        if (pspec) {
            GValue *value = getPspecValue(pspec, argv[i]);
            if (value) {
                param[npar].name = azColName[i];
                g_value_init(&param[npar].value, value->g_type);
                g_value_copy(value, &param[npar].value);
                npar++;
                value_free(value);
            }
        }
    }
    if (param && npar > 0) {
        GObject *model = g_object_newv(G_OBJECT_TYPE(cbo->model), npar, param);
        int      i     = 0;
        for (i = 0; i < npar; i++) {
            if (param[i].value.g_type > G_TYPE_NONE) g_value_unset(&param[i].value);
        }
        if (model) {
            GSList *nl = g_slice_new(GSList);
            nl->data   = model;
            nl->next   = NULL;
            if (cbo->last != NULL) cbo->last->next = nl;
            cbo->last = nl;
            if (cbo->start == NULL) cbo->start = cbo->last;
        }
    }
    if (param) g_free(param);
    return 0;
}

static void sqliteObjectUpdatePrimeryKey(GObject *object, guint64 pk) {
    GParamSpec *pspec = g_object_class_find_property(G_OBJECT_GET_CLASS(object), SQLITE_PK_PROPERTY);
    if (pspec == NULL) return;
    if (G_TYPE_UINT64 != pspec->value_type) return;
    g_object_set(object, pspec->name, pk, NULL);
}

static guint64 sqliteObjectGetPrimeryKey(GObject *object) {
    GParamSpec *pspec = g_object_class_find_property(G_OBJECT_GET_CLASS(object), SQLITE_PK_PROPERTY);
    if (pspec == NULL) return 0;
    if (G_TYPE_UINT64 != pspec->value_type) return 0;
    guint64 pk = 0;
    g_object_get(object, pspec->name, &pk, NULL);
    return pk;
}

gboolean LarSqliteDeleteTable(LarSqlite *connection, const gchar *table_name) {
    g_return_val_if_fail(connection != NULL, FALSE);
    g_return_val_if_fail(IS_LAR_SQLITE(connection), FALSE);
    g_return_val_if_fail(table_name != NULL, FALSE);
    if (!LarSqliteOpen(connection)) {
        return FALSE;
    }
    lock(connection);
    gboolean ret = TRUE;
    char *   buf = g_strdup_printf("DROP TABLE IF EXISTS %s", table_name);
    if (sqlite3_exec(LAR_SQLITE(connection)->priv->mySQL, buf, NULL, NULL, NULL)) {
        g_critical("SQLite-connection: delete table %s error - %s", table_name, sqlite3_errmsg(LAR_SQLITE(connection)->priv->mySQL));
        ret = FALSE;
    }
    g_free(buf);
    unlock(connection);
    LarSqliteClose(connection);
    return ret;
}
gboolean m_LarSqliteCreateTable(LarSqlite *connection, const gchar *table, GObject *object) {
    g_return_val_if_fail(connection != NULL, 0);
    g_return_val_if_fail(IS_LAR_SQLITE(connection), 0);

    if (!LarSqliteOpen(connection)) {
        return 0;
    }
    lock(connection);
    guint rc = sqlite3_exec(connection->priv->mySQL, "BEGIN TRANSACTION", NULL, NULL, NULL);
    if (rc == SQLITE_OK) {
        gchar *createTable = CreateTableSQL(object, table);
        rc                 = sqlite3_exec(connection->priv->mySQL, createTable, NULL, NULL, NULL);
        g_debug("CREATE_TABLE:%s", createTable);
        if (createTable) g_free(createTable);
    }
    gboolean ret = FALSE;
    if (rc == SQLITE_OK) {
        rc  = sqlite3_exec(connection->priv->mySQL, "END TRANSACTION", NULL, NULL, NULL);
        ret = TRUE;
    } else {
        g_warning("larsqlite-error:%s\n", sqlite3_errmsg(LAR_SQLITE(connection)->priv->mySQL));
        rc = sqlite3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, NULL, NULL);
    }
    unlock(connection);
    LarSqliteClose(connection);
    return ret;
}
guint64 m_LarSqliteInsertObject(LarSqlite *connection, const gchar *table, GObject *object) {
    g_return_val_if_fail(connection != NULL, 0);
    g_return_val_if_fail(IS_LAR_SQLITE(connection), 0);

    if (!LarSqliteOpen(connection)) {
        return 0;
    }
    lock(connection);
    guint rc = sqlite3_exec(connection->priv->mySQL, "BEGIN TRANSACTION", NULL, NULL, NULL);
    if (rc == SQLITE_OK) {
        gchar *createTable = CreateTableSQL(object, table);
        rc                 = sqlite3_exec(connection->priv->mySQL, createTable, NULL, NULL, NULL);
        if (createTable) g_free(createTable);
        if (rc == SQLITE_OK) {
            gchar *insertSQL = InsertSQLString(object, table);
            rc               = sqlite3_exec(connection->priv->mySQL, insertSQL, NULL, NULL, NULL);
            if (rc != SQLITE_OK) g_warning("wrong format %d:%s - %s\n", rc, insertSQL, sqlite3_errmsg(LAR_SQLITE(connection)->priv->mySQL));
            if (insertSQL) g_free(insertSQL);
        }
    }
    guint64 ret = 0;
    if (rc == SQLITE_OK) {
        rc  = sqlite3_exec(connection->priv->mySQL, "END TRANSACTION", NULL, NULL, NULL);
        ret = (guint64)sqlite3_last_insert_rowid(connection->priv->mySQL);
        if (ret > 0) sqliteObjectUpdatePrimeryKey(object, ret);
    } else {
        g_warning("larsqlite-error:%s\n", sqlite3_errmsg(LAR_SQLITE(connection)->priv->mySQL));
        rc = sqlite3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, NULL, NULL);
    }
    unlock(connection);
    LarSqliteClose(connection);
    return ret;
}

guint64 m_LarSqliteInsertObjects(LarSqlite *connection, const gchar *table, GSList *objects) {
    g_return_val_if_fail(connection != NULL, 0);
    g_return_val_if_fail(objects != NULL, 0);
    g_return_val_if_fail(IS_LAR_SQLITE(connection), 0);

    if (!LarSqliteOpen(connection)) {
        return 0;
    }
    lock(connection);
    guint rc = sqlite3_exec(connection->priv->mySQL, "BEGIN TRANSACTION", NULL, NULL, NULL);
    if (rc == SQLITE_OK) {
        if (G_IS_OBJECT(objects->data)) {
            gchar *createTable = CreateTableSQL(G_OBJECT(objects->data), table);
            rc                 = sqlite3_exec(connection->priv->mySQL, createTable, NULL, NULL, NULL);
            if (createTable) g_free(createTable);
        }
        if (rc == SQLITE_OK) {
            for (; objects != NULL; objects = objects->next) {
                if (G_IS_OBJECT(objects->data)) {
                    gchar *insertSQL = InsertSQLString(G_OBJECT(objects->data), table);
                    rc               = sqlite3_exec(connection->priv->mySQL, insertSQL, NULL, NULL, NULL);
                    if (insertSQL) g_free(insertSQL);
                    if (rc != SQLITE_OK) {
                        g_warning("wrong format %d:%s\n", rc, insertSQL);
                        break;
                    }
                }
            }
        }
    }
    guint64 ret = 0;
    if (rc == SQLITE_OK) {
        rc  = sqlite3_exec(connection->priv->mySQL, "END TRANSACTION", NULL, NULL, NULL);
        ret = (guint64)sqlite3_last_insert_rowid(connection->priv->mySQL);
    } else {
        g_warning("larsqlite-error:%s\n", sqlite3_errmsg(LAR_SQLITE(connection)->priv->mySQL));
        rc = sqlite3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, NULL, NULL);
    }
    unlock(connection);
    LarSqliteClose(connection);
    return ret;
}

gboolean LarSqliteSendNoSelect(LarSqlite *connection, const gchar *stmt) {
    g_return_val_if_fail(connection != NULL, 0);
    g_return_val_if_fail(IS_LAR_SQLITE(connection), 0);
    if (!LarSqliteOpen(connection)) {
        return FALSE;
    }
    lock(connection);
    sqlite3_stmt *result = NULL;
    // guint64       ret    = 0;
    guint rc = sqlite3_exec(connection->priv->mySQL, "BEGIN TRANSACTION", NULL, NULL, NULL);
    if ((rc == SQLITE_OK)) {
        rc = sqlite3_prepare_v2(connection->priv->mySQL, stmt, strlen(stmt) + 1, &result, NULL);
        if (rc != SQLITE_OK) {
            g_critical("db error: %s STMT=%s", sqlite3_errmsg(connection->priv->mySQL), stmt);
            // exit(1);
        }
        if (result) {
            sqlite3_step(result);
            sqlite3_finalize(result);
        }
    }
    if (rc == SQLITE_OK) {
        rc = sqlite3_exec(connection->priv->mySQL, "COMMIT TRANSACTION", NULL, 0, NULL);
    } else
        rc = sqlite3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, 0, NULL);
    unlock(connection);
    LarSqliteClose(connection);
    return rc == SQLITE_OK;
}

GObject *LarSqliteGetObject(LarSqlite *connection, const gchar *table, GType type, guint64 pk) {
    g_return_val_if_fail(connection != NULL, NULL);
    g_return_val_if_fail(IS_LAR_SQLITE(connection), NULL);
    if (!LarSqliteOpen(connection)) {
        return NULL;
    }
    gchar *       sql = g_strdup_printf("SELECT * FROM %s WHERE %s  %" G_GUINT64_FORMAT ";", table, SQLITE_PK_PROPERTY, pk);
    sqlite3_stmt *res;
    GObject *     object = NULL;
    guint         rc     = sqlite3_prepare_v2(connection->priv->mySQL, sql, -1, &res, 0);
    if (rc == SQLITE_OK) {
        int step = sqlite3_step(res);
        if (step == SQLITE_ROW) {
            gint c = sqlite3_column_count(res);
            gint i = 0;
            for (i = 0; i < c; i++) {
                // sqlite3_column_name(res,i);
            }
        }
    }
    if (res != NULL) sqlite3_finalize(res);
    LarSqliteClose(connection);
    return object;
}

gboolean m_LarSqliteUpdateObjectProperty(LarSqlite *connection, const gchar *table, GObject *object, const gchar *pname) {
    g_return_val_if_fail(connection != NULL, FALSE);
    g_return_val_if_fail(object != NULL, FALSE);
    g_return_val_if_fail(G_IS_OBJECT(object), FALSE);
    g_return_val_if_fail(IS_LAR_SQLITE(connection), FALSE);
    guint64 pk = sqliteObjectGetPrimeryKey(object);
    g_return_val_if_fail(pk > 0, FALSE);
    GParamSpec *pspec = g_object_class_find_property(G_OBJECT_GET_CLASS(object), pname);
    g_return_val_if_fail(pspec != NULL, FALSE);
    gchar *sqlVal = getObjectValue(object, pspec);
    gchar *sql    = g_strdup_printf("UPDATE %s SET %s=%s WHERE %s = %" G_GUINT64_FORMAT ";", table, sqlParserRps(pspec->name), sqlVal, SQLITE_PK_PROPERTY, pk);
    LarSqliteSendNoSelect(connection, sql);
    g_free(sql);
    g_free(sqlVal);
    return TRUE;
}

guint64 LarSqliteLastRowId(LarSqlite *connection) {
    g_return_val_if_fail(connection != NULL, 0);
    g_return_val_if_fail(IS_LAR_SQLITE(connection), 0);
    if (!LarSqliteOpen(connection)) {
        return 0;
    }
    lock(connection);
    guint64 last_id = (guint64)sqlite3_last_insert_rowid(connection->priv->mySQL);
    unlock(connection);
    LarSqliteClose(connection);
    return last_id;
}

static GSList *sqliteConnectionSend(LarSqlite *connection, GType otype, const gchar *smtm) {
    g_return_val_if_fail(smtm != NULL, NULL);
    g_return_val_if_fail(connection != NULL, NULL);
    g_return_val_if_fail(IS_LAR_SQLITE(connection), NULL);
    g_return_val_if_fail(g_type_is_a(otype, G_TYPE_OBJECT), NULL);
    if (!LarSqliteOpen(connection)) {
        // g_critical("open data base %s fail",connection->priv->dbpath);
        return NULL;
    }
    lock(connection);
    GSList *ret = NULL;
    //	g_debug("SQL=%s",smtm);
    sqLiteCallbackObjects *obj = g_malloc(sizeof(sqLiteCallbackObjects));
    obj->last                  = NULL;
    obj->start                 = NULL;
    obj->connection            = connection;
    obj->model                 = g_object_new(otype, NULL);
    guint rc                   = sqlite3_exec(connection->priv->mySQL, "BEGIN TRANSACTION", NULL, 0, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_exec(connection->priv->mySQL, smtm, sqliteConnectionLoadModelsCallback, (void *)obj, NULL);
    }
    if (rc == SQLITE_OK)
        rc = sqlite3_exec(connection->priv->mySQL, "END TRANSACTION", NULL, 0, NULL);
    else
        rc = sqlite3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, 0, NULL);

    ret = obj->start;
    g_object_unref(obj->model);
    g_free(obj);
    if (rc != SQLITE_OK) {
        // g_warning("SEND_ERROR: %s. ERRCODE: %d.\n",
        //	sqlite3_errmsg(connection->priv->mySQL), rc);
    }
    unlock(connection);
    LarSqliteClose(connection);
    return ret;
}

/* MySQL connection create tools
 *
 */

LarSqlite *LarSqliteNew(const gchar *database, guint flag) {
    LarSqlite *connection = LAR_SQLITE(g_object_new(TYPE_LAR_SQLITE, "database", database, "flag", flag, NULL));
    return connection;
}

void SqliteUpdateTable(LarSqlite *connection, GType object_type, const gchar *table) {
    g_return_if_fail(connection != NULL);
    g_return_if_fail(IS_LAR_SQLITE(connection));
    g_return_if_fail(g_type_is_a(object_type, G_TYPE_OBJECT));
    guint ni = 0;
    if (!LarSqliteOpen(connection)) {
        return;
    }
    lock(connection);
    sqlite3_stmt *result    = NULL;
    gchar *       SQL       = g_strdup_printf("SELECT sql FROM sqlite_master where name='%s'", table);
    gint          rc        = sqlite3_prepare(connection->priv->mySQL, SQL, -1, &result, NULL);
    gchar *       old_table = NULL;
    if (rc == SQLITE_OK || rc == SQLITE_DONE) {
        int s;
        s = sqlite3_step(result);
        if (s == SQLITE_ROW) {
            int nCols = sqlite3_column_count(result);
            if (nCols) {
                int nCol;
                for (nCol = 0; nCol < nCols; nCol++) {
                    old_table = g_strdup((gchar *)sqlite3_column_text(result, nCol));
                    if (NULL != strstr((const char *)old_table, "(ref_id") || NULL != strstr((const char *)old_table, "('ref_id'")) {
                        break;
                    } else {
                        g_free(old_table);
                        old_table = NULL;
                    }
                }
            }
        }
        if (result) sqlite3_finalize(result);
    }
    /*	CREATE TABLE LarMeasurementData_backup \
             ('ref_id' INTEGER PRIMARY KEY AUTOINCREMENT,\
             'measurement_art' TEXT,\
             'measurement_changed' DOUBLE,\
             'measurement_channel' INTEGER,\
             'measurement_cv' DOUBLE,\
             'measurement_name' TEXT,\
             'measurement_number' INTUNSIGNED,\
             'measurement_outlier' TINYINT,\
             'measurement_replicate' INTUNSIGNED,\
             'measurement_signal' INTUNSIGNED,\
             'measurement_trigger' INTUNSIGNED,\
             'measurement_type' INTEGER,\
             'measurement_unit' TEXT,\
             'measurement_value' DOUBLE,\
             'measurement_value_row' DOUBLE,\
             'measurement_process' INTEGER DEFAULT 1,\
             'measurement_stream' INTEGER DEFAULT 1); \
             INSERT INTO LarMeasurementData_backup (ref_id, measurement_art, measurement_changed, measurement_channel, measurement_cv, measurement_name,
       measurement_number, measurement_outlier, measurement_replicate, measurement_signal, measurement_trigger, measurement_type, measurement_unit,
       measurement_value, measurement_value_row) \
             SELECT ref_id, measurement_art, measurement_changed, measurement_channel, measurement_cv, measurement_name, measurement_number,
       measurement_outlier, measurement_replicate, measurement_signal, measurement_trigger, measurement_type, measurement_unit, measurement_value,
       measurement_value_row FROM LarMeasurementData; \
             DROP TABLE LarMeasurementData; \
             ALTER TABLE LarMeasurementData_backup RENAME TO LarMeasurementData;
    */
    // g_print("%s check old %s\n",g_type_name(object_type),old_table);
    GString *create = g_string_new("CREATE TABLE ");
    GString *check  = g_string_new("CREATE TABLE ");
    GString *insert = g_string_new("INSERT INTO ");
    GString *select = g_string_new("SELECT ");
    GString *drop   = g_string_new("DROP TABLE ");
    GString *after  = g_string_new("ALTER TABLE  ");
    g_string_append_printf(create, "%s_backup (", g_type_name(object_type));
    g_string_append_printf(check, "\"%s\" (", g_type_name(object_type));
    g_string_append_printf(insert, "%s_backup (", g_type_name(object_type));
    g_string_append_printf(drop, "%s", g_type_name(object_type));
    g_string_append_printf(after, "%s_backup RENAME TO %s", g_type_name(object_type), g_type_name(object_type));
    GType *iface    = g_type_interfaces(object_type, &ni);
    int    i        = 0;
    guint  ccounter = 0;
    guint  icounter = 0;
    for (i = 0; i < ni; i++) {
        // g_debug("load interface[%d]=%s",i,g_type_name(iface[i]));
        guint        npar   = 0;
        gpointer     itable = g_type_default_interface_ref(iface[i]);
        GParamSpec **pspec  = g_object_interface_list_properties(itable, &npar);
        if (pspec != NULL) {
            int j = 0;

            for (j = 0; j < npar; j++) {
                // if (pspec[j]->flags & MODEL_DB_PROP) {
                gchar *name          = sqlParserDupRps(pspec[j]->name);
                gchar *value_type    = SqliteConvertGType(pspec[j]->value_type, pspec[j]->flags);
                gchar *default_value = value_stringify(g_param_spec_get_default_value(pspec[j]));
                // g_print("check %s ccounter=%d icounter=%d\n",name,ccounter,icounter);
                if (pspec[j]->value_type == G_TYPE_STRING)
                    g_string_append_printf(create, "%s'%s' %s DEFAULT '%s'", ccounter > 0 ? ", " : "", name, value_type, default_value);
                else
                    g_string_append_printf(create, "%s'%s' %s DEFAULT %s", ccounter > 0 ? ", " : "", name, value_type, default_value);

                if (pspec[j]->value_type == G_TYPE_STRING)
                    g_string_append_printf(check, "%s'%s' %s DEFAULT '%s'", ccounter > 0 ? ", " : "", name, value_type, default_value);
                else
                    g_string_append_printf(check, "%s'%s' %s DEFAULT %s", ccounter > 0 ? ", " : "", name, value_type, default_value);
                ccounter = ccounter + 1;

                if (old_table != NULL && NULL != strstr(old_table, name)) {
                    g_string_append_printf(insert, "%s%s", icounter > 0 ? ", " : "", name);
                    g_string_append_printf(select, "%s%s", icounter > 0 ? ", " : "", name);
                    icounter++;
                }
                g_free(name);
                g_free(default_value);
                g_free(value_type);
                // }
            }
            g_free(pspec);
        }
        if (itable) g_type_default_interface_unref(itable);
    }
    g_string_append_printf(check, ")");
    g_string_append_printf(insert, ") %s  FROM %s;", select->str, table);
    g_string_append_printf(create, ");");
    g_free(iface);
    if (old_table == NULL) {
        if (rc == SQLITE_OK) {
            guint rc = sqlite3_prepare_v2(connection->priv->mySQL, check->str, check->len, &result, NULL);
            if (rc == SQLITE_OK) {
                sqlite3_step(result);
                sqlite3_finalize(result);
                sqlite3_exec(connection->priv->mySQL, "COMMIT;", NULL, NULL, NULL);
            } else {
                sqlite3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, 0, NULL);
            }
        } else {
            sqlite3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, 0, NULL);
        }
    } else if (0 != g_strcmp0(check->str, old_table)) {
        rc = sqlite3_exec(connection->priv->mySQL, "BEGIN TRANSACTION;", NULL, 0, NULL);
        if (rc == SQLITE_OK) {
            guint rc = sqlite3_prepare_v2(connection->priv->mySQL, create->str, create->len, &result, NULL);
            if (rc == SQLITE_OK) {
                sqlite3_step(result);
                sqlite3_finalize(result);
                rc = sqlite3_prepare_v2(connection->priv->mySQL, insert->str, insert->len, &result, NULL);
                if (rc == SQLITE_OK) {
                    sqlite3_step(result);
                    sqlite3_finalize(result);
                    rc = sqlite3_prepare_v2(connection->priv->mySQL, drop->str, drop->len, &result, NULL);
                    if (rc == SQLITE_OK) {
                        sqlite3_step(result);
                        sqlite3_finalize(result);
                        rc = sqlite3_prepare_v2(connection->priv->mySQL, after->str, after->len, &result, NULL);
                        if (rc == SQLITE_OK) {
                            sqlite3_step(result);
                            sqlite3_finalize(result);
                            sqlite3_exec(connection->priv->mySQL, "COMMIT;", NULL, NULL, NULL);
                        } else {
                            // g_print ( "db error 5: %s\n", sqlite3_errmsg(connection->priv->mySQL));
                            sqlite3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, 0, NULL);
                        }
                    } else {
                        // g_print ( "db error 4: %s\n", sqlite3_errmsg(connection->priv->mySQL));
                        sqlite3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, 0, NULL);
                    }
                } else {
                    // g_print ( "db error 3: %s\n", sqlite3_errmsg(connection->priv->mySQL));
                    sqlite3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, 0, NULL);
                }
            } else {
                // g_print ( "db error 2: %s\n", sqlite3_errmsg(connection->priv->mySQL));
                sqlite3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, 0, NULL);
            }
        } else {
            // g_print ( "db error 1: %s\n", sqlite3_errmsg(connection->priv->mySQL));
            sqlite3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, 0, NULL);
        }
    }
    g_string_free(create, TRUE);
    g_string_free(check, TRUE);
    g_string_free(insert, TRUE);
    g_string_free(select, TRUE);
    g_string_free(drop, TRUE);
    g_string_free(after, TRUE);
    unlock(connection);
    LarSqliteClose(connection);
}

gboolean m_LarSqliteSelectCallback(LarSqlite *connection, int (*callback)(void *, int, char **, char **), gpointer data, const gchar *format, ...) {
    g_return_val_if_fail(connection != NULL, FALSE);
    g_return_val_if_fail(IS_LAR_SQLITE(connection), FALSE);
    if (!LarSqliteOpen(connection)) {
        // g_critical("open data base %s fail",connection->priv->dbpath);
        return FALSE;
    }
    lock(connection);
    gchar * old_locale = g_strdup(setlocale(LC_ALL, NULL));
    va_list args;
    setlocale(LC_ALL, "en_US.UTF-8");
    gchar *SQL;
    va_start(args, format);
    SQL = g_strdup_vprintf(format, args);
    va_end(args);
    gint  rc;
    char *errmsg;
    rc = sqlite3_exec(connection->priv->mySQL, SQL, callback, data, &errmsg);
    if (errmsg != NULL) { /* 3 */
        g_critical("Error in sqlite3_exec: %s", errmsg);
        sqlite3_free(errmsg);
    }
    setlocale(LC_ALL, old_locale);
    g_free(old_locale);
    g_free(SQL);
    unlock(connection);
    LarSqliteClose(connection);
    return rc == SQLITE_OK;
}

GSList *m_LarSqliteSelectObjects(LarSqlite *connection, GType type, const gchar *format, ...) {
    g_return_val_if_fail(connection != NULL, NULL);
    g_return_val_if_fail(IS_LAR_SQLITE(connection), NULL);
    gchar * old_locale = g_strdup(setlocale(LC_ALL, NULL));
    va_list args;
    setlocale(LC_ALL, "en_US.UTF-8");
    gchar *SQL;
    va_start(args, format);
    SQL = g_strdup_vprintf(format, args);
    va_end(args);
    // g_print("SQL:%s", SQL);
    GSList *res = sqliteConnectionSend(connection, type, SQL);
    setlocale(LC_ALL, old_locale);
    g_free(old_locale);
    g_free(SQL);
    return res;
}

typedef struct {
    gchar *sql;
    GType  type;

} taskData;

void taskDataFree(gpointer data) {
    taskData *td = (taskData *)data;
    if (td->sql != NULL) g_free(td->sql);
    g_free(data);
}

static GQuark errorQuark(void) {
    static GQuark error;
    if (!error) error = g_quark_from_static_string("sqlite3");
    return error;
}

void destroyObjectList(gpointer list) {
    if (list != NULL) {
        g_slist_free_full((GSList *)list, g_object_unref);
    }
}

static void sqliteSelectThread(GTask *task, gpointer source_object, gpointer task_data, GCancellable *cancellable) {
    LarSqlite *connection = LAR_SQLITE(source_object);
    GSList *   res        = NULL;
    taskData * td         = (taskData *)g_task_get_task_data(task);
    if (td == NULL) {
        g_task_return_error(task, g_error_new(errorQuark(), 10, "task data (object type and sql string) should not be nil"));
        return;
    }
    if (g_task_return_error_if_cancelled(task)) return;
    res = sqliteConnectionSend(connection, td->type, td->sql);

    if (g_task_return_error_if_cancelled(task)) {
        if (res) {
            g_slist_free_full(res, g_object_unref);
        }
        return;
    }
    g_task_return_pointer(task, res, destroyObjectList);
    // g_object_unref(task);
    return;
}

GSList *m_LarSqliteSelectObjectsAsyncFinish(GAsyncResult *result, GError **error) { return (GSList *)g_task_propagate_pointer(G_TASK(result), error); }

gboolean m_LarSqliteSelectObjectsAsync(LarSqlite *connection, GType type, GCancellable *cancellable, GAsyncReadyCallback callback, gpointer user_data, const gchar *format, ...) {
    gchar * old_locale = g_strdup(setlocale(LC_ALL, NULL));
    va_list args;
    setlocale(LC_ALL, "en_US.UTF-8");
    gchar *SQL;
    va_start(args, format);
    SQL = g_strdup_vprintf(format, args);
    va_end(args);
    setlocale(LC_ALL, old_locale);
    g_free(old_locale);

    GTask *   task = g_task_new(connection, cancellable, callback, user_data);
    taskData *td   = g_new(taskData, 1);
    td->type       = type;
    td->sql        = SQL;
    g_task_set_task_data(task, td, taskDataFree);
    g_task_run_in_thread(task, sqliteSelectThread);
    g_object_unref(task);
    return TRUE;
}
