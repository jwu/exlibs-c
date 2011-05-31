// ======================================================================================
// File         : main.c
// Author       : Wu Jie 
// Last Change  : 05/08/2011 | 12:09:52 PM | Sunday,May
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "engine/engine_inc.h"
#include "editor_inc.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// window
sys_window_t *main_view = NULL;
sys_window_t *inspector = NULL;

// game
ex_ref_t *g_world = NULL;
char *g_world_path = NULL;

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __reset_gl () {
    // glShadeModel(GL_SMOOTH);
    glShadeModel(GL_FLAT);                      // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

    // anti-aliasing { 
    // glEnable(GL_LINE_SMOOTH);
    // glEnable(GL_POINT_SMOOTH);

    // glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    // glHint(GL_POINT_SMOOTH_HINT, GL_DONT_CARE);
    // } anti-aliasing end 

    // enable blend { 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // } enable blend end 

    // enable depath { 
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(true);
    // } enable depath end 

    //
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __resize_world ( int _width, int _height ) {
    if ( g_world ) {
        ex_ref_t *mainCam;

        mainCam = ex_world_main_camera (g_world);
        ex_camera_set_ortho( mainCam, true );
        ex_camera_set_aspect( mainCam, (float)_width/(float)_height );
        ex_camera_set_ortho_size( mainCam, (float)_height/2.0f );
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __update_world () {
    if ( g_world ) {
        ex_world_update(g_world);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
float fps[1024];
int fps_idx = 0;
// ------------------------------------------------------------------ 

static void __draw_world ( sys_window_t *_win ) {
    if ( g_world ) {
        int w,h;
        double rx, ry;

        SDL_GetWindowSize( _win->sdl_win, &w, &h );
        rx = (double)w/2.0;
        ry = (double)h/2.0;

        __reset_gl();

        // clear backbuffer
        // glClearColor( 0.0f, 0.5f, 1.0f, 1.0f );
        glClearColor( 0.4f, 0.4f, 0.4f, 1.0f );
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        // set modelview, projection matrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-rx, rx, -ry, ry, -100.0, 100.0); // this will make camera look along -z
        glTranslatef(0.5f, 0.5f, 0.0f);

        //
        ex_world_render(g_world);
    }

    // TEMP { 
    int w, h;
    cairo_t *cr;
    cr = _win->stage->cr;
    SDL_GetWindowSize ( _win->sdl_win, &w, &h );

    if ( cr == NULL )
        return;

    // clear background
    cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
    cairo_paint (cr);
    cairo_set_operator (cr, CAIRO_OPERATOR_OVER);

    // background color
    cairo_set_source_rgba (cr, 1.0f, 0.0f, 0.0f, 0.2f);
    cairo_rectangle (cr, 0, 0, w, h);
    cairo_fill (cr);

    cairo_save (cr); {

        fps[fps_idx] = ex_fps();
        fps_idx = (fps_idx + 1)%1024;

        // cairo_set_source_rgba (cr, 0.0, 0.0, 1.0, 0.5);
        // cairo_rectangle (cr, 0.25 * w, 0.25 * h, 0.5 * w, 0.5 * h);
        // cairo_fill (cr);

        // cairo_set_line_width (cr, 2.0);
        // cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 1.0);
        // cairo_rectangle (cr, 0.25 * w, 0.25 * h, 0.5 * w, 0.5 * h);
        // cairo_stroke (cr);

        double x = 10.0;
        double y = h * 0.5;
        cairo_move_to (cr, x, y );
        cairo_new_path (cr);
        cairo_set_line_width (cr, 1.0);
        cairo_set_source_rgba (cr, 1.0, 1.0, 0.0, 1.0);

        int i = 0;
        while ( i < 1024 ) {
            int idx = (fps_idx + i)%1024;
            cairo_line_to (cr, x + i * 0.5, y - fps[idx] * 0.5 );
            ++i;
        }
        cairo_stroke (cr);

        char buf[80];
        cairo_text_extents_t extents;
        snprintf ( buf, sizeof (buf), "fps: %f", ex_fps() );
        cairo_text_extents ( cr, buf, &extents );
        cairo_move_to (cr, 10.0, 10.0);
        cairo_show_text (cr, buf);

    } cairo_restore (cr);
    // } TEMP end 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __draw_inspector ( sys_window_t *_win ) {
    int w, h;
    cairo_t *cr;

    cr = _win->stage->cr;
    SDL_GetWindowSize ( _win->sdl_win, &w, &h );

    if ( cr == NULL )
        return;

    // clear background
    cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
    cairo_paint (cr);
    cairo_set_operator (cr, CAIRO_OPERATOR_OVER);

    // background color
    cairo_set_source_rgba (cr, 0.5f, 0.5f, 0.5f, 1.0f);
    cairo_rectangle (cr, 0, 0, w, h);
    cairo_fill (cr);

    //
    cairo_save (cr); {
        cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
        cairo_move_to (cr, 0, 0);
        cairo_line_to (cr, 1 * w, 1 * h);
        cairo_move_to (cr, 1 * w, 0);
        cairo_line_to (cr, 0, 1 * h);
        cairo_set_line_width (cr, 2.0);
        cairo_stroke (cr);

        cairo_rectangle (cr, 0, 0, 0.5 * w, 0.5 * h);
        cairo_set_source_rgba (cr, 1, 0, 0, 0.80 * fabsf(sinf( ex_time() )) );
        cairo_fill (cr);

        cairo_rectangle (cr, 0, 0.5 * h, 0.5 * w, 0.5 * h);
        cairo_set_source_rgba (cr, 0, 1, 0, 0.60);
        cairo_fill (cr);

        cairo_rectangle (cr, 0.5 * w, 0, 0.5 * w, 0.5 * h);
        cairo_set_source_rgba (cr, 0, 0, 1, 0.40);
        cairo_fill (cr);
        cairo_stroke (cr);
    } cairo_restore (cr);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __init_window () {
    int x, y, w, h;

    // main view
    main_view = ex_create_sys_window ( "exsdk Editor",
                                       SDL_WINDOWPOS_CENTERED, 
                                       SDL_WINDOWPOS_CENTERED,
                                       800, 600 );
    main_view->on_update = __update_world; 
    main_view->on_draw = __draw_world;
    main_view->on_resize = __resize_world;

    // inspector
    SDL_GetWindowPosition ( main_view->sdl_win, &x, &y );
    SDL_GetWindowSize ( main_view->sdl_win, &w, &h );
    inspector = ex_create_sys_window ( "Inspector",
                                       x + w + 10, 
                                       y,
                                       200, 
                                       h );
    inspector->on_draw = __draw_inspector;
    // inspector->on_draw = __draw_world;
    // inspector->on_resize = __resize_world;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __init_game () {
    ex_ref_t *mainCam;
    struct lua_State *l = ex_lua_main_state();

    // load/setup the world
    g_world = ex_create_object( EX_RTTI(ex_world_t), ex_generate_uid() );
    ex_incref(g_world);
    EX_REF_CAST(ex_object_t,g_world)->init(g_world);

    // create main camera
    ex_log ("create main camera...");
    ex_world_create_camera2d ( g_world, ex_strid("main_camera") );
    mainCam = ex_world_main_camera (g_world);
    ex_assert ( mainCam, "can't find main camera" );
    ex_camera_set_ortho( mainCam, true );
    ex_camera_set_aspect( mainCam, 800.0f/600.0f );
    ex_camera_set_ortho_size( mainCam, 600.0f/2.0f );

    // create test world
    g_world_path = "scripted_world/test_invoke.lua";
    // g_world_path = "scripted_world/test_coroutine.lua";
    ex_assert_return ( g_world_path, /*void*/, "can't find a world! please specific it by --world=" );
    ex_lua_dofile( l, g_world_path, NULL );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __quit_game () {
    if ( g_world ) {
        ex_world_stop(g_world);
        ex_world_clear(g_world);
        ex_decref(g_world);
        ex_destroy_object_immediately(g_world, true);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __parse_world ( int *_argc_p, char ***_argv_p ) {
    int argc = *_argc_p;
    char **argv = *_argv_p;
    int i, e;

    for ( i = 1; i < argc; ++i ) {
        if ( strcmp ( "--world", argv[i] ) == 0 ||
             strncmp ( "--world=", argv[i], 8 ) == 0 ) 
        {
            char *equal = argv[i] + 7;

            if ( *equal == '=' ) {
                int slen = strlen(equal+1);
                g_world_path = malloc ( slen + 1 );
                strcpy ( g_world_path, equal+1 );
            }
            else if ( i + 1 < argc ) {
                g_world_path = malloc ( strlen(argv[i+1]) + 1 );
                strcpy ( g_world_path, argv[i+1] ); 

                argv[i] = NULL;
                i += 1;
            }
            argv[i] = NULL;
        }
    }

    // reset the argv and argc
    e = 0;
    for (i = 1; i < argc; i++) {
        if (e) {
            if (argv[i]) {
                argv[e++] = argv[i];
                argv[i] = NULL;
            }
        }
        else if (!argv[i])
            e = i;
    }
    if (e)
        *_argc_p = e;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __main_loop () {
    int done = 0;
    ex_array_t *windows;
    SDL_Event event;
    SDL_Window *sdlWindow;
    int frames = 0;

    // run the world
    ex_world_run(g_world);

    // looping the game
    while ( !done ) {
        windows = ex_sys_windows();

        // handle events
        while ( SDL_PollEvent(&event) ) {
            sdlWindow = SDL_GetWindowFromID (event.window.windowID);
            ex_array_each ( windows, sys_window_t *, w ) {
                if ( w->sdl_win == sdlWindow ) {
                    done |= ex_sys_window_handle_event( w, &event );
                    break;
                }
            } ex_array_each_end
        }

        // update windows
        ex_array_each ( windows, sys_window_t *, w ) {

            if ( w != main_view && (frames % 5 != 0) )
                ex_array_continue;

            if ( w->on_update )
                w->on_update();

            ex_sys_window_begin (w);
            if ( w->on_draw )
                w->on_draw(w);
            ex_sys_window_end (w);

        } ex_array_each_end

        ++frames;
    }

    // destroy windows
    ex_array_each ( windows, sys_window_t *, w ) {
        ex_destroy_sys_window (w);
        --__idx__;
    } ex_array_each_end

    SDL_Quit();
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void exit_fn (void) {

    __quit_game();

    // deinit
    ex_lua_deinit();
    ex_editor_deinit();
    ex_core_deinit();

    printf ("================\n");
    printf ("exit exsdk-editor\n");
    printf ("================\n");
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int main( int argc, char *argv[] ) {

    printf ("================\n");
    printf ("start exsdk-editor\n");
    printf ("================\n");

    // setup editor
    ex_set_editor (true);
    ex_set_edit_mode (true);

    // init
    if ( ex_core_init( &argc, &argv ) != -1 ) {
        __parse_world ( &argc, &argv );

        // register exit function
        atexit(exit_fn);

        // ======================================================== 
        // create window
        // ======================================================== 

        // init sdl video and audio
        if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
            ex_error( "Couldn't initialize SDL: %s", SDL_GetError() );
            exit(1);
        }

        // ======================================================== 
        // init editor 
        // ======================================================== 

        ex_editor_init();
        __init_window ();

        // ======================================================== 
        // init engine
        // ======================================================== 

        ex_lua_load_modules( ex_lua_main_state(), "builtin" );
        ex_lua_load_modules( ex_lua_main_state(), "scripts" );

        // ======================================================== 
        // init game 
        // ======================================================== 

        __init_game ();

        // ======================================================== 
        // enter the mainloop
        // ======================================================== 

        __main_loop ();

        return 0;
    }

    return -1;
}
