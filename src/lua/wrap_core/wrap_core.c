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
//
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// local function class_new ( _self, ... )
//     local table = ...
//     return setmetatable( table or {}, _self )
// end
// ------------------------------------------------------------------ 

static int __class_new ( lua_State *_l ) {
    lua_pushvalue(_l,1); // [-0,+1,-]
    if ( lua_isnil(_l,2) != 0 ) // if 2nd args is nil
        lua_newtable(_l); // [-0,+1,m]
    else
        lua_pushvalue(_l,2); // [-0,+1,-]
    lua_setmetatable(_l,-2); // [-1,+0,-] 
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __range_rand ( lua_State *_l ) {
    float low = luaL_checknumber(_l,1); 
    float high = luaL_checknumber(_l,2); 

    lua_pushnumber(_l, ex_range_randf(low,high) );
    return 1;
}

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

static int __deepcopy ( lua_State *_l ) {
    // TODO:
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// typeof = getmetatable
// ------------------------------------------------------------------ 

static int __type_of ( lua_State *_l ) {
    if ( lua_getmetatable(_l,1) == 0 )
        lua_pushnil(_l);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// function isclass (_class)
//     if type(_class) ~= "table" then 
//         return false
//     end
//     local mt = getmetatable(_class)
//     if mt == _R["ex.class.meta"] then return true end
//     return false
// end
// ------------------------------------------------------------------ 

static int __isclass ( lua_State *_l ) {
    // TODO: do we need manually pop those pushed table ????

    // if type(_class) ~= "table" then return false end
    if ( lua_istable( _l, 1 ) == 0 ) {
        lua_pushboolean(_l, false);
        return 1;
    }

    // local mt = getmetatable(_class)
    // if mt == nil then return false end
    if ( lua_getmetatable(_l,1) == 0 ) {
        lua_pushboolean(_l, false);
        return 1;
    }

    // if mt == _R["ex.class.meta"] then return true end
    // return false
    luaL_getmetatable(_l,"ex.class.meta");
    if ( lua_topointer(_l,-1) == lua_topointer(_l,-2) ) {
        lua_pushboolean(_l, true);
        return 1;
    }

    lua_pushboolean(_l, false);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// function class(...)
//     local base,super = ...
//     assert( type(base) == "table", "the first parameter must be a table" )
//
//     if super == nil then
//         rawset(base, "__super", nil)
//     else
//         assert( isclass(super), "super is not a class" )
//         rawset(base, "__super", super)
//     end
//
//     base.__index = class_index
//     base.__newindex = class_newindex
//     base.classof = classof
//     base.superof = superof
//     base.childof = childof
//     base.isa = isa
//     base.derive = function (_t)
//         return class( _t, base )
//     end
//     return setmetatable(base,class_meta)
// end
// ------------------------------------------------------------------ 

static int __class ( lua_State *_l ) {
}

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int luaopen_core ( lua_State *_l ) {
    // ex.class.meta functions
    static const luaL_Reg __meta_funcs[] = {
        { "__call", __class_new },
        { NULL, NULL },
    };
    // ex functions
    static const luaL_Reg __core_funcs[] = {
        { "range_rand", __range_rand }, // TODO: go to ex.random.range
        { "log", __log },
        // { "deepcopy", __deepcopy }, // TODO:
        { "typeof", __type_of }, // NOTE: c don't allow function named __typeof or typeof
        { "isclass", __isclass },
        { "class", __class },
        { 0, 0 }
    };

    //
    ex_lua_global_module ( _l, "ex" ); // [-0,+1,-]

    // register metatable ex.class.meta
    luaL_newmetatable(_l, "ex.class.meta"); // [-0,+1,m] // NOTE: this store a table in LUA_REGISTRYINDEX
    luaL_register(_l, NULL, __meta_funcs); // [-1,+1,m]
    lua_pop(_l, 1); // pops ex.class.meta [-1,+0,-]

    //
    luaL_register( _l, 0, __core_funcs ); // [-1,+1,m]
    lua_pop(_l, 1); // pops ex. [-1,+0,-]

    return 0;
}

//
void luaclose_core () {
}
