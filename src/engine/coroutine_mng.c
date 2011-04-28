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
// defines
extern void ex_lua_process_resume ( void *_params );
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_coroutine_mng_init ( ex_coroutine_mng_t *_self ) {
    _self->mutex = ex_create_mutex();

    _self->num_cur_frame = 0;
    _self->num_next_frame = 0;

    _self->coroutines.head = _self->coroutines.trail = _self->coroutines.count = 0;
    _self->coroutines_nf.head = _self->coroutines_nf.trail = _self->coroutines_nf.count = 0;
    _self->coroutines_eof.head = _self->coroutines_eof.trail = _self->coroutines_eof.count = 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_coroutine_mng_deinit ( ex_coroutine_mng_t *_self ) {
    int i, num_stop;

    // deinit coroutines
    num_stop = _self->coroutines.count;
    for ( i = 0; i < num_stop; ++i ) {
        ex_lua_process_resume ( &(_self->coroutines.params_list[_self->coroutines.head]) );
        _self->coroutines.head = (_self->coroutines.head + 1) % MAX_COROUTINES; 
        _self->coroutines.count -= 1;
    }

    // deinit coroutines next frame
    num_stop = _self->coroutines_nf.count;
    for ( i = 0; i < num_stop; ++i ) {
        ex_lua_process_resume ( &(_self->coroutines_nf.params_list[_self->coroutines_nf.head]) );
        _self->coroutines_nf.head = (_self->coroutines_nf.head + 1) % MAX_COROUTINES; 
        _self->coroutines_nf.count -= 1;
    }
    _self->num_cur_frame = 0;
    _self->num_next_frame = 0;

    // deinit coroutines end of frame
    num_stop = _self->coroutines_eof.count;
    for ( i = 0; i < num_stop; ++i ) {
        ex_lua_process_resume ( &(_self->coroutines_eof.params_list[_self->coroutines_eof.head]) );
        _self->coroutines_eof.head = (_self->coroutines_eof.head + 1) % MAX_COROUTINES; 
        _self->coroutines_eof.count -= 1;
    }

    //
    _self->coroutines.head = _self->coroutines.trail = _self->coroutines.count = 0;
    _self->coroutines_nf.head = _self->coroutines_nf.trail = _self->coroutines_nf.count = 0;
    _self->coroutines_eof.head = _self->coroutines_eof.trail = _self->coroutines_eof.count = 0;
    ex_destroy_mutex(_self->mutex);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_coroutine_mng_add_to_resume ( ex_coroutine_mng_t *_self, ex_coroutine_params_t *_params ) {
    int cur_idx;

    ex_mutex_lock(_self->mutex);
        ex_assert ( _self->coroutines.count + 1 <= MAX_COROUTINES, "coroutines list out of range." );
        cur_idx = _self->coroutines.trail;
        _self->coroutines.trail = (_self->coroutines.trail + 1) % MAX_COROUTINES;
        _self->coroutines.params_list[cur_idx] = *_params;
        _self->coroutines.count += 1;
    ex_mutex_unlock(_self->mutex);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_coroutine_mng_add_to_resume_nf ( ex_coroutine_mng_t *_self, ex_coroutine_params_t *_params ) {
    int cur_idx;

    ex_assert ( _self->coroutines_nf.count + 1 <= MAX_COROUTINES, "coroutines_nf list out of range." );
    cur_idx = _self->coroutines_nf.trail;
    _self->coroutines_nf.trail = (_self->coroutines_nf.trail + 1) % MAX_COROUTINES;
    _self->coroutines_nf.params_list[cur_idx] = *_params;
    _self->coroutines_nf.count += 1;
    _self->num_next_frame += 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_coroutine_mng_add_to_resume_eof ( ex_coroutine_mng_t *_self, ex_coroutine_params_t *_params ) {
    int cur_idx;

    ex_assert ( _self->coroutines_eof.count + 1 <= MAX_COROUTINES, "coroutines_eof list out of range." );
    cur_idx = _self->coroutines_eof.trail;
    _self->coroutines_eof.trail = (_self->coroutines_eof.trail + 1) % MAX_COROUTINES;
    _self->coroutines_eof.params_list[cur_idx] = *_params;
    _self->coroutines_eof.count += 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

//
void ex_coroutine_mng_resume ( ex_coroutine_mng_t *_self ) {
    int i, num_stop;

    // process coroutines timeup
    ex_mutex_lock(_self->mutex);
        num_stop = _self->coroutines.count;
        if ( num_stop > 0 ) {
            for ( i = 0; i < num_stop; ++i ) {
                ex_lua_process_resume( &(_self->coroutines.params_list[_self->coroutines.head]) );
                _self->coroutines.head = (_self->coroutines.head + 1) % MAX_COROUTINES; 
                _self->coroutines.count -= 1;
            }
        }
    ex_mutex_unlock(_self->mutex);

    // process coroutines one frame 
    ex_assert( _self->num_cur_frame <= _self->coroutines_nf.count, 
               "fatal error! the number of coroutines process in" 
               "this frame is more than the number in coroutines_nf" );
    ex_assert( _self->num_next_frame == (_self->coroutines_nf.count - _self->num_cur_frame), 
               "fatal error! the number of coroutines process in" 
               "next frame is not the same as we expect." );
    if ( _self->num_cur_frame > 0 ) {
        for ( i = 0; i < _self->num_cur_frame; ++i ) {
            ex_lua_process_resume( &(_self->coroutines_nf.params_list[_self->coroutines_nf.head]) );
            _self->coroutines_nf.head = (_self->coroutines_nf.head + 1) % MAX_COROUTINES; 
            _self->coroutines_nf.count -= 1;
        }
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_coroutine_mng_resume_eof ( ex_coroutine_mng_t *_self ) {
    int i, num_stop;

    // process coroutines eof
    num_stop = _self->coroutines_eof.count;
    if ( num_stop > 0 ) {
        for ( i = 0; i < num_stop; ++i ) {
            ex_lua_process_resume( &(_self->coroutines_eof.params_list[_self->coroutines_eof.head]) );
            _self->coroutines_eof.head = (_self->coroutines_eof.head + 1) % MAX_COROUTINES; 
            _self->coroutines_eof.count -= 1;
        }
    }

    // switch next frame resume number
    _self->num_cur_frame = _self->num_next_frame;
    _self->num_next_frame = 0;
}
