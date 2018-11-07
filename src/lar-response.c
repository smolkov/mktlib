/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * lar-response.c
 * Copyright (C) 2017 sascha.smolkov <saschasmolkov@saschasmolkov>
 *
 * lar-response.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public Response as published by the
 * Free Software Foundation, either version 3 of the Response, or
 * (at your option) any later version.
 *
 * lar-response.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public Response for more details.
 *
 * You should have received a copy of the GNU General Public Response along
 * with this program.  If not, see <http://www.gnu.org/responses/>.
 */

#include "lar-response.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>
#include <libsoup/soup.h>

struct _LarResponsePrivate {
    SoupMessage *message;
    guint        code;
};

enum { PROP_0, PROP_MESSAGE, PROP_CODE };

G_DEFINE_TYPE(LarResponse, lar_response, G_TYPE_OBJECT);

static void lar_response_init(LarResponse *lar_response) {
    lar_response->priv = G_TYPE_INSTANCE_GET_PRIVATE(lar_response, LAR_TYPE_RESPONSE, LarResponsePrivate);
}

static void lar_response_finalize(GObject *object) {
    LarResponse *response = LAR_RESPONSE(object);
    if (response->priv->message) g_object_unref(response->priv->message);
    G_OBJECT_CLASS(lar_response_parent_class)->finalize(object);
}

static void lar_response_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(LAR_IS_RESPONSE(object));
    LarResponse *response = LAR_RESPONSE(object);
    switch (prop_id) {
    case PROP_MESSAGE:
        if (response->priv->message) g_object_unref(response->priv->message);
        response->priv->message = g_value_dup_object(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_response_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(LAR_IS_RESPONSE(object));
    LarResponse *response = LAR_RESPONSE(object);

    switch (prop_id) {
        g_value_set_object(value, response->priv->message);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_response_class_init(LarResponseClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    g_type_class_add_private(klass, sizeof(LarResponsePrivate));

    object_class->finalize     = lar_response_finalize;
    object_class->set_property = lar_response_set_property;
    object_class->get_property = lar_response_get_property;

    g_object_class_install_property(object_class, PROP_MESSAGE,
        g_param_spec_object("message", "soup message object", "message", SOUP_TYPE_MESSAGE, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
}

guint lar_response_get_code(LarResponse *response) {
    g_return_val_if_fail(response != NULL, 0);
    g_return_val_if_fail(LAR_IS_RESPONSE(response), 0);
    return response->priv->message != NULL ? response->priv->message->status_code : 0;
}
SoupMessage *lar_response_get_message(LarResponse *lic) {
    g_return_val_if_fail(response != NULL, NULL);
    g_return_val_if_fail(LAR_IS_RESPONSE(response), NULL);
    return response->priv->message;
}
