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
#include "../../engine/component/lua_behavior.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

EX_DEF_LUA_BUILTIN_REF( ex_entity_t, entity, "ex.entity" )

///////////////////////////////////////////////////////////////////////////////
// type meta getset
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// type meta method
///////////////////////////////////////////////////////////////////////////////

// TODO:
// ent = ex.entity( "foobar", {
//     ex.trans2d = {
//          ...
//     },
//     ex.rigidbody = {
//          ...
//     },
// })

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __entity_new ( lua_State *_l ) {
    ref_proxy_t *u;
    const char *name;
    int nargs = lua_gettop(_l);
    
    u = ex_lua_pushref(_l,1);
    name = luaL_checkstring(_l,2);
    u->val = ex_world_create_entity( ex_current_world(), ex_strid(name) );
    ex_incref(u->val);

    // add component if 3rd argument is a table
    if ( nargs > 2 ) {
        luaL_checktype(_l, 3, LUA_TTABLE);
        // TODO: get each element, create it as component
    }

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __entity_new_for_child ( lua_State *_l ) {
    ref_proxy_t *u;
    const char *name;
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
    
    u = ex_lua_pushref(_l,lua_gettop(_l));
    name = luaL_checkstring(_l,2);
    u->val = ex_world_create_entity( ex_current_world(), ex_strid(name) );
    ex_incref(u->val);

    // add component if 3rd argument is a table
    if ( nargs > 2 ) {
        luaL_checktype(_l, 3, LUA_TTABLE);
        // TODO:
    }

    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// meta method
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __entity_add_comp ( lua_State *_l ) {
    ex_ref_t *ent, *comp;
    const char *comp_typename;
    ref_proxy_t *u;
    ex_lua_behavior_t *be;
    
    ent = ex_lua_checkentity(_l,1);
    ex_lua_check_nullref(_l,ent);
    comp_typename = luaL_checkstring(_l,2); // TODO: could be table with default member values :)

    comp = ex_entity_add_comp( ent, ex_strid(comp_typename) );
    if ( comp == NULL ) {
        return luaL_error( _l, "can't add component by type %s", comp_typename );
    }
    // HACK { 
    be = EX_REF_AS(ex_lua_behavior_t,comp);
    if ( be ) {
        be->l = _l; 
        be->lua_typename = comp_typename; 
    }
    // } HACK end 

    u = ex_lua_push_generic_component ( _l, comp_typename );
    u->val = comp;
    ex_incref(u->val);

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __entity_get_comp ( lua_State *_l ) {
    ex_ref_t *ent, *comp;
    const char *comp_typename;
    ref_proxy_t *u;
    
    ent = ex_lua_checkentity(_l,1);
    ex_lua_check_nullref(_l,ent);
    comp_typename = luaL_checkstring(_l,2);

    comp = ex_entity_get_comp( ent, ex_strid(comp_typename) );
    // it is allowed to get nil component
    if ( comp == NULL ) {
        lua_pushnil(_l);
        return 1;
    }

    u = ex_lua_push_generic_component ( _l, comp_typename );
    u->val = comp;
    ex_incref(u->val);

    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// meta getset
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __entity_get_trans2d ( lua_State *_l ) {
    ex_ref_t *r;
    ref_proxy_t *u;

    r = ex_lua_checkentity(_l,1);
    ex_lua_check_nullref(_l,r);

    u = ex_lua_pushtrans2d(_l);
    u->val = EX_REF_CAST(ex_entity_t,r)->trans2d; 
    ex_incref(u->val);

    return 1;
}

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
    { "trans2d", __entity_get_trans2d, NULL },
    { NULL, NULL, NULL },
};
static const luaL_Reg __meta_funcs[] = {
    { "__gc", ex_lua_ref_gc },
    { "__newindex", __meta_newindex },
    { "__index", __meta_index },
    { "__tostring", ex_lua_ref_tostring },
    { "__concat", ex_lua_ref_concat },
    { "__eq", ex_lua_ref_eq },
    { "add_comp", __entity_add_comp },
    { "get_comp", __entity_get_comp },
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

