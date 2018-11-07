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

#ifndef _MKT_BOOEXP_IND_H_
#define _MKT_BOOEXP_IND_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define MKT_TYPE_WARN (mkt_warn_get_type())
#define MKT_WARN(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), MKT_TYPE_WARN, MktWarn))
#define MKT_WARN_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), MKT_TYPE_WARN, MktWarnClass))
#define MKT_IS_WARN(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), MKT_TYPE_WARN))
#define MKT_IS_WARN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), MKT_TYPE_WARN))
#define MKT_WARN_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), MKT_TYPE_WARN, MktWarnClass))

typedef struct _MktWarnClass   MktWarnClass;
typedef struct _MktWarn        MktWarn;
typedef struct _MktWarnPrivate MktWarnPrivate;

struct _MktWarnClass {
    GObjectClass parent_class;
};

struct _MktWarn {
    GObject parent_instance;

    MktWarnPrivate *priv;
};

GType mkt_warn_get_type(void) G_GNUC_CONST;
MktWarn *mkt_warn_new();



#define MKT_TYPE_FAIL (mkt_fail_get_type())
#define MKT_FAIL(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), MKT_TYPE_FAIL, MktFail))
#define MKT_FAIL_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), MKT_TYPE_FAIL, MktFailClass))
#define MKT_IS_FAIL(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), MKT_TYPE_FAIL))
#define MKT_IS_FAIL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), MKT_TYPE_FAIL))
#define MKT_FAIL_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), MKT_TYPE_FAIL, MktFailClass))

typedef struct _MktFailClass   MktFailClass;
typedef struct _MktFail        MktFail;
typedef struct _MktFailPrivate MktFailPrivate;

struct _MktFailClass {
    GObjectClass parent_class;
};

struct _MktFail {
    GObject parent_instance;

    MktFailPrivate *priv;
};

GType mkt_fail_get_type(void) G_GNUC_CONST;

MktFail *mkt_fail_new();

G_END_DECLS

#endif /* _MKT_BOOEXP_IND_H_ */
