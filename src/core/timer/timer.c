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

int ex_timer_started = 0;
int ex_timer_running = 0;

static bool __initialized = false;
static ex_mutex_t* __timer_mutex = NULL;

extern bool ex_sys_timer_init ();
extern void ex_sys_timer_deinit ();

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
    if ( ret )
        ex_timer_started = 1;

    //
    __initialized = true;
    return true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_timer_deinit ()
{
    if ( __initialized ) {

        // TODO: clear all timers

        //
        ex_sys_timer_deinit();
        ex_destroy_mutex (__timer_mutex);
        __timer_mutex = NULL;
        ex_timer_started = 0;

        //
        __initialized = false;
    }
}
