/*
 * mkt-mutex.c
 *
 *  Created on: 13.08.2013
 *      Author: sascha
 */

#include "mkt-mutex.h"
#include <stdlib.h>

#if GLIB_CHECK_VERSION(2,31,7)

/**
 * mkt_mutex_new: (skip)
 *
 * Creates a new #MktMutex.
 *
 * Deprecated: 5.2.0: use #GRecMutex instead.
 *
 * Returns: (transfer full): a new #MktMutex
 */
MktMutex*
mkt_mutex_new (void)
{
	MktMutex *mutex;
	mutex = g_new0 (MktMutex, 1);
	g_rec_mutex_init (mutex);
	return mutex;
}

/**
 * mkt_mutex_lock:
 * @mutex: a #MktMutex
 *
 * Locks @mutex. If @mutex is already locked by another thread, the current thread will block until @mutex is unlocked by the other thread.
 *
 * This function can be used even if g_thread_init() has not yet been called, and, in that case, will do nothing.
 *
 * Note: unlike g_mutex_lock(), the #MktMutex is recursive, which means a thread can lock it several times (and has
 * to unlock it as many times to actually unlock it).
 *
 * Deprecated: 5.2.0: use #GRecMutex instead.
 */
void
mkt_mutex_lock (MktMutex *mutex)
{
	g_rec_mutex_lock (mutex);
}

/**
 * mkt_mutex_trylock:
 * @mutex: a #MktMutex
 *
 * Tries to lock @mutex. If @mutex is already locked by another thread, it immediately returns FALSE.
 * Otherwise it locks @mutex and returns TRUE
 *
 * This function can be used even if g_thread_init() has not yet been called, and, in that case, will immediately return TRUE.
 *
 * Note: Unlike g_mutex_trylock(), the #MktMutex is recursive, which means a thread can lock it several times (and has
 * to unlock it as many times to actually unlock it)
 *
 * Returns: TRUE, if @mutex could be locked.
 *
 * Deprecated: 5.2.0: use #GRecMutex instead.
 */
gboolean
mkt_mutex_trylock (MktMutex *mutex)
{
	return g_rec_mutex_trylock (mutex);
}

/**
 * mkt_mutex_unlock:
 * @mutex: a #MktMutex
 *
 * Unlocks @mutex. If another thread is blocked in a mkt_mutex_lock() call for @mutex, it wil
 * be woken and can lock @mutex itself.
 * This function can be used even if g_thread_init() has not yet been called, and, in that case, will do nothing.
 *
 * Deprecated: 5.2.0: use #GRecMutex instead.
 */
void
mkt_mutex_unlock (MktMutex *mutex)
{
	g_rec_mutex_unlock (mutex);
}

/**
 * mkt_mutex_free:
 * @mutex: (transfer full): a #MktMutex
 *
 * Destroys @mutex.
 *
 * Deprecated: 5.2.0: use #GRecMutex instead.
 */
void
mkt_mutex_free (MktMutex *mutex)
{
	g_rec_mutex_clear (mutex);
	g_free (mutex);
}

#ifdef MKT_DEBUG_MUTEX
void
mkt_mutex_debug (MktMutex *mutex, gboolean debug)
{
	g_warning ("Nothing to debug, MktMutex is a GRecMutex.");
}

void
mkt_mutex_dump_usage (MktMutex *mutex, FILE *stream)
{
	g_warning ("Nothing to debug, MktMutex is a GRecMutex.");
	g_fprintf (stream, "%s", "Nothing to debug, MktMutex is a GRecMutex.");
}
#endif


#else /* GLIB_CHECK_VERSION */

#ifdef MKT_DEBUG_MUTEX
#define FRAMES_SIZE 10
#include <glib/gprintf.h>
#include <execinfo.h>
#include <stdlib.h>
#include <string.h>
#endif

enum MutexRecStatus {
	UNKNOWN,
	RECURSIVE,
	NON_RECURSIVE,
	NON_SUPPORTED
};

static enum MutexRecStatus impl_status = UNKNOWN;

#ifdef MKT_DEBUG_MUTEX
static GStaticMutex debug_mutex = G_STATIC_MUTEX_INIT;

typedef enum {
	USAGE_LOCK,
	USAGE_UNLOCK,
} MktMutexUsageType;

typedef struct {
	GThread            *thread;
	MktMutexUsageType   usage;
	gchar             **frames; /* array terminated by a NULL */
} MktMutexUsage;
#endif

struct _MktMutex {
	GMutex  *mutex; /* internal mutex to access the structure's data */
	GCond   *cond;  /* condition to lock on */
	GThread *owner; /* current owner of the mutex, or NULL if not owned */
	short    depth;
#ifdef MKT_DEBUG_MUTEX
	gboolean debug; /* set tu %TRUE to debug this mutex */
	GArray  *usages; /* Array of MktMutexUsage */
	gint     nb_locked_unlocked;
#endif
};

#ifdef MKT_DEBUG_MUTEX
void
mkt_mutex_debug (MktMutex *mutex, gboolean debug)
{
	g_static_mutex_lock (&debug_mutex);
	mutex->debug = debug;
	g_static_mutex_unlock (&debug_mutex);
}

void
mkt_mutex_dump_usage (MktMutex *mutex, FILE *stream)
{
	guint i;
	FILE *st;
	if (stream)
		st = stream;
	else
		st = stdout;

	g_static_mutex_lock (&debug_mutex);
	if (mutex->debug) {
		g_fprintf (st, "%s (mutex=>%p): locked&unlocked %d times\n", __FUNCTION__, mutex,
			   mutex->nb_locked_unlocked);
		for (i = mutex->usages->len; i > 0; i--) {
			MktMutexUsage *usage;
			gint j;
			usage = &g_array_index (mutex->usages, MktMutexUsage, i - 1);
			g_fprintf (st, "%d\t------ BEGIN MktMutex %p usage\n", i - 1, mutex);
			g_fprintf (st, "\t%s, thread %p\n",
				   usage->usage == USAGE_LOCK ? "LOCK" : "UNLOCK",
				   usage->thread);
			for (j = 0; usage->frames[j]; j++)
				g_fprintf (st, "\t%s\n", usage->frames[j]);
			g_fprintf (st, "\t------ END MktMutex %p usage\n", mutex);
		}
	}
	g_static_mutex_unlock (&debug_mutex);
}

static void
mkt_mutex_usage_locked (MktMutex *mutex)
{
	g_static_mutex_lock (&debug_mutex);

	if (mutex->debug) {
		MktMutexUsage usage;
		usage.thread = g_thread_self ();
		usage.usage = USAGE_LOCK;

		void *array[FRAMES_SIZE];
		size_t size;
		char **strings;
		size_t i;

		size = backtrace (array, 10);
		strings = backtrace_symbols (array, size);
		usage.frames = g_new (gchar *, size + 1);
		usage.frames[size] = NULL;
		for (i = 0; i < size; i++)
			usage.frames[i] = g_strdup (strings[i]);
		free (strings);

		g_array_prepend_val (mutex->usages, usage);
	}
	g_static_mutex_unlock (&debug_mutex);
	mkt_mutex_dump_usage (mutex, NULL);
}

static void
mkt_mutex_usage_unlocked (MktMutex *mutex)
{
	g_static_mutex_lock (&debug_mutex);
	if (mutex->debug) {
		void *array[FRAMES_SIZE];
		size_t size;
		char **strings;
		size_t i;
		gboolean matched = FALSE;

		size = backtrace (array, 10);
		strings = backtrace_symbols (array, size);

		if (mutex->usages->len > 0) {
			MktMutexUsage *last;
			last = &g_array_index (mutex->usages, MktMutexUsage, 0);
			if ((size > 3) &&
			    (last->usage == USAGE_LOCK) &&
			    (last->thread == g_thread_self ())) {
				for (i = 3; i < size; i++) {
					if (! last->frames[i] || (last->frames[i] &&
								  strcmp (last->frames[i], strings[i])))
						break;
				}
				if ((i == size) && ! last->frames[i]) {
					/* same stack => delete @last */
					g_strfreev (last->frames);
					g_array_remove_index (mutex->usages, 0);
					matched = TRUE;
					mutex->nb_locked_unlocked++;
				}
			}
		}

		if (! matched) {
			MktMutexUsage usage;
			usage.thread = g_thread_self ();
			usage.usage = USAGE_UNLOCK;

			usage.frames = g_new (gchar *, size + 1);
			usage.frames[size] = NULL;
			for (i = 0; i < size; i++)
				usage.frames[i] = g_strdup (strings[i]);

			g_array_prepend_val (mutex->usages, usage);
		}
		free (strings);
	}
	g_static_mutex_unlock (&debug_mutex);
	mkt_mutex_dump_usage (mutex, NULL);
}

#endif

/**
 * mkt_mutex_new: (skip)
 *
 * Creates a new #MktMutex.
 *
 * Note: Unlike g_mutex_new(), this function will return %NULL if g_thread_init() has not been called yet.
 *
 * Returns: (transfer full): a new #MktMutex
 */
MktMutex*
mkt_mutex_new ()
{
	if (G_UNLIKELY (impl_status == UNKNOWN)) {
		static GStaticMutex init_mutex = G_STATIC_MUTEX_INIT;

		g_static_mutex_lock (&init_mutex);
		if (impl_status == UNKNOWN) {
			if (!g_thread_supported ())
				impl_status = NON_SUPPORTED;
			else {
				GMutex *m;
				m = g_mutex_new ();
				g_mutex_lock (m);
				if (g_mutex_trylock (m)) {
					impl_status = RECURSIVE;
					g_mutex_unlock (m);
				}
				else
					impl_status = NON_RECURSIVE;
				g_mutex_unlock (m);
				g_mutex_free (m);
#ifdef MKT_DEBUG_NO
				g_message ("GMutex %s recursive\n", (impl_status == RECURSIVE) ? "is" : "isn't");
#endif
			}
		}
                g_static_mutex_unlock (&init_mutex);
	}

	if (impl_status == NON_SUPPORTED) {
		MktMutex *m;
		m = calloc (1,sizeof(MktMutex));
#ifdef MKT_DEBUG_MUTEX
		m->usages = g_array_new (FALSE, FALSE, sizeof (MktMutexUsage));
		m->debug = FALSE;
#endif
		return m;
	}
	else {
		MktMutex *m;
		m = calloc (1,sizeof(MktMutex));
		m->mutex = g_mutex_new ();
		m->cond = g_cond_new ();
		m->owner = NULL;
		m->depth = 0;
#ifdef MKT_DEBUG_MUTEX
		m->usages = g_array_new (FALSE, FALSE, sizeof (MktMutexUsage));
		m->debug = FALSE;
#endif
		return m;
	}
}

/**
 * mkt_mutex_lock:
 * @mutex: a #MktMutex
 *
 * Locks @mutex. If @mutex is already locked by another thread, the current thread will block until @mutex is unlocked by the other thread.
 *
 * This function can be used even if g_thread_init() has not yet been called, and, in that case, will do nothing.
 *
 * Note: unlike g_mutex_lock(), the #MktMutex is recursive, which means a thread can lock it several times (and has
 * to unlock it as many times to actually unlock it).
 */
void
mkt_mutex_lock (MktMutex *mutex)
{
	if (impl_status == RECURSIVE)
		g_mutex_lock (mutex->mutex);
	else if (impl_status == NON_SUPPORTED)
		return;
	else {
		GThread *th = g_thread_self ();
		g_mutex_lock (mutex->mutex);
		while (1) {
			if (!mutex->owner) {
				mutex->owner = th;
				mutex->depth = 1;
				break;
			}
			else if (mutex->owner == th) {
				mutex->depth++;
				break;
			}
			else {
				g_cond_wait (mutex->cond, mutex->mutex);
			}
                }
		g_mutex_unlock (mutex->mutex);
	}
#ifdef MKT_DEBUG_MUTEX
	mkt_mutex_usage_locked (mutex);
#endif
}

/**
 * mkt_mutex_trylock:
 * @mutex: a #MktMutex
 *
 * Tries to lock @mutex. If @mutex is already locked by another thread, it immediately returns FALSE.
 * Otherwise it locks @mutex and returns TRUE
 *
 * This function can be used even if g_thread_init() has not yet been called, and, in that case, will immediately return TRUE.
 *
 * Note: Unlike g_mutex_trylock(), the #MktMutex is recursive, which means a thread can lock it several times (and has
 * to unlock it as many times to actually unlock it)
 *
 * Returns: TRUE, if @mutex could be locked.
 */
gboolean
mkt_mutex_trylock (MktMutex *mutex)
{
	if (impl_status == RECURSIVE) {
#ifdef MKT_DEBUG_MUTEX
		gboolean retval;
		retval = g_mutex_trylock (mutex->mutex);
		if (retval)
			mkt_mutex_usage_locked (mutex);
		return retval;
#else
		return g_mutex_trylock (mutex->mutex);
#endif
	}
	else if (impl_status == NON_SUPPORTED)
		return TRUE;
	else {
		GThread *th = g_thread_self ();
		gboolean retval;
		g_mutex_lock (mutex->mutex);
		if (!mutex->owner) {
			mutex->owner = th;
			mutex->depth = 1;
			retval = TRUE;
		}
		else if (mutex->owner == th) {
			mutex->depth++;
			retval = TRUE;
		}
		else
			retval = FALSE;
		g_mutex_unlock (mutex->mutex);
#ifdef MKT_DEBUG_MUTEX
		if (retval)
			mkt_mutex_usage_locked (mutex);
#endif
		return retval;
	}
}

/**
 * mkt_mutex_unlock:
 * @mutex: a #MktMutex
 *
 * Unlocks @mutex. If another thread is blocked in a mkt_mutex_lock() call for @mutex, it wil
 * be woken and can lock @mutex itself.
 * This function can be used even if g_thread_init() has not yet been called, and, in that case, will do nothing.
 */
void
mkt_mutex_unlock (MktMutex *mutex)
{
#ifdef MKT_DEBUG_MUTEX
	mkt_mutex_usage_unlocked (mutex);
#endif
	if (impl_status == RECURSIVE)
		g_mutex_unlock (mutex->mutex);
	else if (impl_status == NON_SUPPORTED)
		return;
	else {
		GThread *th = g_thread_self ();
		g_mutex_lock (mutex->mutex);
		g_assert (th == mutex->owner);
		mutex->depth--;
                if (mutex->depth == 0) {
                        mutex->owner = NULL;
			g_cond_signal (mutex->cond);
                }
		g_mutex_unlock (mutex->mutex);
	}
}

/**
 * mkt_mutex_free:
 * @mutex: (transfer full): a #MktMutex
 *
 * Destroys @mutex.
 */
void
mkt_mutex_free (MktMutex *mutex)
{
	g_assert (mutex);
	if (mutex->cond)
		g_cond_free (mutex->cond);
	mutex->cond = NULL;
	if (mutex->mutex)
		g_mutex_free (mutex->mutex);
	mutex->mutex = NULL;
#ifdef MKT_DEBUG_MUTEX
	guint i;
	for (i = 0; i < mutex->usages->len; i++) {
		MktMutexUsage *usage;
		usage = &g_array_index (mutex->usages, MktMutexUsage, i);
		g_strfreev (usage->frames);
	}
#endif
	g_free (mutex);
}

#endif /* GLIB_CHECK_VERSION */

