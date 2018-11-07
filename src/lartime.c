/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * larintgrec.c
 * Copyright (C) LAR 2017
 *
 */


 #include "lartime.h"
 #include <time.h>
 

/**
 * Now time  sec
 * utc time in second
 * @return the seconds
 **/
gdouble NowTimeSec() {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return (t.tv_nsec / 1000000000. + t.tv_sec);
}
