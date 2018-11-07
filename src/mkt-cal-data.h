/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktCalData
 * @ingroup  MktCalData
 * @{
 * @file  mkt-nitri_cal_data.h	NitriStream interface model header
 * @brief This is NitriStream interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */
#ifndef MKT_CAL_DATA_H_
#define MKT_CAL_DATA_H_


#include "mkt-model.h"


G_BEGIN_DECLS

#define MKT_TYPE_CAL_DATA                  (mkt_cal_data_get_type ())
#define MKT_CAL_DATA(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj),MKT_TYPE_CAL_DATA, MktCalData))
#define MKT_IS_CAL_DATA(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj),MKT_TYPE_CAL_DATA))
#define MKT_CAL_DATA_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_CAL_DATA, MktCalDataInterface))


typedef struct _MktCalDataInterface MktCalDataInterface;
typedef struct _MktCalData MktCalData;


struct _MktCalDataInterface
{
	GTypeInterface parent_iface;

	gdouble              (*cal_data_value)              ( MktCalData *self );
    gboolean             (*cal_data_outlier)            ( MktCalData *self );
    guint                (*cal_data_trigger)            ( MktCalData *self );
    guint                (*cal_data_replicate)          ( MktCalData *self );
    guint64              (*cal_data_point)              ( MktCalData *self );
};


GType                   mkt_cal_data_get_type                                     ( void ) G_GNUC_CONST;

gdouble              	mkt_cal_data_value                                        ( MktCalData *ucal_data );
gboolean              	mkt_cal_data_outlier                                      ( MktCalData *ucal_data );
guint                   mkt_cal_data_trigger                                      ( MktCalData *ucal_data );
guint                   mkt_cal_data_replicate                                    ( MktCalData *ucal_data );
guint64                 mkt_cal_data_point                                        ( MktCalData *ucal_data );




G_END_DECLS

#endif /* MKT_CAL_DATA_H_ */
