/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * larflags.h
 * Copyright (C) LAR 2017
 *

 */



#include <glib-object.h>

#ifndef _LAR_MEASUREMENT_H_
#define _LAR_MEASUREMENT_H_

/**
 * ELarProcessFlags:
 * @LAR_STATE_ONLINE: Onlime measurement state
 * @LAR_STATE_ONLINE: Single measurement state
 * @LAR_MEAS_CHECK_STATE:  Check measurement state
 *
 * Enum with the available views.
 */

typedef enum /*< flags,prefix=Lar >*/
{
    LAR_STATE_INIT                   = 0,    /*<nick=init>*/
    LAR_STATE_OFFLINE                = 1<<1, /*<nick=offline>*/
    LAR_STATE_ERROR                  = 1<<3, /*<nick=error>*/
    LAR_STATE_ONLINE                 = 1<<5, /*<nick=online>*/
    LAR_STATE_DEBUG                  = 1<<6,  /*<nick=debug>*/ 
    LAR_STATE_MEAS                   = 1<<11, /*<nick=single>*/
    LAR_STATE_CAL                    = 1<<12, /*<nick=cal>*/ 
    LAR_STATE_CHECK                  = 1<<13, /*<nick=check>*/

    LAR_STATE_ST1                    = 1<<21,/*<nick=stream1>*/      
    LAR_STATE_ST2                    = 1<<22,/*<nick=stream2>*/      
    LAR_STATE_ST3                    = 1<<23,/*<nick=stream3>*/      
    LAR_STATE_ST4                    = 1<<24,/*<nick=stream4>*/      
    LAR_STATE_ST5                    = 1<<25,/*<nick=stream5>*/      
    LAR_STATE_ST6                    = 1<<26,/*<nick=stream6>*/      
} LarProcessFlags;

#define LAR_PROCESS_FLAGS_TYPE	(lar_process_flags_get_type ())
GType lar_process_flags_get_type (void) G_GNUC_CONST;




#endif
