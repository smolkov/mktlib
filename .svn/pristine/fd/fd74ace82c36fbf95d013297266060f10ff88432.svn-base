/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktCalibration
 * @ingroup  MktCalibration
 * @{
 * @file  mkt-calibration-object.h	Default calibration model header
 * @brief Volständige Beschreibung...
 * 	 Copyright (C) LAR 2014 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef _MKT_CALIBRATION_MODEL_H_
#define _MKT_CALIBRATION_MODEL_H_

#include <mkt-model.h>
#include <mkt-param-object.h>
#include "mkt-calibration.h"


G_BEGIN_DECLS

#define MKT_TYPE_CALIBRATION_MODEL             (mkt_calibration_model_get_type ())
#define MKT_CALIBRATION_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_CALIBRATION_MODEL, MktCalibrationModel))
#define MKT_CALIBRATION_MODEL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass),  MKT_TYPE_CALIBRATION_MODEL, MktCalibrationModelClass))
#define MKT_IS_CALIBRATION_MODEL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_CALIBRATION_MODEL))
#define MKT_IS_CALIBRATION_MODEL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass),  MKT_TYPE_CALIBRATION_MODEL))
#define MKT_CALIBRATION_MODEL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj),  MKT_TYPE_CALIBRATION_MODEL, MktCalibrationModelClass))

typedef struct _MktCalibrationModelClass     MktCalibrationModelClass;
typedef struct _MktCalibrationModel          MktCalibrationModel;
typedef struct _MktCalibrationModelPrivate   MktCalibrationModelPrivate;


struct _MktCalibrationModelClass
{
	MktParamObjectClass                   parent_class;
	gboolean                             (*add_value )             ( MktCalibrationModel *object, gdouble value);
	gboolean                             (*activate )              ( MktCalibrationModel *object, gboolean service);
	gboolean                             (*activate_id )           ( MktCalibrationModel *object, guint64 ref_id , gboolean service);

};

struct _MktCalibrationModel
{
	MktParamObject                        parent_instance;
	MktCalibrationModelPrivate           *priv;
};

GType                   mkt_calibration_model_get_type                 (void) G_GNUC_CONST;

G_END_DECLS

#endif /* _MKT_CALIBRATION_MODEL_H_ */


/** @} */
