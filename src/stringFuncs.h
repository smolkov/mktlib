#ifndef STRING_FUNCS_H_
#define STRING_FUNCS_H_

#define ULTRA_VERSION "5.0.0"

/* Falls onlyPost == FALSE: Stelle 'digits' signifikante Stellen dar.
-- Sonst: Stelle 'digits' Nachkommastellen dar.
*/
const gchar* doubleStr (gdouble value, gboolean onlyPost, guint digits);
#define doubleStr(value) doubleStr (value, FALSE, 5)

#endif
