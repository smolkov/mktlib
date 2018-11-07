/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * larcalibration.h
 * Copyright (C) LAR 2017
 * autor Smolkov

 */
#ifndef _LAR_CALIBRATION_H_
#define _LAR_CALIBRATION_H_

#include <glib-object.h>


G_BEGIN_DECLS

#define TYPE_LAR_CALIBRATION             (lar_calibration_get_type ())
#define LAR_CALIBRATION(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_LAR_CALIBRATION, LarCalibration))
#define LAR_CALIBRATION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass),  TYPE_LAR_CALIBRATION, LarCalibrationClass))
#define IS_LAR_CALIBRATION(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_LAR_CALIBRATION))
#define IS_LAR_CALIBRATION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass),  TYPE_LAR_CALIBRATION))
#define LAR_CALIBRATION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj),  TYPE_LAR_CALIBRATION, LarCalibrationClass))

typedef struct _LarCalibrationClass     LarCalibrationClass;
typedef struct _LarCalibration          LarCalibration;
typedef struct _LarCalibrationPrivate   LarCalibrationPrivate;


struct _LarCalibrationClass
{
	GObjectClass                   parent_class;
};

struct _LarCalibration
{
	GObject                        parent_instance;
	LarCalibrationPrivate           *priv;
};

GType                   lar_calibration_get_type                 (void) G_GNUC_CONST;

G_END_DECLS

#endif /* _LAR_CALIBRATION_H_ */


/** @} */
