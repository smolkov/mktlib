/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup LarRequest
 * @{
 * @file  mkt-axis.c	Axis model interface
 * @brief This is Axis model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "lar-request.h"
#include <glib/gi18n-lib.h>

#include "lar-request.h"
#include <libsoup/soup.h>

static GQuark request_error_quark(void) {
    static GQuark error;
    if (!error) error = g_quark_from_static_string("request_error");
    return error;
}

/**
 * SECTION:lar-request
 * @short_description: LarRequest support for "http" and "https" URIs
 *
 * #LarRequest implements for "http" and "https"
 * URIs.
 *
 * To do more complicated HTTP operations using the #LarRequest APIs,
 * call lar_request_get_message() to get the request's
 * #SoupMessage.
 */

struct _LarRequestPrivate {
    LarLicense *first;
    GList *     licenses_array;
};

enum { PROP_0, PROP_SESSION };

G_DEFINE_TYPE_WITH_PRIVATE(LarRequest, lar_request, G_TYPE_OBJECT)

static void
async_initable_iface_init(GAsyncInitableIface *async_initable_iface) {
  async_initable_iface->init_async = async_initable_init_async;
  async_initable_iface->init_finish = async_initable_init_finish;
}

G_DEFINE_TYPE_WITH_CODE(ProductLicenses, product_licenses, GTK_TYPE_BOX,
                        G_ADD_PRIVATE(ProductLicenses)
                            G_IMPLEMENT_INTERFACE(G_TYPE_ASYNC_INITABLE,
                                                  async_initable_iface_init))


static void lar_request_init(LarRequest *http) { http->priv = lar_request_get_instance_private(http); }

static void lar_request_finalize(GObject *object) {
    LarRequest *http = LAR_REQUEST(object);
    if (http->priv->session) {
        g_object_unref(http->priv->session);
    }

    G_OBJECT_CLASS(lar_request_parent_class)->finalize(object);
}

GObject *lar_request_send(LarRequest *request, SoupMessage *message, GCancellable *cancellable) {
    SoupSession *session = SOUP_SESSION(lar_request_get_session(request));
    g_return_val_if_fail(session != NULL, NULL);
    return soup_session_send_message(session, message);
}

static void lar_request_input_stream_ready_cb(GObject *source, GAsyncResult *result, gpointer user_data) {
    GTask *       task  = G_TASK(user_data);
    GError *      error = NULL;
    GInputStream *stream;
    stream = soup_session_send_finish(SOUP_SESSION(source), result, &error);
    if (stream) {

        LarResponse *response = lar_response g_task_return_pointer(task, response, g_object_unref);
    } else
        g_task_return_error(task, error);
    g_object_unref(task);
}

static void lar_request_send_async(LarRequest *request, GCancellable *cancellable, GAsyncReadyCallback callback, gpointer user_data) {
    GTask *task;
    task = g_task_new(request, cancellable, callback, user_data);

    if (!lar_request_get_session(request) || lar_response_get_message(reques->priv->response) == NULL) {
        GError *error = g_error_new(request_error_quark(), 1, "Operation has failed because of parameter error.");
        g_task_return_error(task, error);
    }
    soup_session_send_async(session, lar_response_get_message(response), cancellable, lar_request_input_stream_ready_cb, task);
}

static GInputStream *soup_request_http_send_finish(SoupRequest *request, GAsyncResult *result, GError **error) {
    g_return_val_if_fail(g_task_is_valid(result, request), NULL);

    return g_task_propagate_pointer(G_TASK(result), error);
}

static void lar_request_send_thread(GTask *task, gpointer source_object, gpointer task_data, GCancellable *cancellable) {
    LarRequest * request = source_object;
    SoupSession *session = g_object_ref(SOUP_SESSION(lar_request_get_session(request)));
    GError *     error   = NULL;
    SoupMessage *message = lar_request_get_message guint code = soup_session_send_message(session, message);
    if (g_task_set_return_on_cancel(task, FALSE)) {
        GObject *response = g_task_return_pointer(task, cake, g_object_unref);
    }
}

static GInputStream *lar_request_send_finish(LarRequest *request, GAsyncResult *result, GError **error) {
    g_return_val_if_fail(g_task_is_valid(result, request), NULL);

    return g_task_propagate_pointer(G_TASK(result), error);
}
static void lar_request_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    LarRequest *request = LAR_REQUEST(object);

    switch (prop_id) {
    case PROP_SESSION:
        if (request->priv->session) g_object_unref(request->priv->session);
        request->priv->session = g_value_dup_object(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_request_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    LaepRequest *request = LAR_REQUEST(object);

    switch (prop_id) {
    case PROP_SESSION:
        g_value_set_object(value, request->priv->session);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_request_class_init(LarRequestClass *request_class) {
    GObjectClass *object_class = G_OBJECT_CLASS(request_class);
    object_class->set_property = lar_request_set_property;
    object_class->get_property = lar_request_get_property;

    object_class->finalize = lar_request_finalize;
}
