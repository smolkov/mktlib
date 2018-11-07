/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mktlibrary
 * Copyright (C) sascha 2013 <sascha@sascha-VirtualBox>
 *
   mktlibrary is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mktlibrary is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mkt-boolexp-parser.h"
#include "mkt-error-message.h"
#include "mkt-limit-message.h"
#include "mkt-model.h"
#include "mkt-status-model.h"
#include "mkt-boolexp-ind.h"
#include "mkt-value.h"
#include <locale.h>
#include <stdlib.h>

#include "../config.h"
#include <glib/gi18n-lib.h>

typedef enum Op {
	op_none,
	op_not,
	op_and,
	op_or,
} Op;




struct _MktBoolexpParserPrivate {
	GScanner *scanner;
	guint intern;
	gint result;
	gchar *   last_error;
	GSList *  errors;
	GSList *  limits;
	GSList *  status;
	gboolean isCritical;
	gboolean isWarning;
};

#define MKT_BOOEXP_PARSER_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), MKT_TYPE_BOOEXP_PARSER, MktBoolexpParserPrivate))

enum { PROP_PARSER_NULL, PROP_PARSER_NAME, PROP_PARSER_TABLE, PROP_PARSER_PK, PROP_PARSER_ADD_SET_VALUES, PROP_PARSER_BOOEXP };

G_DEFINE_TYPE(MktBoolexpParser, mkt_boolexp_parser, G_TYPE_OBJECT);

static const gchar *critical = "FAIL";
static const gchar *warning  = "WARN";


static void mkt_boolexp_parser_init(MktBoolexpParser *mkt_boolexp_parser) {
	MktBoolexpParserPrivate *priv                = MKT_BOOEXP_PARSER_GET_PRIVATE(mkt_boolexp_parser);
	mkt_boolexp_parser->priv = priv;
	priv->scanner                                = g_scanner_new(NULL);
	priv->scanner->config->cset_skip_characters  = "        \t\n";
	priv->scanner->config->scan_symbols          = TRUE;
	priv->scanner->config->symbol_2_token        = TRUE;
	priv->scanner->config->scan_string_sq        = TRUE;
	priv->scanner->config->scan_identifier_1char = TRUE;

	priv->errors = mkt_model_select(MKT_TYPE_ERROR_MESSAGE, "select * from $tablename");
	priv->limits = mkt_model_select(MKT_TYPE_LIMIT_MESSAGE, "select * from $tablename");
	priv->status = mkt_model_select(MKT_TYPE_STATUS_MODEL, "select * from $tablename");
	g_scanner_scope_add_symbol(priv->scanner, 1, critical, mkt_fail_new());
	g_scanner_scope_add_symbol(priv->scanner, 1, warning, mkt_warn_new());
	GSList *l = NULL;
	for (l = priv->errors; l != NULL; l = l->next) {
		gchar *errorsymbol = g_strdup_printf("E%d", mkt_error_number(MKT_ERROR(l->data)));
		g_scanner_scope_add_symbol(priv->scanner, 1, errorsymbol, l->data);
		g_free(errorsymbol);
		if (mkt_error_pending(MKT_ERROR(l->data))) {
			if (mkt_error_type(MKT_ERROR(l->data)) == MKT_ERROR_CRITICAL)
				mkt_boolexp_parser->priv->isCritical = TRUE;
			else if (mkt_error_type(MKT_ERROR(l->data)) == MKT_ERROR_WARNING)
				mkt_boolexp_parser->priv->isWarning = TRUE;
		}
	}

	l = NULL;
	for (l = priv->limits; l != NULL; l = l->next) {
		gchar *limitsymbol = g_strdup_printf("%smin", mkt_limit_name(MKT_LIMIT(l->data)));
		g_scanner_scope_add_symbol(priv->scanner, 1, limitsymbol, l->data);
		g_free(limitsymbol);
		limitsymbol = g_strdup_printf("%smax", mkt_limit_name(MKT_LIMIT(l->data)));
		g_scanner_scope_add_symbol(priv->scanner, 1, limitsymbol, l->data);
		g_free(limitsymbol);
	}
	l = NULL;
	for (l = priv->status; l != NULL; l = l->next) {
		g_scanner_scope_add_symbol(priv->scanner, 1, mkt_status_signification(MKT_STATUS(l->data)), l->data);
	}

	/* TODO: Add initialization code here */
}

static void mkt_boolexp_parser_finalize(GObject *object) {
	/* TODO: Add deinitalization code here */
	// g_debug("mkt_boolexp_parser_finalize1");
	MktBoolexpParser *parser = MKT_BOOEXP_PARSER(object);
	if (parser->priv->scanner) g_scanner_destroy(parser->priv->scanner);
	if (parser->priv->errors) g_slist_free_full(parser->priv->errors, g_object_unref);
	if (parser->priv->limits) g_slist_free_full(parser->priv->limits, g_object_unref);
	if (parser->priv->status) g_slist_free_full(parser->priv->status, g_object_unref);
	if (parser->priv->last_error) g_free(parser->priv->last_error);
	// g_debug("mkt_boolexp_parser_finalize end");
	G_OBJECT_CLASS(mkt_boolexp_parser_parent_class)->finalize(object);
}

static void mkt_parser_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
	// TEST:	g_debug("sensor_set_property %s",pspec->name);
	g_return_if_fail(MKT_IS_BOOEXP_PARSER(object));
	// MktBoolexpParser* parser = MKT_BOOEXP_PARSER(object);
	switch (prop_id) {

	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
		break;
	}
	// TEST:	g_debug("sensor_set_property %s end",pspec->name);
}

static void mkt_parser_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
	// TEST:	g_debug("sensor_get_property %s",pspec->name);
	g_return_if_fail(MKT_IS_BOOEXP_PARSER(object));
	// MktBoolexpParser* parser = MKT_BOOEXP_PARSER(object);
	switch (prop_id) {
	/*case PROP_PARSER_NAME:
	        g_value_set_string(value,parser->priv->name);
	        break;
	   case PROP_PARSER_TABLE:
	        g_value_set_string(value,parser->priv->table);
	        break;
	   case PROP_PARSER_PK:
	        g_value_set_string(value,parser->priv->pk_value);
	        break;
	   case PROP_PARSER_ADD_SET_VALUES:
	        break;
	   case PROP_PARSER_BOOEXP:
	        g_value_set_string(value,parser->priv->parser_boolexp);
	        break;*/
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
		break;
	}
	// TEST:	g_debug("sensor_get_property %s end",pspec->name);
}

static void mkt_boolexp_parser_class_init(MktBoolexpParserClass *klass) {
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	// GObjectClass* parent_class = G_OBJECT_CLASS (klass);
	object_class->finalize     = mkt_boolexp_parser_finalize;
	object_class->set_property = mkt_parser_set_property;
	object_class->get_property = mkt_parser_get_property;

	g_type_class_add_private(klass, sizeof(MktBoolexpParserPrivate));

	object_class->finalize = mkt_boolexp_parser_finalize;
}

static gboolean parser_next(MktBoolexpParser *parser);

static gboolean parser_check_identifier(MktBoolexpParser *parser, gchar *identifier) {
	// g_debug("FOUND IND.%s",identifier);
	

	gpointer data = g_scanner_scope_lookup_symbol(parser->priv->scanner, 1, identifier);
	if (data == NULL) {
		g_scanner_error(parser->priv->scanner, "unknown identifier '%s' - error at line:%d pos:%d token:%d - '%s'", identifier, g_scanner_cur_line(parser->priv->scanner),
		                g_scanner_cur_position(parser->priv->scanner), g_scanner_cur_token(parser->priv->scanner), parser->priv->scanner->text);
		parser->priv->last_error = g_strdup_printf("unknown identifier '%s' - error at line:%d pos:%d token:%d - '%s'", identifier, g_scanner_cur_line(parser->priv->scanner),
		                                           g_scanner_cur_position(parser->priv->scanner), g_scanner_cur_token(parser->priv->scanner), parser->priv->scanner->text);
		return FALSE;
	}
	if ( MKT_IS_FAIL(data)) {
		return parser->priv->isCritical;
	}
	if (MKT_IS_WARN(data)) {
		return parser->priv->isWarning;
	}

	if (MKT_IS_ERROR(data)) {
		return mkt_error_pending(MKT_ERROR(data));
	} else if (MKT_IS_LIMIT(data)) {
		if (g_str_has_suffix(identifier, "min")) {
			return mkt_limit_activated(MKT_LIMIT(data)) && mkt_limit_pending(MKT_LIMIT(data)) == -1;
		} else if (g_str_has_suffix(identifier, "max")) {
			return mkt_limit_activated(MKT_LIMIT(data)) && mkt_limit_pending(MKT_LIMIT(data)) == 1;
		} else {
			g_scanner_error(parser->priv->scanner, "unknown limit identifier '%s' - error at line:%d pos:%d token:%d - '%s'", identifier, g_scanner_cur_line(parser->priv->scanner),
			                g_scanner_cur_position(parser->priv->scanner), g_scanner_cur_token(parser->priv->scanner), parser->priv->scanner->text);
			parser->priv->last_error = g_strdup_printf("unknown limit identifier '%s' - error at line:%d pos:%d token:%d - '%s'", identifier, g_scanner_cur_line(parser->priv->scanner),
			                                           g_scanner_cur_position(parser->priv->scanner), g_scanner_cur_token(parser->priv->scanner), parser->priv->scanner->text);
		}
	} else if (MKT_IS_STATUS(data)) {
		return mkt_status_is_active(MKT_STATUS(data));
	}
	g_scanner_error(parser->priv->scanner, "unknown identifier '%s' - error at line:%d pos:%d token:%d - '%s'", identifier, g_scanner_cur_line(parser->priv->scanner),
	                g_scanner_cur_position(parser->priv->scanner), g_scanner_cur_token(parser->priv->scanner), parser->priv->scanner->text);
	parser->priv->last_error = g_strdup_printf("unknown identifier '%s' - error at line:%d pos:%d token:%d - '%s'", identifier, g_scanner_cur_line(parser->priv->scanner),
	                                           g_scanner_cur_position(parser->priv->scanner), g_scanner_cur_token(parser->priv->scanner), parser->priv->scanner->text);
	return FALSE;
}

static gboolean parser_scan(MktBoolexpParser *parser, guint end_token);

gboolean parser_next(MktBoolexpParser *parser) {
	parser->priv->intern++;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"
	if (parser->priv->scanner->parse_errors > 0) return FALSE;
	gboolean result = FALSE;
	GTokenValue value  = g_scanner_cur_value(parser->priv->scanner);
	// g_debug(" parser_next token:%d SYMBOL:%d value:|%s|
	// text=|%s|",g_scanner_cur_token(parser->priv->scanner),G_TOKEN_SYMBOL,value.v_identifier,parser->priv->scanner->text);

	switch (g_scanner_cur_token(parser->priv->scanner)) {
	case G_TOKEN_IDENTIFIER:
		result = parser_check_identifier(parser, value.v_identifier);
		g_scanner_get_next_token(parser->priv->scanner);
		return result;
	case '!':
    g_scanner_get_next_token(parser->priv->scanner);
		return !parser_next(parser);
	case G_TOKEN_LEFT_PAREN:
		result = parser_scan(parser, G_TOKEN_RIGHT_PAREN);
		g_scanner_get_next_token(parser->priv->scanner);
		return result;
	default:
		g_scanner_error(parser->priv->scanner, "unknown symbol - error at line:%d pos:%d token:%d - near '%s'", g_scanner_cur_line(parser->priv->scanner), g_scanner_cur_position(parser->priv->scanner),
		                g_scanner_cur_token(parser->priv->scanner), parser->priv->scanner->text);
		parser->priv->last_error = g_strdup_printf("unknown symbol - error at line:%d pos:%d token:%d - near '%s'", g_scanner_cur_line(parser->priv->scanner), g_scanner_cur_position(parser->priv->scanner),
		                                           g_scanner_cur_token(parser->priv->scanner), parser->priv->scanner->text);
		break;
	}
	if (parser->priv->scanner->parse_errors > 0) return FALSE;

#pragma GCC diagnostic pop
	return FALSE;
}
gboolean parser_scan(MktBoolexpParser *parser, guint end_token) {
	parser->priv->intern++;
	gboolean result = FALSE;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"
	g_scanner_get_next_token(parser->priv->scanner);
	result = parser_next(parser);
  gboolean next_result = FALSE;
	while (g_scanner_cur_token(parser->priv->scanner) != end_token) {
		if (g_scanner_cur_token(parser->priv->scanner) == G_TOKEN_EOF) {
			g_scanner_error(parser->priv->scanner, "p3 expected '%c' before token G_TOKEN_EOF", end_token);
			parser->priv->last_error = g_strdup_printf("expected '%c' before token G_TOKEN_EOF", end_token);
			return FALSE;
		}
		switch (g_scanner_cur_token(parser->priv->scanner)) {
		case '&':
			g_scanner_get_next_token(parser->priv->scanner);
      next_result =  parser_next(parser);
			result = result && next_result;
			break;
		case '|':
			g_scanner_get_next_token(parser->priv->scanner);
      next_result = parser_next(parser);
			result = result || next_result;
			break;
		default:
			g_scanner_error(parser->priv->scanner, "p1 wrong format  - error at line:%d pos:%d token:%d - '%s'", g_scanner_cur_line(parser->priv->scanner), g_scanner_cur_position(parser->priv->scanner),
			                g_scanner_cur_token(parser->priv->scanner), parser->priv->scanner->text);
			parser->priv->last_error = g_strdup_printf("p1 wrong format (&-|) - error at line:%d pos:%d token:%d - '%s'", g_scanner_cur_line(parser->priv->scanner),
			                                           g_scanner_cur_position(parser->priv->scanner), g_scanner_cur_token(parser->priv->scanner), parser->priv->scanner->text);
			return FALSE;
		}
		if (parser->priv->scanner->parse_errors > 0) return FALSE;
	}
#pragma GCC diagnostic pop
	// g_debug("RESURN SCANN %d",result);
	return result;
}

gint mkt_boolexp_parser_check(MktBoolexpParser *parser, const gchar *format, ...) {
	gchar *old_locale = g_strdup(setlocale(LC_ALL, NULL));
	setlocale(LC_ALL, "en_US.UTF-8");
	va_list args;
	gchar * check_str;
	va_start(args, format);
	check_str = g_strdup_vprintf(format, args);
	va_end(args);
	if (parser->priv->last_error != NULL) g_free(parser->priv->last_error);
	gint result = 0;
	// g_debug("Check: %s",check_str);
	if (g_utf8_strlen(check_str, 256) < 1) {
		g_free(check_str);
		if (old_locale) {
			setlocale(LC_ALL, old_locale);
			g_free(old_locale);
		}
		return 0;
	}
	g_scanner_input_text(parser->priv->scanner, check_str, g_utf8_strlen(check_str, 4096));
	parser->priv->intern = 0;
	parser->priv->result = 5;
	result               = parser_scan(parser, G_TOKEN_EOF);
	if (old_locale) {
		setlocale(LC_ALL, old_locale);
		g_free(old_locale);
	}
	g_free(check_str);
	if (parser->priv->scanner->parse_errors > 0) {
		g_warning("Syntax error %s", parser->priv->last_error);
		return -1;
	}
	return result;
}

MktBoolexpParser *mkt_boolexp_parser_new() {
	MktBoolexpParser *parcer = MKT_BOOEXP_PARSER(g_object_new(MKT_TYPE_BOOEXP_PARSER, NULL));
	return parcer;
}

const gchar *mkt_boolexp_parser_last_error(MktBoolexpParser *parser) {
	g_return_val_if_fail(parser != NULL, NULL);
	g_return_val_if_fail(MKT_IS_BOOEXP_PARSER(parser), NULL);
	return parser->priv->last_error;
}
