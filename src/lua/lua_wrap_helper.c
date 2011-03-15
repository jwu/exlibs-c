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

typedef struct __generic_proxy_t { 
    strid_t typeid;
    // NOTE: no mater what is below, the first two field are the same
} __generic_proxy_t; 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ref_proxy_t *ex_lua_pushref ( lua_State *_l, int _meta_idx ) {
    ref_proxy_t *u;

    u = (ref_proxy_t *)lua_newuserdata(_l, sizeof(ref_proxy_t));
    u->typeid = EX_TYPEID(ref);
    u->val = NULL;
    lua_pushvalue(_l,_meta_idx);
    lua_setmetatable(_l,-2);

    return u;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_lua_isref ( lua_State *_l, int _idx ) {
    ref_proxy_t *u;

    if ( lua_isuserdata(_l, _idx) == 0 )
        return false;

    u = (ref_proxy_t *)lua_touserdata(_l,_idx);
    if ( u->typeid != EX_TYPEID(ref) )
        return false;

    return true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_lua_toref ( lua_State *_l, int _idx ) {
    ref_proxy_t *u;
    u = (ref_proxy_t *)lua_touserdata(_l,_idx);
    return u->val;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_lua_checkref ( lua_State *_l, int _idx ) {
    if ( ex_lua_isref(_l,_idx) == false ) {
        luaL_error( _l, "invalid parameter type, expected ref" );
    }

    return ex_lua_toref(_l,_idx);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_lua_check_nullref ( lua_State *_l, ex_ref_t *_ref ) {
    if ( _ref->ptr == NULL ) {
        luaL_error( _l, "error: null reference" );
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_ref_gc ( lua_State *_l ) {
    ex_ref_t *r = ex_lua_checkref(_l,1);
    // DEBUG { 
    // if ( r->refcount == 1 ) {
    //     ex_log( "DEBUG: object %s destroyed!!!", ex_strid_to_cstr(ex_object_name(r)) ); 
    // } 
    // } DEBUG end 
    ex_decref(r);
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_ref_tostring ( lua_State *_l ) {
    void *obj;
    ex_string_t *str;
    ex_ref_t *r;

    r = ex_lua_checkref(_l,1);
    ex_lua_check_nullref(_l,r);

    obj = r->ptr;
    str = ex_string_alloc( "", 128 );
    ex_rtti_info(obj)->tostring( str, obj );
    lua_pushstring ( _l, str->text );
    ex_string_free(str);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_ref_concat ( lua_State *_l ) {
    void *obj;
    const char *lhs = luaL_checkstring(_l, 1); 
    ex_string_t *str = ex_string_alloc(lhs,256); 
    ex_ref_t *rhs = ex_lua_checkref(_l,2); 

    obj = rhs->ptr;
    ex_rtti_info(obj)->tostring(str, obj); 
    lua_pushstring(_l, str->text); 
    ex_string_free(str); 
    return 1; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_ref_eq ( lua_State *_l ) {
    ex_ref_t *lhs, *rhs;

    lhs = ex_lua_checkref(_l,1);

    // check if we compare with nil
    if ( lua_isnil(_l,2) ) {
        lua_pushboolean(_l,lhs->ptr == NULL);
        return 1;
    }

    // check if we compare with other ref
    rhs = ex_lua_checkref(_l,2);
    lua_pushboolean(_l,lhs == rhs);
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
    ex_assert( _supername, "_supername can't be NULL, put \"\" instead." );
    ex_assert( _typename, "_typename can't be NULL" );
    ex_assert( _meta_funcs, "_meta_funcs can't be NULL" );
    ex_assert( _type_meta_funcs, "_type_meta_funcs can't be NULL" );

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
    // NOTE: instead of use luaL_newmetatable(_l, _typename), we just create a table here, 
    //       but set it in LUA_REGISTRYINDEX by ex_lua_class below. 
    lua_newtable(_l);
    luaL_register( _l, NULL, _meta_funcs );

    // tp.__typename = "object"
    lua_pushstring(_l, _typename);
    lua_setfield(_l,-2,"__typename");

    // tp.__metaclass = { __call = _metacall_for_child }
    // NOTE: _metacall_for_child could be NULL
    if ( _metacall_for_child ) {
        lua_newtable(_l);
        lua_pushcfunction(_l, _metacall_for_child);
        lua_setfield(_l,-2,"__call");
        lua_setfield(_l,-2,"__metaclass");
    }

    // tp = ex.class ( tp )
    // get super metatable, if _supername is "", it will get nil
    luaL_getmetatable(_l,_supername);

    // now create type meta class
    lua_newtable(_l);
    luaL_register(_l, NULL, _type_meta_funcs);
    ex_lua_class( _l, 
                  lua_gettop(_l)-2, // base idx
                  lua_gettop(_l)-1, // super idx
                  lua_gettop(_l),   // meta idx
                  true // is builtin
                  );

    // _G[_field] = ex.class(tp) -- NOTE: here _G depends on what is the current table on the stack
    lua_pushvalue(_l,-1);
    lua_setfield(_l,-6,_field);

    lua_pop(_l, 4); // pops base, type_meta, super, base
    return 0;
}
