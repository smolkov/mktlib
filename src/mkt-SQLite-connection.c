
#include "mkt-SQLite-connection.h"
#include <glib.h>
#include <glib/gprintf.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include "market-time.h"
#include "mkt-data-column.h"
#include "mkt-data-model.h"
#include "mkt-error.h"
#include "mkt-model.h"
#include "mkt-value.h"
#include "mkt-sqlite.h"

enum {
    PROP_MKT_MYSQL_CONNECTION_O,

};

struct _MktSQLiteConnectionPrivate {
    sqlite3 *db;
    gboolean after_close;
};

#define MKT_SQLITE_CONNECTION_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), MKT_TYPE_SQLITE_CONNECTION, MktSQLiteConnectionPrivate))

G_DEFINE_TYPE(MktSQLiteConnection, mkt_sqlite_connection, MKT_TYPE_CONNECTION);

static guint64      mkt_sqlite_connection_send_no_select(MktConnection *connection, const gchar *stmt);
static GSList *     mkt_sqlite_connection_send(MktConnection *connection, GType otype, const gchar *stmt);
static const gchar *mkt_sqlite_connection_sql_create_table(MktConnection *connection);
static gboolean     mkt_sqlite_connection_delete_table(MktConnection *cnc, const gchar *table);

static const gchar *mkt_sqlite_connection_get_provider(MktConnection *connection) { return "SQLite"; }
static const gchar *mkt_sqlite_connection_timestamp_now(MktConnection *connection) { return "DateTime('now')"; }

gboolean open_db(MktSQLiteConnection *connection) {
    if (connection->priv->db != NULL) return TRUE;
    const gchar *db_name   = mkt_connection_get_data_base(MKT_CONNECTION(connection));
    guint        rc        = mkt_sqlite3_open_v2(db_name, &connection->priv->db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK) {
        g_warning("ERROR opening SQLite DB '%s': %s\n", db_name, sqlite3_errmsg(connection->priv->db));
        sqlite3_close(connection->priv->db);
        connection->priv->db = NULL;
        return FALSE;
    } else {
        mkt_sqlite3_setup_wal(connection->priv->db,1000,connection);
        if (rc != SQLITE_OK) {
            g_warning("db %s setup war fail  - %s", db_name,sqlite3_errmsg(connection->priv->db));
            return FALSE;
        }
    }
    return TRUE;
}



// gboolean open_readonly(MktSQLiteConnection *connection) {
//     if (connection->priv->db) return TRUE;
//     const gchar *db_name = mkt_connection_get_data_base(MKT_CONNECTION(connection));
//     guint        rc      = sqlite3_open_v2(db_name, &connection->priv->db, SQLITE_OPEN_READONLY, NULL);
//     if (rc != SQLITE_OK) {
//         g_critical("ERROR opening SQLite DB '%s': %s\n", db_name, sqlite3_errmsg(connection->priv->db));
//         sqlite3_close(connection->priv->db);
//         connection->priv->db = NULL;
//         return FALSE;
//     }
//     rc = sqlite3_exec(connection->priv->db, "PRAGMA journal_mode=MEMORY; PRAGMA synchronous=OFF;", NULL, NULL, NULL);
//     if (rc != SQLITE_OK) {
//         g_critical("db set juornal_mod=MEMORY and synchronous=OFF - %s", sqlite3_errmsg(connection->priv->db));
//     }
//     return TRUE;
// }

void close_db(MktSQLiteConnection *connection) {
    if (connection->priv->after_close && connection->priv->db) {
        sqlite3_close(connection->priv->db);
        connection->priv->db = NULL;
    }
}

static void mkt_sqlite_connection_init(MktSQLiteConnection *object) {
    object->priv              = G_TYPE_INSTANCE_GET_PRIVATE(object, MKT_TYPE_SQLITE_CONNECTION, MktSQLiteConnectionPrivate);
    object->priv->db          = NULL;
    object->priv->after_close = FALSE;
}

static void mkt_sqlite_connection_finalize(GObject *object) {
    MktSQLiteConnection *connection = MKT_SQLITE_CONNECTION(object);
    if (connection->priv->db) {
        sqlite3_close(connection->priv->db);
    }
    G_OBJECT_CLASS(mkt_sqlite_connection_parent_class)->finalize(object);
}

static void mkt_sqlite_connection_constructed(GObject *object) {
    MktSQLiteConnection *connection = MKT_SQLITE_CONNECTION(object);
    open_db(connection);
    G_OBJECT_CLASS(mkt_sqlite_connection_parent_class)->constructed(object);
}

static void mkt_sqlite_connection_class_init(MktSQLiteConnectionClass *klass) {
    GObjectClass *      object_class = G_OBJECT_CLASS(klass);
    MktConnectionClass *parent_class = MKT_CONNECTION_CLASS(klass);
    g_type_class_add_private(klass, sizeof(MktSQLiteConnectionPrivate));
    object_class->constructed    = mkt_sqlite_connection_constructed;
    object_class->finalize       = mkt_sqlite_connection_finalize;
    parent_class->send           = mkt_sqlite_connection_send;
    parent_class->send_no_select = mkt_sqlite_connection_send_no_select;
    parent_class->create_table   = mkt_sqlite_connection_sql_create_table;
    parent_class->delete_table   = mkt_sqlite_connection_delete_table;
    parent_class->provider       = mkt_sqlite_connection_get_provider;
    parent_class->timestamp_now  = mkt_sqlite_connection_timestamp_now;
}

static const gchar *mkt_sqlite_connection_sql_create_table(MktConnection *connection) { return "CREATE TABLE IF NOT EXISTS"; }

// static GType mkt_sqlite_connection_get_g_type(guint MySQLType) {
//     // TEST:g_debug("MySQL type %d",MySQLType);
//     switch (MySQLType) {
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

static gchar *sqlite_gtype_convert(GType type, guint flag) {
    GString *type_string = g_string_new("");
    if (!(flag & MKT_MODEL_PK_PROP)) {
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
            case G_TYPE_OBJECT:
                type_string = g_string_append(type_string, "INT UNSIGNED");
                break;
            case G_TYPE_STRING:
                if (flag & MKT_MODEL_DB_TIMESTAMP) {
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
    }
    if (flag & MKT_MODEL_PK_PROP) type_string = g_string_append(type_string, "INTEGER PRIMARY KEY AUTOINCREMENT");
    if (flag & MKT_MODEL_DB_NO_NULL) type_string = g_string_append(type_string, " NOT NULL");
    gchar *ret = g_strdup(type_string->str);
    g_string_free(type_string, TRUE);
    return ret;
}

typedef struct {
    GSList * last;
    GSList * start;
    GType    type;
    sqlite3 *cnc;

} SQLiteCallbackObjects;

static int load_models_callback(void *data, int argc, char **argv, char **azColName) {
    SQLiteCallbackObjects *cbo = (SQLiteCallbackObjects *)data;
    if (cbo == NULL) return -1;
    if (argc <= 0) return -1;
    GParameter *param = calloc(sizeof(GParameter), argc);
    gint        npar  = 0;
    int         i;
    for (i = 0; i < argc; i++) {
        GParamSpec *pspec = mkt_model_find_pspec(cbo->type, azColName[i]);
        if (pspec) {
            GValue *value = mkt_value_new(pspec->value_type);
            if (g_type_is_a(pspec->value_type, MKT_TYPE_MODEL)) {
                SQLiteCallbackObjects *obj = g_malloc(sizeof(SQLiteCallbackObjects));
                obj->type                  = pspec->value_type;
                obj->last                  = NULL;
                obj->start                 = NULL;
                obj->cnc                   = cbo->cnc;
                gchar *smtm                = g_strdup_printf("select * from %s where ref_id = %s", g_type_name(pspec->value_type), argv[i]);
                sqlite3_exec(obj->cnc, smtm, load_models_callback, (void *)obj, NULL);
                if (obj->start && obj->start->data) {
                    g_value_set_object(value, g_object_ref(obj->start->data));
                    param[npar].name = azColName[i];
                    g_value_init(&param[npar].value, value->g_type);
                    g_value_copy(value, &param[npar].value);
                    npar++;
                }
                mkt_slist_free_full(obj->start, g_object_unref);

                g_free(obj);
            } else {
                if (!mkt_set_gvalue_from_string(value, argv[i])) {
                    mkt_value_free(value);
                    value = NULL;
                    g_warning("Database value %s:%s transform failed ", azColName[i], argv[i] ? argv[i] : "NULL");
                } else {
                    param[npar].name = azColName[i];
                    g_value_init(&param[npar].value, value->g_type);
                    g_value_copy(value, &param[npar].value);
                    npar++;
                }
            }
            mkt_value_free(value);
        }
    }
    if (param && npar > 0) {
        MktModel *model = g_object_newv(cbo->type, npar, param);
        int       i     = 0;
        for (i = 0; i < npar; i++) {
            if (param[i].value.g_type > G_TYPE_NONE) g_value_unset(&param[i].value);
        }
        if (model) {
            mkt_model_thaw_notify(model);
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

gboolean mkt_sqlite_connection_delete_table(MktConnection *cnc, const gchar *table_name) {
    g_return_val_if_fail(cnc != NULL, FALSE);
    g_return_val_if_fail(MKT_IS_SQLITE_CONNECTION(cnc), FALSE);
    g_return_val_if_fail(table_name != NULL, FALSE);
    MktSQLiteConnection *connection = MKT_SQLITE_CONNECTION(cnc);
    open_db(connection);
    g_return_val_if_fail(connection->priv->db != NULL, FALSE);
    gboolean ret = TRUE;
    char *   buf = g_strdup_printf("DROP TABLE IF EXISTS %s", table_name);
    guint    rc  =mkt_sqlite3_exec(connection->priv->db, buf, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        g_warning("SQLite-connection: delete table %s error - %s", table_name, sqlite3_errmsg(connection->priv->db));
        ret = FALSE;
    }
    g_free(buf);
    close_db(connection);
    return ret;
}

guint64 mkt_sqlite_connection_send_no_select(MktConnection *cnc, const gchar *stmt) {
    g_return_val_if_fail(cnc != NULL, FALSE);
    g_return_val_if_fail(MKT_IS_SQLITE_CONNECTION(cnc), FALSE);
    g_return_val_if_fail(stmt != NULL, FALSE);
    MktSQLiteConnection *connection = MKT_SQLITE_CONNECTION(cnc);
    open_db(connection);
    g_return_val_if_fail(connection->priv->db != NULL, FALSE);
    guint64 ret = 0;
    guint   rc  = sqlite3_exec(connection->priv->db, "BEGIN;", NULL, NULL, NULL);
    rc          = sqlite3_exec(connection->priv->db, stmt, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_exec(connection->priv->db, "ROLLBACK;", NULL, NULL, NULL);
        // g_warning("db error: %s STMT=%s", sqlite3_errmsg(connection->priv->db), stmt);
    } else {
        sqlite3_exec(connection->priv->db, "COMMIT;", NULL, NULL, NULL);
        ret = sqlite3_last_insert_rowid(connection->priv->db);
    }
    close_db(connection);
    return ret;
}

static void sqliteExecThread(GTask *task, gpointer source_object, gpointer task_data, GCancellable *cancellable) {
    MktSQLiteConnection *conn = MKT_SQLITE_CONNECTION(source_object);
    gchar *              smtm = (gchar *)g_task_get_task_data(task);
    if (smtm == NULL) {
        g_task_return_error(task, g_error_new(mkt_error_quark(), 15, "task data (object type and sql string) should not be nil"));
        return;
    }
    if (g_task_return_error_if_cancelled(task)) return;
    open_db(conn);
    if (conn->priv->db == NULL) {
        g_task_return_error(task, g_error_new(mkt_error_quark(), 15, "task data (object type and sql string) should not be nil"));
        return;
    }
    guint rc = sqlite3_exec(conn->priv->db, "BEGIN;", NULL, NULL, NULL);
    rc       = sqlite3_exec(conn->priv->db, smtm, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_exec(conn->priv->db, "ROLLBACK;", NULL, NULL, NULL);
    } else {
        sqlite3_exec(conn->priv->db, "COMMIT;", NULL, NULL, NULL);
    }
    close_db(conn);
    g_task_return_boolean(task, TRUE);
    return;
}

void mkt_sqlite_connection_update_peoperty(MktConnection *connection, const gchar *stmt) {
    g_return_if_fail(connection != NULL);
    g_return_if_fail(MKT_IS_CONNECTION(connection));
    const gchar *        db_path = mkt_connection_get_data_base(MKT_CONNECTION(connection));
    MktSQLiteConnection *cnc     = MKT_SQLITE_CONNECTION(g_object_new(MKT_TYPE_SQLITE_CONNECTION, "database", db_path, "flag", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL));
    GTask *              task    = g_task_new(cnc, NULL, NULL, NULL);
    g_task_set_task_data(task, g_strdup(stmt), g_free);
    cnc->priv->after_close = TRUE;
    g_task_run_in_thread(task, sqliteExecThread);
    g_object_unref(cnc);
    g_object_unref(task);
}

GSList *mkt_sqlite_connection_send(MktConnection *cnc, GType otype, const gchar *smtm) {
    g_return_val_if_fail(cnc != NULL, NULL);
    g_return_val_if_fail(smtm != NULL, NULL);
    g_return_val_if_fail(MKT_IS_SQLITE_CONNECTION(cnc), NULL);
    MktSQLiteConnection *connection = MKT_SQLITE_CONNECTION(cnc);
    open_db(connection);
    g_return_val_if_fail(connection->priv->db != NULL, NULL);
    GSList *               ret = NULL;
    SQLiteCallbackObjects *obj = g_malloc(sizeof(SQLiteCallbackObjects));
    obj->type                  = otype;
    obj->last                  = NULL;
    obj->start                 = NULL;
    obj->cnc                   = connection->priv->db;
    sqlite3_exec(connection->priv->db, smtm, load_models_callback, (void *)obj, NULL);
    ret = obj->start;
    g_free(obj);
    close_db(connection);
    return ret;
}

void mkt_sqlite_connection_update_table(MktSQLiteConnection *connection, GType object_type) {
    g_return_if_fail(connection != NULL);
    g_return_if_fail(g_type_is_a(object_type, MKT_TYPE_MODEL));
    open_db(connection);
    g_return_if_fail(connection->priv->db != NULL);
    guint    rc     = sqlite3_exec(connection->priv->db, "BEGIN;", NULL, NULL, NULL);
    GString *create = g_string_new("CREATE TABLE IF NOT EXISTS ");
    guint    ni     = 0;
    g_string_append_printf(create, "%s (", g_type_name(object_type));
    GType *iface    = g_type_interfaces(object_type, &ni);
    int    i        = 0;
    guint  ccounter = 0;
    for (i = 0; i < ni; i++) {
        guint        npar   = 0;
        gpointer     itable = g_type_default_interface_ref(iface[i]);
        GParamSpec **pspec  = g_object_interface_list_properties(itable, &npar);
        if (pspec != NULL) {
            int j = 0;

            for (j = 0; j < npar; j++) {
                if (pspec[j]->flags & MKT_MODEL_DB_PROP) {
                    gchar *name          = g_strdup(mkt_sql_parser_rps(pspec[j]->name));
                    gchar *value_type    = sqlite_gtype_convert(pspec[j]->value_type, pspec[j]->flags);
                    gchar *default_value = mkt_value_stringify(g_param_spec_get_default_value(pspec[j]));
                    if (pspec[j]->value_type == G_TYPE_STRING)
                        g_string_append_printf(create, "%s'%s' %s DEFAULT '%s'", ccounter > 0 ? ", " : "", name, value_type, default_value);
                    else
                        g_string_append_printf(create, "%s'%s' %s DEFAULT %s", ccounter > 0 ? ", " : "", name, value_type, default_value);
                    ccounter = ccounter + 1;
                    g_free(name);
                    g_free(default_value);
                    g_free(value_type);
                }
            }
            g_free(pspec);
        }
        if (itable) g_type_default_interface_unref(itable);
    }
    g_string_append_printf(create, ");");
    g_free(iface);
    sqlite3_stmt *result = NULL;

    if (rc == SQLITE_OK) {
        rc = sqlite3_prepare_v2(connection->priv->db, create->str, create->len, &result, NULL);
        if (rc == SQLITE_OK && result != NULL) {
            sqlite3_step(result);
            sqlite3_finalize(result);
        }
    }

    if (rc != SQLITE_OK)
        sqlite3_exec(connection->priv->db, "ROLLBACK;", NULL, NULL, NULL);
    else
        sqlite3_exec(connection->priv->db, "COMMIT;", NULL, NULL, NULL);
    g_string_free(create, TRUE);
    close_db(connection);
}

static void connection_fix(MktSQLiteConnection *conn) {
    close_db(conn);
    remove(mkt_connection_get_data_base(MKT_CONNECTION(conn)));
    open_db(conn);
}

void mkt_sqlite_connection_check(MktSQLiteConnection *conn) {
    g_return_if_fail(conn);
    g_return_if_fail(MKT_SQLITE_CONNECTION(conn));
    conn->priv->after_close = TRUE;
    open_db(conn);
    if (conn->priv->db == NULL) connection_fix(conn);
    g_return_if_fail(conn->priv->db);
    // g_debug("Checkpoint: %s", mkt_connection_get_data_base(MKT_CONNECTION(conn)));
    // sqlite3_exec(conn->priv->db, "BEGIN;", NULL, NULL, NULL);
    // sqlite3_wal_checkpoint(conn->priv->db, NULL);
    // guint rc = sqlite3_wal_checkpoint_v2(conn->priv->db,NULL,SQLITE_CHECKPOINT_RESTART,NULL,NULL);
    // sqlite3_exec(conn->priv->db, "COMMIT;", NULL, NULL, NULL);
    // g_debug("RESULT:%d", rc);
    close_db(conn);
}

void mkt_sqlite_connection_close_after_use(MktSQLiteConnection *conn) {
    g_return_if_fail(conn);
    g_return_if_fail(MKT_SQLITE_CONNECTION(conn));
    conn->priv->after_close = TRUE;
    close_db(conn);
}