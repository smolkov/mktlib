/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * jsoncodec.c
 * Copyright (C) LAR 2017
 *

 */

#include "jsoncodec.h"

G_DEFINE_INTERFACE(Jsoncodec, jsoncodec, G_TYPE_OBJECT)

static void jsoncodec_default_init(JsoncodecInterface *iface) { /* add properties and signals to the interface here */
}

JsonNode *JsoncodecSerialize(Jsoncodec *jsoncodec, GObject *object) {
    JsoncodeceInterface *iface;

    g_return_if_fail(VIEWER_IS_EDITABLE(jsoncodec));

    iface = JSONCODEC_GET_INTERFACE(self);
    g_return_val_if_fail(iface->serialize != NULL, NULL);
    return iface->serialize(jsoncodec, object);
}
GObject *JsoncodecDeserialize(Jsoncodec *jsoncodec, JsonNode *node) {
    JsoncodeceInterface *iface;

    g_return_if_fail(VIEWER_IS_EDITABLE(jsoncodec));

    iface = JSONCODEC_GET_INTERFACE(self);
    g_return_val_if_fail(iface->deserialize != NULL, NULL);
   return  iface->deserialize(jsoncodec,node);
}


gboolean JsonbuilderAppendFundamentalProperties(JsonBuilder *builder,GObject *object){
    g_return_val_if_fail(builder!= NULL ,FALSE);
    g_return_val_if_fail(object!= NULL ,FALSE);
    GParamSpec **pspec  = g_object_class_list_properties(G_OBJECT_GET_CLASS(object), &len);
    int          i      = 0;
    for (i = 0; i < len; i++) {
        if(G_TYPE_IS_FUNDAMENTAL(pspec[i]->value_type)){
            json_builder_set_member_name (builder, pspec[i]);
            GValue *val = value_new(pspec->value_type);
            g_object_get_property(object, pspec->name, val);
            // JsonNode *member = json_node_new(JSON_)
            json_builder_add_value(builder,val);
            value_free(val);
        }
    }
    return TRUE
}

struct _JsonMeasPrivate {
};

#define JSONMEAS_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_JSONMEAS, JsonMeasPrivate))


static void jsonmeas_codec_iface_init(gpointer g_iface);

G_DEFINE_TYPE_WITH_CODE(JsonMeas, jsonmeas, G_TYPE_OBJECT, G_IMPLEMENT_INTERFACE(TYPE_JSONCODEC, jsonmeas_codec_iface_init));



static JsonNode *jsonmeas_serialize(Jsoncodec *jsoncodec, GObject *object) {
    JsonBuilder *builder = json_builder_new ();
    JsonNode *node;
    JsonGenerator *generator;
    json_builder_begin_object (builder);
    JsonbuilderAppendFundamentalProperties(builder,object);


    return stream_serializable_iface->serialize_property(serializable, name, value, pspec);
}

static GObject* jsonmeas_deserialize(Jsoncodec *jsoncodec, JsonNode *node , GType type) {
    
}

static void jsonmeas_codec_iface_init(gpointer g_iface) {
    JsoncodecInterface *iface = g_iface;
    iface->serialize    = stream_serialize_property;
    iface->deserialize_property  = stream_deserialize_property;
}


static void jsonmeas_init(JsonMeas *jsonmeas) {
    JsonMeasPrivate *priv = JSONMEAS_PRIVATE(jsonmeas);
    priv->min          = 0.;
    priv->max          = 100.;
    priv->activated    = FALSE;
    jsonmeas->priv        = priv;

    /* TODO: Add initialization code here */
}

static void jsonmeas_finalize(GObject *object) {
    // JsonMeas *jsonmeas = JSONMEAS(object);

    G_OBJECT_CLASS(jsonmeas_parent_class)->finalize(object);
}


static void jsonmeas_class_init(JsonMeasClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // ModelClass* parent_class = MODEL_CLASS (klass);
    g_type_class_add_private(klass, sizeof(JsonMeasPrivate));
    object_class->finalize     = jsonmeas_finalize;
    // object_class->set_property = jsonmeas_set_property;
    // object_class->get_property = jsonmeas_get_property;

}



