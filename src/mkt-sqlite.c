/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * MktSqlite.c
 * Copyright (C) LAR 2017
 *

 */
#include <fcntl.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <fcntl.h>
#include <glib/gprintf.h>
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>
#include <locale.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "mkt-model.h"
#include "mkt-sqlite.h"
#include "value.h"

#define MAX_BUSY_RETRY 200

#define DB_PRAGMA "PRAGMA synchronous = FULL;PRAGMA journal_mode = WAL;"

enum { PROP_0, SQLITE_FLAG, SQLITE_DATABASE, SQLITE_TABLE, SQLITE_JOURNAL, SQLITE_MODEL };

struct _MktSqlitePrivate {
    sqlite3 *   db;
    gchar *     dbpath;
    gchar *     table;
    gchar *     journal_mod;
    GObject *   model;
    gboolean    temp_open;
    gboolean    wal;
    GParamSpec *pk_spec;
};

GQuark mkt_sqlite_error_quark() {
    static GQuark error;
    if (!error) error = g_quark_from_static_string("sqlite-error");
    return error;
}

gchar *mkt_sqlite_parser_rps(const gchar *pname) {
    GString *str = g_string_new("");
    gchar *  p   = (gchar *)pname;
    while (*p != '\0' || p - pname < 1024) {
        if (*p == '-') g_string_append_c(str, '_');
        g_string_append_c(str, *p);
        p++;
    }
    gchar *ret = str->str;
    g_string_free(str, FALSE);
    return ret;
}

// static gchar *sqlParserDupRps(const gchar *pname) {
//     gchar *ret = g_strdup(pname);
//     gchar *p   = (gchar *)ret;
//     while (*p != '\0') {
//         if (*p == '-') *p = '_';
//         p++;
//     }
//     return ret;
// }

gchar *mkt_sqlite_now_utc() {
    GDateTime *dt  = g_date_time_new_now_utc();
    gchar *    res = g_date_time_format(dt, "%Y.%m.%dT%H:%M:%S");
    g_date_time_unref(dt);
    return res;
}
gchar *mkt_sqlite_now_local() {
    GTimeZone *tz  = g_time_zone_new_local();
    GDateTime *dt  = g_date_time_new_now(tz);
    gchar *    res = g_date_time_format(dt, "%Y.%m.%dT%H:%M:%S");
    g_date_time_unref(dt);
    g_time_zone_unref(tz);
    return res;
}
gchar *mkt_sqlite_from_dt(GDateTime *dt) {
    gchar *res = g_date_time_format(dt, "%Y.%m.%dT%H:%M:%S");
    return res;
}

// static GType SqliteConvertToGType(guint SqliteType) {
//     switch (SqliteType) {
//         case 1:
//             return G_TYPE_INT;
//         case 2:
//             return G_TYPE_STRING;  // MySQL name TINYINT
//         case 3:
//             return G_TYPE_STRING;  // MySQL name INT
//         case 4:
//             return G_TYPE_DOUBLE;
//         case 5:
//             return G_TYPE_STRING;  // MySQL name BIGINT
//         default:
//             return G_TYPE_NONE;
//     }
// };

static gchar *mkt_sqlite_convert_type(GType type, guint flag) {
    GString *type_string = g_string_new("");
    if (G_TYPE_IS_BOXED(type)) {
        type_string = g_string_append(type_string, "TEXT");
    } else if (G_TYPE_IS_OBJECT(type)) {
        type_string = g_string_append(type_string, "TEXT");
    } else if (!(flag & MKT_SQLITE_PRIMERY_KEY)) {
        switch (type) {
            case G_TYPE_BOOLEAN:
                type_string = g_string_append(type_string, "BOOLEAN");
                break;
            case G_TYPE_CHAR:
                type_string = g_string_append(type_string, "SMALLINT");
                break;
            case G_TYPE_UCHAR:
                type_string = g_string_append(type_string, "INTEGER");
                break;
            case G_TYPE_INT:
                type_string = g_string_append(type_string, "INTEGER");
                break;
            case G_TYPE_UINT:
                type_string = g_string_append(type_string, "INTEGER");
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
                if (flag & MKT_SQLITE_TIMESTAMP) {
                    type_string = g_string_append(type_string, "TIMESTAMP");
                } else {
                    type_string = g_string_append(type_string, "TEXT");
                    break;
                }
                break;
            default:
                g_warning("MYSQL types : unnown GType %" G_GSIZE_FORMAT ":%s ", type, g_type_name(type));
                type_string = g_string_append(type_string, "TEXT");
                break;
        }
    } else if (flag & MKT_SQLITE_PRIMERY_KEY)
        type_string = g_string_append(type_string, "INTEGER PRIMARY KEY AUTOINCREMENT");
    if (flag & MKT_SQLITE_NOT_NULL) type_string = g_string_append(type_string, " NOT NULL");
    gchar *ret = g_strdup(type_string->str);
    g_string_free(type_string, TRUE);
    return ret;
}

static gchar *mkt_sqlite_creta_table_sql(MktSqlite *conn) {
    GString *    string = g_string_new("");
    guint        len    = 0;
    GParamSpec **pspec  = g_object_class_list_properties(G_OBJECT_GET_CLASS(conn->priv->model), &len);
    int          i      = 0;
    for (i = 0; i < len; i++) {
        gchar *type  = mkt_sqlite_convert_type(pspec[i]->value_type, pspec[i]->flags);
        gchar *pname = mkt_sqlite_parser_rps(pspec[i]->name);
        g_string_append_printf(string, "%s%s %s", i > 0 ? "," : "", pname, type);
        if (type) g_free(type);
        if (pname) g_free(pname);
    }
    if (pspec != NULL) g_free(pspec);
    gchar *result = g_strdup_printf("CREATE TABLE IF NOT EXISTS %s (%s);", conn->priv->table, string->str);
    g_string_free(string, TRUE);
    return result;
}

static gchar *mkt_sqlite_get_object_value(GObject *object, GParamSpec *pspec) {
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
static GValue *mkt_sqlite_get_pspec_value(GParamSpec *pspec, const gchar *sqliteVal) {
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

static gchar *mkt_sqlite_insert_sql(MktSqlite *conn, GObject *object) {
    guint        len    = 0;
    GParamSpec **pspec  = g_object_class_list_properties(G_OBJECT_GET_CLASS(object), &len);
    int          i      = 0;
    gint         vs     = 0;
    GString *    names  = g_string_new("");
    GString *    values = g_string_new("");
    for (i = 0; i < len; i++) {
        if (!(pspec[i]->flags & MKT_SQLITE_PRIMERY_KEY)) {
            if (g_object_class_find_property(G_OBJECT_GET_CLASS(conn->priv->model), pspec[i]->name)) {
                gchar *value_str = mkt_sqlite_get_object_value(object, pspec[i]);
                gchar *pname     = mkt_sqlite_parser_rps(pspec[i]->name);
                g_string_append_printf(names, "%s%s", vs > 0 ? "," : "", pname);
                g_string_append_printf(values, "%s%s", vs > 0 ? "," : "", value_str);
                g_free(value_str);
                g_free(pname);
                vs++;
            }
        }
    }
    gchar *result = g_strdup_printf("INSERT INTO %s (%s) VALUES(%s);", conn->priv->table, names->str, values->str);
    g_string_free(names, TRUE);
    g_string_free(values, TRUE);
    if (pspec) g_free(pspec);
    return result;
}
// static const char *larDBPath = "/usr/share/tera/db"

static int busy_handler(void *data, int retry) {
    if (retry < MAX_BUSY_RETRY) {
        sqlite3_sleep(100);
        return 1;
    }
    return 0;
}

guint mkt_sqlite3_exec(sqlite3 *db, const gchar *sql, sqlite3_callback cb, void *v, char **c) {
    guint rc     = SQLITE_BUSY;
    guint repeat = 0;
    while (SQLITE_OK != rc) {
        rc = sqlite3_exec(db, sql, cb, v, c);
        if (rc == SQLITE_BUSY) {
            repeat++;
            if (repeat > 100) {
                g_usleep(100);
            } else {
                return rc;
            }
        } else {
            return rc;
        }
    }
    return rc;
}

guint mkt_sqlite3_open_v2(const char *path, sqlite3 **db, int flag, const char *vfs) {
    gint rc     = sqlite3_open_v2(path, db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    gint repeat = 0;
    while (SQLITE_OK != rc) {
        switch (rc) {
            case SQLITE_BUSY:
            case SQLITE_LOCKED:
            case SQLITE_INTERRUPT:
                repeat++;
                if (repeat > 1000) {
                    sqlite3_close(*db);
                    g_usleep(100);
                    rc = sqlite3_open_v2(path, db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
                } else {
                    return rc;
                }
                continue;
            default:
                return rc;
        }
    }
    return rc;
}

guint mkt_sqlite3_setup_wal(sqlite3 *db, int ms, void *data) {
    guint rc = sqlite3_busy_timeout(db, ms);
    sqlite3_busy_handler(db,busy_handler,data);
    rc       = mkt_sqlite3_exec(db, DB_PRAGMA, NULL, NULL, NULL);
    return rc;
}

gboolean mkt_sqlite_open(MktSqlite *conn) {
    if (conn->priv->db) {
        return TRUE;
    }
    gint rc = mkt_sqlite3_open_v2(conn->priv->dbpath, &conn->priv->db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (SQLITE_OK != rc) {
        g_warning("open %s error - %s", conn->priv->dbpath, sqlite3_errmsg(conn->priv->db));
        sqlite3_close(conn->priv->db);
        conn->priv->db = NULL;
        return FALSE;
    } else {
        rc = mkt_sqlite3_setup_wal(conn->priv->db, 2000, conn);
        if (rc != SQLITE_OK) {
            g_warning("%s : setup db error - %s", conn->priv->dbpath, sqlite3_errmsg(conn->priv->db));
            sqlite3_close(conn->priv->db);
            conn->priv->db = NULL;
            return FALSE;
        }
    }
    return TRUE;
}

gboolean mkt_sqlite_open_readonly(MktSqlite *conn) {
    // if (0 == g_strcmp0("WAL", conn->priv->journal_mod)) {
    //     return mkt_sqlite_open(conn);
    // }
    // gint rc = sqlite3_open_v2(conn->priv->dbpath, &conn->priv->db, SQLITE_OPEN_READONLY, NULL);
    // if (SQLITE_OK != rc) {
    //     g_critical("mkt-sqlite : connect to data base %s failed - %s", conn->priv->dbpath, sqlite3_errmsg(conn->priv->db));
    //     sqlite3_close(conn->priv->db);
    //     conn->priv->db = NULL;
    //     return FALSE;
    // } else {
    //     gchar *init_pragma = g_strdup_printf("PRAGMA journal_mode=WAL; PRAGMA synchronous=OFF;", conn->priv->journal_mod);
    //     rc                 = sqlite3_exec(conn->priv->db, init_pragma, NULL, NULL, NULL);
    //     g_free(init_pragma);
    //     if (rc != SQLITE_OK) {
    //         g_critical("mkt-sqlite : set pragma settings failed - %s", sqlite3_errmsg(conn->priv->db));
    //         sqlite3_close(conn->priv->db);
    //         conn->priv->db = NULL;
    //         return FALSE;
    //     }
    // }
    return TRUE;
}
static void rollback(MktSqlite *conn) {
    if (conn->priv->wal)mkt_sqlite3_exec(conn->priv->db, "ROLLBACK;", NULL, NULL, NULL);
}

static void commit(MktSqlite *conn) {
    if (conn->priv->wal)
        mkt_sqlite3_exec(conn->priv->db, "COMMIT", NULL, NULL, NULL);
    else
        mkt_sqlite3_exec(conn->priv->db, "COMMIT", NULL, NULL, NULL);
}
static gboolean create_table(MktSqlite *conn) {
    if (conn->priv->db == NULL) return FALSE;
    guint rc = sqlite3_exec(conn->priv->db, "BEGIN", NULL, NULL, NULL);
    if (rc == SQLITE_OK) {
        gchar *createTable = mkt_sqlite_creta_table_sql(conn);
        rc                 = sqlite3_exec(conn->priv->db, createTable, NULL, NULL, NULL);
        if (rc != SQLITE_OK) {
            g_warning("mkt-sqlite (%s) error:%s\n", createTable, sqlite3_errmsg(MKT_SQLITE(conn)->priv->db));
            rollback(conn);
        } else
            commit(conn);
        if (createTable) g_free(createTable);
    }
    return rc == SQLITE_OK;
}

void mkt_sqlite_close(MktSqlite *conn) {
    if (conn->priv->db) {
        sqlite3_close(conn->priv->db);
        conn->priv->db = NULL;
    }
}
static void open_db(MktSqlite *conn) {
    if (conn->priv->db == NULL) {
        conn->priv->temp_open = mkt_sqlite_open(conn);
        if (conn->priv->temp_open) create_table(conn);
    }
}

static void close_db(MktSqlite *conn) {
    if (conn->priv->temp_open) {
        mkt_sqlite_close(conn);
        conn->priv->temp_open = FALSE;
    }
}

#define MKT_SQLITE_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), MKT_TYPE_SQLITE, MktSqlitePrivate))

G_DEFINE_TYPE(MktSqlite, mkt_sqlite_conn, G_TYPE_OBJECT);

static void mkt_sqlite_conn_init(MktSqlite *object) {
    object->priv              = G_TYPE_INSTANCE_GET_PRIVATE(object, MKT_TYPE_SQLITE, MktSqlitePrivate);
    object->priv->db          = NULL;
    object->priv->model       = NULL;
    object->priv->table       = NULL;
    object->priv->dbpath      = NULL;
    object->priv->journal_mod = NULL;
    object->priv->temp_open   = FALSE;
    object->priv->wal         = TRUE;
}

static void mkt_sqlite_conn_finalize(GObject *object) {
    MktSqlite *conn = MKT_SQLITE(object);
    mkt_sqlite_close(conn);
    if (conn->priv->dbpath) g_free(conn->priv->dbpath);
    if (conn->priv->model) g_object_unref(conn->priv->model);
    if (conn->priv->table) g_free(conn->priv->table);
    if (conn->priv->journal_mod) g_free(conn->priv->journal_mod);
    G_OBJECT_CLASS(mkt_sqlite_conn_parent_class)->finalize(object);
}

void mkt_sqlite_conn_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    MktSqlite *conn = MKT_SQLITE(object);
    switch (prop_id) {
        case SQLITE_DATABASE:
            if (conn->priv->dbpath) g_free(conn->priv->dbpath);
            conn->priv->dbpath = g_value_dup_string(value);
            break;
        case SQLITE_TABLE:
            if (conn->priv->table) g_free(conn->priv->table);
            conn->priv->table = g_value_dup_string(value);
            break;
        case SQLITE_JOURNAL:
            if (conn->priv->journal_mod) g_free(conn->priv->journal_mod);
            conn->priv->journal_mod = g_value_dup_string(value);
            break;
        case SQLITE_MODEL:
            if (conn->priv->model) g_object_unref(conn->priv->model);
            conn->priv->model = g_value_dup_object(value);

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}

void mkt_sqlite_conn_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    MktSqlite *conn = MKT_SQLITE(object);
    switch (prop_id) {
        case SQLITE_DATABASE:
            g_value_set_string(value, conn->priv->dbpath);
            break;
        case SQLITE_TABLE:
            g_value_set_string(value, conn->priv->table);
            break;
        case SQLITE_JOURNAL:
            g_value_set_string(value, conn->priv->journal_mod);
            break;
        case SQLITE_MODEL:
            g_value_set_object(value, conn->priv->model);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}

void mkt_sqlite_conn_constructed(GObject *object) {
    MktSqlite *conn = MKT_SQLITE(object);
    if (!g_file_test(conn->priv->dbpath, G_FILE_TEST_EXISTS)) {
        gchar *dir = g_path_get_dirname(conn->priv->dbpath);
        g_mkdir_with_parents(dir, 0777);
        if (dir) g_free(dir);
    }
    if (conn->priv->model == NULL) {
        conn->priv->model  = g_object_new(MKT_TYPE_MODEL, NULL);
        guint        len   = 0;
        GParamSpec **pspec = g_object_class_list_properties(G_OBJECT_GET_CLASS(conn->priv->model), &len);
        gint         i     = 0;
        for (i = 0; i < len; i++) {
            if (pspec[i]->flags & MKT_SQLITE_PRIMERY_KEY) {
                conn->priv->pk_spec = g_object_class_find_property(G_OBJECT_GET_CLASS(conn->priv->model), pspec[i]->name);
                break;
            }
        }
        if (pspec) g_free(pspec);
    }
    if (conn->priv->table == NULL) {
        conn->priv->table = g_strdup(G_OBJECT_TYPE_NAME(object));
    }
}

static void mkt_sqlite_conn_class_init(MktSqliteClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // connClass *parent_class = conn_CLASS(klass);
    g_type_class_add_private(klass, sizeof(MktSqlitePrivate));
    object_class->finalize     = mkt_sqlite_conn_finalize;
    object_class->set_property = mkt_sqlite_conn_set_property;
    object_class->get_property = mkt_sqlite_conn_get_property;
    object_class->constructed  = mkt_sqlite_conn_constructed;

    g_object_class_install_property(object_class, SQLITE_DATABASE,
                                    g_param_spec_string("database", "conn construct prop", "Set conn's database", "db.MKT_SQLITE", G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE));

    g_object_class_install_property(object_class, SQLITE_MODEL, g_param_spec_object("model", "conn construct prop", "Set conn's database", G_TYPE_OBJECT, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE));

    g_object_class_install_property(object_class, SQLITE_TABLE, g_param_spec_string("table", "conn construct prop", "Set conn's database", NULL, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE));

    g_object_class_install_property(object_class, SQLITE_JOURNAL,
                                    g_param_spec_string("journal-mod", "conn construct prop", "Set conn's database journal mod", "MEMORY", G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE));
}
typedef struct {
    GSList *   last;
    GSList *   start;
    MktSqlite *conn;
} sqLiteCallbackObjects;

static int mkt_sqlite_load_models_callback(void *data, int argc, char **argv, char **azColName) {
    sqLiteCallbackObjects *cbo = (sqLiteCallbackObjects *)data;
    if (cbo == NULL) return -1;
    if (argc <= 0) return -1;

    GParameter *param = calloc(sizeof(GParameter), argc);
    gint        npar  = 0;
    int         i;
    for (i = 0; i < argc; i++) {
        GParamSpec *pspec = g_object_class_find_property(G_OBJECT_GET_CLASS(cbo->conn->priv->model), azColName[i]);
        if (pspec) {
            GValue *value = mkt_sqlite_get_pspec_value(pspec, argv[i]);
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
        GObject *model = g_object_newv(G_OBJECT_TYPE(cbo->conn->priv->model), npar, param);
        int      i     = 0;
        for (i = 0; i < npar; i++) {
            if (param[i].value.g_type > G_TYPE_NONE) g_value_unset(&param[i].value);
        }
        if (model) {
            cbo->start = g_slist_append(cbo->start, model);
        }
    }
    if (param) g_free(param);
    return 0;
}

static void mkt_sqlite_object_update_pk(MktSqlite *conn, GObject *object, guint64 pk) {
    if (conn->priv->pk_spec == NULL) return;
    if (G_TYPE_UINT64 != conn->priv->pk_spec->value_type) return;
    g_object_set(object, conn->priv->pk_spec->name, pk, NULL);
}

static guint64 sqliteObjectGetPrimeryKey(MktSqlite *conn, GObject *object) {
    if (conn->priv->pk_spec) return 0;
    if (G_TYPE_UINT64 != conn->priv->pk_spec->value_type) return 0;
    guint64 pk = 0;
    g_object_get(object, conn->priv->pk_spec->name, &pk, NULL);
    return pk;
}

gboolean mkt_sqlite_delete_table(MktSqlite *conn) {
    g_return_val_if_fail(conn != NULL, FALSE);
    g_return_val_if_fail(MKT_IS_SQLITE(conn), FALSE);
    open_db(conn);
    gboolean ret = TRUE;
    guint rc = sqlite3_exec(conn->priv->db, "BEGIN;", NULL, NULL, NULL);
    gchar *buf = g_strdup_printf("DROP TABLE IF EXISTS %s", conn->priv->table);
    rc         = sqlite3_exec(conn->priv->db, buf, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        g_warning("delete table (%s) error - %s", buf, sqlite3_errmsg(MKT_SQLITE(conn)->priv->db));
        ret = FALSE;
        rollback(conn);
    } else {
        commit(conn);
    }
    g_free(buf);
    close_db(conn);
    return ret;
}

gboolean mkt_sqlite_create_table(MktSqlite *conn) {
    g_return_val_if_fail(conn != NULL, 0);
    g_return_val_if_fail(MKT_IS_SQLITE(conn), 0);
    open_db(conn);
    gboolean is_create = create_table(conn);
    close_db(conn);
    return is_create;
}

guint64 mkt_sqlite_insert_object(MktSqlite *conn, GObject *object) {
    g_return_val_if_fail(conn != NULL, 0);
    g_return_val_if_fail(MKT_IS_SQLITE(conn), 0);
    gchar *old_locale = g_strdup(setlocale(LC_ALL, NULL));
    setlocale(LC_ALL, "en_US.UTF-8");
    open_db(conn);
    guint64 ret       = 0;
    gchar * insertSQL = mkt_sqlite_insert_sql(conn, object);
    guint   rc        =mkt_sqlite3_exec(conn->priv->db, insertSQL, NULL, NULL, NULL);
    if (rc == SQLITE_OK) {
        ret = (guint64)sqlite3_last_insert_rowid(conn->priv->db);
        if (ret > 0) mkt_sqlite_object_update_pk(conn, object, ret);
    } else {
        g_warning("insert data (%s) error:%s\n", insertSQL, sqlite3_errmsg(MKT_SQLITE(conn)->priv->db));
        // sqlite3_exec(conn->priv->db, "ROLLBACK;", NULL, NULL, NULL);
    }
    if (insertSQL) g_free(insertSQL);
    close_db(conn);
    setlocale(LC_ALL, old_locale);
    g_free(old_locale);
    return ret;
}

guint64 mkt_sqlite_insert_objects(MktSqlite *conn, GSList *objects) {
    g_return_val_if_fail(conn != NULL, 0);
    g_return_val_if_fail(MKT_IS_SQLITE(conn), 0);
    if (objects == NULL || g_slist_length(objects) == 0) return 0;
    open_db(conn);
    g_return_val_if_fail(conn->priv->db != NULL, 0);
    sqlite3_busy_timeout(conn->priv->db, g_slist_length(objects) * 2);
    sqlite3_busy_handler(conn->priv->db,busy_handler,conn);
    gchar *old_locale = g_strdup(setlocale(LC_ALL, NULL));
    setlocale(LC_ALL, "en_US.UTF-8");
    guint rc = mkt_sqlite3_exec(conn->priv->db, "BEGIN", NULL, NULL, NULL);
    if (rc == SQLITE_OK) {
        for (; objects != NULL; objects = objects->next) {
            if (G_IS_OBJECT(objects->data) && g_type_is_a(G_OBJECT_TYPE(objects->data), G_OBJECT_TYPE(conn->priv->model))) {
                gchar *insertSQL = mkt_sqlite_insert_sql(conn, G_OBJECT(objects->data));
                rc               =mkt_sqlite3_exec(conn->priv->db, insertSQL, NULL, NULL, NULL);
                if (rc != SQLITE_OK) {
                    g_warning("insert list (%s) error:%s\n", insertSQL, sqlite3_errmsg(MKT_SQLITE(conn)->priv->db));
                    break;
                }
                if (insertSQL) g_free(insertSQL);
            }
        }
    } else {
        g_warning("insert list open db error:%s\n", sqlite3_errmsg(MKT_SQLITE(conn)->priv->db));
    }
    guint64 ret = 0;
    if (rc == SQLITE_OK) {
        commit(conn);
        ret = (guint64)sqlite3_last_insert_rowid(conn->priv->db);
    } else {
        rollback(conn);
    }
    sqlite3_busy_timeout(conn->priv->db, 2000);
    close_db(conn);
    setlocale(LC_ALL, old_locale);
    g_free(old_locale);
    return ret;
}

gboolean mkt_sqlite_exec(MktSqlite *conn, const gchar *format, ...) {
    g_return_val_if_fail(conn != NULL, 0);
    g_return_val_if_fail(MKT_IS_SQLITE(conn), 0);
    g_return_val_if_fail(conn->priv->db, FALSE);
    gchar *old_locale = g_strdup(setlocale(LC_ALL, NULL));
    setlocale(LC_ALL, "en_US.UTF-8");
    va_list args;
    gchar * stmt;
    va_start(args, format);
    stmt = g_strdup_vprintf(format, args);
    va_end(args);
    open_db(conn);
    guint rc = sqlite3_exec(conn->priv->db, "BEGIN", NULL, NULL, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_exec(conn->priv->db, stmt, NULL, NULL, NULL);
        if (rc != SQLITE_OK) {
            g_warning("exec (%s) error:%s\n", stmt, sqlite3_errmsg(MKT_SQLITE(conn)->priv->db));
            rollback(conn);
        } else {
            commit(conn);
        }
    }
    setlocale(LC_ALL, old_locale);
    g_free(old_locale);
    g_free(stmt);
    close_db(conn);
    return rc == SQLITE_OK;
}

// GObject *mkt_sqlite_get_object(MktSqlite *conn,const gchar *table, GType type, guint64 pk) {
//     g_return_val_if_fail(conn != NULL, NULL);
//     g_return_val_if_fail(MKT_IS_SQLITE(conn), NULL);
//     gchar *       sql = g_strdup_printf("SELECT * FROM %s WHERE %s  %" G_GUINT64_FORMAT ";", table, MKT_SQLITE_PK_PROPERTY, pk);
//     sqlite3_stmt *res;
//     GObject *     object = NULL;
//     guint         rc     = sqlite3_prepare_v2(conn->priv->db, sql, -1, &res, 0);
//     if (rc == SQLITE_OK) {
//         int step = sqlite3_step(res);
//         if (step == SQLITE_ROW) {
//             gint c = sqlite3_column_count(res);
//             gint i = 0;
//             for (i = 0; i < c; i++) {
//                 // sqlite3_column_name(res,i);
//             }
//         }
//     }
//     if (res != NULL) sqlite3_finalize(res);
//     return object;
// }

gboolean mkt_sqlite_update_object_prop(MktSqlite *conn, GObject *object, const gchar *pname) {
    g_return_val_if_fail(conn != NULL, FALSE);
    g_return_val_if_fail(object != NULL, FALSE);
    g_return_val_if_fail(G_IS_OBJECT(object), FALSE);
    g_return_val_if_fail(MKT_IS_SQLITE(conn), FALSE);
    g_return_val_if_fail(conn->priv->pk_spec != NULL, FALSE);
    gchar *old_locale = g_strdup(setlocale(LC_ALL, NULL));
    setlocale(LC_ALL, "en_US.UTF-8");
    open_db(conn);
    g_return_val_if_fail(g_type_is_a(G_OBJECT_TYPE(object), G_OBJECT_TYPE(conn->priv->model)), 0);
    guint64 pk = sqliteObjectGetPrimeryKey(conn, object);
    g_return_val_if_fail(pk > 0, FALSE);
    g_return_val_if_fail(g_object_class_find_property(G_OBJECT_GET_CLASS(conn->priv->model), pname) != NULL, FALSE);
    GParamSpec *pspec = g_object_class_find_property(G_OBJECT_GET_CLASS(object), pname);
    g_return_val_if_fail(pspec != NULL, FALSE);
    gchar *  sqlVal = mkt_sqlite_get_object_value(object, pspec);
    gboolean ret    = mkt_sqlite_exec(conn, "UPDATE %s SET %s=%s WHERE %s = %" G_GUINT64_FORMAT ";", conn->priv->table, mkt_sqlite_parser_rps(pspec->name), sqlVal, conn->priv->pk_spec->name, pk);
    setlocale(LC_ALL, old_locale);
    g_free(old_locale);
    g_free(sqlVal);
    close_db(conn);
    return ret;
}

guint64 mkt_sqlite_last_rowid(MktSqlite *conn) {
    g_return_val_if_fail(conn != NULL, 0);
    g_return_val_if_fail(MKT_IS_SQLITE(conn), 0);
    open_db(conn);
    guint64 last_id = (guint64)sqlite3_last_insert_rowid(conn->priv->db);
    close_db(conn);
    return last_id;
}

static GSList *sqlite_select(MktSqlite *conn, const gchar *smtm) {
    g_return_val_if_fail(smtm != NULL, NULL);
    g_return_val_if_fail(conn != NULL, NULL);
    g_return_val_if_fail(MKT_IS_SQLITE(conn), NULL);
    GSList *               ret = NULL;
    sqLiteCallbackObjects *obj = g_malloc(sizeof(sqLiteCallbackObjects));
    obj->last                  = NULL;
    obj->start                 = NULL;
    obj->conn                  = conn;
    guint rc                   = sqlite3_exec(conn->priv->db, smtm, mkt_sqlite_load_models_callback, (void *)obj, NULL);
    ret                        = obj->start;
    g_free(obj);
    if (rc != SQLITE_OK) {
        g_warning("select %s (%s) error:%s\n", conn->priv->dbpath, smtm, sqlite3_errmsg(MKT_SQLITE(conn)->priv->db));
    }
    return ret;
}

/* MySQL conn create tools
 *
 */

MktSqlite *mkt_sqlite_new(GObject *model, const gchar *database) {
    g_return_val_if_fail(model != NULL, NULL);
    MktSqlite *conn = MKT_SQLITE(g_object_new(MKT_TYPE_SQLITE, "database", database, "model", model, NULL));
    return conn;
}

MktSqlite *mkt_sqlite_new_full(GObject *model, const gchar *database, const gchar *tabel, const gchar *journal_mod) {
    g_return_val_if_fail(model != NULL, NULL);
    g_return_val_if_fail(database != NULL, NULL);
    MktSqlite *conn = MKT_SQLITE(g_object_new(MKT_TYPE_SQLITE, "database", database, "model", model, "table", tabel, "journal-mod", journal_mod, NULL));
    return conn;
}

gboolean mkt_sqlite_select_callback(MktSqlite *conn, int (*callback)(void *, int, char **, char **), gpointer data, const gchar *format, ...) {
    g_return_val_if_fail(conn != NULL, FALSE);
    g_return_val_if_fail(MKT_IS_SQLITE(conn), FALSE);
    open_db(conn);
    g_return_val_if_fail(conn->priv->db, FALSE);
    gchar * old_locale = g_strdup(setlocale(LC_ALL, NULL));
    va_list args;
    setlocale(LC_ALL, "en_US.UTF-8");
    gchar *SQL;
    va_start(args, format);
    SQL = g_strdup_vprintf(format, args);
    va_end(args);
    gint  rc;
    char *errmsg;
    rc = sqlite3_exec(conn->priv->db, SQL, callback, data, &errmsg);
    if (errmsg != NULL) { /* 3 */
        g_warning("mkt-sqlite select error : %s", errmsg);
        sqlite3_free(errmsg);
    }
    setlocale(LC_ALL, old_locale);
    g_free(old_locale);
    g_free(SQL);
    close_db(conn);
    return rc == SQLITE_OK;
}

GSList *mkt_sqlite_select_objects(MktSqlite *conn, const gchar *match, ...) {
    g_return_val_if_fail(conn != NULL, NULL);
    g_return_val_if_fail(MKT_IS_SQLITE(conn), NULL);
    open_db(conn);
    g_return_val_if_fail(conn->priv->db, FALSE);
    gchar * old_locale  = g_strdup(setlocale(LC_ALL, NULL));
    gchar * build_match = NULL;
    va_list args;
    setlocale(LC_ALL, "en_US.UTF-8");
    va_start(args, match);
    build_match = g_strdup_vprintf(match, args);
    va_end(args);
    gchar *  sql_build;
    gboolean is_match = build_match != NULL && g_utf8_strlen(build_match, 200) > 4;
    sql_build         = g_strdup_printf("select * from %s%s%s", conn->priv->table, is_match ? " " : "", is_match ? build_match : ";");
    g_free(build_match);
    GSList *res = sqlite_select(conn, sql_build);
    g_free(sql_build);
    setlocale(LC_ALL, old_locale);
    g_free(old_locale);
    close_db(conn);
    return res;
}

typedef struct {
    gchar *sql;
    GType  type;

} taskData;

static GQuark errorQuark(void) {
    static GQuark error;
    if (!error) error = g_quark_from_static_string("sqlite3");
    return error;
}

static void destroy_object_list(gpointer list) {
    if (list != NULL) {
        g_slist_free_full((GSList *)list, g_object_unref);
    }
}

static void sqliteSelectThread(GTask *task, gpointer source_object, gpointer task_data, GCancellable *cancellable) {
    MktSqlite *conn = MKT_SQLITE(source_object);
    GSList *   res  = NULL;
    gchar *    smtm = (gchar *)g_task_get_task_data(task);
    if (smtm == NULL) {
        g_task_return_error(task, g_error_new(errorQuark(), 10, "task data (object type and sql string) should not be nil"));
        return;
    }
    if (g_task_return_error_if_cancelled(task)) return;

    open_db(conn);
    res = sqlite_select(conn, smtm);
    if (g_task_return_error_if_cancelled(task)) {
        if (res) {
            g_slist_free_full(res, g_object_unref);
        }
        close_db(conn);
        return;
    }
    g_task_return_pointer(task, res, destroy_object_list);
    // g_object_unref(task);
    close_db(conn);
    return;
}

GSList *mkt_sqlite_select_objects_finish(GAsyncResult *result, GError **error) { return (GSList *)g_task_propagate_pointer(G_TASK(result), error); }

gboolean mkt_sqlite_select_objects_async(MktSqlite *conn, GCancellable *cancellable, GAsyncReadyCallback callback, gpointer user_data, const gchar *format, ...) {
    gchar * old_locale = g_strdup(setlocale(LC_ALL, NULL));
    va_list args;
    setlocale(LC_ALL, "en_US.UTF-8");
    gchar *SQL;
    va_start(args, format);
    SQL = g_strdup_vprintf(format, args);
    va_end(args);
    setlocale(LC_ALL, old_locale);
    g_free(old_locale);
    GTask *task = g_task_new(conn, cancellable, callback, user_data);
    g_task_set_task_data(task, SQL, g_free);
    g_task_run_in_thread(task, sqliteSelectThread);
    g_object_unref(task);
    return TRUE;
}

MktSqlite *mkt_sqlite_clone(MktSqlite *conn) { return mkt_sqlite_new_full(conn->priv->model, conn->priv->dbpath, conn->priv->table, conn->priv->journal_mod); }

gboolean mkt_sqlite_check(MktSqlite *conn) {
    g_return_val_if_fail(conn != NULL, FALSE);
    g_return_val_if_fail(MKT_IS_SQLITE(conn), FALSE);
    if (!mkt_sqlite_open(conn)) {
        return FALSE;
    }
    g_return_val_if_fail(conn->priv->db, FALSE);
    close_db(conn);
    return TRUE;
}

void mkt_sqlite_delete_database(MktSqlite *conn) {
    g_return_if_fail(conn != NULL);
    g_return_if_fail(MKT_IS_SQLITE(conn));
    mkt_sqlite_close(conn);
    if (g_str_has_suffix(conn->priv->dbpath, ".sqlite")) {
        g_remove(conn->priv->dbpath);
    }
}
