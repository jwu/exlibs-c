// ======================================================================================
// File         : wrap_enums.c
// Author       : Wu Jie 
// Last Change  : 03/18/2011 | 11:34:57 AM | Friday,March
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "engine/component/trans2d.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int wrap_enums ( lua_State *_l ) {

    lua_pushstring(_l,"space");
    lua_newtable(_l); {
        lua_pushinteger( _l, EX_SPACE_SELF ); lua_setfield( _l, -2, "self" ); // NOTE: can't use local, it is a keyworkd in lua
        lua_pushinteger( _l, EX_SPACE_WORLD ); lua_setfield( _l, -2, "world" );
    } lua_rawset( _l, -3 );

    return 0;
}
