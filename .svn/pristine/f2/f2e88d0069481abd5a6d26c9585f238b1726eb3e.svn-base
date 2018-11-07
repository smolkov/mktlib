/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * lnosql.h
 * Copyright (C) LAR 2017
 *

 */

#ifndef _NOSQL_CONNECTION_H_
#define _NOSQL_CONNECTION_H_

#include <glib-object.h>
#include <nosql3.h>

G_BEGIN_DECLS

#define TYPE_NOSQL_CONNECTION (nosql_connection_get_type())
#define NOSQL_CONNECTION(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_NOSQL_CONNECTION, NosqlCollection))
#define NOSQL_CONNECTION_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_NOSQL_CONNECTION, NosqlCollectionClass))
#define IS_NOSQL_CONNECTION(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_NOSQL_CONNECTION))
#define IS_NOSQL_CONNECTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_NOSQL_CONNECTION))
#define NOSQL_CONNECTION_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_NOSQL_CONNECTION, NosqlCollectionClass))

typedef struct _NosqlCollectionClass   NosqlCollectionClass;
typedef struct _NosqlCollection        NosqlCollection;
typedef struct _NosqlCollectionPrivate NosqlCollectionPrivate;


struct _NosqlCollectionClass {
    GObjectClass parent_class;
};

struct _NosqlCollection {
    GObject                  parent_instance;
    NosqlCollectionPrivate *priv;
};

GType nosql_connection_get_type(void);

NosqlCollection *NosqlCollectionNew(const gchar *database, const gchar *collection);

// guint64 NosqlInsertObject(NosqlCollection *connection, const gchar *table, GObject *object);
// GSList *NosqlSelectObjects(NosqlCollection *connection,GType type, const gchar *format, ...) G_GNUC_PRINTF(3, 4);




// gboolean NosqlInit

//
// gchar *TimeNowNosqlFormatUTC();
// gchar *TimeNowNosqlFormat();

G_END_DECLS

#endif /* _NOSQL_CONNECTION_H_ */
