/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * larredis.c
 * Copyright (C) LAR 2017
 *

 */

#ifndef _REDIS_BINDING_H_
#define _REDIS_BINDING_H_

#include <glib-object.h>
// #include <hiredis.h>


#define TYPE_REDIS_BINDING (redis_binding_get_type())
#define REDIS_BINDING(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_REDIS_BINDING, RedisBinding))
#define IS_REDIS_BINDING(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_REDIS_BINDING))

typedef struct _RedisBindingClass RedisBindingClass;
typedef struct _RedisBinding      RedisBinding;

GType redis_binding_get_type(void) G_GNUC_CONST;

#define TYPE_REDIS_OBJECT (redis_object_get_type())
#define REDIS_OBJECT(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_REDIS_OBJECT, RedisObject))
#define REDIS_OBJECT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_REDIS_OBJECT, RedisObjectClass))
#define IS_REDIS_OBJECT(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_REDIS_OBJECT))
#define IS_REDIS_OBJECT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_REDIS_OBJECT))
#define REDIS_OBJECT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_REDIS_OBJECT, RedisObjectClass))

typedef struct _RedisObjectClass RedisObjectClass;
typedef struct _RedisObject      RedisObject;

GType redis_object_get_type(void) G_GNUC_CONST;

gpointer RedisNewLocalConnection(struct timeval tv);


RedisBinding *NewRedisKey(const gchar *redisKey, const gchar *defaultValue, const gchar *description);
RedisBinding *NewRedisKeyFull(gpointer rConn, const gchar *redisKey, const gchar *defaultValue, const gchar *description);
RedisBinding *NewRedisHash(const gchar *redisKey, const gchar *field, const gchar *defaultValue, const gchar *description);
RedisBinding *NewRedisHashFull(gpointer rConn, const gchar *redisKey, const gchar *field, const gchar *defaultValue, const gchar *description);
RedisObject *NewRedisObject(const gchar *redisKey, gpointer object, const gchar *description);
RedisObject *NewRedisObjectFull(gpointer rConn, const gchar *redisKey, gpointer object, const gchar *description);

GBinding *RedisBindProperty(RedisBinding *redis, gpointer object, const gchar *name, GBindingFlags flags);
gboolean RedisValueSet(RedisBinding *redis, const gchar *value);
const gchar *RedisValueGetString(RedisBinding *redis, GError **error);
gint64 RedisValueGetInt(RedisBinding *redis, GError **error);
gdouble RedisValueGetDouble(RedisBinding *redis, GError **error);

gboolean RedisValueDel(RedisBinding *redis);
gboolean RedisValueUpdate(RedisBinding *redis, GError **error);

gboolean RedisObjectDel(RedisObject *object);

void RedisCloseConnection();
G_END_DECLS

#endif /* _REDIS_BINDING_H_ */
