// ======================================================================================
// File         : timer_win32.c
// Author       : Wu Jie 
// Last Change  : 01/05/2011 | 20:48:02 PM | Wednesday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

static UINT __timerID = 0;
static LARGE_INTEGER __start_ticks;
static LARGE_INTEGER __freq;

// ------------------------------------------------------------------ 
// Desc: 
extern void __threaded_timer_tick ();
// ------------------------------------------------------------------ 

static void CALLBACK HandleAlarm ( UINT uID, 
                                   UINT uMsg, 
                                   DWORD_PTR dwUser, 
                                   DWORD_PTR dw1, 
                                   DWORD_PTR dw2 )
{
    __threaded_timer_tick();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_sys_timer_init () {
    MMRESULT result;

    // init start ticks and cpu frequency
    QueryPerformanceFrequency(&__freq);
    QueryPerformanceCounter(&__start_ticks);

    // Set timer resolution
    result = timeBeginPeriod(EX_TIMER_RESOLUTION);
    if ( result != TIMERR_NOERROR ) {
        ex_error("Warning: Can't set %d ms timer resolution", EX_TIMER_RESOLUTION);
    }

    // Allow 10 ms of drift so we don't chew on CPU
    __timerID = timeSetEvent(EX_TIMER_RESOLUTION, 1, HandleAlarm, 0, TIME_PERIODIC);
    if ( !__timerID ) {
        ex_error("timeSetEvent() failed");
        return false;
    }
    return true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_sys_timer_deinit () {
    if ( __timerID ) {
        timeKillEvent(__timerID);
    }
    timeEndPeriod(EX_TIMER_RESOLUTION);
}

// ------------------------------------------------------------------ 
// Desc: return ms 
// ------------------------------------------------------------------ 

uint32 ex_timer_get_ticks () {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    now.QuadPart -= __start_ticks.QuadPart;
    now.QuadPart *= 1000;
    now.QuadPart /= __freq.QuadPart;

    return (DWORD) now.QuadPart;
}
