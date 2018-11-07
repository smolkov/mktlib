/*
 * jsonreadwrite.c
 *
 *  Created on: Oct 23, 2017
 *      Author: mohsin
 */

#include "jsonreadwrite.h"

enum { PROP_0, PROP_FILEPATH};

struct _JsonReadWritePrivate {
    gchar	*filepath;
};

#define JSONREADWRITE_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), TYPE_JSONREADWRITE, JsonReadWritePrivate))

G_DEFINE_TYPE(JsonReadWrite, jsonreadwrite, G_TYPE_OBJECT)

static void jsonreadwrite_init(JsonReadWrite *jsonreadwrite) {
    JsonReadWritePrivate *priv	= JSONREADWRITE_PRIVATE(jsonreadwrite);
    priv->filepath				= g_strdup("file.json");
    jsonreadwrite->priv			= priv;
    /* TODO: Add initialization code here */
}

static void jsonreadwrite_finalize(GObject *object) {
	JsonReadWrite *jsonrw = JSONREADWRITE(object);

	if (jsonrw->priv->filepath) g_free(jsonrw->priv->filepath);

    G_OBJECT_CLASS(jsonreadwrite_parent_class)->finalize(object);
}

static void jsonreadwrite_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_JSONREADWRITE(object));
    JsonReadWrite *jsonreadwrite = JSONREADWRITE(object);
    switch (prop_id) {
    case PROP_FILEPATH:
        if (jsonreadwrite->priv->filepath) g_free(jsonreadwrite->priv->filepath);
        jsonreadwrite->priv->filepath = g_value_dup_string(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void jsonreadwrite_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_JSONREADWRITE(object));
    JsonReadWrite *jsonreadwrite = JSONREADWRITE(object);
    switch (prop_id) {
    case PROP_FILEPATH:
        g_value_set_string(value, jsonreadwrite->priv->filepath);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void jsonreadwrite_class_init(JsonReadWriteClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    g_type_class_add_private(klass, sizeof(JsonReadWritePrivate));
    object_class->finalize     = jsonreadwrite_finalize;
    object_class->set_property = jsonreadwrite_set_property;
    object_class->get_property = jsonreadwrite_get_property;

    g_object_class_install_property(object_class, PROP_FILEPATH, g_param_spec_string("filepath", "file path", "json file path", "file.json", G_PARAM_READWRITE | G_PARAM_CONSTRUCT));
}

void JsonWrite(JsonReadWrite *jsonwrite, GObject *gwrite)
{
    GError *tmp_error = NULL;

	JsonGenerator *gen = json_generator_new();
	JsonNode *root = json_node_new(JSON_NODE_OBJECT);

	JsonNode *node = json_gobject_serialize(gwrite);
	JsonObject *jobject = json_node_get_object (node);

	json_node_set_object (root,jobject);
	json_generator_set_root (gen, root);

	if (!json_generator_to_file(gen, jsonwrite->priv->filepath, &tmp_error)) {
		return;
	}

	json_node_free(root);
	g_object_unref(gen);
	//json_object_unref(jobject);
	json_node_free(node);
}

GObject *JsonRead(JsonReadWrite *jsonread, GType greadtype)
{
	GObject *gread = NULL;
	GError *tmp_error = NULL;

	JsonParser *json_parser = json_parser_new();

	if (!json_parser_load_from_file(json_parser, jsonread->priv->filepath, &tmp_error)) {
		return NULL;
	}

	JsonNode *root = json_parser_get_root(json_parser);

	gread = json_gobject_deserialize (greadtype, root);

	g_object_unref(json_parser);

	return gread;
}
