/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/**
 * @defgroup time
 * @ingroup market
 * @brief   Module description
 * @ingroup time
 * @{
 * @file  market-system.h
 * @brief time header file description
 *
 * (c) 2008 - 2017 LAR Process Analysers AG - All rights reserved.
 *
 * @author asmolkov@lar.com
 *
 * $Id: $ $URL: $
 */

#ifndef LAR_REQUEST_H
#define LAR_REQUEST_H 1

#include "lar-api-client.h"
#include "lar-response.h"
#include <glib-object.h>
#include <glib.h>
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>
G_BEGIN_DECLS

#define LAR_TYPE_REQUEST (lar_request_get_type())
#define LAR_REQUEST(object) (G_TYPE_CHECK_INSTANCE_CAST((object), LAR_TYPE_REQUEST, LarRequest))
#define LAR_REQUEST_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), LAR_TYPE_REQUEST, LarRequestClass))
#define LAR_IS_REQUEST(object) (G_TYPE_CHECK_INSTANCE_TYPE((object), LAR_TYPE_REQUEST))
#define LAR_IS_REQUEST_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), LAR_TYPE_REQUEST))
#define LAR_REQUEST_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), LAR_TYPE_REQUEST, LarRequestClass))

typedef struct _LarRequestPrivate LarRequestPrivate;
typedef struct _LarRequest        LarRequest;
typedef struct _LarRequestClass   LarRequestClass;
struct _LarRequest {
    GObject            parent;
    LarRequestPrivate *priv;
};

struct _LarRequestClass {
    GObjectClass parent;
};

GType lar_request_get_type(void);

G_END_DECLS

#endif /* LAR_REQUEST_H */
