/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * lar-license.h
 * Copyright (C) 2017 sascha.smolkov <saschasmolkov@saschasmolkov>
 *
 * lar-license.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lar-license.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _LAR_LICENSE_H_
#define _LAR_LICENSE_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define LAR_TYPE_LICENSE (lar_license_get_type())
#define LAR_LICENSE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), LAR_TYPE_LICENSE, LarLicense))
#define LAR_LICENSE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), LAR_TYPE_LICENSE, LarLicenseClass))
#define LAR_IS_LICENSE(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), LAR_TYPE_LICENSE))
#define LAR_IS_LICENSE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), LAR_TYPE_LICENSE))
#define LAR_LICENSE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), LAR_TYPE_LICENSE, LarLicenseClass))

typedef struct _LarLicenseClass   LarLicenseClass;
typedef struct _LarLicense        LarLicense;
typedef struct _LarLicensePrivate LarLicensePrivate;

struct _LarLicenseClass {
    GObjectClass parent_class;
};

struct _LarLicense {
    GObject parent_instance;

    LarLicensePrivate *priv;
};

GType lar_license_get_type(void) G_GNUC_CONST;
void lar_license_print_test(LarLicense *lic);
const gchar *lar_license_get_product(LarLicense *lic);
const gchar *lar_license_get_specification(LarLicense *lic);
const gchar *lar_license_get_license_type(LarLicense *lic);
const gchar *lar_license_get_key(LarLicense *lic);
LarLicense *lar_license_new(const gchar *product, const gchar *specification);

GList *lar_licensse_deserialize_list(const gchar *body);

G_END_DECLS

#endif /* _LAR_LICENSE_H_ */
