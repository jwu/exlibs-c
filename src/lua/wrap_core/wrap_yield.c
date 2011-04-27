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
// extern void ex_lua_process_resume ( void *_params );
// ------------------------------------------------------------------ 

static int32 __yield_time_up ( uint32 _interval, void *_params ) {

    ex_coroutine_params_t *params = (ex_coroutine_params_t *)_params;
    ex_component_t *be = EX_REF_CAST(ex_component_t,params->beref);
    ex_entity_t *ent = EX_REF_CAST(ex_entity_t,be->entity);
    ex_world_t *world = EX_REF_CAST(ex_world_t,ent->world);
    ex_coroutine_mng_add_timeup( &world->coroutine_mng, _params );

    return _interval;
}

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

    // TODO:

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __yield_end_of_frame ( lua_State *_l ) {
    if ( _l == ex_lua_main_state() )
        return luaL_error( _l, "the yield instruction only run in coroutine." );

    // TODO:

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

void ex_lua_process_yield ( void *_params ) {
    ex_coroutine_info_t new_info;
    ex_coroutine_params_t *params = (ex_coroutine_params_t *)_params;
    ex_lua_behavior_t *be = EX_REF_CAST(ex_lua_behavior_t,params->beref);
    int yield_status = luaL_checknumber(params->thread_state,-1);

    switch ( yield_status ) {

    // ======================================================== 
    case EX_YIELD_WAIT_FOR_SECONDS:
    // ======================================================== 
        {
            float secs;
            timespan_t ts;
            int timerID;

            //
            secs = (float)lua_tonumber(params->thread_state,1);
            ts = ex_timespan_from_secs_f32(secs);
            timerID = ex_add_timer( __yield_time_up,
                                    NULL,
                                    params, 
                                    sizeof(*params), 
                                    ts,
                                    ts * 2, // NOTE: this prevent interval invoked before lifetime 
                                    ts );
            // add timer to hashmap 
            new_info.thread_state = params->thread_state;
            new_info.yield_status = EX_YIELD_WAIT_FOR_SECONDS;
            new_info.timerID = timerID;
            new_info.cur_frame = ex_frames();
        }
        break;

    // ======================================================== 
    case EX_YIELD_WAIT_FOR_FINISH:
    // ======================================================== 
        {
            new_info.thread_state = params->thread_state;
            new_info.yield_status = EX_YIELD_WAIT_FOR_SECONDS;
            new_info.timerID = EX_INVALID_TIMER_ID;
            new_info.cur_frame = ex_frames();
        }
        break;
    }

    // if we have nameID, update the coroutine info
    if ( params->nameID != EX_STRID_NULL ) {
        // if we already have the coroutine info, update the value in it.
        size_t idx;
        ex_mutex_lock(be->co_mutex);
            if ( ex_hashmap_insert ( be->name_to_co, &params->nameID, &new_info, &idx ) == false ) {
                ex_coroutine_info_t *my_info = ex_hashmap_get_by_idx ( be->name_to_co, idx );
                *my_info = new_info; 
            }
        ex_mutex_unlock(be->co_mutex);
    }

    // after we update the coroutine info, if we have timer, start it!
    if ( new_info.timerID != EX_INVALID_TIMER_ID )
        ex_start_timer(new_info.timerID);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_lua_process_resume ( void *_params ) {
    int status;
    ex_coroutine_params_t *parent_params = NULL;
    ex_coroutine_params_t *params = (ex_coroutine_params_t *)_params;
    ex_lua_behavior_t *be = EX_REF_CAST(ex_lua_behavior_t,params->beref);

    status = lua_resume(params->thread_state,0);
    if ( status == LUA_YIELD ) {
        ex_lua_process_yield ( params );
    }
    else if ( status != 0 ) {
        ex_lua_alert(ex_lua_main_state());
        be->compile_failed = true;
    }
    else {
        luaL_unref( params->thread_state, LUA_REGISTRYINDEX, params->lua_threadID );

        //
        ex_mutex_lock(be->co_mutex);
            // get parent params before hash remove it
            parent_params = ex_hashmap_get( be->state_to_params, &(params->parent_state), NULL ); 
            if ( params->nameID != EX_STRID_NULL ) {
                ex_hashmap_remove_at ( be->name_to_co, &(params->nameID) );
            }
            ex_hashmap_remove_at ( be->state_to_params, &(params->thread_state) );
        ex_mutex_unlock(be->co_mutex);

        // recursivly process it
        if ( parent_params ) {
            if ( lua_status(parent_params->thread_state) == LUA_YIELD ) {
                ex_lua_process_resume ( parent_params );
            }
        }
    }
}
