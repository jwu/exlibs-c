// ======================================================================================
// File         : main.c
// Author       : Wu Jie 
// Last Change  : 12/15/2010 | 23:49:30 PM | Wednesday,December
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

#include "../../graphics/graphics_inc.h"
#include "../../engine/engine_inc.h"
#include "../../app/app.h"

#if (EX_PLATFORM == EX_MACOSX)
	#include "OpenGL/gl.h"
	#include "OpenGL/glu.h"
	#include <GLUT/glut.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
#endif

#include "main.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// system
// DISABLE { 
// static int paused = 0;
// static int step = 0;
// } DISABLE end 
static int ticks = 0;
static int win_width = 640; 
static int win_height = 480; 

// game
ex_ref_t *g_world = NULL;
game_t g_game = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void save_world () {
    ex_stream_t *stream;

    if ( g_world == NULL )
        return;

    ex_log ("save world simple_world.json...");
    stream = ex_create_json_write_stream();
    ex_world_save( g_world, stream );

    if ( ex_fsys_dir_exists("world") == false )
        ex_fsys_mkdir("world");
    if ( stream->save_to_file( stream, "world/simple_world.json" ) != 0 ) 
        ex_log("failed to save world world/simple_world.json");

    ex_destroy_json_stream((ex_stream_json_t *)stream);
    ex_log ("simple_world.json saved!");
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void load_world () {
    ex_stream_t *stream;

    if ( g_world == NULL )
        return;

    ex_world_stop( g_world );

    ex_world_clear(g_world);
    ex_decref(g_world);
    ex_destroy_object_immediately(g_world, true);

    ex_log ("loading world simple_world.json...");
    stream = ex_create_json_read_stream( "world/simple_world.json" );

    g_world = ex_world_load(stream);
    EX_REF_CAST(ex_object_t,g_world)->init(g_world);
    
    ex_destroy_json_stream((ex_stream_json_t *)stream);
    ex_log ("simple_world.json loaded!");

    ex_world_run ( g_world );
}

// ------------------------------------------------------------------ 
// Desc: 
extern void simple_world ();
extern void test_transform ();
extern void test_script ();
// ------------------------------------------------------------------ 

static void initGame () {

    // create test world
    // NOTE: choose your game { 
    // simple_world();
    // test_transform();
    // test_script("exec/create_world.lua");
    test_script("exec/test_transform.lua");
    // } NOTE end 

    // load/setup the world
    g_world = ex_create_object( EX_RTTI(ex_world_t), ex_generate_uid() );
    ex_incref(g_world);
    EX_REF_CAST(ex_object_t,g_world)->init(g_world);

    // create main camera
    {
        ex_ref_t *mainCam;

        ex_log ("create main camera...");
        ex_world_create_camera2d ( g_world, ex_strid("main_camera") );
        mainCam = ex_world_main_camera (g_world);
        ex_assert ( mainCam, "can't find main camera" );
        ex_camera_set_ortho( mainCam, true );
        ex_camera_set_aspect( mainCam, (float)win_width/(float)win_height );
        ex_camera_set_ortho_size( mainCam, (float)win_width/2.0f );
    }

    // create the world
    if ( g_game.init ) 
        g_game.init();

    // run the world
    ex_world_run(g_world);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void quitGame () {
    ex_world_stop(g_world);

    ex_world_clear(g_world);
    ex_decref(g_world);
    ex_destroy_object_immediately(g_world, true);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void updateGame () {
    // why ?? for global control of the game.
    if ( g_game.update )
        g_game.update();

    ex_world_update(g_world);
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

static void __idle(void) {
    updateGame();
	glutPostRedisplay();
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

    // TODO { 
    // if(!paused || step > 0){
    //     currDemo->updateFunc(ticks);
    //     step = (step > 1 ? step - 1 : 0);
    // }
    // } TODO end 

    // render 2D/3D objects, if g_game have the render function, let it do the things 
    if ( g_game.render ) {
        g_game.render (); 
    }
    else {
        ex_world_render(g_world);
    }

    // draw 2D objects in screen space
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho(0, win_width, win_height, 0, -1.0, 1.0);
    {
        char text[128];
        int x = 10; int y = 10;
        snprintf( text, 128, "fps: %f", ex_fps() );
        ex_screen_print( x, y, text );
        y += 12; ex_screen_print( x, y, "press ESC to quit." );
        y += 12; ex_screen_print( x, y, "this is a simple gl test." );
    }

    glutSwapBuffers();
    ++ticks;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __keyboard ( unsigned char _key, int _x, int _y ) {
    int mod = 0;

    mod = glutGetModifiers();

    //
    if ( mod == GLUT_ACTIVE_CTRL ) {
        switch (_key) {
        case 19: // ^S
            save_world ();
            return;

        case 12: // ^L
            load_world ();
            __reshape( win_width, win_height );
            return;
        }
    }
    else {
        if ( _key == EX_KEY_ESC ) {
            exit(0);
            return;
        }
    }

    //
    if ( g_game.keyboard )
        g_game.keyboard(_key);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __mouse ( int _x, int _y ) {
    // TODO:
    // mousePoint = mouseToSpace(x, y);

    //
    if ( g_game.mouse )
        g_game.mouse(_x,_y);
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

static void createWindow ( int argc, const char *argv[] ) {
	glutInit(&argc, (char **)argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize(win_width, win_height);
	glutCreateWindow("test_gl");
    glutSetWindowTitle("test_gl");
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void initGL () {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glEnableClientState(GL_VERTEX_ARRAY);

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
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void registerFuncs () {
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
    quitGame ();
    ex_lua_deinit();
    ex_app_deinit();
    ex_core_deinit();

    printf ("================\n");
    printf ("exit test_base...\n");
    printf ("================\n");
}

// ------------------------------------------------------------------ 
// Desc: 
extern const char *exsdk_dev_path;
// ------------------------------------------------------------------ 

int main( int argc, const char *argv[] ) {
    char media_path[1024];

    printf ("================\n");
    printf ("start gl testing...\n");
    printf ("================\n");

    // init
    strncpy ( media_path, exsdk_dev_path, 1024 );
    strcat ( media_path, "test_res/test_gl/" );
    if ( ex_core_init(media_path) != -1 ) {

        // register exit function
        atexit(exit_fn);

        // init graphics
        createWindow ( argc, argv );
        initGL();
        registerFuncs();

        // init game
        ex_app_init();
        ex_lua_load_modules( ex_current_lua_state(), "scripts" );
        initGame();

        //
        glutMainLoop();
    }

    return 0;
}


