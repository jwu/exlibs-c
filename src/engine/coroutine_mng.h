// ======================================================================================
// File         : coroutine_mng.h
// Author       : Wu Jie 
// Last Change  : 04/21/2011 | 15:27:12 PM | Thursday,April
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef COROUTINE_MNG_H_1303370836
#define COROUTINE_MNG_H_1303370836
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// structure
///////////////////////////////////////////////////////////////////////////////

// ex_coroutine_params_t
typedef struct ex_coroutine_params_t {
    ex_ref_t *self;
    void *thread_state;
    int lua_threadID;
    int lua_funcID;
    strid_t nameID;
} ex_coroutine_params_t;

// ex_coroutine_queue_t
#define MAX_coroutineS 1024
typedef struct ex_coroutine_queue_t {
    int count;
    int head;
    int trail;
    ex_coroutine_params_t params_list[MAX_coroutineS];
} ex_coroutine_queue_t;

// ex_coroutine_mng_t
typedef struct ex_coroutine_mng_t {

    ex_mutex_t *coroutine_to_call_mutex;
    ex_mutex_t *coroutine_to_stop_mutex;

    // coroutines queue
    ex_coroutine_queue_t coroutines_to_call;
    ex_coroutine_queue_t coroutines_to_stop;

} ex_coroutine_mng_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_coroutine_mng_init ( ex_coroutine_mng_t *_self );
extern void ex_coroutine_mng_deinit ( ex_coroutine_mng_t *_self );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_coroutine_mng_add_to_call ( ex_coroutine_mng_t *_self, ex_coroutine_params_t *_params );
extern void ex_coroutine_mng_add_to_stop ( ex_coroutine_mng_t *_self, ex_coroutine_params_t *_params );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_coroutine_mng_process ( ex_coroutine_mng_t *_self );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 


// #################################################################################
#endif // END COROUTINE_MNG_H_1303370836
// #################################################################################


