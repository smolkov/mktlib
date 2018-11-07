/* -*- mode: c; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 * larparameters.h
 * Copyright (C) 2017 LAR
 *
 */

#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

#include "device.h"
#include <glib-object.h>

#define TYPE_PARAMETERS (parameters_get_type())
#define PARAMETERS(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_PARAMETERS, Parameters))
#define PARAMETERS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_PARAMETERS, ParametersClass))
#define IS_PARAMETERS(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_PARAMETERS))
#define IS_PARAMETERS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_PARAMETERS))
#define PARAMETERS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_PARAMETERS, ParametersClass))

typedef struct _ParametersClass   ParametersClass;
typedef struct _Parameters        Parameters;
typedef struct _ParametersPrivate ParametersPrivate;

struct _ParametersClass {
    GObjectClass parent_class;
};

struct _Parameters {
    GObject parent_instance;

    ParametersPrivate *priv;
};

GType parameters_get_type(void) G_GNUC_CONST;

Device *parameters_get_device(Parameters *parameters);
const gchar *parameters_get_config_file(Parameters *parammeters);
const gchar *parameters_get_path(Parameters *parammeters);


Parameters* parameters_new(const gchar *path,const gchar *name,GObject *object );



#endif /* _PARAMETERS_H_ */

