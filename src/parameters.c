/* -*- mode: c; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 2 -*- */

/*
 * parameters.c
 * Copyright (C) 2017 LAR
 *
 */

#include "parameters.h"
#include <gio/gio.h>
#include <glib/gstdio.h>
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>
#include <glib/gprintf.h>

#include <string.h>

struct _ParametersPrivate {
    gchar * name;
    gchar * path;
    gchar * sqlitePath;
    gchar * recoveryPath;
    Device *device;
    GMutex  m;
    gchar *filePath;
};

enum { PROP_0, PROP_NAME, PROP_PATH, PROP_SQLITE, PROP_RECOVERY };

static void lock(Parameters *parameters) { g_mutex_lock(&parameters->priv->m); }
static void unlock(Parameters *parameters) { g_mutex_unlock(&parameters->priv->m); }

G_DEFINE_TYPE(Parameters, parameters, G_TYPE_OBJECT);

static void parameters_init(Parameters *parameters) {
    parameters->priv = G_TYPE_INSTANCE_GET_PRIVATE(parameters, TYPE_PARAMETERS, ParametersPrivate);
    g_mutex_init(&parameters->priv->m);
    parameters->priv->name         = g_strdup("device");
    parameters->priv->path         = g_build_path("/", g_get_user_config_dir(), ".lar", NULL);
    parameters->priv->sqlitePath   = NULL;
    parameters->priv->recoveryPath = NULL;
    parameters->priv->device       = NULL;

    /* TODO: Add initialization code here */
}

static void parameters_finalize(GObject *object) {
    Parameters *parameters = PARAMETERS(object);
    if (parameters->priv->path) g_free(parameters->priv->path);
    if (parameters->priv->name) g_free(parameters->priv->name);
    if (parameters->priv->sqlitePath) g_free(parameters->priv->sqlitePath);
    if (parameters->priv->recoveryPath) g_free(parameters->priv->recoveryPath);
    if (parameters->priv->filePath) g_free(parameters->priv->filePath);
    if (parameters->priv->device) g_object_unref(parameters->priv->device);
    g_mutex_clear(&parameters->priv->m);
    // g_mutex_

    G_OBJECT_CLASS(parameters_parent_class)->finalize(object);
}

static void parameters_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_PARAMETERS(object));
    Parameters *parameters = PARAMETERS(object);
    switch (prop_id) {
    case PROP_NAME:
        if (parameters->priv->name) g_free(parameters->priv->name);
        parameters->priv->name = g_value_dup_string(value);
        break;
    case PROP_PATH:
        if (parameters->priv->path) g_free(parameters->priv->path);
        parameters->priv->path = g_value_dup_string(value);
        break;
    case PROP_SQLITE:
        if (parameters->priv->sqlitePath) g_free(parameters->priv->sqlitePath);
        parameters->priv->sqlitePath = g_value_dup_string(value);
        break;
    case PROP_RECOVERY:
        if (parameters->priv->recoveryPath) g_free(parameters->priv->recoveryPath);
        parameters->priv->recoveryPath = g_value_dup_string(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void parameters_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    g_return_if_fail(IS_PARAMETERS(object));
    Parameters *parameters = PARAMETERS(object);

    switch (prop_id) {
    case PROP_NAME:
        g_value_set_string(value, parameters->priv->name);
        break;
    case PROP_PATH:
        g_value_set_string(value, parameters->priv->path);
        break;
    case PROP_SQLITE:
        g_value_set_string(value, parameters->priv->sqlitePath);
        break;
    case PROP_RECOVERY:
        g_value_set_string(value, parameters->priv->recoveryPath);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void parameters_class_init(ParametersClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    g_type_class_add_private(klass, sizeof(ParametersPrivate));

    object_class->finalize     = parameters_finalize;
    object_class->set_property = parameters_set_property;
    object_class->get_property = parameters_get_property;

    g_object_class_install_property(object_class, PROP_NAME, g_param_spec_string("name", "name", "name", "device", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
    g_object_class_install_property(object_class, PROP_PATH, g_param_spec_string("path", "path", "path", "/tmp/.lar", G_PARAM_READABLE | G_PARAM_WRITABLE));

    g_object_class_install_property(object_class, PROP_SQLITE, g_param_spec_string("sqlite", "sqlite", "sqlite", "/usr/share/tera/db", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
    g_object_class_install_property(object_class, PROP_RECOVERY, g_param_spec_string("recovery", "recovery", "recovery", "none", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
}

const gchar *parameters_get_path(Parameters *parameters) {
    g_return_val_if_fail(parameters != NULL, NULL);
    return parameters->priv->path;
}

/**
 * parameters_get_device:
 * @parameters: a #Parameters
 *
 * Get parameters device object .
 *
 * Returns: #Device
 */
Device *parameters_get_device(Parameters *parameters) {
    g_return_val_if_fail(parameters != NULL, NULL);
    g_return_val_if_fail(IS_PARAMETERS(parameters), NULL);
    return parameters->priv->device;
}


/**
 * parameter_config_file:
 * @parameters: a #Parameters
 *
 * Get parameters config file path.
 *
 * Returns: gchar*
 */
const gchar *parameters_get_config_file(Parameters *parameters) {
    if (parameters->priv->filePath) {
        g_free(parameters->priv->filePath);
    }
    parameters->priv->filePath = g_strdup_printf("%s/%s.json", parameters->priv->path, parameters->priv->name);
    return parameters->priv->filePath;
}

/**
 * parameters_new:
 * @name: a #const gchar*
 *
 * Created new parameters object
 *
 * Returns: #Parameters
 */
Parameters *parameters_new(const gchar *path, const gchar *name, GObject *object) {
    Parameters *param = NULL;
    param             = PARAMETERS(g_object_new(TYPE_PARAMETERS, "path", path, "name", name, NULL));
    return param;
}

/**
 * parameters_read:
 * @parameters: a #Parameters
 *
 * Read parameter from file
 *
 * Returns: TRUE if parameter file is readed
 */
gboolean parameters_read(Parameters *parameters) {
    g_return_val_if_fail(parameters != NULL, FALSE);

    lock(parameters);
    size_t len = 0;
    gchar *content = NULL;
    if( !g_file_get_contents(parameters_get_config_file(parameters),&content,&len,NULL))
    {
        GError *error = NULL;
        gchar *data;
        gsize len;
        data = json_gobject_to_data (G_OBJECT (parameters_get_device(parameters)), &len);
        gboolean res = g_file_set_contents(parameters_get_config_file(parameters),data,len,&error);
        if(data)g_free(data);
        if (error) {
            g_warning("Parameters create new file - %s",error->message);
            g_error_free(error);
        }
        return res;
    }

    // GObject *object = json_gobject_from_data(G_OBJECT_TYPE(parameters_get_device(parameters)),content,len,NULL);
    unlock(parameters);
    return TRUE;
}

/**
 * parameters_write:
 * @parameters: a #Parameters
 *
 * Write parameter to file
 *
 * Returns: TRUE if parameter file is writed
 */
gboolean parameters_write(Parameters *parameters) {
    g_return_val_if_fail(parameters != NULL, FALSE);

    lock(parameters);
    unlock(parameters);
    return TRUE;
}
