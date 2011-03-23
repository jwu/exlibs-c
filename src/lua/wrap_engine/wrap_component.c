// ======================================================================================
// File         : wrap_component.c
// Author       : Wu Jie 
// Last Change  : 02/25/2011 | 20:04:30 PM | Friday,February
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "../../engine/entity.h"
#include "../../engine/component/component.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

EX_DEF_LUA_BUILTIN_REF( ex_component_t, component, "ex.component" )

///////////////////////////////////////////////////////////////////////////////
// type meta getset
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// type meta method
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __component_new ( lua_State *_l ) {
    luaL_error( _l, "can't construct empty component directly" );
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// meta method
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __component_get_entity ( lua_State *_l ) {
    ex_ref_t *r;
    ex_component_t *self;

    r = ex_lua_checkcomponent(_l,1); 
    ex_lua_check_nullref(_l,r);
    self = EX_REF_CAST(ex_component_t,r);

    ex_object_pushref(self->entity);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __component_get_trans2d ( lua_State *_l ) {
    ex_ref_t *r;
    ex_component_t *self;

    r = ex_lua_checkcomponent(_l,1); 
    ex_lua_check_nullref(_l,r);
    self = EX_REF_CAST(ex_component_t,r);

    ex_object_pushref(EX_REF_CAST(ex_entity_t,self->entity)->trans2d);
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ex.component.meta
static const ex_getset_t __type_meta_getsets[] = {
    { "null", __component_get_null, NULL },
    { NULL, NULL, NULL },
};
static const luaL_Reg __type_meta_funcs[] = {
    { "__newindex", __type_meta_newindex },
    { "__index", __type_meta_index },
    { "__call", __component_new },
    { NULL, NULL },
};

// ex.component
static const ex_getset_t __meta_getsets[] = {
    { "entity", __component_get_entity, NULL },
    { "trans2d", __component_get_trans2d, NULL },
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
const ex_getset_t *ex_component_meta_getsets = __meta_getsets;

// ------------------------------------------------------------------ 
// Desc: 
extern const ex_getset_t *ex_object_meta_getsets;
// ------------------------------------------------------------------ 

int luaopen_component ( lua_State *_l ) {

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
                            "component",
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

void luaclose_component () {
    // deinit the hashtable
    ex_hashmap_deinit ( &__key_to_type_meta_getset );
    ex_hashmap_deinit ( &__key_to_meta_getset );
}

