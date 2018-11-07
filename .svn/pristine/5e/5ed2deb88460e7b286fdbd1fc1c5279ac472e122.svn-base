#include <glib.h>
#include <glib/gprintf.h>
#include "stringFuncs.h"

#undef doubleStr
const gchar* doubleStr (gdouble value, gboolean onlyPost, guint digits)
{
	static gchar  buffer [1024];
	static gchar* writePtr = buffer;

	gdouble compare = 1;
	guint   postDot;
	gchar*  result  = writePtr;
	int     sign    = 1;

	if (onlyPost)
		postDot = digits;

	else {
		if (value < 0) {
			sign  = -1;
			value = - value;
			digits --;
		}

		for (postDot=1; postDot<digits; postDot++)  // Für digits = 0 1  2   3 ...
			compare *= 10;                          //              1 1 10 100 ...

		for (postDot=0; postDot<digits; postDot++) {
			if (compare < value)
				break;

			compare /= 10;
		}

		if (sign < 0)
			value = - value;
	}

	writePtr += g_sprintf (writePtr, "%.*f", postDot, value);

	if (&buffer[1024] - writePtr < 128)
		writePtr = buffer;

	return result;
}
