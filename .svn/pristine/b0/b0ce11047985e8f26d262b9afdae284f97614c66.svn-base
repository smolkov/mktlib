/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * @ingroup MktVessel
 * @{
 * @file  mkt-vessel.c	Vessel model interface
 * @brief This is Vessel model interface description.
 *
 *
 *  Copyright (C) A.Smolkov 2013 <asmolkov@lar.com>
 *
 * @author A.Smolkov
 *
 */


#include "mkt-vessel.h"
#include "market-time.h"



#if GLIB_CHECK_VERSION(2,31,7)
static GRecMutex       init_rmutex;
#define MUTEX_LOCK()   g_rec_mutex_lock(&init_rmutex)
#define MUTEX_UNLOCK() g_rec_mutex_unlock(&init_rmutex)
#else
static GStaticRecMutex init_mutex = G_STATIC_REC_MUTEX_INIT;
#define MUTEX_LOCK()   g_static_rec_mutex_lock(&init_mutex)
#define MUTEX_UNLOCK() g_static_rec_mutex_unlock(&init_mutex)
#endif

static void
mkt_vessel_base_init (gpointer g_iface)
{
	static gboolean is_vessel_initialized = FALSE;
	MUTEX_LOCK();
	if (!is_vessel_initialized)
	{

		g_object_interface_install_property (g_iface,
				g_param_spec_uint("vessel-x-pos",
						"X-Axis position",
						"Set get X-Axis position",
						1,16000,1200,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE ));

		g_object_interface_install_property (g_iface,
				g_param_spec_uint("vessel-y-pos",
						"Y-Axis injection position",
						"Set get Y-Axis injection position",
						10,16000,1250,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE ));

		g_object_interface_install_property (g_iface,
				g_param_spec_uint("vessel-y1-pos",
						"Y-Axis needle position",
						"Set get Y-Axis needle position",
						0,16000,0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE ));

		g_object_interface_install_property (g_iface,
				g_param_spec_uint("vessel-y2-pos",
						"Y-Axis 2 position",
						"Set get Y-Axis 2 position",
						0,16000,0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE ));

		g_object_interface_install_property (g_iface,
				g_param_spec_uint("vessel-y3-pos",
						"Y-Axis 3 position",
						"Set get Y-Axis 3 position",
						0,16000,0,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE ));

		g_object_interface_install_property (g_iface,
				g_param_spec_string("vessel-pump",
						"pump ",
						"Set get Pump",
						"/com/lar/tera/pumps/Pump1",
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP ));

		g_object_interface_install_property (g_iface,
				g_param_spec_boolean("vessel-has-motor",
						"with motor",
						"Set get with motor",
						FALSE,
						G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | MKT_MODEL_DB_PROP|MKT_MODEL_DB_LOG_CHANGE ));

		is_vessel_initialized = TRUE;
	}
	MUTEX_UNLOCK();
}

GType
mkt_vessel_get_type (void)
{
	static GType iface_type = 0;
	if (iface_type == 0)
	{
		static const GTypeInfo info = {
				sizeof (MktVesselInterface),
				(GBaseInitFunc) mkt_vessel_base_init,
				(GBaseFinalizeFunc) NULL,
				(GClassInitFunc) NULL,
				NULL,
				NULL,
				0,
				0,
				(GInstanceInitFunc) NULL,
				0
		};
		MUTEX_LOCK();
		if (iface_type == 0)
		{
			iface_type = g_type_register_static (G_TYPE_INTERFACE, "MktVesselInterface",&info, 0);
			g_type_interface_add_prerequisite (iface_type, MKT_TYPE_MODEL);
		}
		MUTEX_UNLOCK();
	}
	return iface_type;
}



guint
mkt_vessel_xpos                            ( MktVessel *vessel )
{
	g_return_val_if_fail(vessel != NULL , 0);
	g_return_val_if_fail(MKT_IS_VESSEL(vessel) , 0);
	if(MKT_VESSEL_GET_INTERFACE(vessel)->vessel_xpos )
		return MKT_VESSEL_GET_INTERFACE(vessel)->vessel_xpos(vessel);
	return 0;
}

guint
mkt_vessel_ypos                            ( MktVessel *vessel )
{
	g_return_val_if_fail(vessel != NULL , 0);
	g_return_val_if_fail(MKT_IS_VESSEL(vessel) , 0);
	if(MKT_VESSEL_GET_INTERFACE(vessel)->vessel_ypos )
		return MKT_VESSEL_GET_INTERFACE(vessel)->vessel_ypos(vessel);
	return 0;
}

guint
mkt_vessel_y1pos                           ( MktVessel *vessel )
{
	g_return_val_if_fail(vessel != NULL , 0);
	g_return_val_if_fail(MKT_IS_VESSEL(vessel) , 0);
	if(MKT_VESSEL_GET_INTERFACE(vessel)->vessel_y1pos )
		return MKT_VESSEL_GET_INTERFACE(vessel)->vessel_y1pos(vessel);
	return 0;
}

guint
mkt_vessel_y2pos                           ( MktVessel *vessel )
{
	g_return_val_if_fail(vessel != NULL , 0);
	g_return_val_if_fail(MKT_IS_VESSEL(vessel) , 0);
	if(MKT_VESSEL_GET_INTERFACE(vessel)->vessel_y2pos )
		return MKT_VESSEL_GET_INTERFACE(vessel)->vessel_y2pos(vessel);
	return 0;
}

guint
mkt_vessel_y3pos                           ( MktVessel *vessel )
{
	g_return_val_if_fail(vessel != NULL , 0);
	g_return_val_if_fail(MKT_IS_VESSEL(vessel) , 0);
	if(MKT_VESSEL_GET_INTERFACE(vessel)->vessel_y3pos )
		return MKT_VESSEL_GET_INTERFACE(vessel)->vessel_y3pos(vessel);
	return 0;
}

const gchar*
mkt_vessel_pump                            ( MktVessel *vessel )
{
	g_return_val_if_fail(vessel != NULL , NULL);
	g_return_val_if_fail(MKT_IS_VESSEL(vessel) , NULL);
	if(MKT_VESSEL_GET_INTERFACE(vessel)->vessel_pump )
		return MKT_VESSEL_GET_INTERFACE(vessel)->vessel_pump(vessel);
	return NULL;
}





/** @} */
