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
#include "engine/entity.h"
#include "engine/world.h"
#include "engine/time.h"
#include "engine/component/lua_behavior.h"

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

static int __yield ( lua_State *_l ) {
    if ( _l == ex_lua_main_state() )
        return luaL_error( _l, "the yield instruction only run in coroutine." );

    // NOTE: the argument#1 is ex.yield table

    // yield wait for function finished
    if ( lua_isnumber(_l,-1) ) {
        if ( lua_tonumber(_l,-1) == EX_YIELD_FINISHED ) {
            return 0;
        }
        else {
            lua_pushnumber(_l,EX_YIELD_WAIT_FOR_FINISH);
            return lua_yield(_l,1);
        }
    }
    else {
        return luaL_error( _l, "invalid parameter type for last argument, expected number" );
    }
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __yield_wait ( lua_State *_l ) {
    if ( _l == ex_lua_main_state() )
        return luaL_error( _l, "the yield instruction only run in coroutine." );

    // yield wait for seconds
    if ( lua_isnumber(_l,1) ) {
        lua_pushnumber(_l,EX_YIELD_WAIT_FOR_SECONDS);
        return lua_yield(_l,2);
    }
    else {
        return luaL_error( _l, "invalid parameter type, expected number" );
    }

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __yield_one_frame ( lua_State *_l ) {
    if ( _l == ex_lua_main_state() )
        return luaL_error( _l, "the yield instruction only run in coroutine." );

    lua_pushnumber(_l,EX_YIELD_WAIT_FOR_ONE_FRAME);
    return lua_yield(_l,2);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __yield_end_of_frame ( lua_State *_l ) {
    if ( _l == ex_lua_main_state() )
        return luaL_error( _l, "the yield instruction only run in coroutine." );

    lua_pushnumber(_l,EX_YIELD_WAIT_FOR_END_OF_FRAME);
    return lua_yield(_l,2);
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
    { "__call", __yield }, // wait for function finish
    { NULL, NULL },
};

// ex.yield
static const luaL_Reg __meta_funcs[] = {
    { "wait", __yield_wait }, // wait for seconds
    { "one_frame", __yield_one_frame }, // wait for next frame
    { "end_of_frame", __yield_end_of_frame }, // wait for end of frame
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

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_lua_process_yield ( void *_info ) {

    ex_coroutine_info_t *info;
    int yield_status, timerID;

    //
    info = (ex_coroutine_info_t *)_info;
    yield_status = luaL_checknumber(info->thread_state,-1);
    timerID = EX_INVALID_TIMER_ID;

    //
    switch ( yield_status ) {

    // ======================================================== 
    case EX_YIELD_WAIT_FOR_SECONDS:
    // ======================================================== 
        {
            info->wait_for_secs = (float)lua_tonumber(info->thread_state,1);
        }
        break;

    // ======================================================== 
    case EX_YIELD_WAIT_FOR_ONE_FRAME:
    // ======================================================== 
        {
            // do nothing
        }
        break;

    // ======================================================== 
    case EX_YIELD_WAIT_FOR_END_OF_FRAME:
    // ======================================================== 
        {
            // do nothing
        }
        break;

    // ======================================================== 
    case EX_YIELD_WAIT_FOR_FINISH:
    // ======================================================== 
        {
            // do nothing
        }
        break;
    }

    //
    info->yield_status = yield_status;
    info->cur_frame = ex_frames();
}
