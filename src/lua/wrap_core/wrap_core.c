// ======================================================================================
// File         : wrap_core.c
// Author       : Wu Jie 
// Last Change  : 11/15/2010 | 13:36:19 PM | Monday,November
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __log ( lua_State *_l ) {
    const char *msg = luaL_checkstring ( _l, 1 );
    ex_log ( "%s", msg );
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __range_rand ( lua_State *_l ) {
    float low = luaL_checknumber(_l,1); 
    float high = luaL_checknumber(_l,2); 

    lua_pushnumber(_l, ex_range_randf(low,high) );
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

//
static const luaL_Reg __core_funcs[] = {
    { "log", __log },
    { "range_rand", __range_rand },
    { 0, 0 }
};

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int luaopen_core ( lua_State *_l ) {
    ex_lua_global_module ( _l, "ex" ); // [-0,+1,-]
    luaL_register( _l, 0, __core_funcs ); // [-1,+1,m]
    lua_pop(_l, 1); // pops ex. [-1,+0,-]

    return 0;
}

//
void luaclose_core () {
}
