/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * larredis.c
 * Copyright (C) LAR 2017
 *
 */

#include "redis.h"
// #include <adapters/glib.h>
#include <glib/gprintf.h>

#include "value.h"

#include <hiredis.h>
enum
{
    PROP_0,
    PROP_REDIS_VALUE,
    PROP_REDIS_DEFAULT_VALUE,
    PROP_REDIS_VALUE_DESCRIPTION,
    PROP_REDIS_KEY,
    PROP_REDIS_FIELD,
};
enum
{
    PROP_OBJECT_0,
    PROP_REDIS_OBJECT,
    PROP_REDIS_OBJECT_KEY,
    PROP_REDIS_OBJECT_DESCRIPTION,
};

struct _RedisBindingClass
{
    GObjectClass parent_class;
};

typedef enum { REDIS_KEY = 1,
               REDIS_HASHES = 2 } RedisCommand;

struct _RedisBinding
{
    GObject parent_instance;
    gchar *key;
    gchar *field;
    gchar *value;
    gchar *defVal;
    gchar *description;
    RedisCommand command;
    gboolean blockUpload;
    gboolean globalConnection;
    redisContext *rConn;
};

#define REDIS_BINDING_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_REDIS_BINDING, RedisBindingClass))
#define IS_REDIS_BINDING_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_REDIS_BINDING))
#define REDIS_BINDING_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_REDIS_BINDING, RedisBindingClass))

struct _RedisObjectClass
{
    GObjectClass parent_class;
};

struct _RedisObject
{
    GObject parent_instance;
    gchar *key;
    GObject *object;
    gboolean blockUpload;
    gchar *description;
    gboolean globalConnection;
    redisContext *rConn;
};

static GQuark redis_error_quark(void)
{
    static GQuark error;
    if (!error)
        error = g_quark_from_static_string("redis-error");
    return error;
}

static redisContext *_rConn = NULL;

static void redisOpenConnection()
{
    struct timeval timeout = {0, 500000}; // 0.5 seconds
    _rConn = redisConnectWithTimeout("127.0.0.1", 6379, timeout);
    if (_rConn == NULL || _rConn->err)
    {
        if (_rConn)
        {
            g_error("Internal data base connection error: %s\n", _rConn->errstr);
            redisFree(_rConn);
        }
        else
        {
            g_error("Parameter storage error: can't allocate redis context\n");
        }
    }
}

static redisContext *RedisConnection()
{
    if (_rConn == NULL)
    {
        redisOpenConnection();
    }
    return _rConn;
}

void RedisCloseConnection()
{
    if (_rConn != NULL)
    {
        redisFree(_rConn);
        _rConn = NULL;
    }
}

G_DEFINE_TYPE(RedisBinding, redis_binding, G_TYPE_OBJECT)

static void redis_binding_init(RedisBinding *redis_binding)
{
    redis_binding->blockUpload = FALSE;
    redis_binding->command = REDIS_KEY;
    redis_binding->value = g_strdup("0");
    redis_binding->key = g_strdup("some:key");
    redis_binding->rConn = RedisConnection();
    redis_binding->globalConnection = TRUE;
    /* TODO: Add initialization code here */
}

G_DEFINE_TYPE(RedisObject, redis_object, G_TYPE_OBJECT)

static void redis_object_init(RedisObject *redis_object)
{
    redis_object->blockUpload = FALSE;
    redis_object->key = g_strdup("some:key");
    redis_object->object = NULL;
    redis_object->rConn = RedisConnection();
    redis_object->globalConnection = TRUE;

    /* TODO: Add initialization code here */
}

gchar *getRedisValue(RedisBinding *redis)
{
    redisReply *reply = NULL;
    gchar *value = NULL;
    if (redis->field != NULL && g_utf8_strlen(redis->field, 50) > 0)
    {
        reply = redisCommand(redis->rConn, "HMGET %s %s", redis->key, redis->field);

        if (reply != NULL && reply->type == REDIS_REPLY_ARRAY && reply->elements > 0 && reply->element[0]->str != NULL)
        {
            value = g_strdup(reply->element[0]->str);
        }
    }
    else
    {
        reply = redisCommand(redis->rConn, "GET %s", redis->key);
        if (reply != NULL && (reply->type == REDIS_REPLY_STRING || reply->type == REDIS_REPLY_INTEGER) && reply->str != NULL)
        {
            value = g_strdup(reply->str);
        }
    }
    if (reply)
        freeReplyObject(reply);
    return value;
}

static gboolean bindingSet(RedisBinding *redis)
{
    redisReply *reply = NULL;
    if (redis->field != NULL && g_utf8_strlen(redis->field, 50) > 0)
    {
        reply = redisCommand(redis->rConn, "HMSET %s %s %s", redis->key, redis->field, redis->value);
    }
    else
    {
        reply = redisCommand(redis->rConn, "SET %s %s", redis->key, redis->value);
    }
    if (reply)
    {
        freeReplyObject(reply);
        return TRUE;
    }
    return FALSE;
}

static gboolean bindingUpdate(RedisBinding *redis)
{
    gchar *val = getRedisValue(redis);
    if (val)
    {
        redis->blockUpload = TRUE;
        g_object_set(redis, "value", val, NULL);
        redis->blockUpload = FALSE;
        g_free(val);
        return TRUE;
    }
    g_object_set(redis, "value", redis->defVal, NULL);
    return FALSE;
}

static void redis_binding_constructed(GObject *object)
{
    bindingUpdate(REDIS_BINDING(object));
    if (G_OBJECT_CLASS(redis_binding_parent_class)->constructed)
        G_OBJECT_CLASS(redis_binding_parent_class)->constructed(object);
}

static void redis_binding_finalize(GObject *object)
{
    RedisBinding *redis = REDIS_BINDING(object);
    if (redis->value)
        g_free(redis->value);
    if (redis->key)
        g_free(redis->key);
    if (redis->description)
        g_free(redis->description);
    if (redis->field)
        g_free(redis->field);
    if (redis->defVal)
        g_free(redis->defVal);
    if (!redis->globalConnection && redis->rConn != NULL)
        redisFree(redis->rConn);
    G_OBJECT_CLASS(redis_binding_parent_class)->finalize(object);
}

static void redis_binding_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_REDIS_BINDING(object));
    RedisBinding *redis = REDIS_BINDING(object);
    switch (prop_id)
    {
    case PROP_REDIS_VALUE:
        if (redis->value)
            g_free(redis->value);
        redis->value = g_value_dup_string(value);
        if (!redis->blockUpload)
        {
            bindingSet(redis);
        }
        break;
    case PROP_REDIS_DEFAULT_VALUE:
        if (redis->defVal)
            g_free(redis->defVal);
        redis->defVal = g_value_dup_string(value);
        break;
    case PROP_REDIS_VALUE_DESCRIPTION:
        if (redis->description)
            g_free(redis->description);
        redis->description = g_value_dup_string(value);
        break;
    case PROP_REDIS_KEY:
        if (redis->key)
            g_free(redis->key);
        redis->key = g_value_dup_string(value);
        break;
    case PROP_REDIS_FIELD:
        if (redis->field)
            g_free(redis->field);
        redis->field = g_value_dup_string(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void redis_binding_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_REDIS_BINDING(object));
    RedisBinding *stream = REDIS_BINDING(object);
    switch (prop_id)
    {
    case PROP_REDIS_VALUE:
        g_value_set_string(value, stream->value);
        break;
    case PROP_REDIS_DEFAULT_VALUE:
        g_value_set_string(value, stream->defVal);
        break;
    case PROP_REDIS_VALUE_DESCRIPTION:
        g_value_set_string(value, stream->description);
        break;
    case PROP_REDIS_KEY:
        g_value_set_string(value, stream->key);
        break;
    case PROP_REDIS_FIELD:
        g_value_set_string(value, stream->field);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void redis_binding_class_init(RedisBindingClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // Class* parent_class = MODEL_CLASS (klass);
    object_class->finalize = redis_binding_finalize;
    object_class->set_property = redis_binding_set_property;
    object_class->get_property = redis_binding_get_property;
    object_class->constructed = redis_binding_constructed;

    g_object_class_install_property(object_class, PROP_REDIS_VALUE, g_param_spec_string("value", "binding value", "binding value", "0", G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_REDIS_DEFAULT_VALUE,
                                    g_param_spec_string("default-value", "binding default value", "binding default value", "some value", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
    g_object_class_install_property(object_class, PROP_REDIS_VALUE_DESCRIPTION,
                                    g_param_spec_string("description", "parameter description", "parameter lo description", "0", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
    g_object_class_install_property(
        object_class, PROP_REDIS_KEY, g_param_spec_string("key", "binding value", "binding value", "some:key", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
    g_object_class_install_property(object_class, PROP_REDIS_FIELD, g_param_spec_string("field", "binding value", "binding value", "", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

void redisObjectSend(RedisObject *redisObject, const gchar *name, const gchar *value)
{
    redisReply *reply = NULL;
    reply = redisCommand(redisObject->rConn, "HMSET %s %s %s", redisObject->key, name, value);
    if (reply)
        freeReplyObject(reply);
}

static void redisSetProperty(RedisObject *redisObject, const gchar *name, const gchar *value)
{
    if (redisObject->object == NULL)
        return;
    GParamSpec *pspec = g_object_class_find_property(G_OBJECT_GET_CLASS(redisObject->object), name);
    if (pspec == NULL)
        return;
    if (g_type_is_a(pspec->value_type, G_TYPE_OBJECT))
        return;
    GValue *gval = value_new(pspec->value_type);
    if (set_gvalue_from_string(gval, value))
    {
        g_object_set_property(redisObject->object, name, gval);
    }
    value_free(gval);
}

static void redisObjectInit(RedisObject *redisObject)
{
    redisReply *reply = NULL;
    reply = redisCommand(redisObject->rConn, "HGETALL %s", redisObject->key);
    if (reply)
    {
        if (reply->type == REDIS_REPLY_ARRAY)
        {
            gint j = 0;
            for (j = 0; j < reply->elements; j++)
            {
                const gchar *pname = reply->element[j]->str;
                if (j + 1 < reply->elements)
                {
                    const gchar *value = reply->element[j + 1]->str;
                    redisSetProperty(redisObject, pname, value);
                }
            }
        }
        freeReplyObject(reply);
    }
}

void redis_object_notiry(GObject *gobject, GParamSpec *pspec, RedisObject *redisObject)
{
    if (g_type_is_a(pspec->value_type, G_TYPE_OBJECT))
        return;
    GValue *gval = value_new(pspec->value_type);
    g_object_get_property(gobject, pspec->name, gval);
    gchar *valStr = value_stringify(gval);
    if (valStr)
    {
        redisObjectSend(redisObject, pspec->name, valStr);
        g_free(valStr);
    }
    value_free(gval);
}

static void redis_object_constructed(GObject *object)
{
    RedisObject *redisObject = REDIS_OBJECT(object);
    if (redisObject->object)
    {
        redisObjectInit(redisObject);
        g_signal_connect(redisObject->object, "notify", G_CALLBACK(redis_object_notiry), redisObject);
    }
    if (G_OBJECT_CLASS(redis_object_parent_class)->constructed)
        G_OBJECT_CLASS(redis_object_parent_class)->constructed(object);
}

static void redis_object_finalize(GObject *object)
{
    RedisObject *redis = REDIS_OBJECT(object);
    if (redis->object)
        g_object_unref(redis->object);
    if (redis->key)
        g_free(redis->key);
    if (redis->description)
        g_free(redis->description);
    if (!redis->globalConnection && redis->rConn != NULL)
        redisFree(redis->rConn);

    G_OBJECT_CLASS(redis_object_parent_class)->finalize(object);
}

static void redis_object_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_REDIS_OBJECT(object));
    RedisObject *redisObject = REDIS_OBJECT(object);
    switch (prop_id)
    {
    case PROP_REDIS_OBJECT:
        redisObject->object = g_value_dup_object(value);
        break;
    case PROP_REDIS_OBJECT_DESCRIPTION:
        if (redisObject->description)
            g_free(redisObject->description);
        redisObject->description = g_value_dup_string(value);
        break;
    case PROP_REDIS_OBJECT_KEY:
        if (redisObject->key)
            g_free(redisObject->key);
        redisObject->key = g_value_dup_string(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void redis_object_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_REDIS_OBJECT(object));
    RedisObject *redisObject = REDIS_OBJECT(object);
    switch (prop_id)
    {
    case PROP_REDIS_OBJECT:
        g_value_set_object(value, redisObject->object);
        break;
    case PROP_REDIS_OBJECT_DESCRIPTION:
        g_value_set_string(value, redisObject->description);
        break;
    case PROP_REDIS_OBJECT_KEY:
        g_value_set_string(value, redisObject->key);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void redis_object_class_init(RedisObjectClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // Class* parent_class = MODEL_CLASS (klass);
    object_class->finalize = redis_object_finalize;
    object_class->set_property = redis_object_set_property;
    object_class->get_property = redis_object_get_property;
    object_class->constructed = redis_object_constructed;

    g_object_class_install_property(
        object_class, PROP_REDIS_OBJECT, g_param_spec_object("object", "object value", "object value", G_TYPE_OBJECT, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
    g_object_class_install_property(object_class, PROP_REDIS_OBJECT_DESCRIPTION,
                                    g_param_spec_string("description", "parameter description", "parameter lo description", "0", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
    g_object_class_install_property(
        object_class, PROP_REDIS_OBJECT_KEY, g_param_spec_string("key", "object value", "object value", "some:key", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

gpointer RedisNewLocalConnection(struct timeval tv)
{
    redisContext *conn = redisConnectWithTimeout("127.0.0.1", 6379, tv);
    if (conn == NULL || conn->err)
    {
        if (conn)
        {
            g_warning("Connection error: %s\n", conn->errstr);
            redisFree(conn);
            return NULL;
        }
        else
        {
            g_warning("Connection error: can't allocate redis context\n");
            return NULL;
        }
    }
    return conn;
}


RedisBinding *NewRedisKey(const gchar *redisKey, const gchar *defaultValue, const gchar *description)
{
    RedisBinding *redis = REDIS_BINDING(g_object_new(TYPE_REDIS_BINDING, "key", redisKey, "default-value", defaultValue, "description", description, NULL));
    redis->rConn = RedisConnection();
    return redis;
}

RedisBinding *NewRedisKeyFull(gpointer rConn, const gchar *redisKey, const gchar *defaultValue, const gchar *description)
{
    RedisBinding *redis = REDIS_BINDING(g_object_new(TYPE_REDIS_BINDING, "key", redisKey, "default-value", defaultValue, "description", description, NULL));
    redis->rConn = (redisContext*)rConn;
    redis->globalConnection = FALSE;
    return redis;
}

RedisBinding *NewRedisHash(const gchar *redisKey, const gchar *field, const gchar *defaultValue, const gchar *description)
{
    RedisBinding *redis = REDIS_BINDING(g_object_new(TYPE_REDIS_BINDING, "key", redisKey, "field", field, "default-value", defaultValue, "description", description, NULL));
    redis->rConn = RedisConnection();
    return redis;
}
RedisBinding *NewRedisHashFull(gpointer rConn, const gchar *redisKey, const gchar *field, const gchar *defaultValue, const gchar *description)
{
    RedisBinding *redis = REDIS_BINDING(g_object_new(TYPE_REDIS_BINDING, "key", redisKey, "field", field, "default-value", defaultValue, "description", description, NULL));
    redis->rConn = (redisContext*)rConn;
    redis->globalConnection = FALSE;
    return redis;
}

RedisObject *NewRedisObject(const gchar *redisKey, gpointer object, const gchar *description)
{
    RedisObject *redisObject = REDIS_OBJECT(g_object_new(TYPE_REDIS_OBJECT, "key", redisKey, "object", object, "description", description, NULL));
    redisObject->rConn = RedisConnection();
    return redisObject;
}
RedisObject *NewRedisObjectFull(gpointer rConn, const gchar *redisKey, gpointer object, const gchar *description)
{
    RedisObject *redisObject = REDIS_OBJECT(g_object_new(TYPE_REDIS_OBJECT, "key", redisKey, "object", object, "description", description, NULL));
    redisObject->rConn = (redisContext*)rConn;
    redisObject->globalConnection = FALSE;
    return redisObject;
}

static gboolean binding_transform_to(GBinding *binding, const GValue *from_value, GValue *to_value, gpointer user_data) { return set_gvalue_from_string(to_value, g_value_get_string(from_value)); }

static gboolean binding_transform_from(GBinding *binding, const GValue *from_value, GValue *to_value, gpointer user_data)
{
    gchar *newStrVal = value_stringify(from_value);
    g_value_set_string(to_value, newStrVal);
    if (newStrVal)
        g_free(newStrVal);
    return TRUE;
}
GBinding *RedisBindProperty(RedisBinding *redis, gpointer object, const gchar *name, GBindingFlags flags)
{
    return g_object_bind_property_full(redis, "value", object, name, flags, binding_transform_to, binding_transform_from, NULL, NULL);
}

gboolean RedisValueDel(RedisBinding *redis)
{
    redisReply *reply = NULL;
    reply = redisCommand(redis->rConn, "DEL %s", redis->key);
    if (reply)
    {
        freeReplyObject(reply);
        return TRUE;
    }

    return FALSE;
}

gboolean RedisValueUpdate(RedisBinding *redis, GError **error)
{
    gchar *val = getRedisValue(redis);
    if (val == NULL)
    {
        if (error != NULL)
        {
            GError *newError = g_error_new(redis_error_quark(), 1, "get %s value fail - %s", redis->key, redis->rConn->errstr);
            error = &newError;
        }
        g_object_set(redis, "value", redis->defVal, NULL);
        return FALSE;
    }
    redis->blockUpload = TRUE;
    g_object_set(redis, "value", val, NULL);
    redis->blockUpload = FALSE;
    g_free(val);
    return TRUE;
}

gboolean RedisValueSet(RedisBinding *redis, const gchar *value)
{
    g_return_val_if_fail(redis != NULL, FALSE);
    g_return_val_if_fail(value != NULL, FALSE);
    g_return_val_if_fail(IS_REDIS_BINDING(redis), FALSE);
    g_object_set(redis, "value", value, NULL);
    return TRUE;
}

const gchar *RedisValueGetString(RedisBinding *redis, GError **error)
{
    gchar *val = getRedisValue(redis);
    if (val == NULL)
    {
        if (error != NULL)
        {
            GError *newError = g_error_new(redis_error_quark(), 1, "get %s value fail - %s", redis->key, redis->rConn->errstr);
            error = &newError;
        }
        return redis->defVal;
    }
    redis->blockUpload = TRUE;
    g_object_set(redis, "value", val, NULL);
    redis->blockUpload = FALSE;
    g_free(val);
    return redis->value;
}

gint64 RedisValueGetInt(RedisBinding *redis, GError **error)
{
    const gchar *val = RedisValueGetString(redis, error);
    GValue *temp = value_new(G_TYPE_INT64);
    if (!set_gvalue_from_string(temp, val))
    {
        if (error != NULL)
        {
            GError *newError = g_error_new(redis_error_quark(), 2, "%s - %s can not to int convertes", redis->key, val);
            error = &newError;
        }
    }
    gint64 res = g_value_get_int64(temp);
    value_free(temp);
    return res;
}

gdouble RedisValueGetDouble(RedisBinding *redis, GError **error)
{
    const gchar *val = RedisValueGetString(redis, error);
    GValue *temp = value_new(G_TYPE_DOUBLE);
    if (!set_gvalue_from_string(temp, val))
    {
        if (error != NULL)
        {
            GError *newError = g_error_new(redis_error_quark(), 2, "%s - %s can not to double convertes", redis->key, val);
            error = &newError;
        }
    }
    gdouble res = g_value_get_double(temp);
    value_free(temp);
    return res;
}

gboolean RedisObjectDel(RedisObject *object)
{
    redisReply *reply = NULL;
    reply = redisCommand(object->rConn, "del %s", object->key);
    if (reply)
    {
        freeReplyObject(reply);
        return TRUE;
    }

    return FALSE;
}
