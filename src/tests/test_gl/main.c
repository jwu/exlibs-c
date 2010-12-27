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

#include "../../entity/world.h"
#include "../../entity/entity.h"

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

extern void test_rapid();

// system
// DISABLE { 
// static int paused = 0;
// static int step = 0;
// } DISABLE end 
static int ticks = 0;

// game
ex_world_t* world = NULL;

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void _reshape ( int _width, int _height ) {
	glViewport(0, 0, _width, _height);

	double rx = _width / 2.0;
	double ry = _height / 2.0;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho(-rx, rx, -ry, ry, -1.0, 1.0);
    glTranslated(0.5, 0.5, 0.0);
    // glOrtho(0, _width, _height, 0, -1.0, 1.0); // jwu DISABLE
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

    glClearColor(0.0f,0.5f,1.0f,1.0f); // RGBA background color
    glClear(GL_COLOR_BUFFER_BIT);

    // TODO: draw things
    ex_draw_string( -300, 210, "press ESC to quit." );

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
	glutInitWindowSize(640, 480);
	glutCreateWindow("test_gl");
    glutSetWindowTitle("test_gl");
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void initGL () {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void registerFuncs () {
	glutReshapeFunc(_reshape);
	glutDisplayFunc(_display);

    // DISABLE { 
    // glutIdleFunc(idle);
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

static void initGame () {
    ex_entity_t* ent;

    world = ex_world_alloc();

    // TODO { 
    // init the world
    // serialize the world
    // } TODO end 

    // TEMP: instead of serialize the world, I hardcoded the entities.
    ent = ex_world_create_entity ( world, "ent1" );
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
    if ( ex_core_init() ) {
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


