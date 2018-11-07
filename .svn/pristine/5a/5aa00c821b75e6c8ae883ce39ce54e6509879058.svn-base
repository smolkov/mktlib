/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * lar-mongo-database.h
 * Copyright (C) 2017 sascha.smolkov <saschasmolkov@saschasmolkov>
 *
 * lar-mongo-database.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lar-mongo-database.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _LAR_TIMEOUT_CANCEL_H_
#define _LAR_TIMEOUT_CANCEL_H_

#include "lar-device.h"
#include <gio/gio.h>
#include <glib-object.h>
#include <glib.h>
G_BEGIN_DECLS

#define LAR_TYPE_TIMEOUT_CANCEL (lar_timeout_cancel_get_type())
#define LAR_TIMEOUT_CANCEL(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), LAR_TYPE_TIMEOUT_CANCEL, LarTimeoutCancel))
#define LAR_TIMEOUT_CANCEL_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), LAR_TYPE_TIMEOUT_CANCEL, LarTimeoutCancelClass))
#define LAR_IS_TIMEOUT_CANCEL(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), LAR_TYPE_TIMEOUT_CANCEL))
#define LAR_IS_TIMEOUT_CANCEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), LAR_TYPE_TIMEOUT_CANCEL))
#define LAR_TIMEOUT_CANCEL_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), LAR_TYPE_TIMEOUT_CANCEL, LarTimeoutCancelClass))

typedef struct _LarTimeoutCancelClass   LarTimeoutCancelClass;
typedef struct _LarTimeoutCancel        LarTimeoutCancel;
typedef struct _LarTimeoutCancelPrivate LarTimeoutCancelPrivate;

struct _LarTimeoutCancelClass {
    GObjectClass parent_class;
};

struct _LarTimeoutCancel {
    GObject parent_instance;

    LarTimeoutCancelPrivate *priv;
};

GType lar_timeout_cancel_get_type(void) G_GNUC_CONST;

gboolean lar_timeout_cancel_run(GCancellable *cancellable, gdouble seconds);

G_END_DECLS

#endif /* _LAR_TIMEOUT_CANCEL_H_ */
