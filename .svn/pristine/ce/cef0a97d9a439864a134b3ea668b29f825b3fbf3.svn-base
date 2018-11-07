/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktLimit
 * @{
 * @file  mkt-limit.c	Pc model interface
 * @brief This is LIMIT model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */

#include "mkt-limit.h"
#include "market-time.h"
#include "mkt-channel.h"
#include "mkt-limit-message.h"
#include "mkt-log.h"
#include "mkt-utils.h"
#include "mkt-value.h"
#include <math.h>

#include "../config.h"
#include <glib/gi18n-lib.h>

#if GLIB_CHECK_VERSION(2, 31, 7)
static GRecMutex init_rmutex;
#define MUTEX_LOCK() g_rec_mutex_lock(&init_rmutex)
#define MUTEX_UNLOCK() g_rec_mutex_unlock(&init_rmutex)
#else
static GStaticRecMutex init_mutex = G_STATIC_REC_MUTEX_INIT;
#define MUTEX_LOCK() g_static_rec_mutex_lock(&init_mutex)
#define MUTEX_UNLOCK() g_static_rec_mutex_unlock(&init_mutex)
#endif

/* signals */
enum { DATE_CHANGED, LAST_SIGNAL };

// static guint mkt_limit_iface_signals[LAST_SIGNAL];

static void mkt_limit_base_init(gpointer g_iface) {
  static gboolean is_initialized = FALSE;
  MUTEX_LOCK();
  if (!is_initialized) {
    GParamSpec *pspec;

    pspec = g_param_spec_boolean("limit-activated","is activated",
                                 "Set|Get activated property", FALSE,
                                 G_PARAM_READWRITE | MKT_MODEL_DB_PROP |
                                     MKT_MODEL_DB_LOG_CHANGE);
    g_object_interface_install_property(g_iface, pspec);

    pspec = g_param_spec_int("limit-pending", "Limit pending state property",
                             "Set|Get pending state property", -1, 1, 0,
                             G_PARAM_READWRITE | MKT_MODEL_DB_PROP);
    g_object_interface_install_property(g_iface, pspec);

    pspec = g_param_spec_uint("limit-number", "Limit pending state property",
                              "Set|Get pending state property", 1, G_MAXUINT, 1,
                              G_PARAM_READWRITE | MKT_MODEL_DB_PROP);
    g_object_interface_install_property(g_iface, pspec);

    g_object_interface_install_property(g_iface,
        g_param_spec_double("limit-min","minimum",
           "Set  minimal value in double property", 0., G_MAXDOUBLE, 0.,
            G_PARAM_READWRITE | MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));
    g_object_interface_install_property( g_iface,
        g_param_spec_double("limit-max","maximum",
                           "maximal value", 0.,
                            G_MAXDOUBLE, 100.,
                            G_PARAM_READWRITE | G_PARAM_CONSTRUCT |
                                MKT_MODEL_DB_PROP | MKT_MODEL_DB_LOG_CHANGE));

    g_object_interface_install_property(
        g_iface,
        g_param_spec_string("limit-name", "Limit minimal value in double",
                            "Set  minimal value in double property", "L",
                            G_PARAM_READWRITE | MKT_MODEL_DB_PROP));

    is_initialized = TRUE;
  }
  MUTEX_UNLOCK();
}

GType mkt_limit_get_type(void) {
  static GType iface_type = 0;
  if (iface_type == 0) {
    static const GTypeInfo info = {sizeof(MktLimitInterface),
                                   (GBaseInitFunc)mkt_limit_base_init,
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
      iface_type = g_type_register_static(G_TYPE_INTERFACE, "MktLimitInterface",
                                          &info, 0);
    }
    MUTEX_UNLOCK();
  }
  return iface_type;
}

gboolean mkt_limit_activated(MktLimit *limit) {
  g_return_val_if_fail(limit != NULL, FALSE);
  g_return_val_if_fail(MKT_IS_LIMIT(limit), FALSE);
  if (MKT_LIMIT_GET_INTERFACE(limit)->get_number)
    return MKT_LIMIT_GET_INTERFACE(limit)->get_number(limit);
  return FALSE;
}

guint mkt_limit_number(MktLimit *limit) {
  g_return_val_if_fail(limit != NULL, 0);
  g_return_val_if_fail(MKT_IS_LIMIT(limit), 0);
  if (MKT_LIMIT_GET_INTERFACE(limit)->get_number)
    return MKT_LIMIT_GET_INTERFACE(limit)->get_number(limit);
  return 0;
}

gint mkt_limit_pending(MktLimit *limit) {
  g_return_val_if_fail(limit != NULL, 0);
  g_return_val_if_fail(MKT_IS_LIMIT(limit), 0);
  if (MKT_LIMIT_GET_INTERFACE(limit)->get_pending)
    return MKT_LIMIT_GET_INTERFACE(limit)->get_pending(limit);
  return 0;
}
gdouble mkt_limit_min(MktLimit *limit) {
  g_return_val_if_fail(limit != NULL, 0.);
  g_return_val_if_fail(MKT_IS_LIMIT(limit), 0.);
  if (MKT_LIMIT_GET_INTERFACE(limit)->get_min)
    return MKT_LIMIT_GET_INTERFACE(limit)->get_min(limit);
  return 0.0;
}
gdouble mkt_limit_max(MktLimit *limit) {
  g_return_val_if_fail(limit != NULL, 0.);
  g_return_val_if_fail(MKT_IS_LIMIT(limit), 0.);
  if (MKT_LIMIT_GET_INTERFACE(limit)->get_max)
    return MKT_LIMIT_GET_INTERFACE(limit)->get_max(limit);
  return 0.0;
}

const gchar *mkt_limit_name(MktLimit *limit) {
  g_return_val_if_fail(limit != NULL, NULL);
  g_return_val_if_fail(MKT_IS_LIMIT(limit), NULL);
  if (MKT_LIMIT_GET_INTERFACE(limit)->get_name)
    return MKT_LIMIT_GET_INTERFACE(limit)->get_name(limit);
  return NULL;
}


/*
 *
 *
 *  @return new allocated string array ( L1:L2:L3 )
 */

gchar *mkt_limit_dup() {
  GString *limstr = g_string_new("");
  GSList *limits = mkt_model_select(MKT_TYPE_LIMIT_MESSAGE, "select * from $tablename "
                                                    "where limit_pending <> 0 "
                                                    "ORDER BY limit_name ASC");
  GSList *l = NULL;
  for (l = limits; l != NULL; l = l->next) {
    g_string_append_printf(limstr, "%s%s%s", limstr->len > 1 ? " " : "",
                           mkt_limit_name(MKT_LIMIT(l->data)),
                           mkt_limit_pending(MKT_LIMIT(l->data)) > 0 ? "max"
                                                                     : "min");
  }
  if (limits)
    mkt_slist_free_full(limits, g_object_unref);
  gchar *ret = limstr->str;
  g_string_free(limstr, FALSE);
  return ret;
}

/** @} */
