/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * lar-license.c
 * Copyright (C) 2017 sascha.smolkov <saschasmolkov@saschasmolkov>
 *
 * lar-license.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lar-license.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "lar-license.h"

#include "lar-core.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>
#include <string.h>

#include <sqlite3.h>

struct _LarLicensePrivate {
    gchar *id;
    gchar *device;
    gchar *key;
    gchar *product;
    gchar *specification;
    gchar *type;
    gint64 created;
    gint64 updated;
};

#define LICENSE_DB "licenses.database"

enum { PROP_0, PROP_ID, PROP_PRODUCT, PROP_SPECIFICATION, PROP_KEY, PROP_TYPE, PROP_CREATED, PROP_UPDATED };

static JsonSerializableIface *serializable_iface = NULL;

static void json_serializable_iface_init(gpointer g_iface);

G_DEFINE_TYPE_WITH_CODE(LarLicense, lar_license, G_TYPE_OBJECT, G_IMPLEMENT_INTERFACE(JSON_TYPE_SERIALIZABLE, json_serializable_iface_init));

static JsonNode *lar_license_serialize_property(JsonSerializable *serializable, const gchar *name, const GValue *value, GParamSpec *pspec) {
    JsonNode *retval = NULL;
    g_debug("License serealize name %s.", name);
    if (g_strcmp0(name, "product") == 0 || g_strcmp0(name, "key") == 0 || g_strcmp0(name, "specification") == 0) {
        GValue copy = {
            0,
        };

        retval = json_node_new(JSON_NODE_VALUE);
        g_value_init(&copy, G_PARAM_SPEC_VALUE_TYPE(pspec));
        g_value_copy(value, &copy);
        json_node_set_value(retval, &copy);
        g_value_unset(&copy);
    } else if (g_strcmp0(name, "created") == 0 || g_strcmp0(name, "updated")) {
        GValue copy = {
            0,
        };
        retval = json_node_new(JSON_NODE_VALUE);
        g_value_init(&copy, G_TYPE_STRING);
        GTimeVal timeval = {g_value_get_int64(value) / 1000, 0};
        gchar *  tstr    = g_time_val_to_iso8601(&timeval);
        g_print("  %s:%s\n", name, tstr);
        g_value_set_string(&copy, tstr);
        json_node_set_value(retval, &copy);
        g_free(tstr);
    } else {
        retval = serializable_iface->serialize_property(serializable, name, value, pspec);
    }
    return retval;
}

gboolean json_serializable_deserialize_property(
    JsonSerializable *serializable, const gchar *property_name, GValue *value, GParamSpec *pspec, JsonNode *property_node) {
    g_debug("deserialize license property %s", property_name);
    g_return_val_if_fail(JSON_IS_SERIALIZABLE(serializable), FALSE);
    g_return_val_if_fail(property_name != NULL, FALSE);
    g_return_val_if_fail(value != NULL, FALSE);
    g_return_val_if_fail(pspec != NULL, FALSE);
    g_return_val_if_fail(property_node != NULL, FALSE);

    if (g_strcmp0(property_name, "product") == 0 || g_strcmp0(property_name, "key") == 0 || g_strcmp0(property_name, "specification") == 0) {
        g_debug("normal property");
        return serializable_iface->deserialize_property(serializable, property_name, value, pspec, property_node);
    } else if (g_strcmp0(property_name, "created") == 0 || g_strcmp0(property_name, "updated") == 0) {
        g_print("JSON Node type:");
        JsonNodeType ntype = json_node_get_node_type(property_node);
        if (ntype == JSON_NODE_ARRAY) {
            g_print("array.\n");
        } else if (ntype == JSON_NODE_OBJECT) {
            g_print("object.\n");
            JsonNode *val = json_object_get_member(json_node_get_object(property_node), "$date");
            if (val && json_node_get_node_type(val) == JSON_NODE_VALUE) {
                gint64 temp = json_node_get_int(val);
                g_debug("set value : %" G_GINT64_FORMAT, temp);
                g_value_set_int64(value, temp);
                return TRUE;
            }
        } else if (ntype == JSON_NODE_VALUE) {
            g_print("value.\n");
            GTimeVal time = {0, 0};
            if (g_time_val_from_iso8601(json_node_get_string(property_node), &time)) {
                g_value_set_int64(value, time.tv_sec * 1000);
                return TRUE;
            }
        }
        return FALSE;
    }
    return FALSE;
}

static void json_serializable_iface_init(gpointer g_iface) {
    JsonSerializableIface *iface = g_iface;

    serializable_iface          = g_type_default_interface_peek(JSON_TYPE_SERIALIZABLE);
    iface->serialize_property   = lar_license_serialize_property;
    iface->deserialize_property = json_serializable_deserialize_property;
}

static void lar_license_init(LarLicense *lar_license) {
    lar_license->priv = G_TYPE_INSTANCE_GET_PRIVATE(lar_license, LAR_TYPE_LICENSE, LarLicensePrivate);

    /* TODO: Add initialization code here */
}

static void lar_license_finalize(GObject *object) {
    LarLicense *license = LAR_LICENSE(object);
    if (license->priv->product) g_free(license->priv->product);
    if (license->priv->key) g_free(license->priv->key);
    if (license->priv->specification) g_free(license->priv->specification);

    G_OBJECT_CLASS(lar_license_parent_class)->finalize(object);
}

static void lar_license_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(LAR_IS_LICENSE(object));
    LarLicense *license = LAR_LICENSE(object);
    switch (prop_id) {
    case PROP_PRODUCT:
        if (license->priv->product) g_free(license->priv->product);
        license->priv->product = g_value_dup_string(value);
        break;
    case PROP_SPECIFICATION:
        if (license->priv->specification) g_free(license->priv->specification);
        license->priv->specification = g_value_dup_string(value);
        break;
    case PROP_KEY:
        if (license->priv->key) g_free(license->priv->key);
        license->priv->key = g_value_dup_string(value);
        break;
    case PROP_CREATED:
        g_debug("created time value  %" G_GINT64_FORMAT, g_value_get_int64(value));
        license->priv->created = g_value_get_int64(value);
        break;
    case PROP_UPDATED:
        license->priv->updated = g_value_get_int64(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_license_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(LAR_IS_LICENSE(object));
    LarLicense *license = LAR_LICENSE(object);

    switch (prop_id) {
    case PROP_PRODUCT:
        g_value_set_string(value, license->priv->product);
        break;
    case PROP_SPECIFICATION:
        g_value_set_string(value, license->priv->specification);
        break;
    case PROP_KEY:
        g_value_set_string(value, license->priv->key);
        break;
    case PROP_CREATED:
        g_value_set_int64(value, license->priv->created);
        break;
    case PROP_UPDATED:
        g_value_set_int64(value, license->priv->updated);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_license_class_init(LarLicenseClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    g_type_class_add_private(klass, sizeof(LarLicensePrivate));

    object_class->finalize     = lar_license_finalize;
    object_class->set_property = lar_license_set_property;
    object_class->get_property = lar_license_get_property;

    g_object_class_install_property(
        object_class, PROP_PRODUCT, g_param_spec_string("product", "product", "product", "none", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_SPECIFICATION,
        g_param_spec_string("specification", "specification", "specification", "none", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_KEY, g_param_spec_string("key", "key", "key", "-", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_CREATED,
        g_param_spec_int64("created", "created", "created time in msec", 0, G_MAXINT64, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_UPDATED,
        g_param_spec_int64("updated", "updated", "updated time in msec", 0, G_MAXINT64, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
}

void lar_license_print_test(LarLicense *lic) {
    g_debug("TEST Created %" G_GINT64_FORMAT, lic->priv->created);
    g_print("License:%s\n", lic->priv->key);
    GTimeVal timeval = {lic->priv->created / 1000, 0};
    // g_time_val_add(&timeval, lic->priv->created);
    gchar *tstr = g_time_val_to_iso8601(&timeval);
    g_print("  created:%s\n", tstr);
    g_free(tstr);
    timeval.tv_sec = lic->priv->updated / 1000;
    tstr           = g_time_val_to_iso8601(&timeval);
    g_print("  updated:%s\n", tstr);
    g_free(tstr);

    g_print("  product:%s\n", lic->priv->product);
    g_print("  specification:%s\n", lic->priv->specification);
}

const gchar *lar_license_get_product(LarLicense *lic) {
    g_return_val_if_fail(lic != NULL, FALSE);
    g_return_val_if_fail(LAR_IS_LICENSE(lic), FALSE);
    return lic->priv->product;
}
const gchar *lar_license_get_specification(LarLicense *lic) {
    g_return_val_if_fail(lic != NULL, FALSE);
    g_return_val_if_fail(LAR_IS_LICENSE(lic), FALSE);
    return lic->priv->specification;
}
const gchar *lar_license_get_key(LarLicense *lic) {
    g_return_val_if_fail(lic != NULL, FALSE);
    g_return_val_if_fail(LAR_IS_LICENSE(lic), FALSE);
    return lic->priv->key;
}

gboolean lar_license_commit(LarLicense *lic) {
    static gchar *dbpath = NULL;
    gboolean      res    = FALSE;
    if (dbpath == NULL) {
        dbpath = g_build_path("/", lar_core_db_path(), LICENSE_DB, NULL);
    }
    sqlite3 *sqLite = NULL;
    if (0 != sqlite3_open_v2(dbpath, &sqLite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)) {

        g_critical("SQLite-connection: connect to data base %s failed - %s", dbpath, sqlite3_errmsg(sqLite));
    } else {
        static gboolean first = TRUE;
        if (first) {
            sqlite3_exec(sqLite, "PRAGMA journal_mode=WAL", NULL, NULL, NULL);
            sqlite3_exec(sqLite, "PRAGMA synchronous=OFF", NULL, NULL, NULL);
        }

        sqlite3_close(sqLite);
    }
    return res;
}

LarLicense *lar_license_new(const gchar *product, const gchar *specification) {
    LarLicense *new = LAR_LICENSE(g_object_new(LAR_TYPE_LICENSE, "product", product, "specification", specification, NULL));
    return new;
}

static GList *lar_license_deserialize_array(JsonNode *node) {
    GList *    licenses = NULL;
    JsonArray *jarray   = json_node_get_array(node);
    GList *    la       = json_array_get_elements(jarray);
    GList *    element  = NULL;
    for (element = la; element != NULL; element = element->next) {
        if (JSON_NODE_HOLDS_OBJECT((JsonNode *)element->data)) {
            LarLicense *lic   = LAR_LICENSE(json_gobject_deserialize(LAR_TYPE_LICENSE, (JsonNode *)element->data));
            if (lic) licenses = g_list_append(licenses, lic);
        }
    }
    g_list_free(la);
    return licenses;
}

GList *lar_licensse_deserialize_list(const gchar *body) {
    GError *error    = NULL;
    GList * licenses = NULL;
    g_print("BODY:%s", body);

    JsonParser *parser = json_parser_new();
    json_parser_load_from_data(parser, body, -1, &error);
    if (error) {
        g_warning("TEST JSon Parser(%s) decode error %d - %s", g_type_name(LAR_TYPE_LICENSE), error->code, error->message);
        g_error_free(error);
    } else {
        JsonNodeType ntype = json_node_get_node_type(json_parser_get_root(parser));
        g_debug("Node type = %d", ntype);
        if (ntype == JSON_NODE_ARRAY) {
            licenses = lar_license_deserialize_array(json_parser_get_root(parser));
        } else if (ntype == JSON_NODE_OBJECT) {
            JsonNode *node = json_object_get_member(json_node_get_object(json_parser_get_root(parser)), "licenses");
            if (node) {
                licenses = lar_license_deserialize_array(json_parser_get_root(parser));
            } else {
                LarLicense *lic   = LAR_LICENSE(json_gobject_deserialize(LAR_TYPE_LICENSE, json_parser_get_root(parser)));
                if (lic) licenses = g_list_append(licenses, lic);
            }
        }
        g_object_unref(parser);
    }
    return licenses;
}
