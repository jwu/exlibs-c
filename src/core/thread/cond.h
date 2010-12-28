// ======================================================================================
// File         : cond.h
// Author       : Wu Jie 
// Last Change  : 12/28/2010 | 18:47:18 PM | Tuesday,December
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef COND_H_1293533240
#define COND_H_1293533240
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

typedef struct ex_cond_t ex_cond_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern ex_cond_t* ex_create_cond ();

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_destroy_cond ( ex_cond_t* _cond );

// ------------------------------------------------------------------ 
// Desc: 
// Restart one of the threads that are waiting on the condition variable.
// return 0 or -1 on error.
// ------------------------------------------------------------------ 

extern int ex_cond_signal ( ex_cond_t* _cond );

// ------------------------------------------------------------------ 
// Desc: 
// Restart all threads that are waiting on the condition variable.
// return 0 or -1 on error.
// ------------------------------------------------------------------ 

extern int ex_cond_broadcast ( ex_cond_t* _cond );

// ------------------------------------------------------------------ 
// Desc: 
// Wait on the condition variable, unlocking the provided mutex.
// 
// \warning The mutex must be locked before entering this function!
// 
// The mutex is re-locked once the condition variable is signaled.
// 
// \return 0 when it is signaled, or -1 on error.
// ------------------------------------------------------------------ 

extern int ex_cond_wait ( ex_cond_t* _cond, ex_mutex_t* _mutex );

// ------------------------------------------------------------------ 
// Desc: 
// Waits for at most \c ms milliseconds, and returns 0 if the condition
// variable is signaled, ::SDL_MUTEX_TIMEDOUT if the condition is not
// signaled in the allotted time, and -1 on error.
//
// \warning On some platforms this function is implemented by looping with a 
//          delay of 1 ms, and so should be avoided if possible.
// ------------------------------------------------------------------ 

extern int ex_cond_wait_timeout ( ex_cond_t* _cond, ex_mutex_t* _mutex, uint32 _ms );


// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 


// #################################################################################
#endif // END COND_H_1293533240
// #################################################################################


