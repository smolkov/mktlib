/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * lar-mongo-pool.h
 * Copyright (C) 2017 sascha.smolkov <saschasmolkov@saschasmolkov>
 *
 * lar-mongo-pool.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lar-mongo-pool.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _LAR_MONGO_POOL_H_
#define _LAR_MONGO_POOL_H_

#include "lar-license.h"
#include <glib-object.h>
#include <glib.h>

G_BEGIN_DECLS

void lar_mongo_pool_initialize(const gchar *adress, guint port, const gchar *dbname, const gchar *app);
gboolean lar_mongo_pool_connected();
GList *lar_mongo_pool_find_licenses(const gchar *jq, guint skip, guint limit);
G_END_DECLS

#endif /* _LAR_MONGO_POOL_H_ */
