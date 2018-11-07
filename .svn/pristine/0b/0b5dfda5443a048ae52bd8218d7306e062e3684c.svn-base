/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * lar-response.h
 * Copyright (C) 2017 sascha.smolkov <saschasmolkov@saschasmolkov>
 *
 * lar-response.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public ResponseLicense as published by the
 * Free Software Foundation, either version 3 of the ResponseLicense, or
 * (at your option) any later version.
 *
 * lar-response.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public ResponseLicense for more details.
 *
 * You should have received a copy of the GNU General Public ResponseLicense along
 * with this program.  If not, see <http://www.gnu.org/responses/>.
 */

#ifndef _LAR_RESPONSE_LICENSE_H_
#define _LAR_RESPONSE_LICENSE_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define LAR_TYPE_RESPONSE_LICENSE (lar_response_get_type())
#define LAR_RESPONSE_LICENSE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), LAR_TYPE_RESPONSE_LICENSE, LarResponseLicense))
#define LAR_RESPONSE_LICENSE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), LAR_TYPE_RESPONSE_LICENSE, LarResponseLicenseClass))
#define LAR_IS_RESPONSE_LICENSE(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), LAR_TYPE_RESPONSE_LICENSE))
#define LAR_IS_RESPONSE_LICENSE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), LAR_TYPE_RESPONSE_LICENSE))
#define LAR_RESPONSE_LICENSE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), LAR_TYPE_RESPONSE_LICENSE, LarResponseLicenseClass))

typedef struct _LarResponseLicenseClass   LarResponseLicenseClass;
typedef struct _LarResponseLicense        LarResponseLicense;
typedef struct _LarResponseLicensePrivate LarResponseLicensePrivate;

struct _LarResponseLicenseClass {
    GObjectClass parent_class;
};

struct _LarResponseLicense {
    GObject parent_instance;

    LarResponseLicensePrivate *priv;
};

GType lar_response_get_type(void) G_GNUC_CONST;
G_END_DECLS

#endif /* _LAR_RESPONSE_LICENSE_H_ */
