#include <stdio.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <gio/gio.h>



int libfunc(void)
{
	printf("Hello world\n");
	return (0);
}




void library_database() {
    g_mkdir_with_parents("/usr/share/tera/db", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    GPtrArray *args;
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
