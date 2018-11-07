/*
 * mkt-value.h
 *
 *  Created on: 02.05.2013
 *      Author: sascha
 */

#ifndef VALUE_H_
#define VALUE_H_

#include <glib-object.h>
#include <glib.h>

#define value_isa(value, type) (G_VALUE_HOLDS(value, type))

GValue *value_new(GType type);
void value_free(GValue *value);

gboolean set_gvalue_from_string(GValue *value, const gchar *as_string);
gchar *value_stringify(const GValue *value);
const gchar *value_stringify_static(const GValue *value);
const gchar *value_stringify_double(gdouble value);
const gchar *value_stringify_double_enUS(gdouble value);
gchar *value_dup_double(gdouble value);
gchar *value_dup_double_enUS(gdouble value);

#endif /* VALUE_H_ */
