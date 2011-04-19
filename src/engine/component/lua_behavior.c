// ======================================================================================
// File         : lua_behavior.c
// Author       : Wu Jie 
// Last Change  : 03/01/2011 | 15:33:40 PM | Tuesday,March
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "lua_behavior.h"

#include "../entity.h"
#include "../world.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

///////////////////////////////////////////////////////////////////////////////
// internal defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// NOTE: we don't invoke __object_init here, we use ex_lua_newluabehavior instead of ex_lua_newobject 
// ------------------------------------------------------------------ 

static void __lua_behavior_init ( ex_ref_t *_self ) {
    ex_object_t *self = EX_REF_CAST(ex_object_t,_self);
    lua_State *l = ex_lua_main_state();
    ref_proxy_t *u = ex_lua_newluabehavior ( l, ex_strid_to_cstr(ex_rtti_info(self)->typeID) );

    u->val = _self;
    ex_incref(u->val);
    self->l = l;
    self->luaRefID = luaL_ref(l, LUA_REGISTRYINDEX);
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __behavior_deinit ( ex_ref_t * );
// ------------------------------------------------------------------ 

static void __lua_behavior_deinit ( ex_ref_t *_self ) {
    ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,_self);

    // stop all invokes.
    // DELME { 
    // ex_lua_behavior_process_coroutine (_self);
    // } DELME end 
    ex_mutex_lock(self->timer_mutex);
        ex_hashmap_each ( self->name_to_timer, int, el ) {
            ex_stop_timer(el);
        } ex_hashmap_each_end;
        ex_hashmap_free(self->name_to_timer);
    ex_mutex_unlock(self->timer_mutex);

    // destroy all mutex
    ex_destroy_mutex(self->timer_mutex);
    // DELME { 
    // ex_destroy_mutex(self->invoke_mutex);
    // } DELME end 

    __behavior_deinit(_self); // parent deinint
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __lua_behavior_awake ( ex_ref_t *_self ) {
    int status;
    ex_object_t *obj;
    ex_lua_behavior_t *self;

    obj = EX_REF_CAST(ex_object_t,_self);
    self = EX_REF_CAST(ex_lua_behavior_t,_self);
    if ( self->compile_failed )
        return;

    lua_rawgeti(obj->l, LUA_REGISTRYINDEX, obj->luaRefID);
    lua_getfield( obj->l, -1, "awake" );
    if ( lua_isnil(obj->l,-1) == 0 && lua_isfunction(obj->l,-1) ) {
        lua_pushvalue(obj->l,-2);
        status = lua_pcall( obj->l, 1, 0, 0 );
        if ( status ) {
            ex_lua_alert(obj->l);
            self->compile_failed = true;
        }
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __lua_behavior_start ( ex_ref_t *_self ) {
    int status;
    ex_object_t *obj;
    ex_lua_behavior_t *self;

    obj = EX_REF_CAST(ex_object_t,_self);
    self = EX_REF_CAST(ex_lua_behavior_t,_self);
    if ( self->compile_failed )
        return;

    lua_rawgeti(obj->l, LUA_REGISTRYINDEX, obj->luaRefID);
    lua_getfield( obj->l, -1, "start" );
    if ( lua_isnil(obj->l,-1) == 0 && lua_isfunction(obj->l,-1) ) {
        lua_pushvalue(obj->l,-2);
        status = lua_pcall( obj->l, 1, 0, 0 );
        if ( status ) {
            ex_lua_alert(obj->l);
            self->compile_failed = true;
        }
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __lua_behavior_update ( ex_ref_t *_self ) {
    int status;
    ex_object_t *obj;
    ex_lua_behavior_t *self;

    obj = EX_REF_CAST(ex_object_t,_self);
    self = EX_REF_CAST(ex_lua_behavior_t,_self);
    if ( self->compile_failed )
        return;

    lua_rawgeti(obj->l, LUA_REGISTRYINDEX, obj->luaRefID);
    lua_getfield( obj->l, -1, "update" );
    if ( lua_isnil(obj->l,-1) == 0 && lua_isfunction(obj->l,-1) ) {
        lua_pushvalue(obj->l,-2);
        status = lua_pcall( obj->l, 1, 0, 0 );
        if ( status ) {
            ex_lua_alert(obj->l);
            self->compile_failed = true;
        }
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __lua_behavior_post_update ( ex_ref_t *_self ) {
    int status;
    ex_object_t *obj;
    ex_lua_behavior_t *self;

    obj = EX_REF_CAST(ex_object_t,_self);
    self = EX_REF_CAST(ex_lua_behavior_t,_self);
    if ( self->compile_failed )
        return;

    lua_rawgeti(obj->l, LUA_REGISTRYINDEX, obj->luaRefID);
    lua_getfield( obj->l, -1, "post_update" );
    if ( lua_isnil(obj->l,-1) == 0 && lua_isfunction(obj->l,-1) ) {
        lua_pushvalue(obj->l,-2);
        status = lua_pcall( obj->l, 1, 0, 0 );
        if ( status ) {
            ex_lua_alert(obj->l);
            self->compile_failed = true;
        }
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __lua_behavior_on_render ( ex_ref_t *_self ) {
    int status;
    ex_object_t *obj;
    ex_lua_behavior_t *self;

    obj = EX_REF_CAST(ex_object_t,_self);
    self = EX_REF_CAST(ex_lua_behavior_t,_self);
    if ( self->compile_failed )
        return;

    lua_rawgeti(obj->l, LUA_REGISTRYINDEX, obj->luaRefID);
    lua_getfield( obj->l, -1, "on_render" );
    if ( lua_isnil(obj->l,-1) == 0 && lua_isfunction(obj->l,-1) ) {
        lua_pushvalue(obj->l,-2);
        status = lua_pcall( obj->l, 1, 0, 0 );
        if ( status ) {
            ex_lua_alert(obj->l);
            self->compile_failed = true;
        }
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int32 __lua_behavior_invoke ( uint32 _interval, void *_params ) {
    // DELME { 
    // int status;
    // ex_invoke_params_t *params = (ex_invoke_params_t *)_params;
    // lua_State *thread_state = (lua_State *)params->thread_state;

    // //
    // lua_rawgeti( thread_state, LUA_REGISTRYINDEX, params->lua_funcID );
    // ex_lua_pushobject( thread_state, params->self );
    // status = lua_pcall( thread_state, 1, 0, 0 );
    // if ( status ) {
    //     ex_lua_alert(thread_state);
    // }
    // } DELME end 

    // DELME { 
    // int cur_idx;
    // ex_invoke_params_t *params = (ex_invoke_params_t *)_params;
    // ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,params->self);

    // ex_mutex_lock(self->invoke_mutex);
    //     cur_idx = self->num_invokes_to_call; 
    //     ex_assert( cur_idx <= MAX_INVOKES, "ERROR: invokes exceed the max value %d", MAX_INVOKES );
    //     self->invokes_to_call[cur_idx] = *params; 
    //     self->num_invokes_to_call += 1;
    // ex_mutex_unlock(self->invoke_mutex);
    // } DELME end 

    ex_invoke_params_t *params = (ex_invoke_params_t *)_params;
    ex_component_t *self = EX_REF_CAST(ex_component_t,params->self);
    ex_entity_t *ent = EX_REF_CAST(ex_entity_t,self->entity);
    ex_world_add_invoke_to_call ( ent->world, _params );

    return _interval;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __lua_behavior_on_invoke_stop ( void *_params ) {
    // DELME { 
    // ex_invoke_params_t *params = (ex_invoke_params_t *)_params;
    // ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,params->self);
    // lua_State *thread_state = (lua_State *)params->thread_state;

    // luaL_unref( thread_state, LUA_REGISTRYINDEX, params->lua_funcID );
    // luaL_unref( thread_state, LUA_REGISTRYINDEX, params->lua_threadID );

    // ex_mutex_lock(self->timer_mutex);
    //     ex_hashmap_remove_at ( self->name_to_timer, &(params->nameID) );
    // ex_mutex_unlock(self->timer_mutex);
    // } DELME end 

    // DELME { 
    // int cur_idx;
    // ex_invoke_params_t *params = (ex_invoke_params_t *)_params;
    // ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,params->self);

    // // add lua_funcID, lua_threadID to invokes_to_stop array, process them later
    // ex_mutex_lock(self->invoke_mutex);
    //     cur_idx = self->num_invokes_to_stop; 
    //     ex_assert( cur_idx <= MAX_INVOKES, "ERROR: invokes exceed the max value %d", MAX_INVOKES );
    //     self->invokes_to_stop[cur_idx] = *params; 
    //     self->num_invokes_to_stop += 1;
    // ex_mutex_unlock(self->invoke_mutex);
    // } DELME end 

    ex_invoke_params_t *params = (ex_invoke_params_t *)_params;
    ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,params->self);
    ex_component_t *comp = EX_REF_CAST(ex_component_t,params->self);
    ex_entity_t *ent = EX_REF_CAST(ex_entity_t,comp->entity);
    ex_world_add_invoke_to_stop ( ent->world, _params );

    // remove invoke name from name_to_timer table
    ex_mutex_lock(self->timer_mutex);
        ex_hashmap_remove_at ( self->name_to_timer, &(params->nameID) );
    ex_mutex_unlock(self->timer_mutex);
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

EX_DEF_OBJECT_BEGIN( ex_lua_behavior_t,
                     "Lua Behavior",
                     __lua_behavior_init,
                     __lua_behavior_deinit )

    EX_MEMBER( ex_component_t, entity, NULL )

    // invokes
    EX_MEMBER( ex_behavior_t, state, EX_BEHAVIOR_STATE_NEW )
    EX_MEMBER( ex_behavior_t, enabled, true )
    EX_MEMBER( ex_behavior_t, awake, __lua_behavior_awake )
    EX_MEMBER( ex_behavior_t, start, __lua_behavior_start )
    EX_MEMBER( ex_behavior_t, update, __lua_behavior_update )
    EX_MEMBER( ex_behavior_t, post_update, __lua_behavior_post_update )
    // events
    EX_MEMBER( ex_behavior_t, on_render, __lua_behavior_on_render )

    // 
    EX_MEMBER( ex_lua_behavior_t, compile_failed, false )
    EX_MEMBER( ex_lua_behavior_t, name_to_timer, ex_hashmap(strid, int, 8) )
    EX_MEMBER( ex_lua_behavior_t, timer_mutex, ex_create_mutex() )
    // DELME { 
    // // invokes
    // EX_MEMBER( ex_lua_behavior_t, invoke_mutex, ex_create_mutex() )
    // EX_MEMBER( ex_lua_behavior_t, num_invokes_to_call, 0 )
    // EX_MEMBER( ex_lua_behavior_t, num_invokes_to_stop, 0 )
    // } DELME end 

EX_DEF_OBJECT_END

EX_DEF_PROPS_BEGIN(ex_lua_behavior_t)
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN_SUPER(ex_lua_behavior_t,ex_behavior_t)
EX_SERIALIZE_END

EX_DEF_TOSTRING_SUPER_BEGIN(ex_lua_behavior_t,ex_behavior_t)
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// funcitons
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_lua_behavior_invoke ( ex_ref_t *_self, 
                              float _secs_delay, 
                              float _secs_repeat,
                              const char *_name,
                              lua_State *_cur_state ) 
{
    size_t idx = -1;
    size_t hash_idx = -1;
    int timerID = -1;
    strid_t nameID = ex_strid(_name);
    ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,_self);
    timespan_t lifetime = EX_TIMESPAN_INFINITY;

    lua_State *l1;
    int lua_threadID,lua_funcID; 
    ex_invoke_params_t params;

    //
    ex_mutex_lock(self->timer_mutex);
        hash_idx = ex_hashmap_get_hashidx ( self->name_to_timer, &nameID, &idx ); 
    ex_mutex_unlock(self->timer_mutex);
    if ( idx != -1 ) {
        // TEMP DISABLE { 
        // luaL_error( ex_lua_main_state(), "The %s is already invoked! please cancle_invoke it first", _name );
        // } TEMP DISABLE end 
        return;
    }

    // create new thread first
    l1 = lua_newthread(_cur_state);
    ex_assert ( lua_isthread(_cur_state,-1), "it is not a thread" );
    lua_xmove( _cur_state, l1, 1 ); // move the thread_state from _cur_state to l1.
    lua_threadID = luaL_ref(l1, LUA_REGISTRYINDEX); // keep thread_state reference by itself to prevent gc.
    ex_assert ( lua_isfunction(_cur_state,-1), "it is not a function" );
    lua_xmove( _cur_state, l1, 1 ); // move the pushed func from _cur_state to l1.
    lua_funcID = luaL_ref( l1, LUA_REGISTRYINDEX );

    //
    params.self = _self;
    params.thread_state = l1;
    params.lua_threadID = lua_threadID;
    params.lua_funcID = lua_funcID;
    params.nameID = nameID;

    //
    if ( _secs_repeat < 0.0 ) {
        _secs_repeat = _secs_delay;
        lifetime = ex_timespan_from_secs_f32(_secs_delay);
    }

    // create timer
    timerID = ex_add_timer( __lua_behavior_invoke, 
                            __lua_behavior_on_invoke_stop,
                            &params, 
                            sizeof(params), 
                            ex_timespan_from_secs_f32(_secs_delay), 
                            ex_timespan_from_secs_f32(_secs_repeat), 
                            lifetime );
    // add timer to hashmap 
    ex_mutex_lock(self->timer_mutex);
        ex_hashmap_insert_new ( self->name_to_timer, &nameID, &timerID, hash_idx, &idx );
    ex_mutex_unlock(self->timer_mutex);

    // now start it
    ex_start_timer(timerID);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_lua_behavior_cancle_invoke ( ex_ref_t *_self, const char *_name ) {
    strid_t nameID = ex_strid(_name);
    ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,_self);
    int *timerID = NULL;

    ex_mutex_lock(self->timer_mutex);
        timerID = ex_hashmap_get ( self->name_to_timer, &nameID, NULL ); 
    ex_mutex_unlock(self->timer_mutex);
    if ( timerID ) {
        ex_stop_timer(*timerID);
    }
    else {
        luaL_error( ex_lua_main_state(), "can't find %s in invoke list", _name );
        return;
    }
}

// DELME { 
// // ------------------------------------------------------------------ 
// // Desc: 
// // ------------------------------------------------------------------ 

// //
// static void __invoke_to_call ( ex_invoke_params_t *_params ) {
//     int status;
//     lua_State *thread_state = (lua_State *)_params->thread_state;

//     //
//     lua_rawgeti( thread_state, LUA_REGISTRYINDEX, _params->lua_funcID );
//     ex_lua_pushobject( thread_state, _params->self );
//     status = lua_pcall( thread_state, 1, 0, 0 );
//     if ( status ) {
//         ex_lua_alert(thread_state);
//     }
// }

// //
// static void __invoke_to_stop ( ex_invoke_params_t *_params ) {
//     ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,_params->self);
//     lua_State *thread_state = (lua_State *)_params->thread_state;

//     luaL_unref( thread_state, LUA_REGISTRYINDEX, _params->lua_funcID );
//     luaL_unref( thread_state, LUA_REGISTRYINDEX, _params->lua_threadID );

//     // remove invoke name from name_to_timer table
//     ex_hashmap_remove_at ( self->name_to_timer, &(_params->nameID) );
// }

//
// void ex_lua_behavior_process_coroutine ( ex_ref_t *_self ) {
//     ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,_self);
//     int i;

//     ex_mutex_lock(self->invoke_mutex);
//         // call triggered invokes
//         for ( i = 0; i < self->num_invokes_to_call; ++i )
//             __invoke_to_call( &(self->invokes_to_call[i]) );
//         self->num_invokes_to_call = 0;

//         // process stopped invokes
//         ex_mutex_lock(self->timer_mutex);
//             for ( i = 0; i < self->num_invokes_to_stop; ++i )
//                 __invoke_to_stop( &(self->invokes_to_stop[i]) );
//             self->num_invokes_to_stop = 0;
//         ex_mutex_unlock(self->timer_mutex);
//     ex_mutex_unlock(self->invoke_mutex);
// }
// } DELME end 
