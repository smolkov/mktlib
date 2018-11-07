/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * jsoncodec.h
 * Copyright (C) LAR 2017
 *

 */

#ifndef _JSONCODEC_H_
#define _JSONCODEC_H_


#include <glib-object.h>
#include <json-glib/json-glib.h>


G_BEGIN_DECLS

#define TYPE_JSONCODEC                 (jsoncodec_get_type ())
#define JSONCODEC(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_JSONCODEC, Jsoncodec))
#define IS_JSONCODEC(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_JSONCODEC))
#define JSONCODEC_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), TYPE_JSONCODEC, JsoncodecInterface))



typedef struct _Jsoncodec               Jsoncodec; /* dummy object */
typedef struct _JsoncodecInterface      JsoncodecInterface;


enum {
	JSONCODEC_UNKNOWN,
	JSONCODEC_ON_CHANNEL,
	JSONCODEC_LAR_MEASUREMENT_SWITCH,
};

struct _JsoncodecInterface
{
  GTypeInterface     parent_iface;
  JsonNode *       (*serialize)                       ( Jsoncodec *self, GObject *object );
  GObject*         (*deserialize)                     ( Jsoncodec *self, JsonNode *node, GType type );

};


GType            jsoncodec_get_type                       ( void );

JsonNode*        JsoncodecSerialize                       ( Jsoncodec *jsoncodec, GObject *object );
GObject*         JsoncodecDeserialize                     ( Jsoncodec *jsoncodec, JsonNode *node );

//LarMeasurement object decoder 
#define TYPE_JSONMEAS             (jsonmeas_get_type ())
#define JSONMEAS(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_JSONMEAS, JsonMeas))
#define JSONMEAS_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_JSONMEAS, JsonMeasClass))
#define IS_JSONMEAS(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_JSONMEAS))
#define IS_JSONMEAS_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_JSONMEAS))
#define JSONMEAS_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_JSONMEAS, JsonMeasClass))

typedef struct _JsonMeasClass   JsonMeasClass;
typedef struct _JsonMeas        JsonMeas;
typedef struct _JsonMeasPrivate JsonMeasPrivate;


struct _JsonMeasClass
{
	GObjectClass parent_class;
};

struct _JsonMeas
{
	GObject parent_instance;
	JsonMeasPrivate *priv;
};

GType                   jsonmeas_get_type                 (void) G_GNUC_CONST;


JsonNode * JsonMarschal(GObject *object);
GObject  * JsonUnmarschal(JsonNode *node, GType type );

G_END_DECLS


#endif /* _JSONCODEC_H_ */
