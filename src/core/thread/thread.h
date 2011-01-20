// ======================================================================================
// File         : thread.h
// Author       : Wu Jie 
// Last Change  : 06/30/2010 | 14:33:55 PM | Wednesday,June
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef THREAD_H_1277879636
#define THREAD_H_1277879636
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

#if ( EX_PLATFORM == EX_WIN32 )
    typedef HANDLE thread_handle_t;
#else
    typedef pthread_t thread_handle_t;
#endif

// ------------------------------------------------------------------ 
// Desc: ex_thread_t 
// ------------------------------------------------------------------ 

typedef struct ex_thread_t {
    thread_id_t threadID;
    thread_handle_t handle;
    int status;
    void *data;
} ex_thread_t;
typedef int (*ex_thread_pfn_t) (void *_data);

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline thread_id_t ex_current_threadID () {
#if ( EX_PLATFORM == EX_WIN32 )
    return (thread_id_t)GetCurrentThreadId();
#else
    return (thread_id_t)pthread_self();
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern ex_thread_t *ex_create_thread ( ex_thread_pfn_t _fn, void *_data );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_wait_thread ( ex_thread_t *_thread, int *_status );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END THREAD_H_1277879636
// #################################################################################


