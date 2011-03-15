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
extern void __behavior_init ( ex_ref_t * );
// ------------------------------------------------------------------ 

static void init ( ex_ref_t *_self ) {
    __behavior_init(_self); // parent init
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __behavior_deinit ( ex_ref_t * );
// ------------------------------------------------------------------ 

static void deinit ( ex_ref_t *_self ) {
    ex_lua_behavior_t *self;

    __behavior_deinit(_self); // parent deinint

    self = EX_REF_CAST(ex_lua_behavior_t,_self);
    luaL_unref(self->l, LUA_REGISTRYINDEX, self->lua_refID);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void awake ( ex_ref_t *_self ) {
    int status;
    ex_lua_behavior_t *self;
    ex_ref_t *ref;

    self = EX_REF_CAST(ex_lua_behavior_t,_self);
    if ( self->compile_failed )
        return;

    lua_rawgeti(self->l, LUA_REGISTRYINDEX, self->lua_refID);
    ref = ex_lua_checkref(self->l,-1);

    lua_getfield( self->l, -1, "awake" );
    if ( lua_isnil(self->l,-1) == 0 ) {
        // ex_incref(ref); // TODO: looks like we don't need it
        lua_pushvalue(self->l,-2);
        status = lua_pcall( self->l, 1, 0, 0 );
        if ( status ) {
            ex_lua_alert(self->l);
            self->compile_failed = true;
        }
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void start ( ex_ref_t *_self ) {
    int status;
    ex_lua_behavior_t *self;
    ex_ref_t *ref;

    self = EX_REF_CAST(ex_lua_behavior_t,_self);
    if ( self->compile_failed )
        return;

    lua_rawgeti(self->l, LUA_REGISTRYINDEX, self->lua_refID);
    ref = ex_lua_checkref(self->l,-1);

    lua_getfield( self->l, -1, "start" );
    if ( lua_isnil(self->l,-1) == 0 ) {
        // ex_incref(ref); // TODO: looks like we don't need it
        lua_pushvalue(self->l,-2);
        status = lua_pcall( self->l, 1, 0, 0 );
        if ( status ) {
            ex_lua_alert(self->l);
            self->compile_failed = true;
        }
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void update ( ex_ref_t *_self ) {
    int status;
    ex_lua_behavior_t *self;
    ex_ref_t *ref;

    self = EX_REF_CAST(ex_lua_behavior_t,_self);
    if ( self->compile_failed )
        return;

    lua_rawgeti(self->l, LUA_REGISTRYINDEX, self->lua_refID);
    ref = ex_lua_checkref(self->l,-1);

    lua_getfield( self->l, -1, "update" );
    if ( lua_isnil(self->l,-1) == 0 ) {
        // ex_incref(ref); // TODO: looks like we don't need it
        lua_pushvalue(self->l,-2);
        status = lua_pcall( self->l, 1, 0, 0 );
        if ( status ) {
            ex_lua_alert(self->l);
            self->compile_failed = true;
        }
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void post_update ( ex_ref_t *_self ) {
    int status;
    ex_lua_behavior_t *self;
    ex_ref_t *ref;

    self = EX_REF_CAST(ex_lua_behavior_t,_self);
    if ( self->compile_failed )
        return;

    lua_rawgeti(self->l, LUA_REGISTRYINDEX, self->lua_refID);
    ref = ex_lua_checkref(self->l,-1);

    lua_getfield( self->l, -1, "post_update" );
    if ( lua_isnil(self->l,-1) == 0 ) {
        // ex_incref(ref); // TODO: looks like we don't need it
        lua_pushvalue(self->l,-2);
        status = lua_pcall( self->l, 1, 0, 0 );
        if ( status ) {
            ex_lua_alert(self->l);
            self->compile_failed = true;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

EX_DEF_OBJECT_BEGIN( ex_lua_behavior_t,
                     "Lua Behavior",
                     init,
                     deinit )

    EX_MEMBER( ex_component_t, entity, NULL )

    EX_MEMBER( ex_behavior_t, state, EX_BEHAVIOR_STATE_NEW )
    EX_MEMBER( ex_behavior_t, enabled, true )
    EX_MEMBER( ex_behavior_t, awake, awake )
    EX_MEMBER( ex_behavior_t, start, start )
    EX_MEMBER( ex_behavior_t, update, update )
    EX_MEMBER( ex_behavior_t, post_update, post_update )

    EX_MEMBER( ex_lua_behavior_t, l, NULL )
    EX_MEMBER( ex_lua_behavior_t, compile_failed, false )
    EX_MEMBER( ex_lua_behavior_t, lua_refID, LUA_REFNIL )

EX_DEF_OBJECT_END

EX_DEF_PROPS_BEGIN(ex_lua_behavior_t)
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN_SUPER(ex_lua_behavior_t,ex_behavior_t)
EX_SERIALIZE_END

EX_DEF_TOSTRING_SUPER_BEGIN(ex_lua_behavior_t,ex_behavior_t)
EX_DEF_TOSTRING_END
