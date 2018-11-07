/*
 * mkt-value.c
 *
 *  Created on: 02.05.2013
 *      Author: sascha
 */

 #include "value.h"
 #include <ctype.h>
 #include <fcntl.h>
 #include <locale.h>
 #include <math.h>
 #include <stdlib.h>
 #include <string.h>
 #include <sys/stat.h>
 #include <sys/types.h>
 #include <unistd.h>
 
 #define l_g_value_unset(val)                     \
     G_STMT_START {                               \
         if (G_IS_VALUE(val)) g_value_unset(val); \
     }                                            \
     G_STMT_END
 
 static GRecMutex init_rmutex;
 #define MUTEX_LOCK() g_rec_mutex_lock(&init_rmutex)
 #define MUTEX_UNLOCK() g_rec_mutex_unlock(&init_rmutex)
 
 gboolean value_string_digit(const gchar *value) {
     // g_debug("%s",value);
     gchar *p    = (gchar *)value;
     gint   koma = 0;
     while ((*p != '\n' && *p != '\0')) {
         if (!g_ascii_isdigit(*p)) {
             // g_printf("|%c|",*p);
             if ((*p == '.' && koma == 0))
                 koma++;
             else
                 return FALSE;
         }
         p++;
     }
     // g_printf("\n");
     return TRUE;
 }
 
 GValue *value_new(GType type) {
     GValue *value;
     value = calloc(1, sizeof(GValue));
     if (type > G_TYPE_NONE)
         g_value_init(value, type);
     else
         value->g_type = type;
     return value;
 }
 
 void value_free(GValue *value) {
     if (!value) return;
     l_g_value_unset(value);
     g_free(value);
 }
 
 gboolean set_gvalue_from_string(GValue *value, const gchar *as_string) {
     gboolean retval;
     gchar *  endptr[1];
     gdouble  dvalue;
     glong    lvalue;
     gulong   ulvalue;
     GType    type;
 
     g_return_val_if_fail(value, FALSE);
     g_return_val_if_fail(as_string != NULL, FALSE);
     // g_debug("transform value type %s string %s",g_type_name(value->g_type),as_string);
     if (!G_IS_VALUE(value)) {
         g_warning("Can't set value for a G_TYPE_INVALID GValue");
         return FALSE;
     } else if (value->g_type <= G_TYPE_NONE) {
         g_warning("Can't set value for a NULL GValue");
         return FALSE;
     }
     // TEST:g_debug("transfor value type %s vrom str=%s",g_type_name(value->g_type),as_string);
     type = G_VALUE_TYPE(value);
     g_value_reset(value);
 
     /* custom transform function */
     retval = FALSE;
     if (type == G_TYPE_BOOLEAN) {
         if ((as_string[0] == 't') || (as_string[0] == 'T')) {
             g_value_set_boolean(value, TRUE);
             retval = TRUE;
         } else if ((as_string[0] == 'f') || (as_string[0] == 'F')) {
             g_value_set_boolean(value, FALSE);
             retval = TRUE;
         } else {
             gint i;
             i = atoi(as_string); /* Flawfinder: ignore */
             g_value_set_boolean(value, i ? TRUE : FALSE);
             retval = TRUE;
         }
     } else if (type == G_TYPE_INT64) {
         gint64 i64;
         if (g_str_has_prefix(as_string, "0x"))
             i64 = g_ascii_strtoll(as_string, endptr, 16);
         else
             i64 = g_ascii_strtoll(as_string, endptr, 10);
         if (*as_string != '\0' && **endptr == '\0') {
             g_value_set_int64(value, i64);
             retval = TRUE;
         }
     } else if (type == G_TYPE_UINT64) {
         guint64 ui64;
         if (g_str_has_prefix(as_string, "0x"))
             ui64 = g_ascii_strtoull(as_string, endptr, 16);
         else
             ui64 = g_ascii_strtoull(as_string, endptr, 10);
         if (*as_string != '\0' && **endptr == '\0') {
             g_value_set_uint64(value, ui64);
             retval = TRUE;
         }
     } else if (type == G_TYPE_INT) {
         if (g_str_has_prefix(as_string, "0x"))
             lvalue = strtol(as_string, endptr, 16);
         else
             lvalue = strtol(as_string, endptr, 10);
         if (*as_string != '\0' && **endptr == '\0') {
             g_value_set_int(value, (gint32)lvalue);
             retval = TRUE;
         }
     } else if (type == G_TYPE_UINT) {
         if (g_str_has_prefix(as_string, "0x"))
             ulvalue = strtoul(as_string, endptr, 16);
         else
             ulvalue = strtoul(as_string, endptr, 10);
         if (*as_string != 0 && **endptr == 0) {
             g_value_set_uint(value, (guint32)ulvalue);
             retval = TRUE;
         }
     } else if (type == G_TYPE_CHAR) {
         if (g_str_has_prefix(as_string, "0x"))
             lvalue = strtol(as_string, endptr, 16);
         else
             lvalue = strtol(as_string, endptr, 10);
         if (*as_string != 0 && **endptr == 0) {
 #if GLIB_CHECK_VERSION(2, 31, 7)
             g_value_set_schar(value, (gint)lvalue);
 #else
             g_value_set_char(value, (gchar)lvalue);
 #endif
             retval = TRUE;
         }
     } else if (type == G_TYPE_UCHAR) {
         if (g_str_has_prefix(as_string, "0x"))
             ulvalue = strtoul(as_string, endptr, 16);
         else
             ulvalue = strtoul(as_string, endptr, 10);
         if (*as_string != 0 && **endptr == 0) {
             g_value_set_uchar(value, (guchar)ulvalue);
             retval = TRUE;
         }
     } else if (type == G_TYPE_FLOAT) {
         dvalue = g_ascii_strtod(as_string, endptr);
         if (*as_string != '\0' && **endptr == '\0') {
             g_value_set_float(value, (gfloat)dvalue);
             retval = TRUE;
         }
     } else if (type == G_TYPE_DOUBLE) {
         dvalue = g_ascii_strtod(as_string, endptr);
         if (*as_string != '\0' && **endptr == '\0') {
             g_value_set_double(value, dvalue);
             retval = TRUE;
         }
     } else if (type == G_TYPE_GTYPE) {
         GType gt;
         gt = g_type_from_name(as_string);
         if (gt != G_TYPE_INVALID) {
             g_value_set_gtype(value, gt);
             retval = TRUE;
         }
     } else if (type == G_TYPE_ULONG) {
         gulong ulvalue;
         if (g_str_has_prefix(as_string, "0x"))
             ulvalue = strtoul(as_string, endptr, 16);
         else
             ulvalue = strtoul(as_string, endptr, 10);
         if (*as_string != 0 && **endptr == 0) {
             g_value_set_ulong(value, ulvalue);
             retval = TRUE;
         }
     } else if (type == G_TYPE_LONG) {
         glong lvalue;
         if (g_str_has_prefix(as_string, "0x"))
             lvalue = strtol(as_string, endptr, 16);
         else
             lvalue = strtol(as_string, endptr, 10);
         if (*as_string != 0 && **endptr == 0) {
             g_value_set_long(value, lvalue);
             retval = TRUE;
         }
     }
 
     if (!retval && g_value_type_transformable(G_TYPE_STRING, type)) {
         /* use the GLib type transformation function */
         GValue *string;
 
         string = calloc(1, sizeof(GValue));
         g_value_init(string, G_TYPE_STRING);
         g_value_set_string(string, as_string);
 
         g_value_transform(string, value);
         value_free(string);
 
         retval = TRUE;
     }
 
     return retval;
 }
 
 gchar *value_stringify(const GValue *value) {
     if (!value) return g_strdup("NULL");
     if (G_VALUE_TYPE(value) == G_TYPE_BOOLEAN) return g_value_get_boolean(value) ? g_strdup("1") : g_strdup("0");
     if (g_value_type_transformable(G_VALUE_TYPE(value), G_TYPE_STRING)) {
         GValue *string;
         gchar * str;
         string            = value_new(G_TYPE_STRING);
         gchar *old_locale = NULL;
         if (G_VALUE_HOLDS_FLOAT(value) || G_VALUE_HOLDS_DOUBLE(value)) {
             //FIXME: gchar *g_ascii_formatd (gchar *buffer,gint buf_len,const gchar *format,gdouble d);
             old_locale = g_strdup(setlocale(LC_ALL, NULL));
             setlocale(LC_ALL, "en_US.UTF-8");
         }
         g_value_transform(value, string);
         str = g_value_dup_string(string);
         value_free(string);
         if (old_locale) {
             setlocale(LC_ALL, old_locale);
             g_free(old_locale);
         }
         return str;
     } else
         return g_strdup("");
 }
 
 const gchar *value_stringify_static(const GValue *value) {
     MUTEX_LOCK();
     static gchar *internal_value = NULL;
     if (internal_value) g_free(internal_value);
     if (!value)
         internal_value = g_strdup("NULL");
     else {
         if (G_VALUE_TYPE(value) == G_TYPE_BOOLEAN) {
             internal_value = g_value_get_boolean(value) ? g_strdup("1") : g_strdup("0");
         } else if (G_VALUE_TYPE(value) == G_TYPE_DOUBLE) {
             //FIXME: gchar *g_ascii_formatd (gchar *buffer,gint buf_len,const gchar *format,gdouble d);
             if (fabs(g_value_get_double(value)) < 1) {
                 internal_value = g_strdup_printf("%.3f", g_value_get_double(value));
             } else if (fabs(g_value_get_double(value)) < 10) {
                 internal_value = g_strdup_printf("%.2f", g_value_get_double(value));
             } else if (fabs(g_value_get_double(value)) < 100) {
                 internal_value = g_strdup_printf("%.1f", g_value_get_double(value));
             } else {
                 internal_value = g_strdup_printf("%.0f", g_value_get_double(value));
             }
         } else if (g_value_type_transformable(G_VALUE_TYPE(value), G_TYPE_STRING)) {
             GValue *string;
             string = value_new(G_TYPE_STRING);
             g_value_transform(value, string);
             internal_value = g_value_dup_string(string);
             value_free(string);
         } else {
             internal_value = g_strdup("");
         }
     }
     MUTEX_UNLOCK();
     return internal_value;
 }
 
 gchar *value_dup_double(gdouble value) {
     gchar *  text_value = NULL;
     gboolean is_0       = FALSE;
     //FIXME: gchar *g_ascii_formatd (gchar *buffer,gint buf_len,const gchar *format,gdouble d);
     if (fabs(value) < 0.001) {
         text_value = g_strdup_printf("%.4f", value);
     } else if (fabs(value) < 0.01) {
         text_value = g_strdup_printf("%.3f", value);
     } else if (fabs(value) < 2) {
         text_value = g_strdup_printf("%.3f", value);
     } else if (fabs((value)) < 10) {
         text_value = g_strdup_printf("%.2f", (value));
     } else if (fabs((value)) < 100) {
         text_value = g_strdup_printf("%.1f", (value));
     } else {
         text_value = g_strdup_printf("%.0f", (value));
         is_0       = TRUE;
     }
     if (!is_0) {
         gchar *p = NULL;
         p        = text_value;
         p += strlen(p) - 1;
         for (; ((*(p - 1) != '.') && (*(p - 1) != ',') && (*p == '0')); p--) *p = '\0';
     }
     return text_value;
 }
 
 const gchar *value_stringify_double(gdouble value) {
     MUTEX_LOCK();
     static gchar internal_value[65];
     memset(internal_value, 0, sizeof(internal_value));
     gchar *  temp_value = NULL;
     gboolean is_0       = FALSE;
     ;
     if (fabs(value) < 0.001) {
         temp_value = g_strdup_printf("%.4f", value);
     } else if (fabs(value) < 1) {
         temp_value = g_strdup_printf("%.3f", value);
     } else if (fabs((value)) < 10) {
         temp_value = g_strdup_printf("%.2f", (value));
     } else if (fabs((value)) < 100) {
         temp_value = g_strdup_printf("%.1f", (value));
         is_0       = TRUE;
     } else {
         temp_value = g_strdup_printf("%.0f", (value));
         is_0       = TRUE;
     }
     strncpy(internal_value, temp_value, 64);
     g_free(temp_value);
     if (!is_0) {
         gchar *p = NULL;
         p        = internal_value;
         p += strlen(p) - 1;
         for (; ((*(p - 1) != '.') && (*(p - 1) != ',') && (*p == '0')); p--) *p = '\0';
     }
     MUTEX_UNLOCK();
     return internal_value;
 }
 
 const gchar *value_stringify_double_enUS(gdouble value) {
     MUTEX_LOCK();
     static gchar internal_value[128];
     gchar *      old_locale = g_strdup(setlocale(LC_ALL, NULL));
     setlocale(LC_ALL, "en_US.UTF-8");
     memset(internal_value, 0, sizeof(internal_value));
     //FIXME: gchar *g_ascii_formatd (gchar *buffer,gint buf_len,const gchar *format,gdouble d);
     gchar *  temp_value = NULL;
     gboolean is_0       = FALSE;
     ;
     if (fabs(value) < 0.001) {
         temp_value = g_strdup_printf("%.4f", value);
     } else if (fabs(value) < 1) {
         temp_value = g_strdup_printf("%.3f", value);
     } else if (fabs((value)) < 10) {
         temp_value = g_strdup_printf("%.2f", (value));
     } else if (fabs((value)) < 100) {
         temp_value = g_strdup_printf("%.1f", (value));
         is_0       = TRUE;
     } else {
         temp_value = g_strdup_printf("%.0f", (value));
         is_0       = TRUE;
     }
     strncpy(internal_value, temp_value, 64);
     g_free(temp_value);
     if (!is_0) {
         gchar *p = NULL;
         p        = internal_value;
         p += strlen(p) - 1;
         for (; ((*(p - 1) != '.') && (*(p - 1) != ',') && (*p == '0')); p--) *p = '\0';
     }
     if (old_locale) {
         setlocale(LC_ALL, old_locale);
         g_free(old_locale);
     }
     MUTEX_UNLOCK();
     return internal_value;
 }
 
 gchar *value_dup_double_enUS(gdouble value) {
     MUTEX_LOCK();
     gchar *internal_value = NULL;
     gchar *old_locale     = g_strdup(setlocale(LC_ALL, NULL));
     setlocale(LC_ALL, "en_US.UTF-8");
     internal_value = value_dup_double(value);
     if (old_locale) {
         setlocale(LC_ALL, old_locale);
         g_free(old_locale);
     }
     MUTEX_UNLOCK();
     return internal_value;
 }
 
 // Atom service functionen
 
 gboolean atom_strv_equal0(gchar **a, gchar **b) {
     gboolean ret = FALSE;
     guint    n;
     if (a == NULL && b == NULL) {
         ret = TRUE;
         goto out;
     }
     if (a == NULL || b == NULL) goto out;
     if (g_strv_length(a) != g_strv_length(b)) goto out;
     for (n = 0; a[n] != NULL; n++)
         if (g_strcmp0(a[n], b[n]) != 0) goto out;
     ret = TRUE;
 out:
     return ret;
 }
 /*
  *
  For GLib version at 2.30
 
 static gboolean
 _g_variant_equal0 (GVariant *a, GVariant *b)
 {
   gboolean ret = FALSE;
   if (a == NULL && b == NULL)
     {
       ret = TRUE;
       goto out;
     }
   if (a == NULL || b == NULL)
     goto out;
   ret = g_variant_equal (a, b);
 out:
   return ret;
 }
 */
 
 gboolean value_g_value_equal(const GValue *a, const GValue *b) {
     gboolean ret = FALSE;
     // TEST:g_debug("Value is equal type a=%s  type b=%s",G_VALUE_TYPE_NAME(a),G_VALUE_TYPE_NAME(b));
     // TEST:g_debug("%s == %s",value_stringify(a),value_stringify(b));
     g_return_val_if_fail(G_VALUE_TYPE(a) == G_VALUE_TYPE(b), FALSE);
     switch (G_VALUE_TYPE(a)) {
     case G_TYPE_BOOLEAN:
         ret = (g_value_get_boolean(a) == g_value_get_boolean(b));
         break;
     case G_TYPE_CHAR:
         ret = (g_value_get_schar(a) == g_value_get_schar(b));
         break;
     case G_TYPE_UCHAR:
         ret = (g_value_get_uchar(a) == g_value_get_uchar(b));
         break;
     case G_TYPE_INT:
         ret = (g_value_get_int(a) == g_value_get_int(b));
         break;
     case G_TYPE_UINT:
         ret = (g_value_get_uint(a) == g_value_get_uint(b));
         break;
     case G_TYPE_INT64:
         ret = (g_value_get_int64(a) == g_value_get_int64(b));
         break;
     case G_TYPE_UINT64:
         ret = (g_value_get_uint64(a) == g_value_get_uint64(b));
         break;
     case G_TYPE_DOUBLE:
         ret = (g_value_get_double(a) == g_value_get_double(b));
         break;
     case G_TYPE_STRING:
         ret = (g_strcmp0(g_value_get_string(a), g_value_get_string(b)) == 0);
         break;
     /*   For GLib version at 2.30
      *   case G_TYPE_VARIANT:
             ret = _g_variant_equal0 (g_value_get_variant (a), g_value_get_variant (b));
             break;
       */
     default:
         if (G_VALUE_TYPE(a) == G_TYPE_STRV) {
             ret = atom_strv_equal0(g_value_get_boxed(a), g_value_get_boxed(b));
         } else {
             g_warning("_g_value_equal() does not handle type %s", g_type_name(G_VALUE_TYPE(a)));
             g_assert(0);
         }
         break;
     }
     return ret;
 }
 
 gint value_equal(const GValue *a, const GValue *b) {
     gint ret = 0;
     // TEST:g_debug("Value is equal type a=%s  type b=%s",G_VALUE_TYPE_NAME(a),G_VALUE_TYPE_NAME(b));
     // TEST:g_debug("%s == %s",value_stringify(a),value_stringify(b));
     g_return_val_if_fail(G_VALUE_TYPE(a) == G_VALUE_TYPE(b), FALSE);
     switch (G_VALUE_TYPE(a)) {
     case G_TYPE_BOOLEAN:
         ret = (g_value_get_boolean(a) != g_value_get_boolean(b));
         break;
     case G_TYPE_CHAR:
         if ((g_value_get_schar(a) > g_value_get_schar(b)))
             ret = 1;
         else if ((g_value_get_schar(a) < g_value_get_schar(b)))
             ret = -1;
         break;
     case G_TYPE_UCHAR:
         if ((g_value_get_uchar(a) > g_value_get_uchar(b)))
             ret = 1;
         else if ((g_value_get_uchar(a) < g_value_get_uchar(b)))
             ret = -1;
         break;
     case G_TYPE_INT:
         if ((g_value_get_int(a) > g_value_get_int(b)))
             ret = 1;
         else if ((g_value_get_int(a) < g_value_get_int(b)))
             ret = -1;
         break;
     case G_TYPE_UINT:
         if ((g_value_get_uint(a) > g_value_get_uint(b)))
             ret = 1;
         else if ((g_value_get_uint(a) < g_value_get_uint(b)))
             ret = -1;
         break;
     case G_TYPE_INT64:
         if ((g_value_get_int64(a) > g_value_get_int64(b)))
             ret = 1;
         else if ((g_value_get_int64(a) < g_value_get_int64(b)))
             ret = -1;
         break;
     case G_TYPE_UINT64:
         if ((g_value_get_uint64(a) > g_value_get_uint64(b)))
             ret = 1;
         else if ((g_value_get_uint64(a) < g_value_get_uint64(b)))
             ret = -1;
         break;
     case G_TYPE_DOUBLE:
         if ((g_value_get_double(a) > g_value_get_double(b)))
             ret = 1;
         else if ((g_value_get_double(a) < g_value_get_double(b)))
             ret = -1;
         break;
     case G_TYPE_STRING:
         ret = (g_strcmp0(g_value_get_string(a), g_value_get_string(b)));
         break;
     /*   For GLib version at 2.30
      *   case G_TYPE_VARIANT:
     ret = _g_variant_equal0 (g_value_get_variant (a), g_value_get_variant (b));
     break;
      */
     default:
         if (G_VALUE_TYPE(a) == G_TYPE_STRV) {
             ret = atom_strv_equal0(g_value_get_boxed(a), g_value_get_boxed(b));
         } else {
             g_warning("_g_value_equal() does not handle type %s", g_type_name(G_VALUE_TYPE(a)));
             g_assert(0);
         }
         break;
     }
     return ret;
 }
 



 