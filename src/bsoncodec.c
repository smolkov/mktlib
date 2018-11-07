/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * bsoncodec.c
 * Copyright (C) LAR 2017
 *

 */

#include "bsoncodec.h"

G_DEFINE_INTERFACE(Bsoncodec, bsoncodec, G_TYPE_OBJECT)

static void bsoncodec_default_init(BsoncodecInterface *iface) { /* add properties and signals to the interface here */
}

bson *BsoncodecSerialize(Bsoncodec *bsoncodec, GObject *object) {
    BsoncodeceInterface *iface;

    g_return_if_fail(VIEWER_IS_EDITABLE(bsoncodec));

    iface = BSONCODEC_GET_INTERFACE(self);
    g_return_val_if_fail(iface->serialize != NULL, NULL);
    return iface->serialize(bsoncodec, object);
}
GObject *BsoncodecDeserialize(Bsoncodec *bsoncodec, bson *bs) {
    BsoncodeceInterface *iface;

    g_return_if_fail(VIEWER_IS_EDITABLE(bsoncodec));
    // bson_
    iface = BSONCODEC_GET_INTERFACE(self);
    g_return_val_if_fail(iface->deserialize != NULL, NULL);
   return  iface->deserialize(bsoncodec,node);
}
