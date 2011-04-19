// ======================================================================================
// File         : wrap_mat33f.c
// Author       : Wu Jie 
// Last Change  : 03/20/2011 | 10:21:21 AM | Sunday,March
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

EX_DEF_LUA_BUILTIN_TYPE(ex_mat33f_t,mat33f,"ex.mat33f")

///////////////////////////////////////////////////////////////////////////////
// type meta method
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __mat33f_new ( lua_State *_l ) {
    mat33f_proxy_t *u;
    
    u = __pushmat33f(_l,1);
    ex_mat33f_set( &u->val, 
                   lua_tonumber(_l,2), lua_tonumber(_l,3), lua_tonumber(_l,4), 
                   lua_tonumber(_l,5), lua_tonumber(_l,6), lua_tonumber(_l,7), 
                   lua_tonumber(_l,8), lua_tonumber(_l,9), lua_tonumber(_l,10) );
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// type meta getset
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __mat33f_get_identity ( lua_State *_l ) {
    mat33f_proxy_t *u = ex_lua_pushmat33f(_l); 
    u->val = ex_mat33f_identity;
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __mat33f_get_zero ( lua_State *_l ) {
    mat33f_proxy_t *u = ex_lua_pushmat33f(_l); 
    u->val = ex_mat33f_zero;
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// meta method
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __mat33f_len ( lua_State *_l ) {
    lua_pushinteger(_l,9);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// TODO: can we use array ??? same as other op
// ------------------------------------------------------------------ 

static int __mat33f_add ( lua_State *_l ) {
    mat33f_proxy_t *r;
    ex_mat33f_t *lhs,*rhs;

    lhs = ex_lua_checkmat33f(_l,1);
    rhs = ex_lua_checkmat33f(_l,2);

    // push
    r = ex_lua_pushmat33f(_l);
    ex_mat33f_add( &r->val, lhs, rhs );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __mat33f_sub ( lua_State *_l ) {
    mat33f_proxy_t *r;
    ex_mat33f_t *lhs,*rhs;

    lhs = ex_lua_checkmat33f(_l,1);
    rhs = ex_lua_checkmat33f(_l,2);

    // push
    r = ex_lua_pushmat33f(_l);
    ex_mat33f_sub( &r->val, lhs, rhs );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __mat33f_mul ( lua_State *_l ) {
    mat33f_proxy_t *r;
    ex_mat33f_t *lhs = NULL;
    ex_mat33f_t *rhs = NULL;

    //
    if ( ex_lua_ismat33f(_l,1) )
        lhs = ex_lua_tomat33f(_l,1);
    if ( ex_lua_ismat33f(_l,2) )
        rhs = ex_lua_tomat33f(_l,2);

    if ( lhs != NULL && rhs != NULL ) {
        r = ex_lua_pushmat33f(_l);
        ex_mat33f_mul( &r->val, lhs, rhs );
        return 1;
    }

    //
    if ( lhs != NULL ) {
        if ( lua_isnumber(_l,2) ) {
            float rhs = (float)lua_tonumber(_l,2);
            r = ex_lua_pushmat33f(_l);
            ex_mat33f_mul_scalar( &r->val, lhs, rhs );
        }
        // TODO { 
        // else if ( ex_lua_isvec3f(_l,2) ) {
        //     float rhs = (float)ex_lua_tovec3f(_l,2);
        //     r = ex_lua_pushmat33f(_l);
        //     ex_mat33f_mul_vec3f( &r->val, lhs, rhs );
        // }
        // } TODO end 
        else {
            return luaL_error(_l,"invalid parameter type, lhs must be %s, ex.vec3f or number", __typename);
        }
        return 1;
    }

    //
    if ( rhs != NULL ) {
        if ( lua_isnumber(_l,2) ) {
            float lhs = (float)lua_tonumber(_l,2);
            r = ex_lua_pushmat33f(_l);
            ex_mat33f_mul_scalar( &r->val, rhs, lhs );
        }
        // TODO { 
        // else if ( ex_lua_isvec3f(_l,2) ) {
        //     float lhs = (float)ex_lua_tovec3f(_l,2);
        //     r = ex_lua_pushmat33f(_l);
        //     ex_mat33f_mul_vec3f( &r->val, rhs , lhs );
        // }
        // } TODO end 
        else {
            return luaL_error(_l,"invalid parameter type, rhs must be %s, ex.vec3f or number", __typename);
        }
        return 1;
    }

    // push
    return luaL_error(_l,"invalid parameter type, lhs and rhs must be %s, ex.vec3f or number", __typename);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __mat33f_div ( lua_State *_l ) {
    mat33f_proxy_t *r;
    ex_mat33f_t *lhs = NULL;
    ex_mat33f_t *rhs = NULL;

    //
    if ( ex_lua_ismat33f(_l,1) ) {
		float rhs;

        lhs = ex_lua_tomat33f(_l,1);
        rhs = (float)luaL_checknumber(_l,2);
        r = ex_lua_pushmat33f(_l);
        ex_mat33f_div_scalar( &r->val, lhs, rhs );
        return 1;
    }

    //
    if ( ex_lua_ismat33f(_l,2) ) {
        float lhs;

        lhs = (float)luaL_checknumber(_l,1);
        rhs = ex_lua_tomat33f(_l,2);
        r = ex_lua_pushmat33f(_l);
        ex_scalar_div_mat33f( &r->val, lhs, rhs );
        return 1;
    }

    // push
    return luaL_error(_l,"invalid parameter type, lhs and rhs must be %s or number", __typename);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __mat33f_unm ( lua_State *_l ) {
    mat33f_proxy_t *r;
    ex_mat33f_t *lhs;

    lhs = ex_lua_checkmat33f(_l,1);

    // push
    r = ex_lua_pushmat33f(_l);
    ex_mat33f_get_neg( lhs, &r->val );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __mat33f_copy ( lua_State *_l ) {
    mat33f_proxy_t *u;
    ex_mat33f_t *v;

    v = ex_lua_checkmat33f(_l,1);

    // push
    u = ex_lua_pushmat33f(_l);
    ex_mat33f_set( &u->val, 
                   v->m00, v->m01, v->m02,
                   v->m10, v->m11, v->m12,
                   v->m20, v->m21, v->m22 );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __mat33f_m_get ( lua_State *_l ) {
    ex_mat33f_t *v = ex_lua_checkmat33f(_l,1);
    int row = luaL_checknumber(_l,2);
    int col = luaL_checknumber(_l,3);

    // push
    lua_pushnumber(_l, ex_mat33f_m_get(v,row,col) );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __mat33f_m_set ( lua_State *_l ) {
    ex_mat33f_t *v = ex_lua_checkmat33f(_l,1);
    int row = luaL_checknumber(_l,2);
    int col = luaL_checknumber(_l,3);
    float in = luaL_checknumber(_l,4);

    // push
    ex_mat33f_m_set(v,row,col,in);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// meta getset
///////////////////////////////////////////////////////////////////////////////

#define DEF_M_GET_SET(_m) \
    static int __mat33f_get_##_m ( lua_State *_l ) { \
        ex_mat33f_t *v = ex_lua_checkmat33f(_l,1); \
        lua_pushnumber(_l, v->_m); \
        return 1; \
    } \
    static int __mat33f_set_##_m ( lua_State *_l ) { \
        ex_mat33f_t *v; \
        v = ex_lua_checkmat33f(_l,1); \
        v->_m = luaL_checknumber(_l, 3); \
        return 0; \
    }

DEF_M_GET_SET(m00)
DEF_M_GET_SET(m01)
DEF_M_GET_SET(m02)

DEF_M_GET_SET(m10)
DEF_M_GET_SET(m11)
DEF_M_GET_SET(m12)

DEF_M_GET_SET(m20)
DEF_M_GET_SET(m21)
DEF_M_GET_SET(m22)

#undef DEF_M_GET_SET 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __mat33f_get_inverse ( lua_State *_l ) {
    // check the value
    mat33f_proxy_t *r;
    ex_mat33f_t *v = ex_lua_checkmat33f(_l,1);

    r = ex_lua_pushmat33f(_l);
    ex_mat33f_get_inverse( v, &r->val );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __mat33f_get_transpose ( lua_State *_l ) {
    // check the value
    mat33f_proxy_t *r;
    ex_mat33f_t *v = ex_lua_checkmat33f(_l,1);

    r = ex_lua_pushmat33f(_l);
    ex_mat33f_get_transpose( v, &r->val );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __mat33f_get_is_identity ( lua_State *_l ) {
    bool is_identity = false;
    ex_mat33f_t *v = ex_lua_checkmat33f(_l,1);

    if ( v->m00 == 1.0f && v->m01 == 0.0f && v->m02 == 0.0f && 
         v->m10 == 0.0f && v->m11 == 1.0f && v->m12 == 0.0f &&
         v->m20 == 0.0f && v->m21 == 0.0f && v->m22 == 1.0f )
    {
        is_identity = true;
    }

    lua_pushboolean( _l, is_identity );
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ex.mat33f.meta
static const ex_getset_t __type_meta_getsets[] = {
    { "identity", __mat33f_get_identity, NULL },
    { "zero", __mat33f_get_zero, NULL },
    { NULL, NULL, NULL },
};
static const luaL_Reg __type_meta_funcs[] = {
    { "__newindex", __type_meta_newindex },
    { "__index", __type_meta_index },
    { "__call", __mat33f_new },
    // TODO { 
    // { "TRS", __mat33f_new_TRS },
    // { "ortho", __mat33f_new_ortho },
    // { "perspective", __mat33f_new_perspective },
    // } TODO end 
    { NULL, NULL },
};

// ex.mat33f
static const ex_getset_t __meta_getsets[] = {
    //
    { "m00", __mat33f_get_m00, __mat33f_set_m00 },
    { "m01", __mat33f_get_m01, __mat33f_set_m01 },
    { "m02", __mat33f_get_m02, __mat33f_set_m02 },
    { "m10", __mat33f_get_m10, __mat33f_set_m10 },
    { "m11", __mat33f_get_m11, __mat33f_set_m11 },
    { "m12", __mat33f_get_m12, __mat33f_set_m12 },
    { "m20", __mat33f_get_m20, __mat33f_set_m20 },
    { "m21", __mat33f_get_m21, __mat33f_set_m21 },
    { "m22", __mat33f_get_m22, __mat33f_set_m22 },

    //
    { "1", __mat33f_get_m00, __mat33f_set_m00 },
    { "2", __mat33f_get_m01, __mat33f_set_m01 },
    { "3", __mat33f_get_m02, __mat33f_set_m02 },
    { "4", __mat33f_get_m10, __mat33f_set_m10 },
    { "5", __mat33f_get_m11, __mat33f_set_m11 },
    { "6", __mat33f_get_m12, __mat33f_set_m12 },
    { "7", __mat33f_get_m20, __mat33f_set_m20 },
    { "8", __mat33f_get_m21, __mat33f_set_m21 },
    { "9", __mat33f_get_m22, __mat33f_set_m22 },

    { "inverse", __mat33f_get_inverse, NULL },
    { "transpose", __mat33f_get_transpose, NULL },
    { "is_identity", __mat33f_get_is_identity, NULL },
    { NULL, NULL, NULL },
};
static const luaL_Reg __meta_funcs[] = {
    { "__newindex", __meta_newindex },
    { "__index", __meta_index },
    { "__tostring", __mat33f_tostring },
    { "__concat", __mat33f_concat },
    { "__len", __mat33f_len },
    { "__add", __mat33f_add },
    { "__sub", __mat33f_sub },
    { "__mul", __mat33f_mul },
    { "__div", __mat33f_div },
    { "__unm", __mat33f_unm },
    { "copy", __mat33f_copy },
    { "m_get", __mat33f_m_get },
    { "m_set", __mat33f_m_set },
    // TODO { 
    // { "get_row", __mat33f_get_row },
    // { "set_row", __mat33f_set_row },
    // { "get_col", __mat33f_get_col },
    // { "set_col", __mat33f_set_col },
    // { "set_TRS", __mat33f_set_TRS },
    // } TODO end 
    { NULL, NULL },
};

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int luaopen_mat33f ( lua_State *_l ) {

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
                              "mat33f",
                              __typename,
                              __meta_funcs, 
                              __type_meta_funcs );
    lua_pop(_l, 1); // pops ex
    
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void luaclose_mat33f () {
    // deinit the hashtable
    ex_hashmap_deinit ( &__key_to_type_meta_getset );
    ex_hashmap_deinit ( &__key_to_meta_getset );
}

