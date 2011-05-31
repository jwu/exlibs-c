// ======================================================================================
// File         : sys_window.c
// Author       : Wu Jie 
// Last Change  : 05/28/2011 | 12:33:57 PM | Saturday,May
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "engine/engine_inc.h"
#include "editor_inc.h"

///////////////////////////////////////////////////////////////////////////////
// internal
///////////////////////////////////////////////////////////////////////////////

static sys_window_t *__main_window = NULL; 
static ex_array_t __windows;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void __init_window_table() {
    ex_array_init ( &__windows, 
                    EX_STRID_NULL,
                    sizeof(sys_window_t *), 
                    8,
                    __ex_array_alloc_nomng,
                    __ex_array_realloc_nomng,
                    __ex_array_dealloc_nomng );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void __deinit_window_table () {
    ex_array_deinit (&__windows);
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_array_t *ex_sys_windows () { return &__windows; }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

sys_window_t *ex_create_sys_window ( const char *_title, 
                                     int _x, 
                                     int _y, 
                                     int _width, 
                                     int _height ) 
{
    sys_window_t *win;
    int flags;

    win = ex_malloc ( sizeof(sys_window_t) );
    win->sdl_win = NULL;
    win->gl_context = NULL;
    win->texture_id = -1;
    win->pbo_ids[0] = -1;
    win->pbo_ids[1] = -1;
    win->stage = NULL;
    win->pbo_cur = 0;
    win->pbo_next = 0;
    win->on_resize = NULL;
    win->on_update = NULL;
    win->on_draw = NULL;

    // if this is the first window, init gl and create sdl
    if ( __main_window == NULL ) {
        int bpp = 32;
        int rgb_size[3];
        int value = -1;
        int fsaa = 4;    // 4x fsaa
        int accel = -1;  // no accel
        int vsync = 0;   // no vsync

        ex_log( "init sdl opengl" );

        /* Request an opengl 3.2 context.
         * SDL doesn't have the ability to choose which profile at this time of writing,
         * but it should default to the core profile */
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

        // setup bits per-pixel
        if ( bpp == 0 ) {
            if ( SDL_GetVideoInfo()->vfmt->BitsPerPixel <= 8 )
                bpp = 8;
            else
                bpp = 16;
        }
        switch (bpp) {
        case 8:
            rgb_size[0] = 3;
            rgb_size[1] = 3;
            rgb_size[2] = 2;
            break;
        case 15:
        case 16:
            rgb_size[0] = 5;
            rgb_size[1] = 5;
            rgb_size[2] = 5;
            break;
        default:
            rgb_size[0] = 8;
            rgb_size[1] = 8;
            rgb_size[2] = 8;
            break;
        }
        SDL_GL_SetAttribute ( SDL_GL_RED_SIZE,      rgb_size[0] );
        SDL_GL_SetAttribute ( SDL_GL_GREEN_SIZE,    rgb_size[1] );
        SDL_GL_SetAttribute ( SDL_GL_BLUE_SIZE,     rgb_size[2] );
        SDL_GL_SetAttribute ( SDL_GL_DEPTH_SIZE,    24 );
        SDL_GL_SetAttribute ( SDL_GL_DOUBLEBUFFER,  1 );

        // setup the opengl device
        if ( fsaa ) {
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, fsaa);
        }
        if ( accel >= 0 ) {
            SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, accel);
        }
        if ( vsync ) {
            SDL_GL_SetSwapInterval(1);
        } else {
            SDL_GL_SetSwapInterval(0);
        }
        // TODO { 
        // if (gamma != 0.0) {
        //     SDL_SetGamma(gamma, gamma, gamma);
        // }
        // } TODO end 

        // Create our window centered at win_width x win_height resolution
        flags = SDL_WINDOW_OPENGL;
        // flags |= SDL_WINDOW_FULLSCREEN; // TODO: full-screen
        flags |= SDL_WINDOW_RESIZABLE;
        flags |= SDL_WINDOW_SHOWN;
        // flags |= SDL_WINDOW_INPUT_GRABBED;
        // flags |= SDL_WINDOW_INPUT_FOCUS;
        // flags |= SDL_WINDOW_MOUSE_FOCUS;
        win->sdl_win = SDL_CreateWindow( _title,
                                         _x, 
                                         _y,
                                         _width, 
                                         _height, 
                                         flags );
        if ( win->sdl_win == NULL ) {
            ex_error ( "unable to create window" );
            ex_free (win);
            return NULL;
        }

        // ======================================================== 
        // log the graphic-driver status
        // ======================================================== 

        ex_log( "Screen BPP\t\t\t\t: %d", SDL_GetWindowSurface(win->sdl_win)->format->BitsPerPixel );
        ex_log( "Vendor\t\t\t\t\t: %s", glGetString(GL_VENDOR) );
        ex_log( "Renderer:\t\t\t\t: %s", glGetString(GL_RENDERER) );
        ex_log( "Version\t\t\t\t\t: %s", glGetString(GL_VERSION) );
        ex_log( "Extensions:\t\t%s", glGetString(GL_EXTENSIONS) );
        ex_log( "" );

        SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &value);
        ex_log( "red size\t\t\t\t: requested %d, got %d", rgb_size[0], value );

        SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &value);
        ex_log( "green size\t\t\t\t: requested %d, got %d", rgb_size[1], value );

        SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &value);
        ex_log( "blue size\t\t\t\t: requested %d, got %d", rgb_size[2], value );

        SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &value);
        ex_log( "depth size\t\t\t\t: requested %d, got %d", 16, value );

        SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &value);
        ex_log( "double buffer\t\t\t: requested 1, got %d", value );

        if (fsaa) {
            SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &value);
            ex_log( "multi-buffers\t\t\t: requested 1, got %d", value );
            SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &value);
            ex_log( "multi-samples\t\t\t: requested %d, got %d", fsaa, value );
        }
        if (accel >= 0) {
            SDL_GL_GetAttribute(SDL_GL_ACCELERATED_VISUAL, &value);
            ex_log( "accelerated visual\t\t: requested %d, got %d", accel, value );
        }
        if (vsync) {
            ex_log( "buffer swap interval\t: requested 1, got %d", SDL_GL_GetSwapInterval() );
        }

        // set main window
        __main_window = win;
    }
    else {
        // create sdl window
        // flags = SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE|SDL_WINDOW_BORDERLESS;
        flags = SDL_WINDOW_RESIZABLE|SDL_WINDOW_BORDERLESS;
        win->sdl_win = SDL_CreateWindow( _title,
                                         _x,
                                         _y,
                                         _width, 
                                         _height, 
                                         flags );
        if ( win->sdl_win == NULL ) {
            ex_error ( "unable to create window" );
            ex_free (win);
            return NULL;
        }
    }

    // create gl context
    win->gl_context = SDL_GL_CreateContext(win->sdl_win);

    // create texture
    glGenTextures ( 1, &win->texture_id );
    glEnable ( GL_TEXTURE_RECTANGLE_ARB );
    glBindTexture ( GL_TEXTURE_RECTANGLE_ARB, win->texture_id );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexImage2D ( GL_TEXTURE_RECTANGLE_ARB,
                   0,
                   GL_RGBA8,
                   _width,
                   _height,
                   0,
                   GL_BGRA,
                   GL_UNSIGNED_INT_8_8_8_8_REV,
                   NULL );
    glBindTexture ( GL_TEXTURE_RECTANGLE_ARB, 0 );
    glDisable ( GL_TEXTURE_RECTANGLE_ARB );

    // create 2 pbo  
    glGenBuffersARB(2, win->pbo_ids);
    glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, win->pbo_ids[0]);
    glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, _width * _height * 4, 0, GL_STREAM_DRAW_ARB);
    glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, win->pbo_ids[1]);
    glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, _width * _height * 4, 0, GL_STREAM_DRAW_ARB);
    glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);

    // create stage
    win->stage = ex_create_stage( _width, _height );

    // 
    ex_array_append( &__windows, &win );
    return win;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_destroy_sys_window ( sys_window_t *_win ) {
    // destory stage
    if ( _win->stage ) {
        ex_destroy_stage(_win->stage);
        _win->stage = NULL;
    }

    // destroy texture
    glDeleteTextures( 1, &_win->texture_id );
    _win->texture_id = -1;

    // destroy PBOs
    glDeleteBuffersARB( 2, _win->pbo_ids );
    _win->pbo_ids[0] = -1;
    _win->pbo_ids[1] = -1;

    // destroy sdl gl context
    if ( _win->gl_context ) {
        SDL_GL_DeleteContext(_win->gl_context);
        _win->gl_context = NULL;
    }

    // destroy sdl windows
    if ( _win->sdl_win ) {
        SDL_DestroyWindow(_win->sdl_win);
        _win->sdl_win = NULL;
    }

    //
    ex_array_each ( &__windows, sys_window_t *, w ) {
        if ( w == _win ) {
            ex_array_remove_at_fast ( &__windows, __idx__ );
            break;
        }
    } ex_array_each_end
    ex_free(_win);

    // NOTE: looks like the resource are shared
    ex_array_each ( &__windows, sys_window_t *, win ) {
        int w, h;

        //
        SDL_GetWindowSize ( win->sdl_win, &w, &h );
        SDL_GL_MakeCurrent( win->sdl_win, win->gl_context );

        //
        glDeleteTextures (1, &win->texture_id);
        glGenTextures ( 1, &win->texture_id );
        glBindTexture ( GL_TEXTURE_RECTANGLE_ARB, win->texture_id );
        glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP );
        glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP );
        glTexImage2D ( GL_TEXTURE_RECTANGLE_ARB,
                       0,
                       GL_RGBA8,
                       w,
                       h,
                       0,
                       GL_BGRA,
                       GL_UNSIGNED_INT_8_8_8_8_REV,
                       NULL );
        glBindTexture ( GL_TEXTURE_RECTANGLE_ARB, 0 );

        //
        glDeleteBuffersARB(2, win->pbo_ids);
        glGenBuffersARB(2, win->pbo_ids);
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, win->pbo_ids[0]);
        glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, w * h * 4, 0, GL_STREAM_DRAW_ARB);
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, win->pbo_ids[1]);
        glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, w * h * 4, 0, GL_STREAM_DRAW_ARB);
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
    } ex_array_each_end
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_sys_window_resize ( sys_window_t *_win, int _width, int _height ) {
    int status;

    // switch context
    status = SDL_GL_MakeCurrent( _win->sdl_win, _win->gl_context );
    if ( status ) {
        ex_error( "Can't make current gl context: %s", SDL_GetError() );
        return;
    }

    // resize texture
    glDeleteTextures (1, &_win->texture_id);
    glGenTextures ( 1, &_win->texture_id );
    glBindTexture ( GL_TEXTURE_RECTANGLE_ARB, _win->texture_id );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexImage2D ( GL_TEXTURE_RECTANGLE_ARB,
                   0,
                   GL_RGBA8,
                   _width,
                   _height,
                   0,
                   GL_BGRA,
                   GL_UNSIGNED_INT_8_8_8_8_REV,
                   NULL );
    glBindTexture ( GL_TEXTURE_RECTANGLE_ARB, 0 );

    // resize PBOs
    glDeleteBuffersARB(2, _win->pbo_ids);
    glGenBuffersARB(2, _win->pbo_ids);
    glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, _win->pbo_ids[0]);
    glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, _width * _height * 4, 0, GL_STREAM_DRAW_ARB);
    glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, _win->pbo_ids[1]);
    glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, _width * _height * 4, 0, GL_STREAM_DRAW_ARB);
    glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);

    // resize stage
    ex_stage_resize ( _win->stage, _width, _height );

    // resize window
    SDL_SetWindowSize ( _win->sdl_win, _width, _height );

    // trigger on_resize event
    if ( _win->on_resize )
        _win->on_resize ( _width, _height );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_sys_window_show ( sys_window_t *_win, bool _show ) {
    if ( _show )
        SDL_ShowWindow ( _win->sdl_win );
    else 
        SDL_HideWindow ( _win->sdl_win );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_sys_window_begin ( sys_window_t *_win ) {
    int status;
    int w, h;

    // make current gl context
    SDL_GetWindowSize ( _win->sdl_win, &w, &h );
    status = SDL_GL_MakeCurrent( _win->sdl_win, _win->gl_context );
    if ( status ) {
        ex_error( "Can't make current gl context: %s", SDL_GetError() );
        return;
    }

    // setup view port
    glViewport( 0, 0, w, h );

    // setup view,projection matix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

    // clear buffer
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

#if 1
    _win->pbo_cur = (_win->pbo_cur + 1) % 2;
    _win->pbo_next = (_win->pbo_cur + 1) % 2;

    // ======================================================== 
    // buffer 1 
    // ======================================================== 

    // bind the texture and PBO
    glBindTexture ( GL_TEXTURE_RECTANGLE_ARB, _win->texture_id );
    glBindBufferARB ( GL_PIXEL_UNPACK_BUFFER_ARB, _win->pbo_ids[_win->pbo_cur] );

    // copy pixels from PBO to texture object
    // Use offset instead of ponter.
    glTexSubImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, w, h, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, 0 );

    // ======================================================== 
    // buffer 2 
    // ======================================================== 

    // bind PBO to update pixel values
    glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, _win->pbo_ids[_win->pbo_next]);
    glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, w * h * 4, 0, GL_STREAM_DRAW_ARB);
    uint8 *ptr = glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);
    if ( ptr ) {
        cairo_t *cr;
        cairo_surface_t *surf;
        surf = cairo_image_surface_create_for_data ( ptr,
                                                     CAIRO_FORMAT_ARGB32,
                                                     w,
                                                     h,
                                                     4 * w );
        if ( cairo_surface_status (surf) != CAIRO_STATUS_SUCCESS ) {
            ex_error ("can't create cairo surface.");
            return;
        }

        // create cairo context
        cairo_destroy (_win->stage->cr);
        cr = cairo_create (surf);
        cairo_surface_destroy (surf);
        if ( cairo_status (cr) != CAIRO_STATUS_SUCCESS ) {
            ex_error ("can't create cairo context");
            return;
        }

        //
        _win->stage->cr = cr;
        _win->stage->buffer = ptr;

    }
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_sys_window_end ( sys_window_t *_win ) {
    int w, h;

    //
    SDL_GetWindowSize ( _win->sdl_win, &w, &h );

    // setup view,projection matix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

#if 0
    //
    glBindTexture ( GL_TEXTURE_RECTANGLE_ARB, _win->texture_id );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_STORAGE_HINT_APPLE, GL_STORAGE_SHARED_APPLE );
    // glTexImage2D ( GL_TEXTURE_RECTANGLE_ARB,
    //                0,
    //                GL_RGBA8,
    //                w,
    //                h,
    //                0,
    //                GL_BGRA,
    //                GL_UNSIGNED_INT_8_8_8_8_REV,
    //                _win->stage->buffer );
    glTexSubImage2D ( GL_TEXTURE_RECTANGLE_ARB,
                      0,
                      0, // x
                      0, // y
                      w, // width
                      h, // height
                      GL_BGRA,
                      GL_UNSIGNED_INT_8_8_8_8_REV,
                      _win->stage->buffer );

    glEnable (GL_BLEND);
    glEnable (GL_TEXTURE_RECTANGLE_ARB);

    glBlendFunc (GL_SRC_ALPHA, GL_ONE);
    glColor3f ( 1.0f, 1.0f, 1.0f );

    glBegin (GL_QUADS);
    glTexCoord2f ( 0.0f, 0.0f );
    glVertex2f ( 0.0f, 0.0f );

    glTexCoord2f ( (GLfloat)w, 0.0f );
    glVertex2f ( 1.0f, 0.0f );

    glTexCoord2f ( (GLfloat)w, (GLfloat)h );
    glVertex2f ( 1.0f, 1.0f );

    glTexCoord2f ( 0.0f, (GLfloat)h );
    glVertex2f ( 0.0f, 1.0f );
    glEnd ();

    glDisable (GL_TEXTURE_RECTANGLE_ARB);
    glBindTexture (GL_TEXTURE_RECTANGLE_ARB, 0);
#else
    if ( _win->stage->buffer ) {
        glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB); // release pointer to mapping buffer
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
        _win->stage->buffer = NULL;
    }

    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glBindTexture ( GL_TEXTURE_RECTANGLE_ARB, _win->texture_id );

    glEnable (GL_BLEND);
    glEnable (GL_TEXTURE_RECTANGLE_ARB);

    glBlendFunc (GL_SRC_ALPHA, GL_ONE);
    glColor3f ( 1.0f, 1.0f, 1.0f );

    glBegin (GL_QUADS);
    glTexCoord2f ( 0.0f, 0.0f );
    glVertex2f ( 0.0f, 0.0f );

    glTexCoord2f ( (GLfloat)w, 0.0f );
    glVertex2f ( 1.0f, 0.0f );

    glTexCoord2f ( (GLfloat)w, (GLfloat)h );
    glVertex2f ( 1.0f, 1.0f );

    glTexCoord2f ( 0.0f, (GLfloat)h );
    glVertex2f ( 0.0f, 1.0f );
    glEnd ();

    glDisable (GL_TEXTURE_RECTANGLE_ARB);
    glBindTexture (GL_TEXTURE_RECTANGLE_ARB, 0);
#endif

    // swap buffer
    SDL_GL_SwapWindow(_win->sdl_win);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_sys_window_handle_event ( sys_window_t *_win, SDL_Event *_event ) {
    int done;

    done = 0;
    switch ( _event->type ) {
        //
    case SDL_WINDOWEVENT:
        switch (_event->window.event) {
        case SDL_WINDOWEVENT_RESIZED:
            ex_sys_window_resize ( _win, _event->window.data1, _event->window.data2 );
            break;

        // case SDL_WINDOWEVENT_CLOSE:
        //     if ( _event->window.windowID != SDL_GetWindowID(glWindow) ) {
        //         SDL_Window *window = SDL_GetWindowFromID(_event->window.windowID);
        //         if ( window ) {
        //             SDL_DestroyWindow(window);
        //         }
        //     }
        //     break;

        // case SDL_WINDOWEVENT_FOCUS_GAINED:
        //     if ( g_world ) {
        //         ex_world_resume(g_world);
        //     }
        //     break;

        // case SDL_WINDOWEVENT_FOCUS_LOST:
        //     if ( g_world ) {
        //         ex_world_pause(g_world);
        //     }
        //     break;
        }
        break;

        //
    case SDL_KEYDOWN:
        switch ( _event->key.keysym.sym ) {
        case SDLK_ESCAPE:
            if ( __main_window == _win ) {
                done = 1;
            }
            else {
                ex_destroy_sys_window(_win);
            }
            break;
        }
        break;

        //
    case SDL_QUIT:
        done = 1;
        break;
    }
    return (done);
}
