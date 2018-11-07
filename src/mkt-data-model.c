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

#include "mkt-data-model.h"
#include "mkt-data-column.h"
#include <glib/gprintf.h>
#include "mkt-value.h"
#include "mkt-sql-parser.h"
#include "mkt-model.h"
#include "mkt-mutex.h"
#include "mkt-connection.h"
#include "mkt-model.h"
#include "market-time.h"
#include <stdlib.h>


#if GLIB_CHECK_VERSION(2,31,7)
static GRecMutex __data_model_init_mutex;
#define DATA_MODEL_INIT_LOCK() g_rec_mutex_lock(&__data_model_init_mutex)
#define DATA_MODEL_INIT_UNLOCK() g_rec_mutex_unlock(&__data_model_init_mutex)
#else
static GStaticRecMutex __data_model_init_mutex = G_STATIC_REC_MUTEX_INIT;
#define DATA_MODEL_INIT_LOCK() g_static_rec_mutex_lock(&__data_model_init_mutex)
#define DATA_MODEL_INIT_UNLOCK() g_static_rec_mutex_unlock(&__data_model_init_mutex)
#endif


struct _MktDataModelPrivate
{
	gchar        *model_type;
	GType         oType;
	gchar        *table;
	guint         rows;
	GSList       *columns;
	GSList       *ifases;
	guint         db_flag;
	gchar        *pk_field;
	GType         pk_type;
	MktConnection *provider;
	MktSqlParser  *create;
	MktSqlParser  *check;
	MktSqlParser  *select;
	MktSqlParser  *insert;
	MktSqlParser  *insert_or_replace;
	MktSqlParser  *update;
	MktSqlParser  *notify;
	MktSqlParser  *delete;
	MktMutex      *mutex;
	glong          autoid;
	gboolean       no_sync;

};



#define MKT_DATA_MODEL_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), MKT_TYPE_DATA_MODEL, MktDataModelPrivate))

enum {
	PROP_DATA_MODEL_NULL,
	PROP_DATA_MODEL_TYPE_NAME,
	PROP_DATA_MODEL_TABLE,
	PROP_DATA_MODEL_ROWS,
	PROP_DATA_MODEL_COLUMS
};





G_DEFINE_TYPE (MktDataModel, mkt_data_model, G_TYPE_OBJECT);

static void
mkt_data_model_init (MktDataModel *mkt_data_model)
{
    MktDataModelPrivate *priv = MKT_DATA_MODEL_GET_PRIVATE(mkt_data_model);
    priv->columns             = NULL;
    priv->table               = NULL;
    priv->create              = NULL;
    priv->check               = NULL;
    priv->update              = NULL;
    priv->insert              = NULL;
    priv->insert_or_replace   = NULL;
    priv->notify              = NULL;
    priv->select              = NULL;
    priv->delete              = NULL;
    priv->model_type          = g_strdup("unknown type");
    priv->ifases              = NULL;
    priv->pk_field            = NULL;
    priv->pk_type             = G_TYPE_NONE;
    priv->mutex               = mkt_mutex_new();
    priv->provider            = NULL;
    priv->oType               = G_TYPE_NONE;
    mkt_data_model->priv      = priv;
	/* TODO: Add initialization code here */
}

static void
mkt_data_model_destroy_columns ( MktDataModel *data )
{
	if(data->priv->columns)
	{
		g_slist_foreach (data->priv->columns, (GFunc) g_object_unref, NULL);
		g_slist_free (data->priv->columns);
		data->priv->columns = NULL;
	}
}

static void
mkt_data_model_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */
	MktDataModel *data = MKT_DATA_MODEL(object);
	//g_debug("finalize data model %s",data->priv->model_type);
	if(data->priv->model_type) g_free(data->priv->model_type);
	if(data->priv->table)      g_free(data->priv->table);
	if(data->priv->create)     g_object_unref(data->priv->create);
	if(data->priv->check)      g_object_unref(data->priv->check);
	if(data->priv->insert)     g_object_unref(data->priv->insert);
	if(data->priv->update)     g_object_unref(data->priv->update);
	if(data->priv->notify)     g_object_unref(data->priv->notify);
	if(data->priv->select)     g_object_unref(data->priv->select);
	if(data->priv->delete)     g_object_unref(data->priv->delete);
	if(data->priv->insert_or_replace) g_object_unref(data->priv->insert_or_replace);
	if(data->priv->ifases)     mkt_slist_free_full(data->priv->ifases,g_free);
	if(data->priv->pk_field)   g_free(data->priv->pk_field);
	if(data->priv->provider)   g_object_unref(data->priv->provider);
	mkt_mutex_free( data->priv->mutex );
	mkt_data_model_destroy_columns ( data );
	G_OBJECT_CLASS (mkt_data_model_parent_class)->finalize (object);
}


static void
mkt_data_model_set_property ( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec )
{
	//TEST:	g_debug("sensor_set_property %s",pspec->name);
	g_return_if_fail (MKT_IS_DATA_MODEL(object));
	MktDataModel *data = MKT_DATA_MODEL(object);
	switch (prop_id)
	{
	case PROP_DATA_MODEL_TYPE_NAME:
		if(data->priv->model_type)g_free(data->priv->model_type);
		data->priv->model_type = g_value_dup_string(value);
		break;
	case PROP_DATA_MODEL_ROWS:
		break;
	case PROP_DATA_MODEL_COLUMS:
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
	//TEST:	g_debug("sensor_set_property %s end",pspec->name);
}

static void
mkt_data_model_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	//TEST:	g_debug("sensor_get_property %s",pspec->name);
	g_return_if_fail (MKT_IS_DATA_MODEL (object));
	MktDataModel *data = MKT_DATA_MODEL(object);
	switch (prop_id)
	{
	case PROP_DATA_MODEL_TYPE_NAME:
		g_value_set_string(value,data->priv->model_type);
		break;
	case PROP_DATA_MODEL_ROWS:
		g_value_set_uint(value,data->priv->rows);
		break;
	case PROP_DATA_MODEL_COLUMS:
		if(data->priv->columns)
			g_value_set_uint(value,g_slist_length(data->priv->columns));
		else
			g_value_set_uint(value,0);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
	//TEST:	g_debug("sensor_get_property %s end",pspec->name);
}
static void
mkt_data_model_class_init (MktDataModelClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (MktDataModelPrivate));
	object_class->finalize = mkt_data_model_finalize;
	object_class->set_property = mkt_data_model_set_property;
	object_class->get_property = mkt_data_model_get_property;


	GParamSpec *pspec;
	pspec = g_param_spec_string ("data-model-type",
			"Data model type name prop",
			"Set|Get data model type name",
			"",
			G_PARAM_READWRITE );
	g_object_class_install_property(object_class,
			PROP_DATA_MODEL_TYPE_NAME,
			pspec);
	pspec = g_param_spec_string ("data-model-table",
			"Data model type name prop",
			"Set|Get data model type name",
			"",
			G_PARAM_READWRITE );
	g_object_class_install_property(object_class,
			PROP_DATA_MODEL_TYPE_NAME,
			pspec);
	pspec = g_param_spec_uint ("data-model-columns",
			"Data model columns counter",
			"Get data model columns counter",
			0,G_MAXUINT32,0,G_PARAM_READABLE );
	g_object_class_install_property(object_class,
			PROP_DATA_MODEL_COLUMS,
			pspec);
	pspec = g_param_spec_uint ("data-model-rows",
			"Data model rows counter",
			"Get data model rows counter",
			0,G_MAXUINT32,0,G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY );
	g_object_class_install_property(object_class,
			PROP_DATA_MODEL_ROWS,
			pspec);

}


MktDataModel*
mkt_data_model_new (  GObject *cnc )
{
	MktDataModel *data = g_object_new ( MKT_TYPE_DATA_MODEL , NULL );
	if(cnc != NULL )
		data->priv->provider = MKT_CONNECTION(cnc);
	else
		data->priv->provider = mkt_connection_get_default(TRUE);
	return data;
}



static void
mkt_data_model_create_object_parsers (MktDataModel *data )
{
	mkt_mutex_lock(data->priv->mutex);
	gchar  *table_name = g_strdup_printf("%s",data->priv->model_type);
	const gchar  *key   = NULL;
	GValue *value = NULL;
	//g_debug("Set tablename %s for model ",table_name);
	gboolean is_pk = FALSE;
	GString *insert_col     =  g_string_new ("");
	GString *insert_values  =  g_string_new ("");
	GString *replace_col    =  g_string_new ("");
	GString *replace_values =  g_string_new ("");
	GString *update_values  =  g_string_new ("");
	GString *create_cols    =  g_string_new ("");
	GString *delete         =  g_string_new ("");

	GSList *columns = NULL;

	GString *comment = g_string_new ( "");
	GSList *lc = NULL;
	for(lc=data->priv->ifases;lc!=NULL;lc=lc->next)
	{
		g_string_append_printf(comment,"%s%s",comment->len>1?",":"",g_type_name(*((GType*)lc->data)));
	}
	for(columns=data->priv->columns;columns!=NULL;columns = columns->next)
	{
		const gchar *col_name = mkt_data_column_get_name(MKT_DATA_COLUMN(columns->data));
		guint flags = mkt_data_column_get_flags(MKT_DATA_COLUMN(columns->data));
		if(col_name == NULL || flags == 0) continue;
		if((  flags & MKT_MODEL_PK_PROP ) )
		{
			if(!is_pk)key = col_name;
		}
		else
		{
			g_string_append_printf(update_values,"%s%s=$%s",update_values->len>1?",":"",col_name,col_name);
		}
		if(!(  flags & MKT_MODEL_DB_AUTOINCREMENT ))
		{
			g_string_append_printf(insert_values,"%s$%s",insert_values->len>1?",":"",col_name);
			g_string_append_printf(insert_col,"%s%s",insert_col->len>1?",":"",col_name);
		}
		g_string_append_printf(replace_values,"%s$%s",replace_values->len>1?",":"",col_name);
		g_string_append_printf(replace_col,"%s%s",replace_col->len>1?",":"",col_name);
		g_string_append_printf(delete,"%s%s=$%s",delete->len>1?" and ":"",col_name,col_name);
		gchar *value_type    = mkt_connection_convert_value_type_sql(data->priv->provider,mkt_data_column_get_gtype(MKT_DATA_COLUMN(columns->data)),flags);
		//gchar *default_value = mkt_data_column_get_default_value(MKT_DATA_COLUMN(columns->data));
		if( value_type != NULL )
		{
			g_string_append_printf (create_cols ,"%s%s %s",create_cols->len>1?",":"",col_name,value_type);
		}

	}

	// Create check parser ...
	MktSqlParser *parser = g_object_new( MKT_TYPE_SQL_PARSER,"parser-name","check","parser-table",table_name,NULL);
	if( key )
	{
		g_object_set(G_OBJECT(parser),"pk-value",key,NULL);
		if(value)mkt_sql_parser_set_default_pk_value(parser,value);
		gchar* sql = g_strdup_printf("SELECT * FROM $tablename WHERE %s = $%s",key,key);
		//TEST:		g_debug("Check SQL:%s",sql);
		mkt_sql_parser_parce_string(parser,sql);
		g_free(sql);
	}
	else
	{
		g_object_unref(parser);
		parser = NULL;
	}
	data->priv->check = parser;

	// Create create parcer

	//MktConnection *default_conn = mkt_connection_get_default(TRUE);
	data->priv->create = g_object_new( MKT_TYPE_SQL_PARSER,"parser-name","create","parser-table",table_name,NULL);

	gchar *create_sql = NULL ;
	if(NULL!= mkt_connection_get_provider(data->priv->provider))
	{
		if(0== g_strcmp0(mkt_connection_get_provider(data->priv->provider),"SQLite" ))
			create_sql = g_strdup_printf("CREATE TABLE IF NOT EXISTS $tablename (%s);",create_cols->str);
		else
			create_sql = g_strdup_printf("CREATE TABLE IF NOT EXISTS $tablename (%s) COMMENT='%s';",create_cols->str,comment->str);
		mkt_sql_parser_parce_string(data->priv->create,create_sql);
	}
	//TEST:	g_debug("Create SQL:%s",create_sql);

	if(create_sql)g_free(create_sql);

	// Create insert parser
	data->priv->insert = g_object_new( MKT_TYPE_SQL_PARSER,"parser-name","insert","parser-table",table_name,NULL);
	gchar *insert_sql = g_strdup_printf("INSERT INTO $tablename (%s) VALUES(%s);",insert_col->str,insert_values->str);
	mkt_sql_parser_parce_string(data->priv->insert,insert_sql);
	//TEST:	g_debug("Insert SQL:%s",insert_sql);
	g_free(insert_sql);
	data->priv->insert_or_replace =  g_object_new( MKT_TYPE_SQL_PARSER,"parser-name","insert or replace","parser-table",table_name,NULL);
	insert_sql = g_strdup_printf("INSERT OR REPLACE INTO $tablename (%s) VALUES(%s);",replace_col->str,replace_values->str);
	mkt_sql_parser_parce_string(data->priv->insert_or_replace,insert_sql);
	g_free(insert_sql);
	data->priv->delete  =  g_object_new( MKT_TYPE_SQL_PARSER,"parser-name","delete","parser-table",table_name,NULL);
	gchar *delete_sql   =  g_strdup_printf("DELETE FROM $tablename  WHERE ref_id = $ref_id;");
	mkt_sql_parser_parce_string(data->priv->delete,delete_sql);
	g_free(delete_sql);

	// Create update parser
	if(data->priv->check)
	{
		gchar *update     = g_strdup_printf("UPDATE $tablename SET %s WHERE %s = $%s;",update_values->str,key,key);
		data->priv->update = g_object_new ( MKT_TYPE_SQL_PARSER,"parser-name","update","parser-table",table_name, NULL );
		mkt_sql_parser_parce_string(data->priv->update,update);
		//TEST:		g_debug("update SQL:%s",update);
		data->priv->notify =  g_object_new( MKT_TYPE_SQL_PARSER,"parser-name","notify","parser-table",table_name,"pk-value",key,NULL);
		g_free(update);
	}
	if(key)
		data->priv->select =  g_object_new( MKT_TYPE_SQL_PARSER,"parser-name","select","parser-table",table_name,"pk-value",key,NULL);
	else
		data->priv->select =  g_object_new( MKT_TYPE_SQL_PARSER,"parser-name","select","parser-table",table_name,NULL);

	g_string_free(comment,TRUE);
	g_string_free(update_values,TRUE);
	g_string_free(insert_col,TRUE);
	g_string_free(insert_values,TRUE);
	g_string_free(replace_col,TRUE);
	g_string_free(replace_values,TRUE);
	g_string_free(create_cols,TRUE);
	g_string_free(delete,TRUE);
	g_free(table_name);
	mkt_mutex_unlock(data->priv->mutex);
}

// static GHashTable*
// mkt_data_model_create_pk_table ( MktDataModel *data )
// {
// 	GHashTable *table = NULL;
// 	table = g_hash_table_new(g_int_hash,g_int_equal);
// 	return table;
// }


MktDataModel*
mkt_data_model_new_for_object      ( GObject *cnc , GType otype  )
{
	guint ni =0;
	g_assert(g_type_is_a(otype,MKT_TYPE_MODEL));
	DATA_MODEL_INIT_LOCK();
	MktDataModel *data = g_object_new ( MKT_TYPE_DATA_MODEL ,"data-model-type",g_type_name(otype),NULL  );
	DATA_MODEL_INIT_UNLOCK();
	mkt_mutex_lock(data->priv->mutex);
	if(cnc != NULL )
		data->priv->provider = MKT_CONNECTION(g_object_ref(cnc));
	else
		data->priv->provider = MKT_CONNECTION(g_object_ref(mkt_connection_get_default(TRUE)));
	GType *iface = g_type_interfaces (otype,&ni);
	data->priv->oType = otype;
	int i = 0;
	for(i=0;i<ni;i++)
	{
		GType *itype = g_malloc(sizeof(GType));
		*itype = iface[i];
		data->priv->ifases = g_slist_append(data->priv->ifases,itype);
		//g_debug("load interface[%d]=%s",i,g_type_name(iface[i]));
		guint npar = 0;
		gpointer itable = g_type_default_interface_ref  (iface[i]);
		GParamSpec **pspec = g_object_interface_list_properties(itable,&npar);
		if(pspec != NULL)
		{
			int j = 0;
			for(j=0;j<npar;j++)
			{

				if(pspec[j]->flags & MKT_MODEL_DB_PROP)
				{
					const GValue *value = g_param_spec_get_default_value(pspec[j]);
					gchar *def_value = mkt_value_stringify(value);
					MktDataColumn *col = mkt_data_column_new_full(G_OBJECT(data->priv->provider),mkt_sql_parser_rps(pspec[j]->name),def_value,pspec[j]->value_type,pspec[j]->flags);
					g_free(def_value);
					//TEST:	g_debug("Add column %s flags=%d type = %s",pspec[j]->name,pspec[j]->flags,g_type_name(pspec[j]->value_type));
					data->priv->columns = g_slist_append(data->priv->columns,(gpointer)col);
				}
			}
			g_free(pspec);
		}
		if(itable)g_type_default_interface_unref(itable);
	}
	g_free(iface);
	mkt_mutex_unlock(data->priv->mutex);
	mkt_data_model_create_object_parsers(data);
	return data;
}

GObject*
mkt_data_model_get_provider        ( MktDataModel *data )
{
	g_return_val_if_fail (data!=NULL,NULL);
	g_return_val_if_fail (MKT_IS_DATA_MODEL (data),NULL);
	return G_OBJECT(data->priv->provider);
}
GObject*
mkt_data_model_new_provider        ( MktDataModel *data )
{
	MktConnection *connection  = MKT_CONNECTION(mkt_connection_new(mkt_connection_get_host(data->priv->provider),mkt_connection_get_data_base(data->priv->provider),TRUE));
	return G_OBJECT(connection);
}



void
mkt_data_model_clean ( MktDataModel *data )
{
	g_return_if_fail (data!=NULL);
	g_return_if_fail (MKT_IS_DATA_MODEL (data));
	GSList *col = NULL;
	for(col = data->priv->columns;col != NULL ; col=col->next)
	{
		mkt_data_column_clean( MKT_DATA_COLUMN(col->data) );
	}
}

guint
mkt_data_model_get_column_nth      ( MktDataModel *data )
{
	g_return_val_if_fail (data!=NULL,0);
	g_return_val_if_fail (MKT_IS_DATA_MODEL (data),0);
	if(data->priv->columns)
		return g_slist_length(data->priv->columns);
	else
		return 0;
}



guint
mkt_data_model_get_rows (  MktDataModel *data  )
{
	g_return_val_if_fail (data!=NULL,0);
	g_return_val_if_fail (MKT_IS_DATA_MODEL (data),0);
	mkt_mutex_lock(data->priv->mutex);
	GSList *l = NULL;
	guint ret = 0;
	for(l=data->priv->columns;l!=NULL;l=l->next)
	{
		MktDataColumn *col = MKT_DATA_COLUMN(l->data);
		guint ri = mkt_data_column_get_rows(col);if(ri >ret)ret = ri;
	}
	mkt_mutex_unlock(data->priv->mutex);
	return ret;
}



void
mkt_data_model_add_column ( MktDataModel *data , const gchar *name , GType type )
{
	g_return_if_fail (data!=NULL);
	g_return_if_fail (MKT_IS_DATA_MODEL (data));
	mkt_mutex_lock(data->priv->mutex);
	//TEST:g_debug("Data Model Add Column %s type %s",name , g_type_name(type));
	MktDataColumn *col = mkt_data_column_new(G_OBJECT(data->priv->provider),name,type);
	data->priv->columns = g_slist_append(data->priv->columns,(gpointer)col);
	mkt_mutex_unlock(data->priv->mutex);
}


void
mkt_data_model_add_data_to ( MktDataModel *data , guint column , const GValue *value  )
{
	g_return_if_fail (data!=NULL);
	g_return_if_fail (MKT_IS_DATA_MODEL (data));
	mkt_mutex_lock(data->priv->mutex);
	MktDataColumn *col = MKT_DATA_COLUMN(g_slist_nth_data (data->priv->columns,column));
	if(NULL == col )
	{
		g_warning ( "Data model can not column %d create : unknown errror ",column);
		return;
	}
	mkt_data_column_add_value(col,value);
	mkt_mutex_unlock(data->priv->mutex);
}

MktDataColumn*
mkt_data_model_get_column ( MktDataModel *data , guint column )
{
	g_return_val_if_fail (data!=NULL,NULL);
	g_return_val_if_fail (MKT_IS_DATA_MODEL (data),NULL);
	//mkt_mutex_lock(data->priv->mutex);
	MktDataColumn *ret = MKT_DATA_COLUMN(g_slist_nth_data (data->priv->columns,column));
	//mkt_mutex_unlock(data->priv->mutex);
	return ret;
}

static gint
column_is_name_equal(gconstpointer  a, gconstpointer  b)
{
	g_return_val_if_fail(b!=NULL,1);
	g_return_val_if_fail(a!=NULL,1);
	g_return_val_if_fail(MKT_IS_DATA_COLUMN(a),1);
	gchar *name = (gchar *) b;
	MktDataColumn *column = MKT_DATA_COLUMN(a);
	return g_strcmp0(name,mkt_data_column_get_name(column));
}

MktDataColumn*
mkt_data_model_get_column_from_name ( MktDataModel *data , const gchar *name )
{
	g_return_val_if_fail (data!=NULL,NULL);
	g_return_val_if_fail (name!=NULL,NULL);
	g_return_val_if_fail (MKT_IS_DATA_MODEL (data),NULL);
	mkt_mutex_lock(data->priv->mutex);
/*TEST:	GSList *test = NULL ;
	for(test = data->priv->columns;test!=NULL;test=test->next)
		g_debug("Column nane = %s",mkt_data_column_get_name(MKT_DATA_COLUMN(test->data)));*/

	GSList *ld =  g_slist_find_custom(data->priv->columns,name,(GCompareFunc)column_is_name_equal );
	mkt_mutex_unlock(data->priv->mutex);
	if(ld && ld->data) return MKT_DATA_COLUMN(ld->data);
	return NULL;
}

MktDataColumn*
mkt_data_model_get_primary ( MktDataModel *data )
{
	g_return_val_if_fail (data!=NULL,0);
	g_return_val_if_fail (MKT_IS_DATA_MODEL (data),0);
	mkt_mutex_lock(data->priv->mutex);
	GSList *l = NULL;
	for(l=data->priv->columns;l!=NULL;l=l->next)
	{
		guint flag = mkt_data_column_get_flags(MKT_DATA_COLUMN(l->data));
		if( flag & MKT_MODEL_PK_PROP) break;;
	}
	mkt_mutex_unlock(data->priv->mutex);
	return l!=NULL?MKT_DATA_COLUMN(l->data):NULL;
}

const GValue*
mkt_data_model_get_column_value ( MktDataModel *data , guint column , guint row  )
{
	g_return_val_if_fail (data!=NULL,NULL);
	g_return_val_if_fail (MKT_IS_DATA_MODEL (data),NULL);
	mkt_mutex_lock(data->priv->mutex);
	MktDataColumn *col = MKT_DATA_COLUMN(g_slist_nth_data (data->priv->columns,column));
	mkt_mutex_unlock(data->priv->mutex);
	if(col== NULL)
	{
		g_warning ( "Data model get error :column %d not exist",column);
		return NULL;
	}
	return mkt_data_column_get_value(col,row);
}

const gchar*
mkt_data_model_get_column_name ( MktDataModel *data , guint column  )
{
	g_return_val_if_fail (data!=NULL,NULL);
	g_return_val_if_fail (MKT_IS_DATA_MODEL (data),NULL);
	mkt_mutex_lock(data->priv->mutex);
	MktDataColumn *col = MKT_DATA_COLUMN(g_slist_nth_data (data->priv->columns,column));
	mkt_mutex_unlock(data->priv->mutex);
	if(col== NULL)
	{
		g_warning ( "Data model get error :column %d not exist",column);
		return NULL;
	}
	return mkt_data_column_get_name(col);
}

void
mkt_data_model_dump ( MktDataModel *data )
{
	mkt_mutex_lock(data->priv->mutex);
	GSList *l =  data->priv->columns;

	for(;l!=NULL;l=l->next)
	{
		MktDataColumn *col = MKT_DATA_COLUMN(l->data);
		g_printf("%s(%s)|",mkt_data_column_get_name(col),g_type_name(mkt_data_column_get_gtype(col)));
	}
	g_printf("\n\n");
	guint max_row = mkt_data_model_get_rows(data);
	g_printf("Table rows = %d",max_row);
	gint i ;
	for( i=0;i<max_row;i++ )
	{
		for(l=data->priv->columns;l!=NULL;l=l->next)
		{
			MktDataColumn *col = MKT_DATA_COLUMN(l->data);
			const GValue* value = mkt_data_column_get_value(col,i);
			if(value)
			{
				gchar *vs = mkt_value_stringify(value);
				g_printf("%s | ",vs);
				g_free(vs);
			}
			else
			{
				g_printf("null | ");
			}
		}
		g_printf("\n");
	}
	mkt_mutex_unlock(data->priv->mutex);
}


/*GSList*
mkt_data_model_load_all ( MktDataModel *data , gboolean local )
{
	g_return_val_if_fail (data!=NULL,NULL);
	g_return_val_if_fail (MKT_IS_DATA_MODEL (data),NULL);
	guint ncols = mkt_data_model_get_column_nth(data);
	GType   *types   =  g_try_new0(GType,ncols+1);
	guint   *flags   =  g_try_new0(guint,ncols+1);
	gchar   **names  =  (gchar **)calloc(ncols+1,sizeof(gchar*));
	GSList **values;
	values = (GSList **)calloc(ncols+1,sizeof(GSList*));
	gint PK  = 0;
	GSList *models =NULL;
	GSList *curr = NULL;
	gboolean is_pk =FALSE;
	gint  columns  = 0;
	int i;
	for (i = 0; i < ncols; i++)
	{
		MktDataColumn *col = mkt_data_model_get_column(data,i);
		if(col)
		{
			types[i]          = mkt_data_column_get_gtype(col);
			flags[i]          = mkt_data_column_get_flags(col);
			values[i]         = (GSList*) mkt_data_column_get_values(col);
			names[i]          = (char*) mkt_data_column_get_name(col);
			if(MKT_MODEL_PK_PROP &flags[i] )
			{
				PK = i;	is_pk = TRUE;;
			}

			if( !(flags[i]&MKT_MODEL_DB_NOT_REDABLE) )columns++;
		}
		else
		{
			types[i]          = G_TYPE_NONE;
			values[i]         = NULL;
			flags[i]          = 0;
			names[i]          = NULL;
		}
	}
	while(values[PK]!=NULL)
	{

		MktModel *model = NULL;
		if(is_pk && local )
		{

			if(values[PK]->data != NULL)
			{
				GValue *value = values[PK]->data;
				//TEST:	g_debug("Load model %s ref_id = %s",mkt_data_model_get_type_name(data),mkt_value_stringify(value));
				if(G_VALUE_HOLDS_INT(value))
				{
					model = mkt_data_model_lookup_model_local_ref_id( data ,g_value_get_int(value));
					if(model)g_object_ref(model);
					//	g_debug("Lookup object in data model collection %p",model);
				}
			}
		}
		if(model == NULL)
		{
			GParameter *param=  calloc(sizeof (GParameter ),columns );
			gint npar = 0;
			gboolean is_brocked = FALSE;
			for (i = 0; i < ncols; i++)
			{
				if(values[i] !=NULL )
				{
					if(  !( flags[i] & MKT_MODEL_DB_NOT_REDABLE ) && names[i] != NULL )
					{
						param[npar].name   = names[i];
						param[npar].value.g_type = 0;
						const GValue *val = (const GValue*) values[i]->data;
						if(val != NULL)
						{
							g_value_init(&param[npar].value ,val->g_type);
							g_value_copy(val,&param[npar].value);
							//TEST:							g_debug("Test %s = %s",param[npar].name,mkt_value_stringify(&param[npar].value));
						}
						else
						{
							is_brocked = TRUE;
						}
						npar++;
					}
				}
			}
			if( npar > 0 && !is_brocked )
			{
				model=g_object_newv(mkt_data_model_object_type(data),npar,param);
				int i=0;
				for(i=0;i<npar ;i++ )
				{
					if(param[i].value.g_type > G_TYPE_NONE)
						g_value_unset(&param[i].value);
				}
				mkt_model_thaw_notify(model);
				if(local)mkt_data_model_insert_model_local(data,model);
			}
			g_free(param);

		}
		for (i = 0; i < ncols; i++)
			if(values[i] !=NULL )
				values[i] = values[i]->next;
		if(model )
		{
			//mkt_model_print_stdout(model);
			GSList *nl  = g_slice_new(GSList);
			nl->data = model;
			nl->next = NULL;
			if(curr!=NULL)
				curr->next = nl;
			curr=nl;
			if(models==NULL) models = nl;
		}
	}
	g_free(types);
	g_free(flags);
	g_free(names);
	g_free(values);
	return models;
}
*/

GParameter*
mkt_data_model_get_row_parameter ( MktDataModel *data , guint row ,guint *nparam )
{
	g_return_val_if_fail (data!=NULL,NULL);
	g_return_val_if_fail (MKT_IS_DATA_MODEL (data),NULL);
	g_return_val_if_fail (mkt_data_model_get_column_nth(data),NULL);
	mkt_mutex_lock(data->priv->mutex);
	guint columns =  0;
	GSList *col = NULL;
	for(col = data->priv->columns;col != NULL ; col=col->next)
	{
		//TEST:		g_debug("Test column %s type %s",mkt_data_column_get_name(MKT_DATA_COLUMN(col->data)),g_type_name(mkt_data_column_get_gtype(MKT_DATA_COLUMN(col->data)) ));
		if( !(mkt_data_column_get_flags(MKT_DATA_COLUMN(col->data))&MKT_MODEL_DB_NOT_REDABLE) )
		{
			columns++;
		}
	}
	GParameter *param=  calloc(sizeof (GParameter ),columns );
	guint max_column = columns;
	columns = 0;
	for(col = data->priv->columns;col != NULL ; col=col->next)
	{
	    guint flag = mkt_data_column_get_flags(MKT_DATA_COLUMN(col->data));
		if(  !( flag & MKT_MODEL_DB_NOT_REDABLE ) && (max_column > columns) )
		{
			//TEST:	g_debug("Set value column %s of type %s",mkt_data_column_get_name(MKT_DATA_COLUMN(col->data)),	g_type_name(mkt_data_column_get_gtype(MKT_DATA_COLUMN(col->data))));
			param[columns].name   = mkt_data_column_get_name(MKT_DATA_COLUMN(col->data));
			param[columns].value.g_type = 0;
			const GValue *value   = mkt_data_column_get_value(MKT_DATA_COLUMN(col->data),row);
			if(value)
			{
				g_value_init(&param[columns].value ,value->g_type);
				g_value_copy(value,&param[columns].value);
			}
			columns++;
		}
	}
	*nparam = columns-1;
	mkt_mutex_unlock(data->priv->mutex);
	return param;
}


GType
mkt_data_model_object_type         ( MktDataModel *data )
{
	g_return_val_if_fail(data != NULL,G_TYPE_NONE);
	g_return_val_if_fail(MKT_IS_DATA_MODEL(data),G_TYPE_NONE);
	return  data->priv->oType;
}

const gchar*
mkt_data_model_get_type_name       ( MktDataModel *data )
{
	g_return_val_if_fail(data != NULL,NULL);
	g_return_val_if_fail(MKT_IS_DATA_MODEL(data),NULL);
	return data->priv->model_type;
}

MktSqlParser*
mkt_data_model_get_create           ( MktDataModel *data )
{
	g_return_val_if_fail(data != NULL,NULL);
	g_return_val_if_fail(MKT_IS_DATA_MODEL(data),NULL);
	return data->priv->create;
}
MktSqlParser*
mkt_data_model_get_select          ( MktDataModel *data )
{
	g_return_val_if_fail(data != NULL,NULL);
	g_return_val_if_fail(MKT_IS_DATA_MODEL(data),NULL);
	return data->priv->select;
}

MktSqlParser*
mkt_data_model_get_check           ( MktDataModel *data )
{
	g_return_val_if_fail(data != NULL,NULL);
	g_return_val_if_fail(MKT_IS_DATA_MODEL(data),NULL);
	return data->priv->check;
}

MktSqlParser*
mkt_data_model_get_insert          ( MktDataModel *data )
{
	g_return_val_if_fail(data != NULL,NULL);
	g_return_val_if_fail(MKT_IS_DATA_MODEL(data),NULL);
	return data->priv->insert;
}

MktSqlParser*
mkt_data_model_get_replace          ( MktDataModel *data )
{
	g_return_val_if_fail(data != NULL,NULL);
	g_return_val_if_fail(MKT_IS_DATA_MODEL(data),NULL);
	return data->priv->insert_or_replace;
}
MktSqlParser*
mkt_data_model_get_delete          ( MktDataModel *data )
{
	g_return_val_if_fail(data != NULL,NULL);
	g_return_val_if_fail(MKT_IS_DATA_MODEL(data),NULL);
	return data->priv->delete;
}

MktSqlParser*
mkt_data_model_get_update          ( MktDataModel *data )
{
	g_return_val_if_fail(data != NULL,NULL);
	g_return_val_if_fail(MKT_IS_DATA_MODEL(data),NULL);
	return data->priv->update;
}

MktSqlParser*
mkt_data_model_get_notify          ( MktDataModel *data )
{
	g_return_val_if_fail(data != NULL,NULL);
	g_return_val_if_fail(MKT_IS_DATA_MODEL(data),NULL);
	return data->priv->notify;
}

gboolean
mkt_data_model_check_interface     ( MktDataModel *data , GType iface_type )
{
	g_return_val_if_fail(data != NULL,FALSE);
	g_return_val_if_fail(MKT_IS_DATA_MODEL(data),FALSE);
	mkt_mutex_lock(data->priv->mutex);
	GSList *liface =NULL;
	gboolean ret = FALSE;
	for(liface = data->priv->ifases ;liface != NULL;liface=liface->next )
	{
		//GType type = *((GType*)(liface->data));
		//TEST:g_debug("DMIFace =%s UIface=%s",g_type_name(type),g_type_name(iface_type));
		if(iface_type ==(*((GType*)liface->data)))
		{
			ret =  TRUE;
			break;
		}
	}
	mkt_mutex_unlock(data->priv->mutex);
	return ret;
}






