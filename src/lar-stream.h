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

#ifndef _LAR_STREAM_H_
#define _LAR_STREAM_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define LAR_TYPE_STREAM (lar_stream_get_type())
#define LAR_STREAM(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), LAR_TYPE_STREAM, LarStream))
#define LAR_STREAM_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), LAR_TYPE_STREAM, LarStreamClass))
#define LAR_IS_STREAM(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), LAR_TYPE_STREAM))
#define LAR_IS_STREAM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), LAR_TYPE_STREAM))
#define LAR_STREAM_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), LAR_TYPE_STREAM, LarStreamClass))

typedef struct _LarStreamClass   LarStreamClass;
typedef struct _LarStream        LarStream;
typedef struct _LarStreamPrivate LarStreamPrivate;

struct _LarStreamClass {
    GObjectClass parent_class;
};

struct _LarStream {
    GObject parent_instance;

    LarStreamPrivate *priv;
};

GType lar_stream_get_type(void) G_GNUC_CONST;
void lar_stream_print_test(LarStream *device);
const gchar *lar_stream_get_product(LarStream *device);
const gchar *lar_stream_get_serial(LarStream *device);
const gchar *lar_stream_get_id(LarStream *device);
const gchar *lar_stream_get_discription(LarStream *device);
const gchar *lar_stream_get_mac(LarStream *device);
LarStream *lar_stream_new_from_json(const gchar *json_body);

G_END_DECLS

#endif /* _LAR_STREAM_H_ */
