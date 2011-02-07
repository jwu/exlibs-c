// ======================================================================================
// File         : wrap_vec2f.c
// Author       : Wu Jie 
// Last Change  : 02/07/2011 | 22:42:37 PM | Monday,February
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

typedef struct vec2f_proxy_t {
    strid_t typeid;
    ex_vec2f_t vec2f;
} vec2f_proxy_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_new ( lua_State *_l ) {
    vec2f_proxy_t *u;
    
    u = (vec2f_proxy_t *)lua_newuserdata(_l, sizeof(vec2f_proxy_t));
    u->typeid = EX_TYPEID(vec2f);
    ex_vec2f_set( &u->vec2f, lua_tonumber(_l,1), lua_tonumber(_l,2) );

    luaL_newmetatable(_l,"vec2f");
    lua_setmetatable(_l,-2);
    
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_type ( lua_State *_l ) {
    lua_pushstring(_l, "vec2f");
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_tostring ( lua_State *_l ) {
    ex_string_t *str = ex_string_alloc("",256);

    // TODO: check type, must be 1: userdata, 2: ex_vec2f_t { 
    if ( lua_isuserdata(_l, 1) == 0 ) {
        ex_error("invalid parameter type, expected userdata");
    }
    ex_vec2f_t *val = (ex_vec2f_t *)lua_touserdata(_l,1);
    // } TODO end 

    EX_RTTI(vec2f)->tostring(str, val);
    lua_pushstring(_l, str->text);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_add ( lua_State *_l ) {
    return 1;
}


///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

//
static const luaL_Reg __vec2f_funcs[] = {
    { "type", __vec2f_type },
    { "new", __vec2f_new },
    // { "__call", __vec2f_new },
    { "__tostring", __vec2f_tostring },
    { "__add", __vec2f_add },
    { 0, 0 }
};

//
int luaopen_vec2f ( lua_State *_l ) {

    // we create global ex table if it not exists.
    ex_lua_global_module ( _l, "ex" );

    // register vec2f
    luaL_newmetatable(_l, "vec2f");
    lua_pushvalue(_l, -1);
    lua_setfield(_l, -2, "__index");
    luaL_register(_l, NULL, __vec2f_funcs);
    lua_pop(_l, 1); // pops metatable.

    return 0;
}
