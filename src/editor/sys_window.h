// ======================================================================================
// File         : sys_window.h
// Author       : Wu Jie 
// Last Change  : 05/28/2011 | 12:33:45 PM | Saturday,May
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef SYS_WINDOW_H_1306557226
#define SYS_WINDOW_H_1306557226
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "SDL.h"

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

typedef struct sys_window_t {
    SDL_Window *sdl_win;
    SDL_GLContext gl_context;
    ex_color3f_t bgColor;

    // 
    void (*on_resize) ( int _width, int _height );
    void (*on_update) ();
    void (*on_draw) ( struct sys_window_t *_win );
} sys_window_t;

///////////////////////////////////////////////////////////////////////////////
// function defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_array_t *ex_sys_windows ();

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

sys_window_t *ex_create_sys_window ( const char *_title, 
                                     int _x, 
                                     int _y, 
                                     int _width, 
                                     int _height );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_destroy_sys_window ( sys_window_t *_win );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_sys_window_resize ( sys_window_t *_win, int _width, int _height );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_sys_window_show ( sys_window_t *_win, bool _show );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_sys_window_begin ( sys_window_t *_win );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_sys_window_end ( sys_window_t *_win );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_sys_window_handle_event ( sys_window_t *_win, SDL_Event *_event );


// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END SYS_WINDOW_H_1306557226
// #################################################################################