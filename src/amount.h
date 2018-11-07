/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * mkt-sensor-data.c
 * Copyright (C) sascha 2013 <sascha@sascha-VirtualBox>
 *
mkt-sensor-data.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mkt-sensor-data.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _AMOUNT_H_
#define _AMOUNT_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define TYPE_AMOUNT (amount_get_type())
#define AMOUNT(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_AMOUNT, Amount))
#define AMOUNT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_AMOUNT, AmountClass))
#define IS_AMOUNT(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), TYPE_AMOUNT))
#define IS_AMOUNT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_AMOUNT))
#define AMOUNT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), TYPE_AMOUNT, AmountClass))

typedef struct _AmountClass   AmountClass;
typedef struct _Amount        Amount;

GType amount_get_type(void) G_GNUC_CONST;


guint AmountGetCounter(Amount *amount);
gdouble AmountGetPercentage(Amount *amount);

G_END_DECLS

#endif /* _AMOUNT_H_ */
