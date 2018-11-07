/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * bsoncodec.h
 * Copyright (C) LAR 2017
 *

 */

#ifndef _BSONCODEC_H_
#define _BSONCODEC_H_


#include <glib-object.h>
#include <ejdb/ejdb.h>


G_BEGIN_DECLS

#define TYPE_BSONCODEC                 (bsoncodec_get_type ())
#define BSONCODEC(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_BSONCODEC, Bsoncodec))
#define IS_BSONCODEC(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_BSONCODEC))
#define BSONCODEC_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), TYPE_BSONCODEC, BsoncodecInterface))



typedef struct _Bsoncodec               Bsoncodec; /* dummy object */
typedef struct _BsoncodecInterface      BsoncodecInterface;


enum {
	BSONCODEC_UNKNOWN,
	BSONCODEC_ON_CHANNEL,
	BSONCODEC_LAR_MEASUREMENT_SWITCH,
};

struct _BsoncodecInterface
{
  GTypeInterface     parent_iface;
  BsonNode *       (*serialize)                       ( Bsoncodec *self );
  GValue*          (*deserialize)                     ( Bsoncodec *self );

};


GType            bsoncodec_get_type                       ( void );

BsonNode*        BsoncodecSerialize                       ( Bsoncodec *bsoncodec, GObject *object );
GObject*         BsoncodecDeserialize                     ( Bsoncodec *bsoncodec, BsonNode *node );





G_END_DECLS


#endif /* _BSONCODEC_H_ */
