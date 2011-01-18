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
static ex_thread_t* __timer_thread = NULL;
static struct timeval __start_ticks;


// ------------------------------------------------------------------ 
// Desc: 
extern int __timer_running;
extern void __threaded_timer_tick ();
// ------------------------------------------------------------------ 

static int __run_timer ( void* _unused ) {
    while ( __timer_alive ) {
        if ( __timer_running ) {
            __threaded_timer_tick ();
        }
        ex_sleep(1);
    }
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_sys_timer_init () {
    // init start ticks
    gettimeofday(&__start_ticks, NULL);

    //
    __timer_alive = 1;
    __timer_thread = ex_create_thread ( __run_timer, NULL ); 
    if ( __timer_thread == NULL )
        return -1;
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_sys_timer_deinit () {
    __timer_alive = 0;
    if ( __timer_thread ) {
        ex_wait_thread ( __timer_thread, NULL );
        __timer_thread = NULL;
    }
}

// ------------------------------------------------------------------ 
// Desc: return ms 
// ------------------------------------------------------------------ 

uint32 ex_timer_get_ticks () {
    uint32 ticks;
    struct timeval now;
    gettimeofday(&now, NULL);
    ticks = (now.tv_sec - __start_ticks.tv_sec) * 1000 
        + (now.tv_usec - __start_ticks.tv_usec) / 1000;
    return (ticks);
}
