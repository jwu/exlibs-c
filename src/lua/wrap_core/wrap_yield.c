// ======================================================================================
// File         : wrap_yield.c
// Author       : Wu Jie 
// Last Change  : 04/21/2011 | 12:24:16 PM | Thursday,April
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

EX_DEF_LUA_BUILTIN_MODULE()

///////////////////////////////////////////////////////////////////////////////
// type meta getset
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int32 __yield_time_up ( uint32 _interval, void *_params ) {
    return _interval;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __yield_wait ( lua_State *_l ) {
    if ( _l == ex_lua_main_state() )
        return luaL_error( _l, "the yield instruction only run in coroutine." );

    // yield wait for function finished
    if ( lua_isfunction(_l,1) ) {
        // TODO: ex.start_coroutine( func(nargs...) )
        // return lua_yield(_l,...)
    }
    // yield wait for seconds
    else if ( lua_isnumber(_l,1) ) {
        float secs = (float)lua_tonumber(_l,1);
        timespan_t ts = ex_timespan_from_secs_f32(secs);

        // looks we have to do this in start_coroutine, so stop_coroutine can get the nameID.
        // timerID = ex_add_timer( __yield_time_up,
        //                         NULL,
        //                         &params, 
        //                         sizeof(params), 
        //                         ts,
        //                         ts, 
        //                         ts );

        return lua_yield(_l,0);
    }
    else {
        return luaL_error( _l, "invalid parameter type, expected function or number" );
    }

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __yield_wait_for_end_of_frame ( lua_State *_l ) {
    if ( _l == ex_lua_main_state() )
        return luaL_error( _l, "the yield instruction only run in coroutine." );

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ex.yield.meta
static const ex_getset_t __type_meta_getsets[] = {
    { NULL, NULL, NULL },
};
static const luaL_Reg __type_meta_funcs[] = {
    { "__newindex", __type_meta_newindex },
    { "__index", __type_meta_index },
    { NULL, NULL },
};

// ex.yield
static const luaL_Reg __meta_funcs[] = {
    { "wait", __yield_wait },
    { "wait_for_end_of_frame", __yield_wait_for_end_of_frame },
    // TODO: { "wait_for_fixed_update", __yield_wait_for_fixed_update },
    { NULL, NULL },
};

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int luaopen_yield ( lua_State *_l ) {

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

    // we create global ex table if it not exists.
    // tp = {}
    // setmetatable( tp, { __type_meta_funcs } )
    ex_lua_global_module ( _l, "ex" );
    lua_pushstring(_l,"yield");
        lua_newtable(_l);
        luaL_register(_l, NULL, __meta_funcs);
        lua_newtable(_l);
        luaL_register(_l, NULL, __type_meta_funcs);
        lua_setmetatable(_l,-2);
    lua_rawset(_l,-3); // rawset( ex, "yield", tp )
    lua_pop(_l, 1); // pops ex
    
    //
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void luaclose_yield () {
    // deinit the hashtable
    ex_hashmap_deinit ( &__key_to_type_meta_getset );
}


