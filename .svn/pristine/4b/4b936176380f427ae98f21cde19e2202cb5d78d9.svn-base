/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * ultradevice.c
 * Copyright (C) 2017 LAR
 *
 */

#include "ultradevice.h"
#include "vessel.h"
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>
#include <string.h>

struct _UltradeviceClass
{
    DeviceClass parent_class;
};

struct _Ultradevice
{
    Device parent_instance;
    gboolean rangeSwitch;
    gdouble range1;
    gdouble range2;
    gchar *channel1;
    gchar *channel2;
    UltraStream *stream1;
    UltraStream *stream2;
    UltraStream *stream3;
    UltraStream *stream4;
    UltraStream *stream5;
    UltraStream *stream6;
    Vessel *furnace;
    Vessel *ticPort;
    Vessel *V1;
    Vessel *V2;
    Vessel *V3;
    Vessel *V4;
    Vessel *V5;
    Vessel *V6;
    Axis *xAxis;
    Axis *yAxis;
    Injection *injection;
    LarIntegrations *integrations;
    gdouble justificationTime;
    Dilution *dilution;
    Airflow *airflow;
    Humidity *humidity;
    Pressure *pressure;
    Relay *relay1;
    Relay *relay2;
    Relay *relay3;
    Relay *relay4;
    Relay *relay5;
    Relay *relay6;
    Relay *relay7;
    Relay *relay8;
    Analog *analog0;
    Analog *analog1;
    Analog *analog2;
    Analog *analog3;
    Analog *analog4;
    Analog *analog5;
    Analog *analog6;
    Analog *analog7;
    Analog *analog8;
    Analog *analog9;
    Analog *analog10;
    Analog *analog11;
    Analog *analog12;
    Analog *analog13;
    Analog *analog14;
    Analog *analog15;
    Analog *analog16;
    Analog *analog17;
    Analog *analog18;
    Analog *analog19;
    Analog *analog20;
    guint stirrers;
    Sensor *ndir1;
    Sensor *ndir2;
    Sensor *tnb;
    Sensor *codo;
    Temperatur *temperatur;
};

enum
{
    PROP_0,
    PROP_SWITCH_RANGE,
    PROP_RANGE1,
    PROP_RANGE2,
    PROP_CHANNEL1,
    PROP_CHANNEL2,
    PROP_STREAM1,
    PROP_STREAM2,
    PROP_STREAM3,
    PROP_STREAM4,
    PROP_STREAM5,
    PROP_STREAM6,
    PROP_FURNACE,
    PROP_TICPORT,
    PROP_V1,
    PROP_V2,
    PROP_V3,
    PROP_V4,
    PROP_V5,
    PROP_V6,
    PROP_X,
    PROP_Y,
    PROP_INJECTION,
    PROP_LAR_INTEGRATIONS,
    PROP_JUSTIFACATION_TIME,
    PROP_DILUTION,
    PROP_AIRFLOW,
    PROP_HUMIDITY,
    PROP_PRESSURE,
    PROP_RELAY1,
    PROP_RELAY2,
    PROP_RELAY3,
    PROP_RELAY4,
    PROP_RELAY5,
    PROP_RELAY6,
    PROP_RELAY7,
    PROP_RELAY8,
    PROP_ANALOG0,
    PROP_ANALOG1,
    PROP_ANALOG2,
    PROP_ANALOG3,
    PROP_ANALOG4,
    PROP_ANALOG5,
    PROP_ANALOG6,
    PROP_ANALOG7,
    PROP_ANALOG8,
    PROP_ANALOG9,
    PROP_ANALOG10,
    PROP_ANALOG11,
    PROP_ANALOG12,
    PROP_ANALOG13,
    PROP_ANALOG14,
    PROP_ANALOG15,
    PROP_ANALOG16,
    PROP_ANALOG17,
    PROP_ANALOG18,
    PROP_ANALOG19,
    PROP_ANALOG20,
    PROP_STIRRERS,
    PROP_NDIR1,
    PROP_NDIR2,
    PROP_TNB,
    PROP_CODO,
    PROP_TEMPERATUR,
};

G_DEFINE_TYPE(Ultradevice, ultradevice, TYPE_DEVICE);

static void ultradevice_init(Ultradevice *ultradevice)
{
    ultradevice->stream1 = ULTRA_STREAM(g_object_new(TYPE_ULTRA_STREAM, "name", "#1", "number", 1, NULL));
    ultradevice->stream2 = ULTRA_STREAM(g_object_new(TYPE_ULTRA_STREAM, "name", "#2", "number", 2, NULL));
    ultradevice->stream3 = ULTRA_STREAM(g_object_new(TYPE_ULTRA_STREAM, "name", "#3", "number", 3, NULL));
    ultradevice->stream4 = ULTRA_STREAM(g_object_new(TYPE_ULTRA_STREAM, "name", "#4", "number", 4, NULL));
    ultradevice->stream5 = ULTRA_STREAM(g_object_new(TYPE_ULTRA_STREAM, "name", "#5", "number", 5, NULL));
    ultradevice->stream6 = ULTRA_STREAM(g_object_new(TYPE_ULTRA_STREAM, "name", "#6", "number", 6, NULL));
    ultradevice->furnace = VESSEL(g_object_new(TYPE_VESSEL, "xpos", 120, "ypos", 1700, "y1pos", 720, NULL));
    ultradevice->ticPort = VESSEL(g_object_new(TYPE_VESSEL, "xpos", 832, "ypos", 1200, "y1pos", 450, "hasmotor", FALSE, NULL));
    ultradevice->V1 = VESSEL(g_object_new(TYPE_VESSEL, "xpos", 1130, "ypos", 1250, NULL));
    ultradevice->V2 = VESSEL(g_object_new(TYPE_VESSEL, "xpos", 1420, "ypos", 1250, NULL));
    ultradevice->V3 = VESSEL(g_object_new(TYPE_VESSEL, "xpos", 1700, "ypos", 1250, NULL));
    ultradevice->V4 = VESSEL(g_object_new(TYPE_VESSEL, "xpos", 2000, "ypos", 1250, NULL));
    ultradevice->V5 = VESSEL(g_object_new(TYPE_VESSEL, "xpos", 2280, "ypos", 1250, NULL));
    ultradevice->V6 = VESSEL(g_object_new(TYPE_VESSEL, "xpos", 2400, "ypos", 1250, "y1pos", 1420, NULL));
    ultradevice->xAxis = AXIS(g_object_new(TYPE_AXIS, "hold", 45, "max", 2500, "current", 1200, NULL));
    ultradevice->yAxis = AXIS(g_object_new(TYPE_AXIS, "hold", 80, "max", 2000, "current", 1200, NULL));
    ultradevice->injection = INJECTION(g_object_new(TYPE_INJECTION, "hold", 80, "max", 1800, "current", 1200, "air", 100, "rest", 0, "furnaceAir", 50, "dilution", 0, "rinsing",
                                                    580, "push", 3, "samplePull", 2, "rinsingPull", 2, "rinsingPush", 1, NULL));
    ultradevice->integrations = LAR_INTEGRATIONS(g_object_new(TYPE_LAR_INTEGRATIONS, NULL));
    ultradevice->justificationTime = 20.0;
    ultradevice->dilution = DILUTION(g_object_new(TYPE_DILUTION, NULL));
    ultradevice->airflow = AIRFLOW(g_object_new(TYPE_AIRFLOW, NULL));
    ultradevice->humidity = HUMIDITY(g_object_new(TYPE_HUMIDITY, NULL));
    ultradevice->pressure = PRESSURE(g_object_new(TYPE_PRESSURE, NULL));
    ultradevice->relay1 = RELAY(g_object_new(TYPE_RELAY, "boolexpr", "WARN", NULL));
    ultradevice->relay2 = RELAY(g_object_new(TYPE_RELAY, "boolexpr", "FAIL", NULL));
    ultradevice->relay3 = RELAY(g_object_new(TYPE_RELAY, NULL));
    ultradevice->relay4 = RELAY(g_object_new(TYPE_RELAY, NULL));
    ultradevice->relay5 = RELAY(g_object_new(TYPE_RELAY, NULL));
    ultradevice->relay6 = RELAY(g_object_new(TYPE_RELAY, NULL));
    ultradevice->relay7 = RELAY(g_object_new(TYPE_RELAY, NULL));
    ultradevice->relay8 = RELAY(g_object_new(TYPE_RELAY, NULL));
    ultradevice->analog0 = ANALOG(g_object_new(TYPE_ANALOG, "min", 0.0, "max", 20000.0, NULL));
    ultradevice->analog1 = ANALOG(g_object_new(TYPE_ANALOG, "min", 0.0, "max", 32767.0, NULL));
    ultradevice->analog2 = ANALOG(g_object_new(TYPE_ANALOG, "min", 0.0, "max", 32767.0, NULL));
    ultradevice->analog3 = ANALOG(g_object_new(TYPE_ANALOG, "min", 0.0, "max", 32767.0, NULL));
    ultradevice->analog4 = ANALOG(g_object_new(TYPE_ANALOG, "min", 0.0, "max", 32767.0, NULL));
    ultradevice->analog5 = ANALOG(g_object_new(TYPE_ANALOG, "min", 0.0, "max", 32767.0, NULL));
    ultradevice->analog6 = ANALOG(g_object_new(TYPE_ANALOG, "min", 0.0, "max", 32767.0, NULL));
    ultradevice->analog7 = ANALOG(g_object_new(TYPE_ANALOG, "min", 0.0, "max", 32767.0, NULL));
    ultradevice->analog8 = ANALOG(g_object_new(TYPE_ANALOG, "min", 0.0, "max", 32767.0, NULL));
    ultradevice->analog9 = ANALOG(g_object_new(TYPE_ANALOG, "min", 0.0, "max", 32767.0, NULL));
    ultradevice->analog10 = ANALOG(g_object_new(TYPE_ANALOG, "min", 0.0, "max", 32767.0, NULL));
    ultradevice->analog11 = ANALOG(g_object_new(TYPE_ANALOG, "min", 0.0, "max", 32767.0, NULL));
    ultradevice->analog12 = ANALOG(g_object_new(TYPE_ANALOG, "min", 0.0, "max", 32767.0, NULL));
    ultradevice->analog13 = ANALOG(g_object_new(TYPE_ANALOG, "min", 0.0, "max", 32767.0, NULL));
    ultradevice->analog14 = ANALOG(g_object_new(TYPE_ANALOG, "min", 0.0, "max", 32767.0, NULL));
    ultradevice->analog15 = ANALOG(g_object_new(TYPE_ANALOG, "min", 0.0, "max", 32767.0, NULL));
    ultradevice->analog16 = ANALOG(g_object_new(TYPE_ANALOG, "min", 0.0, "max", 32767.0, NULL));
    ultradevice->analog17 = ANALOG(g_object_new(TYPE_ANALOG, "min", 0.0, "max", 32767.0, NULL));
    ultradevice->analog18 = ANALOG(g_object_new(TYPE_ANALOG, "min", 0.0, "max", 32767.0, NULL));
    ultradevice->analog19 = ANALOG(g_object_new(TYPE_ANALOG, "min", 0.0, "max", 32767.0, NULL));
    ultradevice->analog20 = ANALOG(g_object_new(TYPE_ANALOG, "min", 0.0, "max", 32767.0, NULL));
    ultradevice->stirrers = 0;
    ultradevice->ndir1 = SENSOR(g_object_new(TYPE_SENSOR, "model", 0, "min-one", 0.0, "max-one", 1.0, "min-two", 0.0, "max-two", 1.0, "min-three", 0.0, "max-three", 1.0, NULL));
    ultradevice->ndir2 = SENSOR(g_object_new(TYPE_SENSOR, "model", 0, "min-one", 0.0, "max-one", 1.0, "min-two", 0.0, "max-two", 1.0, "min-three", 0.0, "max-three", 1.0, NULL));
    ultradevice->tnb = SENSOR(g_object_new(TYPE_SENSOR, "model", 0, "min-one", 0.0, "max-one", 1.0, "min-two", 0.0, "max-two", 1.0, "min-three", 0.0, "max-three", 1.0, NULL));
    ultradevice->codo = SENSOR(g_object_new(TYPE_SENSOR, "model", 0, "min-one", 0.0, "max-one", 1.0, "min-two", 0.0, "max-two", 1.0, "min-three", 0.0, "max-three", 1.0, NULL));
    ultradevice->temperatur= TEMPERATUR(g_object_new(TYPE_TEMPERATUR, "housing-soll", 5.0 , "furnace-soll", 95.0, NULL));
}

static void ultradevice_finalize(GObject *object)
{
    Ultradevice *ultradevice = ULTRADEVICE(object);
    if (ultradevice->stream1)
        g_object_unref(ultradevice->stream1);
    if (ultradevice->stream2)
        g_object_unref(ultradevice->stream2);
    if (ultradevice->stream3)
        g_object_unref(ultradevice->stream3);
    if (ultradevice->stream4)
        g_object_unref(ultradevice->stream4);
    if (ultradevice->stream5)
        g_object_unref(ultradevice->stream5);
    if (ultradevice->stream6)
        g_object_unref(ultradevice->stream6);
    if (ultradevice->furnace)
        g_object_unref(ultradevice->furnace);
    if (ultradevice->ticPort)
        g_object_unref(ultradevice->ticPort);
    if (ultradevice->V1)
        g_object_unref(ultradevice->V1);
    if (ultradevice->V2)
        g_object_unref(ultradevice->V2);
    if (ultradevice->V3)
        g_object_unref(ultradevice->V3);
    if (ultradevice->V4)
        g_object_unref(ultradevice->V4);
    if (ultradevice->V5)
        g_object_unref(ultradevice->V5);
    if (ultradevice->V6)
        g_object_unref(ultradevice->V6);
    if (ultradevice->xAxis)
        g_object_unref(ultradevice->xAxis);
    if (ultradevice->yAxis)
        g_object_unref(ultradevice->yAxis);
    if (ultradevice->injection)
        g_object_unref(ultradevice->injection);
    if (ultradevice->integrations)
        g_object_unref(ultradevice->integrations);
    if (ultradevice->dilution)
        g_object_unref(ultradevice->dilution);
    if (ultradevice->airflow)
        g_object_unref(ultradevice->airflow);
    if (ultradevice->humidity)
        g_object_unref(ultradevice->humidity);
    if (ultradevice->pressure)
        g_object_unref(ultradevice->pressure);
    if (ultradevice->relay1)
        g_object_unref(ultradevice->relay1);
    if (ultradevice->relay2)
        g_object_unref(ultradevice->relay2);
    if (ultradevice->relay3)
        g_object_unref(ultradevice->relay3);
    if (ultradevice->relay4)
        g_object_unref(ultradevice->relay4);
    if (ultradevice->relay5)
        g_object_unref(ultradevice->relay5);
    if (ultradevice->relay6)
        g_object_unref(ultradevice->relay6);
    if (ultradevice->relay7)
        g_object_unref(ultradevice->relay7);
    if (ultradevice->relay8)
        g_object_unref(ultradevice->relay8);
    if (ultradevice->analog0)
        g_object_unref(ultradevice->analog0);
    if (ultradevice->analog1)
        g_object_unref(ultradevice->analog1);
    if (ultradevice->analog2)
        g_object_unref(ultradevice->analog2);
    if (ultradevice->analog3)
        g_object_unref(ultradevice->analog3);
    if (ultradevice->analog4)
        g_object_unref(ultradevice->analog4);
    if (ultradevice->analog5)
        g_object_unref(ultradevice->analog5);
    if (ultradevice->analog6)
        g_object_unref(ultradevice->analog6);
    if (ultradevice->analog7)
        g_object_unref(ultradevice->analog7);
    if (ultradevice->analog8)
        g_object_unref(ultradevice->analog8);
    if (ultradevice->analog9)
        g_object_unref(ultradevice->analog9);
    if (ultradevice->analog10)
        g_object_unref(ultradevice->analog10);
    if (ultradevice->analog11)
        g_object_unref(ultradevice->analog11);
    if (ultradevice->analog12)
        g_object_unref(ultradevice->analog12);
    if (ultradevice->analog13)
        g_object_unref(ultradevice->analog13);
    if (ultradevice->analog14)
        g_object_unref(ultradevice->analog14);
    if (ultradevice->analog15)
        g_object_unref(ultradevice->analog15);
    if (ultradevice->analog16)
        g_object_unref(ultradevice->analog16);
    if (ultradevice->analog17)
        g_object_unref(ultradevice->analog17);
    if (ultradevice->analog18)
        g_object_unref(ultradevice->analog18);
    if (ultradevice->analog19)
        g_object_unref(ultradevice->analog19);
    if (ultradevice->analog20)
        g_object_unref(ultradevice->analog20);
    if (ultradevice->channel1)
        g_object_unref(ultradevice->channel1);
    if (ultradevice->channel2)
        g_object_unref(ultradevice->channel2);
    if (ultradevice->ndir1)
        g_object_unref(ultradevice->ndir1);
    if (ultradevice->ndir2)
        g_object_unref(ultradevice->ndir2);
    if (ultradevice->tnb)
        g_object_unref(ultradevice->tnb);
    if (ultradevice->codo)
        g_object_unref(ultradevice->codo);
    if (ultradevice->temperatur)
        g_object_unref(ultradevice->temperatur);


    G_OBJECT_CLASS(ultradevice_parent_class)->finalize(object);
}

static void ultradevice_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_ULTRADEVICE(object));
    Ultradevice *ultradevice = ULTRADEVICE(object);
    switch (prop_id)
    {
    case PROP_SWITCH_RANGE:
        ultradevice->rangeSwitch = g_value_get_boolean(value);
        break;
    case PROP_RANGE1:
        ultradevice->range1 = g_value_get_double(value);
        break;
    case PROP_RANGE2:
        ultradevice->range2 = g_value_get_double(value);
        break;
    case PROP_CHANNEL1:
        if (ultradevice->channel1)
            g_free(ultradevice->channel1);
        ultradevice->channel1 = g_value_dup_string(value);
        break;
    case PROP_CHANNEL2:
        if (ultradevice->channel2)
            g_free(ultradevice->channel2);
        ultradevice->channel2 = g_value_dup_string(value);
        break;
    case PROP_STREAM1:
        if (ultradevice->stream1)
            g_object_unref(ultradevice->stream1);
        ultradevice->stream1 = g_value_dup_object(value);
        break;
    case PROP_STREAM2:
        if (ultradevice->stream2)
            g_object_unref(ultradevice->stream2);
        ultradevice->stream2 = g_value_dup_object(value);
        break;
    case PROP_STREAM3:
        if (ultradevice->stream3)
            g_object_unref(ultradevice->stream3);
        ultradevice->stream3 = g_value_dup_object(value);
        break;
    case PROP_STREAM4:
        if (ultradevice->stream4)
            g_object_unref(ultradevice->stream4);
        ultradevice->stream4 = g_value_dup_object(value);
        break;
    case PROP_STREAM5:
        if (ultradevice->stream5)
            g_object_unref(ultradevice->stream5);
        ultradevice->stream5 = g_value_dup_object(value);
        break;
    case PROP_STREAM6:
        if (ultradevice->stream6)
            g_object_unref(ultradevice->stream6);
        ultradevice->stream6 = g_value_dup_object(value);
        break;
    case PROP_FURNACE:
        if (ultradevice->furnace)
            g_object_unref(ultradevice->furnace);
        ultradevice->furnace = g_value_dup_object(value);
        break;
    case PROP_TICPORT:
        if (ultradevice->ticPort)
            g_object_unref(ultradevice->ticPort);
        ultradevice->ticPort = g_value_dup_object(value);
        break;
    case PROP_V1:
        if (ultradevice->V1)
            g_object_unref(ultradevice->V1);
        ultradevice->V1 = g_value_dup_object(value);
        break;
    case PROP_V2:
        if (ultradevice->V2)
            g_object_unref(ultradevice->V2);
        ultradevice->V2 = g_value_dup_object(value);
        break;
    case PROP_V3:
        if (ultradevice->V3)
            g_object_unref(ultradevice->V3);
        ultradevice->V3 = g_value_dup_object(value);
        break;
    case PROP_V4:
        if (ultradevice->V4)
            g_object_unref(ultradevice->V4);
        ultradevice->V4 = g_value_dup_object(value);
        break;
    case PROP_V5:
        if (ultradevice->V5)
            g_object_unref(ultradevice->V5);
        ultradevice->V5 = g_value_dup_object(value);
        break;
    case PROP_V6:
        if (ultradevice->V6)
            g_object_unref(ultradevice->V6);
        ultradevice->V6 = g_value_dup_object(value);
        break;
    case PROP_X:
        if (ultradevice->xAxis)
            g_object_unref(ultradevice->xAxis);
        ultradevice->xAxis = g_value_dup_object(value);
        break;
    case PROP_Y:
        if (ultradevice->yAxis)
            g_object_unref(ultradevice->yAxis);
        ultradevice->yAxis = g_value_dup_object(value);
        break;
    case PROP_INJECTION:
        if (ultradevice->injection)
            g_object_unref(ultradevice->injection);
        ultradevice->injection = g_value_dup_object(value);
        break;
    case PROP_LAR_INTEGRATIONS:
        if (ultradevice->integrations)
            g_object_unref(ultradevice->integrations);
        ultradevice->integrations = g_value_dup_object(value);
        break;
    case PROP_JUSTIFACATION_TIME:
        ultradevice->justificationTime = g_value_get_double(value);
        break;
    case PROP_DILUTION:
        if (ultradevice->dilution)
            g_object_unref(ultradevice->dilution);
        ultradevice->dilution = g_value_dup_object(value);
        break;
    case PROP_AIRFLOW:
        if (ultradevice->airflow)
            g_object_unref(ultradevice->airflow);
        ultradevice->airflow = g_value_dup_object(value);
        break;
    case PROP_HUMIDITY:
        if (ultradevice->humidity)
            g_object_unref(ultradevice->humidity);
        ultradevice->humidity = g_value_dup_object(value);
        break;
    case PROP_PRESSURE:
        if (ultradevice->pressure)
            g_object_unref(ultradevice->pressure);
        ultradevice->pressure = g_value_dup_object(value);
        break;
    case PROP_RELAY1:
        if (ultradevice->relay1)
            g_object_unref(ultradevice->relay1);
        ultradevice->relay1 = g_value_dup_object(value);
        break;
    case PROP_RELAY2:
        if (ultradevice->relay2)
            g_object_unref(ultradevice->relay2);
        ultradevice->relay2 = g_value_dup_object(value);
        break;
    case PROP_RELAY3:
        if (ultradevice->relay3)
            g_object_unref(ultradevice->relay3);
        ultradevice->relay3 = g_value_dup_object(value);
        break;
    case PROP_RELAY4:
        if (ultradevice->relay4)
            g_object_unref(ultradevice->relay4);
        ultradevice->relay4 = g_value_dup_object(value);
        break;
    case PROP_RELAY5:
        if (ultradevice->relay5)
            g_object_unref(ultradevice->relay5);
        ultradevice->relay5 = g_value_dup_object(value);
        break;
    case PROP_RELAY6:
        if (ultradevice->relay6)
            g_object_unref(ultradevice->relay6);
        ultradevice->relay6 = g_value_dup_object(value);
        break;
    case PROP_RELAY7:
        if (ultradevice->relay7)
            g_object_unref(ultradevice->relay7);
        ultradevice->relay7 = g_value_dup_object(value);
        break;
    case PROP_RELAY8:
        if (ultradevice->relay8)
            g_object_unref(ultradevice->relay8);
        ultradevice->relay8 = g_value_dup_object(value);
        break;
    case PROP_ANALOG0:
        if (ultradevice->analog0)
            g_object_unref(ultradevice->analog0);
        ultradevice->analog0 = g_value_dup_object(value);
        break;
    case PROP_ANALOG1:
        if (ultradevice->analog1)
            g_object_unref(ultradevice->analog1);
        ultradevice->analog1 = g_value_dup_object(value);
        break;
    case PROP_ANALOG2:
        if (ultradevice->analog2)
            g_object_unref(ultradevice->analog2);
        ultradevice->analog2 = g_value_dup_object(value);
        break;
    case PROP_ANALOG3:
        if (ultradevice->analog3)
            g_object_unref(ultradevice->analog3);
        ultradevice->analog3 = g_value_dup_object(value);
        break;
    case PROP_ANALOG4:
        if (ultradevice->analog4)
            g_object_unref(ultradevice->analog4);
        ultradevice->analog4 = g_value_dup_object(value);
        break;
    case PROP_ANALOG5:
        if (ultradevice->analog5)
            g_object_unref(ultradevice->analog5);
        ultradevice->analog5 = g_value_dup_object(value);
        break;
    case PROP_ANALOG6:
        if (ultradevice->analog6)
            g_object_unref(ultradevice->analog6);
        ultradevice->analog6 = g_value_dup_object(value);
        break;
    case PROP_ANALOG7:
        if (ultradevice->analog7)
            g_object_unref(ultradevice->analog7);
        ultradevice->analog7 = g_value_dup_object(value);
        break;
    case PROP_ANALOG8:
        if (ultradevice->analog8)
            g_object_unref(ultradevice->analog8);
        ultradevice->analog8 = g_value_dup_object(value);
        break;
    case PROP_ANALOG9:
        if (ultradevice->analog9)
            g_object_unref(ultradevice->analog9);
        ultradevice->analog9 = g_value_dup_object(value);
        break;
    case PROP_ANALOG10:
        if (ultradevice->analog10)
            g_object_unref(ultradevice->analog10);
        ultradevice->analog10 = g_value_dup_object(value);
        break;
    case PROP_ANALOG11:
        if (ultradevice->analog11)
            g_object_unref(ultradevice->analog11);
        ultradevice->analog11 = g_value_dup_object(value);
        break;
    case PROP_ANALOG12:
        if (ultradevice->analog12)
            g_object_unref(ultradevice->analog12);
        ultradevice->analog12 = g_value_dup_object(value);
        break;
    case PROP_ANALOG13:
        if (ultradevice->analog13)
            g_object_unref(ultradevice->analog13);
        ultradevice->analog13 = g_value_dup_object(value);
        break;
    case PROP_ANALOG14:
        if (ultradevice->analog14)
            g_object_unref(ultradevice->analog14);
        ultradevice->analog14 = g_value_dup_object(value);
        break;
    case PROP_ANALOG15:
        if (ultradevice->analog15)
            g_object_unref(ultradevice->analog15);
        ultradevice->analog15 = g_value_dup_object(value);
        break;
    case PROP_ANALOG16:
        if (ultradevice->analog16)
            g_object_unref(ultradevice->analog16);
        ultradevice->analog16 = g_value_dup_object(value);
        break;
    case PROP_ANALOG17:
        if (ultradevice->analog17)
            g_object_unref(ultradevice->analog17);
        ultradevice->analog17 = g_value_dup_object(value);
        break;
    case PROP_ANALOG18:
        if (ultradevice->analog18)
            g_object_unref(ultradevice->analog18);
        ultradevice->analog18 = g_value_dup_object(value);
        break;
    case PROP_ANALOG19:
        if (ultradevice->analog19)
            g_object_unref(ultradevice->analog19);
        ultradevice->analog19 = g_value_dup_object(value);
        break;
    case PROP_ANALOG20:
        if (ultradevice->analog20)
            g_object_unref(ultradevice->analog20);
        ultradevice->analog20 = g_value_dup_object(value);
        break;
    case PROP_STIRRERS:
        ultradevice->stirrers = g_value_get_uint(value);
        break;
    case PROP_NDIR1:
        if (ultradevice->ndir1)
            g_object_unref(ultradevice->ndir1);
        ultradevice->ndir1 = g_value_dup_object(value);
        break;
    case PROP_NDIR2:
        if (ultradevice->ndir2)
            g_object_unref(ultradevice->ndir2);
        ultradevice->ndir2 = g_value_dup_object(value);
        break;
    case PROP_TNB:
        if (ultradevice->tnb)
            g_object_unref(ultradevice->tnb);
        ultradevice->tnb = g_value_dup_object(value);
        break;
    case PROP_CODO:
        if (ultradevice->codo)
            g_object_unref(ultradevice->codo);
        ultradevice->codo = g_value_dup_object(value);
        break;
    case PROP_TEMPERATUR:
        if (ultradevice->temperatur)
            g_object_unref(ultradevice->temperatur);
        ultradevice->temperatur = g_value_dup_object(value);
        break;
     default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void ultradevice_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_ULTRADEVICE(object));
    Ultradevice *ultradevice = ULTRADEVICE(object);

    switch (prop_id)
    {
    case PROP_SWITCH_RANGE:
        g_value_set_boolean(value, ultradevice->rangeSwitch);
        break;
    case PROP_RANGE1:
        g_value_set_double(value, ultradevice->range1);
        break;
    case PROP_RANGE2:
        g_value_set_double(value, ultradevice->range2);
        break;
    case PROP_CHANNEL1:
        g_value_set_string(value, ultradevice->channel1);
        break;
    case PROP_CHANNEL2:
        g_value_set_string(value, ultradevice->channel2);
        break;
    case PROP_STREAM1:
        g_value_set_object(value, ultradevice->stream1);
        break;
    case PROP_STREAM2:
        g_value_set_object(value, ultradevice->stream2);
        break;
    case PROP_STREAM3:
        g_value_set_object(value, ultradevice->stream3);
        break;
    case PROP_STREAM4:
        g_value_set_object(value, ultradevice->stream4);
        break;
    case PROP_STREAM5:
        g_value_set_object(value, ultradevice->stream5);
        break;
    case PROP_STREAM6:
        g_value_set_object(value, ultradevice->stream6);
        break;
    case PROP_FURNACE:
        g_value_set_object(value, ultradevice->furnace);
        break;
    case PROP_TICPORT:
        g_value_set_object(value, ultradevice->ticPort);
        break;
    case PROP_V1:
        g_value_set_object(value, ultradevice->V1);
        break;
    case PROP_V2:
        g_value_set_object(value, ultradevice->V2);
        break;
    case PROP_V3:
        g_value_set_object(value, ultradevice->V3);
        break;
    case PROP_V4:
        g_value_set_object(value, ultradevice->V4);
        break;
    case PROP_V5:
        g_value_set_object(value, ultradevice->V5);
        break;
    case PROP_V6:
        g_value_set_object(value, ultradevice->V6);
        break;
    case PROP_X:
        g_value_set_object(value, ultradevice->xAxis);
        break;
    case PROP_Y:
        g_value_set_object(value, ultradevice->yAxis);
        break;
    case PROP_INJECTION:
        g_value_set_object(value, ultradevice->injection);
        break;
    case PROP_LAR_INTEGRATIONS:
        g_value_set_object(value, ultradevice->integrations);
        break;
    case PROP_JUSTIFACATION_TIME:
        g_value_set_double(value, ultradevice->justificationTime);
        break;
    case PROP_DILUTION:
        g_value_set_object(value, ultradevice->dilution);
        break;
    case PROP_AIRFLOW:
        g_value_set_object(value, ultradevice->airflow);
        break;
    case PROP_HUMIDITY:
        g_value_set_object(value, ultradevice->humidity);
        break;
    case PROP_PRESSURE:
        g_value_set_object(value, ultradevice->pressure);
        break;
    case PROP_RELAY1:
        g_value_set_object(value, ultradevice->relay1);
        break;
    case PROP_RELAY2:
        g_value_set_object(value, ultradevice->relay2);
        break;
    case PROP_RELAY3:
        g_value_set_object(value, ultradevice->relay3);
        break;
    case PROP_RELAY4:
        g_value_set_object(value, ultradevice->relay4);
        break;
    case PROP_RELAY5:
        g_value_set_object(value, ultradevice->relay5);
        break;
    case PROP_RELAY6:
        g_value_set_object(value, ultradevice->relay6);
        break;
    case PROP_RELAY7:
        g_value_set_object(value, ultradevice->relay7);
        break;
    case PROP_RELAY8:
        g_value_set_object(value, ultradevice->relay8);
        break;
    case PROP_ANALOG0:
        g_value_set_object(value, ultradevice->analog0);
        break;
    case PROP_ANALOG1:
        g_value_set_object(value, ultradevice->analog1);
        break;
    case PROP_ANALOG2:
        g_value_set_object(value, ultradevice->analog2);
        break;
    case PROP_ANALOG3:
        g_value_set_object(value, ultradevice->analog3);
        break;
    case PROP_ANALOG4:
        g_value_set_object(value, ultradevice->analog4);
        break;
    case PROP_ANALOG5:
        g_value_set_object(value, ultradevice->analog5);
        break;
    case PROP_ANALOG6:
        g_value_set_object(value, ultradevice->analog6);
        break;
    case PROP_ANALOG7:
        g_value_set_object(value, ultradevice->analog7);
        break;
    case PROP_ANALOG8:
        g_value_set_object(value, ultradevice->analog8);
        break;
    case PROP_ANALOG9:
        g_value_set_object(value, ultradevice->analog9);
        break;
    case PROP_ANALOG10:
        g_value_set_object(value, ultradevice->analog10);
        break;
    case PROP_ANALOG11:
        g_value_set_object(value, ultradevice->analog11);
        break;
    case PROP_ANALOG12:
        g_value_set_object(value, ultradevice->analog12);
        break;
    case PROP_ANALOG13:
        g_value_set_object(value, ultradevice->analog13);
        break;
    case PROP_ANALOG14:
        g_value_set_object(value, ultradevice->analog14);
        break;
    case PROP_ANALOG15:
        g_value_set_object(value, ultradevice->analog15);
        break;
    case PROP_ANALOG16:
        g_value_set_object(value, ultradevice->analog16);
        break;
    case PROP_ANALOG17:
        g_value_set_object(value, ultradevice->analog17);
        break;
    case PROP_ANALOG18:
        g_value_set_object(value, ultradevice->analog18);
        break;
    case PROP_ANALOG19:
        g_value_set_object(value, ultradevice->analog19);
        break;
    case PROP_ANALOG20:
        g_value_set_object(value, ultradevice->analog20);
        break;
    case PROP_STIRRERS:
        g_value_set_uint(value, ultradevice->stirrers);
        break;
    case PROP_NDIR1:
        g_value_set_object(value, ultradevice->ndir1);
        break;
    case PROP_NDIR2:
        g_value_set_object(value, ultradevice->ndir2);
        break;
    case PROP_TNB:
        g_value_set_object(value, ultradevice->tnb);
        break;
    case PROP_CODO:
        g_value_set_object(value, ultradevice->codo);
        break;
    case PROP_TEMPERATUR:
        g_value_set_object(value, ultradevice->temperatur);
        break;
     default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void ultradevice_class_init(UltradeviceClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->finalize = ultradevice_finalize;
    object_class->set_property = ultradevice_set_property;
    object_class->get_property = ultradevice_get_property;
    g_object_class_install_property(object_class, PROP_STREAM1, g_param_spec_object("stream1", "stream #1 parameters", "", TYPE_ULTRA_STREAM, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_STREAM2, g_param_spec_object("stream2", "stream #2 parameters", "", TYPE_ULTRA_STREAM, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_STREAM3, g_param_spec_object("stream3", "stream #3 parameters", "", TYPE_ULTRA_STREAM, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_STREAM4, g_param_spec_object("stream4", "stream #4 parameters", "", TYPE_ULTRA_STREAM, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_STREAM5, g_param_spec_object("stream5", "stream #5 parameters", "", TYPE_ULTRA_STREAM, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_STREAM6, g_param_spec_object("stream6", "stream #6 parameters", "", TYPE_ULTRA_STREAM, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_FURNACE, g_param_spec_object("furnace", "furnace parameters", "", TYPE_VESSEL, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_TICPORT, g_param_spec_object("ticport", "ticport parameters", "", TYPE_VESSEL, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_V1, g_param_spec_object("vessel1", "vessel #1 parameters", "", TYPE_VESSEL, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_V2, g_param_spec_object("vessel2", "vessel #2 parameters", "", TYPE_VESSEL, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_V3, g_param_spec_object("vessel3", "vessel #3 parameters", "", TYPE_VESSEL, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_V4, g_param_spec_object("vessel4", "vessel #4 parameters", "", TYPE_VESSEL, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_V5, g_param_spec_object("vessel5", "vessel #5 parameters", "", TYPE_VESSEL, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_V6, g_param_spec_object("vessel6", "vessel #6 parameters", "", TYPE_VESSEL, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_X, g_param_spec_object("x", "X axis parameters", "", TYPE_AXIS, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_Y, g_param_spec_object("y", "Y axis parameters", "", TYPE_AXIS, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_INJECTION,
                                    g_param_spec_object("injection", "Injection parameters", "", TYPE_INJECTION, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_LAR_INTEGRATIONS,
                                    g_param_spec_object("integrations", "integrations", "", TYPE_LAR_INTEGRATIONS, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_JUSTIFACATION_TIME,
                                    g_param_spec_double("justificationTime", "Justification time", "", 1.0, 100.0, 20.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_SWITCH_RANGE,
                                    g_param_spec_boolean("switchRange", "switch range", "switch range activated", FALSE, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_RANGE1,
                                    g_param_spec_double("range1", "range1", "range1 limit", 0.0, G_MAXDOUBLE, 0.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_RANGE2,
                                    g_param_spec_double("range2", "range2", "range2 limit", 0.0, G_MAXDOUBLE, 0.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_DILUTION, g_param_spec_object("dilution", "Dilution parameters", "", TYPE_DILUTION, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_AIRFLOW, g_param_spec_object("airflow", "Airflow parameters", "", TYPE_AIRFLOW, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_HUMIDITY, g_param_spec_object("humidity", "Humidity parameters", "", TYPE_HUMIDITY, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_PRESSURE, g_param_spec_object("pressure", "Pressure parameters", "", TYPE_PRESSURE, G_PARAM_READABLE | G_PARAM_WRITABLE));

    g_object_class_install_property(object_class, PROP_RELAY1, g_param_spec_object("relay1", "Relay1 parameters", "", TYPE_RELAY, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_RELAY2, g_param_spec_object("relay2", "Relay2 parameters", "", TYPE_RELAY, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_RELAY3, g_param_spec_object("relay3", "Relay3 parameters", "", TYPE_RELAY, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_RELAY4, g_param_spec_object("relay4", "Relay4 parameters", "", TYPE_RELAY, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_RELAY5, g_param_spec_object("relay5", "Relay5 parameters", "", TYPE_RELAY, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_RELAY6, g_param_spec_object("relay6", "Relay6 parameters", "", TYPE_RELAY, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_RELAY7, g_param_spec_object("relay7", "Relay7 parameters", "", TYPE_RELAY, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_RELAY8, g_param_spec_object("relay8", "Relay8 parameters", "", TYPE_RELAY, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_ANALOG0, g_param_spec_object("analog0", "Analog out parameters", "", TYPE_ANALOG, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_ANALOG1, g_param_spec_object("analog1", "Analog out parameters", "", TYPE_ANALOG, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_ANALOG2, g_param_spec_object("analog2", "Analog out parameters", "", TYPE_ANALOG, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_ANALOG3, g_param_spec_object("analog3", "Analog out parameters", "", TYPE_ANALOG, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_ANALOG4, g_param_spec_object("analog4", "Analog out parameters", "", TYPE_ANALOG, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_ANALOG5, g_param_spec_object("analog5", "Analog out parameters", "", TYPE_ANALOG, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_ANALOG6, g_param_spec_object("analog6", "Analog out parameters", "", TYPE_ANALOG, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_ANALOG7, g_param_spec_object("analog7", "Analog out parameters", "", TYPE_ANALOG, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_ANALOG8, g_param_spec_object("analog8", "Analog out parameters", "", TYPE_ANALOG, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_ANALOG9, g_param_spec_object("analog9", "Analog out parameters", "", TYPE_ANALOG, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_ANALOG10, g_param_spec_object("analog10", "Analog out parameters", "", TYPE_ANALOG, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_ANALOG11, g_param_spec_object("analog11", "Analog out parameters", "", TYPE_ANALOG, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_ANALOG12, g_param_spec_object("analog12", "Analog out parameters", "", TYPE_ANALOG, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_ANALOG13, g_param_spec_object("analog13", "Analog out parameters", "", TYPE_ANALOG, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_ANALOG14, g_param_spec_object("analog14", "Analog out parameters", "", TYPE_ANALOG, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_ANALOG15, g_param_spec_object("analog15", "Analog out parameters", "", TYPE_ANALOG, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_ANALOG16, g_param_spec_object("analog16", "Analog out parameters", "", TYPE_ANALOG, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_ANALOG17, g_param_spec_object("analog17", "Analog out parameters", "", TYPE_ANALOG, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_ANALOG18, g_param_spec_object("analog18", "Analog out parameters", "", TYPE_ANALOG, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_ANALOG19, g_param_spec_object("analog19", "Analog out parameters", "", TYPE_ANALOG, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_ANALOG20, g_param_spec_object("analog20", "Analog out parameters", "", TYPE_ANALOG, G_PARAM_READABLE | G_PARAM_WRITABLE));


    g_object_class_install_property(object_class, PROP_CHANNEL1,
                                    g_param_spec_string("channel1", "channel range 1", "channel range 1", "-", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_CHANNEL2,
                                    g_param_spec_string("channel2", "channel range 2", "channel range 2", "-", G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(object_class, PROP_STIRRERS, g_param_spec_uint("stirrers", "Stirrers count parameters", "", 0, 7, 0, G_PARAM_READABLE | G_PARAM_WRITABLE));
// Sensoren
    g_object_class_install_property(object_class, PROP_NDIR1, g_param_spec_object("ndir1", "NDir1 parameters", "", TYPE_SENSOR, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_NDIR2, g_param_spec_object("ndir2", "NDir2 parameters", "", TYPE_SENSOR, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_TNB, g_param_spec_object("tnb", "TNb parameters", "", TYPE_SENSOR, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_CODO, g_param_spec_object("codo", "CODo parameters", "", TYPE_SENSOR, G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_class_install_property(object_class, PROP_TEMPERATUR, g_param_spec_object("temperatur", "Temperatur observe", "", TYPE_SENSOR, G_PARAM_READABLE | G_PARAM_WRITABLE));
}

/**
 * UltradeviceGetStream1:
 * @ultradevice: a #Ultradevice
 *
 * Get stream #1 object.
 *
 * Return: #UltraStream
 */

UltraStream *UltradeviceGetStream1(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->stream1;
}
/**
 * UltradeviceGetStream2:
 * @ultradevice: a #Ultradevice
 *
 * Get stream #2 object.
 *
 * Return: #UltraStream
 */

UltraStream *UltradeviceGetStream2(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->stream2;
}
/**
 * UltradeviceGetStream3:
 * @ultradevice: a #Ultradevice
 *
 * Get stream #3 object.
 *
 * Return: #UltraStream
 */

UltraStream *UltradeviceGetStream3(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->stream3;
}
/**
 * UltradeviceGetStream4:
 * @ultradevice: a #Ultradevice
 *
 * Get stream #4 object.
 *
 * Return: #UltraStream
 */

UltraStream *UltradeviceGetStream4(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->stream4;
}
/**
 * UltradeviceGetStream5:
 * @ultradevice: a #Ultradevice
 *
 * Get stream #5 object.
 *
 * Return: #UltraStream
 */

UltraStream *UltradeviceGetStream5(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->stream5;
}
/**
 * UltradeviceGetStream6:
 * @ultradevice: a #Ultradevice
 *
 * Get stream #6 object.
 *
 * Return: #UltraStream
 */

UltraStream *UltradeviceGetStream6(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->stream6;
}

/**
 * UltradeviceGetStreamNumber:
 * @ultradevice: a #Ultradevice
 *
 * Get stream #6 object.
 *
 * Return: #UltraStream
 */

UltraStream *UltradeviceGetStreamNumber(Ultradevice *ultradevice, guint number)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    switch (number)
    {
    case 1:
        return ultradevice->stream1;
    case 2:
        return ultradevice->stream2;
    case 3:
        return ultradevice->stream3;
    case 4:
        return ultradevice->stream4;
    case 5:
        return ultradevice->stream5;
    case 6:
        return ultradevice->stream6;
    default:
        return ultradevice->stream1;
    }
}

/**
 * UltradeviceGetFurnace:
 * @ultradevice: a #Ultradevice
 *
 * Get furnace pos parameter
 *
 * Return: #Vessel
 */

Vessel *UltradeviceGetFurnace(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->furnace;
}
/**
 * UltradeviceGetTicTort:
 * @ultradevice: a #Ultradevice
 *
 * Get TIC-Port pos parameter
 *:-
 * Return: #Vessel
 */

Vessel *UltradeviceGetTicPort(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->ticPort;
}
/**
 * UltradeviceGetV1:
 * @ultradevice: a #Ultradevice
 *
 * Get V1 pos parameter
 *
 * Return: #Vessel
 */

Vessel *UltradeviceGetV1(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->V1;
}
/**
 * UltradeviceGetV2:
 * @ultradevice: a #Ultradevice
 *
 * Get V2 pos parameter
 *
 * Return: #Vessel
 */

Vessel *UltradeviceGetV2(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->V2;
}

/**
 * UltradeviceGetV3:
 * @ultradevice: a #Ultradevice
 *
 * Get V3 pos parameter
 *
 * Return: #Vessel
 */

Vessel *UltradeviceGetV3(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->V3;
}
/**
 * UltradeviceGetV4:
 * @ultradevice: a #Ultradevice
 *
 * Get V4 pos parameter
 *
 * Return: #Vessel
 */

Vessel *UltradeviceGetV4(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->V4;
}
/**
 * UltradeviceGetV5:
 * @ultradevice: a #Ultradevice
 *
 * Get V5 pos parameter
 *
 * Return: #Vessel
 */

Vessel *UltradeviceGetV5(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->V5;
}

/**
 * UltradeviceGetV6:
 * @ultradevice: a #Ultradevice
 *
 * Get V6 pos parameter
 *
 * Return: #Vessel
 */

Vessel *UltradeviceGetV6(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->V6;
}
/**
 * UltradeviceGetXAxis:
 * @ultradevice: a #Ultradevice
 *
 * Get X Axis parameter
 *
 * Return: #Axis
 */

Axis *UltradeviceGetXAxis(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->xAxis;
}
/**
 * UltradeviceGetYAxis:
 * @ultradevice: a #Ultradevice
 *
 * Get Y Axis parameter
 *
 * Return: #Axis
 */

Axis *UltradeviceGetYAxis(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->yAxis;
}
/**
 * UltradeviceGetInjection:
 * @ultradevice: a #Ultradevice
 *
 * Get injection parameter
 *
 * Return: #Injection
 */

Injection *UltradeviceGetInjection(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->injection;
}
/**
 * UltradeviceGetIntegrations:
 * @ultradevice: a #Ultradevice
 *
 * Get integrations parameter
 *
 * Return: #LarIntegrations
 */

LarIntegrations *UltradeviceGetIntegrations(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->integrations;
}
/**
 * UltradeviceGetJustificationTime:
 * @ultradevice: a #Ultradevice
 *
 * Get sensors justification time in seconds
 *
 * Return: sec
 */

gdouble UltradeviceGetJustificationTime(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, 15.0);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), 15.0);
    return ultradevice->justificationTime;
}
/**
 * UltradeviceGetDilution:
 * @ultradevice: a #Ultradevice
 *
 * Get dilution parameter
 *
 * Return: #Dilution
 */

Dilution *UltradeviceGetDilution(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->dilution;
}
/**
 * UltradeviceGetAirflow:
 * @ultradevice: a #Ultradevice
 *
 * Get airflow parameter
 *
 * Return: #Airflow
 */

Airflow *UltradeviceGetAirflow(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->airflow;
}
/**
 * UltradeviceGetHunidity:
 * @ultradevice: a #Ultradevice
 *
 * Get humidity parameter
 *
 * Return: #Humidity
 */

Humidity *UltradeviceGetHumidity(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->humidity;
}
/**
 * UltradeviceGetPressure:
 * @ultradevice: a #Ultradevice
 *
 * Get pressure parameter
 *
 * Return: #Pressure
 */

Pressure *UltradeviceGetPressure(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->pressure;
}

/**
 * UltradeviceGetRelay1:
 * @ultradevice: a #Ultradevice
 *
 * Get relay parameter
 *
 * Return: #Relay
 */

Relay *UltradeviceGetRelay1(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->relay1;
}
/**
 * UltradeviceGetRelay2:
 * @ultradevice: a #Ultradevice
 *
 * Get relay parameter
 *
 * Return: #Relay
 */

Relay *UltradeviceGetRelay2(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->relay2;
}
/**
 * UltradeviceGetRelay3:
 * @ultradevice: a #Ultradevice
 *
 * Get relay parameter
 *
 * Return: #Relay
 */

Relay *UltradeviceGetRelay3(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->relay3;
}
/**
 * UltradeviceGetRelay4:
 * @ultradevice: a #Ultradevice
 *
 * Get relay parameter
 *
 * Return: #Relay
 */

Relay *UltradeviceGetRelay4(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->relay4;
}
/**
 * UltradeviceGetRelay5:
 * @ultradevice: a #Ultradevice
 *
 * Get relay parameter
 *
 * Return: #Relay
 */

Relay *UltradeviceGetRelay5(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->relay5;
}
/**
 * UltradeviceGetRelay6:
 * @ultradevice: a #Ultradevice
 *
 * Get relay parameter
 *
 * Return: #Relay
 */

Relay *UltradeviceGetRelay6(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->relay6;
}
/**
 * UltradeviceGetRelay7:
 * @ultradevice: a #Ultradevice
 *
 * Get relay parameter
 *
 * Return: #Relay
 */

Relay *UltradeviceGetRelay7(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->relay7;
}
/**
 * UltradeviceGetRelay8:
 * @ultradevice: a #Ultradevice
 *
 * Get relay parameter
 *
 * Return: #Relay
 */

Relay *UltradeviceGetRelay8(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->relay8;
}

/**
 * UltradeviceGetAnalog0:
 * @ultradevice: a #Ultradevice
 *
 * Get analog0 parameter
 *
 * Return: #Analog
 */
Analog *UltradeviceGetAnalog0(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->analog0;
}

/**
 * UltradeviceGetAnalogsOut:
 * @ultradevice: a #Ultradevice
 *
 * Get list  analogs out without Analog0
 *
 * Return: a new #GList not free elemets
 */
GList *UltradeviceGetAnalogsOut(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    GList *analogList = NULL;
    analogList = g_list_append(analogList, ultradevice->analog1);
    analogList = g_list_append(analogList, ultradevice->analog2);
    analogList = g_list_append(analogList, ultradevice->analog3);
    analogList = g_list_append(analogList, ultradevice->analog4);
    analogList = g_list_append(analogList, ultradevice->analog5);
    analogList = g_list_append(analogList, ultradevice->analog6);
    analogList = g_list_append(analogList, ultradevice->analog7);
    analogList = g_list_append(analogList, ultradevice->analog8);
    analogList = g_list_append(analogList, ultradevice->analog9);
    analogList = g_list_append(analogList, ultradevice->analog10);
    analogList = g_list_append(analogList, ultradevice->analog11);
    analogList = g_list_append(analogList, ultradevice->analog12);
    analogList = g_list_append(analogList, ultradevice->analog13);
    analogList = g_list_append(analogList, ultradevice->analog14);
    analogList = g_list_append(analogList, ultradevice->analog15);
    analogList = g_list_append(analogList, ultradevice->analog16);
    analogList = g_list_append(analogList, ultradevice->analog17);
    analogList = g_list_append(analogList, ultradevice->analog18);
    analogList = g_list_append(analogList, ultradevice->analog19);
    analogList = g_list_append(analogList, ultradevice->analog20);
    return analogList;
}

gboolean ultra_device_range_switch(Ultradevice *ultradevice)
{
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), FALSE);
    return ultradevice->rangeSwitch;
}
gdouble ultra_device_get_range1(Ultradevice *ultradevice)
{
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), 0.0);
    return ultradevice->range1;
}
gdouble ultra_device_get_range2(Ultradevice *ultradevice)
{
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), 0.0);
    return ultradevice->range2;
}

guint ultra_device_get_stirrers(Ultradevice *ultradevice)
{
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), 0);
    return ultradevice->stirrers;
}
/**
 * UltradeviceGetNDir1:
 * @ultradevice: a #Ultradevice
 *
 * Get sensor parameter
 *
 * Return: #Sensor
 */
Sensor *UltradeviceGetNDir1(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->ndir1;
}

/**
 * UltradeviceGetNDir2:
 * @ultradevice: a #Ultradevice
 *
 * Get sensor parameter
 *
 * Return: #Sensor
 */
Sensor *UltradeviceGetNDir2(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->ndir2;
}

/**
 * UltradeviceGetTNb:
 * @ultradevice: a #Ultradevice
 *
 * Get sensor parameter
 *
 * Return: #Sensor
 */
Sensor *UltradeviceGetTNb(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->tnb;
}

/**
 * UltradeviceGetCODo:
 * @ultradevice: a #Ultradevice
 *
 * Get sensor parameter
 *
 * Return: #Sensor
 */
Sensor *UltradeviceGetCODo(Ultradevice *ultradevice)
{
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->codo;
}
/**
 * UltradeviceGetTemperaturn:
 * @ultradevice: a #Ultradevice
 *
 * Get sensor parameter
 *
 * Return: #Temperaturn
 */
Temperatur *UltradeviceGetTemperatur(Ultradevice *ultradevice){
    g_return_val_if_fail(ultradevice != NULL, NULL);
    g_return_val_if_fail(IS_ULTRADEVICE(ultradevice), NULL);
    return ultradevice->temperatur;
}
