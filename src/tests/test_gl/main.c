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

#include "simple_behavior.h"

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

#if (EX_PLATFORM == EX_WIN32)
    static const char *__media_file = "e:/project/dev/exsdk/res/";
#else
    static const char *__media_file = "/Users/Johnny/dev/projects/exdev/exsdk/res/";
#endif
#define maxPATH 256

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

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void save_world () {
    char path[maxPATH];
    ex_stream_t *stream;

    if ( g_world == NULL )
        return;

    ex_log ("save world simple_world.json...");
    stream = ex_create_json_write_stream();
    ex_world_save( g_world, stream );
    strncpy ( path, __media_file, maxPATH );
    stream->save_to_file( stream, strcat(path, "simple_world.json") );
    ex_destroy_json_stream((ex_stream_json_t *)stream);
    ex_log ("simple_world.json saved!");
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void load_world () {
    char path[maxPATH];
    ex_stream_t *stream;

    if ( g_world == NULL )
        return;

    ex_world_stop( g_world );

    ex_world_clear(g_world);
    ex_decref(g_world);
    ex_destroy_object_immediately(g_world, true);

    ex_log ("loading world simple_world.json...");
    strncpy ( path, __media_file, maxPATH );
    stream = ex_create_json_read_stream( strcat(path, "simple_world.json") );

    g_world = ex_world_load(stream);
    EX_REF_PTR(ex_object_t,g_world)->init(g_world);
    
    ex_destroy_json_stream((ex_stream_json_t *)stream);
    ex_log ("simple_world.json loaded!");

    ex_world_run ( g_world );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void initGame () {
    // register game classes
    EX_REGISTER_CLASS(ex_simple_t);

    // load/setup the world
    g_world = ex_create_object( EX_TYPEID(ex_world_t), ex_generate_uid() );
    ex_incref(g_world);
    EX_REF_PTR(ex_object_t,g_world)->init(g_world);

    {
        ex_ref_t *mainCam;
        ex_world_create_camera2d ( g_world, ex_strid("main_camera") );
        mainCam = ex_world_main_camera (g_world);
        ex_assert ( mainCam, "can't find main camera" );
        ex_camera_set_ortho( mainCam, true );
        ex_camera_set_aspect( mainCam, (float)win_width/(float)win_height );
        ex_camera_set_ortho_size( mainCam, (float)win_width/2.0f );
    }

    ex_log ("create simple entities...");
    // TEMP: instead of serialize the g_world, I hardcoded the entities.
    for ( int i = 0; i < 100; ++i ) {
        ex_ref_t *ent = ex_world_create_entity ( g_world, ex_strid("ent1") ); 
        {
            // trans2d
            ex_ref_t *trans2d = ex_entity_add_comp( ent, EX_TYPEID(ex_trans2d_t) );
            ex_trans2d_set_world_position( trans2d, ex_range_randf(-400.0f,400.0f), ex_range_randf(-400.0f,400.0f) );
            ex_trans2d_set_world_scale ( trans2d, ex_range_randf(0.0f,1.0f), ex_range_randf(0.0f,1.0f) );
            ex_trans2d_set_world_rotation ( trans2d, ex_range_randf(0.0f,EX_TWO_PI) );

            // dbg2d
            ex_ref_t *dbg2d = ex_entity_add_comp( ent, EX_TYPEID(ex_debug2d_t) );
            ex_debug2d_set_rect ( dbg2d, 0.0f, 0.0f, 100.0f, 100.0f );

            // simple
            ex_ref_t *simple_ref = ex_entity_add_comp( ent, EX_TYPEID(ex_simple_t) );
            ex_simple_t *simple = EX_REF_PTR(ex_simple_t,simple_ref);

            ex_vec2f_set ( &simple->move_dir, ex_range_randf(-1.0f,1.0f), ex_range_randf(-1.0f,1.0f) );
            ex_vec2f_normalize(&simple->move_dir);
            simple->move_speed = ex_range_randf(1.0f,100.0f);
            simple->rot_speed = ex_range_randf(-100.0f,100.0f);
        }
    }
    ex_log ("done!");

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
    ex_world_update(g_world);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __reshape ( int _width, int _height ) {
    win_width = _width;
    win_height = _height;
    ex_ref_t *mainCam = ex_world_main_camera (g_world);

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

static void __idle() {
    updateGame();
	glutPostRedisplay();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __display() {
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

    // render 2D/3D objects in g_world space 
    {
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
        ex_draw_string( x, y, text );
        y += 12; ex_draw_string( x, y, "press ESC to quit." );
        y += 12; ex_draw_string( x, y, "this is a simple gl test." );
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
            break;

        case 12: // ^L
            load_world ();
            break;
        }
    }
    else {
        if ( _key == EX_KEY_ESC ) {
            exit(0);
        }
    }

    // printf ( "key is %c \n", _key );
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

static void createWindow ( int argc, const char *argv[] ) {
	glutInit(&argc, (char **)argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
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

static void exit_fn () {
    // deinit
    quitGame ();
    ex_app_deinit();
    ex_core_deinit();

    printf ("================\n");
    printf ("exit test_base...\n");
    printf ("================\n");
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int main( int argc, const char *argv[] ) {
    printf ("================\n");
    printf ("start gl testing...\n");
    printf ("================\n");

    // init
    if ( ex_core_init() != -1 ) {
        // register exit function
        atexit(exit_fn);

        // init graphics
        createWindow ( argc, argv );
        initGL();
        registerFuncs();

        // init game
        ex_app_init();
        initGame();

        //
        glutMainLoop();
    }

    return 0;
}


