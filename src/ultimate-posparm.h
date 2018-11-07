/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup UltimateLibrary
 * @defgroup UltimatePosparm
 * @ingroup  UltimatePosparm
 * @{
 * @file  mkt-nitri_stream.h	NitriStream interface model header
 * @brief This is NitriStream interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */
#ifndef ULTIMATE_POSPARM_H_
#define ULTIMATE_POSPARM_H_



#include "mkt-model.h"
#include "mkt-decl.h"


G_BEGIN_DECLS

#define ULTIMATE_TYPE_POSPARM                  (ultimate_posparm_get_type ())
#define ULTIMATE_POSPARM(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj),ULTIMATE_TYPE_POSPARM, UltimatePosparm))
#define ULTIMATE_IS_POSPARM(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj),ULTIMATE_TYPE_POSPARM))
#define ULTIMATE_POSPARM_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE ((inst), ULTIMATE_TYPE_POSPARM, UltimatePosparmInterface))


typedef struct _UltimatePosparmInterface UltimatePosparmInterface;
typedef struct _UltimatePosparm UltimatePosparm;


struct _UltimatePosparmInterface
{
	GTypeInterface parent_iface;

};


GType                            ultimate_posparm_get_type                                     ( void ) G_GNUC_CONST;


G_END_DECLS

#endif /* ULTIMATE_POSPARM_H_ */
/** @} */
