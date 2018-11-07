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
    LarApiClient *session;
    SoupMessage * message;
};

enum { PROP_0, PROP_SESSION, PROP_MESSAGE };

G_DEFINE_TYPE_WITH_PRIVATE(LarRequest, lar_request, G_TYPE_OBJECT)

static void lar_request_init(LarRequest *request) {
    request->priv          = lar_request_get_instance_private(request);
    request->priv->message = NULL;
    request->priv->session = NULL;
}

static void lar_request_finalize(GObject *object) {
    LarRequest *http = LAR_REQUEST(object);
    if (http->priv->session) {
        g_object_unref(http->priv->session);
    }
    if (http->priv->message) {
        g_object_unref(http->priv->message);
    }

    G_OBJECT_CLASS(lar_request_parent_class)->finalize(object);
}

static void lar_request_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    LarRequest *request = LAR_REQUEST(object);

    switch (prop_id) {
    case PROP_SESSION:
        if (request->priv->session) g_object_unref(request->priv->session);
        request->priv->session = g_value_dup_object(value);
        break;
    case PROP_MESSAGE:
        if (request->priv->message) g_object_unref(request->priv->message);
        request->priv->message = g_value_dup_object(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_request_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    LarRequest *request = LAR_REQUEST(object);

    switch (prop_id) {
    case PROP_SESSION:
        g_value_set_object(value, request->priv->session);
        break;
    case PROP_MESSAGE:
        g_value_set_object(value, request->priv->message);
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

    // request_class->send        = lar_request_send;
    // request_class->send_async  = lar_request_send_async;
    // request_class->send_finish = lar_request_send_finish;

    /**
     * LarRequest:session:
     *
     * The request's #LarApiClient.
     *
     */
    g_object_class_install_property(object_class, PROP_SESSION,
        g_param_spec_object("session", "Session", "The request's session", LAR_TYPE_API_CLIENT, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
    /**
     * LarRequest:response:
     *
     * The request's #LarResponse.
     *
     */
    g_object_class_install_property(object_class, PROP_MESSAGE,
        g_param_spec_object("message", "message", "The Response object", SOUP_TYPE_MESSAGE, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

/**
 * lar_request_get_session:
 * @request: a #SoupRequest
 *
 * Gets @request's #LarApiClient
 *
 * Return value: (transfer none): @request's #LarApiClient
 *
 */
LarApiClient *lar_request_get_session(LarRequest *request) {
    g_return_val_if_fail(LAR_IS_REQUEST(request), 0);
    return request->priv->session;
}

/**
 * lar_request_get_message:
 * @request: a #LarRequest object
 *
 * Gets a reference to the #SoupMessage associated
 *
 * Returns: (transfer full): a reference to the #SoupMessage
 *
 */
SoupMessage *lar_request_get_message(LarRequest *request) {
    g_return_val_if_fail(LAR_IS_REQUEST(request), NULL);
    return request->priv->message;
}
guint lar_request_get_code(LarRequest *request) {
    g_return_val_if_fail(LAR_IS_REQUEST(request), 0);
    g_return_val_if_fail(request->priv->message != NULL, 0);
    return request->priv->message->status_code;
}
