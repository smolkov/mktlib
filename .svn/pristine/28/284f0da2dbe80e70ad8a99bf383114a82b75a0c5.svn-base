/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * device.h
 * Copyright (C) 2017 LAR
 *
 *
 */
#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define TYPE_DEVICE (device_get_type())
#define DEVICE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_DEVICE, Device))
#define DEVICE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_DEVICE, DeviceClass))
#define IS_DEVICE(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_DEVICE))
#define IS_DEVICE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_DEVICE))
#define DEVICE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_DEVICE, DeviceClass))

typedef struct _DeviceClass   DeviceClass;
typedef struct _Device        Device;
typedef struct _DevicePrivate DevicePrivate;

struct _DeviceClass {
    GObjectClass parent_class;
};

struct _Device {
    GObject parent_instance;

    DevicePrivate *priv;
};

GType        device_get_type(void) G_GNUC_CONST;
void         device_print_test(Device *device);
const gchar *device_get_product(Device *device);
const gchar *device_get_serial(Device *device);
void         device_check_serial(Device *device, const gchar *old_serial);
const gchar *device_get_id(Device *device);
const gchar *device_get_discription(Device *device);
gint64       device_get_created(Device *device);
gint64       device_get_updated(Device *device);
Device *     device_new_from_json(const gchar *json_body);
gboolean     device_get_autostart(Device *device);
guint        device_get_run_counter(Device *device);
void         device_up_run_counter(Device *device);

G_END_DECLS

#endif /* _DEVICE_H_ */
