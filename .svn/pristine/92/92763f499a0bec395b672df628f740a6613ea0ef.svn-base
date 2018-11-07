/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * injection.h
 * Copyright (C) LAR 2017
 *
 */

#ifndef _INJECTION_H_
#define _INJECTION_H_

#include <axis.h>
#include <glib-object.h>

#define TYPE_INJECTION (injection_get_type())
#define INJECTION(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_INJECTION, Injection))
#define INJECTION_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_INJECTION, InjectionClass))
#define IS_INJECTION(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_INJECTION))
#define IS_INJECTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_INJECTION))
#define INJECTION_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_INJECTION, InjectionClass))

typedef struct _InjectionClass InjectionClass;
typedef struct _Injection      Injection;

GType injection_get_type(void) G_GNUC_CONST;
guint InjectionGetAir(Injection *injection);
guint InjectionGetRest(Injection *injection);
guint InjectionGetFurnaceAir(Injection *injection);
guint InjectionGetDilution(Injection *injection);
guint InjectionGetRinsing(Injection *injection);
guint InjectionGetPush(Injection *injection);
guint InjectionGetSamplePull(Injection *injection);
guint InjectionGetRinsingPull(Injection *injection);
guint InjectionGetRinsingPush(Injection *injection);

#endif /* _INJECTION_H_ */
