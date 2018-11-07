/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * larsensor-data.c
 * Copyright (C) sascha 2013 <sascha@sascha-VirtualBox>
 *
larsensor-data.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * larsensor-data.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "amount.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>

enum {
    PROP_0,
    PROP_COUNTER,
    PROP_PERCENTAGE,
};

struct _AmountClass {
    GObjectClass parent_class;
};

struct _Amount {
    GObject parent_instance;
    guint   counter;
    gdouble percentage;
};

static JsonSerializableIface *amount_serializable_iface = NULL;

static void json_amount_iface_init(gpointer g_iface);

static JsonNode *amount_serialize_property(JsonSerializable *serializable, const gchar *name, const GValue *value, GParamSpec *pspec) {
    JsonNode *retval = NULL;
    retval           = amount_serializable_iface->serialize_property(serializable, name, value, pspec);
    return retval;
}

gboolean amount_deserialize_property(JsonSerializable *serializable, const gchar *property_name, GValue *value, GParamSpec *pspec, JsonNode *property_node) {
    // g_debug("deserialize device property %s", property_name);
    
    return amount_serializable_iface->deserialize_property(serializable, property_name, value, pspec, property_node);
}

static void json_amount_iface_init(gpointer g_iface) {
    JsonSerializableIface *iface = g_iface;

    amount_serializable_iface = g_type_default_interface_peek(JSON_TYPE_SERIALIZABLE);
    iface->serialize_property    = amount_serialize_property;
    iface->deserialize_property  = amount_deserialize_property;
}

G_DEFINE_TYPE_WITH_CODE(Amount, amount, G_TYPE_OBJECT, G_IMPLEMENT_INTERFACE(JSON_TYPE_SERIALIZABLE, json_amount_iface_init));

static void amount_init(Amount *amount) {
    amount->counter = 0 ;
    amount->percentage = 20.0;
}

static void amount_finalize(GObject *object) {
    // Amount *amount = AMOUNT(object);
    G_OBJECT_CLASS(amount_parent_class)->finalize(object);
}

static void amount_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_AMOUNT(object));
    Amount *stream = AMOUNT(object);
    switch (prop_id) {
    case PROP_COUNTER:
        stream->counter = g_value_get_uint(value);
        break;
    case PROP_PERCENTAGE:
        stream->percentage = g_value_get_double(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void amount_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_AMOUNT(object));
    Amount *stream = AMOUNT(object);
    switch (prop_id) {
    case PROP_COUNTER:
        g_value_set_uint(value, stream->counter);
        break;
    case PROP_PERCENTAGE:
        g_value_set_double(value, stream->percentage);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void amount_class_init(AmountClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // Class* parent_class = MODEL_CLASS (klass);
    object_class->finalize     = amount_finalize;
    object_class->set_property = amount_set_property;
    object_class->get_property = amount_get_property;

    g_object_class_install_property(
        object_class, PROP_COUNTER, g_param_spec_uint("counter", "amount amount counter", "Set|Get amount amount counter", 0, 5, 0, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_PERCENTAGE,
        g_param_spec_double("percentage", "amount amount percentage", "Set|Get amount amount percentage", 20.0, 50.0, 20.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
}

guint AmountGetCounter(Amount *amount) {
    g_return_val_if_fail(amount != NULL, 0);
    return amount->counter;
}

gdouble AmountGetPercentage(Amount *amount) {
    g_return_val_if_fail(amount != NULL, 0.0);
    return amount->percentage;
}

