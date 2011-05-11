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
// internal test
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void on_paint ( ClutterActor *_actor, gpointer _user_data ) {
    clutter_actor_queue_redraw (CLUTTER_ACTOR (_actor));
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static gboolean queue_redraw ( gpointer _actor ) {
    clutter_actor_queue_redraw (CLUTTER_ACTOR (_actor));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////


// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static gboolean on_enter ( ClutterActor *_actor, ClutterEvent *_event, gpointer _user_data ) {
    ClutterState *state = CLUTTER_STATE (_user_data);
    clutter_state_set_state (state, "hover");
    return TRUE;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static gboolean on_leave ( ClutterActor *_actor, ClutterEvent *_event, gpointer _user_data ) {
    ClutterState *state = CLUTTER_STATE (_user_data);
    clutter_state_set_state (state, "normal");
    return TRUE;
}

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

        // ======================================================== 
        // create stage
        // ======================================================== 

        win_width = 800;
        win_height = 600;
        ClutterColor stage_color = { 0xaf, 0xaf, 0xaf, 0xff };
        stage = clutter_stage_get_default();
        clutter_actor_set_size(stage, win_width, win_height);

        clutter_stage_set_color(CLUTTER_STAGE(stage), &stage_color);
        clutter_stage_set_user_resizable (CLUTTER_STAGE(stage), TRUE);
        clutter_stage_set_accept_focus (CLUTTER_STAGE(stage), TRUE);
        clutter_stage_set_title (CLUTTER_STAGE(stage), "exsdk Editor v1.0.0");

        // ======================================================== 
        // create view-2D
        // ======================================================== 

        // TEMP { 
        ClutterActor *w1 = clutter_widget_new ();
        clutter_actor_set_size ( w1, 400, 300 );
        clutter_actor_set_position ( w1, 50, 50 );
        clutter_container_add_actor (CLUTTER_CONTAINER (stage), w1);
        clutter_actor_show (w1);

        ClutterActor *b1 = clutter_button_new ();
        clutter_actor_set_size ( b1, 20, 10 );
        clutter_actor_set_position ( b1, 50, 50 );
        clutter_container_add_actor (CLUTTER_CONTAINER (w1), b1);
        clutter_actor_show (b1);

        // } TEMP end 

        //
        ex_editor_init();
        ex_lua_load_modules( ex_lua_main_state(), "builtin" );
        ex_lua_load_modules( ex_lua_main_state(), "scripts" );
        __init_game ();

        // FIXME: if we don't keep repaint the scene, we will recieve warning when move the window from one monitor to another.
        guint idle_source = g_idle_add( queue_redraw, stage );
        // g_signal_connect_after (stage, "paint", G_CALLBACK (on_paint), NULL);

        // show the stage and enter the main loop
        clutter_actor_show(stage);
        clutter_main();

        //
        g_source_remove (idle_source);

        //
        return EXIT_SUCCESS;
    }

    return 0;
}


