/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktError
 * @ingroup  MktError
 * @{
 * @file  mkt-error.h	ERROR interface model header
 * @brief This is ERROR interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef _MKT_ERROREN_H_
#define _MKT_ERROREN_H_

#include "mkt-model.h"

G_BEGIN_DECLS

#define MKT_TYPE_ERROR (mkt_error_get_type())
#define MKT_ERROR(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), MKT_TYPE_ERROR, MktError))
#define MKT_IS_ERROR(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), MKT_TYPE_ERROR))
#define MKT_ERROR_GET_INTERFACE(inst) (G_TYPE_INSTANCE_GET_INTERFACE((inst), MKT_TYPE_ERROR, MktErrorInterface))

typedef struct _MktError          MktError; /* dummy object */
typedef struct _MktErrorInterface MktErrorInterface;

typedef enum { MKT_ERROR_WARNING, MKT_ERROR_CRITICAL } MktErrorType;

struct _MktErrorInterface {
    GTypeInterface parent_iface;
    gint (*get_number)(MktError *self);
    gboolean (*get_pending)(MktError *self);
    gchar (*get_type)(MktError *self);
    gdouble (*get_changed)(MktError *self);
    gchar *(*get_error_no)(MktError *self);
    gchar *(*description)(MktError *self);
};

GType        mkt_error_get_type(void);
gint         mkt_error_number(MktError *error);
gboolean     mkt_error_pending(MktError *error);
void         mkt_error_set_pending(MktError *error, gboolean value);
gchar        mkt_error_type(MktError *error);
void         mkt_error_set_type(MktError *error, gchar type);
gdouble      mkt_error_changed(MktError *error);
gchar *      mkt_error_description(MktError *error);
void         mkt_error_set_description(MktError *error, const gchar *format, ...) G_GNUC_PRINTF(2, 3);
const gchar *mkt_error_service(void);
void         mkt_error_set_service(const gchar *service);
gboolean     mkt_error_exist(gint number);
void         mkt_error_reset(gint number);
gboolean     mkt_error_create(gint number, gchar type, const gchar *description);
gboolean     mkt_error_createv(gint number, gchar type, const gchar *format, ...) G_GNUC_PRINTF(3, 4);
MktModel *   mkt_error_get_came(gint number);
MktError *   mkt_error_find(gint number);
void         mkt_error_change_type(gint number, MktErrorType type);
void         mkt_error_came(gint number);
void         mkt_error_gone(gint number);
void         mkt_error_came_simple(gint number, const gchar *format, ...) G_GNUC_PRINTF(2, 3);
void         mkt_error_manually_gone(gint number);
gboolean     mkt_error_pending_number(gint number);
gchar *      mkt_error_dup();
gchar *      mkt_error_dup_type(guint type);
gboolean     mkt_error_is_critical();

/**
 * MktErrorsNumbers:
 * LAR error list.
 */

typedef enum {
    E1700 = 1700, /*< nick=Multi-IO error>*/
    E1710 = 1710, /*< nick=X axis movement error>*/
    E1715 = 1715, /*< nick=X axis movement stall guard error>*/
    E1720 = 1720, /*< nick=Y axis movement error>*/
    E1730 = 1730, /*< nick=Z axis movement error>*/
    E1740 = 1740, /*< nick=Injection volume exceeded>*/
    E1750 = 1750, /*< nick=Sequence fail>*/
    E1810 = 1810, /*< nick=Emergency shutdown furnace>*/
    E1815 = 1815, /*< nick=Cooler temperature not yet reached>*/
    E1820 = 1820, /*< nick=Furnace temperature not yet reached>*/
    E1821 = 1821, /*< nick=Switch 'furnace valve' not found>*/
    E1822 = 1822, /*< nick=Switch 'TIC valve' not found>*/
    E1825 = 1825, /*< nick=Old cooler error>*/
    E1830 = 1830, /*< nick=Error carrier gas supply>*/
    E1831 = 1831, /*< nick=Limit humidity exceeded>*/
    E1832 = 1832, /*< nick=Pressure high > 300 mbar>*/
    E1833 = 1833, /*< nick=Pressure too high > 600 mbar>*/
    E1835 = 1835, /*< nick=Deviation carrier gas flow out>*/
    E1836 = 1836, /*< nick=Deviation carrier gas flow out TC-Measurement>*/
    E1837 = 1837, /*< nick=Deviation carrier gas flow out TIC-Measurement>*/
    E1841 = 1841, /*< nick=No sample / no injection #01>*/
    E1842 = 1842, /*< nick=No sample / no injection #02>*/
    E1843 = 1843, /*< nick=No sample / no injection #03>*/
    E1844 = 1844, /*< nick=No sample / no injection #04>*/
    E1845 = 1845, /*< nick=No sample / no injection #05>*/
    E1846 = 1846, /*< nick=No sample / no injection #06>*/
    E1870 = 1870, /*< nick=Brocked Sensor or Node>*/
    E1871 = 1871, /*< nick=Innenraum temperatur zu nidrig>*/
    E1872 = 1872, /*< nick=Furnaseinnenraum temperatur zu hoch>*/
    E1890 = 1890, /*< nick=Node has been reseted>*/
    E1950 = 1950, /*< nick=Sensor 'External Sample Supply' has responded>*/
    E1960 = 1960, /*< nick=Sensor 'Reagent Missing' has responded>*/
    E2010 = 2010, /*< nick=Process internal error>*/
    E2107 = 2107, /*< nick=No water available (internal sample dilution)>*/

} MktErrorsNumbers;
/**
 * MktErrorsNumbers:
 * LAR critical error list.
 */
typedef enum {
    C1700 = E1700,
    C1710 = E1710,
    C1720 = E1720,
    C1730 = E1730,
    C1750 = E1750,
    C1810 = E1810,
    C1815 = E1815,
    C1820 = E1820,
    C1821 = E1821,
    C1822 = E1822,
    C1831 = E1831,
    C1833 = E1833,
    C1835 = E1835,
    C2010 = E2010,

} MktErrorsCritical;

void     mkt_errors_init(gboolean clean);
void     mkt_errors_clean(MktErrorsNumbers number);
gboolean mkt_errors_report(MktErrorsNumbers number, const gchar *format, ...) G_GNUC_PRINTF(2, 3);
gboolean mkt_errors_come(MktErrorsNumbers number);

#define ERROR_QUARK mkt_error_quark()

GQuark mkt_error_quark(void);

G_END_DECLS

#endif /* _MKT_ERROREN_H_ */
/** @} */
