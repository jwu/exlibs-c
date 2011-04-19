// ======================================================================================
// File         : lua_behavior.h
// Author       : Wu Jie 
// Last Change  : 03/01/2011 | 15:33:24 PM | Tuesday,March
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef LUA_BEHAVIOR_H_1298964806
#define LUA_BEHAVIOR_H_1298964806
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "behavior.h"
    
///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// DELME { 
// #define MAX_INVOKES 4
// typedef struct ex_invoke_params_t {
//     ex_ref_t *self;
//     void *thread_state;
//     int lua_threadID;
//     int lua_funcID;
//     strid_t nameID;
// } ex_invoke_params_t;
// } DELME end 

// ------------------------------------------------------------------ 
/*! 
 @struct ex_lua_behavior_t
 @details
*/// ------------------------------------------------------------------ 

EX_DECL_CLASS_SUPER_BEGIN(ex_lua_behavior_t,ex_behavior_t)
    bool compile_failed;
    ex_hashmap_t *name_to_timer;
    ex_mutex_t *timer_mutex;

    // DELME { 
    // // for invokes
    // ex_mutex_t *invoke_mutex;
    // int num_invokes_to_call;
    // ex_invoke_params_t invokes_to_call[MAX_INVOKES]; // we accept MAX_INVOKES invokes in one frame
    // int num_invokes_to_stop;
    // ex_invoke_params_t invokes_to_stop[MAX_INVOKES]; // we accept MAX_INVOKES invokes in one frame
    // } DELME end 
EX_DECL_CLASS_SUPER_END(ex_lua_behavior_t,ex_behavior_t)

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_lua_behavior_invoke ( ex_ref_t *_self, 
                                     float _secs_delay, 
                                     float _secs_repeat,
                                     const char *_name,
                                     struct lua_State *_cur_state );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_lua_behavior_cancle_invoke ( ex_ref_t *_self, const char *_name );

// DELME { 
// // ------------------------------------------------------------------ 
// // Desc: invoke, yield ... 
// // ------------------------------------------------------------------ 

// extern void ex_lua_behavior_process_coroutine ( ex_ref_t *_self );
// } DELME end 

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END LUA_BEHAVIOR_H_1298964806
// #################################################################################
