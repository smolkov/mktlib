/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * lar-device.h
 * Copyright (C) 2017 sascha.smolkov <saschasmolkov@saschasmolkov>
 *
 * lar-device.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public Device as published by the
 * Free Software Foundation, either version 3 of the Device, or
 * (at your option) any later version.
 *
 * lar-device.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public Device for more details.
 *
 * You should have received a copy of the GNU General Public Device along
 * with this program.  If not, see <http://www.gnu.org/devices/>.
 */

#ifndef _ULTRA_STREAM_H_
#define _ULTRA_STREAM_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define ULTRA_TYPE_STREAM (ultra_stream_get_type())
#define ULTRA_STREAM(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), ULTRA_TYPE_STREAM, UltraStream))
#define ULTRA_STREAM_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), ULTRA_TYPE_STREAM, UltraStreamClass))
#define ULTRA_IS_STREAM(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), ULTRA_TYPE_STREAM))
#define ULTRA_IS_STREAM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), ULTRA_TYPE_STREAM))
#define ULTRA_STREAM_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), ULTRA_TYPE_STREAM, UltraStreamClass))

typedef struct _UltraStreamClass   UltraStreamClass;
typedef struct _UltraStream        UltraStream;
typedef struct _UltraStreamPrivate UltraStreamPrivate;

struct _UltraStreamClass {
    GObjectClass parent_class;
};

struct _UltraStream {
    GObject parent_instance;

    UltraStreamPrivate *priv;
};

GType ultra_stream_get_type(void) G_GNUC_CONST;
void ultra_stream_print_test(UltraStream *device);
const gchar *ultra_stream_get_product(UltraStream *device);
const gchar *ultra_stream_get_serial(UltraStream *device);
const gchar *ultra_stream_get_id(UltraStream *device);
const gchar *ultra_stream_get_discription(UltraStream *device);
const gchar *ultra_stream_get_mac(UltraStream *device);
UltraStream *ultra_stream_new_from_json(const gchar *json_body);

G_END_DECLS

#endif /* _ULTRA_STREAM_H_ */
