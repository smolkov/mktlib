/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup DbLibrary
 * @defgroup DbModel
 * @ingroup  DbModel
 * @{
 * @file  db-model.h	Data base model header
 * @brief This is data base root object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef __DB_MODEL_H__
#define __DB_MODEL_H__

#include <glib-object.h>

#include <gio/gio.h>
#include <glib.h>
#include <libgda/libgda.h>
#include <sql-parser/gda-sql-parser.h>

G_BEGIN_DECLS

#define DB_TYPE_IMODEL (db_imodel_get_type())
#define DB_IMODEL(obj)                                                         \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), DB_TYPE_IMODEL, DbIModel))
#define DB_IS_IMODEL(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), DB_TYPE_IMODEL))
#define DB_IMODEL_GET_INTERFACE(inst)                                          \
  (G_TYPE_INSTANCE_GET_INTERFACE((inst), DB_TYPE_IMODEL, DbIModelInterface))

typedef struct _DbIModel DbIModel;
typedef struct _DbIModelInterface DbIModelInterface;

struct _DbIModelInterface {
  GTypeInterface parent_iface;
};

GType db_imodel_get_type(void);

#define DB_TYPE_MODEL (db_model_get_type())
#define DB_MODEL(obj)                                                          \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), DB_TYPE_MODEL, DbModel))
#define DB_MODEL_CLASS(klass)                                                  \
  (G_TYPE_CHECK_CLASS_CAST((klass), DB_TYPE_MODEL, DbModelClass))
#define DB_IS_MODEL(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), DB_TYPE_MODEL))
#define DB_IS_MODEL_CLASS(klass)                                               \
  (G_TYPE_CHECK_CLASS_TYPE((klass), DB_TYPE_MODEL))
#define DB_MODEL_GET_CLASS(obj)                                                \
  (G_TYPE_INSTANCE_GET_CLASS((obj), DB_TYPE_MODEL, DbModelClass))

typedef struct _DbModelClass DbModelClass;
typedef struct _DbModel DbModel;
typedef struct _DbModelPrivate DbModelPrivate;

typedef enum {
  DB_MODEL_PK_PROP = 1 << 10,
  DB_MODEL_DB_PROP = 1 << 11,
  DB_MODEL_DB_NO_NULL = 1 << 12,
  DB_MODEL_DB_NOT_REDABLE = 1 << 13,
  DB_MODEL_DB_NOT_WRITABLE = 1 << 14,
  DB_MODEL_DB_TIMESTAMP = 1 << 15,
  DB_MODEL_DB_AUTOINCREMENT = 1 << 16,
  DB_MODEL_DB_NO_SYNC = 1 << 17,
  DB_MODEL_DB_REFERENCE = 1 << 18,
  DB_MODEL_DB_LOG_CHANGE = 1 << 19,
  DB_MODEL_OBJECT_PROP = 1 << 20,

} DbModelParamFlags;

struct _DbModelClass {
  GObjectClass parent_class;
};

struct _DbModel {
  GObject parent_instance;
  DbModelPrivate *priv;
};

G_END_DECLS

#endif /* __DB_MODEL_H__ */
/** @} */
