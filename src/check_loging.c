#include <glib.h>
#include <glib/gstdio.h>
#include "mkt-sqlite.h"
#include "mkt-log.h"

static gchar *messages[8][2] = {
    {"message0", "-"}, {"message1", "-"}, {"message2", "-"}, {"message3", "-"}, {"message4", "-"}, {"message5", "-"}, {"message6", "-"}, {"message7", "-"},

};

void done_callback(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    GError *error = NULL;
    GSList *list  = mkt_sqlite_select_objects_finish(res, &error);
    g_assert_null(error);
    g_assert_cmpuint(g_slist_length(list), ==,7);
    g_slist_free_full(list, g_object_unref);
    GMainLoop *loop = (GMainLoop *)user_data;
    g_main_loop_quit(loop);
}
// static GMainLoop *testLoop = NULL;
static void test_log_messages(void) {
    g_message("start  log check");
    MktSqlite *conn = mkt_log_connection();
    g_message("delete old table");
    mkt_sqlite_delete_table(conn);
    g_message("check state error");
    mkt_log_message(MKT_LOG_STATE_ERRORS, "%s", messages[0][0]);
    g_usleep(50000);
    g_message("check state message");
    mkt_log_error_message("%s", messages[1][0]);
    g_usleep(50000);
    mkt_log_message_note(MKT_LOG_STATE_PARAMETER_CHANGE, messages[2][1], "%s", messages[2][0]);
    g_usleep(50000);
    mkt_log_status_message("%s", messages[3][0]);
    g_usleep(50000);
    mkt_log_system_message("%s", messages[4][0]);
    g_usleep(50000);
    mkt_log_error_message("%s", messages[5][0]);
    g_usleep(50000);
    mkt_log_error_message_with_note(messages[6][1], "%s", messages[6][0]);
    g_usleep(50000);
    mkt_log_warning_message("%s", messages[7][0]);

    g_message("waite write all message");
    g_usleep(500000);
    g_assert_true(mkt_sqlite_open(conn));
    GSList *list = mkt_sqlite_select_objects(conn, ";");
    g_assert_cmpint(8, ==, g_slist_length(list));
    g_slist_free_full(list, g_object_unref);
    GSList *mesages = mkt_log_select(";");
    g_assert_cmpint(8, ==, g_slist_length(mesages));
    g_slist_free_full(mesages, g_object_unref);
    g_object_unref(conn);

    GSList *check_messages = mkt_log_select(";");
    GSList *l              = NULL;
    gint    i              = 0;
    for (l = check_messages; l != NULL; l = l->next) {
        MktLog *log = MKT_LOG(l->data);
        g_assert_cmpstr(mkt_log_get_message(log), ==, messages[i][0]);
        g_assert_cmpstr(mkt_log_get_note(log), ==, messages[i][1]);
        i++;
    }
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    mkt_log_async(NULL, done_callback, loop, "ORDER BY changed DESC LIMIT %d OFFSET %d", 10, 1);
    g_main_loop_run(loop);
    g_main_loop_unref(loop);
}

gint main(gint argc, gchar *argv[]) {
    // g_setenv("G_MESSAGES_DEBUG", "all", TRUE);
    g_test_init(&argc, &argv, NULL);
    g_test_add_func("/messages/log", test_log_messages);
    // g_test_add_func("/messages/log", test_err_message);

    return g_test_run();
}
