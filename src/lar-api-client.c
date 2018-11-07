/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * lar-mongo-api_client.c
 * Copyright (C) 2017 sascha.smolkov <saschasmolkov@saschasmolkov>
 *
 * lar-mongo-api_client.c is free software: you can redistribute it and/or
 * modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lar-mongo-api_client.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "lar-api-client.h"
#include "lar-device.h"
#include "lar-license.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>
#include <libsoup/soup.h>
#include <string.h>

struct _LarApiClientPrivate {
    guint         port;
    gchar *       adress;
    LarDevice *   device;
    GList *       licenses;
    SoupMessage * current;
    gboolean      connection_fail;
    gboolean      connected;
    gboolean      is_exist;
    gboolean      busy;
    GCancellable *operation_cancellable;
    guint         lic_counter;
};

struct _LarApiClientClass {
    SoupSessionClass parent_class;
};

struct _LarApiClient {
    SoupSession          parent_instance;
    LarApiClientPrivate *priv;
};

static SoupMessage *lar_api_get_device_message(LarApiClient *api, const gchar *message);
static SoupMessage *lar_api_get_licenses_message(LarApiClient *api);
// static SoupMessage *lar_api_get_licenses_product_message(LarApiClient *api, const gchar *product);
static SoupMessage *lar_api_create_device_message(LarApiClient *api, LarDevice *device);
static SoupMessage *lar_api_create_license_message(LarApiClient *api, LarLicense *license);

enum {
    PROP_0,
    PROP_PORT,
    PROP_ADRESS,
    PROP_SERIAL,
    PROP_CONNECTION_FAILED,
    PROP_CONNECTED,
    PROP_BUSY,
    PROP_LIC_COUNTER,

};

G_DEFINE_TYPE(LarApiClient, lar_api_client, SOUP_TYPE_SESSION);

static void lar_api_client_init(LarApiClient *api) {
    api->priv                        = G_TYPE_INSTANCE_GET_PRIVATE(api, LAR_TYPE_API_CLIENT, LarApiClientPrivate);
    api->priv->adress                = g_strdup("http://localhost");
    api->priv->port                  = 3001;
    api->priv->device                = NULL;
    api->priv->licenses              = NULL;
    api->priv->busy                  = FALSE;
    api->priv->connected             = FALSE;
    api->priv->operation_cancellable = FALSE;
    api->priv->connection_fail       = FALSE;
    api->priv->lic_counter           = 0;
}

static void lar_api_client_clean(LarApiClient *api) {
    lar_api_client_operation_cancel(api);
    if (api->priv->device) g_object_unref(api->priv->device);
    if (api->priv->licenses) g_list_free_full(api->priv->licenses, g_object_unref);
    api->priv->device   = NULL;
    api->priv->licenses = NULL;
    if (api->priv->connected) g_object_set(api, "connected", FALSE, NULL);
}

static void lar_api_client_finalize(GObject *object) {
    LarApiClient *api_client = LAR_API_CLIENT(object);
    if (api_client->priv->adress) g_free(api_client->priv->adress);

    G_OBJECT_CLASS(lar_api_client_parent_class)->finalize(object);
}
static void lar_api_client_dispose(GObject *gobject) {
    LarApiClient *api = LAR_API_CLIENT(gobject);
    lar_api_client_clean(api);
    G_OBJECT_CLASS(gobject)->dispose(gobject);
}

static void lar_api_client_constructed(GObject *object) {
    // LarApiClient *api_client = LAR_API_CLIENT(object);

    G_OBJECT_CLASS(lar_api_client_parent_class)->constructed(object);
}

static void lar_api_client_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(LAR_IS_API_CLIENT(object));
    LarApiClient *api_client = LAR_API_CLIENT(object);
    switch (prop_id) {
    case PROP_PORT:
        api_client->priv->port = g_value_get_uint(value);
        break;
    case PROP_ADRESS:
        if (api_client->priv->adress) g_free(api_client->priv->adress);
        api_client->priv->adress = g_value_dup_string(value);
        break;
    case PROP_BUSY:
        api_client->priv->busy = g_value_get_boolean(value);
        break;
    case PROP_CONNECTED:
        api_client->priv->connected = g_value_get_boolean(value);
        break;
    case PROP_CONNECTION_FAILED:
        api_client->priv->connection_fail = g_value_get_boolean(value);
        break;
    case PROP_LIC_COUNTER:
        api_client->priv->lic_counter = g_value_get_uint(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_api_client_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(LAR_IS_API_CLIENT(object));
    LarApiClient *api_client = LAR_API_CLIENT(object);
    switch (prop_id) {
    case PROP_PORT:
        g_value_set_uint(value, api_client->priv->port);
        break;
    case PROP_ADRESS:
        g_value_set_string(value, api_client->priv->adress);
        break;
    case PROP_BUSY:
        g_value_set_boolean(value, api_client->priv->busy);
        break;
    case PROP_CONNECTED:
        g_value_set_boolean(value, api_client->priv->connected);
        break;
    case PROP_CONNECTION_FAILED:
        g_value_set_boolean(value, api_client->priv->connection_fail);
        break;
    case PROP_LIC_COUNTER:
        g_value_set_uint(value, api_client->priv->lic_counter);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_api_client_class_init(LarApiClientClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    g_type_class_add_private(klass, sizeof(LarApiClientPrivate));

    object_class->finalize    = lar_api_client_finalize;
    object_class->constructed = lar_api_client_constructed;
    object_class->dispose     = lar_api_client_dispose;

    object_class->set_property = lar_api_client_set_property;
    object_class->get_property = lar_api_client_get_property;

    g_object_class_install_property(
        object_class, PROP_PORT, g_param_spec_uint("port", "server port", "api server port", 1, G_MAXUINT32, 3001, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

    g_object_class_install_property(
        object_class, PROP_ADRESS, g_param_spec_string("adress", "server adress", "server adress", "localhost", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

    g_object_class_install_property(object_class, PROP_BUSY, g_param_spec_boolean("busy", "busy", "busy", FALSE, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_CONNECTED, g_param_spec_boolean("connected", "connected", "connected", FALSE, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(
        object_class, PROP_CONNECTION_FAILED, g_param_spec_boolean("connection-failed", "connection is failed", "connection is failed", FALSE, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(
        object_class, PROP_LIC_COUNTER, g_param_spec_uint("licenses", "licenses counter", "License counter on the server", 0, G_MAXUINT32, 0, G_PARAM_READABLE | G_PARAM_WRITABLE));
}
/**
 * lar_api_client_new:
 * @adress: server adress
 * @stream: server port
 *
 * Api function to create new client session
 * Return value: new LarApiClient object if sucefull
 */
LarApiClient *lar_api_client_new(const gchar *adress, guint port) {
    LarApiClient *api_client = g_object_new(LAR_TYPE_API_CLIENT, "use-thread-context", TRUE, "port", port, "adress", adress, NULL);
    return api_client;
}
/**
 * lar_api_clientt_is_busy:
 * @api: LarApiClient object
 *
 * Api function to create new client session
 * Return value: TRUE if LarApiClient ist busy
 */
gboolean lar_api_client_is_busy(LarApiClient *api) {
    g_return_val_if_fail(api != NULL, FALSE);
    g_return_val_if_fail(LAR_IS_API_CLIENT(api), FALSE);
    return api->priv->busy;
}
void lar_api_client_operation_cancel(LarApiClient *api) {
    g_return_if_fail(api != NULL);
    g_return_if_fail(LAR_IS_API_CLIENT(api));
    if (api->priv->operation_cancellable) {
        g_cancellable_cancel(api->priv->operation_cancellable);
        g_object_unref(api->priv->operation_cancellable);
        api->priv->operation_cancellable = NULL;
    }
    api->priv->operation_cancellable = g_cancellable_new();
}

/**
 * lar_device_from_json_body_stream:
 * @stream: GInputStream json text format to deserialize
 *
 * internal function to deserialize a input stream to LarDevice ibject
 * Return value: new LarDevice object if sucefull
 */
static LarDevice *lar_device_from_json_body_stream(GInputStream *stream) {
    gchar      body[4095];
    GError *   error  = NULL;
    LarDevice *device = NULL;
    memset(body, 0, 4095);
    g_input_stream_read(stream, body, 4095, NULL, &error);
    g_print("BODY:%s", body);
    JsonParser *parser = json_parser_new();
    json_parser_load_from_data(parser, body, -1, &error);
    if (error) {
        g_warning("TEST JSon Parser(%s) decode error %d - %s", g_type_name(LAR_TYPE_DEVICE), error->code, error->message);
        g_error_free(error);

    } else {
        JsonNode *jnode = json_parser_get_root(parser);
        device          = LAR_DEVICE(json_gobject_deserialize(LAR_TYPE_DEVICE, jnode));
        if (device == NULL) {
            g_warning("Deserialize device failed");
        }
    }
    g_object_unref(parser);
    return device;
}

/**
 * lar_liceses_from_json_body_stream:
 * @stream: GInputStream json text format to deserialize
 *
 * internal function to deserialize a input stream to LarLicense ibject
 * Return value: new GList
 */

static GList *lar_liceses_from_json_body_stream(GInputStream *stream) {
#define LIC_BODY 8190
    gchar   body[LIC_BODY];
    GError *error = NULL;
    memset(body, 0, LIC_BODY);
    g_input_stream_read(stream, body, LIC_BODY, NULL, &error);
#undef LIC_BODY
    if (error) {
        return NULL;
    }
    // g_print("BODY:%s", body);
    return lar_licensse_deserialize_list(body);
}

/*static void lar_client_print_message(GInputStream *stream) {
    gchar   body[4095];
    GError *error = NULL;
    memset(body, 0, 4095);
    g_input_stream_read(stream, body, 4095, NULL, &error);
    if (error == NULL) {
        g_print("%s\n", body);
    } else {
        g_warning("read input stream error %d - %s", error->code, error->message);
        g_error_free(error);
    }
}*/

static void get_licenses_callback(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    SoupMessage * message = SOUP_MESSAGE(user_data);
    LarApiClient *api     = LAR_API_CLIENT(source_object);
    GInputStream *stream  = NULL;
    GError *      error   = NULL;
    stream                = soup_session_send_finish(SOUP_SESSION(source_object), res, &error);
    if (error) {
        g_object_set(api, "connection-failed", TRUE, "busy", FALSE, NULL);
        return;
    }
    // g_debug("GET LICENSES STATUS:%d", message->status_code);
    if (message->status_code == 200) {

        if (error) {
            g_warning("connect device stream error - %s", error->message);
            g_error_free(error);
        } else {
            if (api->priv->licenses) g_list_free_full(api->priv->licenses, g_object_unref);
            api->priv->licenses = NULL;
            api->priv->licenses = lar_liceses_from_json_body_stream(stream);
        }
    }
    g_object_set(api, "busy", FALSE, "licenses", api->priv->licenses != NULL ? g_list_length(api->priv->licenses) : 0, NULL);
}

static void get_device_callback(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    g_print("get_device callback %p\n", source_object);
    SoupMessage * message   = SOUP_MESSAGE(user_data);
    LarApiClient *api       = LAR_API_CLIENT(source_object);
    gboolean      connected = FALSE;
    GInputStream *stream    = NULL;
    GError *      error     = NULL;
    stream                  = soup_session_send_finish(SOUP_SESSION(source_object), res, &error);
    if (error) {
        g_object_set(api, "connection-failed", TRUE, "busy", FALSE, NULL);
        return;
    }
    // g_debug("GET DEVICE STATUS:%d", message->status_code);
    if (message->status_code == 200) {
        if (error) {
            g_warning("connect device stream error - %s", error->message);
            g_error_free(error);
        } else {
            api->priv->device = lar_device_from_json_body_stream(stream);
            if (api->priv->device) {
                SoupMessage *get_licenses = lar_api_get_licenses_message(api);
                if (get_licenses) {
                    soup_session_send_async(SOUP_SESSION(api), message, api->priv->operation_cancellable, get_licenses_callback, get_licenses);
                    g_object_unref(get_licenses);
                    connected = TRUE;
                }
            }
            g_object_unref(stream);
        }
    }
    g_object_set(api, "connection-failed", FALSE, "connected", api->priv->device != NULL, "busy", connected, NULL);
}

static void license_create_callback(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    SoupMessage * message   = SOUP_MESSAGE(user_data);
    LarApiClient *api       = LAR_API_CLIENT(source_object);
    gboolean      connected = FALSE;
    // g_debug("CREATE DEVICE STATUS:%d", message->status_code);
    if (api->priv->device) {
        SoupMessage *get_licenses = lar_api_get_licenses_message(api);
        if (get_licenses) {
            soup_session_send_async(SOUP_SESSION(api), message, api->priv->operation_cancellable, get_licenses_callback, get_licenses);
            g_object_unref(get_licenses);
            connected = TRUE;
        }
    }
    g_object_set(api, "busy", !connected, NULL);
}

gboolean lar_api_client_connect(LarApiClient *api, const gchar *serial) {
    g_return_val_if_fail(api != NULL, FALSE);
    g_return_val_if_fail(serial != NULL, FALSE);
    g_return_val_if_fail(LAR_IS_API_CLIENT(api), FALSE);
    lar_api_client_clean(api);
    // g_debug("CONNECT %p", api);
    g_object_set(api, "busy", TRUE, "connected", FALSE, "licenses", 0, NULL);
    SoupMessage *message = lar_api_get_device_message(api, serial);
    soup_session_send_async(SOUP_SESSION(api), message, api->priv->operation_cancellable, get_device_callback, message);
    g_object_unref(message);
    return TRUE;
}

gboolean lar_api_create_device(LarApiClient *api, LarDevice *device) {
    g_return_val_if_fail(api != NULL, FALSE);
    g_return_val_if_fail(LAR_IS_API_CLIENT(api), FALSE);
    lar_api_client_clean(api);
    SoupMessage *message = lar_api_create_device_message(api, device);
    g_object_set(api, "busy", TRUE, "connected", FALSE, "licenses", 0, NULL);
    soup_session_send_async(SOUP_SESSION(api), message, api->priv->operation_cancellable, get_device_callback, message);
    return TRUE;
}

gboolean lar_api_create_license(LarApiClient *api, LarLicense *license) {
    g_return_val_if_fail(api != NULL, FALSE);
    g_return_val_if_fail(LAR_IS_API_CLIENT(api), FALSE);
    g_return_val_if_fail(api->priv->device == NULL, FALSE);
    lar_api_client_operation_cancel(api);
    if (api->priv->licenses) g_list_free_full(api->priv->licenses, g_object_unref);
    api->priv->licenses  = NULL;
    SoupMessage *message = lar_api_create_license_message(api, license);
    if (message) {
        g_object_set(api, "busy", TRUE, "licenses", 0, NULL);
        soup_session_send_async(SOUP_SESSION(api), message, api->priv->operation_cancellable, license_create_callback, message);
        return TRUE;
    }
    return FALSE;
}

/**
 * lar_api_client_default_new:
 *
 * Api function to create new default client session
 * Return value: new LarApiClient object if sucefull
 */
LarApiClient *lar_api_client_default_new() { return lar_api_client_new("http://localhost", 3001); }

/**
 * lar_api_client_get_device:
 *
 * Api function to create new default client session
 * Return value: device LarDevice type
 */
LarDevice *lar_api_client_get_device(LarApiClient *api) {
    g_return_val_if_fail(api != NULL, NULL);
    g_return_val_if_fail(LAR_IS_API_CLIENT(api), NULL);
    return api->priv->device;
}

/**
 * lar_api_client_get_licenses:
 *
 * Api function to create new default client session
 * Return value: licenses GLIst of LarLicense objecten
 */

GList *lar_api_client_get_licenses(LarApiClient *api) {
    g_return_val_if_fail(api != NULL, NULL);
    g_return_val_if_fail(LAR_IS_API_CLIENT(api), NULL);

    return api->priv->licenses;
}
/**
 * lar_api_client_check_license:
 *
 * Api function to create new default client session
 * Return value: licenses GLIst of LarLicense objecten
 */

LarLicense *lar_api_client_find_license(LarApiClient *api, const gchar *product, const gchar *spec) {
    g_return_val_if_fail(api != NULL, NULL);
    g_return_val_if_fail(LAR_IS_API_CLIENT(api), NULL);
    if (api->priv->licenses == NULL) return FALSE;
    GList *l = NULL;
    for (l = api->priv->licenses; l != NULL; l = l->next) {
        if (0 == g_strcmp0(lar_license_get_product(LAR_LICENSE(l->data)), product) && g_strcmp0(lar_license_get_specification(LAR_LICENSE(l->data)), spec)) {
            return LAR_LICENSE(l->data);
        }
    }
    return NULL;
}

SoupMessage *lar_api_get_device_message(LarApiClient *api, const gchar *serial) {
    g_return_val_if_fail(api != NULL, NULL);
    g_return_val_if_fail(LAR_IS_API_CLIENT(api), NULL);
    gchar *      url     = g_strdup_printf("%s:%d/api/device/%s", api->priv->adress, api->priv->port, serial);
    SoupMessage *message = soup_message_new("GET", url);
    soup_message_headers_append(message->request_headers, "Content-Type", "application/json");
    soup_message_headers_append(message->request_headers, "q", "0.8");
    soup_message_headers_append(message->request_headers, "version", "1.0");
    g_free(url);
    return message;
}

SoupMessage *lar_api_get_licenses_message(LarApiClient *api) {
    g_return_val_if_fail(api != NULL, NULL);
    g_return_val_if_fail(LAR_IS_API_CLIENT(api), NULL);
    g_return_val_if_fail(api->priv->device != NULL, NULL);

    gchar *      url     = g_strdup_printf("%s:%d/api/licenses/%s", api->priv->adress, api->priv->port, lar_device_get_id(api->priv->device));
    SoupMessage *message = soup_message_new("GET", url);
    soup_message_headers_append(message->request_headers, "Content-Type", "application/json");
    soup_message_headers_append(message->request_headers, "q", "0.8");
    soup_message_headers_append(message->request_headers, "version", "1.0");
    g_free(url);
    return message;
}
/*
SoupMessage *lar_api_get_licenses_product_message(LarApiClient *api, const gchar *product) {
    g_return_val_if_fail(api != NULL, NULL);
    g_return_val_if_fail(LAR_IS_API_CLIENT(api), NULL);
    g_return_val_if_fail(api->priv->device != NULL, NULL);

    gchar *      url     = g_strdup_printf("%s:%d/api/licenses/%s/%s", api->priv->adress, api->priv->port, api->priv->serial, product);
    SoupMessage *message = soup_message_new("GET", url);
    soup_message_headers_append(message->request_headers, "Content-Type", "application/json");
    soup_message_headers_append(message->request_headers, "q", "0.8");
    soup_message_headers_append(message->request_headers, "version", "1.0");
    g_free(url);
    return message;
}
*/
SoupMessage *lar_api_create_device_message(LarApiClient *api, LarDevice *device) {
    g_return_val_if_fail(api != NULL, NULL);
    g_return_val_if_fail(LAR_IS_API_CLIENT(api), NULL);
    SoupMessage *message = NULL;
    gsize        length  = 0;
    gchar *      jsbody  = json_gobject_to_data(G_OBJECT(device), &length);
    if (jsbody == NULL) {
        g_warning("Serialize device object failed");

    } else {
        g_print("create debice jason:\n%s\n", jsbody);
        gchar *url = g_strdup_printf("%s:%d/api/devices", api->priv->adress, api->priv->port);
        message    = soup_message_new("POST", url);
        g_free(url);
        soup_message_headers_append(message->request_headers, "Content-Type", "application/json");
        soup_message_headers_append(message->request_headers, "q", "0.8");
        soup_message_headers_append(message->request_headers, "version", "1.0");
        soup_message_set_request(message, "application/json", SOUP_MEMORY_COPY, jsbody, length);
        g_free(jsbody);
    }
    return message;
}

SoupMessage *lar_api_create_license_message(LarApiClient *api, LarLicense *license) {
    gsize        length  = 0;
    SoupMessage *message = NULL;
    g_return_val_if_fail(api != NULL, NULL);
    g_return_val_if_fail(LAR_IS_API_CLIENT(api), NULL);
    g_return_val_if_fail(api->priv->device != NULL, NULL);

    gchar *jsbody = json_gobject_to_data(G_OBJECT(license), &length);
    if (jsbody == NULL) {
        g_warning("Serialize device object failed");
    } else {
        g_print("create debice jason:\n%s\n", jsbody);
        gchar *url = g_strdup_printf("%s:%d/api/licenses/%s", api->priv->adress, api->priv->port, lar_device_get_id(api->priv->device));
        message    = soup_message_new("POST", url);
        g_free(url);
        soup_message_headers_append(message->request_headers, "Content-Type", "application/json");
        soup_message_headers_append(message->request_headers, "q", "0.8");
        soup_message_headers_append(message->request_headers, "version", "1.0");
        soup_message_set_request(message, "application/json", SOUP_MEMORY_COPY, jsbody, length);
        g_free(jsbody);
    }
    return message;
}
