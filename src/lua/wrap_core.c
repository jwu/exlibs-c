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

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __log ( lua_State *_l ) {
    const char *msg = luaL_checkstring ( _l, 1 );
    ex_log ( "%s", msg );
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

static const luaL_Reg functions[] = {
    { "log", __log },
    { 0, 0 }
};

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int luaopen_core ( lua_State *_l ) {
    // we create global ex table if it not exists.
    ex_lua_global_module ( _l, "ex" );
    luaL_register( _l, 0, functions );
    return 0;
}
