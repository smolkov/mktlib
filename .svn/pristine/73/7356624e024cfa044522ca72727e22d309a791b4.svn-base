/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * lar-mongo-database.c
 * Copyright (C) 2017 sascha.smolkov <saschasmolkov@saschasmolkov>
 *
 * lar-mongo-database.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lar-mongo-database.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "lar-mongo-database.h"
#include <bson.h>
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>
#include <mongoc.h>

struct _LarMongoDatabasePrivate {
    guint              port;
    gchar *            adress;
    gchar *            dbname;
    mongoc_client_t *  client;
    mongoc_database_t *database;
    GHashTable *       collections;
};

enum {
    PROP_0,

    PROP_PORT,
    PROP_ADRESS,
    PROP_DBNAME,
};

G_DEFINE_TYPE(LarMongoDatabase, lar_mongo_database, G_TYPE_OBJECT);

static void lar_mongo_database_init(LarMongoDatabase *lar_mongo_database) {
    lar_mongo_database->priv              = G_TYPE_INSTANCE_GET_PRIVATE(lar_mongo_database, LAR_TYPE_MONGO_DATABASE, LarMongoDatabasePrivate);
    lar_mongo_database->priv->client      = NULL;
    lar_mongo_database->priv->database    = NULL;
    lar_mongo_database->priv->collections = NULL;
    lar_mongo_database->priv->dbname      = NULL;
    lar_mongo_database->priv->adress      = NULL;

    /* TODO: Add initialization code here */
}

static void lar_mongo_database_finalize(GObject *object) {
    LarMongoDatabase *database = LAR_MONGO_DATABASE(object);
    lar_mongo_database_disconnect(database);
    if (database->priv->adress) g_free(database->priv->adress);
    if (database->priv->dbname) g_free(database->priv->dbname);
    G_OBJECT_CLASS(lar_mongo_database_parent_class)->finalize(object);
}

static void lar_mongo_database_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(LAR_IS_MONGO_DATABASE(object));
    LarMongoDatabase *database = LAR_MONGO_DATABASE(object);
    switch (prop_id) {
    case PROP_PORT:
        database->priv->port = g_value_get_uint(value);
        break;
    case PROP_ADRESS:
        if (database->priv->adress) g_free(database->priv->adress);
        database->priv->adress = g_value_dup_string(value);
        break;
    case PROP_DBNAME:
        if (database->priv->dbname) g_free(database->priv->dbname);
        database->priv->dbname = g_value_dup_string(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_mongo_database_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(LAR_IS_MONGO_DATABASE(object));
    LarMongoDatabase *database = LAR_MONGO_DATABASE(object);
    switch (prop_id) {
    case PROP_PORT:
        g_value_set_uint(value, database->priv->port);
        break;
    case PROP_ADRESS:
        g_value_set_string(value, database->priv->adress);
        break;
    case PROP_DBNAME:
        g_value_set_string(value, database->priv->dbname);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_mongo_database_class_init(LarMongoDatabaseClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    g_type_class_add_private(klass, sizeof(LarMongoDatabasePrivate));

    object_class->finalize     = lar_mongo_database_finalize;
    object_class->set_property = lar_mongo_database_set_property;
    object_class->get_property = lar_mongo_database_get_property;

    g_object_class_install_property(object_class, PROP_PORT,
        g_param_spec_uint("port", "port", "MongoDB port", 1, G_MAXUINT32, 27017, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

    g_object_class_install_property(object_class, PROP_ADRESS,
        g_param_spec_string("adress", "adress", "MongoDB adress", "localhost", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
    g_object_class_install_property(object_class, PROP_DBNAME,
        g_param_spec_string("db-name", "db-name", "MongoDB name", "licenses", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

LarMongoDatabase *lar_mongo_database_new(const gchar *adress, guint port, const gchar *dbname) {
    LarMongoDatabase *database = g_object_new(LAR_TYPE_MONGO_DATABASE, "port", port, "adress", adress, "db-name", dbname, NULL);
    lar_mongo_database_connect(database);
    return database;
}

void lar_mongo_database_disconnect(LarMongoDatabase *database) {
    if (database->priv->collections) {
        g_hash_table_destroy(database->priv->collections);
        database->priv->collections = NULL;
    }
    if (database->priv->database) {
        mongoc_database_destroy(database->priv->database);
        database->priv->database = NULL;
    }
    if (database->priv->client) {
        mongoc_client_destroy(database->priv->client);
        database->priv->client = NULL;
    }
}

gboolean lar_mongo_database_connect(LarMongoDatabase *database) {
    lar_mongo_database_disconnect(database);
    gchar *ccna            = g_strdup_printf("mongodb://%s:%d", database->priv->adress, database->priv->port);
    database->priv->client = mongoc_client_new(ccna);
    g_return_val_if_fail(database->priv->client != NULL, FALSE);
    database->priv->database = mongoc_client_get_database(database->priv->client, database->priv->dbname);
    g_return_val_if_fail(database->priv->database != NULL, FALSE);
    return TRUE;
}
GObject *lar_mongo_database_find_document(LarMongoDatabase *database, const gchar *coll, const gchar *id, GType otype) {
    bson_oid_t           oid;
    mongoc_collection_t *collection;
    bson_t *             query = NULL;
    mongoc_cursor_t *    cursor;
    GObject *            object = NULL;

    if (!bson_oid_is_valid(id, sizeof id)) {
        g_warning("Invalid ObjectId: %s", id);
        return NULL;
    }

    collection = mongoc_client_get_collection(database->priv->client, database->priv->dbname, coll);
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
    return object;
}
#include "lar-license.h"
GList *lar_mongo_database_find_documents(LarMongoDatabase *database, const gchar *coll, const gchar *q, guint skip, guint limit, GType otype) {
    // bson_oid_t           oid;
    mongoc_collection_t *collection;
    bson_t *             query = NULL;
    mongoc_cursor_t *    cursor;
    bson_error_t         berror;
    GList *              objects = NULL;
    query                        = bson_new_from_json((const uint8_t *)q, -1, &berror);
    if (query == NULL) {
        g_warning("bosn query from json error %d - %s", berror.code, berror.message);
        return NULL;
    }
    collection = mongoc_client_get_collection(database->priv->client, database->priv->dbname, coll);

    // bson_oid_init_from_string(&oid, id);
    // query  = BCON_NEW("_id", BCON_OID(&oid));
    cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, skip, limit, 0, query, NULL, NULL);
    const bson_t *doc;
    while (mongoc_cursor_next(cursor, &doc)) {

        gchar *str = bson_as_json(doc, NULL);
        printf("%s\n", str);

        GError *error = NULL;

        error              = NULL;
        JsonParser *parser = json_parser_new();
        json_parser_load_from_data(parser, str, -1, &error);
        if (error) {
            g_warning("TEST JSon Parser(%s) decode error %d - %s", g_type_name(otype), error->code, error->message);
            g_error_free(error);

        } else {
            JsonNode *jnode  = json_parser_get_root(parser);
            GObject * object = json_gobject_deserialize(otype, jnode);
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
    return objects;
}
