/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * vessel.h
 * Copyright (C) LAR 2017
 * 
 */

 #ifndef _VESSEL_H_
 #define _VESSEL_H_
 
 #include <glib-object.h>
 
 #define TYPE_VESSEL             (vessel_get_type ())
 #define VESSEL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_VESSEL, Vessel))
 #define VESSEL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass),  TYPE_VESSEL, VesselClass))
 #define IS_VESSEL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_VESSEL))
 #define IS_VESSEL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass),  TYPE_VESSEL))
 #define VESSEL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj),  TYPE_VESSEL, VesselClass))
 
 typedef struct _VesselClass        VesselClass;
 typedef struct _Vessel             Vessel;
 typedef struct _VesselPrivate      VesselPrivate;
 
 GType                   vessel_get_type                 (void) G_GNUC_CONST;
 
gint VesselGetXPos(Vessel *vessel);
gint VesselGetYPos(Vessel *vessel);
gint VesselGetY1Pos(Vessel *vessel);
gint VesselGetY2Pos(Vessel *vessel);
gint VesselGetY3Pos(Vessel *vessel);
gboolean VesselGetHasMotor(Vessel *vessel);

 #endif /* _VESSEL_H_ */
 