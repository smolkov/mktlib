/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * ultrameas.h
 * Copyright (C) LAR 2017
 *

 */

#ifndef _ULTRAMEAS_H_
#define _ULTRAMEAS_H_

#include <glib.h>
#include "larintgrec.h"
#include "larmeasurement.h"

G_BEGIN_DECLS

#define TYPE_ULTRAMEAS (ultrameas_get_type())
#define ULTRAMEAS(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_ULTRAMEAS, Ultrameas))
#define ULTRAMEAS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_ULTRAMEAS, UltrameasClass))
#define IS_ULTRAMEAS(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_ULTRAMEAS))
#define IS_ULTRAMEAS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_ULTRAMEAS))
#define ULTRAMEAS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_ULTRAMEAS, UltrameasClass))

typedef struct _UltrameasClass UltrameasClass;
typedef struct _Ultrameas Ultrameas;
typedef struct _UltrameasPrivate UltrameasPrivate;

struct _UltrameasClass
{
    GObjectClass parent_class;
};

struct _Ultrameas
{
    GObject parent_instance;
    UltrameasPrivate *priv;
};

GType ultrameas_get_type(void) G_GNUC_CONST;

Ultrameas *UltrameasNew(LarIntgrec *rec,gdouble offset,gdouble slope,gdouble factor);
Ultrameas *UltrameasNewFromValue(gdouble value,gdouble factor);


gint64 m_UltrameasGetCreated(Ultrameas *ultrameas);
gboolean m_UltrameasIsOutlier(Ultrameas *ultrameas);
void m_UltrameasSetOutlier(Ultrameas *ultrameas,gboolean isOutlier);
gdouble m_UltrameasGetValue(Ultrameas *ultrameas);
LarIntgrec *m_UltrameasGetLarIntegration(Ultrameas *ultrameas);

G_END_DECLS

#endif /* _ULTRAMEAS_H_ */