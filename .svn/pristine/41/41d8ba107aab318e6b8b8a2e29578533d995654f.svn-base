/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mkt-measurement-data.c
 * Copyright (C) sascha 2013 <sascha@sascha-VirtualBox>
 *
mkt-measurement-data.c is free software: you can redistribute it and/or modify
it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mkt-measurement-data.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mkt-item-object.h"
#include "market-time.h"
#include "mkt-connection.h"

enum {
  PROP_0,
  PROP_ITEM_OBJECT_INDEX,
  PROP_ITEM_OBJECT_TYPE,
  PROP_ITEM_OBJECT_PROPERTY,
  PROP_ITEM_OBJECT_VALUE_TYPE,
  PROP_ITEM_OBJECT_VALUE
};

struct _MktItemObjectPrivate {

  guint64 object_id;
  gchar *object_type;
  gchar *object_property;
  gchar *value_type;
  gchar *value;
};

#define MKT_ITEM_OBJECT_PRIVATE(o)                                             \
  (G_TYPE_INSTANCE_GET_PRIVATE((o), MKT_TYPE_ITEM_OBJECT, MktItemObjectPrivate))

gulong mkt_item_object_get_object_id(MktItem *item) {
  g_return_val_if_fail(item != NULL, (gulong)0);
  g_return_val_if_fail(MKT_IS_ITEM_OBJECT(item), (gulong)0);
  return MKT_ITEM_OBJECT(item)->priv->object_id;
}

const gchar *mkt_item_object_get_object_type(MktItem *item) {
  g_return_val_if_fail(item != NULL, NULL);
  g_return_val_if_fail(MKT_IS_ITEM_OBJECT(item), NULL);
  return MKT_ITEM_OBJECT(item)->priv->object_type;
}

static const gchar *mkt_item_object_get_object_property(MktItem *item) {
  g_return_val_if_fail(item != NULL, NULL);
  g_return_val_if_fail(MKT_IS_ITEM_OBJECT(item), NULL);
  return MKT_ITEM_OBJECT(item)->priv->object_property;
}

static const gchar *mkt_item_object_get_value_type(MktItem *item) {
  g_return_val_if_fail(item != NULL, NULL);
  g_return_val_if_fail(MKT_IS_ITEM_OBJECT(item), NULL);
  return MKT_ITEM_OBJECT(item)->priv->value_type;
}

static const gchar *mkt_item_object_get_value(MktItem *item) {
  g_return_val_if_fail(item != NULL, NULL);
  g_return_val_if_fail(MKT_IS_ITEM_OBJECT(item), NULL);
  return MKT_ITEM_OBJECT(item)->priv->value;
}

static void mkt_item_object_init_item_interface(MktItemInterface *iface) {
  iface->item_object_id = mkt_item_object_get_object_id;
  iface->item_object_type = mkt_item_object_get_object_type;
  iface->item_property = mkt_item_object_get_object_property;
  iface->item_value_type = mkt_item_object_get_value_type;
  iface->item_value = mkt_item_object_get_value;
}

G_DEFINE_TYPE_WITH_CODE(
    MktItemObject, mkt_item_object, MKT_TYPE_MODEL,
    G_IMPLEMENT_INTERFACE(MKT_TYPE_ITEM, mkt_item_object_init_item_interface)
        MKT_CREATE_MODEL_FULL(mkt_connection_get_default(TRUE)))

static void mkt_item_object_init(MktItemObject *mkt_item_object) {
  MktItemObjectPrivate *priv = MKT_ITEM_OBJECT_PRIVATE(mkt_item_object);
  priv->object_id = 0;
  priv->object_type = g_strdup("void");
  priv->object_property = g_strdup("null");
  priv->value_type = g_strdup("void");
  priv->value = g_strdup("null");
  mkt_item_object->priv = priv;

  /* TODO: Add initialization code here */
}

static void mkt_item_object_finalize(GObject *object) {
  MktItemObject *data = MKT_ITEM_OBJECT(object);
  if (data->priv->object_type)
    g_free(data->priv->object_type);
  if (data->priv->object_property)
    g_free(data->priv->object_property);
  if (data->priv->value_type)
    g_free(data->priv->value_type);
  if (data->priv->value)
    g_free(data->priv->value);
  G_OBJECT_CLASS(mkt_item_object_parent_class)->finalize(object);
}

static void mkt_item_object_set_property(GObject *object, guint prop_id,
                                         const GValue *value,
                                         GParamSpec *pspec) {
  g_return_if_fail(MKT_IS_ITEM_OBJECT(object));
  MktItemObject *data = MKT_ITEM_OBJECT(object);
  switch (prop_id) {
  case PROP_ITEM_OBJECT_INDEX:
    data->priv->object_id = g_value_get_uint64(value);
    break;
  case PROP_ITEM_OBJECT_TYPE:
    if (data->priv->object_type != NULL)
      g_free(data->priv->object_type);
    data->priv->object_type = g_value_dup_string(value);
    break;
  case PROP_ITEM_OBJECT_PROPERTY:
    if (data->priv->object_property != NULL)
      g_free(data->priv->object_property);
    data->priv->object_property = g_value_dup_string(value);
    break;
  case PROP_ITEM_OBJECT_VALUE_TYPE:
    if (data->priv->value_type != NULL)
      g_free(data->priv->value_type);
    data->priv->value_type = g_value_dup_string(value);
    break;
  case PROP_ITEM_OBJECT_VALUE:
    if (data->priv->value != NULL)
      g_free(data->priv->value);
    data->priv->value = g_value_dup_string(value);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void mkt_item_object_get_property(GObject *object, guint prop_id,
                                         GValue *value, GParamSpec *pspec) {
  g_return_if_fail(MKT_IS_ITEM_OBJECT(object));
  MktItemObject *data = MKT_ITEM_OBJECT(object);
  switch (prop_id) {
  case PROP_ITEM_OBJECT_INDEX:
    g_value_set_uint64(value, data->priv->object_id);
    break;
  case PROP_ITEM_OBJECT_TYPE:
    g_value_set_string(value, data->priv->object_type);
    break;
  case PROP_ITEM_OBJECT_PROPERTY:
    g_value_set_string(value, data->priv->object_property);
    break;
  case PROP_ITEM_OBJECT_VALUE_TYPE:
    g_value_set_string(value, data->priv->value_type);
    break;
  case PROP_ITEM_OBJECT_VALUE:
    g_value_set_string(value, data->priv->value);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void mkt_item_object_class_init(MktItemObjectClass *klass) {
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  // MktModelClass* parent_class = MKT_MODEL_CLASS (klass);
  g_type_class_add_private(klass, sizeof(MktItemObjectPrivate));
  object_class->finalize = mkt_item_object_finalize;
  object_class->set_property = mkt_item_object_set_property;
  object_class->get_property = mkt_item_object_get_property;

  g_object_class_override_property(object_class, PROP_ITEM_OBJECT_INDEX,
                                   "item-object-index");
  g_object_class_override_property(object_class, PROP_ITEM_OBJECT_TYPE,
                                   "item-object-type");
  g_object_class_override_property(object_class, PROP_ITEM_OBJECT_PROPERTY,
                                   "item-object-property");
  g_object_class_override_property(object_class, PROP_ITEM_OBJECT_VALUE_TYPE,
                                   "item-value-type");
  g_object_class_override_property(object_class, PROP_ITEM_OBJECT_VALUE,
                                   "item-value");
}
