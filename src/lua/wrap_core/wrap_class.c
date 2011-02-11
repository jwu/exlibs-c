// ======================================================================================
// File         : wrap_class.c
// Author       : Wu Jie 
// Last Change  : 02/10/2011 | 21:27:50 PM | Thursday,February
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

///////////////////////////////////////////////////////////////////////////////
// general defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static ex_hashmap_t __key_to_meta_getset;
static ex_hashmap_t __key_to_type_getset;

#if 0
// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

vec2f_proxy_t *ex_lua_checkvec2f ( lua_State *_l, int _idx ) {
    vec2f_proxy_t *u;

    if ( lua_isuserdata(_l, _idx) == 0 )
        return NULL;
    u = (vec2f_proxy_t *)lua_touserdata(_l,_idx);
    if ( u->typeid != EX_TYPEID(vec2f) )
        return NULL;

    return u;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

vec2f_proxy_t *ex_lua_pushvec2f ( lua_State *_l, bool _readonly ) {
    vec2f_proxy_t *u;

    u = (vec2f_proxy_t *)lua_newuserdata(_l, sizeof(vec2f_proxy_t));
    u->readonly = _readonly;
    u->typeid = EX_TYPEID(vec2f);
    luaL_newmetatable(_l,"vec2f_meta");
    lua_setmetatable(_l,-2);

    return u;
}
#endif

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __class_meta_set ( lua_State *_l ) {
    const char* key;
    strid_t key_id;
    ex_getset_t *getset;

    // first use rawget check if we have value in metatable.
    // NOTE: we not allow change the builtin values. so all of them are readonly.
    key = luaL_checkstring(_l, 2);
    lua_getmetatable( _l, 1 );
    lua_pushstring ( _l, key );
    lua_rawget ( _l, -2 );
    // if this is not nil
    if ( lua_isnil( _l, -1 ) == 0 ) {
        ex_error("the key %s is readonly", key);
        return 0;
    }

    // get and check the key
    key_id = ex_strid(key);
    getset = ex_hashmap_get( &__key_to_meta_getset, &key_id, NULL );
    if ( getset == NULL ) {
        ex_error("key %s not found", key);
        return 0;
    }
    if ( getset->set == NULL ) {
        ex_error("the key %s is readonly", key);
        return 0;
    }

    // process
    return getset->set(_l);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __class_meta_get ( lua_State *_l ) {
    const char* key;
    strid_t key_id;
    ex_getset_t *getset;

    // first use rawget check if we have value in metatable.
    key = luaL_checkstring(_l, 2);
    lua_getmetatable( _l, 1 );
    lua_pushstring ( _l, key );
    lua_rawget ( _l, -2 );
    if ( lua_isnil( _l, -1 ) == 0 ) // if this is not nil
        return 1;
    lua_pop(_l,2);

    // check the key
    key_id = ex_strid(key);
    getset = ex_hashmap_get( &__key_to_meta_getset, &key_id, NULL );
    if ( getset == NULL ) {
        ex_error("key %s not found", key);
        lua_pushnil(_l);
        return 1;
    }
    if ( getset->get == NULL ) {
        ex_error("the key %s is writeonly", key);
        lua_pushnil(_l);
        return 1;
    }

    // process
    return getset->get(_l);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __class_type_set ( lua_State *_l ) {
    const char* key;
    strid_t key_id;
    ex_getset_t *getset;

    // first use rawget check if we have value in metatable.
    // NOTE: we not allow change the builtin values. so all of them are readonly.
    key = luaL_checkstring(_l, 2);
    lua_getmetatable( _l, 1 );
    lua_pushstring ( _l, key );
    lua_rawget ( _l, -2 );
    // if this is not nil
    if ( lua_isnil( _l, -1 ) == 0 ) {
        ex_error("the key %s is readonly", key);
        return 0;
    }

    // get and check the key
    key_id = ex_strid(key);
    getset = ex_hashmap_get( &__key_to_type_getset, &key_id, NULL );
    if ( getset == NULL ) {
        ex_error("key %s not found", key);
        return 0;
    }
    if ( getset->set == NULL ) {
        ex_error("the key %s is readonly", key);
        return 0;
    }

    // process
    return getset->set(_l);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __class_type_get ( lua_State *_l ) {
    const char* key;
    strid_t key_id;
    ex_getset_t *getset;

    // first use rawget check if we have value in metatable.
    key = luaL_checkstring(_l, 2);
    lua_getmetatable( _l, 1 );
    lua_pushstring ( _l, key );
    lua_rawget ( _l, -2 );
    if ( lua_isnil( _l, -1 ) == 0 ) // if this is not nil
        return 1;
    lua_pop(_l,2);

    // check the key
    key_id = ex_strid(key);
    getset = ex_hashmap_get( &__key_to_type_getset, &key_id, NULL );
    if ( getset == NULL ) {
        ex_error("key %s not found", key);
        lua_pushnil(_l);
        return 1;
    }
    if ( getset->get == NULL ) {
        ex_error("the key %s is writeonly", key);
        lua_pushnil(_l);
        return 1;
    }

    // process
    return getset->get(_l);
}

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

//
int luaopen_class ( lua_State *_l ) {

    // meta
    static const luaL_Reg __meta_funcs[] = {
        { "__newindex", __class_meta_set },
        { "__index", __class_meta_get },
        // TODO { 
        // { "__tostring", __class_tostring },
        // { "__concat", __class_concat },
        // { "__len", __class_len },
        // } TODO end 
        { NULL, NULL },
    };
    static const ex_getset_t __meta_getsets[] = {
        // TODO { 
        // { "type", __class_get_type, NULL },
        // } TODO end 
        { NULL, NULL, NULL },
    };

    // type
    static const luaL_Reg __type_funcs[] = {
        { "__call", __class_new },
        { "__newindex", __class_type_set },
        { "__index", __class_type_get },
        // { "dot", __class_dot },
        { NULL, NULL },
    };
    static const ex_getset_t __type_getsets[] = {
        // { "new", __class_get_new, NULL }, // NOTE: we use __call instead
        // { "one", __class_get_one, NULL },
        { NULL, NULL, NULL },
    };
    const ex_getset_t *getset;

    // init the meta hashtable
    ex_hashmap_init ( &__key_to_meta_getset,
                      EX_STRID_NULL, sizeof(strid_t),
                      EX_STRID_NULL, sizeof(ex_getset_t),
                      32,
                      ex_hashkey_strid, ex_keycmp_strid,
                      __ex_hashmap_alloc_nomng,
                      __ex_hashmap_realloc_nomng,
                      __ex_hashmap_dealloc_nomng
                    );
    for ( getset = __meta_getsets; getset->key != NULL; ++getset ) {
        strid_t keyid = ex_strid(getset->key);
        ex_hashmap_insert( &__key_to_meta_getset, &keyid, getset, NULL );
    }

    // init the type hashtable
    ex_hashmap_init ( &__key_to_type_getset,
                      EX_STRID_NULL, sizeof(strid_t),
                      EX_STRID_NULL, sizeof(ex_getset_t),
                      32,
                      ex_hashkey_strid, ex_keycmp_strid,
                      __ex_hashmap_alloc_nomng,
                      __ex_hashmap_realloc_nomng,
                      __ex_hashmap_dealloc_nomng
                    );
    for ( getset = __type_getsets; getset->key != NULL; ++getset ) {
        strid_t keyid = ex_strid(getset->key);
        ex_hashmap_insert( &__key_to_type_getset, &keyid, getset, NULL );
    }

    // we create global ex table if it not exists.
    ex_lua_global_module ( _l, "ex" ); // [-0,+1,-]

    // register ex.class.meta
    luaL_newmetatable(_l, "ex.class.meta"); // [-0,+1,m]
    luaL_register(_l, NULL, __meta_funcs); // [-1,+1,m]
    lua_pop(_l, 1); // [-1,+0,-] pops ex.class.meta

    // register class
    lua_newtable(_l); // [-0,+1,m]
    luaL_newmetatable(_l, "ex.class"); // [-0,+1,m]
    luaL_register(_l, NULL, __type_funcs); // [-1,+1,m]
    lua_setmetatable(_l,-2); // [-1,+0,-] setmetatable( new_table, ex.class )
    lua_setfield(_l,-2,"class"); // [-1,+0,e] ex[class] = new_table

    lua_pop(_l, 1); // [-1,+0,-] pops ex

    return 0;
}

//
void luaclose_class () {
    // deinit the hashtable
    ex_hashmap_deinit ( &__key_to_meta_getset );
    ex_hashmap_deinit ( &__key_to_type_getset );
}
