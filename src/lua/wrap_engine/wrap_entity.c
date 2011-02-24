// ======================================================================================
// File         : wrap_entity.c
// Author       : Wu Jie 
// Last Change  : 02/25/2011 | 01:43:15 AM | Friday,February
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "../../engine/world.h"
#include "../../engine/entity.h"

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
static const char *__typename = "ex.entity";

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __type_meta_newindex ( lua_State *_l ) {
    return ex_lua_userdata_newindex( _l, &__key_to_type_meta_getset );
}
static int __type_meta_index ( lua_State *_l ) {
    return ex_lua_userdata_index( _l, &__key_to_type_meta_getset );
}
static int __meta_newindex ( lua_State *_l ) {
    return ex_lua_userdata_newindex( _l, &__key_to_meta_getset );
}
static int __meta_index ( lua_State *_l ) {
    return ex_lua_userdata_index( _l, &__key_to_meta_getset );
}
static int __child_meta_newindex ( lua_State *_l ) {
    return ex_lua_userdata_newindex_for_child( _l, &__key_to_meta_getset );
}
static int __child_meta_index ( lua_State *_l ) {
    return ex_lua_userdata_index_for_child( _l, &__key_to_meta_getset );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ref_proxy_t *ex_lua_pushentity ( lua_State *_l, bool _readonly ) {
    ref_proxy_t *u;

    luaL_newmetatable( _l, __typename ); // NOTE: this find a table in LUA_REGISTRYINDEX
    u = ex_lua_pushref ( _l, lua_gettop(_l), _readonly );
    lua_remove(_l,-2);

    return u;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_lua_isentity ( lua_State *_l, int _idx ) {
    if ( ex_lua_isref(_l,_idx) ) {
        ex_ref_t *r = ex_lua_toref(_l,_idx);
        if ( ex_isa( ex_entity_t, r->ptr ) )
            return true;
    }
    return false;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_lua_toentity ( lua_State *_l, int _idx ) {
    return ex_lua_toref(_l,_idx);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_lua_checkentity ( lua_State *_l, int _idx ) {
    if ( ex_lua_isentity(_l,_idx) == false ) {
        luaL_error( _l, "invalid parameter type, expected %s", __typename );
    }
    return ex_lua_toentity(_l,_idx);
}

///////////////////////////////////////////////////////////////////////////////
// type meta getset
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// type meta method
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __entity_new ( lua_State *_l ) {
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

static int __entity_new_for_child ( lua_State *_l ) {
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

///////////////////////////////////////////////////////////////////////////////
// meta method
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ex.entity.meta
static const ex_getset_t __type_meta_getsets[] = {
    { NULL, NULL, NULL },
};
static const luaL_Reg __type_meta_funcs[] = {
    { "__newindex", __type_meta_newindex },
    { "__index", __type_meta_index },
    { "__call", __entity_new },
    { NULL, NULL },
};

// ex.entity
static const ex_getset_t __meta_getsets[] = {
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
// ------------------------------------------------------------------ 

int luaopen_entity ( lua_State *_l ) {

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
                            "entity",
                            "ex.object",
                            __typename,
                            __meta_funcs,
                            __type_meta_funcs,
                            __entity_new_for_child );
    lua_pop(_l, 1); // pops ex
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void luaclose_entity () {
    // deinit the hashtable
    ex_hashmap_deinit ( &__key_to_type_meta_getset );
    ex_hashmap_deinit ( &__key_to_meta_getset );
}

