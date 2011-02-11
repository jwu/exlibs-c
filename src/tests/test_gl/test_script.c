// ======================================================================================
// File         : test_script.c
// Author       : Wu Jie 
// Last Change  : 02/07/2011 | 22:16:20 PM | Monday,February
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// externs
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

#include "../../graphics/graphics_inc.h"
#include "../../engine/engine_inc.h"

#include "main.h"


///////////////////////////////////////////////////////////////////////////////
// world
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void init () {
    struct lua_State *l = ex_lua_default_state();
    ex_lua_dofile( l, "script_world.lua" );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void update () {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void render () {
    struct lua_State *l = ex_lua_default_state();

    //
    // ex_world_render(g_world);

    // test render lua
    ex_camera_apply ( ex_world_main_camera(g_world) );
    if ( ex_fsys_file_exists( "render_3D.lua" ) )
        ex_lua_dofile( l, "render_3D.lua" );

    // TODO { 
    // // draw 2D objects in screen space
    // glMatrixMode( GL_MODELVIEW );
    // glLoadIdentity();
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // glOrtho(0, win_width, win_height, 0, -1.0, 1.0);
    // if ( ex_fsys_file_exists( "render_2D.lua" ) )
    //     ex_lua_dofile( l, "render_2D.lua" );
    // } TODO end 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void keyboard ( uint8 _key ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_script () {
    g_game.init = init;
    g_game.update = update;
    g_game.render = render;
    g_game.keyboard = keyboard;
    g_game.mouse = NULL;
}
