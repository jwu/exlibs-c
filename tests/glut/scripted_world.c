// ======================================================================================
// File         : scripted_world.c
// Author       : Wu Jie 
// Last Change  : 04/15/2011 | 13:19:46 PM | Friday,April
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// externs
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

#include "graphics/graphics_inc.h"
#include "engine/engine_inc.h"

#include "simple_behavior.h"

///////////////////////////////////////////////////////////////////////////////
// world
extern ex_ref_t *g_world;
extern char *g_world_path;
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void init () {
    struct lua_State *l = ex_lua_main_state();

    // EX_REGISTER_CLASS_2( ex_simple_t, ex.simple );

    // create test world
    // ex_lua_dofile( l, "scripted_world/test_transform.lua", NULL );
    ex_assert_return ( g_world_path, /*void*/, "can't find a world! please specific it by --world=" );
    ex_lua_dofile( l, g_world_path, NULL );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void deinit () {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void update () {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void render () {
    ex_world_render(g_world);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void on_keyboard ( uint8 _key ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void on_mouse ( int _x, int _y ) {
}
