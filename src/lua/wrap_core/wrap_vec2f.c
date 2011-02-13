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

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static ex_hashmap_t __key_to_meta_getset;
static ex_hashmap_t __key_to_type_getset;

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
    luaL_newmetatable(_l,"ex.vec2f.meta"); // NOTE: this find a table in LUA_REGISTRYINDEX
    lua_setmetatable(_l,-2);

    return u;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_meta_newindex ( lua_State *_l ) {
    const char* key;
    strid_t key_id;
    ex_getset_t *getset;

    // first use rawget check if we have value in metatable.
    // NOTE: we not allow change the builtin values. so all of them are readonly.
    if ( lua_getmetatable( _l, 1 ) == 0 ) {
        ex_error ( "fatal error: can't find the metatable!" );
        return 0;
    }
    key = luaL_checkstring(_l, 2);
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

static int __vec2f_meta_index ( lua_State *_l ) {
    const char* key;
    strid_t key_id;
    ex_getset_t *getset;

    // first use rawget check if we have value in metatable.
    if ( lua_getmetatable( _l, 1 ) == 0 ) {
        ex_error ( "fatal error: can't find the metatable!" );
        lua_pushnil(_l);
        return 1;
    }
    key = luaL_checkstring(_l, 2);
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

static int __vec2f_type_newindex ( lua_State *_l ) {
    const char* key;
    strid_t key_id;
    ex_getset_t *getset;

    // first use rawget check if we have value in metatable.
    // NOTE: we not allow change the builtin values. so all of them are readonly.
    if ( lua_getmetatable( _l, 1 ) == 0 ) {
        ex_error ( "fatal error: can't find the metatable!" );
        return 0;
    }
    key = luaL_checkstring(_l, 2);
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

static int __vec2f_type_index ( lua_State *_l ) {
    const char* key;
    strid_t key_id;
    ex_getset_t *getset;

    // first use rawget check if we have value in metatable.
    if ( lua_getmetatable( _l, 1 ) == 0 ) {
        ex_error ( "fatal error: can't find the metatable!" );
        lua_pushnil(_l);
        return 1;
    }
    key = luaL_checkstring(_l, 2);
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
// meta method
///////////////////////////////////////////////////////////////////////////////

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
// TODO: can we use array ??? same as other op
// ------------------------------------------------------------------ 

static int __vec2f_add ( lua_State *_l ) {
    vec2f_proxy_t *r,*lhs,*rhs;

    lhs = ex_lua_checkvec2f(_l,1);
    if ( lhs == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }
    rhs = ex_lua_checkvec2f(_l,2);
    if ( rhs == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }

    // push
    r = ex_lua_pushvec2f(_l,false);
    ex_vec2f_add( &r->val, &lhs->val, &rhs->val );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_sub ( lua_State *_l ) {
    vec2f_proxy_t *r,*lhs,*rhs;

    lhs = ex_lua_checkvec2f(_l,1);
    if ( lhs == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }
    rhs = ex_lua_checkvec2f(_l,2);
    if ( rhs == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }

    // push
    r = ex_lua_pushvec2f(_l,false);
    ex_vec2f_sub( &r->val, &lhs->val, &rhs->val );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_mul ( lua_State *_l ) {
    vec2f_proxy_t *r,*lhs,*rhs;

    lhs = ex_lua_checkvec2f(_l,1);
    rhs = ex_lua_checkvec2f(_l,2);

    if ( lhs != NULL && rhs != NULL ) {
        r = ex_lua_pushvec2f(_l,false);
        ex_vec2f_mul( &r->val, &lhs->val, &rhs->val );
        return 1;
    }

    if ( lhs != NULL ) {
        if ( lua_isnumber(_l,2) ) {
            float rhs = (float)lua_tonumber(_l,2);
            r = ex_lua_pushvec2f(_l,false);
            ex_vec2f_mul_scalar( &r->val, &lhs->val, rhs );
            return 1;
        }
        else {
            ex_error("invalid parameter type, rhs must be vec2f or number");
            lua_pushnil(_l);
            return 1;
        }
    }

    if ( rhs != NULL ) {
        if ( lua_isnumber(_l,1) ) {
            float lhs = (float)lua_tonumber(_l,1);
            r = ex_lua_pushvec2f(_l,false);
            ex_vec2f_mul_scalar( &r->val, &rhs->val, lhs );
            return 1;
        }
        else {
            ex_error("invalid parameter type, lhs must be vec2f or number");
            lua_pushnil(_l);
            return 1;
        }
    }

    // push
    ex_error("invalid parameter type, lhs and rhs must be vec2f or number");
    lua_pushnil(_l);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_div ( lua_State *_l ) {
    vec2f_proxy_t *r,*lhs,*rhs;

    lhs = ex_lua_checkvec2f(_l,1);
    rhs = ex_lua_checkvec2f(_l,2);

    if ( lhs != NULL && rhs != NULL ) {
        r = ex_lua_pushvec2f(_l,false);
        ex_vec2f_div( &r->val, &lhs->val, &rhs->val );
        return 1;
    }

    if ( lhs != NULL ) {
        if ( lua_isnumber(_l,2) ) {
            float rhs = (float)lua_tonumber(_l,2);
            r = ex_lua_pushvec2f(_l,false);
            ex_vec2f_div_scalar( &r->val, &lhs->val, rhs );
            return 1;
        }
        else {
            ex_error("invalid parameter type, rhs must be vec2f or number");
            lua_pushnil(_l);
            return 1;
        }
    }

    if ( rhs != NULL ) {
        if ( lua_isnumber(_l,1) ) {
            float lhs = (float)lua_tonumber(_l,1);
            r = ex_lua_pushvec2f(_l,false);
            ex_scalar_div_vec2f( &r->val, lhs, &rhs->val );
            return 1;
        }
        else {
            ex_error("invalid parameter type, lhs must be vec2f or number");
            lua_pushnil(_l);
            return 1;
        }
    }

    // push
    ex_error("invalid parameter type, lhs and rhs must be vec2f or number");
    lua_pushnil(_l);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_unm ( lua_State *_l ) {
    vec2f_proxy_t *r,*lhs;

    lhs = ex_lua_checkvec2f(_l,1);
    if ( lhs == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }

    // push
    r = ex_lua_pushvec2f(_l,false);
    ex_vec2f_get_neg( &r->val, &lhs->val );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_concat ( lua_State *_l ) {
    const char *lhs;
    vec2f_proxy_t *rhs;
    ex_string_t *str;

    lhs = luaL_checkstring(_l, 1);
    str = ex_string_alloc(lhs,256);
    rhs = ex_lua_checkvec2f(_l,2);
    if ( rhs == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        ex_string_cat( str, "nil" );
        lua_pushstring( _l, str->text );
        return 1;
    }

    // push
    EX_RTTI(vec2f)->tostring(str, &rhs->val);
    lua_pushstring(_l, str->text);
    ex_string_free(str);
    return 1;
}

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
    vec2f_proxy_t *lhs, *rhs;

    lhs = ex_lua_checkvec2f(_l,1);
    if ( lhs == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }
    rhs = ex_lua_checkvec2f(_l,2);
    if ( rhs == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }

    // push
    lua_pushboolean(_l, ex_vec2f_is_equal( &lhs->val, &rhs->val ) );
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// meta getset
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_type ( lua_State *_l ) {
    // check the value
    vec2f_proxy_t *u = ex_lua_checkvec2f(_l,1);
    if ( u == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }
    lua_pushstring(_l, "vec2f");
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_x ( lua_State *_l ) {
    // check the value
    vec2f_proxy_t *u = ex_lua_checkvec2f(_l,1);
    if ( u == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }
    lua_pushnumber(_l, u->val.x);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_set_x ( lua_State *_l ) {
    // get and check the type
    vec2f_proxy_t *u = ex_lua_checkvec2f(_l,1);
    if ( u == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        return 0;
    }
    if ( u->readonly ) {
        ex_error("the value is readonly");
        return 0;
    }
    u->val.x = luaL_checknumber(_l, 3);
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_y ( lua_State *_l ) {
    // check the value
    vec2f_proxy_t *u = ex_lua_checkvec2f(_l,1);
    if ( u == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }
    lua_pushnumber(_l, u->val.y);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_set_y ( lua_State *_l ) {
    // get and check the type
    vec2f_proxy_t *u = ex_lua_checkvec2f(_l,1);
    if ( u == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        return 0;
    }
    if ( u->readonly ) {
        ex_error("the value is readonly");
        return 0;
    }
    u->val.y = luaL_checknumber(_l, 3);
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_normalized ( lua_State *_l ) {
    // check the value
    vec2f_proxy_t *r;
    vec2f_proxy_t *u = ex_lua_checkvec2f(_l,1);
    if ( u == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }

    r = ex_lua_pushvec2f(_l, true);
    ex_vec2f_get_normalize( &r->val, &u->val );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_length ( lua_State *_l ) {
    // check the value
    float r;
    vec2f_proxy_t *u = ex_lua_checkvec2f(_l,1);
    if ( u == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }

    r = ex_vec2f_len( &u->val );
    lua_pushnumber(_l,r);
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_lengthSQR ( lua_State *_l ) {
    // check the value
    float r;
    vec2f_proxy_t *u = ex_lua_checkvec2f(_l,1);
    if ( u == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }

    r = ex_vec2f_lenSQR( &u->val );
    lua_pushnumber(_l,r);
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// type method
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_new ( lua_State *_l ) {
    vec2f_proxy_t *u;
    
    u = ex_lua_pushvec2f(_l,false);
    ex_vec2f_set( &u->val, lua_tonumber(_l,2), lua_tonumber(_l,3) );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_dot ( lua_State *_l ) {
    float r;
    vec2f_proxy_t *lhs,*rhs;

    lhs = ex_lua_checkvec2f(_l,1);
    if ( lhs == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }
    rhs = ex_lua_checkvec2f(_l,2);
    if ( rhs == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }

    // push
    r = ex_vec2f_dot( &lhs->val, &rhs->val );
    lua_pushnumber(_l,r);

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_cross ( lua_State *_l ) {
    float r;
    vec2f_proxy_t *lhs,*rhs;

    lhs = ex_lua_checkvec2f(_l,1);
    if ( lhs == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }
    rhs = ex_lua_checkvec2f(_l,2);
    if ( rhs == NULL ) {
        ex_error("invalid parameter type, expected vec2f");
        lua_pushnil(_l);
        return 1;
    }

    // push
    r = ex_vec2f_cross( &lhs->val, &rhs->val );
    lua_pushnumber(_l,r);

    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// type getset
///////////////////////////////////////////////////////////////////////////////

// DISABLE { 
// // ------------------------------------------------------------------ 
// // Desc: 
// // ------------------------------------------------------------------ 

// static int __vec2f_get_new ( lua_State *_l ) {
//     lua_pushcfunction(_l, __vec2f_new);
//     return 1;
// }
// } DISABLE end 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_one ( lua_State *_l ) {
    vec2f_proxy_t *u = ex_lua_pushvec2f(_l,true); 
    u->val = ex_vec2f_one;
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_zero ( lua_State *_l ) {
    vec2f_proxy_t *u = ex_lua_pushvec2f(_l,true); 
    u->val = ex_vec2f_zero;
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_up ( lua_State *_l ) {
    vec2f_proxy_t *u = ex_lua_pushvec2f(_l,true); 
    ex_vec2f_set( &u->val, 0.0f, 1.0f );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __vec2f_get_right ( lua_State *_l ) {
    vec2f_proxy_t *u = ex_lua_pushvec2f(_l,true); 
    ex_vec2f_set( &u->val, 1.0f, 0.0f );
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

//
int luaopen_vec2f ( lua_State *_l ) {

    // type.meta
    static const ex_getset_t __type_getsets[] = {
        { "one", __vec2f_get_one, NULL },
        { "zero", __vec2f_get_zero, NULL },
        { "up", __vec2f_get_up, NULL },
        { "right", __vec2f_get_right, NULL },
        { NULL, NULL, NULL },
    };
    static const luaL_Reg __type_funcs[] = {
        { "__newindex", __vec2f_type_newindex },
        { "__index", __vec2f_type_index },
        { "__call", __vec2f_new },
        { NULL, NULL },
    };

    // vec2f.meta
    static const ex_getset_t __meta_getsets[] = {
        // { "type", __vec2f_get_type, NULL }, // TODO:
        { "x", __vec2f_get_x, __vec2f_set_x },
        { "y", __vec2f_get_y, __vec2f_set_y },
        { "1", __vec2f_get_x, __vec2f_set_x },
        { "2", __vec2f_get_y, __vec2f_set_y },
        { "normalized", __vec2f_get_normalized, NULL },
        { "length", __vec2f_get_length, NULL },
        { "sqr_length", __vec2f_get_lengthSQR, NULL },
        // { "new", __vec2f_get_new, NULL }, // DISABLE: we use __call instead
        { NULL, NULL, NULL },
    };
    static const luaL_Reg __meta_funcs[] = {
        { "__newindex", __vec2f_meta_newindex },
        { "__index", __vec2f_meta_index },
        { "__tostring", __vec2f_tostring },
        { "__add", __vec2f_add },
        { "__sub", __vec2f_sub },
        { "__mul", __vec2f_mul },
        { "__div", __vec2f_div },
        { "__unm", __vec2f_unm },
        { "__concat", __vec2f_concat },
        { "__len", __vec2f_len },
        { "__eq", __vec2f_eq },
        // { "__type", __vec2f_get_type }, // TODO:
        { "dot", __vec2f_dot },
        { "cross", __vec2f_cross },
        { NULL, NULL },
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

#if 0
    // register ex.vec2f.meta
    luaL_newmetatable(_l, "ex.vec2f.meta"); // [-0,+1,m] // NOTE: this store a table in LUA_REGISTRYINDEX
    luaL_register(_l, NULL, __meta_funcs); // [-1,+1,m]
    lua_pop(_l, 1); // [-1,+0,-] pops ex.vec2f.meta

    // register vec2f
    lua_newtable(_l); // [-0,+1,m]
    luaL_newmetatable(_l, "ex.vec2f"); // [-0,+1,m] // NOTE: this store a table in LUA_REGISTRYINDEX
    luaL_register(_l, NULL, __type_funcs); // [-1,+1,m]
    lua_setmetatable(_l,-2); // [-1,+0,-] setmetatable( new_table, ex.vec2f )
    lua_setfield(_l,-2,"vec2f"); // [-1,+0,e] ex[vec2f] = new_table
#else
    /**
      ex.vec2f = ex.vec2f.meta = {
          type.meta = {
              __call = ...;
              __index = ...;
              __newindex = ...;
          };
          __index = ...;
          __newindex = ...;
      };
     */

    // register metatable ex.vec2f.meta
    luaL_newmetatable(_l, "ex.vec2f.meta"); // [-0,+1,m] // NOTE: this store a table in LUA_REGISTRYINDEX
    luaL_register(_l, NULL, __meta_funcs); // [-1,+1,m]

    // setmetatable( ex.vec2f.meta, newtable(aka. type.meta) )
    lua_newtable(_l); // [-0,+1,m]
    luaL_register(_l, NULL, __type_funcs); // [-1,+1,m]
    lua_setmetatable(_l,-2); // [-1,+0,-] 

    lua_setfield(_l,-2,"vec2f"); // [-1,+0,e] ex[vec2f] = ex.vec2f.meta
    lua_pop(_l, 1); // [-1,+0,-] pops ex.vec2f.meta

#endif

    lua_pop(_l, 1); // [-1,+0,-] pops ex

    return 0;
}

//
void luaclose_vec2f () {
    // deinit the hashtable
    ex_hashmap_deinit ( &__key_to_meta_getset );
    ex_hashmap_deinit ( &__key_to_type_getset );
}
