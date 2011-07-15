// ======================================================================================
// File         : wrap_color3f.c
// Author       : Wu Jie 
// Last Change  : 05/03/2011 | 16:20:33 PM | Tuesday,May
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

EX_DEF_LUA_BUILTIN_TYPE(ex_color3f_t,color3f,"ex.color3f")

///////////////////////////////////////////////////////////////////////////////
// type meta method
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_new ( lua_State *_l ) {
    color3f_proxy_t *u;
    
    u = __pushcolor3f(_l,1);
    ex_color3f_set( &u->val, lua_tonumber(_l,2), lua_tonumber(_l,3), lua_tonumber(_l,4) );

    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// type meta getset
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_get_red ( lua_State *_l ) {
    color3f_proxy_t *u = ex_lua_pushcolor3f(_l); 
    u->val = ex_color3f_red;
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_get_green ( lua_State *_l ) {
    color3f_proxy_t *u = ex_lua_pushcolor3f(_l); 
    u->val = ex_color3f_green;
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_get_blue ( lua_State *_l ) {
    color3f_proxy_t *u = ex_lua_pushcolor3f(_l); 
    u->val = ex_color3f_blue;
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_get_black ( lua_State *_l ) {
    color3f_proxy_t *u = ex_lua_pushcolor3f(_l); 
    u->val = ex_color3f_black;
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_get_white ( lua_State *_l ) {
    color3f_proxy_t *u = ex_lua_pushcolor3f(_l); 
    u->val = ex_color3f_white;
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_get_yellow ( lua_State *_l ) {
    color3f_proxy_t *u = ex_lua_pushcolor3f(_l); 
    u->val = ex_color3f_yellow;
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_get_cyan ( lua_State *_l ) {
    color3f_proxy_t *u = ex_lua_pushcolor3f(_l); 
    u->val = ex_color3f_cyan;
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_get_magenta ( lua_State *_l ) {
    color3f_proxy_t *u = ex_lua_pushcolor3f(_l); 
    u->val = ex_color3f_magenta;
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_get_gray ( lua_State *_l ) {
    color3f_proxy_t *u = ex_lua_pushcolor3f(_l); 
    u->val = ex_color3f_gray;
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// meta method
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_len ( lua_State *_l ) {
    lua_pushinteger(_l,3);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_eq ( lua_State *_l ) {
    ex_color3f_t *lhs, *rhs;

    lhs = ex_lua_checkcolor3f(_l,1);
    rhs = ex_lua_checkcolor3f(_l,2);

    // push
    lua_pushboolean(_l, ex_color3f_is_equal( lhs, rhs ) );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// TODO: can we use array ??? same as other op
// ------------------------------------------------------------------ 

static int __color3f_add ( lua_State *_l ) {
    color3f_proxy_t *r;
    ex_color3f_t *lhs,*rhs;

    lhs = ex_lua_checkcolor3f(_l,1);
    rhs = ex_lua_checkcolor3f(_l,2);

    // push
    r = ex_lua_pushcolor3f(_l);
    ex_color3f_add( &r->val, lhs, rhs );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_sub ( lua_State *_l ) {
    color3f_proxy_t *r;
    ex_color3f_t *lhs,*rhs;

    lhs = ex_lua_checkcolor3f(_l,1);
    rhs = ex_lua_checkcolor3f(_l,2);

    // push
    r = ex_lua_pushcolor3f(_l);
    ex_color3f_sub( &r->val, lhs, rhs );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_mul ( lua_State *_l ) {
    color3f_proxy_t *r;
    ex_color3f_t *lhs = NULL;
    ex_color3f_t *rhs = NULL;

    //
    if ( ex_lua_iscolor3f(_l,1) )
        lhs = ex_lua_tocolor3f(_l,1);
    if ( ex_lua_iscolor3f(_l,2) )
        rhs = ex_lua_tocolor3f(_l,2);

    if ( lhs != NULL && rhs != NULL ) {
        r = ex_lua_pushcolor3f(_l);
        ex_color3f_mul( &r->val, lhs, rhs );
        return 1;
    }

    if ( lhs != NULL ) {
        float rhs = (float)luaL_checknumber(_l,2);
        r = ex_lua_pushcolor3f(_l);
        ex_color3f_mul_scalar( &r->val, lhs, rhs );
        return 1;
    }

    if ( rhs != NULL ) {
        float lhs = (float)luaL_checknumber(_l,1);
        r = ex_lua_pushcolor3f(_l);
        ex_color3f_mul_scalar( &r->val, rhs, lhs );
        return 1;
    }

    // push
    return luaL_error(_l,"invalid parameter type, lhs and rhs must be %s or number", __typename);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_div ( lua_State *_l ) {
    color3f_proxy_t *r;
    ex_color3f_t *lhs = NULL;
    ex_color3f_t *rhs = NULL;

    if ( ex_lua_iscolor3f(_l,1) )
        lhs = ex_lua_tocolor3f(_l,1);
    if ( ex_lua_iscolor3f(_l,2) )
        rhs = ex_lua_tocolor3f(_l,2);

    if ( lhs != NULL && rhs != NULL ) {
        r = ex_lua_pushcolor3f(_l);
        ex_color3f_div( &r->val, lhs, rhs );
        return 1;
    }

    if ( lhs != NULL ) {
        float rhs = (float)luaL_checknumber(_l,2);
        r = ex_lua_pushcolor3f(_l);
        ex_color3f_div_scalar( &r->val, lhs, rhs );
        return 1;
    }

    // DISABLE { 
    // if ( rhs != NULL ) {
    //     float lhs = (float)luaL_checknumber(_l,1);
    //     r = ex_lua_pushcolor3f(_l);
    //     ex_scalar_div_color3f( &r->val, lhs, rhs );
    //     return 1;
    // }
    // } DISABLE end 

    // push
    return luaL_error(_l,"invalid parameter type, lhs and rhs must be %s or number", __typename);
}

// DISABLE { 
// // ------------------------------------------------------------------ 
// // Desc: 
// // ------------------------------------------------------------------ 

// static int __color3f_unm ( lua_State *_l ) {
//     color3f_proxy_t *r;
//     ex_color3f_t *lhs;

//     lhs = ex_lua_checkcolor3f(_l,1);

//     // push
//     r = ex_lua_pushcolor3f(_l);
//     ex_color3f_get_neg( lhs, &r->val );

//     return 1;
// }
// } DISABLE end 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_copy ( lua_State *_l ) {
    color3f_proxy_t *u;
    ex_color3f_t *v;

    v = ex_lua_checkcolor3f(_l,1);

    // push
    u = ex_lua_pushcolor3f(_l);
    ex_color3f_set( &u->val, v->r, v->g, v->b );
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// meta getset
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_get_r ( lua_State *_l ) {
    // check the value
    ex_color3f_t *v = ex_lua_checkcolor3f(_l,1);
    lua_pushnumber(_l, v->r);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_set_r ( lua_State *_l ) {
    // get and check the type
    ex_color3f_t *v;
    
    v = ex_lua_checkcolor3f(_l,1);
    v->r = luaL_checknumber(_l, 3);
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_get_g ( lua_State *_l ) {
    // check the value
    ex_color3f_t *v = ex_lua_checkcolor3f(_l,1);
    lua_pushnumber(_l, v->g);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_set_g ( lua_State *_l ) {
    // get and check the type
    ex_color3f_t *v;
    
    v = ex_lua_checkcolor3f(_l,1);
    v->g = luaL_checknumber(_l, 3);
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_get_b ( lua_State *_l ) {
    // check the value
    ex_color3f_t *v = ex_lua_checkcolor3f(_l,1);
    lua_pushnumber(_l, v->b);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __color3f_set_b ( lua_State *_l ) {
    // get and check the type
    ex_color3f_t *v;
    
    v = ex_lua_checkcolor3f(_l,1);
    v->b = luaL_checknumber(_l, 3);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ex.color3f.meta
static const ex_getset_t __type_meta_getsets[] = {
    { "red", __color3f_get_red, NULL },
    { "green", __color3f_get_green, NULL },
    { "blue", __color3f_get_blue, NULL },
    { "black", __color3f_get_black, NULL },
    { "white", __color3f_get_white, NULL },
    { "yellow", __color3f_get_yellow, NULL },
    { "cyan", __color3f_get_cyan, NULL },
    { "magenta", __color3f_get_magenta, NULL },
    { "gray", __color3f_get_gray, NULL },
    { NULL, NULL, NULL },
};
static const luaL_Reg __type_meta_funcs[] = {
    { "__newindex", __type_meta_newindex },
    { "__index", __type_meta_index },
    { "__call", __color3f_new },
    { NULL, NULL },
};

// ex.color3f
static const ex_getset_t __meta_getsets[] = {
    { "r", __color3f_get_r, __color3f_set_r },
    { "g", __color3f_get_g, __color3f_set_g },
    { "b", __color3f_get_b, __color3f_set_b },
    { "1", __color3f_get_r, __color3f_set_r },
    { "2", __color3f_get_g, __color3f_set_g },
    { "3", __color3f_get_b, __color3f_set_b },
    { NULL, NULL, NULL },
};
static const luaL_Reg __meta_funcs[] = {
    { "__newindex", __meta_newindex },
    { "__index", __meta_index },
    { "__tostring", __color3f_tostring },
    { "__concat", __color3f_concat },
    { "__len", __color3f_len },
    { "__eq", __color3f_eq },
    { "__add", __color3f_add },
    { "__sub", __color3f_sub },
    { "__mul", __color3f_mul },
    { "__div", __color3f_div },
    // { "__unm", __color3f_unm },
    { "copy", __color3f_copy },
    { NULL, NULL },
};

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int luaopen_color3f ( lua_State *_l ) {

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
                              "color3f",
                              __typename,
                              __meta_funcs, 
                              __type_meta_funcs );
    lua_pop(_l, 1); // pops ex
    
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void luaclose_color3f () {
    // deinit the hashtable
    ex_hashmap_deinit ( &__key_to_type_meta_getset );
    ex_hashmap_deinit ( &__key_to_meta_getset );
}

