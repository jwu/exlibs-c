// ======================================================================================
// File         : wrap_behavior.c
// Author       : Wu Jie 
// Last Change  : 03/01/2011 | 20:33:08 PM | Tuesday,March
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "../../engine/entity.h"
#include "../../engine/component/behavior.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

EX_DEF_LUA_BUILTIN_REF( ex_behavior_t, behavior, "ex.behavior" )

///////////////////////////////////////////////////////////////////////////////
// type meta getset
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// type meta method
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __behavior_new ( lua_State *_l ) {
    luaL_error( _l, "can't construct empty behavior directly" );
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// meta method
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __behavior_get_enabled ( lua_State *_l ) {
    ex_ref_t *r;
    ex_behavior_t *self;

    r = ex_lua_checkbehavior(_l,1); 
    ex_lua_check_nullref(_l,r);
    self = EX_REF_CAST(ex_behavior_t,r);

    lua_pushboolean(_l, self->enabled);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __behavior_set_enabled ( lua_State *_l ) {
    ex_ref_t *r;
    ex_behavior_t *self;

    r = ex_lua_checkbehavior(_l,1); 
    ex_lua_check_nullref(_l,r);
    self = EX_REF_CAST(ex_behavior_t,r);

    if ( lua_isboolean(_l,3) ) {
        self->enabled = lua_toboolean(_l, 3);
    }
    else {
        luaL_error( _l, "invalid parameter type, expected boolean" );
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ex.component.meta
static const ex_getset_t __type_meta_getsets[] = {
    { "null", __behavior_get_null, NULL },
    { "enabled", __behavior_get_enabled, __behavior_set_enabled },
    { NULL, NULL, NULL },
};
static const luaL_Reg __type_meta_funcs[] = {
    { "__newindex", __type_meta_newindex },
    { "__index", __type_meta_index },
    { "__call", __behavior_new },
    { NULL, NULL },
};

// ex.component
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
const ex_getset_t *ex_behavior_meta_getsets = __meta_getsets;

// ------------------------------------------------------------------ 
// Desc: 
extern const ex_getset_t *ex_object_meta_getsets;
extern const ex_getset_t *ex_component_meta_getsets;
// ------------------------------------------------------------------ 

int luaopen_behavior ( lua_State *_l ) {

    const ex_getset_t *meta_getsets_including_parents[4];
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
                            "behavior",
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

void luaclose_behavior () {
    // deinit the hashtable
    ex_hashmap_deinit ( &__key_to_type_meta_getset );
    ex_hashmap_deinit ( &__key_to_meta_getset );
}


