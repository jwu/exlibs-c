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

// sdl
int win_width = 800;
int win_height = 600;
int bpp = 32;
uint32 window_flags = 0;
SDL_Window *glWindow = NULL; 
SDL_GLContext *glContext = NULL; 

// game
ex_ref_t *g_world = NULL;
char *g_world_path = NULL;

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __init_window () {
    int rgb_size[3];
    int value = -1;
    int fsaa = 4;       // 4x fsaa
    int accel = -1;     // no accel
    int vsync = 0;       // no vsync

    ex_log( "init sdl opengl" );

    /* Request an opengl 3.2 context.
     * SDL doesn't have the ability to choose which profile at this time of writing,
     * but it should default to the core profile */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // setup bits per-pixel
    if (bpp == 0) {
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
    window_flags = SDL_WINDOW_OPENGL;
    // window_flags |= SDL_WINDOW_FULLSCREEN; // TODO: full-screen
    window_flags |= SDL_WINDOW_RESIZABLE;
    window_flags |= SDL_WINDOW_SHOWN;
    // window_flags |= SDL_WINDOW_INPUT_GRABBED;
    // window_flags |= SDL_WINDOW_INPUT_FOCUS;
    // window_flags |= SDL_WINDOW_MOUSE_FOCUS;
    glWindow = SDL_CreateWindow( "exsdk Editor", 
                                 SDL_WINDOWPOS_CENTERED, 
                                 SDL_WINDOWPOS_CENTERED,
                                 win_width, win_height, 
                                 window_flags );
    if ( glWindow == NULL ) {
        ex_error ( "unable to create window" );
    }
    glContext = SDL_GL_CreateContext(glWindow);

    // log the graphic-driver status
    ex_log( "Screen BPP\t\t\t\t: %d", SDL_GetWindowSurface(glWindow)->format->BitsPerPixel );
    ex_log( "Vendor\t\t\t\t\t: %s", glGetString(GL_VENDOR) );
    ex_log( "Renderer:\t\t\t\t: %s", glGetString(GL_RENDERER) );
    ex_log( "Version\t\t\t\t\t: %s", glGetString(GL_VERSION) );
    // ex_log( "Extensions:\t\t%s", glGetString(GL_EXTENSIONS) );

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
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __init_gl () {
    glViewport( 0, 0, win_width, win_height );
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

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

    glShadeModel(GL_SMOOTH);

    //
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
}

// ------------------------------------------------------------------ 
// Desc: 
SDL_Window *ip_win = NULL;
uint texture_id = -1;
cairo_surface_t *cairo_surf = NULL;
uint8 *cairo_buffer = NULL;
cairo_t *cr = NULL;
// ------------------------------------------------------------------ 

//
static void __init_inspector () {
    int x, y, w, h;

    // create sdl window
    SDL_GetWindowPosition ( glWindow, &x, &y );
    SDL_GetWindowSize ( glWindow, &w, &h );
    ip_win = SDL_CreateWindow( "Inspector", 
                               x + win_width + 10, 
                               y,
                               200, h, 
                               SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE|SDL_WINDOW_SHOWN|SDL_WINDOW_BORDERLESS );

    // create cairo surface
    SDL_GetWindowSize ( ip_win, &w, &h );
	cairo_buffer = ex_malloc ( 4 * w * h * sizeof (uint8) );
    ex_memzero( cairo_buffer,  4 * w * h * sizeof (uint8) );
    cairo_surf = cairo_image_surface_create_for_data ( cairo_buffer,
                                                       CAIRO_FORMAT_ARGB32,
                                                       w,
                                                       h,
                                                       4 * w );
    if ( cairo_surface_status (cairo_surf) != CAIRO_STATUS_SUCCESS ) {
        ex_free (cairo_buffer);
        cairo_buffer = NULL;
        ex_error ("can't create cairo surface.");
    }

    // create cairo context
    cr = cairo_create (cairo_surf);
    if ( cairo_status (cr) != CAIRO_STATUS_SUCCESS ) {
        ex_free (cairo_buffer);
        cairo_buffer = NULL;
        ex_error ("can't create cairo context");
    }

    // create texture
	glDeleteTextures ( 1, &texture_id );
    glGenTextures ( 1, &texture_id );
    glBindTexture ( GL_TEXTURE_RECTANGLE_ARB, texture_id );
    glTexImage2D ( GL_TEXTURE_RECTANGLE_ARB,
                   0,
                   GL_RGBA,
                   w,
                   h,
                   0,
                   GL_BGRA,
                   GL_UNSIGNED_BYTE,
                   NULL );
    // glTexEnvi ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
}

//
static void __draw_inspector () {
    int w, h;
    SDL_GetWindowSize ( ip_win, &w, &h );

    if ( cr == NULL )
        return;

    // clear background
    cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
    cairo_rectangle (cr, 0, 0, w, h);
    cairo_set_source_rgba (cr, 0.0f, 0.0f, 0.0f, 1.0f);
    cairo_fill (cr);
    cairo_stroke (cr);

    cairo_save (cr);
#if 0
    cairo_set_line_width (cr, 2.0);
    cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
    cairo_rectangle (cr, 0.25 * w, 0.25 * h, 0.5 * w, 0.5 * h);
    cairo_stroke (cr);
#else
    //
    cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
    cairo_move_to (cr, 0, 0);
    cairo_line_to (cr, 1 * w, 1 * h);
    cairo_move_to (cr, 1 * w, 0);
    cairo_line_to (cr, 0, 1 * h);
    cairo_set_line_width (cr, 2.0);
    cairo_stroke (cr);

    cairo_rectangle (cr, 0, 0, 0.5 * w, 0.5 * h);
    cairo_set_source_rgba (cr, 1, 0, 0, 0.80);
    cairo_fill (cr);

    cairo_rectangle (cr, 0, 0.5 * h, 0.5 * w, 0.5 * h);
    cairo_set_source_rgba (cr, 0, 1, 0, 0.60);
    cairo_fill (cr);

    cairo_rectangle (cr, 0.5 * w, 0, 0.5 * w, 0.5 * h);
    cairo_set_source_rgba (cr, 0, 0, 1, 0.40);
    cairo_fill (cr);
    cairo_stroke (cr);
#endif
    cairo_restore (cr);
}

//
static void __destroy_inspector () {
    glDeleteTextures (1, &texture_id);
    
    // remove surface
    // NOTE: we still need cairo_surf when resizing.
    if ( cairo_surf )
        cairo_surface_destroy (cairo_surf);

    // destroy cairo
    if ( cr )
        cairo_destroy (cr);

    //
    if ( cairo_buffer ) {
        ex_free (cairo_buffer);
        cairo_buffer = NULL;
    }
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
    ex_camera_set_aspect( mainCam, (float)win_width/(float)win_height );
    ex_camera_set_ortho_size( mainCam, (float)win_height/2.0f );

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

static int __gl_window_handle_event ( SDL_Event *_event ) {
    int done;

    done = 0;
    switch ( _event->type ) {
        //
    case SDL_WINDOWEVENT:
        switch (_event->window.event) {
        case SDL_WINDOWEVENT_RESIZED:
            if (_event->window.windowID == SDL_GetWindowID(glWindow)) {
                int status;

                win_width = _event->window.data1;
                win_height = _event->window.data2;
                status = SDL_GL_MakeCurrent(glWindow,glContext);
                if ( status ) {
                    ex_error( "Can't make current gl context: %s", SDL_GetError() );
                    break;
                }
                if ( g_world ) {
                    ex_ref_t *mainCam;
                    mainCam = ex_world_main_camera (g_world);
                    ex_camera_set_ortho( mainCam, true );
                    ex_camera_set_aspect( mainCam, (float)win_width/(float)win_height );
                    ex_camera_set_ortho_size( mainCam, (float)win_height/2.0f );
                }
                __init_gl();
                ex_log( "glWindow resize to %d x %d", win_width, win_height );
            }
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
            done = 1;
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

// ------------------------------------------------------------------ 
// Desc: 
extern int __sdl_window_handle_event ( SDL_Event *_event );
// ------------------------------------------------------------------ 

static int __handle_event ( SDL_Event *_event ) {
    SDL_Window *sdlWindow = SDL_GetWindowFromID (_event->window.windowID);
    if ( sdlWindow == glWindow ) {
        return __gl_window_handle_event (_event);
    }
    else {
        return __sdl_window_handle_event (_event);
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __main_loop () {
    int done = 0;
    int w, h;

    // run the world
    ex_world_run(g_world);

    // looping the game
    while ( !done ) {
        SDL_Event event;
        int status;

        // handle event
        while ( SDL_PollEvent(&event) ) {
            done |= __handle_event(&event);
        }

        // ======================================================== 
        // draw inspector 
        // ======================================================== 

        __draw_inspector();
        SDL_GetWindowSize ( ip_win, &w, &h );
        status = SDL_GL_MakeCurrent( ip_win, glContext );
        if ( status ) {
            ex_error( "Can't make current gl context: %s", SDL_GetError() );
            break;
        }

        glViewport( 0, 0, w, h );
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho (0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
        // ex_glOrtho(-rx, rx, -ry, ry, -100.0, 100.0); // this will make camera look along -z
        // ex_glTranslate(0.5, 0.5, 0.0);
        glMatrixMode (GL_MODELVIEW);
        glLoadIdentity ();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear( GL_COLOR_BUFFER_BIT );
        glBindTexture (GL_TEXTURE_RECTANGLE_ARB, texture_id);
        glTexImage2D ( GL_TEXTURE_RECTANGLE_ARB,
                       0,
                       GL_RGBA,
                       w,
                       h,
                       0,
                       GL_BGRA,
                       GL_UNSIGNED_BYTE,
                       cairo_buffer );

        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable (GL_TEXTURE_RECTANGLE_ARB);
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
        SDL_GL_SwapWindow(ip_win);


        // ======================================================== 
        // update and render the world
        // ======================================================== 

        status = SDL_GL_MakeCurrent( glWindow, glContext );
        if ( status ) {
            ex_error( "Can't make current gl context: %s", SDL_GetError() );
            break;
        }
        if ( g_world ) {
            ex_world_update(g_world);

            //
            glViewport( 0, 0, win_width, win_height );
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
            ex_world_render(g_world);
        }
        SDL_GL_SwapWindow(glWindow);
    }

    // Destroy inspector
    __destroy_inspector ();

    // Destroy our GL context, etc.
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(glWindow);
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

        // init main gl window
        __init_window ();
        __init_gl ();

        // init other window
        __init_inspector ();

        // ======================================================== 
        // init the editor
        // ======================================================== 

        ex_editor_init();
        ex_lua_load_modules( ex_lua_main_state(), "builtin" );
        ex_lua_load_modules( ex_lua_main_state(), "scripts" );
        __init_game ();

        // ======================================================== 
        // enter the mainloop
        // ======================================================== 

        __main_loop ();

        return 0;
    }

    return -1;
}
