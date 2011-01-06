// ======================================================================================
// File         : test_timer.c
// Author       : Wu Jie 
// Last Change  : 01/06/2011 | 11:55:00 AM | Thursday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// include
///////////////////////////////////////////////////////////////////////////////

#include "../../core/core_inc.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

static int32 _callback1 ( uint32 _interval, void* _param ) {
    printf("callback1 invoked! interval = %d\n", _interval);
    // EX_HW_BREAK();
    return _interval;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void normal () {
    uint32 start,counter;
    int id = ex_add_timer( _callback1, NULL, 0, ex_timespan_from(0,500), EX_TIMESPAN_INFINITY );
    ex_start_timer(id);

    ex_sleep(2000);
    ex_pause_timer(id);
    ex_sleep(2000);
    ex_resume_timer(id);

    start = counter = ex_timer_get_ticks();
    while (1) {
        counter = ex_timer_get_ticks() - start;
        // printf( "seconds %f\n", (float)counter/1000.0f );
        ex_sleep(10);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_timer () {
    normal();
}
