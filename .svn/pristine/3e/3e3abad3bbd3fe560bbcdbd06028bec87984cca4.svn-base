/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * dilution.h
 * Copyright (C) LAR 2017
 *
 */

#ifndef _DILUTION_H_
#define _DILUTION_H_

#include <glib-object.h>

#define TYPE_DILUTION (dilution_get_type())
#define DILUTION(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_DILUTION, Dilution))
#define DILUTION_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_DILUTION, DilutionClass))
#define IS_DILUTION(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_DILUTION))
#define IS_DILUTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_DILUTION))
#define DILUTION_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_DILUTION, DilutionClass))

typedef struct _DilutionClass DilutionClass;
typedef struct _Dilution      Dilution;

GType dilution_get_type(void) G_GNUC_CONST;

gdouble DilutionGetVolume(Dilution *dilution);
guint DilutionGetTakePosition(Dilution *dilution);
guint DilutionGetPushPosition(Dilution *dilution);
guint DilutionGetRepeat(Dilution *dilution);

#endif /* _DILUTION_H_ */
