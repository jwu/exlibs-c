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
int samples[2000];
static int count = 0;
int freq[5];
float pdf[5];
float cdf[5];
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void init () {
    struct lua_State *l = ex_lua_default_state();

    ex_lua_dofile( l, "script_world.lua" );
    {
        float weights[] = { 0.12, 0.4, 0.4, 0.07, 0.9 };
#if 0
        ex_walkers_alias( weights, EX_ARRAY_COUNT(weights), 
                          samples, EX_ARRAY_COUNT(samples) );
#else
        ex_weights_to_cdf( cdf, weights, EX_ARRAY_COUNT(weights) );
#endif

        for ( int i = 0; i < EX_ARRAY_COUNT(freq); ++i ) {
            freq[i] = 0;
            pdf[i] = 0.0;
        }
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void update () {
    if ( count != 0 ) {
        struct lua_State *l = ex_lua_default_state();
#if 0
        int i = (int)floorf( ex_range_randf( 0.0f, 2000.0f) );
        i = samples[i];
#else
        int i = ex_cdf_gen_index( cdf, EX_ARRAY_COUNT(cdf) );
#endif
        pdf[i] = (float)(++freq[i])/(float)count;

        ex_lua_dostring( l, "my_pdf[%d] = %f", i+1, pdf[i] );
    }

    ++count;
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
    g_game.keyboard = keyboard;
    g_game.mouse = NULL;
}
