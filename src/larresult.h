/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * result.h
 * Copyright (C) LAR 2017
 *

 */

#ifndef _LAR_RESULT_H_
#define _LAR_RESULT_H_

#include <glib-object.h>

G_BEGIN_DECLS
#define LAR_DATAS_TYPE_BOXED (lar_datas_get_type())

typedef struct _LarDatas LarDatas;

GType lar_datas_get_type(void);

LarDatas *LarDatasNew();
LarDatas *m_LarDatasCopy(const LarDatas *src);
void m_LarDatasFree(LarDatas *boxed);
gint m_LarDatasAddValues(LarDatas *boxed, gdouble x, gdouble y);
GArray *m_LarDatasGetX(LarDatas *boxed);
GArray *m_LarDatasGetY(LarDatas *boxed);
guint m_LarDatasGetLen(LarDatas *boxed);
gdouble m_LarDatasGetMinX(LarDatas *boxed);
gdouble m_LarDatasGetMaxX(LarDatas *boxed);
gdouble m_LarDatasGetMinY(LarDatas *boxed);
gdouble m_LarDatasGetMaxY(LarDatas *boxed);
LarDatas *m_LarDatasConvertStep(const LarDatas *src);
#define TYPE_LAR_RESULT (result_data_get_type())
#define LAR_RESULT(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_LAR_RESULT, LarResult))
#define LAR_RESULT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_LAR_RESULT, LarResultClass))
#define IS_LAR_RESULT(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_LAR_RESULT))
#define IS_LAR_RESULT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_LAR_RESULT))
#define LAR_RESULT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_LAR_RESULT, LarResultClass))

typedef struct _LarResultClass LarResultClass;
typedef struct _LarResult LarResult;
typedef struct _LarResultPrivate LarResultPrivate;

struct _LarResultClass
{
    GObjectClass parent_class;
};

struct _LarResult
{
    GObject parent_instance;
    LarResultPrivate *priv;
};

GType result_data_get_type(void) G_GNUC_CONST;

guint64 m_LarResultGetKey(LarResult *result);
guint64 m_LarResultGetStream(LarResult *result);
guint64 m_LarResultGetChannel(LarResult *result);
guint m_LarResultGetNumber(LarResult *result);
guint64 m_LarResultGetChanged(LarResult *result);
gdouble m_LarResultGetValue(LarResult *result);
gdouble m_LarResultGetCV(LarResult *result);
const gchar *m_LarResultGetParameter(LarResult *result);
const gchar *m_LarResultGetName(LarResult *result);
const gchar *m_LarResultGetUnit(LarResult *result);
const gchar *m_LarResultGetCreated(LarResult *result);

G_END_DECLS

#endif /* _LAR_RESULT_H_ */
