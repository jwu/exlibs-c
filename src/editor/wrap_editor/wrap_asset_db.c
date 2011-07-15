// ======================================================================================
// File         : wrap_asset_db.c
// Author       : Wu Jie 
// Last Change  : 07/15/2011 | 09:37:03 AM | Friday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "engine/engine_inc.h"

#include "editor/asset_db.h"

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

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __asset_db_load ( lua_State *_l ) {
    const char *path;
    ex_ref_t *ref;

    path = luaL_checkstring(_l,1);
    ref = ex_asset_db_load( path );
    ex_lua_pushobject (_l,ref);
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ex.asset_db.meta
static const ex_getset_t __type_meta_getsets[] = {
    { NULL, NULL, NULL },
};
static const luaL_Reg __type_meta_funcs[] = {
    { "__newindex", __type_meta_newindex },
    { "__index", __type_meta_index },
    { NULL, NULL },
};

// ex.asset_db
static const luaL_Reg __meta_funcs[] = {
    { "load", __asset_db_load },
    { NULL, NULL },
};

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int luaopen_asset_db ( lua_State *_l ) {

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
    // tp = {}
    // setmetatable( tp, { __type_meta_funcs } )
    ex_lua_global_module ( _l, "ex" );
    lua_pushstring(_l,"asset_db");
        lua_newtable(_l);
        luaL_register(_l, NULL, __meta_funcs);
        lua_newtable(_l);
        luaL_register(_l, NULL, __type_meta_funcs);
        lua_setmetatable(_l,-2);
    lua_rawset(_l,-3); // rawset( ex, "asset_db", tp )
    lua_pop(_l, 1); // pops ex
    
    //
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void luaclose_asset_db () {
    // deinit the hashtable
    ex_hashmap_deinit ( &__key_to_type_meta_getset );
}

