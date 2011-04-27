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
extern void ex_lua_process_resume ( void *_params );
// ------------------------------------------------------------------ 

static void __coroutine_timeup ( ex_coroutine_params_t *_params ) {
    ex_lua_process_resume ( _params );
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_coroutine_mng_init ( ex_coroutine_mng_t *_self ) {
    _self->coroutine_timeup_mutex = ex_create_mutex();
    _self->coroutines_timeup.head = _self->coroutines_timeup.trail = _self->coroutines_timeup.count = 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_coroutine_mng_deinit ( ex_coroutine_mng_t *_self ) {
    int i, num_stop;

    //
    num_stop = _self->coroutines_timeup.count;
    for ( i = 0; i < num_stop; ++i ) {
        __coroutine_timeup( &(_self->coroutines_timeup.params_list[_self->coroutines_timeup.head]) );
        _self->coroutines_timeup.head = (_self->coroutines_timeup.head + 1) % MAX_COROUTINES; 
        _self->coroutines_timeup.count -= 1;
    }

    //
    _self->coroutines_timeup.head = _self->coroutines_timeup.trail = _self->coroutines_timeup.count = 0;

    //
    ex_destroy_mutex(_self->coroutine_timeup_mutex);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_coroutine_mng_add_timeup ( ex_coroutine_mng_t *_self, ex_coroutine_params_t *_params ) {
    int cur_idx;

    ex_mutex_lock(_self->coroutine_timeup_mutex);
        ex_assert ( _self->coroutines_timeup.count + 1 <= MAX_COROUTINES, "coroutines_timeup list out of range." );
        cur_idx = _self->coroutines_timeup.trail;
        _self->coroutines_timeup.trail = (_self->coroutines_timeup.trail + 1) % MAX_COROUTINES;
        _self->coroutines_timeup.params_list[cur_idx] = *_params;
        _self->coroutines_timeup.count += 1;
    ex_mutex_unlock(_self->coroutine_timeup_mutex);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

//
void ex_coroutine_mng_process ( ex_coroutine_mng_t *_self ) {
    int i, num_stop;

    // process coroutines to stop
    ex_mutex_lock(_self->coroutine_timeup_mutex);
        num_stop = _self->coroutines_timeup.count;
        if ( num_stop > 0 ) {
            for ( i = 0; i < num_stop; ++i ) {
                __coroutine_timeup( &(_self->coroutines_timeup.params_list[_self->coroutines_timeup.head]) );
                _self->coroutines_timeup.head = (_self->coroutines_timeup.head + 1) % MAX_COROUTINES; 
                _self->coroutines_timeup.count -= 1;
            }
        }
    ex_mutex_unlock(_self->coroutine_timeup_mutex);
}
