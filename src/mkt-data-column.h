/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mkt-data-column.c
 * Copyright (C) sascha 2013 <sascha@sascha-VirtualBox>
 * 
mkt-data-column.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * mkt-data-column.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MKT_DATA_COLUMN_H_
#define _MKT_DATA_COLUMN_H_

#include <glib-object.h>
#include "mkt-decl.h"

G_BEGIN_DECLS

#define MKT_TYPE_DATA_COLUMN             (mkt_data_column_get_type ())
#define MKT_DATA_COLUMN(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_DATA_COLUMN, MktDataColumn))
#define MKT_DATA_COLUMN_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), MKT_TYPE_DATA_COLUMN, MktDataColumnClass))
#define MKT_IS_DATA_COLUMN(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_DATA_COLUMN))
#define MKT_IS_DATA_COLUMN_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), MKT_TYPE_DATA_COLUMN))
#define MKT_DATA_COLUMN_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), MKT_TYPE_DATA_COLUMN, MktDataColumnClass))


typedef struct _MktDataColumn          MktDataColumn;
typedef struct _MktDataColumnClass     MktDataColumnClass;
typedef struct _MktDataColumnPrivate   MktDataColumnPrivate;


struct _MktDataColumnClass
{
	GObjectClass parent_class;
};

struct _MktDataColumn
{
	GObject parent_instance;

    MktDataColumnPrivate *priv; 
};

GType mkt_data_column_get_type (void) G_GNUC_CONST;



MktDataColumn*              mkt_data_column_new                       ( GObject       *provider , const gchar *name ,GType type);
MktDataColumn*              mkt_data_column_new_full                  ( GObject       *provider , const gchar *name ,const gchar *default_value,GType type , guint db_flag );
void                        mkt_data_column_clean                     ( MktDataColumn *column );
const gchar*                mkt_data_column_get_name                  ( MktDataColumn *column );
const gchar*                mkt_data_column_get_default_value         ( MktDataColumn *column );
GType                       mkt_data_column_get_gtype                 ( MktDataColumn *column );
guint                       mkt_data_column_get_flags                 ( MktDataColumn *column );
guint                       mkt_data_column_get_rows                  ( MktDataColumn *column );
gchar*                      mkt_data_column_dup_sql_type              ( MktDataColumn *column );

gboolean                    mkt_data_column_add_value                 ( MktDataColumn *column , const GValue *value );
const GValue*               mkt_data_column_get_value                 ( MktDataColumn *column , guint row );
gboolean                    mkt_data_column_add_value_from_string     ( MktDataColumn *column , const gchar *value  );
gboolean                    mkt_data_column_next_value_from_string    ( MktDataColumn *column , const gchar *value  );
gboolean                    mkt_data_column_add_values                ( MktDataColumn *column , GSList *values  );



void                        mkt_data_column_load_init                 (  MktDataColumn *column );



G_END_DECLS

#endif /* _MKT_DATA_COLUMN_H_ */
