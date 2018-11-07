/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * lar-mongo-database.h
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

#ifndef _LAR_MONGO_DATABASE_H_
#define _LAR_MONGO_DATABASE_H_

#include <glib-object.h>
#include <glib.h>

G_BEGIN_DECLS

#define LAR_TYPE_MONGO_DATABASE (lar_mongo_database_get_type())
#define LAR_MONGO_DATABASE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), LAR_TYPE_MONGO_DATABASE, LarMongoDatabase))
#define LAR_MONGO_DATABASE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), LAR_TYPE_MONGO_DATABASE, LarMongoDatabaseClass))
#define LAR_IS_MONGO_DATABASE(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), LAR_TYPE_MONGO_DATABASE))
#define LAR_IS_MONGO_DATABASE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), LAR_TYPE_MONGO_DATABASE))
#define LAR_MONGO_DATABASE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), LAR_TYPE_MONGO_DATABASE, LarMongoDatabaseClass))

typedef struct _LarMongoDatabaseClass   LarMongoDatabaseClass;
typedef struct _LarMongoDatabase        LarMongoDatabase;
typedef struct _LarMongoDatabasePrivate LarMongoDatabasePrivate;

struct _LarMongoDatabaseClass {
    GObjectClass parent_class;
};

struct _LarMongoDatabase {
    GObject parent_instance;

    LarMongoDatabasePrivate *priv;
};

GType lar_mongo_database_get_type(void) G_GNUC_CONST;

LarMongoDatabase *lar_mongo_database_new(const gchar *adress, guint port, const gchar *dbname);
gboolean lar_mongo_database_connect(LarMongoDatabase *database);
void lar_mongo_database_disconnect(LarMongoDatabase *database);
GObject *lar_mongo_database_find_document(LarMongoDatabase *database, const gchar *collection, const gchar *id, GType otype);
GList *lar_mongo_database_find_documents(LarMongoDatabase *database, const gchar *coll, const gchar *jq, guint skip, guint limit, GType otype);

G_END_DECLS

#endif /* _LAR_MONGO_DATABASE_H_ */
