/**
 * @file  mkt-model.h	Data base model header
 * @brief This is data base root object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef __MKT_MODELL_H__
#define __MKT_MODELL_H__

#include <glib.h>
#include <gio/gio.h>
#include "mkt-connection.h"
#include "mkt-decl.h"
#include "mkt-selection.h"

G_BEGIN_DECLS

#define MKT_TYPE_IMODEL (mkt_imodel_get_type())
#define MKT_IMODEL(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), MKT_TYPE_IMODEL, MktIModel))
#define MKT_IS_IMODEL(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), MKT_TYPE_IMODEL))
#define MKT_IMODEL_GET_INTERFACE(inst) (G_TYPE_INSTANCE_GET_INTERFACE((inst), MKT_TYPE_IMODEL, MktIModelInterface))

typedef struct _MktIModel          MktIModel;
typedef struct _MktIModelInterface MktIModelInterface;

struct _MktIModelInterface {
    GTypeInterface parent_iface;
    guint64 (*ref_id)(MktIModel *self);
};

GType   mkt_imodel_get_type(void);
guint64 mkt_model_ref_id(MktIModel *model);

#define MKT_TYPE_MODEL (mkt_model_get_type())
#define MKT_MODEL(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), MKT_TYPE_MODEL, MktModel))
#define MKT_MODEL_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), MKT_TYPE_MODEL, MktModelClass))
#define MKT_IS_MODEL(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), MKT_TYPE_MODEL))
#define MKT_IS_MODEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), MKT_TYPE_MODEL))
#define MKT_MODEL_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), MKT_TYPE_MODEL, MktModelClass))

typedef struct _MktModelClass   MktModelClass;
typedef struct _MktModel        MktModel;
typedef struct _MktModelPrivate MktModelPrivate;

typedef enum {
    MKT_MODEL_PK_PROP          = 1 << 10,
    MKT_MODEL_DB_PROP          = 1 << 11,
    MKT_MODEL_DB_NO_NULL       = 1 << 12,
    MKT_MODEL_DB_NOT_REDABLE   = 1 << 13,
    MKT_MODEL_DB_NOT_WRITABLE  = 1 << 14,
    MKT_MODEL_DB_TIMESTAMP     = 1 << 15,
    MKT_MODEL_DB_AUTOINCREMENT = 1 << 16,
    MKT_MODEL_DB_NO_SYNC       = 1 << 17,
    MKT_MODEL_DB_REFERENCE     = 1 << 18,
    MKT_MODEL_DB_LOG_CHANGE    = 1 << 19,
    MKT_MODEL_OBJECT_PROP      = 1 << 20,

} MktModelParamFlags;

struct _MktModelClass {
    GObjectClass parent_class;
    void (*changed)(MktModel *self);
    void (*delete)(MktModel *self);
    void (*initialized)(MktModel *self);
};

struct _MktModel {
    GObject          parent_instance;
    MktModelPrivate *priv;
};

typedef void (*MktModelAsyncCallback)(GSList *models, gpointer user_data);
typedef void (*MktRunedAsyncCallback)(GType type, guint64 id, gpointer user_data);

typedef void (*MktModelSignalCallback)(MktModel *model, gpointer user_data);

GType mkt_model_get_type(void) G_GNUC_CONST;

void mkt_model_create(GType type);
void mkt_model_create_full(GType type, MktConnection *connection);

void           mkt_model_register(GType type);
MktModel *     mkt_model_new(GType object_type, const gchar *first_property_name, ...);
void           mkt_model_set_database(MktModel *model, MktConnection *connection);
MktConnection *mkt_model_database(MktModel *model);

gchar *mkt_model_dub_dbreference(MktModel *model);

gboolean mkt_model_connect_async_init();

// Die 'GSList *data' die in 'MktRunedAsyncCallback callback' .

void     mkt_model_query_async(GType type, GCancellable *cancellable, GAsyncReadyCallback callback, gpointer user_data, const gchar *format, ...);
GSList * mkt_model_query_finish(GObject *source_object, GAsyncResult *result, GError **error);
void     mkt_model_exec_async(GType type, GCancellable *cancellable, GAsyncReadyCallback callback, gpointer user_data, const gchar *format, ...);
gboolean mkt_model_exec_finish(GObject *source_object, GAsyncResult *result, GError **error);

gboolean mkt_model_runs_async(GType iface, guint64 id, MktRunedAsyncCallback callback, gpointer user_data);
gboolean mkt_model_select_async(GType type, MktModelAsyncCallback callback, gpointer user_data, const gchar *format, ...) G_GNUC_PRINTF(4, 5);
gboolean mkt_model_connect_signal(GType model_type, guint64 ref_id, const gchar *property, MktModelSignalCallback callback, gpointer user_data);
void     mkt_model_delete_table(GType type);

void     mkt_model_copy_param(MktModel *src, MktModel *dist);
gboolean mkt_model_save(MktModel *model);
gboolean mkt_model_replace(MktModel *model, const gchar *first_property_name, ...);
gboolean mkt_model_reload(MktModel *model);
gboolean mkt_model_sync_run(MktModel *model);
gboolean mkt_model_delete(MktModel *model);
void     mkt_model_unref_and_delete(gpointer object);
void     mkt_model_set_data_model(MktModel *model, MktDataModel *datamodel);
GSList * mkt_model_select(GType type, const gchar *format, ...) G_GNUC_PRINTF(2, 3);
gboolean mkt_model_exec(GType type, const gchar *format, ...) G_GNUC_PRINTF(2, 3);
// GSList*             mkt_model_select_local       ( GType type ,const gchar
// *format,...)G_GNUC_PRINTF (2, 3);

MktModel *mkt_model_select_one(GType type, const gchar *format, ...) G_GNUC_PRINTF(2, 3);
MktModel *mkt_model_select_foreign_key(GType type, MktModel *foreign_key);
MktModel *mkt_model_find_one(GType type, const gchar *format, ...) G_GNUC_PRINTF(2, 3);

gchar *mkt_model_serialize_object(MktModel *model, gint ab);
gchar *mkt_model_serialize(GType type, const gchar *format, ...) G_GNUC_PRINTF(2, 3);
gchar *mkt_model_serialize_one(MktModel *model);

gboolean mkt_model_bind(MktModel *head, MktModel *client);

GType       mkt_model_find_type(GType iface);
GParamSpec *mkt_model_find_pspec(GType model_type, const gchar *pname);
GList *     mkt_model_list_pspec(GType model_type);
guint       mkt_model_nth_pspec(GType model_type);
gboolean    mkt_model_is_constructed(MktModel *model);

MktModel *mkt_model_select_parser(const gchar *wid);

void mkt_model_freeze_notify(MktModel *model);
void mkt_model_thaw_notify(MktModel *model);

void          mkt_model_print_stdout(MktModel *model);
const GValue *mkt_model_get_typed_value(MktModel *model, const gchar *prop);

const gchar *mkt_model_primary_key(GType type);

// void                mkt_model_init_all_default   ( void );
GList *       mkt_model_list_datamodels(GType type);
MktDataModel *mkt_model_lookup_collectors(const gchar *name);
const GValue *mkt_model_get_property_value(MktModel *model, const gchar *prop);

gboolean mkt_model_look(GType type, GCancellable *cancellable, SelectionReadyCallback callback, gpointer user_data, const gchar *format, ...) G_GNUC_PRINTF(5, 6);
gboolean mkt_model_delete_async(GType type, GCancellable *cancellable, SelectionReadyCallback callback, gpointer user_data, const gchar *format, ...) G_GNUC_PRINTF(5, 6);

void mkt_model_write(MktModel *model);

void mkt_model_checkpoint_all();
#define MKT_CREATE_MODEL() \
    { mkt_model_create(g_define_type_id); }
#define MKT_CREATE_MODEL_FULL(connection) \
    { mkt_model_create_full(g_define_type_id, connection); }

#define mktMPGet(model, prop, getter) (g_value_get_##getter(mkt_model_get_property_value(MKT_MODEL(model), (const gchar *)prop)))
#define mktMPSet(model, prop, val) ((model != NULL && MKT_IS_MODEL(model) && prop != NULL) ? g_object_set(G_OBJECT(model), prop, val, NULL) : NULL)

G_END_DECLS

#endif /* __MKT_MODEL_H__ */
/** @} */
