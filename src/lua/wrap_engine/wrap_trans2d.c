// ======================================================================================
// File         : wrap_trans2d.c
// Author       : Wu Jie 
// Last Change  : 02/26/2011 | 00:38:20 AM | Saturday,February
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "../../engine/entity.h"
#include "../../engine/component/trans2d.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

EX_DEF_LUA_BUILTIN_REF( ex_trans2d_t, trans2d, "ex.trans2d" )

///////////////////////////////////////////////////////////////////////////////
// type meta getset
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// type meta method
///////////////////////////////////////////////////////////////////////////////

// TODO: this kind of constructor looks cool!
// ent = ex.entity("entity foo 01")
// -- ent:add_comp("ex.trans2d")
// ex.trans2d( ent, {
//     pos = ex.vec2f(1.0,1.0),
//     scale = ex.vec2f(1.0,1.0),
// } )

#if 0
// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __trans2d_new ( lua_State *_l ) {
    ref_proxy_t *u;
    const char *name;
    
    u = ex_lua_pushref(_l,1,false);
    name = luaL_checkstring(_l,2);
    u->val = ex_world_create_entity( ex_current_world(), ex_strid(name)  );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __trans2d_new_for_child ( lua_State *_l ) {
    ref_proxy_t *u;
    const char *name;

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
    // } TODO end 

    lua_pushvalue(_l,1);
    lua_setmetatable(_l,-2);
    
    u = ex_lua_pushref(_l,lua_gettop(_l),false);
    name = luaL_checkstring(_l,2); // TODO: ???????
    u->val = ex_world_create_entity( ex_current_world(), ex_strid(name)  );

    return 1;
}
#endif

///////////////////////////////////////////////////////////////////////////////
// meta method
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 


///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ex.trans2d.meta
static const ex_getset_t __type_meta_getsets[] = {
    { NULL, NULL, NULL },
};
static const luaL_Reg __type_meta_funcs[] = {
    { "__newindex", __type_meta_newindex },
    { "__index", __type_meta_index },
    { NULL, NULL },
};

// ex.trans2d
static const ex_getset_t __meta_getsets[] = {
    // { "local_pos", __trans2d_get_local_pos, __trans2d_set_local_pos },
    // { "local_ang", __trans2d_get_local_ang, __trans2d_set_local_ang },
    // { "local_scale", __trans2d_get_local_scale, __trans2d_set_local_scale },
    // { "world_pos", __trans2d_get_world_pos, __trans2d_set_world_pos },
    // { "world_ang", __trans2d_get_world_ang, __trans2d_set_world_ang },
    // { "world_scale", __trans2d_get_world_scale, __trans2d_set_world_scale },
    { NULL, NULL, NULL },
};
static const luaL_Reg __meta_funcs[] = {
    { "__gc", ex_lua_ref_gc },
    { "__newindex", __meta_newindex },
    { "__index", __meta_index },
    { "__tostring", ex_lua_ref_tostring },
    { "__eq", ex_lua_ref_eq },
    { NULL, NULL },
};

// ------------------------------------------------------------------ 
// Desc: 
extern const ex_getset_t *ex_object_meta_getsets;
extern const ex_getset_t *ex_component_meta_getsets;
// ------------------------------------------------------------------ 

int luaopen_trans2d ( lua_State *_l ) {

    const ex_getset_t *meta_getsets_including_parents[3];
    const ex_getset_t **getsets;
    const ex_getset_t *getset;

    // NOTE: since we have extern link pointers, we can't use static const define
    meta_getsets_including_parents[0] = ex_object_meta_getsets;
    meta_getsets_including_parents[1] = ex_component_meta_getsets;
    meta_getsets_including_parents[2] = __meta_getsets;
    meta_getsets_including_parents[3] = NULL;

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
                            "trans2d",
                            "ex.component",
                            __typename,
                            __meta_funcs,
                            __type_meta_funcs,
                            NULL );
    lua_pop(_l, 1); // pops ex
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void luaclose_trans2d () {
    // deinit the hashtable
    ex_hashmap_deinit ( &__key_to_type_meta_getset );
    ex_hashmap_deinit ( &__key_to_meta_getset );
}


