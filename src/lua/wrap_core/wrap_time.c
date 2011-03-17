// ======================================================================================
// File         : wrap_time.c
// Author       : Wu Jie 
// Last Change  : 03/17/2011 | 13:09:41 PM | Thursday,March
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
// general defines
///////////////////////////////////////////////////////////////////////////////

EX_DEF_LUA_BUILTIN_MODULE()

///////////////////////////////////////////////////////////////////////////////
// type meta getset
///////////////////////////////////////////////////////////////////////////////

// // ------------------------------------------------------------------ 
// // Desc: 
// // ------------------------------------------------------------------ 

// static int __angf_get_zero ( lua_State *_l ) {
//     angf_proxy_t *u = ex_lua_pushangf(_l); 
//     u->val = ex_angf_zero;
//     return 1;
// }

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ex.angf.meta
static const ex_getset_t __type_meta_getsets[] = {
    // { "zero", __angf_get_zero, NULL },
    { NULL, NULL, NULL },
};
static const luaL_Reg __type_meta_funcs[] = {
    { "__newindex", __type_meta_newindex },
    { "__index", __type_meta_index },
    { NULL, NULL },
};
static const luaL_Reg __meta_funcs[] = {
    { NULL, NULL },
};

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int luaopen_time ( lua_State *_l ) {

    const ex_getset_t *getset;

    // init the type meta hashtable
    ex_hashmap_init ( &__key_to_type_meta_getset,
                      EX_STRID_NULL, sizeof(strid_t),
                      EX_STRID_NULL, sizeof(ex_getset_t),
                      32,
                      ex_hashkey_strid, ex_keycmp_strid,
                      __ex_hashmap_alloc,
                      __ex_hashmap_realloc,
                      __ex_hashmap_dealloc
                    );
    for ( getset = __type_meta_getsets; getset->key != NULL; ++getset ) {
        strid_t keyid = ex_strid(getset->key);
        ex_hashmap_set( &__key_to_type_meta_getset, &keyid, getset );
    }

    // we create global ex table if it not exists.
    // ex.time = {}
    // setmetatable( ex.time, { __type_meta_funcs } )
    ex_lua_global_module ( _l, "ex" );
    lua_newtable(_l);
    luaL_register(_l, NULL, __meta_funcs);
    lua_newtable(_l);
    luaL_register(_l, NULL, __type_meta_funcs);
    lua_setmetatable(_l,-2);
    lua_setfield(_l,-2,"time"); // _G[_field] = _R[_typename]
    lua_pop(_l, 1); // pops ex
    
    //
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void luaclose_time () {
    // deinit the hashtable
    ex_hashmap_deinit ( &__key_to_type_meta_getset );
}

