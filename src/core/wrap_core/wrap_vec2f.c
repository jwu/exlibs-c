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


/**

// EXAMPLE { 
// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

{ "new", __vec2f_get_new, NULL },

static int __vec2f_get_new ( lua_State *_l ) {
    lua_pushcfunction(_l, __vec2f_new);
    return 1;
}
// } EXAMPLE end 

ex.vec2f = [metatable] = {
    [metatable] = {
        [__newindex] = "function: 0x101014640, C function";
        [__index] = "function: 0x101014440, C function";
        {
            { "one", __vec2f_get_one, NULL };
            { "zero", __vec2f_get_zero, NULL };
            { "up", __vec2f_get_up, NULL };
            { "right", __vec2f_get_right, NULL };
        }
        [__call] = "function: 0x101014410, C function";
    };
    [__typename] = "ex.vec2f";
    [__isbuiltin] = "true";
    [__unm] = "function: 0x1010146a0, C function";
    [__eq] = "function: 0x101014560, C function";
    [__concat] = "function: 0x1010146d0, C function";
    [__tostring] = "function: 0x101014380, C function";
    [__add] = "function: 0x1010144d0, C function";
    [__div] = "function: 0x101014670, C function";
    [__len] = "function: 0x101014530, C function";
    [__mul] = "function: 0x1010143e0, C function";
    [__sub] = "function: 0x101014500, C function";
    [__index] = "function: 0x1010143b0, C function";
    {
        { "x", __vec2f_get_x, __vec2f_set_x };
        { "y", __vec2f_get_y, __vec2f_set_y };
        { "1", __vec2f_get_x, __vec2f_set_x };
        { "2", __vec2f_get_y, __vec2f_set_y };
        { "normalized", __vec2f_get_normalized, NULL },
        { "length", __vec2f_get_length, NULL },
        { "sqr_length", __vec2f_get_lengthSQR, NULL },
    }
    [__newindex] = "function: 0x101014350, C function";
    [dot] = "function: 0x101014ac0, C function";
    [cross] = "function: 0x101014af0, C function";
};

v1 = ex.vec2f(1.0,2.0)

v1 = {
    [metatable] = {
        [metatable] = {
            [__newindex] = "function: 0x101014640, C function";
            [__index] = "function: 0x101014440, C function";
            {
                { "one", __vec2f_get_one, NULL },
                { "zero", __vec2f_get_zero, NULL },
                { "up", __vec2f_get_up, NULL },
                { "right", __vec2f_get_right, NULL },
            }
            [__call] = "function: 0x101014410, C function";
        };
        [__typename] = "ex.vec2f";
        [__isbuiltin] = "true";
        [__unm] = "function: 0x1010146a0, C function";
        [__eq] = "function: 0x101014560, C function";
        [__concat] = "function: 0x1010146d0, C function";
        [__tostring] = "function: 0x101014380, C function";
        [__add] = "function: 0x1010144d0, C function";
        [__div] = "function: 0x101014670, C function";
        [__len] = "function: 0x101014530, C function";
        [__mul] = "function: 0x1010143e0, C function";
        [__sub] = "function: 0x101014500, C function";
        [__index] = "function: 0x1010143b0, C function";
        {
            { "x", __vec2f_get_x, __vec2f_set_x },
            { "y", __vec2f_get_y, __vec2f_set_y },
            { "1", __vec2f_get_x, __vec2f_set_x },
            { "2", __vec2f_get_y, __vec2f_set_y },
            { "normalized", __vec2f_get_normalized, NULL },
            { "length", __vec2f_get_length, NULL },
            { "sqr_length", __vec2f_get_lengthSQR, NULL },
        }
        [__newindex] = "function: 0x101014350, C function";
        [dot] = "function: 0x101014ac0, C function";
        [cross] = "function: 0x101014af0, C function";
    };
};
*/

///////////////////////////////////////////////////////////////////////////////
// general defines
///////////////////////////////////////////////////////////////////////////////

EX_DEF_LUA_BUILTIN_TYPE(ex_vec2f_t,vec2f,"ex.vec2f")

///////////////////////////////////////////////////////////////////////////////
// type meta method
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_new ( lua_State *_l ) {
    vec2f_proxy_t *u;
    
    u = __pushvec2f(_l,1);
    ex_vec2f_set( &u->val, lua_tonumber(_l,2), lua_tonumber(_l,3) );

    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// type meta getset
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_one ( lua_State *_l ) {
    vec2f_proxy_t *u = ex_lua_pushvec2f(_l); 
    u->val = ex_vec2f_one;
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_zero ( lua_State *_l ) {
    vec2f_proxy_t *u = ex_lua_pushvec2f(_l); 
    u->val = ex_vec2f_zero;
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_up ( lua_State *_l ) {
    vec2f_proxy_t *u = ex_lua_pushvec2f(_l); 
    ex_vec2f_set( &u->val, 0.0f, 1.0f );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_right ( lua_State *_l ) {
    vec2f_proxy_t *u = ex_lua_pushvec2f(_l); 
    ex_vec2f_set( &u->val, 1.0f, 0.0f );
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// meta method
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_len ( lua_State *_l ) {
    lua_pushinteger(_l,2);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_eq ( lua_State *_l ) {
    ex_vec2f_t *lhs, *rhs;

    lhs = ex_lua_checkvec2f(_l,1);
    rhs = ex_lua_checkvec2f(_l,2);

    // push
    lua_pushboolean(_l, ex_vec2f_is_equal( lhs, rhs ) );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// TODO: can we use array ??? same as other op
// ------------------------------------------------------------------ 

static int __vec2f_add ( lua_State *_l ) {
    vec2f_proxy_t *r;
    ex_vec2f_t *lhs,*rhs;

    lhs = ex_lua_checkvec2f(_l,1);
    rhs = ex_lua_checkvec2f(_l,2);

    // push
    r = ex_lua_pushvec2f(_l);
    ex_vec2f_add( &r->val, lhs, rhs );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_sub ( lua_State *_l ) {
    vec2f_proxy_t *r;
    ex_vec2f_t *lhs,*rhs;

    lhs = ex_lua_checkvec2f(_l,1);
    rhs = ex_lua_checkvec2f(_l,2);

    // push
    r = ex_lua_pushvec2f(_l);
    ex_vec2f_sub( &r->val, lhs, rhs );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_mul ( lua_State *_l ) {
    vec2f_proxy_t *r;
    ex_vec2f_t *lhs = NULL;
    ex_vec2f_t *rhs = NULL;

    //
    if ( ex_lua_isvec2f(_l,1) )
        lhs = ex_lua_tovec2f(_l,1);
    if ( ex_lua_isvec2f(_l,2) )
        rhs = ex_lua_tovec2f(_l,2);

    if ( lhs != NULL && rhs != NULL ) {
        r = ex_lua_pushvec2f(_l);
        ex_vec2f_mul( &r->val, lhs, rhs );
        return 1;
    }

    if ( lhs != NULL ) {
        float rhs = (float)luaL_checknumber(_l,2);
        r = ex_lua_pushvec2f(_l);
        ex_vec2f_mul_scalar( &r->val, lhs, rhs );
        return 1;
    }

    if ( rhs != NULL ) {
        float lhs = (float)luaL_checknumber(_l,1);
        r = ex_lua_pushvec2f(_l);
        ex_vec2f_mul_scalar( &r->val, rhs, lhs );
        return 1;
    }

    // push
    return luaL_error(_l,"invalid parameter type, lhs and rhs must be %s or number", __typename);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_div ( lua_State *_l ) {
    vec2f_proxy_t *r;
    ex_vec2f_t *lhs = NULL;
    ex_vec2f_t *rhs = NULL;

    if ( ex_lua_isvec2f(_l,1) )
        lhs = ex_lua_tovec2f(_l,1);
    if ( ex_lua_isvec2f(_l,2) )
        rhs = ex_lua_tovec2f(_l,2);

    if ( lhs != NULL && rhs != NULL ) {
        r = ex_lua_pushvec2f(_l);
        ex_vec2f_div( &r->val, lhs, rhs );
        return 1;
    }

    if ( lhs != NULL ) {
        float rhs = (float)luaL_checknumber(_l,2);
        r = ex_lua_pushvec2f(_l);
        ex_vec2f_div_scalar( &r->val, lhs, rhs );
        return 1;
    }

    if ( rhs != NULL ) {
        float lhs = (float)luaL_checknumber(_l,1);
        r = ex_lua_pushvec2f(_l);
        ex_scalar_div_vec2f( &r->val, lhs, rhs );
        return 1;
    }

    // push
    return luaL_error(_l,"invalid parameter type, lhs and rhs must be %s or number", __typename);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_unm ( lua_State *_l ) {
    vec2f_proxy_t *r;
    ex_vec2f_t *lhs;

    lhs = ex_lua_checkvec2f(_l,1);

    // push
    r = ex_lua_pushvec2f(_l);
    ex_vec2f_get_neg( lhs, &r->val );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_dot ( lua_State *_l ) {
    float r;
    ex_vec2f_t *lhs,*rhs;

    lhs = ex_lua_checkvec2f(_l,1);
    rhs = ex_lua_checkvec2f(_l,2);

    // push
    r = ex_vec2f_dot( lhs, rhs );
    lua_pushnumber(_l,r);

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_cross ( lua_State *_l ) {
    float r;
    ex_vec2f_t *lhs,*rhs;

    lhs = ex_lua_checkvec2f(_l,1);
    rhs = ex_lua_checkvec2f(_l,2);

    // push
    r = ex_vec2f_cross( lhs, rhs );
    lua_pushnumber(_l,r);

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_copy ( lua_State *_l ) {
    vec2f_proxy_t *u;
    ex_vec2f_t *v;

    v = ex_lua_checkvec2f(_l,1);

    // push
    u = ex_lua_pushvec2f(_l);
    ex_vec2f_set( &u->val, v->x, v->y );
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// meta getset
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_x ( lua_State *_l ) {
    // check the value
    ex_vec2f_t *v = ex_lua_checkvec2f(_l,1);
    lua_pushnumber(_l, v->x);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_set_x ( lua_State *_l ) {
    // get and check the type
    ex_vec2f_t *v;
    
    v = ex_lua_checkvec2f(_l,1);
    v->x = luaL_checknumber(_l, 3);
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_y ( lua_State *_l ) {
    // check the value
    ex_vec2f_t *v = ex_lua_checkvec2f(_l,1);
    lua_pushnumber(_l, v->y);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_set_y ( lua_State *_l ) {
    // get and check the type
    ex_vec2f_t *v;
    
    v = ex_lua_checkvec2f(_l,1);
    v->y = luaL_checknumber(_l, 3);
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_normalized ( lua_State *_l ) {
    // check the value
    vec2f_proxy_t *r;
    ex_vec2f_t *v = ex_lua_checkvec2f(_l,1);

    r = ex_lua_pushvec2f(_l);
    ex_vec2f_get_normalize( v, &r->val );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_length ( lua_State *_l ) {
    // check the value
    float r;
    ex_vec2f_t *v = ex_lua_checkvec2f(_l,1);

    r = ex_vec2f_len( v );
    lua_pushnumber(_l,r);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_lengthSQR ( lua_State *_l ) {
    // check the value
    float r;
    ex_vec2f_t *v = ex_lua_checkvec2f(_l,1);

    r = ex_vec2f_lenSQR( v );
    lua_pushnumber(_l,r);
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ex.vec2f.meta
static const ex_getset_t __type_meta_getsets[] = {
    { "one", __vec2f_get_one, NULL },
    { "zero", __vec2f_get_zero, NULL },
    { "up", __vec2f_get_up, NULL },
    { "right", __vec2f_get_right, NULL },
    { NULL, NULL, NULL },
};
static const luaL_Reg __type_meta_funcs[] = {
    { "__newindex", __type_meta_newindex },
    { "__index", __type_meta_index },
    { "__call", __vec2f_new },
    { NULL, NULL },
};

// ex.vec2f
static const ex_getset_t __meta_getsets[] = {
    { "x", __vec2f_get_x, __vec2f_set_x },
    { "y", __vec2f_get_y, __vec2f_set_y },
    { "1", __vec2f_get_x, __vec2f_set_x },
    { "2", __vec2f_get_y, __vec2f_set_y },
    { "normalized", __vec2f_get_normalized, NULL },
    { "length", __vec2f_get_length, NULL },
    { "sqr_length", __vec2f_get_lengthSQR, NULL },
    { NULL, NULL, NULL },
};
static const luaL_Reg __meta_funcs[] = {
    { "__newindex", __meta_newindex },
    { "__index", __meta_index },
    { "__tostring", __vec2f_tostring },
    { "__concat", __vec2f_concat },
    { "__len", __vec2f_len },
    { "__eq", __vec2f_eq },
    { "__add", __vec2f_add },
    { "__sub", __vec2f_sub },
    { "__mul", __vec2f_mul },
    { "__div", __vec2f_div },
    { "__unm", __vec2f_unm },
    { "copy", __vec2f_copy },
    { "dot", __vec2f_dot },
    { "cross", __vec2f_cross },
    { NULL, NULL },
};

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int luaopen_vec2f ( lua_State *_l ) {

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
                              "vec2f",
                              __typename,
                              __meta_funcs, 
                              __type_meta_funcs );
    lua_pop(_l, 1); // pops ex
    
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void luaclose_vec2f () {
    // deinit the hashtable
    ex_hashmap_deinit ( &__key_to_type_meta_getset );
    ex_hashmap_deinit ( &__key_to_meta_getset );
}
