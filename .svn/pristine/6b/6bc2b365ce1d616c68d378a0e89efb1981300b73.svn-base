/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktError
 * @{
 * @file  mkt-error.c	Pc model interface
 * @brief This is ERROR model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */

#include "market-time.h"
#include "mkt-enum-types.h"
#include "mkt-error-message.h"
#include "mkt-error-message.h"
#include "mkt-error.h"
#include "mkt-value.h"
#include "mkt-log.h"
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

static gchar *SERVICE_ID = NULL;
/* signals */
enum
{
    DATE_CHANGED,
    LAST_SIGNAL
};

// static guint mkt_error_iface_signals[LAST_SIGNAL];

static void mkt_error_base_init(gpointer g_iface)
{
    static gboolean is_initialized = FALSE;
    MUTEX_LOCK();
    if (!is_initialized)
    {
        GParamSpec *pspec;
        pspec = g_param_spec_int("error-number", "Error system number prop", "Set|Get message property", 0, G_MAXINT32, 0, G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | MKT_MODEL_DB_PROP);
        g_object_interface_install_property(g_iface, pspec);

        pspec = g_param_spec_boolean("error-pending", "Error pending state property", "Set|Get pending state property", FALSE, G_PARAM_READWRITE | MKT_MODEL_DB_PROP);
        g_object_interface_install_property(g_iface, pspec);

        pspec = g_param_spec_char("error-type", "Error message type", "Set|Get message type", 0, 40, 0, G_PARAM_READWRITE | MKT_MODEL_DB_PROP);
        g_object_interface_install_property(g_iface, pspec);

        g_object_interface_install_property(g_iface, g_param_spec_double("error-changed", "Error changed date in double", "Set changed date in double seconds.nano-seconds property", 0., G_MAXDOUBLE,
                                                                         0., G_PARAM_READWRITE | MKT_MODEL_DB_PROP));
        pspec = g_param_spec_string("error-description", "Error description prop", "Set|Get error description's ", "Set error description here", G_PARAM_READWRITE | MKT_MODEL_DB_PROP);
        g_object_interface_install_property(g_iface, pspec);

        pspec = g_param_spec_string("error-service", "Error service prop", "Set|Get error service ", "com.lar.tera.general", G_PARAM_READWRITE | MKT_MODEL_DB_PROP);
        g_object_interface_install_property(g_iface, pspec);

        is_initialized = TRUE;
    }
    MUTEX_UNLOCK();
}

GType mkt_error_get_type(void)
{
    static GType iface_type = 0;
    if (iface_type == 0)
    {
        static const GTypeInfo info = {sizeof(MktErrorInterface), (GBaseInitFunc)mkt_error_base_init, (GBaseFinalizeFunc)NULL, (GClassInitFunc)NULL, NULL, NULL, 0, 0, (GInstanceInitFunc)NULL, 0};
        MUTEX_LOCK();
        if (iface_type == 0)
        {
            iface_type = g_type_register_static(G_TYPE_INTERFACE, "MktErrorInterface", &info, 0);
        }
        MUTEX_UNLOCK();
    }
    return iface_type;
}

gint mkt_error_number(MktError *error)
{
    g_return_val_if_fail(error != NULL, 0);
    g_return_val_if_fail(MKT_IS_ERROR(error), 0);
    if (MKT_ERROR_GET_INTERFACE(error)->get_number)
        return MKT_ERROR_GET_INTERFACE(error)->get_number(error);
    return 0;
}

gboolean mkt_error_pending(MktError *error)
{
    g_return_val_if_fail(error != NULL, FALSE);
    g_return_val_if_fail(MKT_IS_ERROR(error), FALSE);
    if (MKT_ERROR_GET_INTERFACE(error)->get_pending)
        return MKT_ERROR_GET_INTERFACE(error)->get_pending(error);
    return FALSE;
}
gchar mkt_error_type(MktError *error)
{
    g_return_val_if_fail(error != NULL, 0);
    g_return_val_if_fail(MKT_IS_ERROR(error), 0);
    if (MKT_ERROR_GET_INTERFACE(error)->get_type)
        return MKT_ERROR_GET_INTERFACE(error)->get_type(error);
    return 1;
}

gdouble mkt_error_changed(MktError *error)
{
    g_return_val_if_fail(error != NULL, 0.);
    g_return_val_if_fail(MKT_IS_ERROR(error), 0.);
    if (MKT_ERROR_GET_INTERFACE(error)->get_changed)
        return MKT_ERROR_GET_INTERFACE(error)->get_changed(error);
    return 0.0;
}

gchar *mkt_error_description(MktError *error)
{
    g_return_val_if_fail(error != NULL, NULL);
    g_return_val_if_fail(MKT_IS_ERROR(error), NULL);
    if (MKT_ERROR_GET_INTERFACE(error)->description)
        return MKT_ERROR_GET_INTERFACE(error)->description(error);
    return NULL;
}

void mkt_error_set_type(MktError *error, gchar type)
{
    g_return_if_fail(error != NULL);
    g_return_if_fail(MKT_IS_ERROR(error));
    gdouble td = market_db_time_now();
    g_object_set(error, "error-changed", td, "error-type", type, NULL);
}

void mkt_error_set_pending(MktError *error, gboolean value)
{
    g_return_if_fail(error != NULL);
    g_return_if_fail(MKT_IS_ERROR(error));
    gdouble td = market_db_time_now();
    g_object_set(error, "error-changed", td, "error-pending", value, NULL);
}

void mkt_error_set_description(MktError *error, const gchar *format, ...)
{
    g_return_if_fail(error != NULL);
    g_return_if_fail(MKT_IS_ERROR(error));
    g_return_if_fail(format != NULL);
    va_list args;
    gchar *description;
    va_start(args, format);
    description = g_strdup_vprintf(format, args);
    va_end(args);
    gdouble td = market_db_time_now();
    g_object_set(error, "error-changed", td, "error-description", description, NULL);
    g_free(description);
}

gboolean mkt_error_exist(gint number)
{
    g_return_val_if_fail(number > 0, FALSE);
    gboolean isExist = FALSE;
    MktModel *model = NULL;
    model = mkt_model_select_one(MKT_TYPE_ERROR_MESSAGE, "select * from $tablename where error_number = %d", number);
    if (model)
    {
        isExist = TRUE;
        g_object_unref(model);
    }
    return isExist;
}

void mkt_error_reset(gint number)
{
    g_return_if_fail(number > 0);
    MktModel *model = NULL;
    model = mkt_model_select_one(MKT_TYPE_ERROR_MESSAGE, "select * from $tablename where error_number = %d", number);
    if (model)
    {
        gdouble td = market_db_time_now();
        mktMPSet(model, "error-changed", td);
        mktMPSet(model, "error-pending", FALSE);
        g_object_unref(model);
    }
}

gboolean mkt_error_create(gint number, gchar type, const gchar *description)
{
    g_return_val_if_fail(number > 0, FALSE);
    g_return_val_if_fail(description != NULL, FALSE);
    MktModel *model = mkt_model_select_one(MKT_TYPE_ERROR_MESSAGE, "select * from $tablename where error_number = %d", number);
    if (model == NULL)
    {
        model = mkt_model_new(MKT_TYPE_ERROR_MESSAGE, "error-number", number, "error-pending", FALSE, "error-type", type, "error-description", description, "error-service", mkt_error_service(), NULL);
        g_object_unref(model);
    }
    else
    {
        g_object_set(model, "error-type", type, "error-description", description, "error-pending", FALSE, NULL);
        g_object_unref(model);
    }
    return TRUE;
}
gboolean mkt_error_createv(gint number, gchar type, const gchar *format, ...)
{
    va_list args;
    gchar *description;
    va_start(args, format);
    description = g_strdup_vprintf(format, args);
    va_end(args);
    mkt_error_create(number, type, description);
    g_free(description);
    return TRUE;
}

MktError *mkt_error_find(gint number)
{
    g_return_val_if_fail(number > 0, NULL);
    return MKT_ERROR(mkt_model_select_one(MKT_TYPE_ERROR_MESSAGE, "select * from $tablename where error_number = %d", number));
}

void mkt_error_change_type(gint number, MktErrorType type)
{
    MktError *error = mkt_error_find(number);
    if (error)
    {
        g_object_set(error, "error-type", type, NULL);
        g_object_unref(error);
    }
}

MktModel *mkt_error_get_came(gint error)
{
    MktModel *model = NULL;
    if (error != 0)
    {
        gchar *note = g_strdup_printf("E%d", error);
        model = mkt_model_select_one(MKT_TYPE_ERROR_MESSAGE, "select * from $tablename where error_number = %d", error);
        if (model)
        {
            if (!mkt_error_pending(MKT_ERROR(model)))
            {
                gchar *emsg = g_strdup_printf("E%d %s came", error, mkt_error_description(MKT_ERROR(model)));
                gdouble td = market_db_time_now();
                mktMPSet(model, "error-changed", td);
                mktMPSet(model, "error-pending", TRUE);
                // MktModel *msg = mkt_model_new(MKT_TYPE_LOG_MESSAGE, "log-message", emsg, "log-type", MKT_LOG_MESSAGE_ERRORS, "log-note", note, NULL);
                // g_object_unref(msg);
                g_free(emsg);
            }
        }
        else
        {
            gchar *emsg = g_strdup_printf("new E%d %s came", error, mkt_error_description(MKT_ERROR(model)));
            model = mkt_model_new(MKT_TYPE_ERROR_MESSAGE, "error-number", error, "error-pending", TRUE, "error-service", mkt_error_service(), NULL);
            // MktModel *msg = mkt_model_new(MKT_TYPE_LOG_MESSAGE, "log-message", emsg, "log-type", MKT_LOG_MESSAGE_ERRORS, "log-note", note, NULL);
            // g_object_unref(msg);
            g_free(emsg);
        }
        g_free(note);
    }
    else
    {
        g_warning("Error number %d do not allow", error);
    }
    return model;
}

void mkt_error_came(gint error)
{
    MktModel *model = mkt_error_get_came(error);
    if (model)
        g_object_unref(model);
}

void mkt_error_gone(gint error)
{
    GSList *models = mkt_model_select(MKT_TYPE_ERROR_MESSAGE, "select * from $tablename where error_number = %d", error);
    GSList *l = NULL;
    for (l = models; l != NULL; l = l->next)
    {
        if (mkt_error_pending(MKT_ERROR(l->data)))
        {
            gdouble td = market_db_time_now();
            mktMPSet(l->data, "error-changed", td);
            mktMPSet(l->data, "error-pending", FALSE);
            if (MKT_IS_ERROR_MESSAGE(l->data))
            {
                gchar *emsg = g_strdup_printf("E%d %s gone", error, mkt_error_description(MKT_ERROR(l->data)));
                // MktModel *msg  = mkt_model_new(MKT_TYPE_LOG_MESSAGE, "log-message", emsg, "log-type", MKT_LOG_MESSAGE_ERRORS, NULL);
                // g_object_unref(msg);
                g_free(emsg);
            }
        }
    }
    mkt_slist_free_full(models, g_object_unref);
}

static MktModel *mkt_error_get_came_simple(gint error, const gchar *description)
{
    MktModel *model = NULL;

    if (error != 0)
    {

        model = mkt_model_select_one(MKT_TYPE_ERROR_MESSAGE, "select * from $tablename where error_number = %d", error);
        if (model)
        {
            if (!mkt_error_pending(MKT_ERROR(model)))
            {
                gdouble td = market_db_time_now();
                mktMPSet(model, "error-changed", td);
                mktMPSet(model, "error-pending", TRUE);
                mktMPSet(model, "error-description", description);
            }
        }
        else
        {
            model = mkt_model_new(MKT_TYPE_ERROR_MESSAGE, "error-number", error, "error-pending", TRUE, "error-service", mkt_error_service(), "error-description", description, NULL);
        }
    }
    else
    {
        g_warning("Error number %d do not allow", error);
    }
    return model;
}

void mkt_error_came_simple(gint error, const gchar *format, ...)
{
    va_list args;
    gchar *description;
    va_start(args, format);
    description = g_strdup_vprintf(format, args);
    va_end(args);
    MktModel *model = mkt_error_get_came_simple(error, description);
    if (model)
        g_object_unref(model);
    g_free(description);
}

gboolean mkt_error_pending_number(gint number)
{
    gboolean pending = FALSE;
    MktModel *model = mkt_model_select_one(MKT_TYPE_ERROR_MESSAGE, "select * from $tablename where error_number = %d", number);
    if (model != NULL)
    {
        pending = mkt_error_pending(MKT_ERROR(model));
        g_object_unref(model);
    }

    return pending;
}

void mkt_error_manually_gone(gint error)
{
    GSList *models = mkt_model_select(MKT_TYPE_ERROR_MESSAGE, "select * from $tablename where error_number = %d", error);
    GSList *l = NULL;
    for (l = models; l != NULL; l = l->next)
    {
        if (mkt_error_pending(MKT_ERROR(l->data)))
        {
            gdouble td = market_db_time_now();
            mktMPSet(l->data, "error-changed", td);
            mktMPSet(l->data, "error-pending", FALSE);
            // if (MKT_IS_ERROR_MESSAGE(l->data)) {
            // gchar *   emsg = g_strdup_printf("E%d %s manually gone", error, mkt_error_description(MKT_ERROR(l->data)));
            // MktModel *msg  = mkt_model_new(MKT_TYPE_LOG_MESSAGE, "log-message", emsg, "log-type", MKT_LOG_MESSAGE_ERRORS, NULL);
            // g_object_unref(msg);
            // g_free(emsg);
            // }
        }
    }
    mkt_slist_free_full(models, g_object_unref);
}

/*
 *
 *
 * @return new allocated string array ( E1:E2:E3 )
 */

gchar *mkt_error_dup()
{
    GString *errstr = g_string_new("");
    GSList *errors = mkt_model_select(MKT_TYPE_ERROR_MESSAGE, "select * from $tablename where error_pending = 1 ORDER BY error_number ASC");
    GSList *l = NULL;
    for (l = errors; l != NULL; l = l->next)
    {
        g_string_append_printf(errstr, "%sE%d", errstr->len > 1 ? " " : "", mkt_error_number(MKT_ERROR(l->data)));
    }
    if (errors)
    {
        mkt_slist_free_full(errors, g_object_unref);
    }
    gchar *ret = errstr->str;
    g_string_free(errstr, FALSE);
    return ret;
}

/*
 *
 *
 * @return new allocated string array ( E1:E2:E3 ) only for type...
 */

gchar *mkt_error_dup_type(guint type)
{
    GString *errstr = g_string_new("");
    GSList *errors = mkt_model_select(MKT_TYPE_ERROR_MESSAGE, "select * from $tablename where error_pending = 1 and error_type = %d", type);
    GSList *l = NULL;
    for (l = errors; l != NULL; l = l->next)
    {
        g_string_append_printf(errstr, "%sE%d", errstr->len > 1 ? " " : "", mkt_error_number(MKT_ERROR(l->data)));
    }
    if (errors)
    {
        mkt_slist_free_full(errors, g_object_unref);
    }
    gchar *ret = errstr->str;
    g_string_free(errstr, FALSE);
    return ret;
}

gboolean mkt_error_is_critical()
{
    gboolean is_critical = FALSE;
    GSList *errors = mkt_model_select(MKT_TYPE_ERROR_MESSAGE, "select * from $tablename where error_pending = 1 and error_type = %d", MKT_ERROR_CRITICAL);
    if (errors)
    {
        is_critical = TRUE;
        mkt_slist_free_full(errors, g_object_unref);
    }
    return is_critical;
}

const gchar *mkt_error_service(void)
{
    if (SERVICE_ID == NULL)
        SERVICE_ID = g_strdup("com.lar.tera.general");
    return SERVICE_ID;
}

void mkt_error_set_service(const gchar *service)
{
    g_return_if_fail(service != NULL);
    if (SERVICE_ID != NULL)
        g_free(SERVICE_ID);
    SERVICE_ID = g_strdup(service);
}

static void error_came(MktError *error, const gchar *description)
{
    if (!mkt_error_pending(error))
    {
        g_object_set(error, "error-pending", TRUE, NULL);
        gchar *note = g_strdup_printf("E%d", mkt_error_number(error));
        mkt_log_error_message_with_note(note, "E%d %s", mkt_error_number(error), description);
        g_free(note);
    }
}

static void error_clean(MktError *error)
{
    g_return_if_fail(error != NULL);
    g_return_if_fail(MKT_IS_ERROR(error));
    if (mkt_error_pending(error))
    {
        mkt_error_set_pending(error, FALSE);
        gchar *note = g_strdup_printf("E%d", mkt_error_number(error));
        mkt_log_error_message_with_note(note, "E%d gone", mkt_error_number(error));
        g_free(note);
    }
}

static GEnumClass *errors_enums = NULL;
static GEnumClass *critical_errors_enums = NULL;

void mkt_errors_init(gboolean clean)
{
    if (errors_enums == NULL)
    {
        errors_enums = g_type_class_ref(MKT_TYPE_ERRORS_NUMBERS);
        critical_errors_enums = g_type_class_ref(MKT_TYPE_ERRORS_CRITICAL);
        // mkt_model_delete_async(MKT_TYPE_SENSOR_DATA, NULL, NULL, NULL, "delete from MktSensorData where data_creator=%" G_GUINT64_FORMAT, mkt_model_ref_id(MKT_IMODEL(channel->priv->channel)));

        if (errors_enums && errors_enums->n_values)
        {
            GEnumValue *enum_value;
            for (enum_value = errors_enums->values; enum_value->value_name; enum_value++)
            {
                MktErrorType type = MKT_ERROR_WARNING;
                if (g_enum_get_value(critical_errors_enums, enum_value->value))
                    type = MKT_ERROR_CRITICAL;
                MktError *error = mkt_error_find(enum_value->value);
                if (error == NULL)
                {
                    error = MKT_ERROR(mkt_model_new(MKT_TYPE_ERROR_MESSAGE, "error-number", enum_value->value, "error-pending", FALSE, "error-type", type, "error-description",
                                                    enum_value->value_nick, "error-service", mkt_error_service(), NULL));
                }
                if (clean)
                {
                    error_clean(error);
                }
                mkt_error_set_description(error, "%s", enum_value->value_nick);
                g_object_unref(error);
            }
        }
    }
}

G_DEFINE_QUARK(lar - error, mkt_error)

gboolean mkt_errors_report(MktErrorsNumbers number, const gchar *format, ...)
{
    mkt_errors_init(FALSE);
    va_list args;
    gchar *description;
    va_start(args, format);
    description = g_strdup_vprintf(format, args);
    va_end(args);
    MktError *model = mkt_error_find(number);
    gboolean reported = FALSE;
    if (model)
    {
        error_came(model, description);
        reported = TRUE;
        g_object_unref(model);
    }
    g_free(description);
    return reported;
}

gboolean mkt_errors_come(MktErrorsNumbers number)
{
    mkt_errors_init(FALSE);
    MktError *model = mkt_error_find(number);
    gboolean reported = FALSE;
    if (model)
    {
        error_came(model, mkt_error_description(model));
        reported = TRUE;
        g_object_unref(model);
    }
    return reported;
}

void mkt_errors_clean(MktErrorsNumbers number)
{
    mkt_errors_init(FALSE);
    MktError *model = mkt_error_find(number);
    if (model)
    {
        error_clean(model);
        g_object_unref(model);
    }
}

/** @} */
