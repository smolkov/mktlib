/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktData
 * @ingroup  MktData
 * @{
 * @file  mkt-data.h	Data interface model header
 * @brief This is Data interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */
#ifndef MKT_DATA_H_
#define MKT_DATA_H_


#include "mkt-model.h"

G_BEGIN_DECLS

#define MKT_TYPE_DATA                  (mkt_data_get_type ())
#define MKT_DATA(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj),MKT_TYPE_DATA, MktData))
#define MKT_IS_DATA(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj),MKT_TYPE_DATA))
#define MKT_DATA_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_DATA, MktDataInterface))


typedef struct _MktDataInterface MktDataInterface;
typedef struct _MktData MktData;



enum
{
	MKT_DATA_TYPE_TEMP   = 1<<1,
	MKT_DATA_TYPE_RESULT = 1<<2,
};

struct _MktDataInterface
{
	GTypeInterface parent_iface;
	gdouble                 (*data_changed)                      ( MktData *data );
	gdouble                 (*data_value)                        ( MktData *data );
	guint                   (*data_trigger)                      ( MktData *data );
	gint                    (*data_type)                         ( MktData *data );
};


GType                   mkt_data_get_type                        (void) G_GNUC_CONST;
gdouble                 mkt_data_changed                  ( MktData *data );
gdouble                 mkt_data_value                    ( MktData *data );
guint                   mkt_data_trigger                  ( MktData *data );
gint                    mkt_data_type                     ( MktData *data );


G_END_DECLS

#endif /* MKT_DATA_H_ */
