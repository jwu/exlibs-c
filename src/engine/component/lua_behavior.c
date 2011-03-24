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

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

// ------------------------------------------------------------------ 
// Desc: 
// NOTE: we don't invoke __object_init here, we use ex_lua_pushluabehavior instead of ex_lua_pushobject 
// ------------------------------------------------------------------ 

static void __lua_behavior_init ( ex_ref_t *_self ) {
    ex_object_t *self = EX_REF_CAST(ex_object_t,_self);
    lua_State *l = ex_current_lua_state();
    ref_proxy_t *u = ex_lua_pushluabehavior ( l, ex_strid_to_cstr(ex_rtti_info(self)->typeID) );

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

    ex_hashmap_each ( self->name_to_timer, int, el ) {
        ex_stop_timer(el);
    } ex_hashmap_each_end;
    ex_hashmap_free(self->name_to_timer);

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
    int status;
    typedef struct param_t {
        ex_ref_t *self;
        int refID;
        char name[256];
    } param_t;
    param_t *param = *((param_t **)_params);
    lua_State *l = ex_current_lua_state();

    //
    lua_rawgeti( l, LUA_REGISTRYINDEX, param->refID );
    status = lua_pcall( l, 0, 0, 0 );
    if ( status )
        ex_lua_alert(l);

    return _interval;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __lua_behavior_on_invoke_stop ( void *_params ) {
    typedef struct param_t {
        ex_ref_t *self;
        int refID;
        char name[256];
    } param_t;
    param_t *param = *((param_t **)_params);
    lua_State *l = ex_current_lua_state();
    ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,param->self);

    //
    luaL_unref( l, LUA_REGISTRYINDEX, param->refID );
    ex_hashmap_remove_at ( self->name_to_timer, param->name );
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
    EX_MEMBER( ex_lua_behavior_t, name_to_timer, ex_hashmap(cstr, int, 8) )

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
                              int _refID ) 
{
    size_t idx = -1;
    size_t hash_idx = -1;
    int timerID = -1;
    ex_lua_behavior_t *self = EX_REF_CAST(ex_lua_behavior_t,_self);
    ex_assert_return ( strlen(_name)+1 < 256, /**/, "the name is too long, only 256 support" );

    //
    hash_idx = ex_hashmap_get_hashidx ( self->name_to_timer, &_name, &idx ); 
    if ( idx != -1 ) {
        luaL_error( EX_REF_CAST(ex_object_t,_self)->l, "The %s is already invoked! please cancle_invoke it first", _name );
        return;
    }

    //
    struct param_t {
        ex_ref_t *self;
        int refID;
        char name[256];
    } param;
    param.self = _self;
    param.refID = _refID;
    strcpy( param.name, _name );

    // create timer
    timerID = ex_add_timer( __lua_behavior_invoke, 
                            __lua_behavior_on_invoke_stop,
                            &param, 
                            sizeof(param), 
                            ex_timespan_from_secs_f32(_secs_delay), 
                            ex_timespan_from_secs_f32(_secs_repeat), 
                            EX_TIMESPAN_INFINITY );
    // add timer to hashmap 
    ex_hashmap_insert_new ( self->name_to_timer, &_name, &timerID, hash_idx, &idx );

    // now start it
    ex_start_timer(timerID);
}
