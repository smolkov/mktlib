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

#ifndef _LAR_DEVICE_H_
#define _LAR_DEVICE_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define LAR_TYPE_DEVICE (lar_device_get_type())
#define LAR_DEVICE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), LAR_TYPE_DEVICE, LarDevice))
#define LAR_DEVICE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), LAR_TYPE_DEVICE, LarDeviceClass))
#define LAR_IS_DEVICE(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), LAR_TYPE_DEVICE))
#define LAR_IS_DEVICE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), LAR_TYPE_DEVICE))
#define LAR_DEVICE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), LAR_TYPE_DEVICE, LarDeviceClass))

typedef struct _LarDeviceClass   LarDeviceClass;
typedef struct _LarDevice        LarDevice;
typedef struct _LarDevicePrivate LarDevicePrivate;

struct _LarDeviceClass {
    GObjectClass parent_class;
};

struct _LarDevice {
    GObject parent_instance;

    LarDevicePrivate *priv;
};

GType lar_device_get_type(void) G_GNUC_CONST;
void lar_device_print_test(LarDevice *device);
const gchar *lar_device_get_product(LarDevice *device);
const gchar *lar_device_get_serial(LarDevice *device);
const gchar *lar_device_get_id(LarDevice *device);
const gchar *lar_device_get_discription(LarDevice *device);
const gchar *lar_device_get_mac(LarDevice *device);
LarDevice *lar_device_new_from_json(const gchar *json_body);



G_END_DECLS

#endif /* _LAR_DEVICE_H_ */
