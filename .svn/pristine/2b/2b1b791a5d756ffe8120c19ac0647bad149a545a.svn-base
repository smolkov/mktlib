/**
 * @defgroup MktBusLibrary
 * @defgroup MktSelection
 * @ingroup  MktSelection
 * @{
 * @file  mkt-selection.h	Selection object header
 * @brief   Selection object header file.
 *
 * @copyright	 Copyright (C) LAR 2015
 *
 * @author       A.Smolkov <asmolkov@lar.com>
 *
 * $Id: $ $URL: $
 */

#ifndef _MKT_SELECTION_H_
#define _MKT_SELECTION_H_

#include <glib-object.h>
#include "mkt-connection.h"

G_BEGIN_DECLS

#define MKT_TYPE_SELECTION             (mkt_selection_get_type ())
#define MKT_SELECTION(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_SELECTION, MktSelection))
#define MKT_SELECTION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), MKT_TYPE_SELECTION, MktSelectionClass))
#define MKT_IS_SELECTION(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_SELECTION))
#define MKT_IS_SELECTION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), MKT_TYPE_SELECTION))
#define MKT_SELECTION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), MKT_TYPE_SELECTION, MktSelectionClass))

typedef struct _MktSelectionClass       MktSelectionClass;
typedef struct _MktSelection            MktSelection;
typedef struct _MktSelectionPrivate     MktSelectionPrivate;


struct _MktSelectionClass
{
	GObjectClass    parent_class;
};

struct _MktSelection
{
	GObject parent_instance;
	MktSelectionPrivate *priv;
};

GType                      mkt_selection_get_type        ( void ) G_GNUC_CONST;

typedef void             (*SelectionReadyCallback)           ( GSList *models,  gpointer user_data );


GType                      mkt_selection_object_type         ( MktSelection *selection );
const gchar*               mkt_selection_sql                 ( MktSelection *selection );
void                       mkt_selection_add_connection      ( MktSelection *selection, MktConnection *connection );
MktConnection*             mkt_selection_get_connection      ( MktSelection *selection );
void                       mkt_selection_add_ready_callback  ( MktSelection *selection, SelectionReadyCallback callback );
SelectionReadyCallback     mkt_selection_get_ready_callback  ( MktSelection *selection );
void                       mkt_selection_add_user_data       ( MktSelection *selection, gpointer user_data );
gboolean                   mkt_selection_run                 ( MktSelection *selection );
gboolean                   mkt_selection_run_no_return       ( MktSelection *selection );
gboolean                   mkt_selection_run_callback        ( MktSelection *selection );



G_END_DECLS

#endif /* _MKT_SELECTION_H_ */

/** @} */
