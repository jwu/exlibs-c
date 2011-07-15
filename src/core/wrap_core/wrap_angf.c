// ======================================================================================
// File         : wrap_angf.c
// Author       : Wu Jie 
// Last Change  : 02/28/2011 | 10:02:04 AM | Monday,February
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

EX_DEF_LUA_BUILTIN_TYPE(ex_angf_t,angf,"ex.angf")

///////////////////////////////////////////////////////////////////////////////
// type meta method
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __angf_new ( lua_State *_l ) {
    angf_proxy_t *u;
    float d;
    
    u = __pushangf(_l,1);
    d = (float)luaL_checknumber(_l,2);
    ex_angf_set_by_degrees ( &u->val, d );

    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// type meta getset
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __angf_get_zero ( lua_State *_l ) {
    angf_proxy_t *u = ex_lua_pushangf(_l); 
    u->val = ex_angf_zero;
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// meta method
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __angf_add ( lua_State *_l ) {
    angf_proxy_t *r;
    ex_angf_t *lhs,*rhs;

    lhs = ex_lua_checkangf(_l,1);
    rhs = ex_lua_checkangf(_l,2);

    // push
    r = ex_lua_pushangf(_l);
    ex_angf_add( &r->val, lhs, rhs );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __angf_sub ( lua_State *_l ) {
    angf_proxy_t *r;
    ex_angf_t *lhs,*rhs;

    lhs = ex_lua_checkangf(_l,1);
    rhs = ex_lua_checkangf(_l,2);

    // push
    r = ex_lua_pushangf(_l);
    ex_angf_sub( &r->val, lhs, rhs );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __angf_mul ( lua_State *_l ) {
    angf_proxy_t *r;
    ex_angf_t *lhs = NULL;
    ex_angf_t *rhs = NULL;

    //
    if ( ex_lua_isangf(_l,1) )
        lhs = ex_lua_toangf(_l,1);
    if ( ex_lua_isangf(_l,2) )
        rhs = ex_lua_toangf(_l,2);

    if ( lhs != NULL && rhs != NULL ) {
        r = ex_lua_pushangf(_l);
        ex_angf_mul( &r->val, lhs, rhs );
        return 1;
    }

    if ( lhs != NULL ) {
        float rhs = (float)luaL_checknumber(_l,2);
        r = ex_lua_pushangf(_l);
        ex_angf_mul_scalar( &r->val, lhs, rhs );
        return 1;
    }

    if ( rhs != NULL ) {
        float lhs = (float)luaL_checknumber(_l,1);
        r = ex_lua_pushangf(_l);
        ex_angf_mul_scalar( &r->val, rhs, lhs );
        return 1;
    }

    // push
    return luaL_error(_l,"invalid parameter type, lhs and rhs must be %s or number", __typename);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __angf_div ( lua_State *_l ) {
    angf_proxy_t *r;
    ex_angf_t *lhs = NULL;
    ex_angf_t *rhs = NULL;

    if ( ex_lua_isangf(_l,1) )
        lhs = ex_lua_toangf(_l,1);
    if ( ex_lua_isangf(_l,2) )
        rhs = ex_lua_toangf(_l,2);

    if ( lhs != NULL && rhs != NULL ) {
        r = ex_lua_pushangf(_l);
        ex_angf_div( &r->val, lhs, rhs );
        return 1;
    }

    if ( lhs != NULL ) {
        float rhs = (float)luaL_checknumber(_l,2);
        r = ex_lua_pushangf(_l);
        ex_angf_div_scalar( &r->val, lhs, rhs );
        return 1;
    }

    if ( rhs != NULL ) {
        float lhs = (float)luaL_checknumber(_l,1);
        r = ex_lua_pushangf(_l);
        ex_scalar_div_angf( &r->val, lhs, rhs );
        return 1;
    }

    // push
    return luaL_error(_l,"invalid parameter type, lhs and rhs must be %s or number", __typename);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __angf_unm ( lua_State *_l ) {
    angf_proxy_t *r;
    ex_angf_t *lhs;

    lhs = ex_lua_checkangf(_l,1);

    // push
    r = ex_lua_pushangf(_l);
    ex_angf_get_neg( &r->val, lhs );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __angf_len ( lua_State *_l ) {
    lua_pushinteger(_l,1);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __angf_eq ( lua_State *_l ) {
    ex_angf_t *lhs, *rhs;

    lhs = ex_lua_checkangf(_l,1);
    rhs = ex_lua_checkangf(_l,2);

    // push
    lua_pushboolean(_l, ex_angf_is_equal( lhs, rhs ) );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __angf_copy ( lua_State *_l ) {
    angf_proxy_t *u;
    ex_angf_t *a;

    a = ex_lua_checkangf(_l,1);

    // push
    u = ex_lua_pushangf(_l);
    u->val = *a;
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// meta getset
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __angf_get_rad ( lua_State *_l ) {
    // check the value
    ex_angf_t *a = ex_lua_checkangf(_l,1);
    lua_pushnumber(_l, a->rad);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __angf_set_rad ( lua_State *_l ) {
    // get and check the type
    ex_angf_t *a;
    
    a = ex_lua_checkangf(_l,1);
    ex_angf_set_by_radians ( a, (float)luaL_checknumber(_l, 3) );
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __angf_get_deg180 ( lua_State *_l ) {
    // check the value
    ex_angf_t *a = ex_lua_checkangf(_l,1);
    lua_pushnumber( _l, ex_angf_to_degrees_180(a) );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __angf_get_deg360 ( lua_State *_l ) {
    // check the value
    ex_angf_t *a = ex_lua_checkangf(_l,1);
    lua_pushnumber( _l, ex_angf_to_degrees_360(a) );
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ex.angf.meta
static const ex_getset_t __type_meta_getsets[] = {
    { "zero", __angf_get_zero, NULL },
    { NULL, NULL, NULL },
};
static const luaL_Reg __type_meta_funcs[] = {
    { "__newindex", __type_meta_newindex },
    { "__index", __type_meta_index },
    { "__call", __angf_new },
    { NULL, NULL },
};

// ex.angf
static const ex_getset_t __meta_getsets[] = {
    { "radians", __angf_get_rad, __angf_set_rad },
    { "degrees", __angf_get_deg360, NULL },
    { "degrees_180", __angf_get_deg180, NULL },
    { NULL, NULL, NULL },
};
static const luaL_Reg __meta_funcs[] = {
    { "__newindex", __meta_newindex },
    { "__index", __meta_index },
    { "__tostring", __angf_tostring },
    { "__concat", __angf_concat },
    { "__add", __angf_add },
    { "__sub", __angf_sub },
    { "__mul", __angf_mul },
    { "__div", __angf_div },
    { "__unm", __angf_unm },
    { "__len", __angf_len },
    { "__eq", __angf_eq },
    { "copy", __angf_copy },
    { NULL, NULL },
};

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int luaopen_angf ( lua_State *_l ) {

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
    for ( getset = __meta_getsets; getset->key != NULL; ++getset ) {
        strid_t keyid = ex_strid(getset->key);
        ex_hashmap_set( &__key_to_meta_getset, &keyid, getset );
    }

    // we create global ex table if it not exists.
    ex_lua_global_module ( _l, "ex" );
    ex_lua_register_builtin ( _l, 
                              "angf",
                              __typename,
                              __meta_funcs, 
                              __type_meta_funcs );
    lua_pop(_l, 1); // pops ex
    
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void luaclose_angf () {
    // deinit the hashtable
    ex_hashmap_deinit ( &__key_to_type_meta_getset );
    ex_hashmap_deinit ( &__key_to_meta_getset );
}
