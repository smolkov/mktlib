#
#
#
# @author ASmolkov

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(GLIB_CFLAGS) \
	-I/usr/include/hiredis \
	$(DB_CFLAGS)



AM_CFLAGS = -Wall   \
	 -g  \
	 -lm


BUILT_SOURCES =

lib_LTLIBRARIES = libmktlib-2.0.la



libmkt_built_sources =    \
	mkt-enum-types.h      \
	mkt-enum-types.c


larsrc = \
	airflow.c   \
	amount.c    \
	analog.c    \
	sensor.c    \
	temperatur.c    \
	axis.c    \
	channel.c    \
	device.c    \
	dilution.c    \
	humidity.c    \
	injection.c    \
	larintegration.c    \
	larintgrec.c    \
	larintegrations.c    \
	interval.c    \
	intervals.c    \
	jsonreadwrite.c    \
	lar-init.c    \
	larflags.c    \
	lartime.c    \
	limit.c    \
	larsqlite.c    \
	larmeasurement.c    \
	parameters.c    \
	posparm.c    \
	pressure.c    \
	redis.c    \
	relay.c    \
	larresult.c    \
	sigdata.c    \
	statistic.c    \
	statistics.c    \
	stream.c    \
	ultradevice.c    \
	ultrameas.c    \
	ultrameasurement.c    \
	ultrastream.c    \
	ultraqueries.c    \
	value.c    \
	vessel.c


larheader = \
	airflow.h      \
	amount.h       \
	analog.h       \
	sensor.h       \
	temperatur.h       \
	axis.h         \
	channel.h      \
	device.h       \
	dilution.h     \
	humidity.h     \
	injection.h     \
	larintegration.h       \
	larintgrec.h       \
	larintegrations.h       \
	interval.h       \
	intervals.h       \
	jsonreadwrite.h       \
	larflags.h       \
	lartime.h       \
	limit.h       \
	larsqlite.h       \
	larmeasurement.h       \
	parameters.h       \
	posparm.h       \
	pressure.h       \
	redis.h       \
	relay.h       \
	larresult.h       \
	sigdata.h       \
	statistic.h       \
	statistics.h       \
	stream.h       \
	ultradevice.h       \
	ultrameas.h       \
	ultrameasurement.h       \
	ultrastream.h       \
	ultraqueries.h       \
	value.h       \
	vessel.h

libmktlib_2_0_la_SOURCES =        \
	$(libmkt_built_sources)       \
	$(larsrc)                     \
	lar-timer.c                   \
	mkt-sqlite.c                  \
	mkt-data-column.c             \
	mkt-data-model.c              \
	mkt-connection.c              \
	mkt-model.c                   \
	mkt-selection.c               \
	mkt-param.c                   \
	mkt-param-object.c            \
	mkt-paramint32.c              \
	mkt-paramint32-model.c        \
	mkt-paramuint32-model.c       \
	mkt-paramuint32.c             \
	mkt-paramdouble.c             \
	mkt-paramdouble-model.c       \
	mkt-paramboolean.c            \
	mkt-paramboolean-model.c      \
	mkt-data.c                    \
	mkt-sensor-data.c             \
	mkt-analog.c                  \
	mkt-analog-model.c            \
	mkt-relay.c                   \
	mkt-relay-model.c             \
	mkt-vessel.c                  \
	mkt-vessel-model.c            \
	mkt-axis.c                    \
	mkt-axis-model.c              \
	mkt-injection.c               \
	mkt-injection-model.c         \
	mkt-channel.c                 \
	mkt-channel-model.c           \
	mkt-point.c                   \
	mkt-point-model.c             \
	mkt-cal-data.c                \
	mkt-cal-data-model.c          \
	mkt-cal-point.c               \
	mkt-cal-point-model.c         \
	mkt-calibration.c             \
	mkt-calibration-model.c       \
	mkt-statistic.c               \
	mkt-statistic-model.c         \
	mkt-category.c                \
	mkt-category-model.c          \
	mkt-stream.c                  \
	mkt-stream-model.c            \
	mkt-process.c                 \
	mkt-process-model.c           \
	ultimate-messparm.c           \
	ultimate-messparm-object.c    \
	ultimate-posparm.c            \
	ultimate-posparm-object.c     \
	ultimate-integration.c        \
	ultimate-integration-object.c \
	mkt-error.c                   \
	mkt-error-message.c           \
	mkt-limit.c                   \
	mkt-limit-message.c           \
	mkt-log.c                     \
	mkt-measurement.c             \
	mkt-measurement-data.c        \
	mkt-module.c                  \
	mkt-mutex.c                   \
	mkt-parcer-apache.c           \
	mkt-SQLite-connection.c       \
	mkt-sql-parser.c              \
	mkt-boolexp-ind.c             \
	mkt-boolexp-parser.c          \
	mkt-status.c                  \
	mkt-status-model.c            \
	mkt-user.c                    \
	mkt-user-model.c              \
	mkt-utils.c                   \
	mkt-value.c                   \
	stringFuncs.c                 \
	market-time.c                 \
	mktlib.c





libmktlib_2_0_la_LDFLAGS =-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)
libmktlib_2_0_la_LIBADD = \
	 $(GLIB_LIBS)    \
	 $(DB_LIBS)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = mktlib-2.0.pc

EXTRA_DIST = \
	mktlib-2.0.pc.in





mktlibinkludedir = $(includedir)/mktlib-2.0
mktlibinklude_HEADERS =           \
	mkt-enum-types.h              \
	$(larheader)                  \
	lar-timer.h                   \
	mkt-sqlite.h                  \
	market-time.h                 \
	malfunc.h                     \
	market-statemachine.h         \
	mkt-analog.h                  \
	mkt-analog-model.h            \
	mkt-relay.h                   \
	mkt-relay-model.h             \
	mkt-vessel.h                  \
	mkt-vessel-model.h            \
	mkt-axis.h                    \
	mkt-axis-model.h              \
	mkt-injection.h               \
	mkt-injection-model.h         \
	mkt-channel.h                 \
	mkt-channel-model.h           \
	mkt-point.h                   \
	mkt-point-model.h             \
	mkt-cal-data.h                \
	mkt-cal-data-model.h          \
	mkt-cal-point.h               \
	mkt-cal-point-model.h         \
	mkt-calibration.h             \
	mkt-calibration-model.h       \
	mkt-process.h                 \
	mkt-process-model.h           \
	mkt-connection.h              \
	mkt-data-column.h             \
	mkt-data.h                    \
	mkt-data-model.h              \
	mkt-decl.h                    \
	mkt-error.h                   \
	mkt-error-message.h           \
	mkt-item.h                    \
	mkt-item-object.h             \
	mkt-limit.h                   \
	mkt-limit-message.h           \
	mkt-log.h                     \
	mkt-measurement-data.h        \
	mkt-measurement.h             \
	mkt-selection.h               \
	mkt-category.h                \
	mkt-category-model.h          \
	mkt-statistic.h               \
	mkt-statistic-model.h         \
	mkt-stream.h                  \
	mkt-stream-model.h            \
	ultimate-messparm.h           \
	ultimate-messparm-object.h    \
	ultimate-posparm.h            \
	ultimate-posparm-object.h     \
	ultimate-integration.h        \
	ultimate-integration-object.h \
	mkt-model.h                   \
	mkt-module.h                  \
	mkt-mutex.h                   \
	mkt-param.h                   \
	mkt-param-object.h            \
	mkt-paramint32.h              \
	mkt-paramint32-model.h        \
	mkt-paramuint32-model.h       \
	mkt-paramuint32.h             \
	mkt-paramdouble.h             \
	mkt-paramdouble-model.h       \
	mkt-paramboolean.h            \
	mkt-paramboolean-model.h      \
	mkt-parcer-apache.h           \
	mkt-sensor-data.h             \
	mkt-SQLite-connection.h       \
	mkt-sql-parser.h              \
	mkt-boolexp-ind.h             \
	mkt-boolexp-parser.h          \
	mkt-status.h                  \
	mkt-status-model.h            \
	mkt-user.h                    \
	mkt-user-model.h              \
	mkt-utils.h                   \
	mkt-value.h                   \
	basics.h                      \
	stringFuncs.h                 \
	mktlib.h



TESTS = check_loging check_data
check_PROGRAMS = check_loging check_data

check_loging_SOURCES = check_loging.c 
check_loging_LDFLAGS = -static libmktlib-2.0.la
check_loging_LDADD = $(GLIB) $(DB) $(G_MODULE_LDFLAGS)

check_data_SOURCES = check_data.c 
check_data_LDFLAGS = -static libmktlib-2.0.la
check_data_LDADD = $(GLIB) $(DB) $(G_MODULE_LDFLAGS)




mkt_ENUM_TYPES	=             \
	larmeasurement.h          \
	mkt-analog.h              \
	mkt-analog-model.h        \
	mkt-error.h               \
	mkt-log.h



mkt_ENUM_GENERATES	=     \
	mkt-enum-types.h      \
	mkt-enum-types.c


mkt-enum-types.h: mkt-enum-types.h.template $(mkt_ENUM_TYPES) $(GLIB_MKENUMS)
	$(AM_V_GEN) $(GLIB_MKENUMS) --template mkt-enum-types.h.template $(mkt_ENUM_TYPES) > $@

mkt-enum-types.c: mkt-enum-types.h mkt-enum-types.c.template $(mkt_ENUM_TYPES) $(GLIB_MKENUMS)
	$(AM_V_GEN) $(GLIB_MKENUMS) --template mkt-enum-types.c.template $(mkt_ENUM_TYPES) > $@


EXTRA_DIST += \
	mkt-enum-types.h.template \
	mkt-enum-types.c.template

lardir = $(sysconfdir)/lar
# dist_redis_DATA =      \
# 


BUILT_SOURCES += $(gedit_built_sources)

CLEANFILES=  \
	$(mkt_ENUM_GENERATES) \
	$(libmkt_built_sources)
