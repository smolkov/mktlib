/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktPoint
 * @ingroup  MktPoint
 * @{
 * @file  mkt-nitri_point.h	NitriStream interface model header
 * @brief This is NitriStream interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */
#ifndef _MKT_POINT_H_
#define _MKT_POINT_H_


#include "mkt-model.h"
#include "mkt-cal-data.h"


G_BEGIN_DECLS

#define MKT_TYPE_POINT                  (mkt_point_get_type ())
#define MKT_POINT(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj),MKT_TYPE_POINT, MktPoint))
#define MKT_IS_POINT(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj),MKT_TYPE_POINT))
#define MKT_POINT_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_POINT, MktPointInterface))


typedef struct _MktPointInterface  MktPointInterface;
typedef struct _MktPoint           MktPoint;


struct _MktPointInterface
{
	GTypeInterface parent_iface;

	gdouble             (*point_average)                                      ( MktPoint *self );
    gdouble             (*point_solution)                                     ( MktPoint *self );
    gdouble             (*point_cv)                                           ( MktPoint *self );
};


GType                   mkt_point_get_type                                    ( void ) G_GNUC_CONST;
gdouble              	mkt_point_average                                     ( MktPoint *upoint );
gdouble              	mkt_point_solution                                    ( MktPoint *upoint );
gdouble              	mkt_point_cv                                          ( MktPoint *upoint );


G_END_DECLS

#endif /* _MKT_POINT_H_ */
