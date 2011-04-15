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

///////////////////////////////////////////////////////////////////////////////
// world
extern ex_ref_t *g_world;
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void init () {
    struct lua_State *l = ex_lua_main_state();

    // create test world
    ex_lua_dofile( l, "scripted_world/test_transform.lua", NULL );
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
    //
    ex_world_render(g_world);

    // TODO: to test rendering { 
    // struct lua_State *l = ex_lua_main_state();
    // // test render lua
    // ex_camera_apply ( ex_world_main_camera(g_world) );
    // if ( ex_fsys_file_exists( "exec/render_3D.lua" ) )
    //     ex_lua_dofile( l, "exec/render_3D.lua", NULL );

    // TODO { 
    // // draw 2D objects in screen space
    // glMatrixMode( GL_MODELVIEW );
    // glLoadIdentity();
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // glOrtho(0, win_width, win_height, 0, -1.0, 1.0);
    // if ( ex_fsys_file_exists( "exec/render_2D.lua" ) )
    //     ex_lua_dofile( l, "exec/render_2D.lua", NULL );
    // } TODO end 
    // } TODO end 
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
