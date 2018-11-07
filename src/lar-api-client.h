/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */

#ifndef _LAR_API_CLIENT_H_
#define _LAR_API_CLIENT_H_
#include "lar-device.h"
#include "lar-license.h"
#include <gio/gio.h>
#include <glib-object.h>
#include <glib.h>
G_BEGIN_DECLS

#define LAR_TYPE_API_CLIENT (lar_api_client_get_type())
#define LAR_API_CLIENT(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), LAR_TYPE_API_CLIENT, LarApiClient))
#define LAR_API_CLIENT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), LAR_TYPE_API_CLIENT, LarApiClientClass))
#define LAR_IS_API_CLIENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), LAR_TYPE_API_CLIENT))
#define LAR_IS_API_CLIENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), LAR_TYPE_API_CLIENT))
#define LAR_API_CLIENT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), LAR_TYPE_API_CLIENT, LarApiClientClass))

typedef struct _LarApiClientClass   LarApiClientClass;
typedef struct _LarApiClient        LarApiClient;
typedef struct _LarApiClientPrivate LarApiClientPrivate;

GType lar_api_client_get_type(void) G_GNUC_CONST;

gboolean lar_api_client_is_busy(LarApiClient *client);
void lar_api_client_operation_cancel(LarApiClient *client);

LarApiClient *lar_api_client_new(const gchar *adress, guint port);

LarApiClient *lar_api_client_default_new();
LarDevice *lar_api_client_get_device(LarApiClient *client);
GList *lar_api_client_get_licenses(LarApiClient *client);
LarLicense *lar_api_client_find_license(LarApiClient *api, const gchar *product, const gchar *spec);
gboolean lar_api_create_device(LarApiClient *api, LarDevice *device);
gboolean lar_api_client_connect(LarApiClient *api, const gchar *serial);
gboolean lar_api_create_license(LarApiClient *api, LarLicense *license);

G_END_DECLS

#endif /* _LAR_API_CLIENT_H_ */
