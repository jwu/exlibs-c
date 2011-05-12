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
#include "../time.h"

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
    ex_object_t *obj;
    lua_State *l;
    ref_proxy_t *u;

    obj = EX_REF_CAST(ex_object_t,_self);
    l = ex_lua_main_state();
    u = ex_lua_newluabehavior ( l, ex_strid_to_cstr(ex_rtti_info(obj)->typeID) );

    u->val = _self;
    ex_incref(u->val);
    obj->l = l;
    obj->luaRefID = luaL_ref(l, LUA_REGISTRYINDEX);
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __behavior_deinit ( ex_ref_t * );
// ------------------------------------------------------------------ 

static void __lua_behavior_deinit ( ex_ref_t *_self ) {
    ex_lua_behavior_t *self;
    
    self = EX_REF_CAST(ex_lua_behavior_t,_self);

    //
    ex_array_raw_each ( self->invoke_list, ex_invoke_info_t *, invokeInfo ) {
        luaL_unref( invokeInfo->thread_state, LUA_REGISTRYINDEX, invokeInfo->lua_funcID );
        luaL_unref( invokeInfo->thread_state, LUA_REGISTRYINDEX, invokeInfo->lua_threadID );
    } ex_array_each_end;
    ex_array_free ( self->invoke_list );

    //
    ex_array_raw_each ( self->coroutine_list, ex_coroutine_info_t *, coInfo ) {
        luaL_unref( coInfo->thread_state, LUA_REGISTRYINDEX, coInfo->lua_threadID );
    } ex_array_each_end;
    ex_array_free ( self->coroutine_list );

    // parent deinint
    __behavior_deinit(_self);
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
            self->compile_failed = true;
            ex_lua_alert(obj->l);
        }
    }
    lua_settop(obj->l, 0);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __lua_behavior_start ( ex_ref_t *_self ) {
    ex_object_t *obj;
    ex_lua_behavior_t *self;

    obj = EX_REF_CAST(ex_object_t,_self);
    self = EX_REF_CAST(ex_lua_behavior_t,_self);
    if ( self->compile_failed )
        return;

    lua_rawgeti(obj->l, LUA_REGISTRYINDEX, obj->luaRefID);
    lua_getfield( obj->l, -1, "start" );
    // we call start as a coroutine
    if ( lua_isnil(obj->l,-1) == 0 && lua_isfunction(obj->l,-1) ) {
        lua_pushvalue(obj->l,-2);
        ex_lua_behavior_start_coroutine( _self, obj->l, NULL, 1 );
    }
    lua_settop(obj->l, 0);
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
            self->compile_failed = true;
            ex_lua_alert(obj->l);
        }
    }
    lua_settop(obj->l, 0);
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
            self->compile_failed = true;
            ex_lua_alert(obj->l);
        }
    }
    lua_settop(obj->l, 0);
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
            self->compile_failed = true;
            ex_lua_alert(obj->l);
        }
    }
    lua_settop(obj->l, 0);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __lua_behavior_invoke ( ex_ref_t *_self, void *_info ) {
    ex_invoke_info_t *invokeInfo;
    int status;
    lua_State *thread_state;
    
    invokeInfo = (ex_invoke_info_t *)_info;
    thread_state = (lua_State *)invokeInfo->thread_state;
    lua_rawgeti( thread_state, LUA_REGISTRYINDEX, invokeInfo->lua_funcID );
    ex_lua_pushobject( thread_state, _self );
    status = lua_pcall( thread_state, 1, 0, 0 );
    if ( status ) {
        ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,_self);
        self->compile_failed = true;
        ex_lua_alert(thread_state);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
extern void ex_lua_process_yield ( void *_params );
// ------------------------------------------------------------------ 

static void __lua_behavior_resume ( ex_ref_t *_self, void *_info ) {
    int status;
    ex_coroutine_info_t *parentInfo, *coInfo;

    coInfo = (ex_coroutine_info_t *)_info;
    status = lua_resume(coInfo->thread_state,0); // resume lua script
    if ( status == LUA_YIELD ) {
        ex_lua_process_yield ( coInfo );
    }
    else if ( status != 0 ) {
        ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,_self);
        self->compile_failed = true;
        ex_lua_alert(coInfo->thread_state);
    }
    else {
        coInfo->is_dead = true;

        // get parent coInfo before hash remove it
        parentInfo = ex_lua_behavior_get_coroutine_info( _self, coInfo->parent_state );

        // recursivly process it
        // to prevent it parent state resume by other yield_status, we only allow resume from wait for finish 
        if ( parentInfo && parentInfo->yield_status == EX_YIELD_WAIT_FOR_FINISH ) {
            if ( lua_status(parentInfo->thread_state) == LUA_YIELD ) {
                __lua_behavior_resume ( _self, parentInfo );
            }
        }
    }
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
    EX_MEMBER( ex_lua_behavior_t, invoke_list, ex_array_notype(sizeof(ex_invoke_info_t),16) )
    EX_MEMBER( ex_lua_behavior_t, coroutine_list, ex_array_notype(sizeof(ex_coroutine_info_t),16) )

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

int ex_lua_behavior_start_coroutine ( ex_ref_t *_self, 
                                      lua_State *_cur_state,
                                      const char *_name,
                                      int _nargs ) {
    lua_State *l1;
    int status;
    strid_t nameID;
    
    nameID = ex_strid(_name);

    // create new thread first
    l1 = lua_newthread(_cur_state);
    ex_assert ( lua_isthread(_cur_state,-1), "it is not a thread" );
    // TODO: I think we could have smart way make this move to lua_threadID { 
    lua_xmove ( _cur_state, l1, 1 ); // move the thread_state from _cur_state to l1.
    // } TODO end 
    ex_assert ( lua_isfunction(_cur_state,-_nargs-1), "it is not a function" );
    lua_xmove ( _cur_state, l1, _nargs+1 );

    // if resume is DEAD, just stop it.
    status = lua_resume(l1,_nargs); // status 0 means normal finished.  
    if ( status == LUA_YIELD ) {
        int lua_threadID;
        ex_coroutine_info_t coInfo;
        ex_lua_behavior_t *self;

        //
        lua_pushthread(l1);
        lua_threadID = luaL_ref( l1, LUA_REGISTRYINDEX );

        // keep thread_state reference by itself to prevent gc.
        coInfo.is_dead = false;
        coInfo.parent_state = _cur_state;
        coInfo.thread_state = l1;
        coInfo.lua_threadID = lua_threadID;
        coInfo.nameID = nameID;
        ex_lua_process_yield ( &coInfo );

        //
        self = EX_REF_CAST(ex_lua_behavior_t,_self);
        ex_array_append ( self->coroutine_list, &coInfo );

        //
        lua_xmove ( l1, _cur_state, 1 );
        return 1;
    } 
    else if ( status != 0 ) {
        ex_lua_behavior_t *self;
        
        self = EX_REF_CAST(ex_lua_behavior_t,_self);
        self->compile_failed = true;
        ex_lua_alert(l1);
    }

    //
    lua_pushnumber( _cur_state, EX_YIELD_FINISHED );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// NOTE: stop coroutine only stop the specific coroutine function, and 
//       will not stop coroutine invoked by this coroutine function. 
// ------------------------------------------------------------------ 

int ex_lua_behavior_stop_coroutine ( ex_ref_t *_self, const char *_name ) {

    ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,_self);
    strid_t nameID = ex_strid(_name);
    ex_coroutine_info_t *parentInfo;

    ex_array_raw_each ( self->coroutine_list, ex_coroutine_info_t *, coInfo ) {
        if ( coInfo->nameID == nameID ) {
            coInfo->is_dead = true;

            // get parent coInfo before hash remove it
            parentInfo = ex_lua_behavior_get_coroutine_info( _self, coInfo->parent_state );

            // recursivly process it
            // to prevent it parent state resume by other yield_status, we only allow resume from wait for finish 
            if ( parentInfo && parentInfo->yield_status == EX_YIELD_WAIT_FOR_FINISH ) {
                if ( lua_status(parentInfo->thread_state) == LUA_YIELD ) {
                    __lua_behavior_resume ( _self, parentInfo );
                }
            }
        }
    } ex_array_each_end;

    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_lua_behavior_process_coroutines ( ex_ref_t *_self ) {
    ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,_self);

    ex_array_raw_each ( self->coroutine_list, ex_coroutine_info_t *, coInfo ) {
        //
        if ( coInfo->is_dead ) {
            luaL_unref( coInfo->thread_state, LUA_REGISTRYINDEX, coInfo->lua_threadID );

            ex_array_remove_at_fast( self->coroutine_list, __idx__ );
            continue;
        }

        //
        if ( coInfo->yield_status == EX_YIELD_WAIT_FOR_SECONDS ) {
            coInfo->wait_for_secs -= ex_dt ();
            if ( coInfo->wait_for_secs <= 0.0f ) {
                __lua_behavior_resume ( _self, coInfo );
            }
        }
        else if ( coInfo->yield_status == EX_YIELD_WAIT_FOR_ONE_FRAME ) {
            coInfo->cur_frame != ex_frames ();
            __lua_behavior_resume ( _self, coInfo );
        }
    } ex_array_each_end;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_lua_behavior_process_coroutines_eof ( ex_ref_t *_self ) {
    ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,_self);

    ex_array_raw_each ( self->coroutine_list, ex_coroutine_info_t *, coInfo ) {
        //
        if ( coInfo->is_dead ) {
            luaL_unref( coInfo->thread_state, LUA_REGISTRYINDEX, coInfo->lua_threadID );

            ex_array_remove_at_fast( self->coroutine_list, __idx__ );
            continue;
        }

        //
        if ( coInfo->yield_status == EX_YIELD_WAIT_FOR_END_OF_FRAME ) {
            __lua_behavior_resume ( _self, coInfo );
        }
    } ex_array_each_end;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_coroutine_info_t *ex_lua_behavior_get_coroutine_info ( ex_ref_t *_self, lua_State *_state ) {
    ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,_self);
    ex_array_raw_each ( self->coroutine_list, ex_coroutine_info_t *, coInfo ) {
        if ( coInfo->thread_state == _state )
            return coInfo;
    } ex_array_each_end;
    return NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_lua_behavior_invoke ( ex_ref_t *_self, 
                              lua_State *_cur_state,
                              float _secs_delay, 
                              float _secs_repeat,
                              const char *_name ) 
{
    strid_t nameID = ex_strid(_name);
    ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,_self);

    lua_State *l1;
    int lua_threadID,lua_funcID; 
    ex_invoke_info_t invokeInfo;

    // create new thread first
    l1 = lua_newthread(_cur_state);
    ex_assert ( lua_isthread(_cur_state,-1), "it is not a thread" );
    lua_xmove( _cur_state, l1, 1 ); // move the thread_state from _cur_state to l1.
    lua_threadID = luaL_ref(l1, LUA_REGISTRYINDEX); // keep thread_state reference by itself to prevent gc.
    ex_assert ( lua_isfunction(_cur_state,-1), "it is not a function" );
    lua_xmove( _cur_state, l1, 1 ); // move the pushed func from _cur_state to l1.
    lua_funcID = luaL_ref( l1, LUA_REGISTRYINDEX );

    //
    invokeInfo.is_dead = false;
    invokeInfo.remain = _secs_delay;
    invokeInfo.repeat = _secs_repeat;
    invokeInfo.nameID = nameID;
    invokeInfo.thread_state = l1;
    invokeInfo.lua_threadID = lua_threadID;
    invokeInfo.lua_funcID = lua_funcID;

    ex_array_append ( self->invoke_list, &invokeInfo );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_lua_behavior_cancle_invoke ( ex_ref_t *_self, const char *_name ) {
    ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,_self);
    strid_t nameID = ex_strid(_name);

    ex_array_raw_each ( self->invoke_list, ex_invoke_info_t *, invokeInfo ) {
        if ( invokeInfo->nameID == nameID ) {
            invokeInfo->is_dead = true;
        }
    } ex_array_each_end;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_lua_behavior_is_invoking ( ex_ref_t *_self, const char *_name ) {
    ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,_self);
    strid_t nameID = ex_strid(_name);

    ex_array_raw_each ( self->invoke_list, ex_invoke_info_t *, invokeInfo ) {
        if ( invokeInfo->nameID == nameID )
            return true;
    } ex_array_each_end;

    return false;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_lua_behavior_process_invokes ( ex_ref_t *_self ) {
    ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,_self);

    ex_array_raw_each ( self->invoke_list, ex_invoke_info_t *, invokeInfo ) {
        if ( invokeInfo->is_dead ) {
            luaL_unref( invokeInfo->thread_state, LUA_REGISTRYINDEX, invokeInfo->lua_funcID );
            luaL_unref( invokeInfo->thread_state, LUA_REGISTRYINDEX, invokeInfo->lua_threadID );

            ex_array_remove_at_fast( self->invoke_list, __idx__ );
            continue;
        }

        //
        invokeInfo->remain -= ex_dt();
        if ( invokeInfo->remain <= 0.0f ) {
            __lua_behavior_invoke ( _self, invokeInfo );
            if ( invokeInfo->repeat <= 0.0 ) {
                invokeInfo->is_dead = true;
            }
            else {
                invokeInfo->remain = invokeInfo->repeat + invokeInfo->remain;
            }
        }
    } ex_array_each_end;
}
