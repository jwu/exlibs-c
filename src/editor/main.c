// ======================================================================================
// File         : main.c
// Author       : Wu Jie 
// Last Change  : 05/04/2011 | 14:43:57 PM | Wednesday,May
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "editor.h"
#include "engine/engine_inc.h"

#if (EX_PLATFORM == EX_MACOSX)

	#include "OpenGL/gl.h"
	#include "OpenGL/glu.h"
	#include <GLUT/glut.h>

#else

	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>

#endif

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// system
extern int win_width;
extern int win_height;

// game
extern ex_ref_t *g_world;
extern char *g_world_path;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __save_world () {
    ex_stream_t *stream;

    if ( g_world == NULL )
        return;

    ex_log ("save world last_save.json...");
    stream = ex_create_json_write_stream();
    ex_world_save( g_world, stream );

    if ( ex_fsys_dir_exists("world") == false )
        ex_fsys_mkdir("world");
    if ( stream->save_to_file( stream, "world/last_save.json" ) != 0 ) 
        ex_log("failed to save world world/last_save.json");

    ex_destroy_json_stream((ex_stream_json_t *)stream);
    ex_log ("last_save.json saved!");
}

// ------------------------------------------------------------------ 
// Desc: 
static void __quit_game ();
// ------------------------------------------------------------------ 

static void __load_world () {
    ex_stream_t *stream;

    if ( g_world == NULL )
        return;

    //
    __quit_game ();

    ex_log ("loading world last_save.json...");
    stream = ex_create_json_read_stream( "world/saved_world.json" );

    g_world = ex_world_load(stream);
    EX_REF_CAST(ex_object_t,g_world)->init(g_world);
    
    ex_destroy_json_stream((ex_stream_json_t *)stream);
    ex_log ("last_save.json loaded!");

    ex_world_run ( g_world );
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

static void __update_game () {
    ex_world_update(g_world);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __display(void) {
    // DISABLE { 
    // cpVect newPoint = cpvlerp(mousePoint_last, mousePoint, 0.25f);

    // mouseBody->p = newPoint;
    // mouseBody->v = cpvmult(cpvsub(newPoint, mousePoint_last), 60.0f);
    // mousePoint_last = newPoint;
    // } DISABLE end 

    // render 2D/3D objects
    ex_world_render(g_world);

    // draw 2D objects in screen space
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho(0, win_width, win_height, 0, -1.0, 1.0);
    {
        char text[128];
        int x = 10; int y = 15;
        snprintf( text, 128, "fps: %f", ex_fps() );
        ex_screen_print( x, y, text );
        y += 15; ex_screen_print( x, y, "world time %.1f", ex_worldtime() );
        y += 15; ex_screen_print( x, y, "press \"ESC\" to quit." );
        y += 15; ex_screen_print( x, y, "press \"r\" to restart the game." );
        y += 15; ex_screen_print( x, y, "this is a simple gl test." );
        y += 15; ex_screen_print( x, y, "total memory used by ex: %f MB", (float)ex_total_org_memory() / (float)(1024 * 1024) );
        y += 15; ex_screen_print( x, y, "total memory used by dl: %f MB", (float)ex_total_dl_memory() / (float)(1024 * 1024) );
        y += 15; ex_screen_print( x, y, "total memory used by lua: %f MB", (float)ex_lua_totoal_memory(ex_lua_main_state()) / (float)(1024) );
    }

    glutSwapBuffers();
}

// ------------------------------------------------------------------ 
// Desc: 
static void __reshape ( int, int );
static void __restart_game ();
// ------------------------------------------------------------------ 

static void __keyboard ( unsigned char _key, int _x, int _y ) {
    int mod = 0;

    mod = glutGetModifiers();

    //
    if ( mod == GLUT_ACTIVE_CTRL ) {
        switch (_key) {
        case 19: // ^S
            __save_world ();
            return;

        case 12: // ^L
            __load_world ();
            __reshape( win_width, win_height );
            return;
        }
    }
    else {
        if ( _key == EX_KEY_ESC ) {
            exit(0);
            return;
        }
        if ( _key == EX_KEY_r ) {
            __restart_game();
        }
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __mouse ( int _x, int _y ) {
    // TODO:
    // mousePoint = mouseToSpace(x, y);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __click ( int _button, int _state, int _x, int _y ) {
    // if(button == GLUT_LEFT_BUTTON){
    //     if(state == GLUT_DOWN){
    //         cpVect point = mouseToSpace(x, y);
    //     
    //         cpShape *shape = cpSpacePointQueryFirst(space, point, GRABABLE_MASK_BIT, CP_NO_GROUP);
    //         if(shape){
    //             cpBody *body = shape->body;
    //             mouseJoint = cpPivotJointNew2(mouseBody, body, cpvzero, cpBodyWorld2Local(body, point));
    //             mouseJoint->maxForce = 50000.0f;
    //             mouseJoint->biasCoef = 0.15f;
    //             cpSpaceAddConstraint(space, mouseJoint);
    //         }
    //     } else if(mouseJoint){
    //         cpSpaceRemoveConstraint(space, mouseJoint);
    //         cpConstraintFree(mouseJoint);
    //         mouseJoint = NULL;
    //     }
    // }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __reshape ( int _width, int _height ) {
    ex_ref_t *mainCam;
    
    win_width = _width;
    win_height = _height;
    mainCam = ex_world_main_camera (g_world);

    // setup viewport
    glViewport(0, 0, win_width, win_height);

    // set camera
    ex_assert ( mainCam, "can't find main camera" );
    ex_camera_set_ortho( mainCam, true );
    ex_camera_set_aspect( mainCam, (float)win_width/(float)win_height );
    ex_camera_set_ortho_size( mainCam, (float)win_height/2.0f );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __restart_game () {
    __quit_game();
    __init_game();
    __reshape( win_width, win_height );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __idle(void) {
    __update_game();
	glutPostRedisplay();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __create_window ( int argc, char *argv[] ) {
    win_width = 640;
    win_height = 480;

	glutInit(&argc, (char **)argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize(win_width, win_height);
	glutCreateWindow("test_gl");
    glutSetWindowTitle("test_gl");
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __init_gl () {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

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

    //
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __register_funcs () {
	glutReshapeFunc(__reshape);
	glutDisplayFunc(__display);
    glutIdleFunc(__idle);

    // DISABLE { 
    // glutTimerFunc(SLEEP_TICKS, timercall, 0);
    // } DISABLE end 

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(__keyboard);
    // glutSpecialFunc(arrowKeyDownFunc);
    // glutSpecialUpFunc(arrowKeyUpFunc);

	glutMotionFunc(__mouse);
	glutPassiveMotionFunc(__mouse);
	glutMouseFunc(__click);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void exit_fn (void) {
    // deinit
    __quit_game ();
    ex_lua_deinit();
    ex_editor_deinit();
    ex_core_deinit();

    if ( g_world_path ) {
        free(g_world_path);
        g_world_path = NULL;
    }

    printf ("================\n");
    printf ("exit exsdk-editor\n");
    printf ("================\n");
}


///////////////////////////////////////////////////////////////////////////////
// intneral defines
///////////////////////////////////////////////////////////////////////////////

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


// // ------------------------------------------------------------------ 
// // Desc: 
// // ------------------------------------------------------------------ 

// int main( int argc, char *argv[] ) {

//     printf ("================\n");
//     printf ("start exsdk-editor\n");
//     printf ("================\n");

//     // setup editor
//     ex_set_editor (true);
//     ex_set_edit_mode (true);

//     // init
//     if ( ex_core_init( &argc, &argv ) != -1 ) {
//         __parse_world ( &argc, &argv );

//         // register exit function
//         atexit(exit_fn);

//         // init graphics
//         __create_window ( argc, argv );
//         __init_gl();
//         __register_funcs();

//         // init game
//         ex_editor_init();
//         ex_lua_load_modules( ex_lua_main_state(), "builtin" );
//         ex_lua_load_modules( ex_lua_main_state(), "scripts" );
//         __init_game();

//         //
//         glutMainLoop();
//     }

//     return 0;
// }


