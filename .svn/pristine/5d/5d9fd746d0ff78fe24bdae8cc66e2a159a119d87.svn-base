/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * @defgroup MktLibrary
 * @defgroup MktStatistic
 * @ingroup  MktStatistic
 * @{
 * @file  mkt-statistic.h	Statistic interface model header
 * @brief This is Statistic interface model object header file.
 * 	 Copyright (C) LAR 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 * $Id: $ $URL: $
 */

#ifndef MKT_STATISTIC_H_
#define MKT_STATISTIC_H_


#include "mkt-model.h"

G_BEGIN_DECLS

#define MKT_TYPE_STATISTIC                  (mkt_statistic_get_type ())
#define MKT_STATISTIC(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj),MKT_TYPE_STATISTIC, MktStatistic))
#define MKT_IS_STATISTIC(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj),MKT_TYPE_STATISTIC))
#define MKT_STATISTIC_GET_INTERFACE(inst)   (G_TYPE_INSTANCE_GET_INTERFACE ((inst), MKT_TYPE_STATISTIC, MktStatisticInterface))


typedef struct _MktStatisticInterface MktStatisticInterface;
typedef struct _MktStatistic          MktStatistic;



struct _MktStatisticInterface
{
	GTypeInterface             parent_iface;
	guint                    (*statistic_replicates)                   ( MktStatistic *self );
	guint                    (*statistic_outliers)                     ( MktStatistic *self );
	gdouble                  (*statistic_max_cv)                       ( MktStatistic *self );
	gdouble                  (*statistic_threshold)                    ( MktStatistic *self );
};



GType                    mkt_statistic_get_type                        (void) G_GNUC_CONST;
guint                    mkt_statistic_replicates                      ( MktStatistic *statistic );
guint                    mkt_statistic_outliers                        ( MktStatistic *statistic );
gdouble                  mkt_statistic_max_cv                          ( MktStatistic *statistic );
gdouble                  mkt_statistic_threshold                       ( MktStatistic *statistic );







G_END_DECLS

#endif /* MKT_STATISTIC_H_ */

/** @} */
