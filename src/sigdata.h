/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * sigdata.h
 * Copyright (C) LAR 2017
 *

 */

#ifndef _SIGDATA_H_
#define _SIGDATA_H_

#include <glib-object.h>


#define TYPE_SIGDATA (sigdata_get_type())
#define SIGDATA(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_SIGDATA, Sigdata))
#define SIGDATA_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_SIGDATA, SigdataClass))
#define IS_SIGDATA(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_SIGDATA))
#define IS_SIGDATA_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_SIGDATA))
#define SIGDATA_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_SIGDATA, SigdataClass))

typedef struct _SigdataClass   SigdataClass;
typedef struct _Sigdata        Sigdata;
typedef struct _SigdataPrivate SigdataPrivate;

struct _SigdataClass {
    GObjectClass parent_class;
};

struct _Sigdata {
    GObject                 parent_instance;
    SigdataPrivate *priv;
};

GType sigdata_get_type(void) G_GNUC_CONST;



gdouble SigdataGetSecond(Sigdata *sigdata);
gdouble SigdataGetValue(Sigdata *sigdata);


#endif /* _SIGDATA_H_ */
