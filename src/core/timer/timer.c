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
// private defines
///////////////////////////////////////////////////////////////////////////////

#define __ROUND_RESOLUTION(_x) \
	(((_x+EX_TIMER_RESOLUTION-1)/EX_TIMER_RESOLUTION)*EX_TIMER_RESOLUTION)

// ------------------------------------------------------------------ 
// Desc: timer_t 
// ------------------------------------------------------------------ 

typedef struct timer_t {
    int state;
    bool affect_by_timescale;

    // start delay for msecs
    uint32 delay;    // msecs
    uint32 interval; // msecs
    // if reach lifetime, the timer will stopped but not removed
    uint32 lifetime;    // msecs

    uint32 start;       // msecs
    uint32 last_alarm;  // msecs

    // NOTE: it is possible the counter be minus
    int32 start_counter; // msecs
    int32 interval_counter; // msecs
    int32 lifetime_counter; // msecs

    ex_timer_pfn cb;
    ex_timer_stop_pfn on_stop;
    void *params;
} timer_t;

// when finish ex_timer_init, this will set to 1. in ex_timer_deinit, it will set to 0
int __timer_running = 0;

static ex_pool_t __timers; // NOTE: we don't use pointer pool, because we known we only use it in this file.
static ex_array_t __unused_timers; // NOTE: we don't use pointer array, because we known we only use it in this file.
static bool __initialized = false;
static ex_mutex_t *__timer_mutex = NULL;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void __threaded_timer_tick () {
    uint32 now;
    int32 ms;

    ex_mutex_lock(__timer_mutex);
    now = ex_timer_get_ticks();
    ex_pool_raw_each ( &__timers, timer_t *, t ) {
        int32 time_since_start;
        int32 time_since_lastAlarm;

        if ( t->state != EX_TIMER_STATE_RUNNING )
            ex_pool_continue;

        // process start
        if ( t->start_counter != 0 ) {
            ms = t->start_counter - EX_TIMER_SLICE;
            time_since_start = (int32)(now - t->start); 
            // if we still wait for start
            if ( time_since_start <= ms )
                ex_pool_continue;
        }

        // process internval
        ms = t->interval_counter - EX_TIMER_SLICE;
        time_since_lastAlarm = (int32)(now - t->last_alarm); 

        // if we exceed the interval
        if ( time_since_lastAlarm > ms ) {
            if ( time_since_lastAlarm < t->interval_counter )
                t->last_alarm += t->interval;
            else
                t->last_alarm = now;

            // execute the timer callback
            ex_mutex_unlock(__timer_mutex);
            ms = t->cb(t->interval, t->params);
            ex_mutex_lock(__timer_mutex);

            // process return interval
            if (ms != t->interval) {
                // if ms not zero round it and set as the next interval.
                if ( ms > 0 )
                    t->interval_counter = t->interval = __ROUND_RESOLUTION(ms);
                else
                    ex_array_append_int32(&__unused_timers, __id__);
            }
            else {
                t->interval_counter = t->interval;
            }
        }

        // process lifetime
        if ( t->lifetime != -1 ) {
            int32 time_since_start;

            ms = t->lifetime_counter - EX_TIMER_SLICE;
            time_since_start = (int32)(now - t->start); 

            // if we exceed the lifetime, remove the timer
            if ( time_since_start > ms ) {
                ex_array_append_int32(&__unused_timers, __id__);
                ex_pool_continue;
            }
        }

    } ex_pool_each_end

    // remove all timer here
    ex_array_each ( &__unused_timers, int, id ) {
        timer_t *t = (timer_t *)ex_pool_get ( &__timers, id );
        if (t) ex_free_nomng (t->params);
        ex_pool_remove_at_safe ( &__timers, id ); // this can work around if we got several same id to remove.
    } ex_array_each_end
    ex_array_remove_all(&__unused_timers);
    ex_mutex_unlock(__timer_mutex);
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
extern bool ex_sys_timer_init ();
// ------------------------------------------------------------------ 

int ex_timer_init () {
    int ret = false;

    // if the core already inited, don't init it second times.
    if ( __initialized ) {
        ex_warning ( "timer already inited" );
        return 1;
    }

    // init timer
    ret = ex_sys_timer_init();
    __timer_mutex = ex_create_mutex();
    if ( ret == -1 )
        return -1;

    //
    ex_pool_init ( &__timers, 
                   EX_STRID_NULL, 
                   sizeof(timer_t),
                   8,
                   __ex_pool_alloc_nomng,
                   __ex_pool_realloc_nomng,
                   __ex_pool_dealloc_nomng );
    //
    ex_array_init ( &__unused_timers, 
                    EX_STRID_NULL, 
                    sizeof(int), 
                    32,
                    __ex_array_alloc_nomng,
                    __ex_array_realloc_nomng,
                    __ex_array_dealloc_nomng );

    // now start the timer
    __timer_running = 1;

    //
    __initialized = true;
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
extern void ex_sys_timer_deinit ();
// ------------------------------------------------------------------ 

void ex_timer_deinit () {

    if ( __initialized ) {
        // we must stop all timers before we remove them.
        __timer_running = 0;

        // free params in timer
        ex_pool_raw_each ( &__timers, timer_t *, t ) {
            ex_free_nomng (t->params);
        } ex_pool_each_end

        // clear all timers
        ex_pool_deinit(&__timers);
        ex_array_deinit(&__unused_timers);

        //
        ex_sys_timer_deinit();
        ex_destroy_mutex (__timer_mutex);
        __timer_mutex = NULL;

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

int ex_add_timer ( ex_timer_pfn _cb, 
                   ex_timer_stop_pfn _on_stop,
                   void *_params, 
                   size_t _size, /*parameter byte-size*/ 
                   timespan_t _delay,
                   timespan_t _interval,
                   timespan_t _lifetime /*EX_TIMESPAN_INFINITY*/ ) {

    int id;
    timer_t newTimer;

    // check the inputs
    ex_assert_return ( _cb != NULL, -1, "callback function can't not be NULL" );
    ex_assert_return ( _delay != EX_TIMESPAN_INFINITY, -1, "delay can't be EX_TIMESPAN_INFINITY, if you don't need delay, put it to 0" );
    ex_assert_return ( _interval != 0, -1, "interval can't be 0, the minimum value is 1" );
    ex_assert_return ( _interval != EX_TIMESPAN_INFINITY, -1, "interval can't be infinity, the minimum value is 1" );
    ex_assert_return ( _lifetime != 0, -1, "lifetime can't be 0, the minimum value is 1, if you don't need lifetime, put it to EX_TIMESPAN_INFINITY" );

    // init new timer
    newTimer.state = EX_TIMER_STATE_STOPPED;
    newTimer.start_counter = newTimer.delay = _delay;
    newTimer.interval_counter = newTimer.interval = __ROUND_RESOLUTION(ex_timespan_to_msecs(_interval));
    newTimer.lifetime_counter = newTimer.lifetime = (_lifetime == EX_TIMESPAN_INFINITY) ? -1 : __ROUND_RESOLUTION(ex_timespan_to_msecs(_lifetime));
    newTimer.start = newTimer.last_alarm = -1; // start and counter should assign in ex_start_timer
    newTimer.cb = _cb;
    newTimer.on_stop = _on_stop;
    if ( _params ) {
        newTimer.params = ex_malloc_nomng(_size);
        memcpy( newTimer.params, _params, _size );
    } else {
        newTimer.params = NULL;
    }

    //
    id = ex_pool_insert ( &__timers, &newTimer );
    return id;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_remove_timer ( int _id ) {
    bool validTimer = ex_pool_isvalid ( &__timers, _id );

    // the timer already removed
    if ( !validTimer )
        return false;

    ex_stop_timer(_id); // mark timer as stopped
    ex_array_append_int32(&__unused_timers, _id);
    return true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_start_timer ( int _id ) {
    timer_t *t;

    if ( _id == EX_INVALID_TIMER_ID )
        return;

    t = (timer_t *)ex_pool_get ( &__timers, _id );
    if (t) {
        ex_mutex_lock(__timer_mutex);
            t->start = t->last_alarm = ex_timer_get_ticks();
            t->start_counter = t->delay;
            t->interval_counter = t->interval; 
            t->lifetime_counter = t->lifetime;
            t->state = EX_TIMER_STATE_RUNNING;
        ex_mutex_unlock(__timer_mutex);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_stop_timer ( int _id ) {
    timer_t *t;

    if ( _id == EX_INVALID_TIMER_ID )
        return;

    t = (timer_t *)ex_pool_get ( &__timers, _id );
    if (t) {
        ex_mutex_lock(__timer_mutex);
            if ( t->on_stop )
                t->on_stop(t->params);
            t->start = t->last_alarm = -1;
            t->state = EX_TIMER_STATE_STOPPED;
        ex_mutex_unlock(__timer_mutex);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_pause_timer ( int _id ) {
    timer_t *t;

    if ( _id == EX_INVALID_TIMER_ID )
        return;
    
    t = (timer_t *)ex_pool_get ( &__timers, _id );
    if (t) {
        uint32 now;
        int32 time_since_lastAlarm,time_since_start;

        ex_mutex_lock(__timer_mutex);
            now = ex_timer_get_ticks();
            time_since_lastAlarm = (int32)(now - t->last_alarm);
            time_since_start = (int32)(now - t->start);
            t->start_counter = t->start_counter - time_since_start;
            t->interval_counter = t->interval_counter - time_since_lastAlarm;
            t->lifetime_counter = t->lifetime_counter - time_since_start;
            t->state = EX_TIMER_STATE_PAUSED;
        ex_mutex_unlock(__timer_mutex);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_resume_timer ( int _id ) {
    timer_t *t;

    if ( _id == EX_INVALID_TIMER_ID )
        return;

    t = (timer_t *)ex_pool_get ( &__timers, _id );
    if (t) {
        uint32 now;

        ex_mutex_lock(__timer_mutex);
            now = ex_timer_get_ticks();
            t->start = t->last_alarm = now;
            t->state = EX_TIMER_STATE_RUNNING;
        ex_mutex_unlock(__timer_mutex);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_reset_timer ( int _id ) {
    timer_t *t;

    if ( _id == EX_INVALID_TIMER_ID )
        return;

    t = (timer_t *)ex_pool_get ( &__timers, _id );
    if (t) {
        ex_mutex_lock(__timer_mutex);
            t->start = t->last_alarm = ex_timer_get_ticks();
            t->start_counter = t->delay; 
            t->interval_counter = t->interval; 
            t->lifetime_counter = t->lifetime;
            t->state = EX_TIMER_STATE_RUNNING;
        ex_mutex_unlock(__timer_mutex);
    }
}
