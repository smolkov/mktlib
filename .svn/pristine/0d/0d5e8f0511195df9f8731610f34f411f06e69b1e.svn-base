/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * lnosql.c
 * Copyright (C) LAR 2017
 *

 */

#include "lnosql.h"
#include "value.h"
#include <glib.h>
#include <glib/gprintf.h>
#include <stdlib.h>
#include <string.h>
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>
#include <locale.h>
#include <ejdb/ejdb.h>




static GHashTable *noqlDBs = NULL;


EJDB * findDataBase(const gchar *path ){
    if(noqlDBs == NULL){
        noqlDBs = g_hash_table_new(g_str_hash,g_str_equal);
    }
    gpointer p = g_hash_table_lookup(noqlDBs,path);
    if (p == NULL ){
        EJDB *jb = ejdbnew();
        if (!ejdbopen(jb, path, JBOWRITER | JBOCREAT | JBOTRUNC)) {
            return NULL;
        }
        g_hash_table_insert(nosqlDBs,path,jb);
        p = jb;
    }
    return (EJDB*)p;
}




enum { PROP_0, NOSQL_COLLECTION };


struct _NosqlCollectionPrivate {
    nosql3 *mySQL;
    gchar *  table;

    gchar *dbpath;
    guint  flag;

    gboolean isInit;
    GObject *model;
    GMutex   m;
};

static void lock(NosqlCollection *connection) { g_mutex_lock(&connection->priv->m); }
static void unlock(NosqlCollection *connection) { g_mutex_unlock(&connection->priv->m); }

#define NOSQL_CONNECTION_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_NOSQL_CONNECTION, NosqlCollectionPrivate))

G_DEFINE_TYPE(NosqlCollection, nosql_connection, G_TYPE_OBJECT);

static void nosql_connection_init(NosqlCollection *object) {
    object->priv         = G_TYPE_INSTANCE_GET_PRIVATE(object, TYPE_NOSQL_CONNECTION, NosqlCollectionPrivate);
    object->priv->mySQL  = NULL;
    object->priv->flag   = NOSQL_OPEN_READWRITE | NOSQL_OPEN_CREATE;
    object->priv->dbpath = g_build_filename(g_get_home_dir(), ".lar", "database.nosql", NULL);
}

static void nosql_connection_finalize(GObject *object) {
    /* TODO: Add deinitalization code here */
    // g_debug("Nosql connection finalize");
    NosqlCollection *connection = NOSQL_CONNECTION(object);
    if (connection->priv->mySQL) nosql3_close(connection->priv->mySQL);
    connection->priv->mySQL = NULL;
    if (connection->priv->dbpath) g_free(connection->priv->dbpath);
    G_OBJECT_CLASS(nosql_connection_parent_class)->finalize(object);
}

void nosql_connection_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    NosqlCollection *connection = NOSQL_CONNECTION(object);
    switch (prop_id) {
    case NOSQL_FLAG:
        connection->priv->flag = g_value_get_uint(value);
        break;
    case NOSQL_DATABASE:
        if (connection->priv->dbpath) g_free(connection->priv->dbpath);
        connection->priv->dbpath = g_value_dup_string(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

void nosql_connection_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    NosqlCollection *connection = NOSQL_CONNECTION(object);
    switch (prop_id) {
    case NOSQL_FLAG:
        g_value_set_uint(value, connection->priv->flag);
        break;
    case NOSQL_DATABASE:
        g_value_set_string(value, connection->priv->dbpath);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void nosql_connection_class_init(NosqlCollectionClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // CollectionClass *parent_class = CONNECTION_CLASS(klass);
    g_type_class_add_private(klass, sizeof(NosqlCollectionPrivate));
    object_class->finalize     = nosql_connection_finalize;
    object_class->set_property = nosql_connection_set_property;
    object_class->get_property = nosql_connection_get_property;
    g_object_class_install_property(object_class, NOSQL_FLAG,
        g_param_spec_uint("flag", "connection construct prop", "Set connection's port", 0, G_MAXUINT16, NOSQL_OPEN_READWRITE | NOSQL_OPEN_CREATE, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE));
    g_object_class_install_property(
        object_class, NOSQL_DATABASE, g_param_spec_string("database", "connection construct prop", "Set connection's database", "nosql", G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE));
}

static int NosqlCollectionBusyHandler(void *data, int retry) {
    if (retry < MAX_BUSY_RETRY) {
        nosql3_sleep(100);
        return 1;
    }
    return 0;
}

gboolean NosqlCollectionInit(NosqlCollection *connection) {
    g_return_val_if_fail(connection != NULL, FALSE);
    g_return_val_if_fail(IS_NOSQL_CONNECTION(connection), FALSE);
    if (connection->priv->mySQL) return TRUE;
    lock(connection);
    gboolean ret             = TRUE;
    connection->priv->isInit = FALSE;
    if (0 != nosql3_open_v2(connection->priv->dbpath, &connection->priv->mySQL, connection->priv->flag, NULL)) {
        g_critical("Nosql-connection: connect to data base %s failed - %s", connection->priv->dbpath, nosql3_errmsg(connection->priv->mySQL));
        // nosql3_close(connection->priv->mySQL);
        connection->priv->mySQL = NULL;
        ret                     = FALSE;
    } else {
        // Create Logbook table ...
        nosql3_exec(connection->priv->mySQL, "PRAGMA journal_mode=WAL", NULL, NULL, NULL);
        nosql3_exec(connection->priv->mySQL, "PRAGMA synchronous=OFF", NULL, NULL, NULL);
        nosql3_busy_handler(connection->priv->mySQL, NosqlCollectionBusyHandler, connection);
        connection->priv->isInit = TRUE;
    }
    unlock(connection);
    return ret;
}

gboolean NosqlCollectionOpen(NosqlCollection *connection) {
    lock(connection);

    if (connection->priv->mySQL) {
        if (connection->priv->mySQL) nosql3_close(connection->priv->mySQL);
        connection->priv->mySQL = NULL;
    }
    gboolean ret = FALSE;
    gint     rc  = nosql3_open_v2(connection->priv->dbpath, &connection->priv->mySQL, connection->priv->flag, NULL);
    if (NOSQL_OK != rc) {
        g_critical("Nosql-connection: connect to data base %s failed - %s", connection->priv->dbpath, nosql3_errmsg(connection->priv->mySQL));
        // nosql3_close(connection->priv->mySQL);
        connection->priv->mySQL = NULL;
        unlock(connection);

    } else {
        if (!connection->priv->isInit) {
            nosql3_exec(connection->priv->mySQL, "PRAGMA journal_mode=WAL", NULL, NULL, NULL);
            nosql3_exec(connection->priv->mySQL, "PRAGMA synchronous=OFF", NULL, NULL, NULL);
        }
        nosql3_busy_handler(connection->priv->mySQL, NosqlCollectionBusyHandler, connection);
        ret = TRUE;
    }
    unlock(connection);
    return ret;
}

void NosqlCollectionClose(NosqlCollection *connection) {
    lock(connection);
    if (connection->priv->mySQL) {
        if (connection->priv->mySQL) nosql3_close(connection->priv->mySQL);
        connection->priv->mySQL = NULL;
    }
    unlock(connection);
}

typedef struct {
    GSList *          last;
    GSList *          start;
    NosqlCollection *connection;
    GObject *         model;

} sqLiteCallbackObjects;

static int nosqlCollectionLoadModelsCallback(void *data, int argc, char **argv, char **azColName) {
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
            GValue *value = getPspecValue(pspec,argv[i]);
            if(value){
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
            GSList *nl                             = g_slice_new(GSList);
            nl->data                               = model;
            nl->next                               = NULL;
            if (cbo->last != NULL) cbo->last->next = nl;
            cbo->last                              = nl;
            if (cbo->start == NULL) cbo->start     = cbo->last;
        }
    }
    if (param) g_free(param);
    return 0;
}

gboolean NosqlCollectionDeleteTable(NosqlCollection *connection, const gchar *table_name) {
    g_return_val_if_fail(connection != NULL, FALSE);
    g_return_val_if_fail(IS_NOSQL_CONNECTION(connection), FALSE);
    g_return_val_if_fail(table_name != NULL, FALSE);
    if (!NosqlCollectionOpen(connection)) {
        return FALSE;
    }
    lock(connection);
    gboolean ret = TRUE;
    char *   buf = g_strdup_printf("DROP TABLE IF EXISTS %s", table_name);
    if (nosql3_exec(NOSQL_CONNECTION(connection)->priv->mySQL, buf, NULL, NULL, NULL)) {
        g_critical("Nosql-connection: delete table %s error - %s", table_name, nosql3_errmsg(NOSQL_CONNECTION(connection)->priv->mySQL));
        ret = FALSE;
    }
    g_free(buf);
    unlock(connection);
    NosqlCollectionClose(connection);
    return ret;
}

guint64 NosqlInsertObject(NosqlCollection *connection, const gchar *table, GObject *object) {
    g_return_val_if_fail(connection != NULL, 0);
    g_return_val_if_fail(IS_NOSQL_CONNECTION(connection), 0);

    if (!NosqlCollectionOpen(connection)) {
        return 0;
    }
    lock(connection);
    guint rc = nosql3_exec(connection->priv->mySQL, "BEGIN TRANSACTION", NULL, NULL, NULL);
    if (rc == NOSQL_OK) {
        gchar *createTable = CreateTableSQL(object, table);
        // g_print("create table :%s", createTable);
        rc = nosql3_exec(connection->priv->mySQL, createTable, NULL, NULL, NULL);
        if (createTable) g_free(createTable);
        if (rc == NOSQL_OK) {
            gchar *insertSQL = InsertSQLString(object, table);
            // g_print("create table :%s", insertSQL);
            rc = nosql3_exec(connection->priv->mySQL, insertSQL, NULL, NULL, NULL);
            if (insertSQL) g_free(insertSQL);
        }
    }
    guint64 ret = 0;
    if (rc == NOSQL_OK) {
        rc  = nosql3_exec(connection->priv->mySQL, "END TRANSACTION", NULL, NULL, NULL);
        ret = (guint64)nosql3_last_insert_rowid(connection->priv->mySQL);
    } else {
        rc = nosql3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, NULL, NULL);
    }
    unlock(connection);
    NosqlCollectionClose(connection);
    return ret;
}

guint64 NosqlCollectionSendNoSelect(NosqlCollection *connection, const gchar *stmt) {
    g_return_val_if_fail(connection != NULL, 0);
    g_return_val_if_fail(IS_NOSQL_CONNECTION(connection), 0);
    if (!NosqlCollectionOpen(connection)) {
        return FALSE;
    }
    lock(connection);
    nosql3_stmt *result = NULL;
    guint64       ret    = 0;
    guint         rc     = nosql3_exec(connection->priv->mySQL, "BEGIN TRANSACTION", NULL, NULL, NULL);
    if ((rc == NOSQL_OK)) {
        rc = nosql3_prepare_v2(connection->priv->mySQL, stmt, strlen(stmt) + 1, &result, NULL);
        if (rc != NOSQL_OK) {
            g_critical("db error: %s STMT=%s", nosql3_errmsg(connection->priv->mySQL), stmt);
            // exit(1);
        }
        if (result) {
            nosql3_step(result);
            nosql3_finalize(result);
        }
    }
    if (rc == NOSQL_OK) {
        rc  = nosql3_exec(connection->priv->mySQL, "COMMIT TRANSACTION", NULL, 0, NULL);
        ret = (guint64)nosql3_last_insert_rowid(connection->priv->mySQL);
    } else
        rc = nosql3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, 0, NULL);
    unlock(connection);
    NosqlCollectionClose(connection);
    return ret;
}

guint64 NosqlCollectionLastRowId(NosqlCollection *connection) {
    g_return_val_if_fail(connection != NULL, 0);
    g_return_val_if_fail(IS_NOSQL_CONNECTION(connection), 0);
    if (!NosqlCollectionOpen(connection)) {
        return 0;
    }
    lock(connection);

    guint64 last_id = (guint64)nosql3_last_insert_rowid(connection->priv->mySQL);
    unlock(connection);
    NosqlCollectionClose(connection);
    return last_id;
}

static GSList *nosqlCollectionSend(NosqlCollection *connection, GType otype, const gchar *smtm) {
    g_return_val_if_fail(smtm != NULL, NULL);
    g_return_val_if_fail(connection != NULL, NULL);
    g_return_val_if_fail(IS_NOSQL_CONNECTION(connection), NULL);
    g_return_val_if_fail(g_type_is_a(otype, G_TYPE_OBJECT), NULL);
    if (!NosqlCollectionOpen(connection)) {
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
    guint rc                   = nosql3_exec(connection->priv->mySQL, "BEGIN TRANSACTION", NULL, 0, NULL);
    if (rc == NOSQL_OK) {
        rc = nosql3_exec(connection->priv->mySQL, smtm, nosqlCollectionLoadModelsCallback, (void *)obj, NULL);
    }
    if (rc == NOSQL_OK)
        rc = nosql3_exec(connection->priv->mySQL, "END TRANSACTION", NULL, 0, NULL);
    else
        rc = nosql3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, 0, NULL);

    ret = obj->start;
    g_object_unref(obj->model);
    g_free(obj);
    if (rc != NOSQL_OK) {
        // g_warning("SEND_ERROR: %s. ERRCODE: %d.\n",
        //	nosql3_errmsg(connection->priv->mySQL), rc);
    }
    unlock(connection);
    NosqlCollectionClose(connection);
    return ret;
}

/* MySQL connection create tools
 *
 */

NosqlCollection *NosqlCollectionNew(const gchar *database, guint flag) {
    NosqlCollection *connection = NOSQL_CONNECTION(g_object_new(TYPE_NOSQL_CONNECTION, "database", database, "flag", flag, NULL));
    return connection;
}

void NosqlUpdateTable(NosqlCollection *connection, GType object_type, const gchar *table) {
    g_return_if_fail(connection != NULL);
    g_return_if_fail(IS_NOSQL_CONNECTION(connection));
    g_return_if_fail(g_type_is_a(object_type, G_TYPE_OBJECT));
    guint ni = 0;
    if (!NosqlCollectionOpen(connection)) {
        return;
    }
    lock(connection);
    nosql3_stmt *result    = NULL;
    gchar *       SQL       = g_strdup_printf("SELECT sql FROM nosql_master where name='%s'", table);
    gint          rc        = nosql3_prepare(connection->priv->mySQL, SQL, -1, &result, NULL);
    gchar *       old_table = NULL;
    if (rc == NOSQL_OK || rc == NOSQL_DONE) {
        int s;
        s = nosql3_step(result);
        if (s == NOSQL_ROW) {
            int nCols = nosql3_column_count(result);
            if (nCols) {
                int nCol;
                for (nCol = 0; nCol < nCols; nCol++) {

                    old_table = g_strdup((gchar *)nosql3_column_text(result, nCol));
                    if (NULL != strstr((const char *)old_table, "(ref_id") || NULL != strstr((const char *)old_table, "('ref_id'")) {
                        break;
                    } else {
                        g_free(old_table);
                        old_table = NULL;
                    }
                }
            }
        }
        if (result) nosql3_finalize(result);
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
                gchar *value_type    = NosqlConvertGType(pspec[j]->value_type, pspec[j]->flags);
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
        if (rc == NOSQL_OK) {
            guint rc = nosql3_prepare_v2(connection->priv->mySQL, check->str, check->len, &result, NULL);
            if (rc == NOSQL_OK) {
                nosql3_step(result);
                nosql3_finalize(result);
                nosql3_exec(connection->priv->mySQL, "COMMIT;", NULL, NULL, NULL);
            } else {
                nosql3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, 0, NULL);
            }
        } else {
            nosql3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, 0, NULL);
        }
    } else if (0 != g_strcmp0(check->str, old_table)) {

        rc = nosql3_exec(connection->priv->mySQL, "BEGIN TRANSACTION;", NULL, 0, NULL);
        if (rc == NOSQL_OK) {

            guint rc = nosql3_prepare_v2(connection->priv->mySQL, create->str, create->len, &result, NULL);
            if (rc == NOSQL_OK) {
                nosql3_step(result);
                nosql3_finalize(result);
                rc = nosql3_prepare_v2(connection->priv->mySQL, insert->str, insert->len, &result, NULL);
                if (rc == NOSQL_OK) {
                    nosql3_step(result);
                    nosql3_finalize(result);
                    rc = nosql3_prepare_v2(connection->priv->mySQL, drop->str, drop->len, &result, NULL);
                    if (rc == NOSQL_OK) {
                        nosql3_step(result);
                        nosql3_finalize(result);
                        rc = nosql3_prepare_v2(connection->priv->mySQL, after->str, after->len, &result, NULL);
                        if (rc == NOSQL_OK) {
                            nosql3_step(result);
                            nosql3_finalize(result);
                            nosql3_exec(connection->priv->mySQL, "COMMIT;", NULL, NULL, NULL);
                        } else {
                            // g_print ( "db error 5: %s\n", nosql3_errmsg(connection->priv->mySQL));
                            nosql3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, 0, NULL);
                        }
                    } else {
                        // g_print ( "db error 4: %s\n", nosql3_errmsg(connection->priv->mySQL));
                        nosql3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, 0, NULL);
                    }
                } else {
                    // g_print ( "db error 3: %s\n", nosql3_errmsg(connection->priv->mySQL));
                    nosql3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, 0, NULL);
                }
            } else {
                // g_print ( "db error 2: %s\n", nosql3_errmsg(connection->priv->mySQL));
                nosql3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, 0, NULL);
            }
        } else {
            // g_print ( "db error 1: %s\n", nosql3_errmsg(connection->priv->mySQL));
            nosql3_exec(connection->priv->mySQL, "ROLLBACK TRANSACTION", NULL, 0, NULL);
        }
    }
    g_string_free(create, TRUE);
    g_string_free(check, TRUE);
    g_string_free(insert, TRUE);
    g_string_free(select, TRUE);
    g_string_free(drop, TRUE);
    g_string_free(after, TRUE);
    unlock(connection);
    NosqlCollectionClose(connection);
}



GSList *NosqlSelectObjects(NosqlCollection *connection,GType type, const gchar *format, ...){
    g_return_val_if_fail(connection!=NULL,NULL);
    g_return_val_if_fail(IS_NOSQL_CONNECTION(connection),NULL);
    gchar * old_locale = g_strdup(setlocale(LC_ALL, NULL));
    va_list args;
    setlocale(LC_ALL, "en_US.UTF-8");
    gchar * SQL;
    va_start(args, format);
    SQL = g_strdup_vprintf(format, args);
    va_end(args);
    GSList *res = nosqlCollectionSend(connection,type,SQL);
    setlocale(LC_ALL, old_locale);
    g_free(old_locale);
    return res;
}