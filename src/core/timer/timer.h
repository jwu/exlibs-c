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

///////////////////////////////////////////////////////////////////////////////
// decls
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline uint64 ex_cpu_cycle() {
#if (EX_PLATFORM == EX_WIN32) || (EX_PLATFORM == EX_XENON)
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return uint64(counter.QuadPart);
#elif (EX_PLATFORM == EX_PS3 )
    uint64 counter;
    __asm__ __volatile__( "mftb %0" : "=r" (counter) );
    return counter;
#else // unix-like sys (mac, apple, linux)
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000 + now.tv_usec/1000;
#endif
}

// ------------------------------------------------------------------
// Desc:
// ------------------------------------------------------------------

static inline uint64 ex_cpu_freq () {
#if (EX_PLATFORM == EX_WIN32) || (EX_PLATFORM == EX_XENON)
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    return uint64(freq.QuadPart);    
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
    ::Sleep(_ms);
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

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END TIMER_H_1293526038
// #################################################################################
