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

EX_DEF_LUA_BUILTIN_REF( ex_world_t, world, "ex.world" )

///////////////////////////////////////////////////////////////////////////////
// type meta getset
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __world_get_current ( lua_State *_l ) {
    ex_ref_t *cur = ex_current_world();
    ex_object_pushref(cur);
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
    ex_ref_t *r;
    
    r = ex_create_object( EX_RTTI(ex_world_t), ex_generate_uid() );
    ex_object_pushref(r);
    EX_REF_CAST(ex_object_t,r)->init(r);

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
    
    r = ex_lua_checkworld(_l,1);
    ex_lua_check_nullref(_l,r);

    name = luaL_checkstring(_l,2);
    ent = ex_world_create_entity( r, ex_strid(name) );

    ex_object_pushref(ent);
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ex.world.meta
static const ex_getset_t __type_meta_getsets[] = {
    { "null", __world_get_null, NULL },
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
    { "__concat", ex_lua_ref_concat },
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
                            NULL );
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
