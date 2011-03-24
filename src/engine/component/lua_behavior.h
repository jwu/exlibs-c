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
/*! 
 @struct ex_lua_behavior_t
 @details
*/// ------------------------------------------------------------------ 

EX_DECL_CLASS_SUPER_BEGIN(ex_lua_behavior_t,ex_behavior_t)
    bool compile_failed;
    ex_hashmap_t *name_to_timer;
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
                                     int _refID );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END LUA_BEHAVIOR_H_1298964806
// #################################################################################
