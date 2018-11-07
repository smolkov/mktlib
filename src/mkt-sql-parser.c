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

#include "mkt-model.h"
#include "mkt-sql-parser.h"
#include "mkt-value.h"
#include <locale.h>
#include <stdlib.h>

#include <sqlite3.h>

#define GETTEXT_PACKAGE "mkt-sql-parser"
#include <glib/gi18n-lib.h>

struct _MktSqlParserPrivate {
    gchar *  name;
    gchar *  table;
    gchar *  pk_value;
    GValue * pk_def;
    GString *set_values;
    gchar *  parser_sql;
    guint    offset;
    guint    limit;

    GHashTable *value_table;
};

#define MKT_SQL_PARSER_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), MKT_TYPE_SQL_PARSER, MktSqlParserPrivate))

enum { PROP_PARSER_NULL, PROP_PARSER_NAME, PROP_PARSER_TABLE, PROP_PARSER_PK, PROP_PARSER_ADD_SET_VALUES, PROP_PARSER_SQL };

enum {
    DB_MODEL_PK_KEY     = G_TOKEN_LAST + 1,
    DB_MODEL_PK_VALUE   = G_TOKEN_LAST + 2,
    DB_MODEL_SET_VALUES = G_TOKEN_LAST + 3,
    DB_MODEL_TABLE      = G_TOKEN_LAST + 4,

};

typedef struct {
    gchar *symbol_name;
    guint  symbol_token;
} MktSqlParserSymbol;

static const MktSqlParserSymbol __DB_SYMBOLS[] = {
    {
        "pk_key", DB_MODEL_PK_KEY,
    },
    {
        "pk_value", DB_MODEL_PK_VALUE,
    },
    {"set_values", DB_MODEL_SET_VALUES},
    {
        "tablename", DB_MODEL_TABLE,
    },

    {
        NULL, 0,
    },
};

const gchar *mkt_sql_parser_rps(const gchar *pname) {
    static gchar ret[1024] = "";
    memset(ret, 0, sizeof(ret));
    strncpy(ret, pname, sizeof(ret));
    gchar *p = (gchar *)ret;
    while (*p != '\0') {
        if (*p == '-') *p = '_';
        p++;
    }
    return ret;
}

gchar *mkt_sql_parser_dup_rps(const gchar *pname) {
    gchar *ret = g_strdup(pname);
    gchar *p   = (gchar *)ret;
    while (*p != '\0') {
        if (*p == '-') *p = '_';
        p++;
    }
    return ret;
}

G_DEFINE_TYPE(MktSqlParser, mkt_sql_parser, G_TYPE_OBJECT);

static void destroy_value_calback(gpointer data) {
    if (data) mkt_value_free((GValue *)data);
}

static void destroy_key_calback(gpointer data) {
    if (data) g_free(data);
}

static GScanner *mkt_sql_parser_create_scanner() {
    GScanner *scanner                     = g_scanner_new(NULL);
    scanner->config->cset_skip_characters = "\t\n";
    scanner->config->symbol_2_token       = TRUE;
    scanner->config->scan_string_sq       = TRUE;
    scanner->config->char_2_token         = FALSE;
    const MktSqlParserSymbol *symbol_p    = __DB_SYMBOLS;
    for (; symbol_p->symbol_name != NULL; symbol_p++) {
        g_scanner_add_symbol(scanner, symbol_p->symbol_name, GINT_TO_POINTER(symbol_p->symbol_token));
    }
    return scanner;
}

static void mkt_sql_parser_init(MktSqlParser *mkt_sql_parser) {
    MktSqlParserPrivate *priv = MKT_SQL_PARSER_GET_PRIVATE(mkt_sql_parser);
    priv->name                = NULL;
    priv->pk_value            = NULL;
    priv->table               = NULL;
    priv->pk_def              = NULL;
    priv->set_values          = NULL;
    priv->parser_sql          = NULL;

    priv->value_table    = g_hash_table_new_full(g_str_hash, g_str_equal, destroy_key_calback, destroy_value_calback);
    mkt_sql_parser->priv = priv;
    /* TODO: Add initialization code here */
}

static void mkt_sql_parser_finalize(GObject *object) {
    /* TODO: Add deinitalization code here */
    // g_debug("mkt_sql_parser_finalize1");
    MktSqlParser *parser = MKT_SQL_PARSER(object);
    if (parser->priv->name) g_free(parser->priv->name);
    if (parser->priv->table) g_free(parser->priv->table);
    if (parser->priv->pk_value) g_free(parser->priv->pk_value);
    if (parser->priv->parser_sql) g_free(parser->priv->parser_sql);
    if (parser->priv->pk_def) mkt_value_free(parser->priv->pk_def);
    if (parser->priv->set_values) g_string_free(parser->priv->set_values, TRUE);
    g_hash_table_destroy(parser->priv->value_table);
    // g_debug("mkt_sql_parser_finalize end");
    G_OBJECT_CLASS(mkt_sql_parser_parent_class)->finalize(object);
}

static void mkt_parser_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
    // TEST:	g_debug("sensor_set_property %s",pspec->name);
    g_return_if_fail(MKT_IS_SQL_PARSER(object));
    MktSqlParser *parser = MKT_SQL_PARSER(object);
    switch (prop_id) {
    case PROP_PARSER_NAME:
        if (parser->priv->name) g_free(parser->priv->name);
        parser->priv->name = g_value_dup_string(value);
        break;
    case PROP_PARSER_TABLE:
        if (parser->priv->table) g_free(parser->priv->table);
        parser->priv->table = g_value_dup_string(value);
        break;
    case PROP_PARSER_PK:
        if (parser->priv->pk_value) g_free(parser->priv->pk_value);
        parser->priv->pk_value = g_value_dup_string(value);
        break;
    case PROP_PARSER_ADD_SET_VALUES:

        break;
    case PROP_PARSER_SQL:
        if (parser->priv->parser_sql) g_free(parser->priv->parser_sql);
        parser->priv->parser_sql = g_value_dup_string(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
    // TEST:	g_debug("sensor_set_property %s end",pspec->name);
}

static void mkt_parser_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
    // TEST:	g_debug("sensor_get_property %s",pspec->name);
    g_return_if_fail(MKT_IS_SQL_PARSER(object));
    MktSqlParser *parser = MKT_SQL_PARSER(object);
    switch (prop_id) {
    case PROP_PARSER_NAME:
        g_value_set_string(value, parser->priv->name);
        break;
    case PROP_PARSER_TABLE:
        g_value_set_string(value, parser->priv->table);
        break;
    case PROP_PARSER_PK:
        g_value_set_string(value, parser->priv->pk_value);
        break;
    case PROP_PARSER_ADD_SET_VALUES:
        break;
    case PROP_PARSER_SQL:
        g_value_set_string(value, parser->priv->parser_sql);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
    // TEST:	g_debug("sensor_get_property %s end",pspec->name);
}

static void mkt_sql_parser_class_init(MktSqlParserClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    // GObjectClass* parent_class = G_OBJECT_CLASS (klass);
    object_class->finalize     = mkt_sql_parser_finalize;
    object_class->set_property = mkt_parser_set_property;
    object_class->get_property = mkt_parser_get_property;

    g_type_class_add_private(klass, sizeof(MktSqlParserPrivate));

    object_class->finalize = mkt_sql_parser_finalize;

    GParamSpec *pspec;
    pspec = g_param_spec_string("parser-name", "Sql parcer prop", "Set|Get Sql parcer prop name", "", G_PARAM_READWRITE);
    g_object_class_install_property(object_class, PROP_PARSER_NAME, pspec);
    pspec = g_param_spec_string("parser-table", "Sql parcer prop table", "Set|Get Sql parcer prop table", "", G_PARAM_READWRITE);
    g_object_class_install_property(object_class, PROP_PARSER_TABLE, pspec);
    pspec = g_param_spec_string("pk-value", "Sql parcer prop", "Set|Get Sql parcer prop", "", G_PARAM_READWRITE);
    g_object_class_install_property(object_class, PROP_PARSER_PK, pspec);
    pspec = g_param_spec_string("add-set-values", "Sql parcer prop set-values", "Set|Get Sql parcer prop set-values", "", G_PARAM_READWRITE);
    g_object_class_install_property(object_class, PROP_PARSER_ADD_SET_VALUES, pspec);
    pspec = g_param_spec_string("parser-sql", "Sql parcer prop", "Set|Get Sql parcer prop", "", G_PARAM_READWRITE);
    g_object_class_install_property(object_class, PROP_PARSER_SQL, pspec);
}

static gboolean parser_scan_variable(MktSqlParser *parser, GObject *params, GString *result, GScanner *scanner) {
    gboolean    add       = FALSE;
    GValue *    value     = NULL;
    GParamSpec *pspec     = NULL;
    gchar *     value_str = NULL;

// Warnung:
//    case value '271' not in enumerated type 'GTokenType' [-Wswitch]
//    case value '272' not in enumerated type 'GTokenType' [-Wswitch]
//    ...
// Lösung:
//    #pragma GCC diagnostic
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"
    switch (g_scanner_get_next_token(scanner)) {

    case DB_MODEL_PK_KEY:
        if (parser->priv->pk_value == NULL) {
            g_scanner_error(scanner, "Parser model have not primary key - error at line:%d pos:%d tocken:%d - %s", g_scanner_cur_line(scanner), g_scanner_cur_position(scanner),
                g_scanner_cur_token(scanner), scanner->text);
        } else {
            add = TRUE;
            g_string_append_printf(result, "%s", parser->priv->pk_value);
        }
        break;
    case DB_MODEL_PK_VALUE:
        if (parser->priv->pk_def == NULL && parser->priv->pk_value == NULL) {
            g_scanner_error(scanner, "Parser model have not primary value - error at line:%d pos:%d tocken:%d - %s", g_scanner_cur_line(scanner), g_scanner_cur_position(scanner),
                g_scanner_cur_token(scanner), scanner->text);
        } else {
            if (parser->priv->pk_def) {
                value_str = mkt_value_stringify(parser->priv->pk_def);
                if (parser->priv->pk_def->g_type == G_TYPE_STRING)
                    g_string_append_printf(result, "'%s'", value_str);
                else
                    g_string_append_printf(result, "%s", value_str);
                g_free(value_str);
            } else {
                pspec = g_object_class_find_property(G_OBJECT_GET_CLASS(G_OBJECT(params)), parser->priv->pk_value);
                if (pspec != NULL) {
                    value = mkt_value_new(pspec->value_type);
                    g_object_get_property(params, pspec->name, value);
                    if (pspec->value_type == G_TYPE_STRING) {
                        g_string_append_printf(result, "'%s'", g_value_get_string(value));
                    } else {
                        value_str = mkt_value_stringify(value);
                        g_string_append_printf(result, "%s", value_str);
                        g_free(value_str);
                    }
                    mkt_value_free(value);
                    add = TRUE;
                } else
                    g_scanner_error(scanner, "Parser property %s not found error at line:%d pos:%d tocken:%d - %s", parser->priv->pk_value, g_scanner_cur_line(scanner),
                        g_scanner_cur_position(scanner), g_scanner_cur_token(scanner), scanner->text);
            }
            add = TRUE;
        }
        break;
    case DB_MODEL_TABLE:
        g_string_append_printf(result, "%s", parser->priv->table);
        add = TRUE;
        break;
    case G_TOKEN_IDENTIFIER:
        pspec = g_object_class_find_property(G_OBJECT_GET_CLASS(G_OBJECT(params)), (g_scanner_cur_value(scanner)).v_string);
        if (pspec != NULL) {
            value = mkt_value_new(pspec->value_type);
            g_object_get_property(params, pspec->name, value);
            if (g_type_is_a(pspec->value_type, MKT_TYPE_MODEL)) {
                if (g_value_get_object(value) == NULL)
                    g_string_append_printf(result, "0");
                else
                    g_string_append_printf(result, "%" G_GUINT64_FORMAT, mkt_model_ref_id(MKT_IMODEL(g_value_get_object(value))));
            } else {
                value_str     = mkt_value_stringify(value);
                char *escaped = sqlite3_mprintf("%q", (char *)value_str);
                if (pspec->value_type == G_TYPE_STRING) {

                    if (pspec->flags & MKT_MODEL_DB_TIMESTAMP) {
                        if (!strchr(escaped, ':'))
                            g_string_append_printf(result, "%s", escaped);
                        else
                            g_string_append_printf(result, "'%s'", escaped);
                    } else
                        g_string_append_printf(result, "'%s'", escaped);
                } else {
                    g_string_append_printf(result, "%s", escaped);
                }
                g_free(value_str);
                sqlite3_free(escaped);
            }
            mkt_value_free(value);
            add = TRUE;
        } else
            g_scanner_error(scanner, "Parser property %s not found error at line:%d pos:%d tocken:%d - %s", (g_scanner_cur_value(scanner)).v_string, g_scanner_cur_line(scanner),
                g_scanner_cur_position(scanner), g_scanner_cur_token(scanner), scanner->text);
        break;
    default:
        add = FALSE;
        g_scanner_error(scanner, "Parser have unknown variable - error at line:%d pos:%d tocken:%d - %s", g_scanner_cur_line(scanner), g_scanner_cur_position(scanner), g_scanner_cur_token(scanner),
            scanner->text);
        break;
    }
#pragma GCC diagnostic pop

    return add;
}

static gboolean parser_scan(MktSqlParser *parser, GObject *params, GString *result, GScanner *scanner) {

    while (g_scanner_cur_token(scanner) != G_TOKEN_EOF && g_scanner_cur_token(scanner) != ';') {
// Warnung:
//    variable 'add' set but not used [-Wunused-but-set-variable]
// Lösung:
//    add entfernen
//		gboolean add = FALSE;

// Warnung:
//    case value '36' not in enumerated type 'GTokenType' [-Wswitch]
//    case value '42' not in enumerated type 'GTokenType' [-Wswitch]
//    ...
// Lösung:
//    #pragma GCC diagnostic
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"
        char *         escaped = NULL;
        switch (g_scanner_cur_token(scanner)) {
        case '$':
            //			add = parser_scan_variable(parser,params,result,scanner);
            break;
        case G_TOKEN_IDENTIFIER:
            result = g_string_append(result, (g_scanner_cur_value(scanner)).v_identifier);
            //			if(!g_str_equal( (g_scanner_cur_value(scanner)).v_string,"CHAR"))
            //				add = TRUE;
            break;
        case '*':
            result = g_string_append_c(result, '*');
            //			add = TRUE;
            break;
        case G_TOKEN_EQUAL_SIGN:
            result = g_string_append_c(result, '=');
            //			add = TRUE;
            break;
        case '-':
            result = g_string_append_c(result, '-');
            break;
        case G_TOKEN_COMMA:
            result = g_string_append_c(result, ',');
            //			add = TRUE;
            break;
        case '(':
            result = g_string_append_c(result, '(');
            break;
        case ')':
            result = g_string_append_c(result, ')');
            //			add = TRUE;
            break;
        case '<':
            result = g_string_append_c(result, '<');
            //			add = TRUE;
            break;
        case '>':
            result = g_string_append_c(result, '>');
            //			add = TRUE;
            break;
        case G_TOKEN_INT:
            g_string_append_printf(result, "%lu", (g_scanner_cur_value(scanner)).v_int);
            //			if ( g_scanner_peek_next_token(scanner) != ')')add = TRUE;
            g_scanner_peek_next_token(scanner);
            break;
        case G_TOKEN_FLOAT:
            g_string_append_printf(result, "%f", (g_scanner_cur_value(scanner)).v_float);
            //			if ( g_scanner_peek_next_token(scanner) != ')')add = TRUE;
            g_scanner_peek_next_token(scanner);
            break;
        case G_TOKEN_NONE:
            break;
        case G_TOKEN_STRING:
            escaped = sqlite3_mprintf("%q", (char *)(g_scanner_cur_value(scanner)).v_string);
            g_string_append_printf(result, "'%s'", escaped);
            sqlite3_free(escaped);

            //			add = TRUE;
            break;
        case G_TOKEN_CHAR:
            if (((gchar)((g_scanner_cur_value(scanner)).v_char)) == '$') {
                parser_scan_variable(parser, params, result, scanner);
            } else {
                result = g_string_append_c(result, ((gchar)((g_scanner_cur_value(scanner)).v_char)));
            }
            //			add = FALSE;
            break;
        default:
            g_scanner_error(
                scanner, "Parser unknown syntax- error at line:%d pos:%d tocken:%d - %s", g_scanner_cur_line(scanner), g_scanner_cur_position(scanner), g_scanner_cur_token(scanner), scanner->text);
            break;
        }
#pragma GCC diagnostic pop

        if (scanner->parse_errors > 0) break;
        g_scanner_get_next_token(scanner);
        // if(g_scanner_cur_token(scanner)!=G_TOKEN_EOF && g_scanner_cur_token(scanner)!=';')
        // if(add) result = g_string_append(result," ");
    }
    return scanner->parse_errors == 0;
}

gchar *mkt_sql_parser_extended(MktSqlParser *parser, const gchar *parcer_str, GObject *params) {
    g_return_val_if_fail(parser != NULL, NULL);
    g_return_val_if_fail(MKT_IS_SQL_PARSER(parser), NULL);
    if (parcer_str == NULL) {
        g_warning("parser %s sql string not exist", parser->priv->name);
        return NULL;
    }
    gchar *old_locale = g_strdup(setlocale(LC_ALL, NULL));
    setlocale(LC_ALL, "en_US.UTF-8");
    GScanner *scanner = mkt_sql_parser_create_scanner();
    g_scanner_input_text(scanner, parcer_str, g_utf8_strlen(parcer_str, 4096));
    scanner->parse_errors = 0;
    GString *result       = g_string_new("");
    parser_scan(parser, params, result, scanner);
    gchar *SQL = NULL;
    SQL        = g_strdup(result->str);
    g_string_free(result, TRUE);
    if (old_locale) {
        setlocale(LC_ALL, old_locale);
        g_free(old_locale);
    }
    if (scanner->parse_errors > 0) {
        g_warning("Query parser string %s failed", parcer_str);
        g_free(SQL);
        // exit(1);
        return NULL;
    }
    g_scanner_destroy(scanner);
    return SQL;
}

gchar *mkt_sql_parser_extended_default(MktSqlParser *parser, GObject *params) { return mkt_sql_parser_extended(parser, mkt_sql_parser_get_default_string(parser), params); }

void mkt_sql_parser_parce_string(MktSqlParser *parser, const gchar *sql) {
    g_return_if_fail(sql != NULL);
    g_return_if_fail(MKT_IS_SQL_PARSER(parser));
    g_object_set(parser, "parser-sql", sql, NULL);
}

const gchar *mkt_sql_parser_get_default_string(MktSqlParser *parser) {
    g_return_val_if_fail(parser != NULL, NULL);
    g_return_val_if_fail(MKT_IS_SQL_PARSER(parser), NULL);
    return parser->priv->parser_sql;
}

void mkt_sql_parser_set_default_pk_value(MktSqlParser *parser, GValue *value) {
    g_return_if_fail(value != NULL);
    g_return_if_fail(MKT_IS_SQL_PARSER(parser));
    if (parser->priv->pk_def != NULL) mkt_value_free(parser->priv->pk_def);
    parser->priv->pk_def = mkt_value_new(value->g_type);
    g_value_copy(value, parser->priv->pk_def);
}

// Parser init local values

void mkt_parser_add_set_property(MktSqlParser *parser, const gchar *pname) {
    g_return_if_fail(parser != NULL);
    g_return_if_fail(pname != NULL);
    g_return_if_fail(MKT_IS_SQL_PARSER(parser));

    if (parser->priv->set_values == NULL)
        parser->priv->set_values = g_string_new("");
    else if (g_strrstr(parser->priv->set_values->str, mkt_sql_parser_rps(pname))) {
        g_warning("SQL parser target $set_values:(%s) has attribut key %s", parser->priv->set_values->str, pname);
        return;
    }

    g_string_append_printf(parser->priv->set_values, "%s'%s'=$%s", parser->priv->set_values->len > 1 ? "," : "", mkt_sql_parser_rps(pname), mkt_sql_parser_rps(pname));
}

/*
Gerhard: warning: 'mkt_parser_clean_set_data' defined but not used [-Wunused-function]
static void mkt_parser_clean_set_data(MktSqlParser *parser) {
    g_return_if_fail(parser != NULL);
    g_return_if_fail(MKT_IS_SQL_PARSER(parser));
    if (parser->priv->set_values != NULL) g_string_free(parser->priv->set_values, TRUE);
    parser->priv->set_values = NULL;
}
*/

const gchar *mkt_sql_parser_get_pk(MktSqlParser *parser) {
    g_return_val_if_fail(parser != NULL, NULL);
    g_return_val_if_fail(MKT_IS_SQL_PARSER(parser), NULL);
    return parser->priv->pk_value;
}

gboolean mkt_sql_parser_add_value(MktSqlParser *parser, const gchar *id, const gchar *val) {
    g_return_val_if_fail(parser != NULL, FALSE);
    g_return_val_if_fail(MKT_IS_SQL_PARSER(parser), FALSE);
    if (NULL == g_hash_table_lookup(parser->priv->value_table, (gconstpointer)id)) {
        GValue *value = mkt_value_new(G_TYPE_STRING);
        g_value_set_string(value, val);
        g_hash_table_insert(parser->priv->value_table, (gpointer)g_strdup(id), value);
    }

    // Warnung:
    //    control reaches end of non-void function [-Wreturn-type]
    // Lösung:
    //    return einbauen.
    return TRUE;
}
