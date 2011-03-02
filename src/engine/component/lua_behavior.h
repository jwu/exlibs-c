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
    void *l; // lua state
    const char *lua_typename;
EX_DECL_CLASS_SUPER_END(ex_lua_behavior_t,ex_behavior_t)

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END LUA_BEHAVIOR_H_1298964806
// #################################################################################
