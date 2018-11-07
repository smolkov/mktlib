/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mkt-sensor-data.c
 * Copyright (C) sascha 2013 <sascha@sascha-VirtualBox>
 *
mkt-sensor-data.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mkt-sensor-data.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MKT_SENSOR_DATA_H_
#define _MKT_SENSOR_DATA_H_

#include "mkt-sqlite.h"

G_BEGIN_DECLS

#define MKT_TYPE_SENSOR_DATA (mkt_sensor_data_get_type())
#define MKT_SENSOR_DATA(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), MKT_TYPE_SENSOR_DATA, MktSensorData))
#define MKT_SENSOR_DATA_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), MKT_TYPE_SENSOR_DATA, MktSensorDataClass))
#define MKT_IS_SENSOR_DATA(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), MKT_TYPE_SENSOR_DATA))
#define MKT_IS_SENSOR_DATA_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), MKT_TYPE_SENSOR_DATA))
#define MKT_SENSOR_DATA_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), MKT_TYPE_SENSOR_DATA, MktSensorDataClass))

typedef struct _MktSensorDataClass   MktSensorDataClass;
typedef struct _MktSensorData        MktSensorData;
typedef struct _MktSensorDataPrivate MktSensorDataPrivate;

struct _MktSensorDataClass {
    GObjectClass parent_class;
};

struct _MktSensorData {
    GObject               parent_instance;
    MktSensorDataPrivate *priv;
};

GType mkt_sensor_data_get_type(void) G_GNUC_CONST;

gchar *        mkt_sensor_data_dir();
MktSqlite *    mkt_sensor_data_conn(guint64 channel);
MktSqlite *    mkt_sensor_data_conn_history(guint64 channel);
MktSensorData *mkt_sensor_data_new(gdouble value, guint trigger, guint state);
gdouble        mkt_sensor_data_changed(MktSensorData *data);
gdouble        mkt_sensor_data_value(MktSensorData *data);
guint          mkt_sensor_data_trigger(MktSensorData *data);
guint          mkt_sensor_data_state(MktSensorData *data);
void           mkt_sensor_data_append(guint64 channel, GSList *datas, GCancellable *cancel, GAsyncReadyCallback callback, gpointer user_data);
gboolean       mkt_sensor_data_append_sync(guint64 channel, GSList *datas);
void           mkt_sensor_data_append_history(guint64 channel, GSList *datas, GCancellable *cancel, GAsyncReadyCallback callback, gpointer user_data);
void           mkt_sensor_data_clean(MktSqlite *conn);
void           mkt_sensor_data_select(guint64 channel, GCancellable *cancel, GAsyncReadyCallback callback, gpointer user_data);
void           mkt_sensor_data_select_offset(guint64 channel, guint offset, GCancellable *cancel, GAsyncReadyCallback callback, gpointer user_data);
void           mkt_sensor_data_select_trigger(guint64 channel, guint trigger, GCancellable *cancel, GAsyncReadyCallback callback, gpointer user_data);
void           mkt_sensor_data_clean_history(guint64 channel, guint records, GCancellable *cancel, GAsyncReadyCallback callback, gpointer user_data);
void           mkt_sensor_data_check();
void           mkt_sensor_data_all_clean();

void           mkt_sensor_append_data(guint64 channel, GSList *datas, GCancellable *cancel, GAsyncReadyCallback callback, gpointer user_data);

G_END_DECLS

#endif /* _MKT_SENSOR_DATA_H_ */
