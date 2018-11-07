/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup DbModule
 * @{
 * @file  db-model.c	Application object file.
 * @brief This is model object file description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */

#include "db-model.h"

#include <gio/gio.h>
#include <glib/gmacros.h>
#include <glib/gstdio.h>
#include <gobject/gvaluecollector.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <locale.h>

#include "../config.h"
#include <glib/gi18n-lib.h>

// Test change c file ..

#if GLIB_CHECK_VERSION(2, 31, 7)
static GRecMutex model_init_mutex;
#define MODEL_INIT_LOCK() g_rec_mutex_lock(&model_init_mutex)
#define MODEL_INIT_UNLOCK() g_rec_mutex_unlock(&model_init_mutex)
#else
static GStaticRecMutex model_init_mutex = G_STATIC_REC_MUTEX_INIT;
#define MODEL_INIT_LOCK() g_static_rec_mutex_lock(&model_init_mutex)
#define MODEL_INIT_UNLOCK() g_static_rec_mutex_unlock(&model_init_mutex)
#endif

#if GLIB_CHECK_VERSION(2, 31, 7)
static GRecMutex signal_rmutex;
#define SIGNAL_LOCK() g_rec_mutex_lock(&signal_rmutex)
#define SIGNAL_UNLOCK() g_rec_mutex_unlock(&signal_rmutex)
#else
static GStaticRecMutex signal_rmutex = G_STATIC_REC_MUTEX_INIT;
#define SIGNAL_LOCK() g_static_rec_mutex_lock(&signal_rmutex)
#define SIGNAL_UNLOCK() g_static_rec_mutex_unlock(&signal_rmutex)
#endif

static void db_imodel_base_init(gpointer g_iface) {
    static gboolean is_initialized = FALSE;
    MODEL_INIT_LOCK();
    if (!is_initialized) {
        GParamSpec *pspec;
        pspec = g_param_spec_uint64("ref-id", "Data Base model id", "Set model data base id ", 0, G_MAXUINT64, 0,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | DB_MODEL_DB_PROP | DB_MODEL_PK_PROP | DB_MODEL_DB_AUTOINCREMENT);
        g_object_interface_install_property(g_iface, pspec);

        pspec = g_param_spec_string("database", "SQlite Data base patch", "SQlite Data base patch", NULL, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
        g_object_interface_install_property(g_iface, pspec);
        is_initialized = TRUE;
    }
    MODEL_INIT_UNLOCK();
}

GType db_imodel_get_type(void) {
    static GType iface_type = 0;
    if (iface_type == 0) {
        static const GTypeInfo info = {sizeof(DbIModelInterface), (GBaseInitFunc)db_imodel_base_init, (GBaseFinalizeFunc)NULL, (GClassInitFunc)NULL, NULL, NULL,
            0, 0, (GInstanceInitFunc)NULL, 0};
        MODEL_INIT_LOCK();
        if (iface_type == 0) {
            iface_type = g_type_register_static(G_TYPE_INTERFACE, "DbIModelInterface", &info, 0);
        }
        MODEL_INIT_UNLOCK();
    }
    return iface_type;
}

guint64 db_model_ref_id(DbIModel *imodel) {
    g_return_val_if_fail(imodel != NULL, 0);
    g_return_val_if_fail(DB_IS_IMODEL(imodel), 0);
    if (DB_IMODEL_GET_INTERFACE(imodel)->ref_id) return DB_IMODEL_GET_INTERFACE(imodel)->ref_id(imodel);
    return 0;
}

enum { MODEL_BUILDER_CHECK, MODEL_BUILDER_INSERT, MODEL_BUILDER_UPDATE, MODEL_BUILDER_SYNC, MODEL_BUILDER_LAST };

struct _DbModelPrivate {
    DbMutex *    mutex;
    guint        sync_tag;
    DbDataModel *sync_model;
    gboolean     freeze_save;
    guint64      ref_id;
    gboolean     constructed;
    gchar *      database;
};

/* signals */
enum { MODEL_CHANGED, MODEL_DISPOSE, LAST_SIGNAL };

static guint db_model_signals[LAST_SIGNAL];

#define DB_MODEL_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), DB_TYPE_MODEL, DbModelPrivate))

guint64 db_model_ref_id_real(DbIModel *imodel) {
    g_return_val_if_fail(imodel != NULL, 0);
    g_return_val_if_fail(DB_IS_MODEL(imodel), 0);
    return DB_MODEL(imodel)->priv->ref_id;
}

void db_model_init_interface(DbIModelInterface *iface) { iface->ref_id = db_model_ref_id_real; }

G_DEFINE_TYPE_WITH_CODE(DbModel, db_model, G_TYPE_OBJECT, G_IMPLEMENT_INTERFACE(DB_TYPE_IMODEL, db_model_init_interface))

enum { PROP_MODEL_NULL, PROP_MODEL_ID, PROP_MODEL_DATABASE };

static GHashTable *__data_model_collector__ = NULL;
static GHashTable *model_data_get_collector() {
    if (__data_model_collector__ == NULL) {
        __data_model_collector__ = g_hash_table_new(g_str_hash, g_str_equal);
    }
    return __data_model_collector__;
}
DbDataModel *db_model_lookup_collectors(const gchar *name) {
    g_return_val_if_fail(name != NULL, NULL);
    return DB_DATA_MODEL(g_hash_table_lookup(model_data_get_collector(), name));
}

static gboolean model_insert_collectors(DbDataModel *data) {
    if (NULL == db_model_lookup_collectors(db_data_model_get_type_name(data))) {
        g_hash_table_insert(model_data_get_collector(), (gpointer)db_data_model_get_type_name(data), (gpointer)data);
        return TRUE;
    }
    return FALSE;
}

DbDataModel *db_model_find_data_models_for_interfaces(GType otype) {
    DbDataModel *dmodel      = NULL;
    guint        ni          = 0;
    GType *      iface       = g_type_interfaces(otype, &ni);
    GList *      data_models = g_hash_table_get_values(model_data_get_collector());
    int          i           = 0;
    for (i = 0; i < ni; i++) {
        GList *l = NULL;
        for (l = data_models; l != NULL; l = l->next) {
            if (l->data && DB_IS_DATA_MODEL(l->data)) {
                if (!db_data_model_check_interface(DB_DATA_MODEL(l->data), iface[i])) {
                    data_models = g_list_delete_link(data_models, l);
                }
            }
        }
    }
    if (g_list_length(data_models) > 1) {
        g_warning("More data model fotr interface type %s find", g_type_name(otype));
    }
    if (data_models && data_models->data) {
        dmodel = DB_DATA_MODEL(data_models->data);
        g_list_free(data_models);
    }
    return dmodel;
}

GList *db_model_find_for_gtype(GType otype) {
    GList *ret         = NULL;
    guint  ni          = 0;
    GType *iface       = g_type_interfaces(otype, &ni);
    GList *data_models = g_hash_table_get_values(model_data_get_collector());
    int    i           = 0;
    for (i = 0; i < ni; i++) {
        GList *l = NULL;
        for (l = data_models; l != NULL; l = l->next) {
            if (l->data && DB_IS_DATA_MODEL(l->data)) {
                if (!db_data_model_check_interface(DB_DATA_MODEL(l->data), iface[i])) {
                    data_models = g_list_delete_link(data_models, l);
                }
            }
        }
    }
    if (data_models && data_models->data) {
        ret = g_list_copy(data_models);
        g_list_free(data_models);
    }
    return ret;
}

GList *db_model_find_for_interface(GType iface) {
    GList *        ret = NULL;
    GHashTableIter iter;
    g_hash_table_iter_init(&iter, model_data_get_collector());
    gpointer key, value;
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        if (value && DB_IS_DATA_MODEL(value)) {
            if (db_data_model_check_interface(DB_DATA_MODEL(value), iface)) {
                ret = g_list_append(ret, value);
            }
        }
    }
    return ret;
}

GList *db_model_list_datamodels(GType type) {
    GList *      data_models = NULL;
    DbDataModel *dm          = db_model_lookup_collectors(g_type_name(type));
    if (dm != NULL)
        data_models = g_list_append(data_models, dm);
    else
        data_models = db_model_find_for_interface(type);
    return data_models;
}

/*static void
model_remove_collectors ( DbDataModel *data )
{
        g_hash_table_remove(sensors_get_collector(),(gpointer)
db_data_model_get_type_name(data));
}*/

static void db_model_init(DbModel *db_model) {
    DbModelPrivate *priv = DB_MODEL_GET_PRIVATE(db_model);
    priv->mutex          = db_mutex_new();
    priv->freeze_save    = TRUE;
    priv->sync_tag       = 0;
    priv->sync_model     = NULL;
    priv->database       = NULL;
    priv->ref_id         = 0;
    priv->constructed    = FALSE;
    db_model->priv       = priv;
}

const GValue *db_model_get_property_value(DbModel *model, const gchar *prop) {
    static GValue *value = NULL;
    // MUTEX:g_mutex_lock (&atom->priv->lock);
    if (value != NULL) {
        db_value_free(value);
    }
    g_return_val_if_fail(model != NULL, (const GValue *)value);
    g_return_val_if_fail(prop != NULL, (const GValue *)value);
    g_return_val_if_fail(DB_IS_MODEL(model), (const GValue *)value);
    GParamSpec *pspec = g_object_class_find_property(G_OBJECT_GET_CLASS(G_OBJECT(model)), prop);
    if (pspec != NULL) {
        value = db_value_new(G_PARAM_SPEC_VALUE_TYPE(pspec));
        g_object_get_property(G_OBJECT(model), prop, value);
    }
    // MUTEX:g_mutex_unlock (&atom->priv->lock);
    return (const GValue *)value;
}

static void db_model_insert(DbModel *model) {
    if (model->priv->ref_id > 0) {
        g_warning("Model id(%" G_GUINT64_FORMAT ") not 0 initialized", model->priv->ref_id);
        return;
    }
    DbDataModel *data = model->priv->sync_model;
    if (db_data_model_get_insert(data) != NULL) {
        gchar *SQL = db_sql_parser_extended_default(db_data_model_get_insert(data), G_OBJECT(model));
        if (SQL) {
            model->priv->ref_id = db_connection_send_no_select(DB_CONNECTION(db_data_model_get_provider(data)), SQL);
            g_free(SQL);
        }
    }
}

static void db_model_replace_real(DbModel *model) {
    MODEL_INIT_LOCK();
    DbDataModel *data = model->priv->sync_model;
    if (db_data_model_get_replace(data) != NULL) {
        gchar *SQL = db_sql_parser_extended_default(db_data_model_get_replace(data), G_OBJECT(model));
        if (SQL) {
            db_connection_send_no_select(DB_CONNECTION(db_data_model_get_provider(data)), SQL);
            g_free(SQL);
        }
    }
    MODEL_INIT_UNLOCK();
}
static gboolean db_model_delete_real(DbModel *model) {
    MODEL_INIT_LOCK();
    DbDataModel *data = model->priv->sync_model;
    if (model->priv->ref_id > 0 && data) {
        // g_debug("delete id =%llu",model->priv->ref_id);
        gchar *delete_sql = g_strdup_printf("DELETE FROM %s WHERE ref_id = %" G_GUINT64_FORMAT ";", G_OBJECT_TYPE_NAME(G_OBJECT(model)), model->priv->ref_id);
        db_connection_send_no_select(DB_CONNECTION(db_data_model_get_provider(data)), delete_sql);
        model->priv->ref_id = 0;
        g_free(delete_sql);
    }
    MODEL_INIT_UNLOCK();
    return TRUE;
}

static void db_model_finalize(GObject *object) {
    /* TODO: Add deinitalization code here */
    // g_debug("model finalize");
    DbModel *model = DB_MODEL(object);
    db_mutex_free(model->priv->mutex);
    if (model->priv->database) g_free(model->priv->database);
    G_OBJECT_CLASS(db_model_parent_class)->finalize(object);
}

void db_model_set_data_model(DbModel *model, DbDataModel *datamodel) {
    g_return_if_fail(model != NULL);
    g_return_if_fail(datamodel != NULL);
    g_return_if_fail(DB_IS_MODEL(model));
    g_return_if_fail(DB_IS_DATA_MODEL(datamodel));
    if (model->priv->sync_model == NULL) {
        model->priv->sync_model = DB_DATA_MODEL(g_object_ref(datamodel));
    }
}

static void db_model_constructed(GObject *object) {
    DbModel *model           = DB_MODEL(object);
    model->priv->freeze_save = TRUE;
    GType        type        = G_OBJECT_TYPE(G_OBJECT(object));
    DbDataModel *data        = db_model_lookup_collectors(g_type_name(type));
    if (NULL != data) {
        db_model_set_data_model(model, data);
        model->priv->constructed = TRUE;
    }
    if (DB_MODEL_GET_CLASS(model)->initialized) DB_MODEL_GET_CLASS(model)->initialized(model);

    // g_debug("db_model_constructed");
}

static void db_model_dispose(GObject *object) {
    // g_debug("db_model_dispose");
    // DbModel *model = DB_MODEL (object);
    DbModel *model = DB_MODEL(object);
    // if(model->priv->sync_tag)g_source_remove (model->priv->sync_tag);
    // model->priv->sync_tag = 0;
    if (model->priv->sync_model != NULL) {
        // db_data_model_remove_model(model->priv->sync_model,DB_MODEL(object));
        g_object_unref(model->priv->sync_model);
    }
    // g_signal_emit(model,db_model_signals[MODEL_DISPOSE],0);
    G_OBJECT_CLASS(db_model_parent_class)->dispose(object);
}

void db_model_delete_callback(DbModel *model) {
    // g_debug("Model delete run ");
    // if(!DB_IS_LINK(model))	db_link_delete(model);
    db_model_delete_real(model);
}

static void db_model_real_notify(GObject *object, GParamSpec *pspec) {

    DbModel *model = DB_MODEL(object);
    // TEST:	g_print("Test ... set value ... freez = %d
    // proptype=%d\n",model->priv->freeze_save, pspec->flags & DB_MODEL_DB_PROP);
    if (!(model->priv->freeze_save) && (pspec->flags & DB_MODEL_DB_PROP) && (!(pspec->flags & DB_MODEL_PK_PROP)) &&
        (!(pspec->flags & DB_MODEL_DB_NOT_WRITABLE)) && (pspec->flags & G_PARAM_READABLE)) {
        // g_signal_emit(model,db_model_signals[MODEL_CHANGED],0,pspec);
        if (model->priv->ref_id > 0 && model->priv->sync_model != NULL) {
            // g_debug("Prop real notify changed %s sync",pspec->name);
            GValue *value = db_value_new(pspec->value_type);
            g_object_get_property(object, pspec->name, value);
            gchar *old_locale = g_strdup(setlocale(LC_ALL, NULL));
            setlocale(LC_ALL, "en_US.UTF-8");
            gchar *value_str = db_value_stringify(value);

            gchar *notify = NULL;
            if (G_TYPE_STRING == pspec->value_type)
                notify = g_strdup_printf("UPDATE %s SET %s='%s' WHERE ref_id = %" G_GUINT64_FORMAT ";", G_OBJECT_TYPE_NAME(object),
                    db_sql_parser_rps(pspec->name), value_str, model->priv->ref_id);
            else if (g_type_is_a(pspec->value_type, DB_TYPE_MODEL)) {
                // g_print("Object pointer =%p\n",g_value_get_object(value));
                notify = g_strdup_printf("UPDATE %s SET %s='%" G_GUINT64_FORMAT "' WHERE ref_id = %" G_GUINT64_FORMAT ";", G_OBJECT_TYPE_NAME(object),
                    db_sql_parser_rps(pspec->name), g_value_get_object(value) ? db_model_ref_id(DB_IMODEL(g_value_get_object(value))) : 0, model->priv->ref_id);
            } else
                notify = g_strdup_printf("UPDATE %s SET %s=%s WHERE ref_id = %" G_GUINT64_FORMAT ";", G_OBJECT_TYPE_NAME(object),
                    db_sql_parser_rps(pspec->name), value_str, model->priv->ref_id);
            // g_print("Model %s ID(%" G_GUINT64_FORMAT ") change property
            // %s=%s\n",G_OBJECT_TYPE_NAME(model),model->priv->ref_id,pspec->name,value_str);
            db_connection_send_no_select(DB_CONNECTION(db_data_model_get_provider(model->priv->sync_model)), notify);
            if ((pspec->flags & DB_MODEL_DB_LOG_CHANGE) && (DB_IS_PARAM(object))) {
                db_log_message(DB_LOG_MESSAGE_PARAMETER_CHANGE, "parameter %s %s changed (%s)", db_param_name(DB_PARAM(object)), pspec->_nick,
                    db_value_stringify_static(value));
            }
            g_free(value_str);
            db_value_free(value);
            g_free(notify);
            if (old_locale) {
                setlocale(LC_ALL, old_locale);
                g_free(old_locale);
            }
        }
    }
}
static void db_model_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(DB_IS_MODEL(object));
    DbModel *model = DB_MODEL(object);
    switch (prop_id) {
    case PROP_MODEL_ID:
        model->priv->ref_id = g_value_get_uint64(value);
        break;
    case PROP_MODEL_DATABASE:
        if (model->priv->database) g_free(model->priv->database);
        model->priv->database = g_value_dup_string(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void db_model_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(DB_IS_MODEL(object));
    DbModel *model = DB_MODEL(object);
    switch (prop_id) {
    case PROP_MODEL_ID:
        g_value_set_uint64(value, model->priv->ref_id);
        break;
    case PROP_MODEL_DATABASE:
        g_value_set_string(value, model->priv->database);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void db_model_class_init(DbModelClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // GObjectClass* parent_class = G_OBJECT_CLASS (klass);
    g_type_class_add_private(klass, sizeof(DbModelPrivate));
    object_class->constructed  = db_model_constructed;
    object_class->dispose      = db_model_dispose;
    object_class->finalize     = db_model_finalize;
    object_class->notify       = db_model_real_notify;
    object_class->set_property = db_model_set_property;
    object_class->get_property = db_model_get_property;
    klass->changed             = NULL;
    klass->initialized         = NULL;
    klass->delete              = db_model_delete_callback;
    g_object_class_override_property(object_class, PROP_MODEL_ID, "ref-id");
    g_object_class_override_property(object_class, PROP_MODEL_DATABASE, "database");

    db_model_signals[MODEL_CHANGED] = g_signal_new("model-changed", DB_TYPE_MODEL, G_SIGNAL_RUN_LAST, G_STRUCT_OFFSET(DbModelClass, changed), NULL, NULL,
        g_cclosure_marshal_VOID__PARAM, G_TYPE_NONE, 1, G_TYPE_PARAM);
    db_model_signals[MODEL_DISPOSE] =
        g_signal_new("model-dispose", DB_TYPE_MODEL, G_SIGNAL_RUN_LAST, 0, NULL, NULL, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);
}

/*
static gboolean
db_model_sync_idle ( gpointer user_data )
{
        //g_debug("Sync model .. ");
        DbModel* model  = DB_MODEL(user_data);
        if(model->priv->sync_model == NULL)  return FALSE;
        //db_model_sync(model);
        return TRUE;
}
*/

void db_model_set(DbModel *model, const gchar *first_property_name, ...) {
    va_list var_args;
    g_return_if_fail(G_IS_OBJECT(model));
    va_start(var_args, first_property_name);
    g_object_set_valist(G_OBJECT(model), first_property_name, var_args);
    va_end(var_args);
}

DbModel *db_model_new(GType object_type, const gchar *first_property_name, ...) {
    va_list  var_args;
    DbModel *model = NULL;
    g_return_val_if_fail(g_type_is_a(object_type, DB_TYPE_MODEL), NULL);
    if (!first_property_name) model = DB_MODEL(g_object_newv(object_type, 0, NULL));
    va_start(var_args, first_property_name);
    model = DB_MODEL(g_object_new_valist(object_type, first_property_name, var_args));
    db_model_thaw_notify(model);
    if (model->priv->ref_id == 0) {
        db_model_insert(model);
        if (model->priv->ref_id == 0) {
            g_critical("model type %s can not insert to data base", g_type_name(object_type));
            g_object_unref(model);
            model = NULL;
        }
    }
    va_end(var_args);
    return DB_MODEL(model);
}

void db_model_copy_param(DbModel *src, DbModel *dist) {
    g_return_if_fail(src != NULL);
    g_return_if_fail(DB_IS_MODEL(src));
    g_return_if_fail(dist != NULL);
    g_return_if_fail(DB_IS_MODEL(dist));
    // g_debug("Copy Model %s data to
    // %s",G_OBJECT_TYPE_NAME(src),G_OBJECT_TYPE_NAME(dist));
    guint        np        = 0;
    GParamSpec **paramlist = g_object_class_list_properties(G_OBJECT_GET_CLASS(src), &np);
    int          i;
    for (i = 0; i < np; i++) {
        if (!(paramlist[i]->flags & G_PARAM_CONSTRUCT_ONLY) && !(paramlist[i]->flags & DB_MODEL_PK_PROP) && !(paramlist[i]->flags & DB_MODEL_DB_REFERENCE)) {
            GParamSpec *pcopy = g_object_class_find_property(G_OBJECT_GET_CLASS(dist), paramlist[i]->name);
            if (pcopy != NULL && pcopy->value_type == paramlist[i]->value_type) {
                // g_debug("Copy property %s",paramlist[i]->name);
                GValue *value = db_value_new(paramlist[i]->value_type);
                g_object_get_property(G_OBJECT(src), paramlist[i]->name, value);
                g_object_set_property(G_OBJECT(dist), paramlist[i]->name, value);
                db_value_free(value);
            }
        }
    }
    g_free(paramlist);
}

void db_model_create_full(GType type, DbConnection *connection) {
    DbDataModel *dmodel = db_model_lookup_collectors(g_type_name(type));
    if (dmodel == NULL) {
        if (!g_type_is_a(type, DB_TYPE_MODEL)) {
            g_critical("Create table for unknown type ");
            return;
        }
        if (G_TYPE_IS_INTERFACE(type)) {
            g_critical("Create table for interface type ");
            return;
        }
        dmodel = db_data_model_new_for_object(G_OBJECT(connection), type);
        model_insert_collectors(dmodel);

        if (db_data_model_get_create(dmodel) != NULL) {
            db_sqlite_update_table(DB_SQLITE_CONNECTION(db_data_model_get_provider(dmodel)), type);
            /*	gchar *SQL =
            db_sql_parser_extended_default(db_data_model_get_create(dmodel),G_OBJECT(dmodel));

            if(!db_connection_check_table(DB_CONNECTION(db_data_model_get_provider(dmodel)),db_data_model_get_type_name(dmodel),SQL))
            {

                    //g_debug("Delete Table
            %s",db_data_model_get_type_name(dmodel));exit(1);
            }
            if(SQL)
            {
                    db_connection_send_no_select(DB_CONNECTION(db_data_model_get_provider(dmodel)),SQL);
                    g_free(SQL);
            }*/
        }
    }
    /*else
    {
            g_message ( "Model %s already created",g_type_name(type));
    }*/
}

void db_model_create(GType type) {
    // g_debug("Create type =%s",g_type_name(type));
    DbDataModel *dmodel = db_model_lookup_collectors(g_type_name(type));
    if (dmodel == NULL) {
        if (!g_type_is_a(type, DB_TYPE_MODEL)) {
            g_critical("Create table for unknown type ");
            return;
        }
        if (G_TYPE_IS_INTERFACE(type)) {
            g_critical("Create table for interface type ");
            return;
        }
        dmodel = db_data_model_new_for_object(NULL, type);
        model_insert_collectors(dmodel);
        if (db_data_model_get_create(dmodel) != NULL) {
            db_sqlite_update_table(DB_SQLITE_CONNECTION(db_data_model_get_provider(dmodel)), type);
            /*gchar *SQL =
            db_sql_parser_extended_default(db_data_model_get_create(dmodel),G_OBJECT(dmodel));
            if(SQL)
            {

                    if(!db_connection_check_table(DB_CONNECTION(db_data_model_get_provider(dmodel)),db_data_model_get_type_name(dmodel),SQL))
                    {
                            //g_debug("Delete Table
            %s",db_data_model_get_type_name(dmodel));//exit(1);
                    }
                    //db_connection_send_no_select(DB_CONNECTION(db_data_model_get_provider(dmodel)),SQL);
                    g_free(SQL);
            }*/
        }
    }
    /*else
    {
            g_message ( "Model %s already created",g_type_name(type));
    }*/
}

const gchar *db_model_primary_key(GType type) { return "ref_id"; }

static GThreadPool *model_async_pool = NULL;
static GThreadPool *runs_async_pool  = NULL;

typedef struct {
    gpointer *           userdata;
    DbModelAsyncCallback callback;
    GList *              data_models;
    gchar *              SQL;
    GSList *             models;

} DbModelAsync;

typedef struct {
    gpointer *           userdata;
    DbRunedAsyncCallback callback;
    guint64              id;
    GType                type;

} DbModelRunsAsync;

static gboolean db_model_sync_load_at_main(gpointer data) {
    DbModelAsync *async = (DbModelAsync *)data;
    g_return_val_if_fail(async != NULL, FALSE);

    if (async->callback) async->callback(async->models, async->userdata);
    if (async->data_models) g_list_free(async->data_models);
    if (async->models) db_slist_free_full(async->models, g_object_unref);
    if (async->SQL) g_free(async->SQL);
    g_free(data);
    return FALSE;
}

static void db_model_async_load_func(gpointer data, gpointer user_data) {
    DbModelAsync *async = (DbModelAsync *)data;
    g_return_if_fail(async != NULL);
    GList *l      = NULL;
    async->models = NULL;

    for (l = async->data_models; l != NULL; l = l->next) {
        GType otype = db_data_model_object_type(DB_DATA_MODEL(l->data));
        if (l->data && DB_IS_DATA_MODEL(l->data) && G_TYPE_NONE != otype) {
            DbConnection *connection = DB_CONNECTION(db_data_model_new_provider(DB_DATA_MODEL(l->data)));
            // TEST:g_debug("Test find model ..%s
            // ",db_data_model_get_type_name(DB_DATA_MODEL(l->data)));
            // DbDataModel *dm = DB_DATA_MODEL(l->data);
            DbSqlParser *parser = g_object_new(DB_TYPE_SQL_PARSER, "parser-name", "async", "parser-table", g_type_name(otype), NULL);

            gchar *RSQL = db_sql_parser_extended(parser, async->SQL, G_OBJECT(parser));
            if (RSQL) {
                // TEST:	g_debug("Async RSQL =%s",RSQL);
                GSList *lm = db_connection_send(connection, otype, RSQL);
                g_object_unref(parser);
                // g_debug("test ... len = %d",g_slist_length(lm));
                if (async->models == NULL && lm != NULL)
                    async->models = lm;
                else if (async->models != NULL && lm != NULL)
                    async->models = g_slist_concat(async->models, lm);
                g_free(RSQL);
            }
            g_object_unref(connection);
        }
    }

    g_timeout_add(30, db_model_sync_load_at_main, data);
}

static void db_model_async_run_func(gpointer data, gpointer user_data) {
    DbModelRunsAsync *async = (DbModelRunsAsync *)data;
    g_return_if_fail(async != NULL);
    if (async->callback) {
        async->callback(async->type, async->id, async->userdata);
    }
    g_free(async);
}

#define MODEL_MAX_ASYNC_JOBS 100

static gboolean db_model_async_init() {
    if (model_async_pool == NULL && runs_async_pool == NULL) {
#if GLIB_CHECK_VERSION(2, 31, 7)
#else
        g_thread_init(NULL);
#endif
        model_async_pool = g_thread_pool_new((GFunc)db_model_async_load_func, NULL, MODEL_MAX_ASYNC_JOBS, FALSE, NULL);
        runs_async_pool  = g_thread_pool_new((GFunc)db_model_async_run_func, NULL, MODEL_MAX_ASYNC_JOBS, FALSE, NULL);
    }
    return TRUE;
}
gboolean db_model_runs_async(GType iface, guint64 id, DbRunedAsyncCallback callback, gpointer user_data) {
    g_assert(db_model_async_init() == TRUE);
    if (g_thread_pool_get_num_threads(runs_async_pool) == MODEL_MAX_ASYNC_JOBS) {
        g_critical("Max async jobs was reached");
        return FALSE;
    }
    DbModelRunsAsync *async = g_malloc(sizeof(DbModelRunsAsync));
    async->type             = iface;
    async->id               = id;
    async->callback         = callback;
    async->userdata         = user_data;
    g_thread_pool_push(runs_async_pool, async, NULL);
    return TRUE;
}

gboolean db_model_select_async(GType iface, DbModelAsyncCallback callback, gpointer user_data, const gchar *format, ...) {
    // GSList *models     = NULL;
    g_assert(db_model_async_init() == TRUE);
    if (g_thread_pool_get_num_threads(model_async_pool) == MODEL_MAX_ASYNC_JOBS) {
        g_critical("Max async jobs was reached");
        return FALSE;
    }
    gchar *old_locale = g_strdup(setlocale(LC_ALL, NULL));
    setlocale(LC_ALL, "en_US.UTF-8");
    va_list args;
    gchar * SQL;
    va_start(args, format);
    SQL = g_strdup_vprintf(format, args);
    va_end(args);
    setlocale(LC_ALL, old_locale);
    g_free(old_locale);
    DbModelAsync *async = g_malloc(sizeof(DbModelAsync));
    async->SQL          = SQL;
    async->data_models  = db_model_list_datamodels(iface);
    async->callback     = callback;
    async->userdata     = user_data;
    async->models       = NULL;
    g_thread_pool_push(model_async_pool, async, NULL);
    return TRUE;
}

/*
static gboolean
model_async_look_timeout_callback ( gpointer data )
{
        DbSelection *selection = DB_SELECTION(data);
        db_selection_run_callback(selection);
        g_object_unref(selection);
        return FALSE;
}
*/
static void async_look_ready_callback(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    GCancellable *cancel = g_task_get_cancellable(G_TASK(res));
    if (cancel && g_cancellable_is_cancelled(cancel)) {
        g_object_unref(source_object);
        return;
    }
    if (!g_task_had_error(G_TASK(res))) {
        db_selection_run_callback(DB_SELECTION(source_object));
        g_object_unref(source_object);
        // g_timeout_add(10,model_async_look_timeout_callback,source_object);
    } else
        g_object_unref(source_object);
}

static void db_model_look_thread(GTask *task, gpointer source_object, gpointer task_data, GCancellable *cancellable) {
    // error_pending = 1 and error_type = %d",DB_ERROR_CRITICAL
    DbSelection *selection = DB_SELECTION(source_object);
    if (g_task_return_error_if_cancelled(task)) {
        return;
    }
    db_selection_run(selection);
    if (g_task_return_error_if_cancelled(task)) {
        return;
    }
    g_task_return_int(task, 1);
}

gboolean db_model_look(GType type, GCancellable *cancellable, SelectionReadyCallback callback, gpointer user_data, const gchar *format, ...) {
    gchar *      SQL;
    DbDataModel *model = db_model_lookup_collectors(g_type_name(type));
    if (model == NULL) {
        g_warning(" Data model for type %s not found", g_type_name(type));
        return FALSE;
    }
    gchar *old_locale = g_strdup(setlocale(LC_ALL, NULL));
    setlocale(LC_ALL, "en_US.UTF-8");
    va_list       args;
    DbConnection *connection = DB_CONNECTION(db_data_model_new_provider(model));
    va_start(args, format);
    SQL = g_strdup_vprintf(format, args);
    va_end(args);
    GObject *selection = g_object_new(DB_TYPE_SELECTION, "selection-sql", SQL, "selection-gtype", type, NULL);
    g_free(SQL);
    setlocale(LC_ALL, old_locale);
    g_free(old_locale);
    db_selection_add_connection(DB_SELECTION(selection), connection);
    db_selection_add_ready_callback(DB_SELECTION(selection), callback);
    db_selection_add_user_data(DB_SELECTION(selection), user_data);
    g_object_unref(connection);
    GTask *task = g_task_new(selection, cancellable, async_look_ready_callback, user_data);
    g_task_run_in_thread(task, db_model_look_thread);
    g_object_unref(task);
    return TRUE;
}

static gboolean model_async_delete_timeout_callback(gpointer data) {
    DbSelection *selection = DB_SELECTION(data);
    db_selection_run_callback(selection);
    g_object_unref(selection);
    return FALSE;
}

static void async_delete_ready_callback(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    if (!g_task_had_error(G_TASK(res))) g_timeout_add(10, model_async_delete_timeout_callback, source_object);
    // g_object_unref(res);
}

static void db_model_delete_thread(GTask *task, gpointer source_object, gpointer task_data, GCancellable *cancellable) {
    // error_pending = 1 and error_type = %d",DB_ERROR_CRITICAL
    DbSelection *selection = DB_SELECTION(source_object);
    if (!g_cancellable_is_cancelled(cancellable)) db_selection_run_no_return(selection);
    if (!g_task_return_error_if_cancelled(task)) g_task_return_int(task, 1);
}

gboolean db_model_delete_async(GType type, GCancellable *cancellable, SelectionReadyCallback callback, gpointer user_data, const gchar *format, ...) {
    gchar *      SQL;
    DbDataModel *model = db_model_lookup_collectors(g_type_name(type));
    if (model == NULL) {
        g_warning(" Data model for type %s not found", g_type_name(type));
        return FALSE;
    }
    gchar *old_locale = g_strdup(setlocale(LC_ALL, NULL));
    setlocale(LC_ALL, "en_US.UTF-8");
    va_list       args;
    DbConnection *connection = DB_CONNECTION(db_data_model_new_provider(model));
    va_start(args, format);
    SQL = g_strdup_vprintf(format, args);
    va_end(args);
    GObject *selection = g_object_new(DB_TYPE_SELECTION, "selection-sql", SQL, "selection-gtype", type, NULL);
    g_free(SQL);
    setlocale(LC_ALL, old_locale);
    g_free(old_locale);
    db_selection_add_connection(DB_SELECTION(selection), connection);
    db_selection_add_ready_callback(DB_SELECTION(selection), callback);
    db_selection_add_user_data(DB_SELECTION(selection), user_data);
    g_object_unref(connection);
    GTask *task = g_task_new(selection, cancellable, async_delete_ready_callback, user_data);
    g_task_run_in_thread(task, db_model_delete_thread);
    g_object_unref(task);
    return TRUE;
}

typedef struct _ModeTypeKey ModelTypedKey;
typedef struct _ModeUserKey ModeUserKey;
typedef struct _ModelIdKey  ModelIdKey;

struct _ModeUserKey {
    DbModelSignalCallback callback;
    gpointer              user_data;
    gchar *               property;
    GValue *              value;
};

struct _ModelIdKey {
    guint64 ID;
    GSList *user_keys;
    guint   miss;
};

struct _ModeTypeKey {
    GType         otype;
    DbConnection *cnc;
    GSList *      ids;
};

GSList *models_signals_root = NULL;

/*
static gint
model_user_key_compare  (gconstpointer a, gconstpointer b)
{
        ModeUserKey *Key  = (ModeUserKey*) a;
        ModeUserKey *uKey = (ModeUserKey*) b;
        if(Key->callback !=  uKey->callback)  return 1;
        if(Key->user_data!=  uKey->user_data) return 1;
        if(0!=g_strcmp0(Key->property ,uKey->property) )return 1;
        return 0;
}*/

static gint model_ids_compare(gconstpointer a, gconstpointer b) {
    ModelIdKey *tKey = (ModelIdKey *)a;
    ModelIdKey *nKey = (ModelIdKey *)b;
    if (tKey->ID != nKey->ID) return 1;
    return 0;
}

static gint model_type_compare(gconstpointer a, gconstpointer b) {
    ModelTypedKey *tKey = (ModelTypedKey *)a;
    ModelTypedKey *nKey = (ModelTypedKey *)b;
    if (tKey->otype != nKey->otype) return 1;
    return 0;
}

ModeUserKey *model_find_user_key(ModelIdKey *iKey, const gchar *property, DbModelSignalCallback callback, gpointer user_data) {
    g_return_val_if_fail(iKey != NULL, NULL);
    if (iKey->user_keys == NULL) return NULL;
    ModeUserKey uKey;
    uKey.callback   = callback;
    uKey.user_data  = user_data;
    uKey.property   = g_strdup(property);
    GSList *keyList = g_slist_find_custom(iKey->user_keys, (gconstpointer)&uKey, model_type_compare);
    g_free(uKey.property); // TODO:check_memory ... new 12.12 16:54
    if (keyList == NULL) return NULL;
    return (ModeUserKey *)keyList->data;
}
ModelIdKey *model_find_id_key(ModelTypedKey *Key, guint64 ref_id) {
    g_return_val_if_fail(Key != NULL, NULL);
    if (Key->ids == NULL) return NULL;
    ModelIdKey iKey;
    iKey.ID         = ref_id;
    GSList *keyList = g_slist_find_custom(Key->ids, (gconstpointer)&iKey, model_ids_compare);
    if (keyList == NULL) return NULL;
    return (ModelIdKey *)keyList->data;
}

ModelTypedKey *model_lookup_typed_key(GType model_type) {
    if (models_signals_root == NULL) return NULL;
    ModelTypedKey tKey;
    tKey.otype    = model_type;
    GSList *tList = g_slist_find_custom(models_signals_root, (gconstpointer)&tKey, model_type_compare);
    if (tList == NULL) return NULL;
    return (ModelTypedKey *)tList->data;
}
ModeUserKey *model_new_user_key(ModelIdKey *Key, const gchar *property, DbModelSignalCallback callback, gpointer user_data) {
    g_return_val_if_fail(Key != NULL, NULL);
    ModeUserKey *uKey = g_malloc(sizeof(ModeUserKey));
    uKey->callback    = callback;
    uKey->user_data   = user_data;
    uKey->value       = NULL;
    uKey->property    = g_strdup(property);
    Key->user_keys    = g_slist_append(Key->user_keys, uKey);
    return uKey;
}

ModelIdKey *model_new_id_key(ModelTypedKey *Key, guint64 ref_id, const gchar *property, DbModelSignalCallback callback, gpointer user_data) {
    g_return_val_if_fail(Key != NULL, NULL);
    ModelIdKey *iKey = g_malloc(sizeof(ModelIdKey));
    iKey->ID         = ref_id;
    iKey->user_keys  = NULL;
    Key->ids         = g_slist_append(Key->ids, iKey);
    model_new_user_key(iKey, property, callback, user_data);
    return iKey;
}
static ModelTypedKey *model_new_typed_key(GType model_type, guint64 ID, const gchar *property, DbModelSignalCallback callback, gpointer user_data) {
    DbDataModel *model = db_model_lookup_collectors(g_type_name(model_type));
    ModelTypedKey *new = NULL;
    if (model) {
        new        = g_malloc(sizeof(ModelTypedKey));
        new->otype = model_type;
        new->ids   = NULL;
        new->cnc   = DB_CONNECTION(db_data_model_new_provider(model));
        model_new_id_key(new, ID, property, callback, user_data);
    }
    return new;
}

static void moder_signal_user_key_unref(gpointer ptr) {
    ModeUserKey *uKey = (ModeUserKey *)ptr;
    g_free(uKey->property);
    if (uKey->value) db_value_free(uKey->value);
    g_free(ptr);
}

static void model_remove_id_key(gpointer ptr) {
    ModelIdKey *uKey = (ModelIdKey *)ptr;
    db_slist_free_full(uKey->user_keys, moder_signal_user_key_unref);
    g_free(ptr);
}

/*
static void
model_remove_typed_key (ModelTypedKey *remove )
{
        g_debug("Remove %s ",g_type_name(remove->otype));
        db_slist_free_full(remove->ids,model_remove_id_key);
        models_signals_root = g_slist_remove (models_signals_root,remove);
}
*/

static GThreadPool *model_signal_async_pool = NULL;

static void db_model_signal_async_func(gpointer data, gpointer user_data) {
    static GSList *last_typed = NULL;
    // struct stat file_stat;
    while (1) {
        SIGNAL_LOCK();
        if (models_signals_root == NULL) {
            SIGNAL_UNLOCK();
            g_usleep(60000);
            continue;
        }
        ModelTypedKey *Key = NULL;
        if (last_typed == NULL || last_typed->next == NULL) {
            last_typed = models_signals_root;
        } else {
            last_typed = last_typed->next;
        }

        Key = (ModelTypedKey *)last_typed->data;
        SIGNAL_UNLOCK();
        GSList *l = NULL;
        for (l = Key->ids; l != NULL; l = l->next) {
            ModelIdKey *iKey        = (ModelIdKey *)l->data;
            guint64     id          = iKey->ID;
            gchar *     RSQL        = g_strdup_printf("select * from %s where ref_id = %" G_GUINT64_FORMAT, g_type_name(Key->otype), id);
            GSList *    list_models = db_connection_send(Key->cnc, Key->otype, RSQL);
            g_free(RSQL);
            SIGNAL_LOCK();
            if (list_models) {
                if (iKey->miss > 0) iKey->miss--;
                GSList *lm = NULL;
                for (lm = list_models; lm != NULL; lm = lm->next) {
                    g_usleep(10000);
                    GSList *l = NULL;
                    for (l = iKey->user_keys; l != NULL; l = l->next) {
                        ModeUserKey *UKey  = (ModeUserKey *)l->data;
                        GParamSpec * pspec = NULL;
                        pspec              = g_object_class_find_property(G_OBJECT_GET_CLASS(G_OBJECT(lm->data)), UKey->property);
                        if (pspec) {
                            GValue *value = db_value_new(pspec->value_type);
                            g_object_get_property(G_OBJECT(lm->data), pspec->name, value);
                            // g_debug("%s=%s",pspec->name,db_value_stringify(value));
                            if (UKey->value) {
                                if (!db_value_g_value_equal(UKey->value, value)) {
                                    UKey->callback(DB_MODEL(lm->data), UKey->user_data);
                                }
                                db_value_free(UKey->value);
                                UKey->value = value;
                            } else {
                                UKey->value = value;
                            }
                        }
                    }
                }
                db_slist_free_full(list_models, g_object_unref);
            } else {
                if (iKey->miss > 100) {
                    model_remove_id_key(iKey);
                    last_typed = NULL;
                }
            }
        }
        SIGNAL_UNLOCK();
        g_usleep(50000);
    }
}

static gint test_value = 0;

gboolean db_model_connect_async_init() {
    if (model_signal_async_pool == NULL) {
#if GLIB_CHECK_VERSION(2, 31, 7)
#else
        g_thread_init(NULL);
#endif
        model_signal_async_pool = g_thread_pool_new((GFunc)db_model_signal_async_func, NULL, 2, FALSE, NULL);
        g_thread_pool_push(model_signal_async_pool, &test_value, NULL);
    }
    return TRUE;
}

gboolean db_model_connect_signal(GType model_type, guint64 ref_id, const gchar *property, DbModelSignalCallback callback, gpointer user_data) {
    g_return_val_if_fail(g_type_is_a(model_type, DB_TYPE_MODEL), FALSE);
    g_return_val_if_fail(ref_id > 0, FALSE);
    g_return_val_if_fail(property != NULL, FALSE);
    g_return_val_if_fail(callback, FALSE);
    SIGNAL_LOCK();
    gboolean       ret = FALSE;
    ModelTypedKey *Key = model_lookup_typed_key(model_type);
    if (Key == NULL) {
        Key                 = model_new_typed_key(model_type, ref_id, property, callback, user_data);
        models_signals_root = g_slist_append(models_signals_root, Key);
        ret                 = TRUE;
    } else {
        ModelIdKey *iKey = model_find_id_key(Key, ref_id);
        if (iKey == NULL) {
            iKey = model_new_id_key(Key, ref_id, property, callback, user_data);
            ret  = TRUE;
        } else {
            ModeUserKey *UKey = model_find_user_key(iKey, property, callback, user_data);
            if (UKey == NULL) {
                UKey = model_new_user_key(iKey, property, callback, user_data);
                ret  = TRUE;
            } else {
                g_critical(" Signal exist ....");
            }
        }
    }
    SIGNAL_UNLOCK();
    return ret;
}

GSList *db_model_select_sql(GType iface, gboolean local, const gchar *SQL) {
    GList * lmodel = db_model_list_datamodels(iface);
    GSList *models = NULL;
    GList * l      = NULL;
    if (lmodel == NULL) {
        g_critical("DataModel for %s not find", g_type_name(iface));
        g_assert(lmodel != NULL);
    }
    for (l = lmodel; l != NULL; l = l->next) {
        if (l->data && DB_IS_DATA_MODEL(l->data) && G_TYPE_NONE != db_data_model_object_type(DB_DATA_MODEL(l->data))) {
            // TEST:			g_debug("Test find model ..%s
            // ",db_data_model_get_type_name(DB_DATA_MODEL(l->data)));
            DbDataModel *dm     = DB_DATA_MODEL(l->data);
            DbSqlParser *parser = db_data_model_get_select(dm);
            gchar *      RSQL   = db_sql_parser_extended(parser, SQL, G_OBJECT(parser));
            if (RSQL) {
                // TEST:				g_debug("RSQL =%s",RSQL);
                GSList *lm = db_connection_send(DB_CONNECTION(db_data_model_get_provider(dm)), db_data_model_object_type(DB_DATA_MODEL(l->data)), RSQL);
                //		g_debug("test ... len = %d",g_slist_length(lm));
                if (models == NULL && lm != NULL)
                    models = lm;
                else if (models != NULL && lm != NULL)
                    models = g_slist_concat(models, lm);
                g_free(RSQL);
            }
        }
    }
    if (lmodel) g_list_free(lmodel);
    return models;
}

GSList *db_model_select(GType iface, const gchar *format, ...) {
    GSList *models = NULL;
    va_list args;
    gchar * SQL;
    gchar * old_locale = g_strdup(setlocale(LC_ALL, NULL));
    setlocale(LC_ALL, "en_US.UTF-8");
    va_start(args, format);
    SQL = g_strdup_vprintf(format, args);
    va_end(args);
    setlocale(LC_ALL, old_locale);
    g_free(old_locale);
    models = db_model_select_sql(iface, FALSE, SQL);
    g_free(SQL);
    return models;
}

gboolean db_model_exec(GType type, const gchar *format, ...) {
    va_list args;
    gchar * SQL;
    gchar * old_locale = g_strdup(setlocale(LC_ALL, NULL));
    setlocale(LC_ALL, "en_US.UTF-8");
    va_start(args, format);
    SQL = g_strdup_vprintf(format, args);
    va_end(args);
    setlocale(LC_ALL, old_locale);
    g_free(old_locale);
    GList *lmodel = db_model_list_datamodels(type);
    GList *l      = NULL;
    if (lmodel == NULL) {
        g_critical("DataModel for %s not find", g_type_name(type));
    } else {
        for (l = lmodel; l != NULL; l = l->next) {
            if (l->data && DB_IS_DATA_MODEL(l->data) && G_TYPE_NONE != db_data_model_object_type(DB_DATA_MODEL(l->data))) {
                DbDataModel *dm     = DB_DATA_MODEL(l->data);
                DbSqlParser *parser = db_data_model_get_select(dm);
                gchar *      RSQL   = db_sql_parser_extended(parser, SQL, G_OBJECT(parser));
                if (RSQL) {
                    db_connection_send_no_select(DB_CONNECTION(db_data_model_get_provider(dm)), RSQL);
                    g_free(RSQL);
                }
                // TEST:	g_debug("RSQL =%s",RSQL);
            }
        }
        if (lmodel) g_list_free(lmodel);
    }
    g_free(SQL);
    return TRUE;
}

GParamSpec *db_model_find_pspec(GType model_type, const gchar *pname) {
    guint       ni    = 0;
    GType *     iface = g_type_interfaces(model_type, &ni);
    int         i     = 0;
    GParamSpec *pspec = NULL;
    for (i = 0; i < ni; i++) {
        gpointer itable = g_type_default_interface_ref(iface[i]);
        if (itable) {
            pspec = g_object_interface_find_property(itable, pname);
            if (itable) g_type_default_interface_unref(itable);
            if (pspec != NULL) break;
        }
    }
    g_free(iface);
    return pspec;
}

GList *db_model_list_pspec(GType model_type) {
    guint  ni    = 0;
    GType *iface = g_type_interfaces(model_type, &ni);
    GList *plist = NULL;
    int    i     = 0;
    for (i = 0; i < ni; i++) {
        gpointer itable = g_type_default_interface_ref(iface[i]);
        if (itable) {
            GParamSpec **pspec = NULL;
            guint        plen  = 0;
            pspec              = g_object_interface_list_properties(itable, &plen);
            int j              = 0;
            for (j = 0; j < plen; j++) {
                if (pspec != NULL && pspec[j]) {
                    plist = g_list_append(plist, pspec[j]);
                }
            }
            g_free(pspec);
        }
    }
    return plist;
}

guint db_model_nth_pspec(GType model_type) {
    guint  ni    = 0;
    guint  len   = 0;
    GType *iface = g_type_interfaces(model_type, &ni);
    int    i     = 0;
    for (i = 0; i < ni; i++) {
        gpointer itable = g_type_default_interface_ref(iface[i]);
        if (itable) {
            guint        np    = 0;
            GParamSpec **pscpe = g_object_interface_list_properties(itable, &np);
            len                = len + np;
            g_free(pscpe);
            if (itable) g_type_default_interface_unref(itable);
        }
    }
    return len;
}

/*
 * "objects": [
    {
      "min_hw": 0.0,
      "description": "This is default sensor description",
      "min": 0.0,
      "max": 1.0,
      "online": false,
      "registred": false,
      "max_hw": 20.0,
      "handler": "handler.SensorHandler",
      "average_sec": 1.0,
      "value": 0.0,
      "created_date": "2013-09-16 21:30:41",
      "id": 1,
      "unit": "mA",
      "name": "Test sensor"
    },
 *
 *
 *{
  "meta": {
    "model": "sensor",
    "next": "",
    "page": 1,
    "previous": ""
  },
  "objects": [
    {
 *
 *
 */

const gchar *serialize_space_stdout(int count) {
    if (count == 0) return "";
    static gchar buf[50];
    memset(buf, 0, sizeof(buf));
    memset(buf, ' ', count);
    return buf;
}

gchar *db_model_serialize_object(DbModel *model, gint ab) {
    // g_debug("Serialize model type %s",G_OBJECT_TYPE_NAME(model));
    GString *string = g_string_new("");
    g_string_append_printf(string, "{\n\r");
    guint        pn    = 0;
    GParamSpec **pspec = g_object_class_list_properties(G_OBJECT_GET_CLASS(G_OBJECT(model)), &pn);
    gint         i     = 0;
    for (i = 0; i < pn; i++) {
        GValue *value = db_value_new(pspec[i]->value_type);
        g_object_get_property(G_OBJECT(model), pspec[i]->name, value);
        if (g_type_is_a(pspec[i]->value_type, DB_TYPE_MODEL)) {
            DbModel *child = g_value_get_object(value);

            gchar *model_str = child != NULL ? db_model_serialize_object(child, ab + 5) : g_strdup("Nil");
            g_string_append_printf(string, "%s  \"%s\": %s,\r\n", serialize_space_stdout(ab), pspec[i]->name, model_str);
            g_free(model_str);
        } else if (G_TYPE_STRING == pspec[i]->value_type) {

            g_string_append_printf(string, "%s  \"%s\": \"%s\",\r\n", serialize_space_stdout(ab), pspec[i]->name, g_value_get_string(value));
        } else {
            gchar *str_val = db_value_stringify(value);
            g_string_append_printf(string, "%s  \"%s\": %s,\r\n", serialize_space_stdout(ab), pspec[i]->name, str_val);
            g_free(str_val);
        }
        db_value_free(value);
    }
    g_string_append_printf(string, "%s}", serialize_space_stdout(ab));

    gchar *json = string->str;
    g_string_free(string, FALSE);
    return json;
}

gchar *db_model_serialize_one(DbModel *model) {
    g_return_val_if_fail(model != NULL, NULL);
    gchar *  json = NULL;
    GString *str  = g_string_new("{\n\r");
    g_string_append_printf(str, "  \"meta\": {\n\r");
    g_string_append_printf(str, "  \"model\": \"%s\",\n\r", G_OBJECT_TYPE_NAME(model));
    g_string_append_printf(str, "  \"next\": \"\",\n\r");
    g_string_append_printf(str, "  \"length\": %d,\n\r", 1);
    g_string_append_printf(str, "  \"previous\": \"\",\n\r");
    g_string_append_printf(str, "  },\n\r");
    g_string_append_printf(str, "  \"objects\": [\n\r");
    gchar *object = db_model_serialize_object(DB_MODEL(model), 4);
    g_string_append_printf(str, "%s\n\r", object);
    g_string_append_printf(str, "  ]\n\r");
    g_string_append_printf(str, "}\n\r");
    g_free(object);
    json = str->str;
    g_string_free(str, FALSE);
    return json;
}

gchar *db_model_serialize(GType iface, const gchar *format, ...) {
    GSList *models = NULL;
    va_list args;
    gchar * SQL;
    gchar * old_locale = g_strdup(setlocale(LC_ALL, NULL));
    setlocale(LC_ALL, "en_US.UTF-8");
    va_start(args, format);
    SQL = g_strdup_vprintf(format, args);
    va_end(args);
    setlocale(LC_ALL, old_locale);
    g_free(old_locale);

    models = db_model_select_sql(iface, FALSE, SQL);
    // g_debug("SQL=%s",SQL);
    g_free(SQL);
    //	g_debug("Test type = %s lenght=
    //%d",g_type_name(iface),g_slist_length(models));
    gchar *json = NULL;
    if (models) {
        if (g_slist_length(models) == 1) {
            json = db_model_serialize_object(DB_MODEL(models->data), 0);
        } else {
            GString *str = g_string_new("{\n\r");
            g_string_append_printf(str, "  \"meta\": {\n\r");
            g_string_append_printf(str, "  \"model\": \"%s\",\n\r", g_type_name(iface));
            g_string_append_printf(str, "  \"next\": \"\",\n\r");
            g_string_append_printf(str, "  \"length\": %d,\n\r", g_slist_length(models));
            g_string_append_printf(str, "  \"previous\": \"\",\n\r");
            g_string_append_printf(str, "  },\n\r");
            g_string_append_printf(str, "  \"objects\": [\n\r");
            GSList *l = NULL;
            for (l = models; l != NULL; l = l->next) {
                gchar *object = db_model_serialize_object(DB_MODEL(l->data), 4);
                g_string_append_printf(str, "%s%s\n\r", object, l->next ? "," : "");
                g_free(object);
            }
            g_string_append_printf(str, "  ]\n\r");
            g_string_append_printf(str, "}\n\r");
            json = str->str;
            g_string_free(str, FALSE);
        }
    }
    return json;
}

/*GSList*
db_model_select_local  ( GType iface , const gchar *format, ... )
{
        GSList *models     = NULL;
        va_list args;
        gchar *SQL;
        va_start (args, format);
        SQL =g_strdup_vprintf (format, args);
        va_end (args);
        models = db_model_select_sql(iface,TRUE,SQL);
        g_free(SQL);
        return models;
}*/

DbModel *db_model_select_one(GType iface, const gchar *format, ...) {
    GSList *models     = NULL;
    gchar * old_locale = g_strdup(setlocale(LC_ALL, NULL));
    setlocale(LC_ALL, "en_US.UTF-8");
    va_list args;
    gchar * SQL;
    va_start(args, format);
    SQL = g_strdup_vprintf(format, args);
    va_end(args);
    setlocale(LC_ALL, old_locale);
    g_free(old_locale);
    models = db_model_select_sql(iface, FALSE, SQL);
    g_free(SQL);
    if (models == NULL) return NULL;
    if (g_slist_length(models) > 1) {
        g_warning("Select one returned more as 1 model");
        g_warning("First model be returned own models freed");
    }
    if (models->data == NULL || !DB_IS_MODEL(models->data)) {
        g_warning("selected data model contains unknown data");
        g_slist_free(models);
        return NULL;
    }
    DbModel *model = DB_MODEL(g_object_ref(models->data));
    db_slist_free_full(models, g_object_unref);
    return model;
}

/*
 *
 *
 * @example: String example for widget parcer
 *                   interface   ref_id    property
 *                   ------------------------------
 *                   DbLimit  :   %d  :  limit_min
 */

DbModel *db_model_select_parser(const gchar *wid) {
    g_return_val_if_fail(wid != NULL, NULL);
    //	g_debug("db_model_select_parser %s",wid);
    gchar **parts = g_strsplit_set(wid, ":", -1);
    if (parts == NULL || g_strv_length(parts) < 2) {
        g_warning("Parse widget SQL %s failed", wid);
        if (parts) g_strfreev(parts);
        return NULL;
    }
    GType iface = g_type_from_name(parts[0]);
    if (iface < G_TYPE_INTERFACE) {
        g_warning("Model parse SQL %s interface (%" G_GSIZE_FORMAT ":%s) fail", wid, iface, g_type_name(iface));
        g_strfreev(parts);
        return NULL;
    }
    guint64 id = g_ascii_strtoull(parts[1], NULL, 0);
    g_strfreev(parts);
    if (id == 0) {
        g_warning("Model parse SQL %s invalid reference id %" G_GUINT64_FORMAT, wid, id);
        return NULL;
    }
    // TEST:	g_debug("%s (%" G_GSIZE_FORMAT ":%s - ref_id:%" G_GUINT64_FORMAT
    // ")",wid,iface,g_type_name(iface),id);
    DbModel *model = NULL;
    model          = db_model_select_one(iface, "select * from $tablename where ref_id = %" G_GUINT64_FORMAT, id);
    return model;
}

gboolean db_model_delete(DbModel *model) {
    g_return_val_if_fail(model != NULL, FALSE);
    g_return_val_if_fail(DB_IS_MODEL(model), FALSE);
    if (DB_MODEL_GET_CLASS(model)->delete) {
        DB_MODEL_GET_CLASS(model)->delete (model);
    }
    /*else
    {
            db_model_delete_real(model);
    }*/
    return TRUE;
}

void db_model_unref_and_delete(gpointer object) {
    db_model_delete(DB_MODEL(object));
    g_object_unref(object);
}

void db_model_print_spacing(guint level) {
    while (level > 0) {
        g_printf(" ");
        level--;
    };
}

void db_model_print_stdout(DbModel *model) {

    guint        np        = 0;
    GParamSpec **paramlist = g_object_class_list_properties(G_OBJECT_GET_CLASS(model), &np);
    int          i;
    for (i = 0; i < np; i++) {
        db_model_print_spacing(3);
        GValue *value = db_value_new(paramlist[i]->value_type);
        g_object_get_property(G_OBJECT(model), paramlist[i]->name, value);
        gchar *strval = db_value_stringify(value);
        g_free(strval);
        db_value_free(value);
    }
    g_free(paramlist);
}

gchar *db_model_dub_dbreference(DbModel *model) {
    g_return_val_if_fail(model != NULL, NULL);
    g_return_val_if_fail(DB_IS_MODEL(model), NULL);
    const gchar *db    = db_connection_get_data_base(DB_CONNECTION(db_data_model_get_provider(model->priv->sync_model)));
    const gchar *table = G_OBJECT_TYPE_NAME(model);
    return g_strdup_printf("%s.%s.%" G_GUINT64_FORMAT, db, table, model->priv->ref_id);
}

DbConnection *db_model_database(DbModel *model) {
    g_return_val_if_fail(model != NULL, NULL);
    g_return_val_if_fail(DB_IS_MODEL(model), NULL);
    if (model->priv->sync_model == NULL) {
        g_critical("Model type %s not found", G_OBJECT_TYPE_NAME(model));
        return NULL;
    }
    return DB_CONNECTION(db_data_model_get_provider(model->priv->sync_model));
}

void db_model_delete_table(GType type) {
    GList *lmodel = db_model_list_datamodels(type);
    GList *l      = NULL;
    if (lmodel == NULL) {
        g_critical("DataModel for %s not find", g_type_name(type));
    } else {
        for (l = lmodel; l != NULL; l = l->next) {
            if (l->data && DB_IS_DATA_MODEL(l->data) && G_TYPE_NONE != db_data_model_object_type(DB_DATA_MODEL(l->data))) {
                DbDataModel *dm = DB_DATA_MODEL(l->data);
                if (dm) {
                    gchar *RSQL = g_strdup_printf("DROP TABLE IF EXIST main.%s", g_type_name(type));
                    db_connection_send_no_select(DB_CONNECTION(db_data_model_get_provider(dm)), RSQL);
                    g_free(RSQL);
                }
            }
        }
        if (lmodel) g_list_free(lmodel);
    }
}

/*gboolean
db_model_reload ( DbModel *model  )
{
        g_return_val_if_fail(model!=NULL,FALSE);
        g_return_val_if_fail(DB_IS_MODEL(model),FALSE);
        if(model->priv->sync_model==NULL)
        {
                GType type = G_OBJECT_TYPE(G_OBJECT(model));
                DbDataModel *data =
db_model_lookup_collectors(g_type_name(type));
                if(NULL != data )
                {
                        db_mode_set_data_model(model,data);
                }
        }
//	return db_model_sync(model);
}*/

gboolean db_model_save(DbModel *model) {
    g_return_val_if_fail(model != NULL, FALSE);
    g_return_val_if_fail(DB_IS_MODEL(model), FALSE);
    if (model->priv->sync_model == NULL) {
        g_critical("Model type %s not found", G_OBJECT_TYPE_NAME(model));
        g_assert(model->priv->sync_model != NULL);
    }
    db_model_insert(model);
    return TRUE;
}

gboolean db_model_replace_all(DbModel *model) {
    g_return_val_if_fail(model != NULL, FALSE);
    g_return_val_if_fail(DB_IS_MODEL(model), FALSE);
    if (model->priv->sync_model == NULL) {
        GType        type = G_OBJECT_TYPE(G_OBJECT(model));
        DbDataModel *data = db_model_lookup_collectors(g_type_name(type));
        if (NULL != data) {
            db_model_set_data_model(model, data);
        }
    }
    if (model->priv->sync_model == NULL) {
        g_critical("Model type %s not found", G_OBJECT_TYPE_NAME(model));
        g_assert(model->priv->sync_model != NULL);
    }
    db_model_replace_real(model);

    return TRUE;
}

gboolean db_model_replace_valist(DbModel *model, const gchar *first_property_name, va_list var_args) {
    GString *    columns = g_string_new("");
    GString *    values  = g_string_new("");
    const gchar *name;
    name = first_property_name;
    while (name) {
        g_string_append_printf(columns, "%s%s", columns->len > 1 ? "," : "", name);
        g_string_append_printf(values, "%s$%s", values->len > 1 ? "," : "", name);
        name = va_arg(var_args, gchar *);
    }
    if (model->priv->sync_model == NULL) {
        g_critical("Model type %s not found", G_OBJECT_TYPE_NAME(model));
        g_assert(model->priv->sync_model != NULL);
    }
    const gchar *tname  = db_data_model_get_type_name(model->priv->sync_model);
    DbSqlParser *parcer = g_object_new(DB_TYPE_SQL_PARSER, "parser-name", "replace", "parser-table", tname, NULL);
    gchar *      sql    = g_strdup_printf("INSERT OR REPLACE INTO $tablename (%s) VALUES(%s);", columns->str, values->str);
    gchar *      SQL    = db_sql_parser_extended(parcer, sql, G_OBJECT(model));
    if (SQL) {
        db_connection_send_no_select(DB_CONNECTION(db_data_model_get_provider(model->priv->sync_model)), SQL);
        g_free(SQL);
    }
    g_free(sql);
    g_object_unref(parcer);
    g_string_free(columns, TRUE);
    g_string_free(values, TRUE);
    return TRUE;
}

gboolean db_model_replace(DbModel *model, const gchar *first_property_name, ...) {
    g_return_val_if_fail(model != NULL, FALSE);
    g_return_val_if_fail(DB_IS_MODEL(model), FALSE);
    if (first_property_name == NULL) {
        return db_model_replace_all(model);
    }
    if (model->priv->sync_model == NULL) {
        GType        type = G_OBJECT_TYPE(G_OBJECT(model));
        DbDataModel *data = db_model_lookup_collectors(g_type_name(type));
        if (NULL != data) {
            db_model_set_data_model(model, data);
        }
    }
    g_return_val_if_fail(model->priv->sync_model, FALSE);
    va_list var_args;
    va_start(var_args, first_property_name);
    db_model_replace_valist(model, first_property_name, var_args);
    va_end(var_args);
    return TRUE;
}

void db_model_freeze_notify(DbModel *model) {
    g_return_if_fail(model != NULL);
    g_return_if_fail(DB_IS_MODEL(model));
    model->priv->freeze_save = TRUE;
}

void db_model_thaw_notify(DbModel *model) {
    g_return_if_fail(model != NULL);
    g_return_if_fail(DB_IS_MODEL(model));
    model->priv->freeze_save = FALSE;
}

gboolean db_model_is_constructed(DbModel *model) {
    g_return_val_if_fail(model != NULL, FALSE);
    g_return_val_if_fail(DB_IS_MODEL(model), FALSE);
    return model->priv->constructed;
}

/** @} */
