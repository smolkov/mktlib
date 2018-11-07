/*
 * libmkt.h
 *
 *  Created on: 05.09.2014
 *      Author: doseus
 */

#ifndef LIBMKT_H_
#define LIBMKT_H_

#include "market-time.h"
#include <malfunc.h>
#include <mkt-connection.h>
#include <mkt-model.h>
#include <mkt-param-object.h>
#include <mkt-utils.h>
#include <mkt-value.h>
#include <amount.h>
#include <analog.h>
#include <sensor.h>
#include <temperatur.h>
#include <axis.h>
#include <airflow.h>
#include <channel.h>
#include <device.h>
#include <dilution.h>
#include <injection.h>
#include <larintegration.h>
#include <larintegrations.h>
#include <interval.h>
#include <intervals.h>
#include <limit.h>
#include <parameters.h>
#include <posparm.h>
#include <redis.h>
#include <relay.h>
#include <statistic.h>
#include <statistics.h>
#include <stream.h>
#include <ultradevice.h>
#include <ultrastream.h>
#include <larflags.h>
#include <value.h>
#include <humidity.h>
#include <pressure.h>
#include <vessel.h>
#include <ultrameas.h>
#include <ultrameasurement.h>
#include <ultraqueries.h>
#include <larresult.h>
#include <larsqlite.h>


#include <mkt-paramboolean-model.h>
#include <mkt-paramdouble-model.h>
#include <mkt-paramdouble.h>
#include <mkt-paramint32-model.h>
#include <mkt-paramint32.h>
#include <mkt-paramuint32-model.h>
#include <mkt-paramuint32.h>

#include <mkt-boolexp-parser.h>
#include <mkt-limit-message.h>
#include <mkt-error-message.h>
#include <mkt-log.h>
#include <mkt-point-model.h>
#include <mkt-point.h>
#include <mkt-process-model.h>
#include <mkt-process.h>
#include <mkt-relay-model.h>
#include <mkt-status-model.h>

#include <mkt-axis-model.h>
#include <mkt-injection-model.h>
#include <mkt-measurement-data.h>
#include <mkt-sensor-data.h>
#include <mkt-vessel-model.h>
//#include <mkt-cal.h>
//#include <mkt-cal-model.h>
#include <mkt-analog-model.h>
#include <mkt-cal-data-model.h>
#include <mkt-cal-data.h>
#include <mkt-cal-point-model.h>
#include <mkt-cal-point.h>
#include <mkt-calibration-model.h>
#include <mkt-calibration.h>
#include <mkt-category-model.h>
#include <mkt-category.h>
#include <mkt-channel-model.h>
#include <mkt-statistic-model.h>
#include <mkt-statistic.h>
#include <mkt-stream-model.h>
#include <mkt-user-model.h>
#include <mkt-user.h>
#include <ultimate-integration-object.h>
#include <ultimate-integration.h>
#include <ultimate-messparm-object.h>
#include <ultimate-messparm.h>
#include <ultimate-posparm-object.h>
#include <ultimate-posparm.h>

#include <basics.h>
#include <mkt-enum-types.h>
#include <stringFuncs.h>
#include <lar-timer.h>
#include <mkt-sqlite.h>

void mkt_library_init(void);
void mkt_library_database_init(void);
void mkt_library_sync();
void mkt_library_autocheck(GCancellable *cancellable , GAsyncReadyCallback callback,gpointer user_data);
void mkt_library_autocheck_sync();
#endif /* LIBMKT_H_ */




