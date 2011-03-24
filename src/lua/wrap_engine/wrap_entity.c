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
    ex_ref_t *r;
    const char *name;
    int nargs = lua_gettop(_l);
    
    name = luaL_checkstring(_l,2);
    r = ex_world_create_entity( ex_current_world(), ex_strid(name) );
    ex_object_pushref(r);

    // add component if 3rd argument is a table
    if ( nargs > 2 ) {
        luaL_checktype(_l, 3, LUA_TTABLE);
        // TODO: get each element, create it as component
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
    ex_ref_t *entref, *compref;
    const char *comp_typename;
    ex_behavior_t *be;
    int nargs = lua_gettop(_l);
    
    entref = ex_lua_checkentity(_l,1);
    ex_lua_check_nullref(_l,entref);
    comp_typename = luaL_checkstring(_l,2);

    compref = ex_entity_add_comp( entref, ex_strid(comp_typename) );
    if ( compref == NULL ) {
        return luaL_error( _l, "can't add component by type %s", comp_typename );
    }

    //
    ex_object_pushref(compref);

    // if we have table parameters, setup them
    if ( nargs > 2 ) {
        if ( lua_istable(_l,3) == false ) {
            return luaL_error( _l, "the third argument must be table" );
        }
        lua_pushnil(_l); /* first key */
        while ( lua_next(_l,3) != 0 ) {
            /* uses 'key' (at index -2) and 'value' (at index -1) */
            lua_pushvalue(_l,-3); // push userdata
            lua_pushvalue(_l,-3); // push key
            lua_pushvalue(_l,-3); // push value
            lua_settable(_l,-3);
            lua_pop(_l,2); // pops value, pops userdata
        }
    }

    // awake behavior, and setup lua behavior
    be = EX_REF_AS(ex_behavior_t,compref);
    if ( be ) {
        // awake if the world is running
        ex_entity_t *ent = EX_REF_CAST(ex_entity_t,entref);
        if ( be->awake && 
             ( ex_world_is_running ( ent->world ) || ex_world_is_paused ( ent->world ) ) ) 
        {
            be->awake(compref);
        }
    }

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __entity_get_comp ( lua_State *_l ) {
    ex_ref_t *ent, *comp;
    const char *comp_typename;
    
    ent = ex_lua_checkentity(_l,1);
    ex_lua_check_nullref(_l,ent);
    comp_typename = luaL_checkstring(_l,2);

    comp = ex_entity_get_comp( ent, ex_strid(comp_typename) );
    // it is allowed to get nil component
    if ( comp == NULL ) {
        lua_pushnil(_l);
        return 1;
    }

    ex_object_pushref(comp);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __send_msg ( lua_State *_l, bool _need_receiver ) {
    ex_ref_t *entref;
    ex_entity_t *ent;
    const char *msg_name;
    int nargs = lua_gettop(_l);
    bool hasReceiver = false;
    
    entref = ex_lua_checkentity(_l,1);
    ex_lua_check_nullref(_l,entref);
    msg_name = luaL_checkstring(_l,2);
    ent = EX_REF_CAST(ex_entity_t,entref);

    ex_array_each ( ent->comps, ex_ref_t *, comp ) {
        ex_object_t *obj = EX_REF_CAST(ex_object_t,comp);
        int argidx = 3;
        int status;

        lua_rawgeti(_l, LUA_REGISTRYINDEX, obj->luaRefID);
        lua_getfield( _l, -1, msg_name );
        if ( lua_isnil(_l,-1) == 0 && lua_isfunction(_l,-1) ) {
            lua_pushvalue(_l,-2);
            while ( argidx <= nargs ) {
                lua_pushvalue(_l,argidx);
                ++argidx;
            }
            // NOTE: func args = nargs - 2 + 1
            status = lua_pcall( _l, nargs - 1, 0, 0 );
            if ( status )
                ex_lua_alert(_l);

            hasReceiver = true;
        }
    } ex_array_each_end

    //
    if ( _need_receiver && hasReceiver == false ) {
        luaL_error( _l, "none of the component have interface %s", msg_name );
    }

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __entity_send_msg ( lua_State *_l ) {
    return __send_msg( _l, true );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __entity_send_msg_noreply ( lua_State *_l ) {
    return __send_msg( _l, false );
}

///////////////////////////////////////////////////////////////////////////////
// meta getset
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __entity_get_trans2d ( lua_State *_l ) {
    ex_ref_t *r;

    r = ex_lua_checkentity(_l,1);
    ex_lua_check_nullref(_l,r);

    ex_object_pushref( EX_REF_CAST(ex_entity_t,r)->trans2d );
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ex.entity.meta
static const ex_getset_t __type_meta_getsets[] = {
    { "null", __entity_get_null, NULL },
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
    { "send_msg", __entity_send_msg },
    { "send_msg_noreply", __entity_send_msg_noreply },
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
                            NULL );
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

