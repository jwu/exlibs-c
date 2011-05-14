// ======================================================================================
// File         : timer.c
// Author       : Wu Jie 
// Last Change  : 12/29/2010 | 10:10:06 AM | Wednesday,December
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: structures
// ------------------------------------------------------------------ 

typedef struct timer_t {
    int timer_id;
    int status;
    ex_timer_pfn callback;
    void *params;
    uint32 interval;
    uint32 scheduled;
    volatile int canceled;
    struct timer_t *next;
} timer_t;

typedef struct timer_map_t {
    int timer_id;
    timer_t *timer;
    struct timer_map_t *next;
} timer_map_t;

/* The timers are kept in a sorted list */
typedef struct timer_data_t {
    /* Data used by the main thread */
    ex_thread_t *thread;
    ex_atomic_t next_id;
    timer_map_t *timer_map;
    ex_mutex_t *timer_map_lock;

    /* Padding to separate cache lines between threads */
    char cache_pad[128];

    /* Data used to communicate with the timer thread */
    ex_spin_lock_t lock;
    ex_semaphore_t *sem;
    timer_t * volatile pending;
    timer_t * volatile freelist;
    volatile int active;

    /* List of timers - this is only touched by the timer thread */
    timer_t *timers;
} timer_data_t;

static timer_data_t __timer_data;
static bool __initialized = false;

// ------------------------------------------------------------------ 
// Desc: 
// The idea here is that any thread might add a timer, but a single
// thread manages the active timer queue, sorted by scheduling time.
// Timers are removed by simply setting a canceled flag
// ------------------------------------------------------------------ 

static void __add_timer_internal ( timer_data_t *_data, timer_t *_timer ) {
    timer_t *prev, *curr;

    prev = NULL;
    for ( curr = _data->timers; curr; prev = curr, curr = curr->next ) {
        if ( (int32)(_timer->scheduled-curr->scheduled) < 0 ) {
            break;
        }
    }

    /* Insert the _timer here! */
    if ( prev ) {
        prev->next = _timer;
    } else {
        _data->timers = _timer;
    }
    _timer->next = curr;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __timer_thread ( void *_data ) {
    timer_data_t *data = (timer_data_t *)_data;
    timer_t *pending;
    timer_t *current;
    timer_t *freelist_head = NULL;
    timer_t *freelist_tail = NULL;
    uint32 tick, now, interval, delay;

    /* Threaded timer loop:
     *  1. Queue timers added by other threads
     *  2. Handle any timers that should dispatch this cycle
     *  3. Wait until next dispatch time or new timer arrives
     */
    for ( ; ; ) {
        /* Pending and freelist maintenance */
        ex_atomic_lock (&data->lock);
            /* Get any timers ready to be queued */
            pending = data->pending;
            data->pending = NULL;

            /* Make any unused timer structures available */
            if (freelist_head) {
                freelist_tail->next = data->freelist;
                data->freelist = freelist_head;
            }
        ex_atomic_unlock (&data->lock);

        /* Sort the pending timers into our list */
        while (pending) {
            current = pending;
            pending = pending->next;
            __add_timer_internal(data, current);
        }
        freelist_head = NULL;
        freelist_tail = NULL;

        /* Check to see if we're still running, after maintenance */
        if (!data->active) {
            break;
        }

        /* Initial delay if there are no timers */
        delay = EX_MUTEX_MAXWAIT;

        tick = ex_timer_get_ticks();

        /* Process all the pending timers for this tick */
        while ( data->timers ) {
            current = data->timers;

            if ((int32)(tick-current->scheduled) < 0) {
                /* Scheduled for the future, wait a bit */
                delay = (current->scheduled - tick);
                break;
            }

            /* We're going to do something with this timer */
            data->timers = current->next;

            // re-scheduled it
            if ( current->status == EX_TIMER_STATE_PAUSED ) {
                continue;
            }

            //
            if ( current->canceled ) {
                interval = 0;
            } else {
                interval = current->callback(current->interval, current->params);
            }

            if (interval > 0) {
                /* Reschedule this timer */
                current->scheduled = tick + interval;
                __add_timer_internal(data, current);
            } else {

                // free params before sending to freelist
                if ( current->params ) {
                    ex_free_nomng (current->params);
                    current->params = NULL;
                }
                current->status = EX_TIMER_STATE_STOPPED;

                if (!freelist_head) {
                    freelist_head = current;
                }
                if (freelist_tail) {
                    freelist_tail->next = current;
                }
                freelist_tail = current;

                current->canceled = 1;
            }
        }

        /* Adjust the delay based on processing time */
        now = ex_timer_get_ticks();
        interval = (now - tick);
        if (interval > delay) {
            delay = 0;
        } else {
            delay -= interval;
        }

        /* Note that each time a timer is added, this will return
           immediately, but we process the timers added all at once.
           That's okay, it just means we run through the loop a few
           extra times.
           */
        ex_semaphore_wait_timeout(data->sem, delay);
    }
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
extern bool ex_sys_timer_init ();
// ------------------------------------------------------------------ 

int ex_timer_init () {

    timer_data_t *data = &__timer_data;

    // if the core already inited, don't init it second times.
    if ( __initialized ) {
        ex_warning ( "timer already inited" );
        return 1;
    }

    // init sys timer
    ex_sys_timer_init();

    //
    if ( !data->active ) {
        data->timer_map_lock = ex_create_mutex ();
        if ( !data->timer_map_lock ) {
            return -1;
        }

        data->sem = ex_create_semaphore (0);
        if (!data->sem) {
            ex_destroy_mutex (data->timer_map_lock);
            return -1;
        }

        data->active = 1;
        data->thread = ex_create_thread (__timer_thread, data);
        if (!data->thread) {
            ex_timer_deinit ();
            return -1;
        }

        ex_atomic_set(&data->next_id, 1);
    }

    //
    __initialized = true;
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
extern void ex_sys_timer_deinit ();
// ------------------------------------------------------------------ 

void ex_timer_deinit () {
    timer_data_t *data = &__timer_data;
    timer_t *timer;
    timer_map_t *entry;

    if ( __initialized ) {

        if (data->active) {
            data->active = 0;

            /* Shutdown the timer thread */
            if ( data->thread ) {
                ex_semaphore_post (data->sem);
                ex_wait_thread (data->thread, NULL);
                data->thread = NULL;
            }

            ex_destroy_semaphore (data->sem);
            data->sem = NULL;

            /* Clean up the timer entries */
            while (data->timers) {
                timer = data->timers;
                data->timers = timer->next;
                // free params before sending to freelist
                if ( timer->params ) {
                    ex_free_nomng (timer->params);
                    timer->params = NULL;
                }
                ex_free_nomng(timer);
            }
            while (data->freelist) {
                timer = data->freelist;
                data->freelist = timer->next;
                // free params before sending to freelist
                if ( timer->params ) {
                    ex_free_nomng (timer->params);
                    timer->params = NULL;
                }
                ex_free_nomng(timer);
            }
            while (data->timer_map) {
                entry = data->timer_map;
                data->timer_map = entry->next;
                ex_free_nomng(entry);
            }

            ex_destroy_mutex(data->timer_map_lock);
            data->timer_map_lock = NULL;
        }

        ex_sys_timer_deinit();

        //
        __initialized = false;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_timer_initialized () {
    return __initialized;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_add_timer ( timespan_t _interval,
                   ex_timer_pfn _callback, 
                   void *_params, 
                   size_t _size )
{
    timer_data_t *data = &__timer_data;
    timer_t *timer;
    timer_map_t *entry;

    if (!data->active) {
        int status = 0;

        ex_atomic_lock(&data->lock);
            if (!data->active) {
                status = ex_timer_init();
            }
        ex_atomic_unlock(&data->lock);

        if ( status < 0 ) {
            return EX_INVALID_TIMER_ID;
        }
    }

    ex_atomic_lock(&data->lock);
        timer = data->freelist;
        if (timer) {
            data->freelist = timer->next;
        }
    ex_atomic_unlock(&data->lock);

    if (timer) {
        ex_assert ( timer->params == NULL, "the timer is removed without free params" );
        ex_assert ( timer->status == EX_TIMER_STATE_STOPPED, "the timer is not stopped" );
        ex_remove_timer (timer->timer_id);
    } else {
        timer = (timer_t *)ex_malloc_nomng(sizeof(*timer));
        if (!timer) {
            return EX_INVALID_TIMER_ID;
        }
    }
    timer->status = EX_TIMER_STATE_STOPPED;
    timer->timer_id = ex_atomic_incref(&data->next_id);
    timer->callback = _callback;
    timer->params = NULL;
    if ( _params ) {
        timer->params = ex_malloc_nomng(_size);
        memcpy( timer->params, _params, _size );
    }
    timer->interval = ex_timespan_to_msecs(_interval);
    timer->scheduled = -1;
    timer->canceled = 0;

    entry = (timer_map_t *)ex_malloc_nomng(sizeof(*entry));
    if (!entry) {
        ex_free_nomng (timer);
        return EX_INVALID_TIMER_ID;
    }
    entry->timer = timer;
    entry->timer_id = timer->timer_id;

    ex_mutex_lock(data->timer_map_lock);
        entry->next = data->timer_map;
        data->timer_map = entry;
    ex_mutex_unlock(data->timer_map_lock);

    // DELME { 
    // /* Add the timer to the pending list for the timer thread */
    // ex_atomic_lock(&data->lock);
    //     timer->next = data->pending;
    //     data->pending = timer;
    // ex_atomic_unlock(&data->lock);

    // /* Wake up the timer thread if necessary */
    // ex_semaphore_post(data->sem);
    // } DELME end 

    return entry->timer_id;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_remove_timer ( int _id ) {

    timer_data_t *data = &__timer_data;
    timer_map_t *prev, *entry;
    int canceled = 0;

    /* Find the timer */
    ex_mutex_lock(data->timer_map_lock);
        prev = NULL;
        for ( entry = data->timer_map; entry; prev = entry, entry = entry->next ) {
            if (entry->timer_id == _id) {
                if (prev) {
                    prev->next = entry->next;
                } else {
                    data->timer_map = entry->next;
                }
                break;
            }
        }
    ex_mutex_unlock(data->timer_map_lock);

    if (entry) {
        if (!entry->timer->canceled) {
            entry->timer->canceled = 1;
            canceled = 1;
        }
        ex_free_nomng(entry);
    }
    return canceled;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_start_timer ( int _id ) {

    timer_data_t *data = &__timer_data;
    timer_map_t *prev, *entry;

    /* Find the timer */
    ex_mutex_lock(data->timer_map_lock);
        prev = NULL;
        for ( entry = data->timer_map; entry; prev = entry, entry = entry->next ) {
            if ( entry->timer_id == _id ) 
            {
                break;
            }
        }
    ex_mutex_unlock(data->timer_map_lock);

    /* Add the timer to the pending list for the timer thread */
    // NOTE: we not start timer twice.
    // NOTE: pause timer will be reset and start.
    if ( entry->timer->status != EX_TIMER_STATE_RUNNING ) {
        ex_atomic_lock(&data->lock);
            entry->timer->next = data->pending;
            data->pending = entry->timer;
            entry->timer->scheduled = ex_timer_get_ticks() + entry->timer->interval;
            entry->timer->status = EX_TIMER_STATE_RUNNING;
        ex_atomic_unlock(&data->lock);
    }

    /* Wake up the timer thread if necessary */
    ex_semaphore_post(data->sem);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_pause_timer ( int _id ) {

    timer_data_t *data = &__timer_data;
    timer_map_t *prev, *entry;

    /* Find the timer */
    ex_mutex_lock(data->timer_map_lock);
        prev = NULL;
        for ( entry = data->timer_map; entry; prev = entry, entry = entry->next ) {
            // NOTE: only running timer can paused
            if ( entry->timer_id == _id &&
                 entry->timer->status == EX_TIMER_STATE_RUNNING ) 
            {
                entry->timer->scheduled = entry->timer->scheduled - ex_timer_get_ticks();
                entry->timer->status = EX_TIMER_STATE_PAUSED;
                break;
            }
        }
    ex_mutex_unlock(data->timer_map_lock);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_resume_timer ( int _id ) {

    timer_data_t *data = &__timer_data;
    timer_map_t *prev, *entry;

    /* Find the timer */
    ex_mutex_lock(data->timer_map_lock);
        prev = NULL;
        for ( entry = data->timer_map; entry; prev = entry, entry = entry->next ) {
            if (entry->timer_id == _id) {
                break;
            }
        }
    ex_mutex_unlock(data->timer_map_lock);

    /* Add the timer to the pending list for the timer thread */
    // NOTE: only paused timer can resume
    if ( entry->timer->status == EX_TIMER_STATE_PAUSED ) {
        ex_atomic_lock(&data->lock);
            entry->timer->next = data->pending;
            data->pending = entry->timer;
            entry->timer->scheduled = ex_timer_get_ticks() + entry->timer->scheduled;
            entry->timer->status = EX_TIMER_STATE_RUNNING;
        ex_atomic_unlock(&data->lock);
    }

    /* Wake up the timer thread if necessary */
    ex_semaphore_post(data->sem);
}
