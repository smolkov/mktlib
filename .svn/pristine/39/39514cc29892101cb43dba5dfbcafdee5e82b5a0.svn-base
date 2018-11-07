/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktAxis
 * @ingroup  MktAxis
 * @{
 * @file  mkt-axis.h	Axis interface model header
 * @brief This is Axis interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef _MKT_AXIS_H_
#define _MKT_AXIS_H_

#include "mkt-decl.h"
#include "mkt-model.h"

G_BEGIN_DECLS

#define MKT_TYPE_AXIS (mkt_axis_get_type())
#define MKT_AXIS(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), MKT_TYPE_AXIS, MktAxis))
#define MKT_IS_AXIS(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), MKT_TYPE_AXIS))
#define MKT_AXIS_GET_INTERFACE(inst) (G_TYPE_INSTANCE_GET_INTERFACE((inst), MKT_TYPE_AXIS, MktAxisInterface))

typedef struct _MktAxisInterface MktAxisInterface;
typedef struct _MktAxis          MktAxis;

struct _MktAxisInterface {
    GTypeInterface parent_iface;
};

GType mkt_axis_get_type(void) G_GNUC_CONST;

G_END_DECLS

#endif /* MKT_AXIS_H_ */

/** @} */
