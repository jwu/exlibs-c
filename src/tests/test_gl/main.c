// ======================================================================================
// File         : main.c
// Author       : Wu Jie 
// Last Change  : 12/15/2010 | 23:49:30 PM | Wednesday,December
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "../../core/core_inc.h"
#include "../../graphics/graphics_inc.h"

#include "../../entity/engine.h"
#include "../../entity/world.h"
#include "../../entity/entity.h"
#include "../../entity/eng_time.h"
#include "../../entity/trans2d.h"
#include "../../entity/debug2d.h"
#include "../../entity/camera.h"

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
// DISABLE { 
// static int paused = 0;
// static int step = 0;
// } DISABLE end 
static int ticks = 0;
static int win_width = 640; 
static int win_height = 480; 

// game
ex_world_t* world = NULL;
ex_entity_t* entity1 = NULL;

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void initGame () {
    world = ex_world_alloc();

    // TODO { 
    // init the world
    // serialize the world
    // } TODO end 

    {
        ex_camera_t* mainCam;
        ex_world_create_camera2d ( world, ex_strid("main_camera") );
        mainCam = ex_world_main_camera (world);
        ex_assert ( mainCam, "can't find main camera" );
        ex_camera_set_ortho( mainCam, true );
        ex_camera_set_aspect( mainCam, (float)win_width/(float)win_height );
        ex_camera_set_ortho_size( mainCam, (float)win_width/2.0f );
    }

    // TEMP: instead of serialize the world, I hardcoded the entities.
    for ( int i = 0; i < 200; ++i ) {
        entity1 = ex_world_create_entity ( world, ex_strid("ent1") ); {
            ex_trans2d_t* trans2d = (ex_trans2d_t*)ex_entity_add_comp( entity1, EX_CLASSID(ex_trans2d_t) );
            ex_vec2f_set ( &trans2d->_pos, ex_range_randf(-400.0f,400.0f), ex_range_randf(-400.0f,400.0f) );
            ex_vec2f_set ( &trans2d->_scale, ex_range_randf(0.0f,1.0f), ex_range_randf(0.0f,1.0f) );
            ex_angf_set_by_radians ( &trans2d->_ang, ex_range_randf(0.0f,EX_TWO_PI) );
            ex_debug2d_t* dbg2d = (ex_debug2d_t*)ex_entity_add_comp( entity1, EX_CLASSID(ex_debug2d_t) );
            ex_debug2d_set_rect ( dbg2d, 0.0f, 0.0f, 100.0f, 100.0f );
        }
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void quitGame () {
    ex_world_free(world);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void updateGame () {
    {
        // ex_vec2f_t d_pos = { 1.0f, 1.0f };
        ex_angf_t d_ang;
        ex_trans2d_t* trans2d = (ex_trans2d_t*)ex_entity_get_comp( entity1, EX_CLASSID(ex_trans2d_t) );

        // ex_vec2f_set ( &d_pos, 100.0f, 0.0f );
        // ex_vec2f_mul_scalar ( &d_pos, &d_pos, ex_dt() );
        // ex_vec2f_add ( &trans2d->_pos, &trans2d->_pos, &d_pos );

        ex_angf_set_by_degrees( &d_ang, 1.0f );
        ex_angf_mul_scalar ( &d_ang, &d_ang, ex_dt() * 10.0f );
        ex_angf_add ( &trans2d->_ang, &trans2d->_ang, &d_ang );
    }

    ex_world_update(world);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void _reshape ( int _width, int _height ) {
    win_width = _width;
    win_height = _height;
    ex_camera_t* mainCam = ex_world_main_camera (world);

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

static void _idle() {
    updateGame();
	glutPostRedisplay();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void _display() {
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

    // render 2D/3D objects in world space 
    {
        ex_world_render(world);
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

static void _keyboard ( unsigned char _key, int _x, int _y ) {
    if ( _key == EX_KEY_ESC ) {
        exit(0);
    }
    printf ( "key is %c \n", _key );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void _mouse ( int _x, int _y ) {
    // TODO:
    // mousePoint = mouseToSpace(x, y);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void _click ( int _button, int _state, int _x, int _y ) {
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
	glutInit(&argc, (char**)argv);
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
	glutReshapeFunc(_reshape);
	glutDisplayFunc(_display);
    glutIdleFunc(_idle);

    // DISABLE { 
    // glutTimerFunc(SLEEP_TICKS, timercall, 0);
    // } DISABLE end 

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(_keyboard);
    // glutSpecialFunc(arrowKeyDownFunc);
    // glutSpecialUpFunc(arrowKeyUpFunc);

	glutMotionFunc(_mouse);
	glutPassiveMotionFunc(_mouse);
	glutMouseFunc(_click);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void exit_fn () {
    // deinit
    quitGame ();
    ex_engine_deinit();
    ex_core_deinit();

    printf ("================\n");
    printf ("exit test_base...\n");
    printf ("================\n");
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int main( int argc, const char* argv[] ) {
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
        ex_engine_init();
        initGame();

        //
        glutMainLoop();
    }

    return 0;
}


