/*
 * libmkt.c
 *
 *  Created on: 05.09.2014
 *      Author: doseus
 */

#include "mktlib.h"
#include "mkt-SQLite-connection.h"
#include <glib.h>
#include <glib/gstdio.h>

#include "../config.h"
#include <glib/gi18n-lib.h>

#include <fcntl.h>

#define DB_DIR "/usr/share/tera/db"

void mkt_library_init()
{
  GType init = G_TYPE_NONE;
  init = MKT_TYPE_LIMIT_MESSAGE;
  // setlocale(LC_NUMERIC, "en_US.UTF-8");

  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_STATUS_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_SENSOR_DATA;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_PARAM_OBJECT;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_AXIS_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_INJECTION_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_STREAM_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_MEASUREMENT_DATA;
  // g_debug("initialize model type %s", g_type_name(MKT_TYPE_MEASUREMENT));
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_CHANNEL_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_ANALOG_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_VESSEL_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_AXIS_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_CATEGORY_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_STATISTIC_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = ULTIMATE_TYPE_MESSPARM_OBJECT;
  g_debug("initialize model type %s", g_type_name(init));
  init = ULTIMATE_TYPE_POSPARM_OBJECT;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_USER_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_POINT_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_CAL_POINT_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_CAL_DATA_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_CALIBRATION_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_PROCESS_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_PARAMDOUBLE_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_PARAMINT32_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_PARAMUINT32_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_PARAMBOOLEAN_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
}

void mkt_library_database_init()
{
  if (!g_file_test(DB_DIR, G_FILE_TEST_IS_DIR))
  {
    g_mkdir_with_parents(DB_DIR, 0777);
    return;
  }
  else
  {
    g_chmod(DB_DIR, 0777);
    GDir *dir = g_dir_open(DB_DIR,0,NULL);
    const gchar *name = NULL;
    while ((name = g_dir_read_name(dir)))
    {
      gchar *dbfile = g_build_path("/", DB_DIR, name, NULL);
      g_chmod(dbfile, 0777);
      g_free(dbfile);
    }
    g_dir_close(dir);
  }
}

void mkt_library_sync()
{
  GType init = G_TYPE_NONE;
  init = MKT_TYPE_LIMIT_MESSAGE;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_STATUS_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_SENSOR_DATA;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_PARAM_OBJECT;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_AXIS_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_INJECTION_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_STREAM_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_MEASUREMENT_DATA;
  // g_debug("initialize model type %s", g_type_name(MKT_TYPE_MEASUREMENT));
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_CHANNEL_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_ANALOG_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_VESSEL_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_AXIS_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_CATEGORY_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_STATISTIC_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = ULTIMATE_TYPE_MESSPARM_OBJECT;
  g_debug("initialize model type %s", g_type_name(init));
  init = ULTIMATE_TYPE_POSPARM_OBJECT;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_USER_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_POINT_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_CAL_POINT_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_CAL_DATA_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_CALIBRATION_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_PROCESS_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_PARAMDOUBLE_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_PARAMINT32_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_PARAMUINT32_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
  init = MKT_TYPE_PARAMBOOLEAN_MODEL;
  g_debug("initialize model type %s", g_type_name(init));
}

void mkt_library_autocheck_sync()
{
  // Check log data base.
  g_debug("database autocheck run..");
  MktSqlite *conn = mkt_log_connection();
  mkt_sqlite_check(conn);
  mkt_sqlite_create_table(conn);
  g_object_unref(conn);
  g_debug("sensor data check");
  // Check sensor database
  mkt_sensor_data_check();

  g_debug("default data.. ");
  MktConnection *connection = mkt_connection_default();
  mkt_sqlite_connection_check(MKT_SQLITE_CONNECTION(connection));
  g_object_unref(connection);

  g_debug("error database... ");
  connection = mkt_connection_errors();
  mkt_sqlite_connection_check(MKT_SQLITE_CONNECTION(connection));
  g_object_unref(connection);

  g_debug("parameter database... ");
  connection = mkt_connection_parameter();
  mkt_sqlite_connection_check(MKT_SQLITE_CONNECTION(connection));
  g_object_unref(connection);

  g_debug("connection special... ");
  connection = mkt_connection_special();
  mkt_sqlite_connection_check(MKT_SQLITE_CONNECTION(connection));
  g_object_unref(connection);

  g_debug("connection measurement... ");
  connection = mkt_connection_measurement();
  mkt_sqlite_connection_check(MKT_SQLITE_CONNECTION(connection));
  g_object_unref(connection);

  g_debug("calibration database... ");
  connection = mkt_connection_calibration();
  mkt_sqlite_connection_check(MKT_SQLITE_CONNECTION(connection));
  g_object_unref(connection);

  g_debug("process database... ");
  connection = mkt_connection_process();
  mkt_sqlite_connection_check(MKT_SQLITE_CONNECTION(connection));
  g_object_unref(connection);
}

static void autocheckThread(GTask *task, gpointer source_object,
                            gpointer task_data, GCancellable *cancellable)
{
  if (g_task_return_error_if_cancelled(task))
  {
    return;
  }
  mkt_library_autocheck_sync();
  if (g_task_return_error_if_cancelled(task))
  {
    return;
  }
  g_task_return_boolean(task, TRUE);
}
void mkt_library_autocheck(GCancellable *cancel, GAsyncReadyCallback callback,
                           gpointer user_data)
{
  GTask *task = g_task_new(NULL, cancel, callback, user_data);
  g_task_run_in_thread(task, autocheckThread);
  g_object_unref(task);
}
