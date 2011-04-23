// ======================================================================================
// File         : coroutine_mng.c
// Author       : Wu Jie 
// Last Change  : 04/19/2011 | 10:39:48 AM | Tuesday,April
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "coroutine_mng.h"
#include "component/lua_behavior.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

///////////////////////////////////////////////////////////////////////////////
// internal defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __coroutine_to_call ( ex_coroutine_params_t *_params ) {
    int status;
    lua_State *thread_state = (lua_State *)_params->thread_state;

    //
    lua_rawgeti( thread_state, LUA_REGISTRYINDEX, _params->lua_funcID );
    ex_lua_pushobject( thread_state, _params->self );
    status = lua_pcall( thread_state, 1, 0, 0 );
    if ( status ) {
        ex_lua_alert(thread_state);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __coroutine_to_stop ( ex_coroutine_params_t *_params ) {
    // ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,_params->self);
    lua_State *thread_state = (lua_State *)_params->thread_state;

    luaL_unref( thread_state, LUA_REGISTRYINDEX, _params->lua_funcID );
    luaL_unref( thread_state, LUA_REGISTRYINDEX, _params->lua_threadID );
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_coroutine_mng_init ( ex_coroutine_mng_t *_self ) {
    _self->coroutine_to_call_mutex = ex_create_mutex();
    _self->coroutine_to_stop_mutex = ex_create_mutex();

    _self->coroutines_to_call.head = _self->coroutines_to_call.trail = _self->coroutines_to_call.count = 0;
    _self->coroutines_to_stop.head = _self->coroutines_to_stop.trail = _self->coroutines_to_stop.count = 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_coroutine_mng_deinit ( ex_coroutine_mng_t *_self ) {
    int i, num_stop;

    //
    num_stop = _self->coroutines_to_stop.count;
    for ( i = 0; i < num_stop; ++i ) {
        __coroutine_to_stop( &(_self->coroutines_to_stop.params_list[_self->coroutines_to_stop.head]) );
        _self->coroutines_to_stop.head = (_self->coroutines_to_stop.head + 1) % MAX_COROUTINES; 
        _self->coroutines_to_stop.count -= 1;
    }

    //
    _self->coroutines_to_call.head = _self->coroutines_to_call.trail = _self->coroutines_to_call.count = 0;
    _self->coroutines_to_stop.head = _self->coroutines_to_stop.trail = _self->coroutines_to_stop.count = 0;

    //
    ex_destroy_mutex(_self->coroutine_to_call_mutex);
    ex_destroy_mutex(_self->coroutine_to_stop_mutex);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_coroutine_mng_add_to_call ( ex_coroutine_mng_t *_self, ex_coroutine_params_t *_params ) {
    int cur_idx;

    ex_mutex_lock(_self->coroutine_to_call_mutex);
        ex_assert ( _self->coroutines_to_call.count + 1 <= MAX_COROUTINES, "coroutines_to_call list out of range." );
        cur_idx = _self->coroutines_to_call.trail;
        _self->coroutines_to_call.trail = (_self->coroutines_to_call.trail + 1) % MAX_COROUTINES;
        _self->coroutines_to_call.params_list[cur_idx] = *_params;
        _self->coroutines_to_call.count += 1;
    ex_mutex_unlock(_self->coroutine_to_call_mutex);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_coroutine_mng_add_to_stop ( ex_coroutine_mng_t *_self, ex_coroutine_params_t *_params ) {
    int cur_idx;

    ex_mutex_lock(_self->coroutine_to_stop_mutex);
        ex_assert ( _self->coroutines_to_stop.count + 1 <= MAX_COROUTINES, "coroutines_to_stop list out of range." );
        cur_idx = _self->coroutines_to_stop.trail;
        _self->coroutines_to_stop.trail = (_self->coroutines_to_stop.trail + 1) % MAX_COROUTINES;
        _self->coroutines_to_stop.params_list[cur_idx] = *_params;
        _self->coroutines_to_stop.count += 1;
    ex_mutex_unlock(_self->coroutine_to_stop_mutex);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

//
void ex_coroutine_mng_process ( ex_coroutine_mng_t *_self ) {
    int i, num_call, num_stop;

    // process coroutines to call
    ex_mutex_lock(_self->coroutine_to_call_mutex);
        num_call = _self->coroutines_to_call.count;
        for ( i = 0; i < num_call; ++i ) {
            __coroutine_to_call( &(_self->coroutines_to_call.params_list[_self->coroutines_to_call.head]) );
            _self->coroutines_to_call.head = (_self->coroutines_to_call.head + 1) % MAX_COROUTINES; 
            _self->coroutines_to_call.count -= 1;
        }
    ex_mutex_unlock(_self->coroutine_to_call_mutex);

    // process coroutines to stop
    // NOTE: this prevent us stop latest coroutines_to_stop which may still exists in coroutines_to_call  
    ex_mutex_lock(_self->coroutine_to_stop_mutex);
        num_stop = _self->coroutines_to_stop.count - num_call;
        if ( num_stop > 0 ) {
            for ( i = 0; i < num_stop; ++i ) {
                __coroutine_to_stop( &(_self->coroutines_to_stop.params_list[_self->coroutines_to_stop.head]) );
                _self->coroutines_to_stop.head = (_self->coroutines_to_stop.head + 1) % MAX_COROUTINES; 
                _self->coroutines_to_stop.count -= 1;
            }
        }
    ex_mutex_unlock(_self->coroutine_to_stop_mutex);
}
