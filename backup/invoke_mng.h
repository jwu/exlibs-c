// ======================================================================================
// File         : invoke_mng.h
// Author       : Wu Jie 
// Last Change  : 04/19/2011 | 10:21:17 AM | Tuesday,April
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef INVOKE_MNG_H_1303179710
#define INVOKE_MNG_H_1303179710
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// structure
///////////////////////////////////////////////////////////////////////////////

// ex_invoke_params_t
typedef struct ex_invoke_params_t {
    bool is_dead;
    float invoke_at;
    float repeat;
    strid_t nameID;
    void *thread_state;
    int lua_threadID;
    int lua_funcID;
} ex_invoke_params_t;

// ex_invoke_queue_t
#define MAX_INVOKES 32
typedef struct ex_invoke_queue_t {
    int count;
    int head;
    int trail;
    ex_invoke_params_t params_list[MAX_INVOKES];
} ex_invoke_queue_t;

// ex_invoke_mng_t
typedef struct ex_invoke_mng_t {

    ex_mutex_t *invoke_to_call_mutex;
    ex_mutex_t *invoke_to_stop_mutex;

    // invokes queue
    ex_invoke_queue_t invokes_to_call;
    ex_invoke_queue_t invokes_to_stop;

} ex_invoke_mng_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_invoke_mng_init ( ex_invoke_mng_t *_self );
extern void ex_invoke_mng_deinit ( ex_invoke_mng_t *_self );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_invoke_mng_add_to_call ( ex_invoke_mng_t *_self, ex_invoke_params_t *_params );
extern void ex_invoke_mng_add_to_stop ( ex_invoke_mng_t *_self, ex_invoke_params_t *_params );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_invoke_mng_process ( ex_invoke_mng_t *_self );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 


// #################################################################################
#endif // END INVOKE_MNG_H_1303179710
// #################################################################################

