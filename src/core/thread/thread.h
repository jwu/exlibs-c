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

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern inline uint32 ex_current_threadID () {
#if ( EX_PLATFORM == EX_WIN32 )
    return (uint32)GetCurrentThreadId();
#else
    ex_assert ( false, "pls implement ex_current_threadID in other platform." );
    return -1;
#endif
}

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 


// #################################################################################
#endif // END THREAD_H_1277879636
// #################################################################################


