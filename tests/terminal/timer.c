// ======================================================================================
// File         : test_timer.c
// Author       : Wu Jie 
// Last Change  : 01/06/2011 | 11:55:00 AM | Thursday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// include
///////////////////////////////////////////////////////////////////////////////

#include "core/core_inc.h"
#include "engine/time.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

static int32 _callback1 ( uint32 _interval, void *_param ) {
    printf("callback1 invoked! interval = %d\n", _interval);
    // EX_HW_BREAK();
    return _interval;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void normal () {
    // uint32 start,counter;
    int id = ex_add_timer( ex_timespan_from(0,500), _callback1, NULL, 0 );
    ex_start_timer(id);

    ex_sleep(2000);
    ex_pause_timer(id);
    ex_sleep(2000);
    ex_resume_timer(id);

    ex_sleep(4000);

    // start = counter = ex_timer_get_ticks();
    // while (1) {
    //     counter = ex_timer_get_ticks() - start;
    //     // printf( "seconds %f\n", (float)counter/1000.0f );
    //     ex_sleep(10);
    // }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void timescale () {
    // timer with timescale
    int id = ex_add_timer( ex_timespan_from(0,2000), _callback1, NULL, 0 );

    ex_start_timer(id); ex_log("start!");
    ex_sleep(4000);
    ex_set_timescale(0.5f);
    ex_sleep(5000);
    ex_set_timescale(1.0f);
    ex_remove_timer(id); ex_log("stop!");
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void lifetime () {
    uint32 start,counter;
    int id = ex_add_timer( ex_timespan_from(0,500), _callback1, NULL, 0 );
    ex_start_timer(id);

    start = counter = ex_timer_get_ticks();
    ex_sleep(5000);
    // while (1) {
    //     counter = ex_timer_get_ticks() - start;
    //     // printf( "seconds %f\n", (float)counter/1000.0f );
    //     ex_sleep(10);
    // }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void run_test () {
    normal();
    // timescale();
    // lifetime();
}
