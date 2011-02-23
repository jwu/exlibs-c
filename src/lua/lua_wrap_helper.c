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

int ex_lua_ref_gc ( lua_State *_l ) {
    ref_proxy_t *u;

    if ( lua_isuserdata(_l,1) == 0 ) {
        ex_error ( "fatal error: the gc object is not an userdata" );
        return 0;
    }

    u = (ref_proxy_t *)lua_touserdata(_l,1);
    ex_decref(u->ref);

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_ref_tostring ( lua_State *_l ) {
    ref_proxy_t *u;
    void *obj;
    ex_string_t *str;

    if ( lua_isuserdata(_l,1) == 0 ) {
        ex_error ( "fatal error: the gc object is not an userdata" );
        lua_pushstring(_l,"");
        return 1;
    }

    u = (ref_proxy_t *)lua_touserdata(_l,1);
    obj = u->ref->ptr;
    str = ex_string_alloc( "", 128 );
    ex_rtti_info(obj)->tostring( str, obj );
    lua_pushstring ( _l, str->text );
    ex_string_free(str);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_ref_eq ( struct lua_State *_l ) {
    ref_proxy_t *lhs, *rhs;

    if ( lua_isuserdata(_l,1) == 0 ) {
        ex_error ( "fatal error: the gc object is not an userdata" );
        lua_pushstring(_l,"");
        return 1;
    }
    lhs = (ref_proxy_t *)lua_touserdata(_l,1);

    // check if we compare with nil
    if ( lua_isnil(_l,2) ) {
        lua_pushboolean(_l,lhs->ref->ptr == NULL);
        return 1;
    }

    // check if we compare with other ref
    rhs = (ref_proxy_t *)lua_touserdata(_l,2);
    lua_pushboolean(_l,lhs->ref->ptr == rhs->ref->ptr);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// TODO: we could do it even bettern if we can anlysis _field by split them by "."
// ------------------------------------------------------------------ 

int ex_lua_register_builtin ( lua_State *_l, 
                              const char *_field, 
                              const char *_typename, 
                              const void *_meta_funcs,
                              const void *_type_meta_funcs ) 
{
    ex_assert( _field, "_field can't be NULL" );
    ex_assert( _typename, "_typename can't be NULL" );
    ex_assert( _meta_funcs, "_meta_funcs can't be NULL" );
    ex_assert( _type_meta_funcs, "_type_meta_funcs can't be NULL" );

    /**
     * if
          _typename = "ex.vec2f"
          _field = "vec2f"

       and we add it in global table "ex",
       then the table looks like this:

          ex.vec2f = _R["ex.vec2f"] = {
              meta = {
                  __call = ...;
                  __index = ...;
                  __newindex = ...;
              };
              __index = ...;
              __newindex = ...;
          };
     */

    // the lua code
    // _field = "vec2f"
    // _typename = "ex.vec2f"
    // _R[_typename] = { _meta_funcs } --  do register
    // _R[_typename].__typename = _typename
    // _R[_typename].__isbuiltin = true
    // setmetatable ( _R[_typename], { _type_meta_funcs } )
    // _G[_field] = _R[_typename] -- NOTE: _G depends on the table on the stack

    // register metatable -- _typename
    // _R[_typename] = { _meta_funcs }
    luaL_newmetatable(_l, _typename); // NOTE: this store a table in LUA_REGISTRYINDEX
    luaL_register(_l, NULL, _meta_funcs);

    // tp.__typename = _typename
    // NOTE: the __typename is for print (ex.typename(v1)), for faster typecheck, 
    //       use ex.typeof(v1) == ex.vec2f is faster instead.
    lua_pushstring(_l, _typename);
    lua_setfield(_l,-2,"__typename");

    //  tp.__isbuiltin = true, used in isbuiltin(v1)
    lua_pushboolean(_l, true);
    lua_setfield(_l,-2,"__isbuiltin");

    // setmetatable( tp, { _type_meta_funcs } )
    lua_newtable(_l);
    luaL_register(_l, NULL, _type_meta_funcs);
    lua_setmetatable(_l,-2);

    lua_setfield(_l,-2,_field); // _G[_field] = _R[_typename]
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// TODO: we could do it even bettern if we can anlysis _field by split them by "."
// ------------------------------------------------------------------ 

int ex_lua_register_class ( lua_State *_l, 
                            const char *_field, 
                            const char *_supername, 
                            const char *_typename, 
                            const void *_meta_funcs,
                            const void *_type_meta_funcs,
                            const void *_metacall_for_child ) 
{
    ex_assert( _field, "_field can't be NULL" );
    ex_assert( _typename, "_typename can't be NULL" );
    ex_assert( _meta_funcs, "_meta_funcs can't be NULL" );
    ex_assert( _type_meta_funcs, "_type_meta_funcs can't be NULL" );
    ex_assert( _metacall_for_child, "_metacall_for_child can't be NULL" );

    // the lua code
    // _field = "object"
    // _typename = "ex.object"
    // _R[_typename] = { _meta_funcs } --  do register
    // _R[_typename].__typename = _field
    // _R[_typename].__isbuiltin = true
    // _R[_typename].__isclass = true
    // _R[_typename].__metaclass = { __call = _metacall_for_child }
    // _G[_field] = ex.class(_R[_typename]) -- NOTE: here _G depends on what is the current table on the stack
    //                 |- _R[_typename].__super = nil -- TODO: we should have super I think
    //                 |- setmetatable ( _R[_typename], { _type_meta_funcs } )

    //
    luaL_newmetatable(_l, _typename); // NOTE: this store a table in LUA_REGISTRYINDEX
    luaL_register( _l, NULL, _meta_funcs );

    // tp.__typename = "object"
    lua_pushstring(_l, _typename);
    lua_setfield(_l,-2,"__typename");

    // tp.__isbuiltin = true
    lua_pushboolean(_l, true);
    lua_setfield(_l,-2,"__isbuiltin");

    // tp.__meta_for_child = { __call = _metacall_for_child }
    lua_newtable(_l);
    lua_pushcfunction(_l, _metacall_for_child);
    lua_setfield(_l,-2,"__call");
    lua_setfield(_l,-2,"__metaclass");

    // tp = ex.class ( tp )
    if ( _supername == NULL ) {
        lua_pushnil(_l); // super is nil
    }
    else {
        luaL_newmetatable(_l,_supername); // get super metatable
    }

    // now create type meta class
    lua_newtable(_l);
    luaL_register(_l, NULL, _type_meta_funcs);
    ex_lua_class( _l, 
                  lua_gettop(_l)-2, // base idx
                  lua_gettop(_l)-1, // super idx
                  lua_gettop(_l),   // meta idx
                  NULL, 
                  NULL );

    // _G[_field] = ex.class(tp) -- NOTE: here _G depends on what is the current table on the stack
    lua_pushvalue(_l,-1);
    lua_setfield(_l,-6,_field);

    lua_pop(_l, 4); // pops base, type_meta, super, base
    return 0;
}
