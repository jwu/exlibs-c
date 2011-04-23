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
#include "engine/coroutine_mng.h"
#include "engine/component/lua_behavior.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

///////////////////////////////////////////////////////////////////////////////
// general defines
///////////////////////////////////////////////////////////////////////////////

EX_DEF_LUA_BUILTIN_MODULE()

///////////////////////////////////////////////////////////////////////////////
// internal define
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int32 __yield_time_up ( uint32 _interval, void *_params ) {
    // TODO: add to coroutine_mng stop resume ???? do we need to process there ????
    // TODO: yes we do!
    int status;
    ex_coroutine_params_t *params = (ex_coroutine_params_t *)_params;
    lua_State *thread_state = (lua_State *)params->thread_state;

    status = lua_resume(thread_state,0);
    if ( status == LUA_YIELD ) {
        ex_lua_yield ( thread_state, 
                       params->beref,
                       params->lua_threadID,
                       params->nameID );
    }
    else if ( status != 0 ) {
        // it is possible the threadID unrefed by __yield_stop call from stop_coroutine
        if ( params->lua_threadID != LUA_REFNIL ) {
            ex_lua_behavior_t *be;

            //
            luaL_unref( thread_state, LUA_REGISTRYINDEX, params->lua_threadID );
            params->lua_threadID = LUA_REFNIL;

            //
            be = EX_REF_CAST(ex_lua_behavior_t,params->beref);
            if ( params->nameID != EX_STRID_NULL ) {
                ex_mutex_lock(be->co_mutex);
                    ex_hashmap_remove_at ( be->name_to_co, &(params->nameID) );
                ex_mutex_unlock(be->co_mutex);
            }
        }
    }

    return _interval;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __yield_stop ( void *_params ) {
    ex_coroutine_params_t *params = (ex_coroutine_params_t *)_params;
    lua_State *thread_state = (lua_State *)params->thread_state;

    // FIXME { 
    // // it is possible the threadID unrefed by __yield_time_up
    // if ( params->lua_threadID != LUA_REFNIL ) {
    //     ex_lua_behavior_t *be;

    //     //
    //     luaL_unref( thread_state, LUA_REGISTRYINDEX, params->lua_threadID );
    //     params->lua_threadID = LUA_REFNIL;

    //     //
    //     be = EX_REF_CAST(ex_lua_behavior_t,params->beref);
    //     if ( params->nameID != EX_STRID_NULL ) {
    //         ex_mutex_lock(be->co_mutex);
    //             ex_hashmap_remove_at ( be->name_to_co, &(params->nameID) );
    //         ex_mutex_unlock(be->co_mutex);
    //     }
    // }
    // } FIXME end 
}

///////////////////////////////////////////////////////////////////////////////
// type meta getset
///////////////////////////////////////////////////////////////////////////////

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
        // TODO: this will return value from lua_resume(_l);
    }
    // yield wait for seconds
    else if ( lua_isnumber(_l,1) ) {
        lua_pushnumber(_l,EX_YIELD_WAIT_FOR_SECONDS);
        return lua_yield(_l,2);
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

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_lua_yield ( lua_State *_l, ex_ref_t *_beref, int _lua_threadID, strid_t _nameID ) {
    int yield_status = luaL_checknumber(_l,2);
    ex_lua_behavior_t *be = EX_REF_CAST(ex_lua_behavior_t,_beref);

    switch ( yield_status ) {
    case EX_YIELD_WAIT_FOR_SECONDS:
        {
            float secs;
            timespan_t ts;
            int timerID;
            ex_coroutine_params_t params;
            ex_coroutine_info_t info;

            // keep thread_state reference by itself to prevent gc.
            params.beref = _beref;
            params.thread_state = _l;
            params.lua_threadID = _lua_threadID;
            params.nameID = _nameID;

            //
            secs = (float)lua_tonumber(_l,1);
            ts = ex_timespan_from_secs_f32(secs);
            timerID = ex_add_timer( __yield_time_up,
                                    __yield_stop,
                                    &params, 
                                    sizeof(params), 
                                    ts,
                                    ts * 2, // NOTE: this prevent interval invoked before lifetime 
                                    ts );
            // FIXME: this is wrong for second time yield {
            // add timer to hashmap 
            info.timerID = timerID;
            info.threadID = -1;
            if ( _nameID != EX_STRID_NULL ) {
                ex_mutex_lock(be->co_mutex);
                    ex_hashmap_insert ( be->name_to_co, &_nameID, &info, NULL );
                ex_mutex_unlock(be->co_mutex);
            }
            // } FIXME end 

            ex_start_timer(timerID);
        }
        break;
    }
    return 0;
}
