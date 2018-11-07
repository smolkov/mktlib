## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src po

mktdocdir = ${datarootdir}/doc/mktlib-2.0

mktdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS



#install-data-hook:
#	mkdir -m 777 -p ${datarootdir}/tera/db
#	chmod 777 -R ${datarootdir}/tera/db

#install-exec-am:
#	mkdir -m 777 -p ${datarootdir}/tera/db
#	test -f /usr/share/tera/db/device.database || sqlite3 /usr/share/tera/db/device.database .databases
#	test -f /usr/share/tera/db/device-default.database || sqlite3 /usr/share/tera/db/device-default.database .databases
#	test -f /usr/share/tera/db/device-calibration.database || sqlite3 /usr/share/tera/db/device-calibration.database .databases
#	test -f /usr/share/tera/db/device-parameter.database || sqlite3 /usr/share/tera/db/device-parameter.database .databases
#	test -f /usr/share/tera/db/device-sensor.database || sqlite3 /usr/share/tera/db/device-sensor.database .databases
#	test -f /usr/share/tera/db/device-measurement.database || sqlite3 /usr/share/tera/db/device-measurement.database .databases
#	chmod -R 777 /usr/share/tera/db


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(mktdocdir)
	