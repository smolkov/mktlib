/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * larintgrec.h
 * Copyright (C) LAR 2017
 * 
 */

#ifndef _LAR_INTGREC_H_
#define _LAR_INTGREC_H_

#include <glib-object.h>
#include "larintegration.h"

#define ARRAY_TYPE_BOXED (array_boxed_get_type())
GType array_boxed_get_type(void) G_GNUC_CONST;

#define TYPE_LAR_INTGREC (larintgrec_get_type())
#define LAR_INTGREC(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_LAR_INTGREC, LarIntgrec))
#define LAR_INTGREC_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_LAR_INTGREC, LarIntgrecClass))
#define IS_LAR_INTGREC(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_LAR_INTGREC))
#define IS_LAR_INTGREC_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_LAR_INTGREC))
#define LAR_INTGREC_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_LAR_INTGREC, LarIntgrecClass))

typedef struct _LarIntgrecClass LarIntgrecClass;
typedef struct _LarIntgrec LarIntgrec;

GType larintgrec_get_type(void) G_GNUC_CONST;

LarIntgrec *LarIntgrecNew(LarIntegration *integration);

gint m_LarIntgrecGetStartZero(LarIntgrec *larintgrec);
gint m_LarIntgrecGetStopZero(LarIntgrec *larintgrec);

gint m_LarIntgrecGetRunIngPos(LarIntgrec *larintgrec);
gint m_LarIntgrecGetStart(LarIntgrec *larintgrec);
gint m_LarIntgrecGetStop(LarIntgrec *larintgrec);

gdouble m_LarIntgrecGetZero(LarIntgrec *larintgrec);
gdouble m_LarIntgrecGetResult(LarIntgrec *larintgrec);
GArray *m_LarIntgrecGetData(LarIntgrec *larintgrec);
gboolean m_LarIntgrecAddValue(LarIntgrec *larintgrec, gdouble value);

void m_LarIntgrecStartLastZero(LarIntgrec *larintgrec);
void m_LarIntgrecStopLastZero(LarIntgrec *larintgrec);
void m_LarIntgrecRunIntegration(LarIntgrec *larintgrec);
void m_LarIntgrecStartIntegration(LarIntgrec *larintgrec);
void m_LarIntgrecStopIntegration(LarIntgrec *larintgrec);

gdouble m_LarIntgrecCalculateIntegration(LarIntgrec *larintgrec);
void m_LarIntgrecCalculateZero(LarIntgrec *larintgrec);

void m_LarIntgrecSetPar(LarIntgrec *larintgrec, LarIntegration *integration);
LarIntegration *m_LarIntgrecGetPar(LarIntgrec *larintgrec);

#endif /* _LAR_INTGREC_H_ */
