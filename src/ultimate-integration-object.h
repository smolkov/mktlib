/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mkt-integration-data.c
 * Copyright (C) sascha 2013 <sascha@sascha-VirtualBox>
 * 
mkt-integration-data.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * mkt-integration-data.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _ULTIMATE_INTEGRATION_OBJECT_H_
#define _ULTIMATE_INTEGRATION_OBJECT_H_

#include "mkt-model.h"
#include "mkt-param-object.h"
#include "ultimate-integration.h"


G_BEGIN_DECLS

#define ULTIMATE_TYPE_INTEGRATION_OBJECT             (ultimate_integration_object_get_type ())
#define ULTIMATE_INTEGRATION_OBJECT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), ULTIMATE_TYPE_INTEGRATION_OBJECT, UltimateIntegrationObject))
#define ULTIMATE_INTEGRATION_OBJECT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass),  ULTIMATE_TYPE_INTEGRATION_OBJECT, UltimateIntegrationObjectClass))
#define ULTIMATE_IS_INTEGRATION_OBJECT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ULTIMATE_TYPE_INTEGRATION_OBJECT))
#define ULTIMATE_IS_INTEGRATION_OBJECT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass),  ULTIMATE_TYPE_INTEGRATION_OBJECT))
#define ULTIMATE_INTEGRATION_OBJECT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj),  ULTIMATE_TYPE_INTEGRATION_OBJECT, UltimateIntegrationObjectClass))

typedef struct _UltimateIntegrationObjectClass     UltimateIntegrationObjectClass;
typedef struct _UltimateIntegrationObject          UltimateIntegrationObject;
typedef struct _UltimateIntegrationObjectPrivate   UltimateIntegrationObjectPrivate;


struct _UltimateIntegrationObjectClass
{
	MktParamObjectClass                   parent_class;

};

struct _UltimateIntegrationObject
{
	MktParamObject                        parent_instance;
	UltimateIntegrationObjectPrivate     *priv;
};

GType                   ultimate_integration_object_get_type                 (void) G_GNUC_CONST;

G_END_DECLS

#endif /* _ULTIMATE_INTEGRATION_OBJECT_H_ */
/** @} */
