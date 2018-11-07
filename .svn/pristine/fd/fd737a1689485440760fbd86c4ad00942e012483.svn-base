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

#ifndef _MKT_SQL_PARSER_H_
#define _MKT_SQL_PARSER_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define MKT_TYPE_SQL_PARSER             (mkt_sql_parser_get_type ())
#define MKT_SQL_PARSER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_SQL_PARSER, MktSqlParser))
#define MKT_SQL_PARSER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), MKT_TYPE_SQL_PARSER, MktSqlParserClass))
#define MKT_IS_SQL_PARSER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_SQL_PARSER))
#define MKT_IS_SQL_PARSER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), MKT_TYPE_SQL_PARSER))
#define MKT_SQL_PARSER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), MKT_TYPE_SQL_PARSER, MktSqlParserClass))

typedef struct _MktSqlParserClass MktSqlParserClass;
typedef struct _MktSqlParser MktSqlParser;
typedef struct _MktSqlParserPrivate MktSqlParserPrivate;



struct _MktSqlParserClass
{
	GObjectClass parent_class;
};

struct _MktSqlParser
{
	GObject parent_instance;

    MktSqlParserPrivate *priv;
};

GType mkt_sql_parser_get_type (void) G_GNUC_CONST;



MktSqlParser*             mkt_sql_parser_new                     (  const gchar *string );

void                      mkt_sql_parser_set_default_pk_value    ( MktSqlParser *parser , GValue *value );

gchar*                    mkt_sql_parser_extended                ( MktSqlParser* parser , const gchar *parcer_str, GObject *params );
gchar*                    mkt_sql_parser_extended_default        ( MktSqlParser* parser , GObject *params );

void                      mkt_sql_parser_parce_string            ( MktSqlParser *parser , const gchar *sql );
const gchar*              mkt_sql_parser_get_default_string      ( MktSqlParser *parser );
void                      mkt_parser_add_set_property            ( MktSqlParser *parser , const gchar *pname );


const gchar*              mkt_sql_parser_get_pk                  ( MktSqlParser *parser );
const gchar*              mkt_sql_parser_rps                     ( const gchar *pname );
gchar*                    mkt_sql_parser_dup_rps                 ( const gchar *pname );
gboolean                  mkt_sql_parser_add_value               ( MktSqlParser *parser ,const gchar *id , const gchar *value );

gboolean                  mkt_sql_parser_append_sql              ( GType type ,const gchar *format,...)G_GNUC_PRINTF (2, 3);


G_END_DECLS

#endif /* _MKT_SQL_PARSER_H_ */
