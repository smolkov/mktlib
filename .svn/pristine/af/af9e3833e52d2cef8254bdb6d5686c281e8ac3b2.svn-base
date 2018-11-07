/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktItem
 * @ingroup  MktItem
 * @{
 * @file  mkt-item.h	Item interface model header
 * @brief This is Item interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */
#ifndef MKT_ITEM_H_
#define MKT_ITEM_H_


#include "mkt-model.h"

G_BEGIN_DECLS

#define MKT_TYPE_ITEM                  (mkt_item_get_type ())
#define MKT_ITEM(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj),MKT_TYPE_ITEM, MktItem))
#define MKT_IS_ITEM(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj),MKT_TYPE_ITEM))
#define MKT_ITEM_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_ITEM, MktItemInterface))


typedef struct _MktItemInterface MktItemInterface;
typedef struct _MktItem MktItem;


struct _MktItemInterface
{
	GTypeInterface parent_iface;
	gulong                  (*item_object_id)                      ( MktItem *self );
	const gchar*            (*item_property)                       ( MktItem *self );
	const gchar*            (*item_value)                          ( MktItem *self );
	const gchar*            (*item_value_type)                     ( MktItem *self );
	const gchar*            (*item_object_type)                    ( MktItem *self );
};


GType                   mkt_item_get_type                          (void) G_GNUC_CONST;
gulong                  mkt_item_get_object_id                     ( MktItem *item );
const gchar*            mkt_item_get_object_type                   ( MktItem *item );
const gchar*            mkt_item_get_property                      ( MktItem *item );

const gchar*            mkt_item_get_value                         ( MktItem *item );
const gchar*            mkt_item_get_value_type                    ( MktItem *item );



GValue*                 mkt_item_value                             ( MktItem *item );
gboolean                mkt_item_object_save                       ( MktItem *item );
MktItem*                mkt_item_new_object                        ( MktModel *save , const gchar *property ,const GValue *value );




G_END_DECLS

#endif /* MKT_ITEM_H_ */
