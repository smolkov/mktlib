/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * lar-response.c
 * Copyright (C) 2017 sascha.smolkov <saschasmolkov@saschasmolkov>
 *
 * lar-response.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public ResponseLicense as published by the
 * Free Software Foundation, either version 3 of the ResponseLicense, or
 * (at your option) any later version.
 *
 * lar-response.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public ResponseLicense for more details.
 *
 * You should have received a copy of the GNU General Public ResponseLicense along
 * with this program.  If not, see <http://www.gnu.org/responses/>.
 */

#include "lar-response.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>
#include <libsoup/soup.h>

struct _LarResponseLicensePrivate {
    gchar *message;
    guint  code;
};

enum { PROP_0, PROP_MESSAGE, PROP_CODE };

G_DEFINE_TYPE(LarResponseLicense, lar_response, G_TYPE_OBJECT);

static void lar_response_init(LarResponseLicense *lar_response) {
    lar_response->priv = G_TYPE_INSTANCE_GET_PRIVATE(lar_response, LAR_TYPE_RESPONSE_LICENSE, LarResponseLicensePrivate);

    /* TODO: Add initialization code here */
}

static void lar_response_finalize(GObject *object) {
    LarResponseLicense *response = LAR_RESPONSE_LICENSE(object);
    if (response->priv->message) g_free(response->priv->message);

    G_OBJECT_CLASS(lar_response_parent_class)->finalize(object);
}
static void lar_response_constructed(GObject *object) {
    LarResponseLicense *response = LAR_RESPONSE_LICENSE(object);

    G_OBJECT_CLASS(lar_response_parent_class)->constructed(object);
}

static void lar_response_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(LAR_IS_RESPONSE_LICENSE(object));
    // LarResponseLicense *response = LAR_RESPONSE_LICENSE(object);
    switch (prop_id) {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_response_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(LAR_IS_RESPONSE_LICENSE(object));
    // LarResponseLicense *response = LAR_RESPONSE_LICENSE(object);
    switch (prop_id) {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_response_class_init(LarResponseLicenseClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    g_type_class_add_private(klass, sizeof(LarResponseLicensePrivate));

    object_class->finalize     = lar_response_finalize;
    object_class->set_property = lar_response_set_property;
    object_class->get_property = lar_response_get_property;
    object_class->constructed  = lar_response_constructed;
}
