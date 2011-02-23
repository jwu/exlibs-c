// ======================================================================================
// File         : wrap_object.c
// Author       : Wu Jie 
// Last Change  : 02/17/2011 | 20:47:12 PM | Thursday,February
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "../../engine/object.h"

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
static const char *__typename = "ex.object";

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

ref_proxy_t *ex_lua_checkobject ( lua_State *_l, int _idx ) {
    ref_proxy_t *u;

    if ( lua_isuserdata(_l, _idx) == 0 )
        return NULL;
    u = (ref_proxy_t *)lua_touserdata(_l,_idx);
    if ( ex_rtti_isa( ex_rtti_get(u->typeid), EX_RTTI(ex_object_t) ) )
        return NULL;

    return u;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static ref_proxy_t *__pushobject ( lua_State *_l, int _meta_idx, bool _readonly ) {
    ref_proxy_t *u;

    u = (ref_proxy_t *)lua_newuserdata(_l, sizeof(ref_proxy_t));
    u->readonly = _readonly;
    u->typeid = EX_TYPEID(ex_object_t);
    u->ref = NULL;
    lua_pushvalue(_l,_meta_idx);
    lua_setmetatable(_l,-2);

    return u;
}

ref_proxy_t *ex_lua_pushobject ( lua_State *_l, bool _readonly ) {
    ref_proxy_t *u;

    luaL_newmetatable( _l, __typename ); // NOTE: this find a table in LUA_REGISTRYINDEX
    u = __pushobject ( _l, lua_gettop(_l), _readonly );
    lua_remove(_l,-2);

    return u;
}

///////////////////////////////////////////////////////////////////////////////
// type meta method
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __object_new ( lua_State *_l ) {
    ref_proxy_t *u;
    
    u = __pushobject(_l,1,false);
    u->ref = ex_create_object( EX_RTTI(ex_object_t), ex_generate_uid() );
    ex_incref(u->ref);

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __object_new_for_child ( lua_State *_l ) {
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
    
    u = __pushobject(_l,lua_gettop(_l),false);
    u->ref = ex_create_object( EX_RTTI(ex_object_t), ex_generate_uid() );
    ex_incref(u->ref);

    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// meta getset
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __object_get_uid ( lua_State *_l ) {
    ex_object_t *self;
    ref_proxy_t *u;
    
    u = ex_lua_checkobject(_l,1);
    if ( u == NULL ) {
        ex_error("invalid parameter type, expected %s", __typename);
        lua_pushnil(_l);
        return 1;
    }
    self = EX_REF_CAST(ex_object_t,u->ref);
    if ( self == NULL ) {
        ex_error("error: null reference!");
        lua_pushnil(_l);
        return 1;
    }
    // CHECK: i don't know could this be done in 32-bit system (such as win32.) { 
    lua_pushinteger( _l, self->uid ); 
    // } CHECK end 
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __object_get_name ( lua_State *_l ) {
    ex_object_t *self;
    ref_proxy_t *u;
    
    u = ex_lua_checkobject(_l,1);
    if ( u == NULL ) {
        ex_error("invalid parameter type, expected %s", __typename);
        lua_pushnil(_l);
        return 1;
    }
    self = EX_REF_CAST(ex_object_t,u->ref);
    if ( self == NULL ) {
        ex_error("error: null reference!");
        lua_pushnil(_l);
        return 1;
    }
    lua_pushstring( _l, ex_strid_to_cstr(self->name) ); 
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __object_set_name ( lua_State *_l ) {
    ex_object_t *self;
    ref_proxy_t *u;
    const char *in_name;

    in_name = luaL_checkstring(_l,3);
    u = ex_lua_checkobject(_l,1);
    if ( u == NULL ) {
        ex_error("invalid parameter type, expected %s", __typename);
        return 0;
    }
    self = EX_REF_CAST(ex_object_t,u->ref);
    if ( self == NULL ) {
        ex_error("error: null reference!");
        return 0;
    }
    self->name = ex_strid(in_name);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// meta method
///////////////////////////////////////////////////////////////////////////////

static int __object_destroy ( lua_State *_l ) {
    ref_proxy_t *u;

    u = ex_lua_checkobject(_l,1);
    if ( u == NULL ) {
        ex_error("invalid parameter type, expected %s", __typename);
        return 0;
    }

    ex_destroy_object(u->ref);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int luaopen_object ( lua_State *_l ) {

    // ex.object.meta
    static const ex_getset_t __type_meta_getsets[] = {
        { NULL, NULL, NULL },
    };
    static const luaL_Reg __type_meta_funcs[] = {
        { "__newindex", __type_meta_newindex },
        { "__index", __type_meta_index },
        { "__call", __object_new },
        { NULL, NULL },
    };

    // ex.object
    static const ex_getset_t __meta_getsets[] = {
        { "uid", __object_get_uid, NULL },
        { "name", __object_get_name, __object_set_name },
        { NULL, NULL, NULL },
    };
    static const luaL_Reg __meta_funcs[] = {
        { "__gc", ex_lua_ref_gc },
        { "__newindex", __meta_newindex },
        { "__index", __meta_index },
        { "__tostring", ex_lua_ref_tostring },
        { "__eq", ex_lua_ref_eq },
        { "destroy", __object_destroy },
        { NULL, NULL },
    };
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
        ex_hashmap_insert( &__key_to_type_meta_getset, &keyid, getset, NULL );
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
    for ( getset = __meta_getsets; getset->key != NULL; ++getset ) {
        strid_t keyid = ex_strid(getset->key);
        ex_hashmap_insert( &__key_to_meta_getset, &keyid, getset, NULL );
    }

    // we create global ex table if it not exists.
    ex_lua_global_module ( _l, "ex" );
    ex_lua_register_class ( _l,
                            "object",
                            "NULL",
                            __typename,
                            __meta_funcs,
                            __type_meta_funcs,
                            __object_new_for_child );
    lua_pop(_l, 1); // pops ex
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void luaclose_object () {
    // deinit the hashtable
    ex_hashmap_deinit ( &__key_to_type_meta_getset );
    ex_hashmap_deinit ( &__key_to_meta_getset );
}
