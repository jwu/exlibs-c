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

    timespan_t interval;
    timespan_t lifetime; // if reach lifetime, the timer will stopped but not removed

    uint32 start;    // msecs
    uint32 counter;  // msecs
    uint32 last_alarm;

    ex_timer_pfn cb;
    void* params;
} timer_t;

int __timer_running = 0; // controlled by ex_timer_pause, ex_timer_resume
ex_pool_t* __timers = NULL; 
ex_array_t* __unused_timers = NULL; 

static bool __initialized = false;
static ex_mutex_t* __timer_mutex = NULL;

extern bool ex_sys_timer_init ();
extern void ex_sys_timer_deinit ();

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void __threaded_timer_tick () {
    uint32 now, ms;

    ex_mutex_lock(__timer_mutex);
    now = ex_timer_get_ticks();
    ex_pool_id_each ( __timers, timer_t, t, id ) {
        ms = t.interval - EX_TIMER_SLICE;

        // TODO: lifetime (should consider pause, stopped...)
        // TODO: pause, stopped ...

        // if we exceed the interval
        if ( (int)(now - t.last_alarm) > (int)ms ) {
            if ( (now - t.last_alarm) < t.interval )
                t.last_alarm += t.interval;
            else
                t.last_alarm = now;

            // execute the timer callback
            ex_mutex_unlock(__timer_mutex);
            ms = t.cb(t.interval, t.params);
            ex_mutex_lock(__timer_mutex);

            // process return interval
            if (ms != t.interval) {
                // if ms not zero round it and set as the next interval.
                if ( ms != 0 )
                    t.interval = __ROUND_RESOLUTION(ms);
                else
                    ex_remove_timer(id);
            }
        }

    } ex_pool_each_end

    // remove all timer here
    ex_array_each ( __unused_timers, int, id ) {
        ex_pool_remove_at_safe ( __timers, id ); // this can work around if we got several same id to remove.
    } ex_array_each_end
    ex_array_remove_all(__unused_timers);
    ex_mutex_unlock(__timer_mutex);
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_timer_init () {
    bool ret = false;

    // if the core already inited, don't init it second times.
    if ( __initialized ) {
        ex_warning ( "timer already inited" );
        return true;
    }

    // init timer
    ret = ex_sys_timer_init();
    __timer_mutex = ex_create_mutex();
    if ( ret == false )
        return false;

    //
    __timers = ex_pool_alloc_nomng( sizeof(timer_t), 8 ); 
    __unused_timers = ex_array_alloc_nomng ( sizeof(int), 32 ); 

    // now start the timer
    ex_timer_resume();

    //
    __initialized = true;
    return true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_timer_deinit () {

    if ( __initialized ) {
        // we must stop all timers before we remove them.
        ex_timer_pause();

        // free params in timer
        ex_pool_each ( __timers, timer_t, t ) {
            ex_free_nomng (t.params);
        } ex_pool_each_end

        // clear all timers
        ex_pool_free_nomng(__timers);
        ex_array_free_nomng(__unused_timers);

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

void ex_timer_pause () { --__timer_running; }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_timer_resume () { ++__timer_running; }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_add_timer ( ex_timer_pfn _cb, void* _params, size_t _size, 
                   timespan_t _interval,
                   timespan_t _lifetime ) {
    //
    timer_t newTimer;

    // check the inputs
    ex_assert_return ( _cb != NULL, -1, "callback function can't not be NULL" );
    ex_assert_return ( _interval != 0, -1, "interval can't be 0, the minimum value is 1" );
    ex_assert_return ( _lifetime != 0, -1, "lifetime can't be 0, the minimum value is 1, if you don't need lifetime, put it to EX_TIMESPAN_INFINITY" );

    // init new timer
    newTimer.state = EX_TIMER_STATE_STOPPED;
    newTimer.counter = 0;
    newTimer.interval = __ROUND_RESOLUTION(_interval);
    newTimer.lifetime = (_lifetime == EX_TIMESPAN_INFINITY) ? EX_TIMESPAN_INFINITY : __ROUND_RESOLUTION(_lifetime);
    newTimer.start = newTimer.counter = ex_timer_get_ticks();
    newTimer.cb = _cb;
    if ( _params ) {
        newTimer.params = ex_malloc_nomng(_size);
        memcpy( newTimer.params, _params, _size );
    } else {
        newTimer.params = NULL;
    }

    //
    int id = ex_pool_insert ( __timers, &newTimer );
    return id;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_remove_timer ( int _id ) {
    bool validTimer = ex_pool_isvalid ( __timers, _id );

    // the timer already removed
    if ( !validTimer )
        return false;

    ex_array_append_int32(__unused_timers, _id);
    return true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_start_timer ( int _id ) {
    timer_t* t = (timer_t*)ex_pool_get ( __timers, _id );
    if (t) {
        ex_mutex_lock(__timer_mutex);
        t->state = EX_TIMER_STATE_RUNNING;
        ex_mutex_unlock(__timer_mutex);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_stop_timer ( int _id ) {
    timer_t* t = (timer_t*)ex_pool_get ( __timers, _id );
    if (t) {
        ex_mutex_lock(__timer_mutex);
        t->state = EX_TIMER_STATE_STOPPED;
        ex_mutex_unlock(__timer_mutex);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_pause_timer ( int _id ) {
    timer_t* t = (timer_t*)ex_pool_get ( __timers, _id );
    if (t) {
        ex_mutex_lock(__timer_mutex);
        t->state = EX_TIMER_STATE_PAUSED;
        ex_mutex_unlock(__timer_mutex);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_resume_timer ( int _id ) {
    timer_t* t = (timer_t*)ex_pool_get ( __timers, _id );
    if (t) {
        ex_mutex_lock(__timer_mutex);
        t->state = EX_TIMER_STATE_RUNNING;
        ex_mutex_unlock(__timer_mutex);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_reset_timer ( int _id ) {
    timer_t* t = (timer_t*)ex_pool_get ( __timers, _id );
    if (t) {
        ex_mutex_lock(__timer_mutex);
        t->state = EX_TIMER_STATE_RESET;
        ex_mutex_unlock(__timer_mutex);
    }
}