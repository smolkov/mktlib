/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * larintgrec.c
 * Copyright (C) LAR 2017
 *
 */

#include "larintgrec.h"
#include "lartime.h"
#include <glib-object.h>
#include <json-glib/json-glib.h>

enum
{
    PROP_0,
    PROP_ZERO,
    PROP_INTERVAL,
    PROP_ZEROSTART,
    PROP_ZEROSTOP,
    PROP_RUN,
    PROP_START,
    PROP_STOP,
    PROP_DATAS,
};

// typedef struct _ObjectArrayBoxed ObjectArrayBoxed;

// struct _ObjectArrayBoxed {
//     GSList *datas;
// };

// /*** implementation ***/

// gpointer copyDatas(gconstpointer src, gpointer data) {
//     if (src != NULL && G_IS_OBJECT(src)) {
//         return g_object_ref(G_OBJECT(src));
//     }
//     return (gpointer)src;
// }

// ObjectArrayBoxed *ObjectArrayBoxedNew() {
//     ObjectArrayBoxed *new = g_slice_new(ObjectArrayBoxed);
//     new->datas            = NULL;
//     return new;
// }

// static ObjectArrayBoxed *objectArrayBoxedCopy(const ObjectArrayBoxed *src) {
//     ObjectArrayBoxed *copy = ObjectArrayBoxedNew();
//     copy->datas            = NULL;
//     if (src != NULL && src->datas != NULL) {
//         GSList *l = src->datas;
//         for (; l != NULL; l = l->next) {
//             if (IS_SIGDATA(l->data)) {
//                 copy->datas = g_slist_append(copy->datas, g_object_ref(l->data));
//             }
//         }
//     }
//     // g_print("objectArrayBoxedCopy %p\n",copy);
//     return copy;
// }

// static void objectArrayBoxedFree(ObjectArrayBoxed *boxed) {
//     if (G_LIKELY(boxed)) {
//         if (boxed->datas != NULL) {
//             if (boxed->datas->data != NULL && IS_SIGDATA(boxed->datas->data))
//                 g_slist_free_full(boxed->datas, g_object_unref);
//             else
//                 g_slist_free(boxed->datas);
//         }
//         g_slice_free(ObjectArrayBoxed, boxed);
//     }
// }

// static JsonNode *objectArrayBoxedSerialize(gconstpointer boxed) {
//     const ObjectArrayBoxed *objectArray = boxed;
//     JsonArray *             array;
//     JsonNode *              node;

//     if (boxed == NULL) return json_node_new(JSON_NODE_NULL);
//     array      = json_array_new();
//     node       = json_node_new(JSON_NODE_ARRAY);
//     GSList *ld = NULL;
//     for (ld = objectArray->datas; ld != NULL; ld = ld->next) {
//         if (IS_SIGDATA(ld->data)) {
//             JsonNode *node = json_gobject_serialize(G_OBJECT(ld->data));
//             if (node) json_array_add_element(array, node);
//         }
//     }
//     json_node_take_array(node, array);
//     return node;
// }

// static gpointer objectArrayBoxedDeserialize(JsonNode *node) {
//     JsonArray *       array;
//     ObjectArrayBoxed *objectArray;

//     if (json_node_get_node_type(node) != JSON_NODE_ARRAY) return NULL;

//     array = json_node_get_array(node);

//     objectArray = ObjectArrayBoxedNew();
//     GList *el   = json_array_get_elements(array);
//     for (; el != NULL; el = el->next) {
//         if (JSON_NODE_HOLDS_OBJECT((JsonNode *)(el->data))) {
//             GObject *object = json_gobject_deserialize(TYPE_SIGDATA, (JsonNode *)(el->data));
//             if (object && IS_SIGDATA(object)) {
//                 objectArray->datas = g_slist_append(objectArray->datas, g_object_ref(object));
//             }
//         }
//     }
//     return objectArray;
// }

// GType objectarray_boxed_get_type(void) {
//     static GType b_type = 0;

//     if (G_UNLIKELY(b_type == 0)) {
//         b_type = g_boxed_type_register_static("ObjectArrayBoxed", (GBoxedCopyFunc)objectArrayBoxedCopy, (GBoxedFreeFunc)objectArrayBoxedFree);
//         json_boxed_register_serialize_func(b_type, JSON_NODE_ARRAY, objectArrayBoxedSerialize);
//         json_boxed_register_deserialize_func(b_type, JSON_NODE_ARRAY, objectArrayBoxedDeserialize);
//     }

//     return b_type;
// }

// gboolean objectArrayAddObject(ObjectArrayBoxed *boxed, GObject *object) {
//     g_return_val_if_fail(boxed != NULL, FALSE);
//     g_return_val_if_fail(object != NULL, FALSE);
//     g_return_val_if_fail(G_IS_OBJECT(object), FALSE);
//     boxed->datas = g_slist_append(boxed->datas, g_object_ref(object));
//     return TRUE;
// }

// static GSList *objectArrayGetDatas(ObjectArrayBoxed *boxed) {
//     g_return_val_if_fail(boxed != NULL, NULL);
//     return boxed->datas;
// }

// #define ARRAY_TYPE_BOXED (array_boxed_get_type())
typedef struct _ArrayBoxed ArrayBoxed;

struct _ArrayBoxed
{
    GArray *array;
};

/*** implementation ***/

static ArrayBoxed *ArrayBoxedNew()
{
    ArrayBoxed *new = g_slice_new(ArrayBoxed);
    new->array = g_array_new(FALSE, FALSE, sizeof(gdouble));
    return new;
}

static ArrayBoxed *ArrayBoxedCopy(const ArrayBoxed *src)
{
    ArrayBoxed *copy = ArrayBoxedNew();
    if (src != NULL && src->array != NULL)
    {
        copy->array = g_array_append_vals(copy->array, src->array->data, src->array->len);
    }
    return copy;
}

static void ArrayBoxedFree(ArrayBoxed *boxed)
{
    if (G_LIKELY(boxed))
    {
        if (boxed->array != NULL)
        {
            g_array_free(boxed->array, TRUE);
        }
        g_slice_free(ArrayBoxed, boxed);
    }
}

static JsonNode *arrayBoxedSerialize(gconstpointer boxed)
{
    const ArrayBoxed *bArray = boxed;
    JsonArray *array;
    JsonNode *node;

    if (bArray == NULL)
        return json_node_new(JSON_NODE_NULL);
    if (bArray->array == NULL)
        return json_node_new(JSON_NODE_NULL);
    array = json_array_new();
    node = json_node_new(JSON_NODE_ARRAY);
    gint i = 0;
    for (; i < bArray->array->len; i++)
    {
        json_array_add_double_element(array, g_array_index(bArray->array, gdouble, i));
    }
    json_node_take_array(node, array);
    return node;
}

static gpointer arrayBoxedDeserialize(JsonNode *node)
{
    JsonArray *array;
    ArrayBoxed *bArray;
    if (json_node_get_node_type(node) != JSON_NODE_ARRAY)
        return NULL;
    array = json_node_get_array(node);
    bArray = ArrayBoxedNew();
    // GList *el = json_array_get_elements(array);
    gint i = 0;
    for (; i < json_array_get_length(array); i++)
    {
        gdouble val = json_array_get_double_element(array, i);
        g_array_append_val(bArray->array, val);
    }
    return bArray;
}

GType array_boxed_get_type(void)
{
    static GType b_type = 0;

    if (G_UNLIKELY(b_type == 0))
    {
        b_type = g_boxed_type_register_static("ArrayBoxed", (GBoxedCopyFunc)ArrayBoxedCopy, (GBoxedFreeFunc)ArrayBoxedFree);
        json_boxed_register_serialize_func(b_type, JSON_NODE_ARRAY, arrayBoxedSerialize);
        json_boxed_register_deserialize_func(b_type, JSON_NODE_ARRAY, arrayBoxedDeserialize);
    }

    return b_type;
}

gint ArrayBoxedAddValue(ArrayBoxed *boxed, gdouble val)
{
    g_return_val_if_fail(boxed != NULL, FALSE);
    g_return_val_if_fail(boxed->array != NULL, FALSE);
    g_array_append_val(boxed->array, val);
    // g_return_val_if_fail(object != NULL, FALSE);
    // g_return_val_if_fail(G_IS_OBJECT(object), FALSE);
    // boxed->datas = g_slist_append(boxed->datas, g_object_ref(object));
    return boxed->array->len - 1;
}

GArray *ArrayBoxedGetDatas(ArrayBoxed *boxed)
{
    g_return_val_if_fail(boxed != NULL, NULL);
    return boxed->array;
}

struct _LarIntgrecClass
{
    GObjectClass parent_class;
};

struct _LarIntgrec
{
    GObject parent_instance;
    gdouble zero;
    gint startZero;
    gint stopZero;

    gint run;
    gint start;
    gint stop;
    gdouble interval;
    gdouble result;
    ArrayBoxed *data;
    LarIntegration *parameter;

    gboolean isCalculated;
};

G_DEFINE_TYPE(LarIntgrec, larintgrec, G_TYPE_OBJECT)

static void larintgrec_init(LarIntgrec *larintgrec)
{
    larintgrec->zero = 0.0;
    larintgrec->result = 0.0;
    larintgrec->start = 0;
    larintgrec->stop = 0;
    larintgrec->data = NULL;
    larintgrec->parameter = NULL;
    larintgrec->interval = 1.0;
    larintgrec->isCalculated = FALSE;
    /* TODO: Add initialization code here */
}

static void larintgrec_finalize(GObject *object)
{
    LarIntgrec *irec = LAR_INTGREC(object);
    if (irec->data != NULL)
        ArrayBoxedFree(irec->data);
    if (irec->parameter != NULL)
        g_object_unref(irec->parameter);
    G_OBJECT_CLASS(larintgrec_parent_class)->finalize(object);
}

static void larintgrec_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_LAR_INTGREC(object));
    LarIntgrec *larintgrec = LAR_INTGREC(object);
    switch (prop_id)
    {
    case PROP_ZERO:
        larintgrec->zero = g_value_get_double(value);
        break;
    case PROP_ZEROSTART:
        larintgrec->startZero = g_value_get_int(value);
        break;
    case PROP_ZEROSTOP:
        larintgrec->stopZero = g_value_get_int(value);
        break;
    case PROP_RUN:
        larintgrec->run = g_value_get_int(value);
        break;
    case PROP_START:
        larintgrec->start = g_value_get_int(value);
        break;
    case PROP_STOP:
        larintgrec->stop = g_value_get_int(value);
        break;
    case PROP_DATAS:
        if (larintgrec->data)
            ArrayBoxedFree(larintgrec->data);
        larintgrec->data = ArrayBoxedCopy(g_value_get_boxed(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void larintgrec_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(IS_LAR_INTGREC(object));
    LarIntgrec *larintgrec = LAR_INTGREC(object);
    switch (prop_id)
    {
    case PROP_ZERO:
        g_value_set_double(value, larintgrec->zero);
        break;
    case PROP_ZEROSTART:
        g_value_set_int(value, larintgrec->startZero);
        break;
    case PROP_ZEROSTOP:
        g_value_set_int(value, larintgrec->stopZero);
        break;
    case PROP_RUN:
        g_value_set_int(value, larintgrec->run);
        break;
    case PROP_START:
        g_value_set_int(value, larintgrec->start);
        break;
    case PROP_STOP:
        g_value_set_int(value, larintgrec->stop);
        break;
    case PROP_DATAS:
        g_value_set_boxed(value, larintgrec->data);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

static void larintgrec_class_init(LarIntgrecClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->finalize = larintgrec_finalize;
    object_class->set_property = larintgrec_set_property;
    object_class->get_property = larintgrec_get_property;

    g_object_class_install_property(
        object_class, PROP_ZEROSTART, g_param_spec_int("zerostart", "last zero pos", "last zero pos", 0, G_MAXINT, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));

    g_object_class_install_property(
        object_class, PROP_ZEROSTOP, g_param_spec_int("zerostop", "last zero pos", "last zero pos", 0, G_MAXINT, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_RUN, g_param_spec_int("runpos", "run integration pos", "run integration pos", 0, G_MAXINT, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_START, g_param_spec_int("startpos", "Start integration position", "Start integration position", 0, G_MAXINT, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(
        object_class, PROP_STOP, g_param_spec_int("stoppos", "Stop integration position", "Stop integration position", 0, G_MAXINT, 0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_ZERO, g_param_spec_double("zero", "last zero", "last zero", -1.0, 1.0, 0.0, G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
    g_object_class_install_property(object_class, PROP_DATAS, g_param_spec_boxed("data", "signal data ", "Set|Get signal data", ARRAY_TYPE_BOXED, G_PARAM_READABLE | G_PARAM_WRITABLE));
}

LarIntgrec *LarIntgrecNew(LarIntegration *integration)
{
    g_return_val_if_fail(integration != NULL, NULL);
    g_return_val_if_fail(IS_LAR_INTEGRATION(integration), NULL);
    LarIntgrec *rec = LAR_INTGREC(g_object_new(TYPE_LAR_INTGREC, NULL));
    rec->parameter = g_object_ref(integration);
    return rec;
}

gint m_LarIntgrecGetStartZero(LarIntgrec *larintgrec)
{
    g_return_val_if_fail(larintgrec != NULL, 0);
    g_return_val_if_fail(IS_LAR_INTGREC(larintgrec), 0);
    return LAR_INTGREC(larintgrec)->startZero;
}
gint m_LarIntgrecGetStopZero(LarIntgrec *larintgrec)
{
    g_return_val_if_fail(larintgrec != NULL, 0);
    g_return_val_if_fail(IS_LAR_INTGREC(larintgrec), 0);
    return LAR_INTGREC(larintgrec)->stopZero;
}
gint m_LarIntgrecGetRunIngPos(LarIntgrec *larintgrec)
{
    g_return_val_if_fail(larintgrec != NULL, 0);
    g_return_val_if_fail(IS_LAR_INTGREC(larintgrec), 0);
    return LAR_INTGREC(larintgrec)->run;
}
gint m_LarIntgrecGetStart(LarIntgrec *larintgrec)
{
    g_return_val_if_fail(larintgrec != NULL, 0);
    g_return_val_if_fail(IS_LAR_INTGREC(larintgrec), 0);
    return LAR_INTGREC(larintgrec)->start;
}
gint m_LarIntgrecGetStop(LarIntgrec *larintgrec)
{
    g_return_val_if_fail(larintgrec != NULL, 0.);
    g_return_val_if_fail(IS_LAR_INTGREC(larintgrec), 0);
    return LAR_INTGREC(larintgrec)->stop;
}

gdouble m_LarIntgrecGetZero(LarIntgrec *larintgrec)
{
    g_return_val_if_fail(larintgrec != NULL, 0.);
    g_return_val_if_fail(IS_LAR_INTGREC(larintgrec), 0.);
    return LAR_INTGREC(larintgrec)->zero;
}

gdouble m_LarIntgrecGetResult(LarIntgrec *larintgrec)
{
    g_return_val_if_fail(larintgrec != NULL, 0.);
    g_return_val_if_fail(IS_LAR_INTGREC(larintgrec), 0.);
    return larintgrec->result;
}
void m_LarIntgrecRunIntegration(LarIntgrec *larintgrec)
{
    g_return_if_fail(larintgrec != NULL);
    g_return_if_fail(IS_LAR_INTGREC(larintgrec));
    g_return_if_fail(larintgrec->data != NULL);
    GArray *array = ArrayBoxedGetDatas(larintgrec->data);
    larintgrec->run = array->len + 1;
}
void m_LarIntgrecStartIntegration(LarIntgrec *larintgrec)
{
    g_return_if_fail(larintgrec != NULL);
    g_return_if_fail(IS_LAR_INTGREC(larintgrec));
    g_return_if_fail(larintgrec->data != NULL);
    GArray *array = ArrayBoxedGetDatas(larintgrec->data);
    larintgrec->start = array->len;
}

void m_LarIntgrecStopIntegration(LarIntgrec *larintgrec)
{
    g_return_if_fail(larintgrec != NULL);
    g_return_if_fail(IS_LAR_INTGREC(larintgrec));
    g_return_if_fail(larintgrec->data != NULL);
    GArray *array = ArrayBoxedGetDatas(larintgrec->data);
    larintgrec->stop = array->len;
}

gdouble m_LarIntgrecCalculateIntegration(LarIntgrec *larintgrec)
{
    g_return_val_if_fail(larintgrec != NULL, 0.);
    g_return_val_if_fail(larintgrec->data != NULL, 0.);
    g_return_val_if_fail(IS_LAR_INTGREC(larintgrec), 0.);
    g_return_val_if_fail(larintgrec->start > 0 && larintgrec->stop > 0 && larintgrec->stop > larintgrec->start, 0.);
    GArray *array = ArrayBoxedGetDatas(larintgrec->data);
    gint i = 0;
    gint ip = 0;
    gdouble area = 0.0;
    for (i = 0; i < array->len; i++)
    {
        if (i >= larintgrec->start && i <= larintgrec->stop)
        {
            ip++;
            gdouble sig = g_array_index(array, gdouble, i) - larintgrec->zero;
            if (sig < 0.0)
                sig = 0.0;
            area += (sig)*larintgrec->interval;
        }
    }
    if (ip > 0)
    {
        larintgrec->isCalculated = TRUE;
        larintgrec->result = area;
    }
    return area;
}

void m_LarIntgrecStartLastZero(LarIntgrec *larintgrec)
{
    g_return_if_fail(larintgrec != NULL);
    g_return_if_fail(IS_LAR_INTGREC(larintgrec));
    larintgrec->startZero = 0;
    if (larintgrec->data != NULL)
    {
        GArray *array = ArrayBoxedGetDatas(larintgrec->data);
        larintgrec->startZero = array->len + 1;
    }
}
void m_LarIntgrecStopLastZero(LarIntgrec *larintgrec)
{
    g_return_if_fail(larintgrec != NULL);
    g_return_if_fail(IS_LAR_INTGREC(larintgrec));
    g_return_if_fail(larintgrec->data != NULL);
    GArray *array = ArrayBoxedGetDatas(larintgrec->data);
    larintgrec->stopZero = array->len;
}

void m_LarIntgrecCalculateZero(LarIntgrec *larintgrec)
{
    g_return_if_fail(larintgrec != NULL);
    g_return_if_fail(IS_LAR_INTGREC(larintgrec));
    g_return_if_fail(larintgrec->data != NULL);

    GArray *array = ArrayBoxedGetDatas(larintgrec->data);
    gint i = 0;
    gdouble points = 0.0;
    larintgrec->zero = 0.0;
    for (i = 0; i < array->len; i++)
    {
        if (i >= larintgrec->startZero && i <= larintgrec->stopZero)
        {
            gdouble sig = g_array_index(array, gdouble, i);
            if (sig < 0.0)
                sig = 0.0;
            larintgrec->zero += sig;
            points += 1.0;
        }
    }
    if (points > 1.0)
        larintgrec->zero = larintgrec->zero / (points - 1.0);
}

GArray *m_LarIntgrecGetData(LarIntgrec *larintgrec)
{
    g_return_val_if_fail(larintgrec != NULL, NULL);
    g_return_val_if_fail(IS_LAR_INTGREC(larintgrec), NULL);
    g_return_val_if_fail(larintgrec->data != NULL, NULL);
    return ArrayBoxedGetDatas(larintgrec->data);
}

gint m_LarIntgrecAddValue(LarIntgrec *larintgrec, gdouble value)
{
    g_return_val_if_fail(larintgrec != NULL, FALSE);
    g_return_val_if_fail(IS_LAR_INTGREC(larintgrec), FALSE);
    if (larintgrec->data == NULL)
    {
        larintgrec->data = ArrayBoxedNew();
    }
    ArrayBoxedAddValue(larintgrec->data, value);
    return TRUE;
}

void m_LarIntgrecSetPar(LarIntgrec *larintgrec, LarIntegration *integration)
{
    g_return_if_fail(larintgrec != NULL);
    g_return_if_fail(IS_LAR_INTGREC(larintgrec));
    if (larintgrec->parameter)
        g_object_unref(larintgrec->parameter);
    larintgrec->parameter = g_object_ref(integration);
}
LarIntegration *m_LarIntgrecGetPar(LarIntgrec *larintgrec)
{
    g_return_val_if_fail(larintgrec != NULL, FALSE);
    g_return_val_if_fail(IS_LAR_INTGREC(larintgrec), FALSE);
    return larintgrec->parameter;
}

/** @} */
