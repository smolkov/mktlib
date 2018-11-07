/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * lar-sensor-data.c
 * Copyright (C) sascha 2013 <sascha@sascha-VirtualBox>
 * 
lar-sensor-data.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * lar-sensor-data.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _LAR_STATISTIC_MODEL_H_
#define _LAR_STATISTIC_MODEL_H_





G_BEGIN_DECLS

#define LAR_TYPE_STATISTIC_MODEL             (lar_statistic_model_get_type ())
#define LAR_STATISTIC_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), LAR_TYPE_STATISTIC_MODEL, LarStatisticModel))
#define LAR_STATISTIC_MODEL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), LAR_TYPE_STATISTIC_MODEL, LarStatisticModelClass))
#define LAR_IS_STATISTIC_MODEL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LAR_TYPE_STATISTIC_MODEL))
#define LAR_IS_STATISTIC_MODEL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), LAR_TYPE_STATISTIC_MODEL))
#define LAR_STATISTIC_MODEL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), LAR_TYPE_STATISTIC_MODEL, LarStatisticModelClass))

typedef struct _LarStatisticModelClass     LarStatisticModelClass;
typedef struct _LarStatisticModel          LarStatisticModel;
typedef struct _LarStatisticModelPrivate   LarStatisticModelPrivate;


struct _LarStatisticModelClass
{
	LarParamObjectClass parent_class;

};

struct _LarStatisticModel
{
	LarParamObject parent_instance;
	LarStatisticModelPrivate *priv;
};

GType                   lar_statistic_model_get_type                 (void) G_GNUC_CONST;

G_END_DECLS

#endif /* _LAR_STATISTIC_MODEL_H_ */
