/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * relay.h
 * Copyright (C) LAR 2017
 *

 */

#ifndef _RELAY_H_
#define _RELAY_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define TYPE_RELAY (relay_get_type())
#define RELAY(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_RELAY, Relay))
#define RELAY_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_RELAY, RelayClass))
#define IS_RELAY(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_RELAY))
#define IS_RELAY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_RELAY))
#define RELAY_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_RELAY, RelayClass))

typedef struct _RelayClass   RelayClass;
typedef struct _Relay        Relay;
typedef struct _RelayPrivate RelayPrivate;

struct _RelayClass {
    GObjectClass parent_class;
};

struct _Relay {
    GObject       parent_instance;
    RelayPrivate *priv;
};

GType relay_get_type(void) G_GNUC_CONST;

G_END_DECLS

const gchar* RelayGetBoolExpression(Relay *relay);

#endif /* _RELAY_H_ */
