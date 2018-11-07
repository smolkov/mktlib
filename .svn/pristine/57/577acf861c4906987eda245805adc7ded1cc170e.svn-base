/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * lar-mongo-timeout_cancel.c
 * Copyright (C) 2017 sascha.smolkov <saschasmolkov@saschasmolkov>
 *
 * lar-mongo-timeout_cancel.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lar-mongo-timeout_cancel.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "lar-device.h"
#include "lar-timeout-cancel.h"

struct _LarTimeoutCancelPrivate {
    gdouble       seconds;
    GCancellable *cancellable;
    GTimer *      timer;
};

enum {
    PROP_0,
    PROP_PORT,
    PROP_ADRESS,
    PROP_DBNAME,
};

G_DEFINE_TYPE(LarTimeoutCancel, lar_timeout_cancel, G_TYPE_OBJECT);

static void lar_timeout_cancel_init(LarTimeoutCancel *lar_timeout_cancel) {
    lar_timeout_cancel->priv          = G_TYPE_INSTANCE_GET_PRIVATE(lar_timeout_cancel, LAR_TYPE_TIMEOUT_CANCEL, LarTimeoutCancelPrivate);
    lar_timeout_cancel->priv->timer   = g_timer_new();
    lar_timeout_cancel->priv->seconds = 1.0;

    /* TODO: Add initialization code here */
}

static void lar_timeout_cancel_finalize(GObject *object) {
    LarTimeoutCancel *timeout_cancel = LAR_TIMEOUT_CANCEL(object);
    if (timeout_cancel->priv->cancellable) {
        if (!g_cancellable_is_cancelled(timeout_cancel->priv->cancellable)) g_cancellable_cancel(timeout_cancel->priv->cancellable);
        g_object_unref(timeout_cancel->priv->cancellable);
    }
    if (timeout_cancel->priv->timer) g_timer_destroy(timeout_cancel->priv->timer);
    G_OBJECT_CLASS(lar_timeout_cancel_parent_class)->finalize(object);
}

static void lar_timeout_cancel_constructed(GObject *object) {
    // LarTimeoutCancel *timeout_cancel = LAR_TIMEOUT_CANCEL(object);
    G_OBJECT_CLASS(lar_timeout_cancel_parent_class)->constructed(object);
}

static void lar_timeout_cancel_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(LAR_IS_TIMEOUT_CANCEL(object));
    LarTimeoutCancel *timeout_cancel = LAR_TIMEOUT_CANCEL(object);
    switch (prop_id) {
    case PROP_PORT:
        timeout_cancel->priv->seconds = g_value_get_double(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_timeout_cancel_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(LAR_IS_TIMEOUT_CANCEL(object));
    LarTimeoutCancel *timeout_cancel = LAR_TIMEOUT_CANCEL(object);
    switch (prop_id) {
    case PROP_PORT:
        g_value_set_double(value, timeout_cancel->priv->seconds);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void lar_timeout_cancel_class_init(LarTimeoutCancelClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    g_type_class_add_private(klass, sizeof(LarTimeoutCancelPrivate));

    object_class->finalize    = lar_timeout_cancel_finalize;
    object_class->constructed = lar_timeout_cancel_constructed;

    object_class->set_property = lar_timeout_cancel_set_property;
    object_class->get_property = lar_timeout_cancel_get_property;

    g_object_class_install_property(object_class, PROP_PORT, g_param_spec_double("timeout", "timeout", "timeout", 0.0, G_MAXDOUBLE, 0.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

LarTimeoutCancel *lar_timeout_cancel_new(const gchar *adress, guint port) {
    LarTimeoutCancel *timeout_cancel = g_object_new(LAR_TYPE_TIMEOUT_CANCEL, "port", port, "adress", adress, NULL);
    return timeout_cancel;
}
static gboolean lar_timeout_check_timer(gpointer user_data) {
    LarTimeoutCancel *timeout = LAR_TIMEOUT_CANCEL(user_data);
    if (g_timer_elapsed(timeout->priv->timer, NULL) > timeout->priv->seconds) {
        if (!g_cancellable_is_cancelled(timeout->priv->cancellable)) g_cancellable_cancel(timeout->priv->cancellable);
        g_object_unref(timeout);
        return FALSE;
    }
    return TRUE;
}
gboolean lar_timeout_cancel_run(GCancellable *cancellable, gdouble seconds) {
    g_return_val_if_fail(cancellable != NULL, FALSE);
    g_return_val_if_fail(G_IS_CANCELLABLE(cancellable), FALSE);
    LarTimeoutCancel *timeout_cancel  = g_object_new(LAR_TYPE_TIMEOUT_CANCEL, "timeout", seconds, NULL);
    timeout_cancel->priv->cancellable = g_object_ref(cancellable);
    g_timeout_add(20, lar_timeout_check_timer, timeout_cancel);
    return TRUE;
}
