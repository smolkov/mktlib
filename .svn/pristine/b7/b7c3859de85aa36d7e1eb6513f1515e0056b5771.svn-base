/*
 * @ingroup MktSelection
 * @{
 * @file  mkt-operation-group.c	Selection  object
 * @brief This is Selection object description.
 *
 *
 * @copyright  Copyright (C) LAR  2015
 *
 * @author A.Smolkov <asmolkov@lar.com>
 *
 *
 */



#include <gio/gio.h>
#include "mkt-selection.h"
#include "mkt-model.h"

#include "../config.h"
#include <glib/gi18n-lib.h>


struct _MktSelectionPrivate
{
	gchar                   *sql;
	GType                    gtype;
	MktConnection           *connection;
	SelectionReadyCallback   callback;
	GSList                  *selection_models;
	gpointer                 user_data;
};

enum{
	SELECTION_PROP0,
	SELECTION_SQL,
	SELECTION_TYPE
};




G_DEFINE_TYPE_WITH_PRIVATE (MktSelection, mkt_selection, G_TYPE_OBJECT);

static void
mkt_selection_init (MktSelection *mkt_selection)
{
	mkt_selection->priv                   = mkt_selection_get_instance_private(mkt_selection);
	mkt_selection->priv->sql              = NULL;
	mkt_selection->priv->connection       = NULL;
	mkt_selection->priv->selection_models = NULL;

	/* TODO: Add initialization code here */
}

static void
mkt_selection_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */
	MktSelection *selection = MKT_SELECTION(object);
	if(selection->priv->sql)                  g_free(selection->priv->sql);
	if(selection->priv->connection)           g_object_unref(selection->priv->connection);
	if(selection->priv->selection_models)     g_slist_free_full(selection->priv->selection_models,g_object_unref);
	G_OBJECT_CLASS (mkt_selection_parent_class)->finalize (object);
}

static void
mkt_selection_set_property(  GObject        *object,
                             guint           prop_id,
                             const GValue   *value,
                             GParamSpec     *pspec)
{
	MktSelection *selection = MKT_SELECTION( object );
	//TEST:	g_debug("atom %s  property %s %d",mkt_atom_get_id(atom),pspec->name,prop_id);
	switch(prop_id)
	{
	case SELECTION_SQL:
		if(selection->priv->sql)g_free(selection->priv->sql);
		selection->priv->sql =  g_value_dup_string(value);
		break;
	case SELECTION_TYPE:
		selection->priv->gtype =  g_value_get_gtype(value);
		break;

	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object,prop_id,pspec);
		break;
	}
}

static void
mkt_selection_get_property(GObject        *object,
		guint           prop_id,
		GValue   *value,
		GParamSpec     *pspec)
{
	MktSelection *selection = MKT_SELECTION( object );
	switch(prop_id)
	{
	case SELECTION_SQL:
		g_value_set_string(value,selection->priv->sql);
		break;
	case SELECTION_TYPE:
		g_value_set_gtype(value,selection->priv->gtype);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object,prop_id,pspec);
		break;
	}
}



static void
mkt_selection_class_init (MktSelectionClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);

	object_class->set_property = mkt_selection_set_property;
	object_class->get_property = mkt_selection_get_property;
	object_class->finalize     = mkt_selection_finalize;


	GParamSpec  *pspec;
	pspec = g_param_spec_string ( "selection-sql",
			"Selection prop",
			"Selection",
			"None",
			G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE );
	g_object_class_install_property(object_class,
			SELECTION_SQL,
			pspec);

	pspec = g_param_spec_gtype( "selection-gtype",
			"Selection prop",
			"Selection name",
			MKT_TYPE_MODEL,
			G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE );
	g_object_class_install_property(object_class,
			SELECTION_TYPE,
			pspec);

}



const gchar*
mkt_selection_sql      ( MktSelection *selection )
{
	g_return_val_if_fail(selection!=NULL,NULL);
	return selection->priv->sql;
}

GType
mkt_selection_object_type     ( MktSelection *selection )
{
	g_return_val_if_fail(selection!=NULL,G_TYPE_INVALID);
	return selection->priv->gtype;
}

void
mkt_selection_add_connection  ( MktSelection *selection, MktConnection *connection )
{
	g_return_if_fail(selection!=NULL);
	if(selection->priv->connection) g_object_unref(selection->priv->connection);
	selection->priv->connection = MKT_CONNECTION(g_object_ref(connection));
}

MktConnection*
mkt_selection_get_connection  ( MktSelection *selection )
{
	g_return_val_if_fail(selection!=NULL,NULL);
	return selection->priv->connection;
}

void
mkt_selection_add_ready_callback  ( MktSelection *selection, SelectionReadyCallback callback )
{
	g_return_if_fail(selection!=NULL);
	selection->priv->callback = callback;
}

SelectionReadyCallback
mkt_selection_get_ready_callback  ( MktSelection *selection )
{
	g_return_val_if_fail(selection!=NULL,NULL);
	return selection->priv->callback;
}

void
mkt_selection_add_user_data       ( MktSelection *selection , gpointer user_data )
{
	g_return_if_fail(selection!=NULL);
	selection->priv->user_data = user_data;
}

gboolean
mkt_selection_run_no_return ( MktSelection *selection )
{

	if(selection->priv->selection_models)g_slist_free_full(selection->priv->selection_models,g_object_unref);
	selection->priv->selection_models = NULL;
	mkt_connection_send_no_select(mkt_selection_get_connection(selection),mkt_selection_sql(selection));
	return TRUE;
}

gboolean
mkt_selection_run ( MktSelection *selection )
{
	g_return_val_if_fail(selection!=NULL,FALSE);
	if(selection->priv->selection_models)g_slist_free_full(selection->priv->selection_models,g_object_unref);
	selection->priv->selection_models = mkt_connection_send(mkt_selection_get_connection(selection),
				                                        mkt_selection_object_type(selection),
														mkt_selection_sql(selection));

	return TRUE;
}
gboolean
mkt_selection_run_callback      ( MktSelection *selection )
{
	g_return_val_if_fail(selection!=NULL,FALSE);
	if(selection->priv->callback)
	{
		selection->priv->callback(selection->priv->selection_models,selection->priv->user_data);
		return TRUE;
	}
	return FALSE;
}




/** @} */
