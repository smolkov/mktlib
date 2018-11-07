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

#ifndef _MKT_BOOEXP_PARSER_H_
#define _MKT_BOOEXP_PARSER_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define MKT_TYPE_BOOEXP_PARSER (mkt_boolexp_parser_get_type())
#define MKT_BOOEXP_PARSER(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), MKT_TYPE_BOOEXP_PARSER, MktBoolexpParser))
#define MKT_BOOEXP_PARSER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), MKT_TYPE_BOOEXP_PARSER, MktBoolexpParserClass))
#define MKT_IS_BOOEXP_PARSER(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), MKT_TYPE_BOOEXP_PARSER))
#define MKT_IS_BOOEXP_PARSER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), MKT_TYPE_BOOEXP_PARSER))
#define MKT_BOOEXP_PARSER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), MKT_TYPE_BOOEXP_PARSER, MktBoolexpParserClass))

typedef struct _MktBoolexpParserClass   MktBoolexpParserClass;
typedef struct _MktBoolexpParser        MktBoolexpParser;
typedef struct _MktBoolexpParserPrivate MktBoolexpParserPrivate;

struct _MktBoolexpParserClass {
    GObjectClass parent_class;
};

struct _MktBoolexpParser {
    GObject parent_instance;

    MktBoolexpParserPrivate *priv;
};

GType mkt_boolexp_parser_get_type(void) G_GNUC_CONST;

MktBoolexpParser *mkt_boolexp_parser_new();
gint mkt_boolexp_parser_check(MktBoolexpParser *parser, const gchar *format, ...) G_GNUC_PRINTF(2, 3);
const gchar *mkt_boolexp_parser_last_error(MktBoolexpParser *parser);

G_END_DECLS

#endif /* _MKT_BOOEXP_PARSER_H_ */
