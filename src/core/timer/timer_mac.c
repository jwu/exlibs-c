// ======================================================================================
// File         : timer_mac.c
// Author       : Wu Jie 
// Last Change  : 12/29/2010 | 10:11:37 AM | Wednesday,December
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

static int __timer_alive = 0;
static ex_thread_t* __timer = NULL;

extern int ex_timer_started;
extern int ex_timer_running;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __threaded_timer_tick () {
    // TODO:
    // you should use list timer.
    // you should remove timer at the end of the tick.
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __run_timer ( void* _unused ) {
    while ( __timer_alive ) {
        if ( ex_timer_running ) {
            __threaded_timer_tick ();
        }
        ex_sleep(1);
    }
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_sys_timer_init () {
    __timer_alive = 1;
    __timer = ex_create_thread ( __run_timer, NULL ); 
    if ( __timer == NULL )
        return false;
    return true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_sys_timer_deinit () {
    __timer_alive = 0;
    if ( __timer ) {
        ex_wait_thread ( __timer, NULL );
        __timer = NULL;
    }
}
