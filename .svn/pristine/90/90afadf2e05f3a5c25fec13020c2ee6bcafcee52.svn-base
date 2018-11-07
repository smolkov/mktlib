/*
 * mkt-mutex.h
 *
 *  Created on: 13.08.2013
 *      Author: sascha
 */

#ifndef MKT_MUTEX_H_
#define MKT_MUTEX_H_

#include <glib.h>

G_BEGIN_DECLS

#if GLIB_CHECK_VERSION(2,31,7)
typedef GRecMutex MktMutex;
#else
typedef struct _MktMutex MktMutex;
#endif


MktMutex*   mkt_mutex_new       (void);
void        mkt_mutex_lock      (MktMutex *mutex);
gboolean    mkt_mutex_trylock   (MktMutex *mutex);
void        mkt_mutex_unlock    (MktMutex *mutex);
void        mkt_mutex_free      (MktMutex *mutex);

#ifdef MKT_DEBUG_MUTEX
#include <stdio.h>
void        mkt_mutex_debug      (MktMutex *mutex, gboolean debug);
void        mkt_mutex_dump_usage (MktMutex *mutex, FILE *stream);
#endif

G_END_DECLS

#endif /* MKT_MUTEX_H_ */
