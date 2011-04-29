// ======================================================================================
// File         : invoke_mng.c
// Author       : Wu Jie 
// Last Change  : 04/19/2011 | 10:39:48 AM | Tuesday,April
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "invoke_mng.h"
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

static void __invoke_to_call ( ex_invoke_params_t *_params ) {
    int status;
    lua_State *thread_state;
    
    //
    thread_state = (lua_State *)_params->thread_state;
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

static void __invoke_to_stop ( ex_invoke_params_t *_params ) {
    // ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,_params->self);
    lua_State *thread_state;
    
    thread_state = (lua_State *)_params->thread_state;
    luaL_unref( thread_state, LUA_REGISTRYINDEX, _params->lua_funcID );
    luaL_unref( thread_state, LUA_REGISTRYINDEX, _params->lua_threadID );
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_invoke_mng_init ( ex_invoke_mng_t *_self ) {
    _self->invoke_to_call_mutex = ex_create_mutex();
    _self->invoke_to_stop_mutex = ex_create_mutex();

    _self->invokes_to_call.head = _self->invokes_to_call.trail = _self->invokes_to_call.count = 0;
    _self->invokes_to_stop.head = _self->invokes_to_stop.trail = _self->invokes_to_stop.count = 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_invoke_mng_deinit ( ex_invoke_mng_t *_self ) {
    int i, num_stop;

    //
    num_stop = _self->invokes_to_stop.count;
    for ( i = 0; i < num_stop; ++i ) {
        __invoke_to_stop( &(_self->invokes_to_stop.params_list[_self->invokes_to_stop.head]) );
        _self->invokes_to_stop.head = (_self->invokes_to_stop.head + 1) % MAX_INVOKES; 
        _self->invokes_to_stop.count -= 1;
    }

    //
    _self->invokes_to_call.head = _self->invokes_to_call.trail = _self->invokes_to_call.count = 0;
    _self->invokes_to_stop.head = _self->invokes_to_stop.trail = _self->invokes_to_stop.count = 0;

    //
    ex_destroy_mutex(_self->invoke_to_call_mutex);
    ex_destroy_mutex(_self->invoke_to_stop_mutex);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_invoke_mng_add_to_call ( ex_invoke_mng_t *_self, ex_invoke_params_t *_params ) {
    int cur_idx;

    ex_mutex_lock(_self->invoke_to_call_mutex);
        ex_assert ( _self->invokes_to_call.count + 1 <= MAX_INVOKES, "invokes_to_call list out of range." );
        cur_idx = _self->invokes_to_call.trail;
        _self->invokes_to_call.trail = (_self->invokes_to_call.trail + 1) % MAX_INVOKES;
        _self->invokes_to_call.params_list[cur_idx] = *_params;
        _self->invokes_to_call.count += 1;
    ex_mutex_unlock(_self->invoke_to_call_mutex);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_invoke_mng_add_to_stop ( ex_invoke_mng_t *_self, ex_invoke_params_t *_params ) {
    int cur_idx;

    ex_mutex_lock(_self->invoke_to_stop_mutex);
        ex_assert ( _self->invokes_to_stop.count + 1 <= MAX_INVOKES, "invokes_to_stop list out of range." );
        cur_idx = _self->invokes_to_stop.trail;
        _self->invokes_to_stop.trail = (_self->invokes_to_stop.trail + 1) % MAX_INVOKES;
        _self->invokes_to_stop.params_list[cur_idx] = *_params;
        _self->invokes_to_stop.count += 1;
    ex_mutex_unlock(_self->invoke_to_stop_mutex);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

//
void ex_invoke_mng_process ( ex_invoke_mng_t *_self ) {
    int i, num_call, num_stop;

    // process invokes to call
    ex_mutex_lock(_self->invoke_to_call_mutex);
        num_call = _self->invokes_to_call.count;
        for ( i = 0; i < num_call; ++i ) {
            __invoke_to_call( &(_self->invokes_to_call.params_list[_self->invokes_to_call.head]) );
            _self->invokes_to_call.head = (_self->invokes_to_call.head + 1) % MAX_INVOKES; 
            _self->invokes_to_call.count -= 1;
        }
    ex_mutex_unlock(_self->invoke_to_call_mutex);

    // process invokes to stop
    ex_mutex_lock(_self->invoke_to_stop_mutex);
        // DISABLE: num_stop = _self->invokes_to_stop.count - num_call;
        // NOTE: this prevent us stop latest invokes_to_stop which may still exists in invokes_to_call, 
        //       we leave 100 latest stop not process immediately.   
        // num_stop = _self->invokes_to_stop.count - 100;
        num_stop = _self->invokes_to_stop.count;
        if ( num_stop > 0 ) {
            for ( i = 0; i < num_stop; ++i ) {
                __invoke_to_stop( &(_self->invokes_to_stop.params_list[_self->invokes_to_stop.head]) );
                _self->invokes_to_stop.head = (_self->invokes_to_stop.head + 1) % MAX_INVOKES; 
                _self->invokes_to_stop.count -= 1;
            }
        }
    ex_mutex_unlock(_self->invoke_to_stop_mutex);
}
