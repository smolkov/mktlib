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
    g_mkdir_with_parents("/usr/share/tera/db", 0777);
}
