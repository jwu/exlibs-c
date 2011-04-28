// ======================================================================================
// File         : wrap_time.c
// Author       : Wu Jie 
// Last Change  : 03/17/2011 | 13:09:41 PM | Thursday,March
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "../../engine/time.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

///////////////////////////////////////////////////////////////////////////////
// general defines
///////////////////////////////////////////////////////////////////////////////

EX_DEF_LUA_BUILTIN_MODULE()

///////////////////////////////////////////////////////////////////////////////
// type meta getset
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __time_get_time ( lua_State *_l ) {
    lua_pushnumber( _l, ex_time() );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __time_get_time_noscale ( lua_State *_l ) {
    lua_pushnumber( _l, ex_time_noscale() );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __time_get_worldtime ( lua_State *_l ) {
    lua_pushnumber( _l, ex_worldtime() );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __time_get_realtime ( lua_State *_l ) {
    lua_pushnumber( _l, ex_realtime() );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __time_get_dt ( lua_State *_l ) {
    lua_pushnumber( _l, ex_dt() );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __time_get_dt_noscale ( lua_State *_l ) {
    lua_pushnumber( _l, ex_dt_noscale() );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __time_get_dt_fixed ( lua_State *_l ) {
    lua_pushnumber( _l, ex_dt_fixed() );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __time_get_timescale ( lua_State *_l ) {
    lua_pushnumber( _l, ex_timescale() );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __time_set_timescale ( lua_State *_l ) {
    float r = luaL_checknumber(_l,1);
    ex_set_timescale(r);
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __time_get_fps ( lua_State *_l ) {
    lua_pushnumber( _l, ex_fps() );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __time_get_frames ( lua_State *_l ) {
    lua_pushnumber( _l, ex_frames() );
    return 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __time_pause ( lua_State *_l ) {
    ex_pause();
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static int __time_resume ( lua_State *_l ) {
    ex_resume();
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// register
///////////////////////////////////////////////////////////////////////////////

// ex.time.meta
static const ex_getset_t __type_meta_getsets[] = {
    { "time", __time_get_time, NULL },
    { "time_noscale", __time_get_time_noscale, NULL },
    { "worldtime", __time_get_worldtime, NULL },
    { "realtime", __time_get_realtime, NULL },
    { "dt", __time_get_dt, NULL },
    { "dt_noscale", __time_get_dt_noscale, NULL },
    { "dt_fixed", __time_get_dt_fixed, NULL },
    { "timescale", __time_get_timescale, __time_set_timescale },
    { "fps", __time_get_fps, NULL },
    { "frames", __time_get_frames, NULL },
    { NULL, NULL, NULL },
};
static const luaL_Reg __type_meta_funcs[] = {
    { "__newindex", __type_meta_newindex },
    { "__index", __type_meta_index },
    { NULL, NULL },
};

// ex.time
static const luaL_Reg __meta_funcs[] = {
    { "pause", __time_pause },
    { "resume", __time_resume },
    { NULL, NULL },
};

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int luaopen_time ( lua_State *_l ) {

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
    lua_pushstring(_l,"time");
        lua_newtable(_l);
        luaL_register(_l, NULL, __meta_funcs);
        lua_newtable(_l);
        luaL_register(_l, NULL, __type_meta_funcs);
        lua_setmetatable(_l,-2);
    lua_rawset(_l,-3); // rawset( ex, "time", tp )
    lua_pop(_l, 1); // pops ex
    
    //
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void luaclose_time () {
    // deinit the hashtable
    ex_hashmap_deinit ( &__key_to_type_meta_getset );
}

