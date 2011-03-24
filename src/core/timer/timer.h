// ======================================================================================
// File         : timer.h
// Author       : Wu Jie 
// Last Change  : 12/28/2010 | 16:47:16 PM | Tuesday,December
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef TIMER_H_1293526038
#define TIMER_H_1293526038
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

#define EX_TIMER_STATE_RUNNING   1
#define EX_TIMER_STATE_PAUSED    2
#define EX_TIMER_STATE_STOPPED   3

#define EX_TIMER_SLICE      10
#define EX_TIMER_RESOLUTION 10

#define EX_INVALID_TIMER_ID -1

// ------------------------------------------------------------------ 
// Desc: return the new interval, if 0 the timer will be removed. 
// ------------------------------------------------------------------ 

typedef int32 (*ex_timer_pfn) ( uint32 _interval, void *_param );
typedef void (*ex_timer_stop_pfn) ( void *_param );

///////////////////////////////////////////////////////////////////////////////
// decls
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// returns usecs since the process start up.
// ------------------------------------------------------------------ 

static inline uint64 ex_cpu_cycle() {
#if (EX_PLATFORM == EX_WIN32) || (EX_PLATFORM == EX_XENON)
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (uint64)(counter.QuadPart);
#elif (EX_PLATFORM == EX_PS3 )
    uint64 counter;
    __asm__ __volatile__( "mftb %0" : "=r" (counter) );
    return counter;
#else // unix-like sys (mac, apple, linux)
    ex_warning( "please implement it in unix!" );
    return 0;
#endif
}

// ------------------------------------------------------------------
// Desc:
// ------------------------------------------------------------------

static inline uint64 ex_cpu_freq () {
#if (EX_PLATFORM == EX_WIN32) || (EX_PLATFORM == EX_XENON)
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    return (uint64)(freq.QuadPart);    
#elif (EX_PLATFORM == EX_PS3 )
    return sys_time_get_timebase_frequency();
#else
    ex_warning( "please define a default frequency for other platform" );
    return 0;
#endif
}

// ------------------------------------------------------------------
// Desc: 
// ------------------------------------------------------------------

static inline void ex_sleep ( uint32 _ms ) {
#if (EX_PLATFORM == EX_WIN32) || (EX_PLATFORM == EX_XENON)
    Sleep(_ms);
#elif (EX_PLATFORM == EX_PS3) 
    sys_timer_usleep( _ms * 1000 );
#else
    int was_error;
    struct timespec elapsed, tv;
    elapsed.tv_sec = _ms / 1000;
    elapsed.tv_nsec = (_ms % 1000) * 1000000;

    do {
        errno = 0;
        tv.tv_sec = elapsed.tv_sec;
        tv.tv_nsec = elapsed.tv_nsec;
        was_error = nanosleep(&tv, &elapsed);
    } while (was_error && (errno == EINTR));
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// success: 0
// already inited: 1
// failed: -1
extern int ex_timer_init ();
extern void ex_timer_deinit ();
extern bool ex_timer_initialized ();

// ------------------------------------------------------------------ 
// Desc: return ms 
// ------------------------------------------------------------------ 

extern uint32 ex_timer_get_ticks ();

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern int ex_add_timer ( ex_timer_pfn _cb, 
                          ex_timer_stop_pfn _on_stop,
                          void *_params, 
                          size_t _size, /*parameter byte-size*/ 
                          timespan_t _delay,
                          timespan_t _interval,
                          timespan_t _lifetime /*EX_TIMESPAN_INFINITY*/ );
extern bool ex_remove_timer ( int _id );

extern void ex_start_timer ( int _id );
extern void ex_stop_timer ( int _id );
extern void ex_pause_timer ( int _id );
extern void ex_resume_timer ( int _id );
extern void ex_reset_timer ( int _id );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END TIMER_H_1293526038
// #################################################################################
