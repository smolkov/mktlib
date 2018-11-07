
#include "mkt-relay-model.h"
#include "market-time.h"



enum
{
	PROP_0,
	PROP_RELAY_STRING_PARAM,
	PROP_RELAY_WARNING_PARAM,
	PROP_RELAY_CRITICAL_PARAM,
};


struct _MktRelayModelPrivate
{
	gchar      *string_parameter;
	gboolean    critical;
	gboolean    warning;
};


#define MKT_RELAY_MODEL_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), MKT_TYPE_RELAY_MODEL, MktRelayModelPrivate))



static const gchar*
mkt_relay_string_parameter                 ( MktRelay *relay )
{
	g_return_val_if_fail(relay != NULL ,NULL);
	g_return_val_if_fail(MKT_IS_RELAY_MODEL(relay) , NULL );
	return MKT_RELAY_MODEL(relay)->priv->string_parameter;
}


static void
mkt_relay_model_init_interface ( MktRelayInterface *iface )
{
	iface->relay_string        = mkt_relay_string_parameter;

}



G_DEFINE_TYPE_WITH_CODE (MktRelayModel, mkt_relay_model, MKT_TYPE_PARAM_OBJECT,
		                                G_IMPLEMENT_INTERFACE (MKT_TYPE_RELAY,
		                                		mkt_relay_model_init_interface)
		                                		MKT_CREATE_MODEL_FULL(mkt_connection_new_default_parameter(TRUE)))


static void
mkt_relay_model_init (MktRelayModel *mkt_relay_model)
{
	MktRelayModelPrivate *priv                = MKT_RELAY_MODEL_PRIVATE(mkt_relay_model);
	mkt_relay_model->priv                     = priv;
	mkt_relay_model->priv->string_parameter   = NULL;
	/* TODO: Add initialization code here */
}

static void
mkt_relay_model_finalize (GObject *model)
{
	MktRelayModel *data = MKT_RELAY_MODEL(model);
	if(data->priv->string_parameter)g_free(data->priv->string_parameter);
	G_OBJECT_CLASS (mkt_relay_model_parent_class)->finalize (model);
}




static void
mkt_relay_model_set_property (GObject *model, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_RELAY_MODEL (model));
	MktRelayModel *relay = MKT_RELAY_MODEL(model);
	switch (prop_id)
	{
	case PROP_RELAY_STRING_PARAM:
		if(relay->priv->string_parameter)g_free(relay->priv->string_parameter);
		relay->priv->string_parameter    = g_value_dup_string(value);
		break;
	case PROP_RELAY_WARNING_PARAM:
		relay->priv->warning    = g_value_get_boolean(value);
		break;
	case PROP_RELAY_CRITICAL_PARAM:
		relay->priv->critical    = g_value_get_boolean(value);
		break;

	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (model, prop_id, pspec);
		break;
	}
}

static void
mkt_relay_model_get_property (GObject *model, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (MKT_IS_RELAY_MODEL (model));
	MktRelayModel *stream = MKT_RELAY_MODEL(model);
	switch (prop_id)
	{
	case PROP_RELAY_STRING_PARAM:
		g_value_set_string(value,stream->priv->string_parameter);
		break;
	case PROP_RELAY_WARNING_PARAM:
		g_value_set_boolean(value,stream->priv->warning);
		break;
	case PROP_RELAY_CRITICAL_PARAM:
		g_value_set_boolean(value,stream->priv->critical);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (model, prop_id, pspec);
		break;
	}
}

static void
mkt_relay_model_class_init (MktRelayModelClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//MktModelClass* parent_class = MKT_MODEL_CLASS (klass);
	g_type_class_add_private (klass, sizeof (MktRelayModelPrivate));
	object_class->finalize     = mkt_relay_model_finalize;
	object_class->set_property = mkt_relay_model_set_property;
	object_class->get_property = mkt_relay_model_get_property;
	g_object_class_override_property(object_class,PROP_RELAY_STRING_PARAM,"relay-string");
	g_object_class_override_property(object_class,PROP_RELAY_WARNING_PARAM,"relay-warning");
	g_object_class_override_property(object_class,PROP_RELAY_CRITICAL_PARAM,"relay-critical");
}

