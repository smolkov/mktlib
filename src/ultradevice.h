/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * ultradevice.h
 * Copyright (C) 2017 LAR
 *
 *
 */
#ifndef _ULTRADEVICE_H_
#define _ULTRADEVICE_H_

#include <airflow.h>
#include <analog.h>
#include <sensor.h>
#include <temperatur.h>
#include <axis.h>
#include <device.h>
#include <dilution.h>
#include <glib-object.h>
#include <injection.h>
#include <larintegrations.h>
#include <relay.h>
#include <humidity.h>
#include <pressure.h>
#include <ultrastream.h>
#include <vessel.h>

#define TYPE_ULTRADEVICE (ultradevice_get_type())
#define ULTRADEVICE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_ULTRADEVICE, Ultradevice))
#define ULTRADEVICE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_ULTRADEVICE, UltradeviceClass))
#define IS_ULTRADEVICE(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_ULTRADEVICE))
#define IS_ULTRADEVICE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_ULTRADEVICE))
#define ULTRADEVICE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_ULTRADEVICE, UltradeviceClass))

typedef struct _UltradeviceClass UltradeviceClass;
typedef struct _Ultradevice      Ultradevice;

GType ultradevice_get_type(void) G_GNUC_CONST;

UltraStream *UltradeviceGetStream1(Ultradevice *ultradevice);
UltraStream *UltradeviceGetStream2(Ultradevice *ultradevice);
UltraStream *UltradeviceGetStream3(Ultradevice *ultradevice);
UltraStream *UltradeviceGetStream4(Ultradevice *ultradevice);
UltraStream *UltradeviceGetStream5(Ultradevice *ultradevice);
UltraStream *UltradeviceGetStream6(Ultradevice *ultradevice);
UltraStream *UltradeviceGetStreamNumber(Ultradevice *ultradevice, guint number);

Vessel *         UltradeviceGetFurnace(Ultradevice *ultradevice);
Vessel *         UltradeviceGetTicPort(Ultradevice *ultradevice);
Vessel *         UltradeviceGetV1(Ultradevice *ultradevice);
Vessel *         UltradeviceGetV2(Ultradevice *ultradevice);
Vessel *         UltradeviceGetV3(Ultradevice *ultradevice);
Vessel *         UltradeviceGetV4(Ultradevice *ultradevice);
Vessel *         UltradeviceGetV5(Ultradevice *ultradevice);
Vessel *         UltradeviceGetV6(Ultradevice *ultradevice);
Axis *           UltradeviceGetXAxis(Ultradevice *ultradevice);
Axis *           UltradeviceGetYAxis(Ultradevice *ultradevice);
Injection *      UltradeviceGetInjection(Ultradevice *ultradevice);
LarIntegrations *UltradeviceGetIntegrations(Ultradevice *ultradevice);
gdouble          UltradeviceGetJustificationTime(Ultradevice *ultradevice);
Dilution *       UltradeviceGetDilution(Ultradevice *ultradevice);
Airflow *        UltradeviceGetAirflow(Ultradevice *ultradevice);
Humidity *       UltradeviceGetHumidity(Ultradevice *ultradevice);
Pressure *       UltradeviceGetPressure(Ultradevice *ultradevice);
Relay *          UltradeviceGetRelay1(Ultradevice *ultradevice);
Relay *          UltradeviceGetRelay2(Ultradevice *ultradevice);
Relay *          UltradeviceGetRelay3(Ultradevice *ultradevice);
Relay *          UltradeviceGetRelay4(Ultradevice *ultradevice);
Relay *          UltradeviceGetRelay5(Ultradevice *ultradevice);
Relay *          UltradeviceGetRelay6(Ultradevice *ultradevice);
Relay *          UltradeviceGetRelay7(Ultradevice *ultradevice);
Relay *          UltradeviceGetRelay8(Ultradevice *ultradevice);
Analog *         UltradeviceGetAnalog0(Ultradevice *ultradevice);
GList *          UltradeviceGetAnalogsOut(Ultradevice *ultradevice);

gboolean ultra_device_range_switch(Ultradevice *ultradevice);
gdouble ultra_device_get_range1(Ultradevice *ultradevice);
gdouble ultra_device_get_range2(Ultradevice *ultradecice);
guint ultra_device_get_stirrers(Ultradevice *ultradevice);
Sensor *UltradeviceGetNDir1(Ultradevice *ultradevice); 
 Sensor *UltradeviceGetNDir2(Ultradevice *ultradevice);
Sensor *UltradeviceGetTNb(Ultradevice *ultradevice);
Sensor *UltradeviceGetCODo(Ultradevice *ultradevice);
Temperatur *UltradeviceGetTemperatur(Ultradevice *ultradevice);

#endif /* _ULTRADEVICE_H_ */
