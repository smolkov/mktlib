/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2016, LAR.
 */
#include "lar-core.h"
#include <fcntl.h>
#include <stdlib.h>

#define CORE_SQL_LOCATION "/usr/share/lar/db"

void core_init_database() {
    GPtrArray *args;
    g_mkdir_with_parents(CORE_SQL_LOCATION, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    args = g_ptr_array_new_with_free_func(g_free);
    g_ptr_array_add(args, g_strdup("chmod"));
    g_ptr_array_add(args, g_strdup("-R"));
    g_ptr_array_add(args, g_strdup("777"));
    g_ptr_array_add(args, g_strdup("/usr/share/tera/db"));
    g_ptr_array_add(args, NULL);
    GError *     error   = NULL;
    GSubprocess *process = g_subprocess_newv((const gchar *const *)args->pdata, G_SUBPROCESS_FLAGS_NONE, &error);
    if (process == NULL || error) {
        g_critical("Update error = %s", error ? error->message : "unknown");
    }
    g_subprocess_wait(process, NULL, NULL);
    g_object_unref(process);
}

const gchar *lar_core_db_path() { return CORE_SQL_LOCATION; }

void lar_core_init() { core_init_database(); }
