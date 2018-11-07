/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * lar-mongo-pool.c
 * Copyright (C) 2017 sascha.smolkov <saschasmolkov@saschasmolkov>
 *
 * lar-mongo-pool.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lar-mongo-pool.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "lar-mongo-pool.h"
#include <bson.h>
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>
#include <mongoc.h>

#define LAR_TYPE_MONGO_POOL (lar_mongo_pool_get_type())
#define LAR_MONGO_POOL(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), LAR_TYPE_MONGO_POOL, LarMongoPool))
#define LAR_MONGO_POOL_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), LAR_TYPE_MONGO_POOL, LarMongoPoolClass))
#define LAR_IS_MONGO_POOL(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), LAR_TYPE_MONGO_POOL))
#define LAR_IS_MONGO_POOL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), LAR_TYPE_MONGO_POOL))
#define LAR_MONGO_POOL_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), LAR_TYPE_MONGO_POOL, LarMongoPoolClass))

typedef struct _LarMongoPoolClass   LarMongoPoolClass;
typedef struct _LarMongoPool        LarMongoPool;
typedef struct _LarMongoPoolPrivate LarMongoPoolPrivate;

struct _LarMongoPoolClass {
    GObjectClass parent_class;
};

struct _LarMongoPool {
    GObject parent_instance;

    LarMongoPoolPrivate *priv;
};

GType lar_mongo_pool_get_type(void) G_GNUC_CONST;

struct _LarMongoPoolPrivate {
    guint                 port;
    gchar *               adress;
    gchar *               dbname;
    gchar *               app;
    mongoc_client_pool_t *pool;
};

enum {
    PROP_0,
    PROP_PORT,
    PROP_ADRESS,
    PROP_DBNAME,
    PROP_APP,
};

#if GLIB_CHECK_VERSION(2, 31, 7)
static GRecMutex init_rmutex;
#define MUTEX_LOCK() g_rec_mutex_lock(&init_rmutex)
#define MUTEX_UNLOCK() g_rec_mutex_unlock(&init_rmutex)
#else
static GStaticRecMutex init_mutex = G_STATIC_REC_MUTEX_INIT;
#define MUTEX_LOCK() g_static_rec_mutex_lock(&init_mutex)
#define MUTEX_UNLOCK() g_static_rec_mutex_unlock(&init_mutex)
#endif

static LarMongoPool *LAR_POOL = NULL;

G_DEFINE_TYPE(LarMongoPool, lar_mongo_pool, G_TYPE_OBJECT);
static void lar_mongo_pool_disconnect(LarMongoPool *pool) { mongoc_client_pool_destroy(pool->priv->pool); }

static void lar_mongo_pool_init(LarMongoPool *lar_mongo_pool) {
    lar_mongo_pool->priv         = G_TYPE_INSTANCE_GET_PRIVATE(lar_mongo_pool, LAR_TYPE_MONGO_POOL, LarMongoPoolPrivate);
    lar_mongo_pool->priv->pool   = NULL;
    lar_mongo_pool->priv->dbname = NULL;
    lar_mongo_pool->priv->adress = NULL;
    lar_mongo_pool->priv->app    = NULL;

    /* TODO: Add initialization code here */
}

static void lar_mongo_pool_finalize(GObject *object) {
    LarMongoPool *pool = LAR_MONGO_POOL(object);
    lar_mongo_pool_disconnect(pool);
    if (pool->priv->adress) g_free(pool->priv->adress);
    if (pool->priv->dbname) g_free(pool->priv->dbname);
    G_OBJECT_CLASS(lar_mongo_pool_parent_class)->finalize(object);
}

static void lar_mongo_pool_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(LAR_IS_MONGO_POOL(object));
    LarMongoPool *pool = LAR_MONGO_POOL(object);
    switch (prop_id) {
    case PROP_PORT:
        pool->priv->port = g_value_get_uint(value);
        break;
    case PROP_ADRESS:
        if (pool->priv->adress) g_free(pool->priv->adress);
        pool->priv->adress = g_value_dup_string(value);
        break;
    case PROP_DBNAME:
        if (pool->priv->dbname) g_free(pool->priv->dbname);
        pool->priv->dbname = g_value_dup_string(value);
        break;
    case PROP_APP:
        if (pool->priv->app) g_free(pool->priv->app);
        pool->priv->app = g_value_dup_string(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_mongo_pool_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(LAR_IS_MONGO_POOL(object));
    LarMongoPool *pool = LAR_MONGO_POOL(object);
    switch (prop_id) {
    case PROP_PORT:
        g_value_set_uint(value, pool->priv->port);
        break;
    case PROP_ADRESS:
        g_value_set_string(value, pool->priv->adress);
        break;
    case PROP_DBNAME:
        g_value_set_string(value, pool->priv->dbname);
        break;
    case PROP_APP:
        g_value_set_string(value, pool->priv->app);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_mongo_pool_class_init(LarMongoPoolClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    g_type_class_add_private(klass, sizeof(LarMongoPoolPrivate));

    object_class->finalize     = lar_mongo_pool_finalize;
    object_class->set_property = lar_mongo_pool_set_property;
    object_class->get_property = lar_mongo_pool_get_property;

    g_object_class_install_property(object_class, PROP_PORT,
        g_param_spec_uint("port", "port", "MongoDB port", 1, G_MAXUINT32, 27017, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

    g_object_class_install_property(object_class, PROP_ADRESS,
        g_param_spec_string("adress", "adress", "MongoDB adress", "localhost", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
    g_object_class_install_property(object_class, PROP_DBNAME,
        g_param_spec_string("db-name", "db-name", "MongoDB name", "licenses", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
    g_object_class_install_property(object_class, PROP_APP,
        g_param_spec_string("app-name", "app-name", "connection app name", "app", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

static gboolean lar_mongo_pool_connect(LarMongoPool *pool) {
    mongoc_uri_t *uri;

    lar_mongo_pool_disconnect(pool);
    gchar *uristr = g_strdup_printf("mongodb://%s:%d/?appname=%s", pool->priv->adress, pool->priv->port, pool->priv->app);
    uri           = mongoc_uri_new(uristr);
    if (!uri) {
        // g_error("Failed to parse URI: \"%s\".\n", uristr);
        return FALSE;
    }

    pool->priv->pool = mongoc_client_pool_new(uri);

    return TRUE;
}

void lar_mongo_pool_initialize(const gchar *adress, guint port, const gchar *dbname, const gchar *app) {
    if (LAR_POOL != NULL) {
        g_warning("Lar mongo DB pool alredy initialized");
        return;
    }
    LarMongoPool *pool = g_object_new(LAR_TYPE_MONGO_POOL, "port", port, "adress", adress, "db-name", dbname, "app-name", app, NULL);
    lar_mongo_pool_connect(pool);
}

GObject *lar_mongo_pool_find_document(const gchar *collname, const gchar *id, GType otype) {
    g_return_val_if_fail(LAR_POOL != NULL, NULL);
    bson_oid_t           oid;
    mongoc_collection_t *collection;
    bson_t *             query = NULL;
    mongoc_cursor_t *    cursor;
    GObject *            object = NULL;
    mongoc_client_t *    client;

    MUTEX_LOCK();
    client = mongoc_client_pool_pop(LAR_POOL->priv->pool);
    if (!bson_oid_is_valid(id, sizeof id)) {
        g_warning("Invalid ObjectId: %s", id);
        return NULL;
    }

    collection = mongoc_client_get_collection(client, LAR_POOL->priv->dbname, collname);
    bson_oid_init_from_string(&oid, id);
    query  = BCON_NEW("_id", BCON_OID(&oid));
    cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
    if (cursor) {
        const bson_t *doc;
        if (mongoc_cursor_next(cursor, &doc)) {
            gchar *str = bson_as_json(doc, NULL);
            printf("%s\n", str);
            bson_free(str);
        }
    }

    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
    mongoc_client_pool_push(LAR_POOL->priv->pool, client);
    MUTEX_UNLOCK();

    return object;
}
GList *lar_mongo_pool_find_licenses(const gchar *jq, guint skip, guint limit) {
    g_return_val_if_fail(LAR_POOL != NULL, NULL);

    mongoc_collection_t *collection;
    bson_t *             query = NULL;
    mongoc_cursor_t *    cursor;
    bson_error_t         berror;
    GList *              objects = NULL;
    mongoc_client_t *    client;

    MUTEX_LOCK();
    client = mongoc_client_pool_pop(LAR_POOL->priv->pool);
    query  = bson_new_from_json((const uint8_t *)jq, -1, &berror);
    if (query == NULL) {
        g_warning("bosn query from json error %d - %s", berror.code, berror.message);
        return NULL;
    }
    collection = mongoc_client_get_collection(client, LAR_POOL->priv->dbname, "licenses");
    cursor     = mongoc_collection_find(collection, MONGOC_QUERY_NONE, skip, limit, 0, query, NULL, NULL);
    const bson_t *doc;
    while (mongoc_cursor_next(cursor, &doc)) {

        gchar *str = bson_as_json(doc, NULL);
        printf("%s\n", str);

        GError *error = NULL;

        error              = NULL;
        JsonParser *parser = json_parser_new();
        json_parser_load_from_data(parser, str, -1, &error);
        if (error) {
            g_warning("TEST JSon Parser(%s) decode error %d - %s", g_type_name(LAR_TYPE_LICENSE), error->code, error->message);
            g_error_free(error);

        } else {
            JsonNode *jnode  = json_parser_get_root(parser);
            GObject * object = json_gobject_deserialize(LAR_TYPE_LICENSE, jnode);
            if (object == NULL) {
                g_warning("Deserialize is failed");

            } else {
                objects = g_list_append(objects, object);
            }
        }

        bson_free(str);
    }
    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
    mongoc_client_pool_push(LAR_POOL->priv->pool, client);
    MUTEX_UNLOCK();
    return objects;
}
