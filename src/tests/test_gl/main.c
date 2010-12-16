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

// DISABLE { 
// static int paused = 0;
// static int step = 0;
// } DISABLE end 
static int ticks = 0;

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

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

static void reshape ( int _width, int _height ) {
	glViewport(0, 0, _width, _height);

	double rx = _width / 2.0;
	double ry = _height / 2.0;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-rx, rx, -ry, ry, -1.0, 1.0);
	glTranslated(0.5, 0.5, 0.0);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void display() {

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

    glClearColor(1.0f,0.0f,0.0f,1.0f); // background color
    glClear(GL_COLOR_BUFFER_BIT);

    // drawSpace(space, currDemo->drawOptions ? currDemo->drawOptions : &options);
    // if(!paused){
    //     drawInstructions();
    //     drawInfo();
    // }
    // drawString(-300, -210, messageString);

    glutSwapBuffers();
    ++ticks;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void createWindow ( int argc, const char *argv[] ) {
	glutInit(&argc, (char**)argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(640, 480);
	glutCreateWindow("test_gl");
	
	initGL();
	
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

#if 0
    // glutIdleFunc(idle);
	glutTimerFunc(SLEEP_TICKS, timercall, 0);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(keyboard);
	
	glutSpecialFunc(arrowKeyDownFunc);
	glutSpecialUpFunc(arrowKeyUpFunc);

	glutMotionFunc(mouse);
	glutPassiveMotionFunc(mouse);
	glutMouseFunc(click);
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void exit_fn () {
    // deinit
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

        //
        createWindow ( argc, argv );
        glutSetWindowTitle("test_gl");

        // TODO { 
        // special
        // test_rapid ();
        // } TODO end 

        glutMainLoop();
    }

    return 0;
}


