// ======================================================================================
// File         : clutter_main.c
// Author       : Wu Jie 
// Last Change  : 05/05/2011 | 13:12:49 PM | Thursday,May
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "editor.h"
#include "engine/engine_inc.h"

#include "clutter/clutter.h"
#include "cogl/cogl.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// system
float win_width = 0.0;
float win_height = 0.0;
ClutterActor *stage = NULL;

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

    clutter_actor_get_size ( stage, &win_width, &win_height );

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
    ex_assert_return ( g_world_path, /*void*/, "can't find a world! please specific it by --world=" );
    ex_lua_dofile( l, g_world_path, NULL );

    // run the world
    ex_world_run(g_world);
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
// internal test
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void on_paint ( ClutterActor *_actor, gpointer _user_data ) {
    ex_world_update(g_world);

    //
    glViewport(0, 0, win_width, win_height);

    // anti-aliasing { 
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glHint(GL_POINT_SMOOTH_HINT, GL_DONT_CARE);
    // } anti-aliasing end 

    // enable depath { 
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(true);
    // } enable depath end 

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    //
    ex_world_render(g_world,false);

    // //
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // glOrtho(0, win_width, win_height, 0, -1.0, 1.0);
    // clutter_actor_queue_redraw (_actor); 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static gboolean queue_redraw ( gpointer _stage ) {
    clutter_actor_queue_redraw (CLUTTER_ACTOR (_stage));

    return TRUE;
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

        // init clutter
        clutter_init(&argc, &argv);

        // create stage
        win_width = 800;
        win_height = 600;
        ClutterColor stage_color = { 0xaf, 0xaf, 0xaf, 0xff };
        stage = clutter_stage_get_default();
        clutter_actor_set_size(stage, win_width, win_height);

        clutter_stage_set_color(CLUTTER_STAGE(stage), &stage_color);
        clutter_stage_set_user_resizable (CLUTTER_STAGE(stage), TRUE);

        //
        ClutterColor rect_color = { 0x44, 0xdd, 0x44, 0x9f };
        ClutterActor *rect = clutter_rectangle_new_with_color (&rect_color);
        clutter_container_add_actor (CLUTTER_CONTAINER (stage), rect);
        clutter_actor_set_size (rect, 50, 50);
        clutter_actor_set_position (rect, 0.0, 0.0);
        clutter_actor_show (rect);

        // TODO: implement your own actor with your own paint { 
        // //
        // ClutterColor bg_color = { 0x3f, 0x3f, 0x3f, 0xff };
        // ClutterColor bd_color = { 0x00, 0x00, 0x00, 0xff };
        // ClutterActor *game_view = clutter_rectangle_new_with_color (&bg_color);
        // clutter_rectangle_set_border_color ( CLUTTER_RECTANGLE(game_view), &bd_color );
        // clutter_rectangle_set_border_width ( CLUTTER_RECTANGLE(game_view), 1 );

        // clutter_container_add_actor (CLUTTER_CONTAINER (stage), game_view);
        // clutter_actor_set_size (game_view, 400, 300);
        // clutter_actor_set_position (game_view, win_width/2-200, win_height/2-150);
        // clutter_actor_show (game_view);

        // //
        // // guint idle_source = g_idle_add( queue_redraw, game_view );
        // g_signal_connect_after (game_view, "paint", G_CALLBACK (on_paint), NULL);
        // } TODO end 

        //
        ex_editor_init();
        ex_lua_load_modules( ex_lua_main_state(), "builtin" );
        ex_lua_load_modules( ex_lua_main_state(), "scripts" );
        __init_game ();

        // show the stage and enter the main loop
        clutter_actor_show(stage);
        clutter_main();

        // g_source_remove (idle_source);

        return EXIT_SUCCESS;
    }

    return 0;
}

