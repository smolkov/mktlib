#include <glib.h>
#include <glib/gstdio.h>
#include "mkt-sqlite.h"
#include "mkt-sensor-data.h"
#include <stdlib.h>

static GMainLoop *testLoop = NULL;

void done_select_offset2_callback(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    GError *error = NULL;
    GSList *list  = mkt_sqlite_select_objects_finish(res, &error);
    g_assert_null(error);
    g_assert_cmpuint(g_slist_length(list), ==, 3000);
    g_slist_free_full(list, g_object_unref);
    GMainLoop *loop = (GMainLoop *)user_data;
    g_main_loop_quit(loop);
}

void done_select_offset_callback(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    GError *error = NULL;
    GSList *list  = mkt_sqlite_select_objects_finish(res, &error);
    g_assert_null(error);
    g_assert_cmpuint(g_slist_length(list), ==, 4000);
    g_slist_free_full(list, g_object_unref);
    mkt_sensor_data_select_offset(100, 1000, NULL, done_select_offset2_callback, user_data);
}

static void done_append_callback(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    g_assert_true(g_task_propagate_boolean(G_TASK(res), NULL));
    mkt_sensor_data_select_offset(100, 0, NULL, done_select_offset_callback, user_data);
}

static void test_sensor(guint64 channel) {
    MktSqlite *conn = mkt_sensor_data_conn(channel);
    mkt_sqlite_open(conn);
    if (!mkt_sqlite_check(conn)) {
    }
    mkt_sqlite_create_table(conn);
    GSList *l       = mkt_sqlite_select_objects(conn, "LIMIT 1;");
    guint   trigger = 0;
    if (l != NULL && l->data) {
        if (MKT_IS_SENSOR_DATA(l->data)) {
            trigger = mkt_sensor_data_trigger(MKT_SENSOR_DATA(l->data));
        }
        g_slist_free_full(l, g_object_unref);
    }

    trigger += 1;
    mkt_sensor_data_clean(conn);

    // testLoop = g_main_loop_new(NULL,FALSE);
    int     i     = 0;
    gdouble value = 0.0;
    GSList *list  = NULL;
    for (i = 0; i < 4000; i++) {
        value               = rand();
        MktSensorData *data = mkt_sensor_data_new(value, trigger, 1);
        list                = g_slist_append(list, data);
    }
    mkt_sqlite_create_table(conn);
    g_assert_true(mkt_sqlite_insert_objects(conn, list) != 0);
    GSList *datas = mkt_sqlite_select_objects(conn, ";");
    g_assert_cmpuint(g_slist_length(datas), ==, g_slist_length(list));
    g_assert_cmpuint(g_slist_length(datas), ==, 4000);

    GSList *temp = mkt_sqlite_select_objects(conn, "ORDER BY changed DESC LIMIT 1");
    g_assert_nonnull(temp);
    g_slist_free_full(temp, g_object_unref);
    mkt_sensor_data_append_history(channel, datas, NULL, NULL, NULL);
    g_usleep(500000);
    g_slist_free_full(datas, g_object_unref);
    g_slist_free_full(list, g_object_unref);
    g_object_unref(conn);
}
static void test_data_sensor(void) {
    mkt_sensor_data_all_clean();
    test_sensor(1);
    test_sensor(2);
    test_sensor(3);
    test_sensor(4);
}

void done_callback(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    GError *error = NULL;
    GSList *list  = mkt_sqlite_select_objects_finish(res, &error);
    g_assert_null(error);
    g_assert_cmpuint(g_slist_length(list), ==, 4000);
    g_slist_free_full(list, g_object_unref);
    int     i     = 0;
    gdouble value = 0.0;
    GSList *new   = NULL;
    for (i = 0; i < 4000; i++) {
        value               = rand();
        MktSensorData *data = mkt_sensor_data_new(value, 9999, 1);
        new                 = g_slist_append(new, data);
    }
    mkt_sensor_data_append(100, new, NULL, done_append_callback, user_data);
    g_slist_free_full(new, g_object_unref);
    new = NULL;
}

void test_select_data(void) {
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    // GAsyncReadyCallback
    mkt_sensor_data_select_trigger(1, 1, NULL, done_callback, loop);
    g_main_loop_run(loop);
    g_main_loop_unref(loop);
}
void test_clean_history_sensor(void) {
    MktSqlite *conn = mkt_sensor_data_conn_history(1);
    g_assert_true(mkt_sqlite_open(conn));
    mkt_sqlite_exec(conn, "delete from datas where trigger < 2;");
    GSList *list = mkt_sqlite_select_objects(conn, "where trigger < 2;");
    g_assert_null(list);
    g_object_unref(conn);
}

gint main(gint argc, gchar *argv[]) {
    // g_setenv("G_MESSAGES_DEBUG", "all", TRUE);
    g_test_init(&argc, &argv, NULL);
    g_test_add_func("/sensor/create", test_data_sensor);
    g_test_add_func("/sensor/select", test_select_data);
    // g_test_add_func("/sensor/clean", test_clean_history_sensor);

    return g_test_run();
}
