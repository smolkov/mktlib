/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * ProOptKa
 * Copyright (C) A.Smolkov 2011 <asmolkov@lar.com>
 * 
 * ProOptKa is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ProOptKa is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MKT_MODULE_H_
#define _MKT_MODULE_H_

#include <glib.h>
#include <glib-object.h>
#include <gmodule.h>

G_BEGIN_DECLS

#define MKT_TYPE_MODULE             (mkt_module_get_type ())
#define MKT_MODULE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_MODULE, MktModule))
#define MKT_MODULE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), MKT_TYPE_MODULE, MktModuleClass))
#define MKT_IS_MODULE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_MODULE))
#define MKT_IS_MODULE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), MKT_TYPE_MODULE))
#define MKT_MODULE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), MKT_TYPE_MODULE, MktModuleClass))

typedef struct _MktModuleClass      MktModuleClass;
typedef struct _MktModule           MktModule;
typedef struct _MktModulePrivate    MktModulePrivate;

struct _MktModuleClass
{
	GTypeModuleClass     parent_class;
};

struct _MktModule
{
	GTypeModule          parent_instance;
	MktModulePrivate     *priv;
	void              (* load   )       (MktModule *module );
	void              (* unload )       (MktModule *module );

};

GType                           mkt_module_get_type              ( void ) G_GNUC_CONST;

void                            mkt_lar_module_load              ( MktModule *module );
void                            mkt_lar_module_unload            ( MktModule *module );

MktModule*                      mkt_module_new                   ( const gchar *filepath );

gboolean                        mkt_module_is_valid_module_name  ( const gchar *basename );
gboolean                        mkt_module_is_valid_object_name  ( const gchar *basename );
G_END_DECLS

#endif /* _MKT_MODULE_H_ */
