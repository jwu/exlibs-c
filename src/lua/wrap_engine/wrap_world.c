// ======================================================================================
// File         : wrap_world.c
// Author       : Wu Jie 
// Last Change  : 02/07/2011 | 22:24:43 PM | Monday,February
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "../../engine/world.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static ex_hashmap_t __key_to_type_meta_getset;
static ex_hashmap_t __key_to_meta_getset;
static const char *__typename = "ex.world";

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __type_meta_newindex ( lua_State *_l ) {
    return ex_lua_type_meta_newindex( _l, &__key_to_type_meta_getset );
}
static int __type_meta_index ( lua_State *_l ) {
    return ex_lua_type_meta_index( _l, &__key_to_type_meta_getset );
}
static int __meta_newindex ( lua_State *_l ) {
    return ex_lua_meta_newindex( _l, &__key_to_meta_getset );
}
static int __meta_index ( lua_State *_l ) {
    return ex_lua_meta_index( _l, &__key_to_meta_getset );
}
static int __child_meta_newindex ( lua_State *_l ) {
    return ex_lua_child_meta_newindex( _l, &__key_to_meta_getset );
}
static int __child_meta_index ( lua_State *_l ) {
    return ex_lua_child_meta_index( _l, &__key_to_meta_getset );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ref_proxy_t *ex_lua_pushworld ( lua_State *_l, bool _readonly ) {
    ref_proxy_t *u;

    luaL_newmetatable( _l, __typename ); // NOTE: this find a table in LUA_REGISTRYINDEX
    u = ex_lua_pushref ( _l, lua_gettop(_l), _readonly );
    lua_remove(_l,-2);

    return u;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_lua_isworld ( lua_State *_l, int _idx ) {
    if ( ex_lua_isref(_l,_idx) ) {
        ex_ref_t *r = ex_lua_toref(_l,_idx);
        if ( ex_isa( ex_world_t, r->ptr ) )
            return true;
    }
    return false;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_lua_toworld ( lua_State *_l, int _idx ) {
    return ex_lua_toref(_l,_idx);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_lua_checkworld ( lua_State *_l, int _idx ) {
    if ( ex_lua_isworld(_l,_idx) == false ) {
        luaL_error( _l, "invalid parameter type, expected %s", __typename );
    }
    return ex_lua_toworld(_l,_idx);
}

///////////////////////////////////////////////////////////////////////////////
// type meta getset
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __world_get_current ( lua_State *_l ) {
    ex_ref_t *cur = ex_current_world();
    ref_proxy_t *u = ex_lua_pushworld(_l,false);
    u->val = cur;
    ex_incref(u->val);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __world_set_current ( lua_State *_l ) {
    ex_ref_t *r = ex_lua_checkworld(_l,3);
    ex_set_current_world(r);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// type meta method
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __world_new ( lua_State *_l ) {
    ref_proxy_t *u;
    
    u = ex_lua_pushref(_l,1,false);
    u->val = ex_create_object( EX_RTTI(ex_world_t), ex_generate_uid() );
    ex_incref(u->val);
    EX_REF_CAST(ex_object_t,u->val)->init(u->val);

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __world_new_for_child ( lua_State *_l ) {
    ref_proxy_t *u;

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
    u->val = ex_create_object( EX_RTTI(ex_world_t), ex_generate_uid() );
    ex_incref(u->val);
    EX_REF_CAST(ex_object_t,u->val)->init(u->val);

    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// meta method
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __world_new_entity ( lua_State *_l ) {
    ex_ref_t *r, *ent; 
    const char *name;
    ref_proxy_t *u;
    
    r = ex_lua_checkworld(_l,1);
    ex_lua_check_nullref(_l,r);

    name = luaL_checkstring(_l,2);
    ent = ex_world_create_entity( r, ex_strid(name) );

    u = ex_lua_pushentity(_l,false);
    u->val = ent; 

    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ex.world.meta
static const ex_getset_t __type_meta_getsets[] = {
    { "current", __world_get_current, __world_set_current },
    { NULL, NULL, NULL },
};
static const luaL_Reg __type_meta_funcs[] = {
    { "__newindex", __type_meta_newindex },
    { "__index", __type_meta_index },
    { "__call", __world_new },
    { NULL, NULL },
};

// ex.world
static const ex_getset_t __meta_getsets[] = {
    // TODO: { "main_camera", __world_get_maincamera, __world_set_maincamera },
    { NULL, NULL, NULL },
};
static const luaL_Reg __meta_funcs[] = {
    { "__gc", ex_lua_ref_gc },
    { "__newindex", __meta_newindex },
    { "__index", __meta_index },
    { "__tostring", ex_lua_ref_tostring },
    { "__eq", ex_lua_ref_eq },
    { "new_entity", __world_new_entity },
    { NULL, NULL },
};

// ------------------------------------------------------------------ 
// Desc: 
extern const ex_getset_t *ex_object_meta_getsets;
// ------------------------------------------------------------------ 

int luaopen_world ( lua_State *_l ) {

    const ex_getset_t *meta_getsets_including_parents[3];
    const ex_getset_t **getsets;
    const ex_getset_t *getset;

    // NOTE: since we have extern link pointers, we can't use static const define
    meta_getsets_including_parents[0] = ex_object_meta_getsets;
    meta_getsets_including_parents[1] = __meta_getsets;
    meta_getsets_including_parents[2] = NULL;

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
                            "world",
                            "ex.object",
                            __typename,
                            __meta_funcs,
                            __type_meta_funcs,
                            __world_new_for_child );
    lua_pop(_l, 1); // pops ex
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void luaclose_world () {
    // deinit the hashtable
    ex_hashmap_deinit ( &__key_to_type_meta_getset );
    ex_hashmap_deinit ( &__key_to_meta_getset );
}
