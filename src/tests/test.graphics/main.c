// ======================================================================================
// File         : main.c
// Author       : Wu Jie 
// Last Change  : 06/26/2010 | 09:16:13 AM | Saturday,June
// Description  : 
// ======================================================================================

#include <stdio.h>
#include "SDL/include/SDL.h"
#undef main

#include "../../core/core_inc.h"
#include "../../graphics/graphics_inc.h"

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int process_events () {

    int quit = 0;

    // process event
    SDL_Event event;
    while ( SDL_PollEvent(&event) )
    {
        if ( event.type == SDL_QUIT ) 
        { 
            quit = 1; 
        }
        else if ( event.type == SDL_KEYDOWN )
        {
            switch ( event.key.keysym.sym )
            {
            case SDLK_ESCAPE: 
                {
                    quit = 1;
                }
                break;
            }
        }
    }
    return quit;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void render_scene ()
{
    // TODO
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void main_loop ()
{
    int quit = 0;

    while ( !quit ) {
        // process events 
        quit = process_events();

        // render
        begin_frame();
        render_scene();
        end_frame();
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int main ( int argc, char* argv[] )
{
    printf ( "start test...\n" );

    init_sdl_gl();
    main_loop();

    return 0;
}


