// ======================================================================================
// File         : wrap_trans2d.c
// Author       : Wu Jie 
// Last Change  : 02/26/2011 | 00:38:20 AM | Saturday,February
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "../../engine/entity.h"
#include "../../engine/component/trans2d.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

EX_DEF_LUA_BUILTIN_REF( ex_trans2d_t, trans2d, "ex.trans2d" )

///////////////////////////////////////////////////////////////////////////////
// type meta getset
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// type meta method
///////////////////////////////////////////////////////////////////////////////

// TODO: this kind of constructor looks cool!
// ent = ex.entity("entity foo 01")
// -- ent:add_comp("ex.trans2d")
// ex.trans2d( ent, {
//     pos = ex.vec2f(1.0,1.0),
//     scale = ex.vec2f(1.0,1.0),
// } )

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __trans2d_new ( lua_State *_l ) {
    ref_proxy_t *u;
    ex_ref_t *ent, *comp;
    int nargs = lua_gettop(_l);
    
    //
    u = ex_lua_pushref(_l,1);
    ent = ex_lua_checkentity(_l,2);
    comp = ex_entity_add_comp( ent, EX_TYPEID(ex_trans2d_t) );
    u->val = comp;
    ex_incref(u->val);

    if ( nargs > 1 ) {
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

static int __trans2d_get_local_pos ( lua_State *_l ) {
    ex_ref_t *r;
    vec2f_proxy_t *u;

    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);

    u = ex_lua_pushvec2f(_l);
    ex_trans2d_local_position( r, &u->val );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __trans2d_set_local_pos ( lua_State *_l ) {
    ex_ref_t *r;

    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);

    ex_vec2f_t *v = ex_lua_checkvec2f(_l,3);
    ex_trans2d_set_local_position( r, v->x, v->y );

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int __trans2d_get_local_ang ( lua_State *_l ) {
    ex_ref_t *r;
    angf_proxy_t *u;

    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);

    u = ex_lua_pushangf(_l);
    ex_trans2d_local_angle( r, &u->val );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int __trans2d_set_local_ang ( lua_State *_l ) {
    ex_ref_t *r;

    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);

    // if pass vector
    if ( lua_isnumber(_l,3) ) {
        float rad = (float)lua_tonumber(_l,3);
        ex_trans2d_set_local_angle( r, rad );
    }
    else if ( ex_lua_isangf(_l,3) ) {
        ex_angf_t *a = ex_lua_toangf(_l,3);
        ex_trans2d_set_local_angle( r, a->rad );
    }
    else {
        luaL_error( _l, "invalid parameter type, expected ex.angf or number " );
    }

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int __trans2d_get_local_scale ( lua_State *_l ) {
    ex_ref_t *r;
    vec2f_proxy_t *u;

    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);

    u = ex_lua_pushvec2f(_l);
    ex_trans2d_local_scale( r, &u->val );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int __trans2d_set_local_scale ( lua_State *_l ) {
    ex_ref_t *r;

    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);

    ex_vec2f_t *v = ex_lua_checkvec2f(_l,3);
    ex_trans2d_set_local_scale( r, v->x, v->y );

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int __trans2d_get_world_pos ( lua_State *_l ) {
    ex_ref_t *r;
    vec2f_proxy_t *u;

    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);

    u = ex_lua_pushvec2f(_l);
    ex_trans2d_world_position( r, &u->val );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int __trans2d_set_world_pos ( lua_State *_l ) {
    ex_ref_t *r;

    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);

    ex_vec2f_t *v = ex_lua_checkvec2f(_l,3);
    ex_trans2d_set_world_position( r, v->x, v->y );

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int __trans2d_get_world_ang ( lua_State *_l ) {
    ex_ref_t *r;
    angf_proxy_t *u;

    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);

    u = ex_lua_pushangf(_l);
    ex_trans2d_world_angle( r, &u->val );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int __trans2d_set_world_ang ( lua_State *_l ) {
    ex_ref_t *r;

    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);

    // if pass vector
    if ( lua_isnumber(_l,3) ) {
        float rad = (float)lua_tonumber(_l,3);
        ex_trans2d_set_world_angle( r, rad );
    }
    else if ( ex_lua_isangf(_l,3) ) {
        ex_angf_t *a = ex_lua_toangf(_l,3);
        ex_trans2d_set_world_angle( r, a->rad );
    }
    else {
        luaL_error( _l, "invalid parameter type, expected ex.angf or number " );
    }

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int __trans2d_get_world_scale ( lua_State *_l ) {
    ex_ref_t *r;
    vec2f_proxy_t *u;

    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);

    u = ex_lua_pushvec2f(_l);
    ex_trans2d_world_scale( r, &u->val );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int __trans2d_set_world_scale ( lua_State *_l ) {
    ex_ref_t *r;

    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);

    ex_vec2f_t *v = ex_lua_checkvec2f(_l,3);
    ex_trans2d_set_world_scale( r, v->x, v->y );

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int __trans2d_get_right ( lua_State *_l ) {
    ex_ref_t *r;
    vec2f_proxy_t *u;

    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);

    u = ex_lua_pushvec2f(_l);
    ex_trans2d_right( r, &u->val );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int __trans2d_set_right ( lua_State *_l ) {
    ex_ref_t *r;

    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);

    ex_vec2f_t *v = ex_lua_checkvec2f(_l,3);
    ex_trans2d_set_right( r, v );

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int __trans2d_get_up ( lua_State *_l ) {
    ex_ref_t *r;
    vec2f_proxy_t *u;

    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);

    u = ex_lua_pushvec2f(_l);
    ex_trans2d_up( r, &u->val );

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int __trans2d_set_up ( lua_State *_l ) {
    ex_ref_t *r;

    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);

    ex_vec2f_t *v = ex_lua_checkvec2f(_l,3);
    ex_trans2d_set_up( r, v );

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int __trans2d_get_parent ( lua_State *_l ) {
    ex_ref_t *r, *parent;
    ref_proxy_t *u;

    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);

    parent = EX_REF_CAST(ex_trans2d_t,r)->parent;
    if ( parent ) {
        u = ex_lua_pushtrans2d(_l);
        u->val = parent;
        ex_incref(u->val);
    }
    else {
        lua_pushnil(_l);
    }

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int __trans2d_set_parent ( lua_State *_l ) {
    ex_ref_t *r, *parent;

    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);

    if ( lua_isnil(_l,3) ) {
        ex_trans2d_set_parent ( r, NULL );
    }
    else {
        parent = ex_lua_checktrans2d(_l,3);
        ex_trans2d_set_parent ( r, parent );
    }

    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int __trans2d_get_ltow ( lua_State *_l ) {
    ex_ref_t *r;
    mat33f_proxy_t *u;

    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);

    u = ex_lua_pushmat33f(_l);
    ex_trans2d_local_to_world_mat33f( r, &u->val );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int __trans2d_get_wtol ( lua_State *_l ) {
    ex_ref_t *r;
    mat33f_proxy_t *u;

    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);

    u = ex_lua_pushmat33f(_l);
    ex_trans2d_world_to_local_mat33f( r, &u->val );
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// trans2d.move( self, x, y, [,space,trans2d] )
// ------------------------------------------------------------------ 

int __trans2d_move ( lua_State *_l ) {
    ex_ref_t *r;
    float x, y;
    int nargs = lua_gettop(_l);
    int space = EX_SPACE_SELF;

    //
    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);
    x = luaL_checknumber(_l,2);
    y = luaL_checknumber(_l,3);

    //
    if ( nargs == 4 ) {
        if ( ex_lua_istrans2d(_l,4) ) {
            ex_trans2d_translate_relative_to( r, x, y, ex_lua_totrans2d(_l,4) );
            return 0;
        }
        else {
            space = luaL_checknumber(_l,4);
        }
    }
    ex_trans2d_translate( r, x, y, space );

    //
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// trans2d.rotate( self, r )
// ------------------------------------------------------------------ 

int __trans2d_rotate ( lua_State *_l ) {
    ex_ref_t *r;
    float rad;

    //
    r = ex_lua_checktrans2d(_l,1);
    ex_lua_check_nullref(_l,r);
    rad = luaL_checknumber(_l,2);

    ex_trans2d_rotate( r, rad );

    //
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ex.trans2d.meta
static const ex_getset_t __type_meta_getsets[] = {
    { "null", __trans2d_get_null, NULL },
    { NULL, NULL, NULL },
};
static const luaL_Reg __type_meta_funcs[] = {
    { "__newindex", __type_meta_newindex },
    { "__index", __type_meta_index },
    { "__call", __trans2d_new },
    { NULL, NULL },
};

// ex.trans2d
static const ex_getset_t __meta_getsets[] = {
    { "local_position", __trans2d_get_local_pos, __trans2d_set_local_pos },
    { "local_angle", __trans2d_get_local_ang, __trans2d_set_local_ang },
    { "local_scale", __trans2d_get_local_scale, __trans2d_set_local_scale },
    { "position", __trans2d_get_world_pos, __trans2d_set_world_pos },
    { "angle", __trans2d_get_world_ang, __trans2d_set_world_ang },
    { "scale", __trans2d_get_world_scale, __trans2d_set_world_scale },
    { "right", __trans2d_get_right, __trans2d_set_right },
    { "up", __trans2d_get_up, __trans2d_set_up },
    { "parent", __trans2d_get_parent, __trans2d_set_parent },
    { "local_to_world", __trans2d_get_ltow, NULL },
    { "world_to_local", __trans2d_get_wtol, NULL },
    { NULL, NULL, NULL },
};
static const luaL_Reg __meta_funcs[] = {
    { "__gc", ex_lua_ref_gc },
    { "__newindex", __meta_newindex },
    { "__index", __meta_index },
    { "__tostring", ex_lua_ref_tostring },
    { "__concat", ex_lua_ref_concat },
    { "__eq", ex_lua_ref_eq },
    { "move", __trans2d_move },
    { "rotate", __trans2d_rotate },
    { NULL, NULL },
};

// ------------------------------------------------------------------ 
// Desc: 
extern const ex_getset_t *ex_object_meta_getsets;
extern const ex_getset_t *ex_component_meta_getsets;
// ------------------------------------------------------------------ 

int luaopen_trans2d ( lua_State *_l ) {

    const ex_getset_t *meta_getsets_including_parents[4];
    const ex_getset_t **getsets;
    const ex_getset_t *getset;

    // NOTE: since we have extern link pointers, we can't use static const define
    meta_getsets_including_parents[0] = ex_object_meta_getsets;
    meta_getsets_including_parents[1] = ex_component_meta_getsets;
    meta_getsets_including_parents[2] = __meta_getsets;
    meta_getsets_including_parents[3] = NULL;

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
                            "trans2d",
                            "ex.component",
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

void luaclose_trans2d () {
    // deinit the hashtable
    ex_hashmap_deinit ( &__key_to_type_meta_getset );
    ex_hashmap_deinit ( &__key_to_meta_getset );
}


