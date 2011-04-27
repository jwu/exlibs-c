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

#define EX_YIELD_FINISHED               0
#define EX_YIELD_WAIT_FOR_SECONDS       1
#define EX_YIELD_WAIT_FOR_FINISH        2
#define EX_YIELD_WAIT_FOR_END_OF_FRAME  3
#define EX_YIELD_WAIT_FOR_ONE_FRAME     4

///////////////////////////////////////////////////////////////////////////////
// structure
///////////////////////////////////////////////////////////////////////////////

// ex_coroutine_info_t
typedef struct ex_coroutine_info_t {
    void *thread_state;
    int yield_status;
    int timerID;
    uint32 cur_frame;
} ex_coroutine_info_t;

// ex_coroutine_params_t
typedef struct ex_coroutine_params_t {
    ex_ref_t *beref;
    void *parent_state;
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
    // TODO: give a proper name { 
    // time up
    ex_mutex_t *coroutine_timeup_mutex;
    ex_coroutine_queue_t coroutines_timeup;
    // } TODO end 

    // TODO { 
    // // next frame
    // ex_mutex_t *coroutine_to_call_nf_mutex;
    // ex_coroutine_queue_t coroutines_to_call_nf;

    // // end of frame
    // ex_mutex_t *coroutine_to_call_eof_mutex;
    // ex_coroutine_queue_t coroutines_to_call_eof;
    // } TODO end 
} ex_coroutine_mng_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_coroutine_mng_init ( ex_coroutine_mng_t *_self );
extern void ex_coroutine_mng_deinit ( ex_coroutine_mng_t *_self );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_coroutine_mng_add_timeup ( ex_coroutine_mng_t *_self, ex_coroutine_params_t *_params );

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


