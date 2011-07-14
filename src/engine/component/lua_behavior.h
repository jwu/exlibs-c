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

#define EX_YIELD_FINISHED               0
#define EX_YIELD_WAIT_FOR_SECONDS       1
#define EX_YIELD_WAIT_FOR_FINISH        2
#define EX_YIELD_WAIT_FOR_END_OF_FRAME  3
#define EX_YIELD_WAIT_FOR_ONE_FRAME     4

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

typedef struct ex_invoke_info_t {
    bool is_dead;
    float remain;
    float repeat;
    strid_t nameID;
    void *lua_state;
    int lua_funcID;
} ex_invoke_info_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

typedef struct ex_coroutine_info_t {
    bool is_dead;

    //
    strid_t nameID;
    void *parent_state;
    void *thread_state;
    int lua_threadID;

    //
    int yield_status;
    uint32 cur_frame;
    float wait_for_secs;
} ex_coroutine_info_t;

// ------------------------------------------------------------------ 
/*! 
 @struct ex_lua_behavior_t
 @details
*/// ------------------------------------------------------------------ 

EX_DECL_CLASS_SUPER_BEGIN(ex_lua_behavior_t,ex_behavior_t)
    bool compile_failed;
    ex_array_t *invoke_list;
    ex_array_t *coroutine_list;
EX_DECL_CLASS_SUPER_END(ex_lua_behavior_t,ex_behavior_t)

#define EX_LUA_BEHAVIOR_DEFAULT_MEMBER \
    EX_BEHAVIOR_DEFAULT_MEMBER \
    EX_MEMBER( ex_lua_behavior_t, compile_failed, false ) \
    EX_MEMBER( ex_lua_behavior_t, invoke_list, ex_array_notype(sizeof(ex_invoke_info_t),16) ) \
    EX_MEMBER( ex_lua_behavior_t, coroutine_list, ex_array_notype(sizeof(ex_coroutine_info_t),16) )

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_lua_behavior_invoke ( ex_ref_t *_self, 
                                     struct lua_State *_cur_state,
                                     float _secs_delay, 
                                     float _secs_repeat,
                                     const char *_name );
extern void ex_lua_behavior_cancle_invoke ( ex_ref_t *_self, const char *_name );
extern bool ex_lua_behavior_is_invoking ( ex_ref_t *_self, const char *_name );
extern void ex_lua_behavior_process_invokes ( ex_ref_t *_self );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern int ex_lua_behavior_start_coroutine ( ex_ref_t *_self, 
                                             struct lua_State *_cur_state,
                                             const char *_name,
                                             int _nargs );
extern int ex_lua_behavior_stop_coroutine ( ex_ref_t *_self, const char *_name );
extern void ex_lua_behavior_process_coroutines ( ex_ref_t *_self );
extern void ex_lua_behavior_process_coroutines_eof ( ex_ref_t *_self );
extern ex_coroutine_info_t *ex_lua_behavior_get_coroutine_info ( ex_ref_t *_self, struct lua_State *_state );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END LUA_BEHAVIOR_H_1298964806
// #################################################################################
