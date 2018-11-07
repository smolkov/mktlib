/*
 * jsonreadwrite.h
 *
 *  Created on: Oct 23, 2017
 *      Author: mohsin
 */

#ifndef JSONREADWRITE_H_
#define JSONREADWRITE_H_

#include <glib.h>
#include <glib/gstdio.h>
#include <glib-object.h>
#include <json-glib/json-glib.h>

#define TYPE_JSONREADWRITE (jsonreadwrite_get_type())
#define JSONREADWRITE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_JSONREADWRITE, JsonReadWrite))
#define JSONREADWRITE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_JSONREADWRITE, JsonReadWriteClass))
#define IS_JSONREADWRITE(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_JSONREADWRITE))
#define IS_JSONREADWRITE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_JSONREADWRITE))
#define JSONREADWRITE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_JSONREADWRITE, JsonReadWriteClass))

typedef struct _JsonReadWriteClass   JsonReadWriteClass;
typedef struct _JsonReadWrite        JsonReadWrite;
typedef struct _JsonReadWritePrivate JsonReadWritePrivate;

struct _JsonReadWriteClass {
    GObjectClass parent_class;
};

struct _JsonReadWrite {
    GObject        parent_instance;
    JsonReadWritePrivate *priv;
};

GType jsonreadwrite_get_type(void) G_GNUC_CONST;

void JsonWrite(JsonReadWrite *jsonwrite, GObject *gwrite);
GObject *JsonRead(JsonReadWrite *jsonread, GType greadtype);

#endif /* JSONREADWRITE_H_ */
