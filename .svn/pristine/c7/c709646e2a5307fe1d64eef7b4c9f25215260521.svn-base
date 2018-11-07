/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * posparm.h
 * Copyright (C) LAR 2017 
 *

 */

#ifndef _POSPARM_H_
#define _POSPARM_H_

#include <glib-object.h>



#define TYPE_POSPARM                 (posparm_get_type ())
#define POSPARM(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_POSPARM, Posparm))
#define POSPARM_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass),  TYPE_POSPARM, PosparmClass))
#define IS_POSPARM(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_POSPARM))
#define IS_POSPARM_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass),  TYPE_POSPARM))
#define POSPARM_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj),  TYPE_POSPARM, PosparmClass))

typedef struct _PosparmClass     PosparmClass;
typedef struct _Posparm          Posparm;
typedef struct _PosparmPrivate   PosparmPrivate;


struct _PosparmClass
{
	GObjectClass                    parent_class;
};

struct _Posparm
{
	GObject                         parent_instance;
	PosparmPrivate          *priv;
};



GType                   posparm_get_type                 (void) G_GNUC_CONST;

const gchar* PosparmGetOnlineVessel(Posparm *posparm);
const gchar* PosparmGetDrainVessel(Posparm *posparm);
const gchar* PosparmGetCalibrationVessel(Posparm *posparm);
const gchar* PosparmGetSingleVessel(Posparm *posparm);
const gchar* PosparmGetCheckVessel(Posparm *posparm);
const gchar* PosparmGetDilutionVessel(Posparm *posparm);

#endif /* _POSPARM_H_ */



/** @} */
