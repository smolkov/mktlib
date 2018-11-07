/*
 * mktparcerapache.h
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


#ifndef MKTPARCERAPACHE_H_
#define MKTPARCERAPACHE_H_ 1

#include <glib-object.h>
#include "mkt-model.h"

G_BEGIN_DECLS


#define MKT_TYPE_PARCER_APACHE             (mkt_parcer_apache_get_type ())
#define MKT_PARCER_APACHE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_PARCER_APACHE, MktParcerApache))
#define MKT_PARCER_APACHE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), MKT_TYPE_PARCER_APACHE, MktParcerApacheClass))
#define MKT_IS_PARCER_APACHE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_PARCER_APACHE))
#define MKT_IS_PARCER_APACHE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), MKT_TYPE_PARCER_APACHE))
#define MKT_PARCER_APACHE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), MKT_TYPE_PARCER_APACHE, MktParcerApacheClass))

typedef struct MktParcerApache_         MktParcerApache;
typedef struct MktParcerApacheClass_    MktParcerApacheClass;
typedef struct MktParcerApachePrivate_  MktParcerApachePrivate;

enum
{
	APACHE_METHOD_NONE,
	APACHE_METHOD_GET,
	APACHE_METHOD_POST,
	APACHE_METHOD_UNKNOWN,
	APACHE_METHOD_LAST
};


struct MktParcerApache_
{
  GObject parent;
  /* add your public declarations here */
  MktParcerApachePrivate *priv;
};

struct MktParcerApacheClass_
{
  GObjectClass parent_class;
};


GType               mkt_parcer_apache_get_type          ( void );

GObject*            mkt_parcer_apache_new               ( GType otype );
gchar*              mkt_parcer_apache_serialize         ( MktParcerApache *parser );
GSList*             mkt_parcer_apache_objects           ( MktParcerApache *parser );
gchar*              mkt_parser_create_object_http_table ( MktParcerApache *parser , MktModel *model );


G_END_DECLS

#endif /* MKTPARCERAPACHE_H_ */
