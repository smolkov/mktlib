/*
 * mktparcerapache.c
 * 
 * Copyright 2013 doseus <doseus@sascha-VirtualBox>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mkt-parcer-apache.h"
#include "mkt-model.h"
#include "mkt-value.h"

enum 
{
	APACHE_PROP0,
	APACHE_TYPE,
	APACHE_PORT,
	APACHE_METHOD,
	APACHE_LIMIT,
	APACHE_OFFSET,
	APACHE_ORDER_BY,

};


struct MktParcerApachePrivate_
{

	gchar         *SERVER_SOFTWARE;
	gchar         *SERVER_NAME;
	gchar         *SERVER_PROTOCOL;
	gchar         *SERVER_PORT;
	gchar         *REQUEST_URI;
    gchar         *REQUEST_METHOD;
    gchar         *GATEWAY_INTERFACE;
	gchar         *HTTP_CONNECTION;
	gchar         *PATH_INFO;
	gchar         *PATH_TRANSLATED;
	gchar         *REMOTE_HOST;
	gchar         *REMOTE_ADDR;
	gchar         *REMOTE_PORT;
	gchar         *REMOTE_IDENT;
	gchar         *SCRIPT_FILENAME;
	gchar         *SCRIPT_NAME;
	gchar         *QUERY_STRING;
	gchar         *CONTENT_TYPE;
	gchar         *CONTENT_LENGTH;
	
    guint          port;
    guint          method;
    GType          serialize_type;


    guint          limit;
    guint          offset;
    gchar         *order_by;

   
    
};

static void mkt_parcer_apache_finalize (GObject *object);

G_DEFINE_TYPE (MktParcerApache, mkt_parcer_apache, G_TYPE_OBJECT)

static void
mkt_parser_apache_set_property ( GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec )
{
	//TEST:	g_debug("sensor_set_property %s",pspec->name);
	g_return_if_fail (MKT_IS_PARCER_APACHE (object));
	MktParcerApache* parser = MKT_PARCER_APACHE(object);
	switch (prop_id)
	{
	case APACHE_PORT:
		parser->priv->port = g_value_get_uint(value);
		break;
	case APACHE_TYPE:
		parser->priv->serialize_type = g_value_get_gtype(value);
		break;
	case APACHE_METHOD:
		parser->priv->method = g_value_get_uint(value);
		break;
	case APACHE_LIMIT:
		parser->priv->limit = g_value_get_uint(value);
		break;
	case APACHE_OFFSET:
		parser->priv->offset = g_value_get_uint(value);
		break;
	case APACHE_ORDER_BY:
		if(parser->priv->order_by)g_free(parser->priv->order_by);
		parser->priv->order_by = g_strdup_printf("order by %s",g_value_get_string(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
	//TEST:	g_debug("sensor_set_property %s end",pspec->name);
}

static void
mkt_parser_apache_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	//TEST:	g_debug("sensor_get_property %s",pspec->name);
	g_return_if_fail (MKT_IS_PARCER_APACHE (object));
	MktParcerApache* parser = MKT_PARCER_APACHE(object);
	switch (prop_id)
	{
	case APACHE_PORT:
		g_value_set_uint(value,parser->priv->port);
		break;
	case APACHE_TYPE:
		g_value_set_gtype(value,parser->priv->serialize_type);
		break;
	case APACHE_METHOD:
		g_value_set_uint(value,parser->priv->method);
		break;
	case APACHE_LIMIT:
		g_value_set_uint(value,parser->priv->limit);
		break;
	case APACHE_OFFSET:
		g_value_set_uint(value,parser->priv->offset);
		break;
	case APACHE_ORDER_BY:
		g_value_set_string(value,parser->priv->order_by);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
	//TEST:	g_debug("sensor_get_property %s end",pspec->name);
}



static void
mkt_parcer_apache_class_init (MktParcerApacheClass *klass)
{
	GObjectClass *object_class;
	object_class = G_OBJECT_CLASS (klass);
	g_type_class_add_private ((gpointer)klass, sizeof (MktParcerApachePrivate));
	object_class->finalize = mkt_parcer_apache_finalize;
	
	object_class->set_property = mkt_parser_apache_set_property;
	object_class->get_property = mkt_parser_apache_get_property;
	
	GParamSpec *pspec;
	pspec = g_param_spec_gtype ("apache-serialize-type",
			"Scann parser socket prop",
			"Set|Get Scann parser socket property",
			G_TYPE_NONE,
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY );
	g_object_class_install_property(object_class,
			APACHE_TYPE,
			pspec);
	pspec = g_param_spec_uint ("apache-port",
			"Scann parser socket prop",
			"Set|Get Scann parser socket property",
			0,G_MAXUINT32,0,G_PARAM_READWRITE );
	g_object_class_install_property(object_class,
			APACHE_PORT,
			pspec);
	pspec = g_param_spec_uint ("limit",
			"Scann parser socket prop",
			"Set|Get Scann parser socket property",
			0,G_MAXUINT32,4000,G_PARAM_READWRITE | G_PARAM_CONSTRUCT );
	g_object_class_install_property(object_class,
			APACHE_LIMIT,
			pspec);
	pspec = g_param_spec_uint ("offset",
			"Scann parser socket prop",
			"Set|Get Scann parser socket property",
			0,G_MAXUINT32,0,G_PARAM_READWRITE | G_PARAM_CONSTRUCT  );
	g_object_class_install_property(object_class,
			APACHE_OFFSET,
			pspec);
	pspec = g_param_spec_string ("order-by",
				"Scann parser order by column name prop",
				"Set|Get Scann parser order by column name property",
				"",
				G_PARAM_READWRITE );
		g_object_class_install_property(object_class,
				APACHE_ORDER_BY,
				pspec);

	pspec = g_param_spec_uint ("apache-method",
			"Scann parser socket prop",
			"Set|Get Scann parser socket property",
			0,G_MAXUINT32,0,	G_PARAM_READWRITE );
	g_object_class_install_property(object_class,
			APACHE_METHOD,
			pspec);

	
}


static void
mkt_parcer_apache_finalize (GObject *object)
{
//  MktParcerApache *self;

  g_return_if_fail (MKT_IS_PARCER_APACHE (object));

// Gerhard
// Warnung:
//    warning: variable 'self' set but not used [-Wunused-but-set-variable]
// Lösung:
//    Herauskommentieren von 'self'
//  self = MKT_PARCER_APACHE (object);
//
// Jetzt neue Warnung:
//    value computed is not used [-Wunused-value]
// Lösung:
//     Verwenden von #pragma GCC diagnostic
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
  MKT_PARCER_APACHE (object);
#pragma GCC diagnostic pop

  G_OBJECT_CLASS (mkt_parcer_apache_parent_class)->finalize (object);
}


static void
mkt_parcer_apache_init (MktParcerApache *self)
{
	self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self, MKT_TYPE_PARCER_APACHE, MktParcerApachePrivate);
	/*self->priv->SERVER_SOFTWARE   = getenv ("SERVER_SOFTWARE");
	self->priv->SERVER_NAME       = getenv ("SERVER_NAME");
	self->priv->SERVER_PROTOCOL   = getenv ("SERVER_PROTOCOL");
	self->priv->SERVER_PORT       = getenv ("SERVER_PORT");
	self->priv->REQUEST_URI       = getenv ("REQUEST_URI");
    self->priv->REQUEST_METHOD    = getenv ("REQUEST_METHOD");
    self->priv->GATEWAY_INTERFACE = getenv ("GATEWAY_INTERFACE");
	self->priv->HTTP_CONNECTION   = getenv ("HTTP_CONNECTION");
	self->priv->PATH_INFO         = getenv ("PATH_INFO");
	self->priv->PATH_TRANSLATED   = getenv ("PATH_TRANSLATED");
	self->priv->REMOTE_HOST       = getenv ("REMOTE_HOST");
	self->priv->REMOTE_ADDR       = getenv ("REMOTE_ADDR");
	self->priv->REMOTE_PORT       = getenv ("REMOTE_PORT");
	self->priv->REMOTE_IDENT      = getenv ("REMOTE_IDENT");
	self->priv->SCRIPT_FILENAME   = getenv ("SCRIPT_FILENAME");
	self->priv->SCRIPT_NAME       = getenv ("SCRIPT_NAME");
	self->priv->QUERY_STRING      = getenv ("QUERY_STRING");
	self->priv->CONTENT_TYPE      = getenv ("CONTENT_TYPE");
	self->priv->CONTENT_LENGTH    = getenv ("CONTENT_LENGTH");*/
	char *method = getenv ("REQUEST_METHOD");
	if(method == NULL )
	{
		self->priv->method = APACHE_METHOD_NONE;
	}
	else if(0==strcmp(method,"GET"))
	{
		self->priv->method = APACHE_METHOD_GET;
	}
	else if(0==strcmp(method,"POST"))
	{
		self->priv->method = APACHE_METHOD_GET;
	}
	else
	{
		self->priv->method = APACHE_METHOD_UNKNOWN;	
	}
	char *port = getenv ("SERVER_PORT");
	if(port != NULL )
	{
		self->priv->port = atoi(port);
	}

	self->priv->order_by = NULL;
}

/*
Gerhard: warning: 'mkt_parcer_apache_deserialize' defined but not used [-Wunused-function]
static gboolean 
mkt_parcer_apache_deserialize ( MktParcerApache *parser )
{
// Warnung:
//    control reaches end of non-void function [-Wreturn-type]
// Lösung:
//    Einbau eines return
	return FALSE;
}
*/

gchar* 
mkt_parcer_apache_serialize ( MktParcerApache *parser )
{
	gchar *json = NULL;
	json = mkt_model_serialize(parser->priv->serialize_type,"select * from $tablename %s LIMIT %d OFFSET %d",parser->priv->order_by?parser->priv->order_by:"",parser->priv->limit,parser->priv->offset);
	return json;
}

GSList*
mkt_parcer_apache_objects ( MktParcerApache *parser )
{
	GSList *models = NULL;
	models = mkt_model_select(parser->priv->serialize_type,"select * from $tablename %s LIMIT %d OFFSET %d",parser->priv->order_by?parser->priv->order_by:"",parser->priv->limit,parser->priv->offset);
	return models;
}

gchar*
mkt_parser_create_object_http_table( MktParcerApache *parser , MktModel *model )
{
	GString *string = g_string_new("");
	guint len = 0;
	GParamSpec **pspec = g_object_class_list_properties(G_OBJECT_GET_CLASS(G_OBJECT(model)),&len);
	if(pspec)
	{

		g_string_append_printf(string,"<table border=\"0\" cellpadding=\"1\" cellspacing=\"1\" style=\"width: 400px;\">");
		g_string_append_printf(string,"<tbody>");
// Warnung:
//    format '%llu' expects argument of type 'long long unsigned int', but argument 3 has type 'guint64' [-Wformat]
// Lösung:
//    Verwendung des vordefinierten Formatbezeichners.
		g_string_append_printf(string,"<caption>Object index:%" G_GUINT64_FORMAT "</caption>",mkt_model_ref_id(MKT_IMODEL(model)));
		g_string_append_printf(string,"<tr>");

		g_string_append_printf(string,"</tr>");
		g_string_append_printf(string,"<td style=\"background-color: rgb(102, 102, 102);\"");
		g_string_append_printf(string,"<strong>Property</strong>");
		g_string_append_printf(string,"</td>");
		g_string_append_printf(string,"<td style=\"background-color: rgb(102, 102, 102);\"");
		g_string_append_printf(string,"<strong>Value</strong>");
		g_string_append_printf(string,"</td>");
		g_string_append_printf(string,"</tr>");
		int i = 0;
		for( i = 0; i<len;i++)
		{
			GValue *value = mkt_value_new(pspec[i]->value_type);
			g_object_get_property(G_OBJECT(model),pspec[i]->name,value);
			gchar *vstr = mkt_value_stringify(value);
			g_string_append_printf(string,"<tr>");
			g_string_append_printf(string,"<td style=\"text-align: left; vertical-align: middle;background-color: rgb(204, 204, 204);\">");
			g_string_append_printf(string,"<span style=\"font-family:arial,helvetica,sans-serif;\"><em><span style=\"color:#006400;\">%s</span></em></span>",pspec[i]->name);
			g_string_append_printf(string,"</td>");
			g_string_append_printf(string,"<td>");
			g_string_append_printf(string,"<input name=\"property-value\" type=\"text\" value=\"%s\" /></td>",vstr);
			g_string_append_printf(string,"</tr>");
			if(value)mkt_value_free(value);
			if(vstr)g_free(vstr);
		}
		g_string_append_printf(string,"</tbody>");
		g_string_append_printf(string,"</table>");
	}
	gchar *table = string->str;
	g_string_free(string,FALSE);
	return table;
}


GObject *
mkt_parcer_apache_new ( GType otype )
{
	return g_object_new ( MKT_TYPE_PARCER_APACHE,"apache-serialize-type",otype,NULL);
  
}
