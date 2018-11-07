/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktCategory
 * @ingroup  MktCategory
 * @{
 * @file  mkt-category.h	Category interface model header
 * @brief This is Category interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef MKT_CATEGORY_H_
#define MKT_CATEGORY_H_


#include "mkt-model.h"
#include "mkt-decl.h"
#include "mkt-statistic.h"


G_BEGIN_DECLS

#define MKT_TYPE_CATEGORY                  (mkt_category_get_type ())
#define MKT_CATEGORY(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj),MKT_TYPE_CATEGORY, MktCategory))
#define MKT_IS_CATEGORY(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj),MKT_TYPE_CATEGORY))
#define MKT_CATEGORY_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_CATEGORY, MktCategoryInterface))


typedef struct _MktCategoryInterface MktCategoryInterface;
typedef struct _MktCategory          MktCategory;


struct _MktCategoryInterface
{
	GTypeInterface        parent_iface;
	gdouble             (*interval)                                         ( MktCategory *category );

};


GType                    mkt_category_get_type                              (void) G_GNUC_CONST;



gdouble                  mkt_category_interval                              ( MktCategory *category );





G_END_DECLS

#endif /* MKT_CATEGORY_H_ */

/** @} */
