// ======================================================================================
// File         : lua_wrap_helper.c
// Author       : Wu Jie 
// Last Change  : 02/08/2011 | 12:32:52 PM | Tuesday,February
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

static int __gc ( lua_State *_l ) {
    // TODO { 
    // Proxy *p = (Proxy *)lua_touserdata(_l, 1);
    // Object *obj = (Object *)p->user_data;
    // if ( p->own_by_gc )
    //     obj->release();
    // } TODO end 
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __tostring ( lua_State *_l ) {
    // lua_pushvalue ( _l, lua_upvalueindex(1) );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __type ( lua_State *_l ) {
    lua_pushvalue ( _l, lua_upvalueindex(1) );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __type_of ( lua_State *_l ) {
    lua_pushvalue ( _l, lua_upvalueindex(1) );
    return 1;
}

#if 0
// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_register_builtin ( lua_State *_l, const char *_typeName, const void *_funcs ) {
    const luaL_Reg *reg_funcs = (const luaL_Reg *)_funcs;

    // lua: setmetatable()
    luaL_newmetatable(_l, _typeName);

    // m.__index = m
    lua_pushvalue(_l, -1);
    lua_setfield(_l, -2, "__index");

    // add tostring function.
    lua_pushcfunction(_l, __tostring);
    lua_setfield(_l, -2, "__tostring");

    // NOTE: builtin type no need for gc

    // add tostring to as type() as well.
    lua_pushstring(_l, _typeName);
    lua_pushcclosure(_l, __type, 1);
    lua_setfield(_l, -2, "type");

    // add typeof
    lua_pushcfunction(_l, __type_of);
    lua_setfield(_l, -2, "typeof");

    if ( reg_funcs != NULL )
        luaL_register(_l, 0, reg_funcs);

    lua_pop(_l, 1); // pops metatable.

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_register_class ( struct lua_State *_l, const char *_typeName, const void *_funcs ) {
    const luaL_Reg *reg_funcs = (const luaL_Reg *)_funcs;

    // lua: setmetatable()
    luaL_newmetatable(_l, _typeName);

    // m.__index = m
    lua_pushvalue(_l, -1);
    lua_setfield(_l, -2, "__index");

    // setup gc
    lua_pushcfunction(_l, __gc);
    lua_setfield(_l, -2, "__gc");

    // add tostring function.
    lua_pushstring(_l, _typeName);
    lua_pushcclosure(_l, __tostring, 1);
    lua_setfield(_l, -2, "__tostring");

    // add tostring to as type() as well.
    lua_pushstring(_l, _typeName);
    lua_pushcclosure(_l, __tostring, 1);
    lua_setfield(_l, -2, "type");

    // add typeof
    lua_pushcfunction(_l, __type_of);
    lua_setfield(_l, -2, "typeof");

    // add childof
    lua_pushcfunction(_l, __childof);
    lua_setfield(_l, -2, "childof");

    // add superof
    lua_pushcfunction(_l, __superof);
    lua_setfield(_l, -2, "superof");

    // add isa
    lua_pushcfunction(_l, __isa);
    lua_setfield(_l, -2, "isa");

    //
    if ( reg_funcs != NULL )
        luaL_register(_l, 0, reg_funcs);

    lua_pop(_l, 1); // pops metatable.

    return 0;
}
#endif
