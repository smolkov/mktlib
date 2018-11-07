/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mktlibrary
 * Copyright (C) sascha 2013 <sascha@sascha-VirtualBox>
 *
   mktlibrary is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mktlibrary is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mkt-boolexp-ind.h"
#include "mkt-value.h"
#include <locale.h>
#include <stdlib.h>

#include "../config.h"
#include <glib/gi18n-lib.h>

struct _MktWarnPrivate {
    gboolean isWarning;
};

#define MKT_WARN_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), MKT_TYPE_WARN, MktWarnPrivate))

G_DEFINE_TYPE(MktWarn, mkt_warn, G_TYPE_OBJECT);

static void mkt_warn_init(MktWarn *mkt_warn) {
    MktWarnPrivate *priv = MKT_WARN_GET_PRIVATE(mkt_warn);
    mkt_warn->priv       = priv;

    /* TODO: Add initialization code here */
}

static void mkt_warn_finalize(GObject *object) {
    /* TODO: Add deinitalization code here */
    // g_debug("mkt_warn_finalize1");

    G_OBJECT_CLASS(mkt_warn_parent_class)->finalize(object);
}

static void mkt_parser_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    // TEST:	g_debug("sensor_set_property %s",pspec->name);
    g_return_if_fail(MKT_IS_WARN(object));
    // MktWarn* parser = MKT_WARN(object);
    switch (prop_id) {

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
    // TEST:	g_debug("sensor_set_property %s end",pspec->name);
}

static void mkt_parser_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    // TEST:	g_debug("sensor_get_property %s",pspec->name);
    g_return_if_fail(MKT_IS_WARN(object));
    // MktWarn* parser = MKT_WARN(object);
    switch (prop_id) {

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
    // TEST:	g_debug("sensor_get_property %s end",pspec->name);
}

static void mkt_warn_class_init(MktWarnClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // GObjectClass* parent_class = G_OBJECT_CLASS (klass);
    object_class->finalize     = mkt_warn_finalize;
    object_class->set_property = mkt_parser_set_property;
    object_class->get_property = mkt_parser_get_property;

    g_type_class_add_private(klass, sizeof(MktWarnPrivate));

    object_class->finalize = mkt_warn_finalize;
}

MktWarn *mkt_warn_new() {
    MktWarn *parcer = MKT_WARN(g_object_new(MKT_TYPE_WARN, NULL));
    return parcer;
}

struct _MktFailPrivate {
    gboolean isWarning;
};

#define MKT_FAIL_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), MKT_TYPE_FAIL, MktFailPrivate))

G_DEFINE_TYPE(MktFail, mkt_fail, G_TYPE_OBJECT);

static void mkt_fail_init(MktFail *mkt_fail) {
    MktFailPrivate *priv = MKT_FAIL_GET_PRIVATE(mkt_fail);
    mkt_fail->priv       = priv;

    /* TODO: Add initialization code here */
}

static void mkt_fail_finalize(GObject *object) {
    /* TODO: Add deinitalization code here */

    G_OBJECT_CLASS(mkt_fail_parent_class)->finalize(object);
}

static void mkt_fail_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    // TEST:	g_debug("sensor_set_property %s",pspec->name);
    g_return_if_fail(MKT_IS_FAIL(object));
    // MktFail* parser = MKT_FAIL(object);
    switch (prop_id) {

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
    // TEST:	g_debug("sensor_set_property %s end",pspec->name);
}

static void mkt_fail_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    // TEST:	g_debug("sensor_get_property %s",pspec->name);
    g_return_if_fail(MKT_IS_FAIL(object));
    // MktFail* parser = MKT_FAIL(object);
    switch (prop_id) {

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
    // TEST:	g_debug("sensor_get_property %s end",pspec->name);
}

static void mkt_fail_class_init(MktFailClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // GObjectClass* parent_class = G_OBJECT_CLASS (klass);
    object_class->finalize     = mkt_fail_finalize;
    object_class->set_property = mkt_fail_set_property;
    object_class->get_property = mkt_fail_get_property;

    g_type_class_add_private(klass, sizeof(MktFailPrivate));

    object_class->finalize = mkt_fail_finalize;
}

MktFail *mkt_fail_new() {
    MktFail *parcer = MKT_FAIL(g_object_new(MKT_TYPE_FAIL, NULL));
    return parcer;
}
