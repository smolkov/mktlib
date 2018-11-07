/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * ltime.c
 * Copyright (C) LAR 2017
 *
 */

#ifndef _LARTIME_H_
#define _LARTIME_H_

#include <glib-object.h>
#include <glib.h>

#define LARTIME_SPAN_DAY                 ((gdouble) (86400.0))
#define LARTIME_SPAN_HOUR                ((gdouble) (3600.0))
#define LARTIME_SPAN_MINUTE              ((gdouble) (60.0))



gdouble NowTimeSec();

#endif /* LTIME_H_ */




