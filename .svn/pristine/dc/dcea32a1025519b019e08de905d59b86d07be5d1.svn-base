/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mkt-sensor-data.c
 * Copyright (C) sascha 2013 <sascha@sascha-VirtualBox>
 * 
mkt-sensor-data.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * mkt-sensor-data.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _ULTIMATE_POSPARM_OBJECT_H_
#define _ULTIMATE_POSPARM_OBJECT_H_


#include "mkt-param-object.h"
#include "ultimate-posparm.h"


G_BEGIN_DECLS

#define ULTIMATE_TYPE_POSPARM_OBJECT                 (ultimate_posparm_object_get_type ())
#define ULTIMATE_POSPARM_OBJECT(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), ULTIMATE_TYPE_POSPARM_OBJECT, UltimatePosparmObject))
#define ULTIMATE_POSPARM_OBJECT_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass),  ULTIMATE_TYPE_POSPARM_OBJECT, UltimatePosparmObjectClass))
#define ULTIMATE_IS_POSPARM_OBJECT(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ULTIMATE_TYPE_POSPARM_OBJECT))
#define ULTIMATE_IS_POSPARM_OBJECT_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass),  ULTIMATE_TYPE_POSPARM_OBJECT))
#define ULTIMATE_POSPARM_OBJECT_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj),  ULTIMATE_TYPE_POSPARM_OBJECT, UltimatePosparmObjectClass))

typedef struct _UltimatePosparmObjectClass     UltimatePosparmObjectClass;
typedef struct _UltimatePosparmObject          UltimatePosparmObject;
typedef struct _UltimatePosparmObjectPrivate   UltimatePosparmObjectPrivate;


struct _UltimatePosparmObjectClass
{
	MktParamObjectClass                    parent_class;
};

struct _UltimatePosparmObject
{
	MktParamObject                         parent_instance;
	UltimatePosparmObjectPrivate          *priv;
};



GType                   ultimate_posparm_object_get_type                 (void) G_GNUC_CONST;



G_END_DECLS

#endif /* _ULTIMATE_POSPARM_OBJECT_H_ */



/** @} */
