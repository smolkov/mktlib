/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * larflags.h
 * Copyright (C) LAR 2017
 *
 * 
 */



#include "larflags.h"

GType
lar_process_flags_get_type (void)
{
	static GType the_type = 0;
	
	if (the_type == 0)
	{
		static const GFlagsValue values[] = {
			{ LAR_STATE_INIT,
			  "initialize",
			  "initialize" },
			{ LAR_STATE_OFFLINE,
			  "offline",
			  "offline" },
			{ LAR_STATE_ERROR,
			  "offline",
			  "offline" },
			{ LAR_STATE_ONLINE,
			  "online",
			  "online" },
			{ LAR_STATE_DEBUG,
			  "debug",
			  "debug" },
			{ LAR_STATE_MEAS,
			  "measurement",
			  "single" },
			{ LAR_STATE_CAL,
			  "LAR_STATE_CAL",
			  "cal" },
			{ LAR_STATE_CHECK,
			  "LAR_STATE_CHECK",
			  "check" },
			{ LAR_STATE_ST1,
			  "LAR_STATE_ST1",
			  "stream1" },
			{ LAR_STATE_ST2,
			  "LAR_STATE_ST2",
			  "stream2" },
			{ LAR_STATE_ST3,
			  "LAR_STATE_ST3",
			  "stream3" },
			{ LAR_STATE_ST4,
			  "LAR_STATE_ST4",
			  "stream4" },
			{ LAR_STATE_ST5,
			  "LAR_STATE_ST5",
			  "stream5" },
			{ LAR_STATE_ST6,
			  "LAR_STATE_ST6",
			  "stream6" },
			{ 0, NULL, NULL }
		};
		the_type = g_flags_register_static (
				g_intern_static_string ("ELarMeasState"),
				values);
	}
	return the_type;
}