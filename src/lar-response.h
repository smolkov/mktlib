/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * lar-response.h
 * Copyright (C) 2017 sascha.smolkov <saschasmolkov@saschasmolkov>
 *
 * lar-response.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public Response as published by the
 * Free Software Foundation, either version 3 of the Response, or
 * (at your option) any later version.
 *
 * lar-response.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public Response for more details.
 *
 * You should have received a copy of the GNU General Public Response along
 * with this program.  If not, see <http://www.gnu.org/responses/>.
 */

#ifndef _LAR_RESPONSE_H_
#define _LAR_RESPONSE_H_

#include <glib-object.h>
#include <libsoup/soup.h>

G_BEGIN_DECLS

#define LAR_TYPE_RESPONSE (lar_response_get_type())
#define LAR_RESPONSE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), LAR_TYPE_RESPONSE, LarResponse))
#define LAR_RESPONSE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), LAR_TYPE_RESPONSE, LarResponseClass))
#define LAR_IS_RESPONSE(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), LAR_TYPE_RESPONSE))
#define LAR_IS_RESPONSE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), LAR_TYPE_RESPONSE))
#define LAR_RESPONSE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), LAR_TYPE_RESPONSE, LarResponseClass))

typedef struct _LarResponseClass   LarResponseClass;
typedef struct _LarResponse        LarResponse;
typedef struct _LarResponsePrivate LarResponsePrivate;

struct _LarResponseClass {
    GObjectClass parent_class;
    gboolean (*reload_stream) (LarResponse *response, GInputStream *stream);
};

struct _LarResponse {
    GObject parent_instance;

    LarResponsePrivate *priv;
};

GType lar_response_get_type(void) G_GNUC_CONST;
guint lar_response_get_code(LarResponse *response);
SoupMessage *lar_response_get_message(LarResponse *response);

G_END_DECLS

#endif /* _LAR_RESPONSE_H_ */
