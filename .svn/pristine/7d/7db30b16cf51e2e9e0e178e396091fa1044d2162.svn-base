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

#include "mkt-data-column.h"
#include "mkt-value.h"
#include "mkt-model.h"
#include "mkt-connection.h"




struct _MktDataColumnPrivate
{
	guint         rows;
	gchar         *name;
	gchar         *default_value;
    GSList        *rows_data;
    GSList        *last_data;
    GType          type ;
    guint          db_flag;
    gchar         *extra_type;
    MktConnection *provider;
};

#define MKT_DATA_COLUMN_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), MKT_TYPE_DATA_COLUMN, MktDataColumnPrivate))

enum
{
	PROP_0,
	PROP_COLUMN_NAME,
	PROP_COLUMN_TYPE,
	PROP_COLUMN_DEFAULT_VALUE,
	PROP_COLUMN_FLAGS,
	PROP_COLUMN_ROWS
};



static void
mkt_data_column_free_rows (GSList *slist)
{
	g_slist_foreach (slist, (GFunc) mkt_value_free, NULL);
	g_slist_free (slist);
}


G_DEFINE_TYPE (MktDataColumn, mkt_data_column, G_TYPE_OBJECT);

static void
mkt_data_column_init (MktDataColumn *mkt_data_column)
{
    MktDataColumnPrivate *priv = MKT_DATA_COLUMN_GET_PRIVATE(mkt_data_column);
    priv->rows_data      = NULL;
    priv->name           = NULL;
    priv->default_value  = NULL;
    priv->type           = G_TYPE_NONE;
    priv->db_flag        = 0;
    priv->provider       = NULL;
    priv->last_data      = NULL;
    mkt_data_column->priv = priv;

	/* TODO: Add initialization code here */
}

static void
mkt_data_column_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */
	MktDataColumn *column = MKT_DATA_COLUMN(object);
	//g_debug("mkt_data_column_finalize %s",column->priv->name);
	if(column->priv->rows_data!= NULL)mkt_data_column_free_rows(column->priv->rows_data);
	if(column->priv->name )          g_free(column->priv->name);
	if(column->priv->default_value ) g_free(column->priv->default_value);
	if(column->priv->provider)       g_object_unref(column->priv->provider);
	G_OBJECT_CLASS (mkt_data_column_parent_class)->finalize (object);
}


static void
mkt_data_column_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_DATA_COLUMN (object));
	MktDataColumn *column = MKT_DATA_COLUMN(object);
	switch (prop_id)
	{
	case PROP_COLUMN_NAME:
		if(column->priv->name) g_free(column->priv->name);
		column->priv->name = g_value_dup_string(value);
		break;
	case PROP_COLUMN_DEFAULT_VALUE:
		if(column->priv->default_value) g_free(column->priv->default_value);
		column->priv->default_value = g_value_dup_string(value);
		break;
	case PROP_COLUMN_TYPE:
		column->priv->type = g_value_get_gtype(value);
		//g_debug("set type %s",g_type_name(column->priv->type));
		break;
	case PROP_COLUMN_FLAGS:
		column->priv->db_flag = g_value_get_uint(value);
		//g_debug("set type %s",g_type_name(column->priv->type));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_data_column_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_DATA_COLUMN (object));
	MktDataColumn *column = MKT_DATA_COLUMN(object);
	switch (prop_id)
	{
	case PROP_COLUMN_ROWS:
		if(column->priv->rows_data!= NULL)
			g_value_set_uint(value,g_slist_length(column->priv->rows_data));
		else
			g_value_set_uint(value,0);
		break;
	case PROP_COLUMN_FLAGS:
		g_value_set_uint(value,column->priv->db_flag);
		break;
	case PROP_COLUMN_NAME:
		g_value_set_string(value,column->priv->name);
		break;
	case PROP_COLUMN_DEFAULT_VALUE:
		g_value_set_string(value,column->priv->default_value);
		break;
	case PROP_COLUMN_TYPE:
		g_value_set_gtype(value,column->priv->type);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_data_column_class_init (MktDataColumnClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	g_type_class_add_private (klass, sizeof (MktDataColumnPrivate));

	object_class->finalize     = mkt_data_column_finalize;
	object_class->set_property = mkt_data_column_set_property;
	object_class->get_property = mkt_data_column_get_property;

	g_object_class_install_property (object_class,
										 PROP_COLUMN_FLAGS,
		                                 g_param_spec_uint ("column-flags",
		                                                    "Property column flags",
		                                                    "Set Get column flags",
		                                                    0, /* TODO: Adjust minimum property value */
		                                                    G_MAXUINT, /* TODO: Adjust maximum property value */
		                                                    0,
		                                                    G_PARAM_READWRITE));


	g_object_class_install_property (object_class,
	                                 PROP_COLUMN_ROWS,
	                                 g_param_spec_uint ("column-rows",
	                                                    "Property column rows",
	                                                    "Set Get Column rows counter",
	                                                    0, /* TODO: Adjust minimum property value */
	                                                    G_MAXUINT, /* TODO: Adjust maximum property value */
	                                                    0,
	                                                    G_PARAM_READABLE));
	g_object_class_install_property (object_class,
									PROP_COLUMN_NAME,
	                                 g_param_spec_string ("column-name",
	                                 "Column name prop",
	                                 "Set|Get column name prop",
	                                 "",	G_PARAM_READWRITE ));
	g_object_class_install_property (object_class,
										PROP_COLUMN_DEFAULT_VALUE,
		                                 g_param_spec_string ("default-value",
		                                 "Column name prop",
		                                 "Set|Get column name prop",
		                                 "",	G_PARAM_READWRITE ));

	g_object_class_install_property (object_class,
										PROP_COLUMN_TYPE,
										g_param_spec_gtype("column-type",
												"Column type prop",
												"Set|Get column type prop",
												G_TYPE_NONE,
												G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

}



MktDataColumn*
mkt_data_column_new      ( GObject *provider ,const gchar *name ,GType type )
{
	MktDataColumn *col = MKT_DATA_COLUMN(g_object_new(MKT_TYPE_DATA_COLUMN,"column-name",name,"column-type",type,NULL));
	if(provider!=NULL)
		col->priv->provider = MKT_CONNECTION(g_object_ref(provider));
	else
		col->priv->provider = MKT_CONNECTION(g_object_ref(mkt_connection_get_default(TRUE)));
	return col;
}

MktDataColumn*
mkt_data_column_new_full (GObject *provider , const gchar *name ,const gchar *default_value ,GType type , guint db_flag )
{
	MktDataColumn *col = MKT_DATA_COLUMN(g_object_new(MKT_TYPE_DATA_COLUMN,"column-name",name,
			                                                               "column-type",type,
			                                                               "column-flags",db_flag,
																		   "default-value",default_value,
			                                                               NULL));
	return col;
}

void
mkt_data_column_clean   ( MktDataColumn *column )
{
	g_return_if_fail (column!=NULL);
	g_return_if_fail (MKT_IS_DATA_COLUMN (column));
	if(column->priv->rows_data!= NULL)mkt_data_column_free_rows(column->priv->rows_data);
	column->priv->rows_data = NULL;
	column->priv->last_data = NULL;

}

const gchar*
mkt_data_column_get_name ( MktDataColumn *column )
{
	g_return_val_if_fail(column!= NULL,NULL );
	g_return_val_if_fail(MKT_IS_DATA_COLUMN(column ), NULL);
	return column->priv->name;
}

const gchar*
mkt_data_column_get_default_value ( MktDataColumn *column )
{
	g_return_val_if_fail(column!= NULL,NULL );
	g_return_val_if_fail(MKT_IS_DATA_COLUMN(column ), NULL);
	return column->priv->default_value;
}

GType
mkt_data_column_get_gtype (MktDataColumn *column  )
{
	g_return_val_if_fail(column!= NULL,G_TYPE_NONE );
	g_return_val_if_fail(MKT_IS_DATA_COLUMN(column ), G_TYPE_NONE);
	return column->priv->type;
}

guint
mkt_data_column_get_flags             ( MktDataColumn *column )
{
	g_return_val_if_fail(column!= NULL,0 );
	g_return_val_if_fail(MKT_IS_DATA_COLUMN(column ), 0);
	return column->priv->db_flag;
}

guint
mkt_data_column_get_rows (MktDataColumn *column  )
{
	g_return_val_if_fail(column!= NULL,G_TYPE_NONE );
	g_return_val_if_fail(MKT_IS_DATA_COLUMN(column ), G_TYPE_NONE);
	if(column->priv->rows_data!= NULL)
		return  g_slist_length(column->priv->rows_data);
	else
		return  0;
	return column->priv->type;
}


gboolean
mkt_data_column_add_value ( MktDataColumn *column , const GValue *value )
{
	g_return_val_if_fail(column!= NULL,FALSE );
	g_return_val_if_fail(MKT_IS_DATA_COLUMN(column ), FALSE);
	if(column->priv->type == G_TYPE_NONE)
		column->priv->type = value->g_type;

	if(column->priv->type != value->g_type )
	{
		if(!g_value_type_transformable(value->g_type,column->priv->type ) )
		{
			g_warning( "Column %s have other data type %s value is a %s type",column->priv->name,g_type_name(column->priv->type),g_type_name(value->g_type));
			return FALSE;
		}
	}
	GValue *new = mkt_value_new(column->priv->type);
	if(column->priv->type == value->g_type)
		g_value_copy(value,new);
	else
		g_value_transform(value,new);
	column->priv->rows_data = g_slist_append(column->priv->rows_data,new );
	return TRUE;
}

const GValue*
mkt_data_column_get_value ( MktDataColumn *column , guint row )
{
	g_return_val_if_fail(column!= NULL,NULL );
	g_return_val_if_fail(MKT_IS_DATA_COLUMN(column ), NULL);
	const GValue* ret =  g_slist_nth_data (column->priv->rows_data,row);
	return ret;
}


/*
 * Not free it !!!
 */
// static const GSList*
// mkt_data_column_get_values ( MktDataColumn *column)
// {
// 	g_return_val_if_fail(column!= NULL,NULL );
// 	g_return_val_if_fail(MKT_IS_DATA_COLUMN(column ), NULL);
// 	return column->priv->rows_data;
// }


gboolean
mkt_data_column_add_values                ( MktDataColumn *column , GSList *values  )
{
	g_return_val_if_fail(column!= NULL,FALSE );
	g_return_val_if_fail(MKT_IS_DATA_COLUMN(column ), FALSE);
	g_return_val_if_fail(values != NULL,FALSE );
	GValue *val = (GValue*)values->data;
	if(val != NULL )
	{
		if(val->g_type == column->priv->type || g_value_type_compatible(val->g_type,column->priv->type) || g_value_type_transformable(val->g_type,column->priv->type) )
		{
			if(column->priv->rows_data)
			{
				mkt_data_column_free_rows(column->priv->rows_data);
			}
			column->priv->rows_data = values;
			return TRUE;
		}
		else
		{
			g_warning( "add value column unconvertible types %s to %s ",g_type_name(val->g_type),g_type_name(column->priv->type) );
		}


	}
	return FALSE;
}


gboolean
mkt_data_column_add_value_from_string (MktDataColumn *column , const gchar *value  )
{
	g_return_val_if_fail(column!= NULL,FALSE );
	g_return_val_if_fail(MKT_IS_DATA_COLUMN(column ), FALSE);
	if(column->priv->type == G_TYPE_NONE)
	{
		g_warning ( "Data column %s have unknown type ",column->priv->name);
		return FALSE;
	}
	GValue *new = mkt_value_new(column->priv->type);
	if(!mkt_set_gvalue_from_string(new,value))
	{
		g_warning ( " Data column %s can not transform string value to %s",column->priv->name, g_type_name(column->priv->type));
		mkt_value_free(new);
		return FALSE;
	}
//TEST:	g_debug("Add value %s to %s list ==%p",value,column->priv->name,column->priv->rows_data);
	column->priv->rows_data = g_slist_append(column->priv->rows_data,new );
	//g_debug("Row#=%d",g_slist_length(column->priv->rows_data));
	return TRUE;
}


gboolean
mkt_data_column_next_value_from_string    ( MktDataColumn *column , const gchar *value  )
{
	g_return_val_if_fail(column!= NULL,FALSE );
	g_return_val_if_fail(MKT_IS_DATA_COLUMN(column ), FALSE);

	if(column->priv->type == G_TYPE_NONE)
	{
		g_warning ( "Data column %s have unknown type ",column->priv->name);
		return FALSE;
	}
	GValue *new = mkt_value_new(column->priv->type);
	if(!mkt_set_gvalue_from_string(new,value))
	{
		g_warning ( " Data column %s can not transform string value to %s",column->priv->name, g_type_name(column->priv->type));
		mkt_value_free(new);
		return FALSE;
	}

	GSList *nl  = g_slice_new(GSList);
	nl->data = new;
	nl->next = NULL;
	if(column->priv->last_data!=NULL)
		column->priv->last_data->next = nl;
	column->priv->last_data=nl;
	if(column->priv->rows_data == NULL)
	{
		column->priv->rows_data = column->priv->last_data;
	}
	return TRUE;
}



