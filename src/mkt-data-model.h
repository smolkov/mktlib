/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mktlibrary
 * Copyright (C) sascha 2013 <sascha@sascha-VirtualBox>
 * 
mktlibrary is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * mktlibrary is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MKT_DATA_MODELL_H_
#define _MKT_DATA_MODELL_H_

#include "mkt-data-column.h"
#include "mkt-sql-parser.h"
#include <glib-object.h>



G_BEGIN_DECLS

#define MKT_TYPE_DATA_MODEL             (mkt_data_model_get_type ())
#define MKT_DATA_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_DATA_MODEL, MktDataModel))
#define MKT_DATA_MODEL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), MKT_TYPE_DATA_MODEL, MktDataModelClass))
#define MKT_IS_DATA_MODEL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_DATA_MODEL))
#define MKT_IS_DATA_MODEL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), MKT_TYPE_DATA_MODEL))
#define MKT_DATA_MODEL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), MKT_TYPE_DATA_MODEL, MktDataModelClass))


typedef struct _MktDataModel        MktDataModel;
typedef struct _MktDataModelClass   MktDataModelClass;
typedef struct _MktDataModelPrivate MktDataModelPrivate;


struct _MktDataModelClass
{
	GObjectClass parent_class;
};

struct _MktDataModel
{
	GObject parent_instance;

    MktDataModelPrivate *priv; 
};

GType mkt_data_model_get_type (void) G_GNUC_CONST;


MktDataModel*             mkt_data_model_new                 ( GObject *cnc );
MktDataModel*             mkt_data_model_new_for_object      ( GObject *cnc , GType iface );
GObject*                  mkt_data_model_get_provider        ( MktDataModel *data );
GObject*                  mkt_data_model_new_provider        ( MktDataModel *data );

void                      mkt_data_model_dump                ( MktDataModel *data );
void                      mkt_data_model_clean               ( MktDataModel *data );
void                      mkt_data_model_add_column          ( MktDataModel *data , const gchar *name , GType type );
void                      mkt_data_model_add_data_to         ( MktDataModel *data , guint column , const GValue *value  );
const GValue*             mkt_data_model_get_column_value    ( MktDataModel *data , guint column , guint row  );
const gchar*              mkt_data_model_get_column_name     ( MktDataModel *data , guint column );
MktDataColumn*            mkt_data_model_get_column          ( MktDataModel *data , guint column );
MktDataColumn*            mkt_data_model_get_column_from_name( MktDataModel *data , const gchar *name );
MktDataColumn*            mkt_data_model_get_primary         ( MktDataModel *data );
guint                     mkt_data_model_get_column_nth      ( MktDataModel *data );
guint                     mkt_data_model_get_rows            ( MktDataModel *data );
GParameter*               mkt_data_model_get_row_parameter   ( MktDataModel *data , guint row ,guint *nparam );

GType                     mkt_data_model_object_type         ( MktDataModel *data );
const gchar*              mkt_data_model_get_type_name       ( MktDataModel *data );
MktSqlParser*             mkt_data_model_get_create          ( MktDataModel *data );
MktSqlParser*             mkt_data_model_get_select          ( MktDataModel *data );
MktSqlParser*             mkt_data_model_get_check           ( MktDataModel *data );
MktSqlParser*             mkt_data_model_get_insert          ( MktDataModel *data );
MktSqlParser*             mkt_data_model_get_replace         ( MktDataModel *data );
MktSqlParser*             mkt_data_model_get_delete          ( MktDataModel *data );
MktSqlParser*             mkt_data_model_get_update          ( MktDataModel *data );
MktSqlParser*             mkt_data_model_get_notify          ( MktDataModel *data );

gboolean                  mkt_data_model_check_interface     ( MktDataModel *data , GType iface_type );

//MktModel*                 mkt_data_model_check_model_at_row  ( MktDataModel *data , guint row );
//GSList*                   mkt_data_model_load_all            ( MktDataModel *data , gboolean local );

//gboolean                  mkt_data_model_check_model         ( MktDataModel *data , MktModel *model );
//void                      mkt_data_model_remove_model        ( MktDataModel *data , MktModel *model  );


G_END_DECLS

#endif /* _MKT_DATA_MODELL_H_ */
