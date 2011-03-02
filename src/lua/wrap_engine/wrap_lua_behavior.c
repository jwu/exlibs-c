// ======================================================================================
// File         : wrap_lua_behavior.c
// Author       : Wu Jie 
// Last Change  : 03/01/2011 | 15:33:17 PM | Tuesday,March
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "../../engine/entity.h"
#include "../../engine/component/lua_behavior.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

EX_DEF_LUA_BUILTIN_REF( ex_lua_behavior_t, lua_behavior, "ex.lua_behavior" )

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ref_proxy_t *ex_lua_push_generic_component ( lua_State *_l, const char *_lua_typename ) {
    ref_proxy_t *u;
    bool is_builtin = true;

    // DEBUG { 
    // ex_log( "get typename %s", _lua_typename );
    // luaL_newmetatable( _l, _lua_typename );
    // ex_lua_global_module ( _l, "ex" );
    // lua_getfield(_l, -1, "debug");
    // lua_getfield(_l, -1, "dump");
    // lua_pushvalue(_l,-4);
    // lua_call( _l, 1, 0 );
    // } DEBUG end 
    luaL_newmetatable( _l, _lua_typename );

    //
    lua_pushstring(_l,"__isbuiltin");
    lua_rawget ( _l, -2 );
    is_builtin = lua_toboolean(_l,-1);
    lua_pop(_l,1);

    //
    if ( !is_builtin ) {
        lua_newtable(_l);

        lua_pushboolean(_l,true);
        lua_setfield(_l,-2,"__isinstance");
        lua_pushcfunction(_l,__child_meta_index);
        lua_setfield(_l,-2,"__index");
        lua_pushcfunction(_l,__child_meta_newindex);
        lua_setfield(_l,-2,"__newindex");
        lua_pushcfunction(_l,ex_lua_ref_gc);
        lua_setfield(_l,-2,"__gc");
        lua_pushcfunction(_l,ex_lua_ref_eq);
        lua_setfield(_l,-2,"__eq");
        // TODO: should be lua class __tostring { 
        lua_pushcfunction(_l,ex_lua_ref_tostring);
        lua_setfield(_l,-2,"__tostring");
        lua_pushcfunction(_l,ex_lua_ref_concat);
        lua_setfield(_l,-2,"__concat");
        // } TODO end 

        lua_pushvalue(_l,-2); // push luaL_newmetatable(_l,_lua_typename);
        lua_setmetatable(_l,-2);
    }
    u = ex_lua_pushref ( _l, lua_gettop(_l) );

    return u;
}

///////////////////////////////////////////////////////////////////////////////
// type meta getset
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __lua_behavior_new ( lua_State *_l ) {
    luaL_error( _l, "can't construct empty lua behavior directly" );
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __lua_behavior_new_for_child ( lua_State *_l ) {
    ref_proxy_t *u;
    ex_ref_t *ent, *comp;
    const char *tp_name;
    int nargs = lua_gettop(_l);

    // TODO: new table or from argument { 
    lua_newtable(_l);
    // } TODO end 

    lua_pushboolean(_l,true);
    lua_setfield(_l,-2,"__isinstance");
    lua_pushcfunction(_l,__child_meta_index);
    lua_setfield(_l,-2,"__index");
    lua_pushcfunction(_l,__child_meta_newindex);
    lua_setfield(_l,-2,"__newindex");
    lua_pushcfunction(_l,ex_lua_ref_gc);
    lua_setfield(_l,-2,"__gc");
    lua_pushcfunction(_l,ex_lua_ref_eq);
    lua_setfield(_l,-2,"__eq");
    // TODO: should be lua class __tostring { 
    lua_pushcfunction(_l,ex_lua_ref_tostring);
    lua_setfield(_l,-2,"__tostring");
    lua_pushcfunction(_l,ex_lua_ref_concat);
    lua_setfield(_l,-2,"__concat");
    // } TODO end 

    lua_pushvalue(_l,1);
    lua_setmetatable(_l,-2);
    
    //
    u = ex_lua_pushref(_l,lua_gettop(_l));
    ent = ex_lua_checkentity(_l,2);

    lua_getfield(_l,1,"__typename");
    tp_name = luaL_checkstring(_l,-1);
    comp = ex_entity_add_comp( ent, ex_strid(tp_name) ); // NOTE: because it is derived class
    u->val = comp;
    ex_incref(u->val);

    if ( nargs > 1 ) {
        // TODO:
    }

    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// meta method
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ex.lua_behavior.meta
static const ex_getset_t __type_meta_getsets[] = {
    { NULL, NULL, NULL },
};
static const luaL_Reg __type_meta_funcs[] = {
    { "__newindex", __type_meta_newindex },
    { "__index", __type_meta_index },
    { "__call", __lua_behavior_new },
    { NULL, NULL },
};

// ex.lua_behavior
static const ex_getset_t __meta_getsets[] = {
    { NULL, NULL, NULL },
};
static const luaL_Reg __meta_funcs[] = {
    { "__gc", ex_lua_ref_gc },
    { "__newindex", __meta_newindex },
    { "__index", __meta_index },
    { "__tostring", ex_lua_ref_tostring },
    { "__concat", ex_lua_ref_concat },
    { "__eq", ex_lua_ref_eq },
    { NULL, NULL },
};

// ------------------------------------------------------------------ 
// Desc: 
extern const ex_getset_t *ex_object_meta_getsets;
extern const ex_getset_t *ex_component_meta_getsets;
extern const ex_getset_t *ex_behavior_meta_getsets;
// ------------------------------------------------------------------ 

int luaopen_lua_behavior ( lua_State *_l ) {

    const ex_getset_t *meta_getsets_including_parents[5];
    const ex_getset_t **getsets;
    const ex_getset_t *getset;

    // NOTE: since we have extern link pointers, we can't use static const define
    meta_getsets_including_parents[0] = ex_object_meta_getsets;
    meta_getsets_including_parents[1] = ex_component_meta_getsets;
    meta_getsets_including_parents[2] = ex_behavior_meta_getsets;
    meta_getsets_including_parents[3] = __meta_getsets;
    meta_getsets_including_parents[4] = NULL;

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

    // init the meta hashtable
    ex_hashmap_init ( &__key_to_meta_getset,
                      EX_STRID_NULL, sizeof(strid_t),
                      EX_STRID_NULL, sizeof(ex_getset_t),
                      32,
                      ex_hashkey_strid, ex_keycmp_strid,
                      __ex_hashmap_alloc,
                      __ex_hashmap_realloc,
                      __ex_hashmap_dealloc
                    );
    for ( getsets = meta_getsets_including_parents; *getsets != NULL; ++getsets ) {
        for ( getset = *getsets; getset->key != NULL; ++getset ) {
            strid_t keyid = ex_strid(getset->key);
            ex_hashmap_set( &__key_to_meta_getset, &keyid, getset );
        }
    }

    // we create global ex table if it not exists.
    ex_lua_global_module ( _l, "ex" );
    ex_lua_register_class ( _l,
                            "lua_behavior",
                            "ex.behavior",
                            __typename,
                            __meta_funcs,
                            __type_meta_funcs,
                            __lua_behavior_new_for_child );
    lua_pop(_l, 1); // pops ex
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void luaclose_lua_behavior () {
    // deinit the hashtable
    ex_hashmap_deinit ( &__key_to_type_meta_getset );
    ex_hashmap_deinit ( &__key_to_meta_getset );
}


