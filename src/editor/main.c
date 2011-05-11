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

// system
float win_width = 0.0f;
float win_height = 0.0f;
float view_width = 640.0f;
float view_height = 480.0f;

// game
ex_ref_t *g_world = NULL;
char *g_world_path = NULL;

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

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
    ex_camera_set_aspect( mainCam, view_width/view_height );
    ex_camera_set_ortho_size( mainCam, view_height/2.0f );

    // create test world
    ex_assert_return ( g_world_path, /*void*/, "can't find a world! please specific it by --world=" );
    ex_lua_dofile( l, g_world_path, NULL );

    // run the world
    // ex_world_run(g_world);
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
        // create view-2D
        // ======================================================== 

        // TODO:

        // ======================================================== 
        // init the editor
        // ======================================================== 

        //
        ex_editor_init();
        ex_lua_load_modules( ex_lua_main_state(), "builtin" );
        ex_lua_load_modules( ex_lua_main_state(), "scripts" );
        __init_game ();

        // ======================================================== 
        // enter the mainloop
        // ======================================================== 

        // TODO:

        //
        return EXIT_SUCCESS;
    }

    return 0;
}


