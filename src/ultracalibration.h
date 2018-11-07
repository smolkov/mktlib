/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * ultracalibration.h
 * Copyright (C) LAR 2017
 *

 */

#ifndef _ULTRA_CALIBRATION_H_
#define _ULTRA_CALIBRATION_H_

#include <glib.h>
#include "larcalibration.h"
#include "ultrapoint.h"
#include "ultrameas.h"
#include "statistic.h"
G_BEGIN_DECLS

#define TYPE_ULTRA_CALIBRATION (ultra_calibration_get_type())
#define ULTRA_CALIBRATION(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_ULTRA_CALIBRATION, UltraCalibration))
#define ULTRA_CALIBRATION_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_ULTRA_CALIBRATION, UltraCalibrationClass))
#define IS_ULTRA_CALIBRATION(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_ULTRA_CALIBRATION))
#define IS_ULTRA_CALIBRATION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_ULTRA_CALIBRATION))
#define ULTRA_CALIBRATION_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_ULTRA_CALIBRATION, UltraCalibrationClass))

typedef struct _UltraCalibrationClass UltraCalibrationClass;
typedef struct _UltraCalibration UltraCalibration;
typedef struct _UltraCalibrationPrivate UltraCalibrationPrivate;

struct _UltraCalibrationClass
{
    LarCalibrationClass parent_class;
};

struct _UltraCalibration
{
    LarCalibration parent_instance;
    UltraCalibrationPrivate *priv;
};

GType ultra_calibration_get_type(void) G_GNUC_CONST;
UltraCalibration *UltraCalibrationNew(Statistic *statistic);
GSList *m_UltraCalibrationGetPoints(UltraCalibration *ultraCal);
gboolean m_UltraCalibrationAddMeas(UltraCalibration *ultraCal, Ultrameas *meas);
gboolean m_UltraCalibrationNextPoint(UltraCalibration *ultraCal);
gboolean m_UltraCalibrationCalculateCurrentPoint(UltraCalibration *ultraCal);
void m_UltraCalibrationCalculate(UltraCalibration *ultraCal);

G_END_DECLS

#endif /* _ULTRA_CALIBRATION_H_ */