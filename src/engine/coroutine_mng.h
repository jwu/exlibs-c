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
// yield status
///////////////////////////////////////////////////////////////////////////////

#define EX_YIELD_WAIT_FOR_SECONDS       1
#define EX_YIELD_WAIT_FOR_FINISH        2
#define EX_YIELD_WAIT_FOR_END_OF_FRAME  3
#define EX_YIELD_WAIT_FOR_ONE_FRAME     4

///////////////////////////////////////////////////////////////////////////////
// structure
///////////////////////////////////////////////////////////////////////////////

// ex_coroutine_info_t
typedef struct ex_coroutine_info_t {
    int timerID;
    int threadID;
} ex_coroutine_info_t;

// ex_coroutine_params_t
typedef struct ex_coroutine_params_t {
    bool do_stop;
    ex_ref_t *beref;
    void *thread_state;
    int lua_threadID;
    strid_t nameID;
} ex_coroutine_params_t;

// ex_coroutine_queue_t
#define MAX_COROUTINES 1024
typedef struct ex_coroutine_queue_t {
    int count;
    int head;
    int trail;
    ex_coroutine_params_t params_list[MAX_COROUTINES];
} ex_coroutine_queue_t;

// ex_coroutine_mng_t
typedef struct ex_coroutine_mng_t {

    ex_mutex_t *coroutine_to_stop_mutex;
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


