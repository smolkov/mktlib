/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mkt-measurement-data.c
 * Copyright (C) sascha 2013 <sascha@sascha-VirtualBox>
 * 
mkt-measurement-data.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * mkt-measurement-data.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MKT_ERROR_MESSAGE_H_
#define _MKT_ERROR_MESSAGE_H_

#include "mkt-model.h"
#include "mkt-error.h"

G_BEGIN_DECLS

#define MKT_TYPE_ERROR_MESSAGE             (mkt_error_message_get_type ())
#define MKT_ERROR_MESSAGE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_ERROR_MESSAGE, MktErrorMessage))
#define MKT_ERROR_MESSAGE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), MKT_TYPE_ERROR_MESSAGE, MktErrorMessageClass))
#define MKT_IS_ERROR_MESSAGE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_ERROR_MESSAGE))
#define MKT_IS_ERROR_MESSAGE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), MKT_TYPE_ERROR_MESSAGE))
#define MKT_ERROR_MESSAGE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), MKT_TYPE_ERROR_MESSAGE, MktErrorMessageClass))

typedef struct _MktErrorMessageClass   MktErrorMessageClass;
typedef struct _MktErrorMessage        MktErrorMessage;
typedef struct _MktErrorMessagePrivate MktErrorMessagePrivate;


struct _MktErrorMessageClass
{
	MktModelClass parent_class;
};

struct _MktErrorMessage
{
	MktModel parent_instance;
	MktErrorMessagePrivate *priv;
};

GType                   mkt_error_message_get_type                 (void) G_GNUC_CONST;

G_END_DECLS

#endif /* _MKT_ERROR_MESSAGE_H_ */
