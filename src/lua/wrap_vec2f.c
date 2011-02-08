// ======================================================================================
// File         : wrap_vec2f.c
// Author       : Wu Jie 
// Last Change  : 02/07/2011 | 22:42:37 PM | Monday,February
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
// defines
///////////////////////////////////////////////////////////////////////////////

vec2f_proxy_t *ex_lua_checkvec2f ( lua_State *_l, int _idx ) {
    vec2f_proxy_t *u;

    if ( lua_isuserdata(_l, _idx) == 0 )
        return NULL;
    u = (vec2f_proxy_t *)lua_touserdata(_l,_idx);
    if ( u->typeid != EX_TYPEID(vec2f) )
        return NULL;

    return u;
}

vec2f_proxy_t *ex_lua_pushvec2f ( lua_State *_l, bool _readonly ) {
    vec2f_proxy_t *u;

    u = (vec2f_proxy_t *)lua_newuserdata(_l, sizeof(vec2f_proxy_t));
    u->readonly = _readonly;
    u->typeid = EX_TYPEID(vec2f);
    luaL_newmetatable(_l,"vec2f_meta");
    lua_setmetatable(_l,-2);

    return u;
}

typedef int (*vec2f_pfn) ( lua_State *, ex_vec2f_t * );
typedef struct getset_t {
    const char *key;
    vec2f_pfn get;
    vec2f_pfn set;
} getset_t;

static ex_hashmap_t __key_to_getset;

///////////////////////////////////////////////////////////////////////////////
// meta functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_meta_set ( lua_State *_l ) {
    const char* key;
    strid_t key_id;
    vec2f_proxy_t *u;
    getset_t *getset;

    // get and check the type
    u = ex_lua_checkvec2f(_l,1);
    if ( u == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        return 0;
    }
    if ( u->readonly ) {
        ex_error("the value is readonly");
        return 0;
    }

    // get and check the key
    key = luaL_checkstring(_l, 2);
    key_id = ex_strid(key);
    getset = ex_hashmap_get( &__key_to_getset, &key_id, NULL );
    if ( getset == NULL ) {
        ex_error("key %s not found", key);
        return 0;
    }
    if ( getset->set == NULL ) {
        ex_error("the key %s is readonly", key);
        return 0;
    }

    // process
    return getset->set( _l, &u->val );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_meta_get ( lua_State *_l ) {
    const char* key;
    strid_t key_id;
    vec2f_proxy_t *u;
    getset_t *getset;

    u = ex_lua_checkvec2f(_l,1);
    if ( u == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }

    // get and check the key
    key = luaL_checkstring(_l, 2);
    key_id = ex_strid(key);
    getset = ex_hashmap_get( &__key_to_getset, &key_id, NULL );
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
    return getset->get( _l, &u->val );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_tostring ( lua_State *_l ) {
    ex_string_t *str = ex_string_alloc("",256);
    vec2f_proxy_t *u;

    u = ex_lua_checkvec2f(_l,1);
    if ( u == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }

    EX_RTTI(vec2f)->tostring(str, &u->val);
    lua_pushstring(_l, str->text);
    ex_string_free(str);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_add ( lua_State *_l ) {
    vec2f_proxy_t *u,*u1,*u2;

    u1 = ex_lua_checkvec2f(_l,1);
    if ( u1 == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }
    u2 = ex_lua_checkvec2f(_l,2);
    if ( u2 == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }

    // push
    u = ex_lua_pushvec2f(_l,false);
    ex_vec2f_add( &u->val, &u1->val, &u2->val );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_sub ( lua_State *_l ) {
    vec2f_proxy_t *u,*u1,*u2;

    u1 = ex_lua_checkvec2f(_l,1);
    if ( u1 == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }
    u2 = ex_lua_checkvec2f(_l,2);
    if ( u2 == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }

    // push
    u = ex_lua_pushvec2f(_l,false);
    ex_vec2f_sub( &u->val, &u1->val, &u2->val );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_new ( lua_State *_l ) {
    vec2f_proxy_t *u;
    
    u = ex_lua_pushvec2f(_l,false);
    ex_vec2f_set( &u->val, lua_tonumber(_l,1), lua_tonumber(_l,2) );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static const luaL_Reg __vec2f_meta[] = {
    { "__newindex", __vec2f_meta_set },
    { "__index", __vec2f_meta_get },
    { "__tostring", __vec2f_tostring },
    { "__add", __vec2f_add },
    { "__sub", __vec2f_sub },
    { "new", __vec2f_new },
    { 0, 0 }
};

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_type ( lua_State *_l, ex_vec2f_t *_v ) {
    lua_pushstring(_l, "vec2f");
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_x ( lua_State *_l, ex_vec2f_t *_v ) {
    lua_pushnumber(_l, _v->x);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_set_x ( lua_State *_l, ex_vec2f_t *_v ) {
    _v->x = luaL_checknumber(_l, 3);
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_y ( lua_State *_l, ex_vec2f_t *_v ) {
    lua_pushnumber(_l, _v->y);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_set_y ( lua_State *_l, ex_vec2f_t *_v ) {
    _v->y = luaL_checknumber(_l, 3);
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static getset_t __getset_list[] = {
    { "type", __vec2f_get_type, NULL },
    { "x", __vec2f_get_x, __vec2f_set_x },
    { "y", __vec2f_get_y, __vec2f_set_y },
    { NULL, NULL, NULL },
};

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

//
int luaopen_vec2f ( lua_State *_l ) {
    getset_t *getset;
    vec2f_proxy_t *u;

    // init the hashtable
    ex_hashmap_init ( &__key_to_getset,
                      EX_STRID_NULL, sizeof(strid_t),
                      EX_STRID_NULL, sizeof(getset_t),
                      32,
                      ex_hashkey_strid, ex_keycmp_strid,
                      __ex_hashmap_alloc_nomng,
                      __ex_hashmap_realloc_nomng,
                      __ex_hashmap_dealloc_nomng
                    );
    for ( getset = __getset_list; getset->key != NULL; ++getset ) {
        strid_t keyid = ex_strid(getset->key);
        ex_hashmap_insert( &__key_to_getset, &keyid, getset, NULL );
    }

    // we create global ex table if it not exists.
    ex_lua_global_module ( _l, "ex" ); // [-0,+1,-]

    // register vec2f_meta
    luaL_newmetatable(_l, "vec2f_meta"); // [-0,+1,m]
    luaL_register(_l, NULL, __vec2f_meta); // [-1,+1,m]

    // register vec2f
    lua_newtable(_l); // [-0,+1,m]
    lua_setmetatable(_l,-2); // [-1,+0,-] setmetatable( ex.vec2f, vec2f_meta )

    // FIXME: readonly not working { 
    // ex.vec2f.zero = ex_vec2f_zero
    u = ex_lua_pushvec2f(_l,true); u->val = ex_vec2f_zero; lua_setfield(_l,-2,"zero");

    // ex.vec2f.one = ex_vec2f_one
    u = ex_lua_pushvec2f(_l,true); u->val = ex_vec2f_one; lua_setfield(_l,-2,"one");
    // } FIXME end 

    //
    lua_setfield(_l,-2,"vec2f"); // [-1,+0,e] ex.vec2f = table
    lua_pop(_l, 1); // [-1,+0,-] pops ex.

    return 0;
}

//
void luaclose_vec2f () {
    // deinit the hashtable
    ex_hashmap_deinit ( &__key_to_getset );
}
