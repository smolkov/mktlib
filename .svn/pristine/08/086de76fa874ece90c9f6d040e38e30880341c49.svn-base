/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktItem
 * @{
 * @file  mkt-item.c	Item model interface
 * @brief This is Item model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */

#include "mkt-item.h"
#include "market-time.h"
#include "mkt-item-object.h"
#include "mkt-value.h"

#if GLIB_CHECK_VERSION(2, 31, 7)
static GRecMutex init_rmutex;
#define MUTEX_LOCK() g_rec_mutex_lock(&init_rmutex)
#define MUTEX_UNLOCK() g_rec_mutex_unlock(&init_rmutex)
#else
static GStaticRecMutex init_mutex = G_STATIC_REC_MUTEX_INIT;
#define MUTEX_LOCK() g_static_rec_mutex_lock(&init_mutex)
#define MUTEX_UNLOCK() g_static_rec_mutex_unlock(&init_mutex)
#endif

static void mkt_item_base_init(gpointer g_iface) {
  static gboolean is_item_initialized = FALSE;
  MUTEX_LOCK();
  if (!is_item_initialized) {
    g_object_interface_install_property(
        g_iface, g_param_spec_uint64(
                     "item-object-index", "Item object index  property",
                     "Set get item object index property", 0, G_MAXUINT64, 0,
                     G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

    g_object_interface_install_property(
        g_iface, g_param_spec_string(
                     "item-object-type", "Item object data type property",
                     "Set get item object data type property", "void",
                     G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

    g_object_interface_install_property(
        g_iface, g_param_spec_string(
                     "item-object-property", "Item object data type property",
                     "Set get item object data type property", "void",
                     G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

    g_object_interface_install_property(
        g_iface, g_param_spec_string(
                     "item-value-type", "Item object data type property",
                     "Set get item object data type property", "void",
                     G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));
    g_object_interface_install_property(
        g_iface, g_param_spec_string(
                     "item-value", "Item object data type property",
                     "Set get item object data type property", "null",
                     G_PARAM_READABLE | G_PARAM_WRITABLE | MKT_MODEL_DB_PROP));

    is_item_initialized = TRUE;
  }
  MUTEX_UNLOCK();
}

GType mkt_item_get_type(void) {
  static GType iface_type = 0;
  if (iface_type == 0) {
    static const GTypeInfo info = {sizeof(MktItemInterface),
                                   (GBaseInitFunc)mkt_item_base_init,
                                   (GBaseFinalizeFunc)NULL,
                                   (GClassInitFunc)NULL,
                                   NULL,
                                   NULL,
                                   0,
                                   0,
                                   (GInstanceInitFunc)NULL,
                                   0};
    MUTEX_LOCK();
    if (iface_type == 0) {
      iface_type = g_type_register_static(G_TYPE_INTERFACE, "MktItemInterface",
                                          &info, 0);
    }
    MUTEX_UNLOCK();
  }
  return iface_type;
}

gulong mkt_item_get_object_id(MktItem *item) {
  g_return_val_if_fail(item != NULL, (gulong)0);
  g_return_val_if_fail(MKT_IS_ITEM(item), (gulong)0);
  if (MKT_ITEM_GET_INTERFACE(item)->item_object_id)
    return MKT_ITEM_GET_INTERFACE(item)->item_object_id(item);
  return (gulong)0;
}

const gchar *mkt_item_get_object_type(MktItem *item) {
  g_return_val_if_fail(item != NULL, NULL);
  g_return_val_if_fail(MKT_IS_ITEM(item), NULL);
  if (MKT_ITEM_GET_INTERFACE(item)->item_object_type)
    return MKT_ITEM_GET_INTERFACE(item)->item_object_type(item);
  return NULL;
}

const gchar *mkt_item_get_property(MktItem *item) {
  g_return_val_if_fail(item != NULL, NULL);
  g_return_val_if_fail(MKT_IS_ITEM(item), NULL);
  if (MKT_ITEM_GET_INTERFACE(item)->item_property)
    return MKT_ITEM_GET_INTERFACE(item)->item_property(item);
  return NULL;
}

const gchar *mkt_item_get_value(MktItem *item) {
  g_return_val_if_fail(item != NULL, NULL);
  g_return_val_if_fail(MKT_IS_ITEM(item), NULL);
  if (MKT_ITEM_GET_INTERFACE(item)->item_value)
    return MKT_ITEM_GET_INTERFACE(item)->item_value(item);
  return NULL;
}

const gchar *mkt_item_get_value_type(MktItem *item) {
  g_return_val_if_fail(item != NULL, NULL);
  g_return_val_if_fail(MKT_IS_ITEM(item), NULL);
  if (MKT_ITEM_GET_INTERFACE(item)->item_value_type)
    return MKT_ITEM_GET_INTERFACE(item)->item_value_type(item);
  return NULL;
}

GValue *mkt_item_value(MktItem *item) {
  g_return_val_if_fail(item != NULL, NULL);
  g_return_val_if_fail(MKT_IS_ITEM(item), NULL);
  g_return_val_if_fail(mkt_item_get_value(item) != NULL, NULL);
  g_return_val_if_fail(mkt_item_get_value_type(item) != NULL, NULL);
  GType vtype = g_type_from_name(mkt_item_get_value(item));
  if (vtype > G_TYPE_NONE)
    return NULL;
  GValue *new = mkt_value_new(vtype);
  if (new) {
    if (mkt_set_gvalue_from_string(new, mkt_item_get_value_type(item))) {
      return new;
    }
  }
  return NULL;
}

gboolean mkt_item_object_save(MktItem *item) {
  g_return_val_if_fail(item != NULL, FALSE);
  g_return_val_if_fail(MKT_IS_ITEM(item), FALSE);
  g_return_val_if_fail(mkt_item_get_object_type(item) != NULL, FALSE);
  g_return_val_if_fail(mkt_item_get_object_id(item) > 0, FALSE);
  g_return_val_if_fail(mkt_item_get_property(item) != NULL, FALSE);

  gboolean saved = FALSE;
  GType type = g_type_from_name(mkt_item_get_object_type(item));
  if (g_type_is_a(type, MKT_TYPE_MODEL)) {
    MktModel *model =
        mkt_model_select_one(type, "select * from $tablename from ref_id = %lu",
                             mkt_item_get_object_id(item));
    if (model) {
      GParamSpec *pspec = g_object_class_find_property(
          G_OBJECT_GET_CLASS(G_OBJECT(model)), mkt_item_get_property(item));
      GValue *value = mkt_item_value(item);
      if (pspec && value && pspec->value_type == value->g_type) {
        g_object_set_property(G_OBJECT(model), pspec->name, value);
        saved = TRUE;
      }
      if (value)
        mkt_value_free(value);
      g_object_unref(model);
    }
  }
  return saved;
}

MktItem *mkt_item_new_object(MktModel *save, const gchar *property,
                             const GValue *value) {
  g_return_val_if_fail(save != NULL, FALSE);
  g_return_val_if_fail(MKT_IS_MODEL(save), FALSE);
  g_return_val_if_fail(property != NULL, FALSE);
  g_return_val_if_fail(value != NULL, FALSE);

  guint64 id = mkt_model_ref_id(MKT_IMODEL(save));
  const gchar *object_type = G_OBJECT_TYPE_NAME(G_OBJECT(save));
  const gchar *value_type = g_type_name(value->g_type);
  gchar *value_str = mkt_value_stringify(value);
  MktModel *item = NULL;
  GParamSpec *pspect = g_object_class_find_property(
      G_OBJECT_GET_CLASS(G_OBJECT(save)), property);
  if (pspect) {
    const gchar *desc = pspect->_nick;
    item = mkt_model_new(
        MKT_TYPE_ITEM_OBJECT, "param-description", desc, "item-object-index",
        id, "item-object-type", object_type, "item-object-property", property,
        "item-value-type", value_type, "item-value", value_str, NULL);
    mkt_model_save(item);
  }

  g_free(value_str);
  return MKT_ITEM(item);
}
