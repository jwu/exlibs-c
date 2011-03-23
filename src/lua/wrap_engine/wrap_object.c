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

EX_DEF_LUA_BUILTIN_REF( ex_object_t, object, "ex.object" )

///////////////////////////////////////////////////////////////////////////////
// type meta method
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __object_new ( lua_State *_l ) {
    ex_ref_t *r;
    
    r = ex_create_object( EX_RTTI(ex_object_t), ex_generate_uid() );
    ex_object_pushref(r);

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
    ex_ref_t *r;
    
    r = ex_lua_checkobject(_l,1);
    ex_lua_check_nullref(_l,r);
    self = EX_REF_CAST(ex_object_t,r);

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
    ex_ref_t *r;
    
    r = ex_lua_checkobject(_l,1);
    ex_lua_check_nullref(_l,r);
    self = EX_REF_CAST(ex_object_t,r);

    lua_pushstring( _l, ex_strid_to_cstr(self->name) ); 
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __object_set_name ( lua_State *_l ) {
    ex_object_t *self;
    ex_ref_t *r;
    const char *in_name;

    in_name = luaL_checkstring(_l,3);
    r = ex_lua_checkobject(_l,1);
    ex_lua_check_nullref(_l,r);
    self = EX_REF_CAST(ex_object_t,r);
    self->name = ex_strid(in_name);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// meta method
///////////////////////////////////////////////////////////////////////////////

static int __object_destroy ( lua_State *_l ) {
    ex_ref_t *r;

    r = ex_lua_checkobject(_l,1);
    ex_destroy_object(r);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ex.object.meta
static const ex_getset_t __type_meta_getsets[] = {
    { "null", __object_get_null, NULL },
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
    { "__concat", ex_lua_ref_concat },
    { "__eq", ex_lua_ref_eq },
    { "destroy", __object_destroy },
    { NULL, NULL },
};

// exposed meta getsets
const ex_getset_t *ex_object_meta_getsets = __meta_getsets;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int luaopen_object ( lua_State *_l ) {

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
    ex_lua_register_class ( _l,
                            "object",
                            "",
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

void luaclose_object () {
    // deinit the hashtable
    ex_hashmap_deinit ( &__key_to_type_meta_getset );
    ex_hashmap_deinit ( &__key_to_meta_getset );
}
