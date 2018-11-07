
#ifndef _MKT_RELAY_MODEL_H_
#define _MKT_RELAY_MODEL_H_

#include "mkt-relay.h"
#include "mkt-param-object.h"



G_BEGIN_DECLS

#define MKT_TYPE_RELAY_MODEL             (mkt_relay_model_get_type ())
#define MKT_RELAY_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), MKT_TYPE_RELAY_MODEL, MktRelayModel))
#define MKT_RELAY_MODEL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), MKT_TYPE_RELAY_MODEL, MktRelayModelClass))
#define MKT_IS_RELAY_MODEL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MKT_TYPE_RELAY_MODEL))
#define MKT_IS_RELAY_MODEL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), MKT_TYPE_RELAY_MODEL))
#define MKT_RELAY_MODEL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), MKT_TYPE_RELAY_MODEL, MktRelayModelClass))

typedef struct _MktRelayModelClass        MktRelayModelClass;
typedef struct _MktRelayModel             MktRelayModel;
typedef struct _MktRelayModelPrivate      MktRelayModelPrivate;


struct _MktRelayModelClass
{
	MktParamObjectClass            parent_class;
};

struct _MktRelayModel
{
	MktParamObject                  parent_instance;
	MktRelayModelPrivate          *priv;
};

GType                              mkt_relay_model_get_type                 (void) G_GNUC_CONST;

G_END_DECLS

#endif /* _MKT_RELAY_MODEL_H_ */
