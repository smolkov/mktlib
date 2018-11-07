/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * ProOptKa
 * Copyright (C) A.Smolkov 2011 <asmolkov@lar.com>
 * 
 * ProOptKa is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ProOptKa is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mkt-module.h"


struct _MktModulePrivate
{
	GModule     *library;
	gchar       *modulpath;
};

#define MKT_MODULE_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), MKT_TYPE_MODULE, MktModulePrivate))



enum {
	PROP_NULL,
	PROP_PATH,
};


G_DEFINE_TYPE (MktModule, mkt_module, G_TYPE_TYPE_MODULE);



gboolean
mkt_module_is_valid_module_name (const gchar *basename)
{
#if !defined(G_OS_WIN32) && !defined(G_WITH_CYGWIN)
	return  g_str_has_suffix (basename, ".so");
#else
	return g_str_has_suffix (basename, ".dll");
#endif
}


static void
mkt_module_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	//g_printf("Set (MKT_MODUL) property \n");
	g_return_if_fail (MKT_IS_MODULE (object));
	MktModule* module = MKT_MODULE(object);
	switch (prop_id)
	{
	case PROP_PATH:
		/* TODO: Add setter for "mdule_path" property here */
		g_free(module->priv->modulpath );
		module->priv->modulpath     = g_value_dup_string (value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_module_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	//g_printf("Get (MKT_MODUL) property \n");
	g_return_if_fail (MKT_IS_MODULE (object));
	MktModule* module = MKT_MODULE(object);
	switch (prop_id)
	{
	case PROP_PATH:
		g_value_set_string(value,module->priv->modulpath);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
mkt_static_module_unload         (GTypeModule  *gmodule)
{
	MktModule *module = MKT_MODULE( gmodule );
	//TEST:
	//g_debug("module unload from %s ",module->priv->modulpath);
	if(module->unload )module->unload (module);
	if(module->priv->library)
	{
		g_module_close (module->priv->library);
	}
	module->priv->library = NULL;

	module->load   = NULL;
	module->unload = NULL;
}


static void
mkt_module_init (MktModule *module)
{
	module->priv = G_TYPE_INSTANCE_GET_PRIVATE(module,MKT_TYPE_MODULE,MktModulePrivate);
	module->priv->modulpath   = g_strdup("/lar/gui/module/");
	module->priv->library     = NULL;
	module->load              = NULL;
	module->unload            = NULL;
	/* TODO: Add initialization code here */
}

static void
mkt_module_finalize (GObject *gmodule)
{
	/* TODO: Add deinitalization code here */
	MktModule *module = MKT_MODULE(gmodule);
	mkt_static_module_unload(G_TYPE_MODULE(module));
	g_free(module->priv->modulpath);
	G_OBJECT_CLASS (mkt_module_parent_class)->finalize (gmodule);
}


static gboolean
mkt_static_module_load           (GTypeModule  *gmodule)
{
	MktModule *module = MKT_MODULE(gmodule);
	if (!module->priv->modulpath)
	{
		g_warning ("Module path not set");
		return FALSE;
	}
	if(mkt_module_is_valid_module_name(module->priv->modulpath))
	{
		module->priv->library = g_module_open (module->priv->modulpath, 0);
		if (!module->priv->library)
		{
			g_critical ("%s", g_module_error ());
			return FALSE;
		}
		if (! g_module_symbol (module->priv->library,
				"mkt_lar_module_load",
				(gpointer *) &module->load) ||
				! g_module_symbol (module->priv->library,
						"mkt_lar_module_unload",
						(gpointer *) &module->unload))
		{
			g_critical  ("module load error :%s\n", g_module_error ());
			g_module_close (module->priv->library);
			module->priv->library = NULL;
			module->load          = NULL;
			module->unload        = NULL;
			return FALSE;
		}
		else
		{
			/* Initialize the loaded module */
			module->load (module);
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	return FALSE;
}

static void
mkt_module_class_init (MktModuleClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	GTypeModuleClass* parent_class = G_TYPE_MODULE_CLASS (klass);

	g_type_class_add_private (klass, sizeof (MktModulePrivate));

	object_class->finalize        = mkt_module_finalize;
	object_class->set_property    = mkt_module_set_property;
	object_class->get_property    = mkt_module_get_property;

	parent_class->load            = mkt_static_module_load;
	parent_class->unload          = mkt_static_module_unload;

	GParamSpec *pspec;
	pspec = g_param_spec_string ("modulpath",
			"Module path",
			"Set module path",
			"/lar/gui/modules/stTestPlugIn/",
			G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY);
	g_object_class_install_property (object_class,
			PROP_PATH,pspec);
}

MktModule*
mkt_module_new (const gchar* filepath)
{
	/* TODO: Add public function implementation here */
	gchar* priv_filepath = filepath ? ((gchar *) filepath) : ((gchar *) "/lar/gui/modules/TestPlugIn/");
	MktModule *module = MKT_MODULE(g_object_new(MKT_TYPE_MODULE,"modulpath",priv_filepath,NULL));
	return module;
}
