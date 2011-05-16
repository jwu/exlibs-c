// ======================================================================================
// File         : inspector.c
// Author       : Wu Jie 
// Last Change  : 05/16/2011 | 09:26:47 AM | Monday,May
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "engine/engine_inc.h"
#include "editor_inc.h"

// ------------------------------------------------------------------ 
// Desc: 
extern SDL_Surface *ip_surf;
extern SDL_Window *ip_win;
// ------------------------------------------------------------------ 

int __sdl_window_handle_event ( SDL_Event *_event ) {
    SDL_Window *window = SDL_GetWindowFromID(_event->window.windowID);

    switch ( _event->type ) {
        //
    case SDL_WINDOWEVENT:
        switch (_event->window.event) {
        case SDL_WINDOWEVENT_RESIZED:
            SDL_SetWindowSize ( window, _event->window.data1, _event->window.data2 );
            ip_surf = SDL_GetWindowSurface(ip_win);
            ex_log("window resized to %d, %d", _event->window.data1, _event->window.data2);
            break;

        case SDL_WINDOWEVENT_CLOSE:
            if ( window )
                SDL_DestroyWindow(window);
            break;
        }
        break;

        //
    case SDL_KEYDOWN:
        switch ( _event->key.keysym.sym ) {
        case SDLK_ESCAPE:
            SDL_DestroyWindow(window);
            break;
        }
        break;
    }
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

