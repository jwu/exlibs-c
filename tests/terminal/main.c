// ======================================================================================
// File         : main.c
// Author       : Wu Jie 
// Last Change  : 06/27/2010 | 08:50:48 AM | Sunday,June
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "core/core_inc.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

static jmp_buf exception;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void exit_fn (void) {
    // deinit
    ex_core_deinit();

    printf ("================\n");
    printf ("exit test in terminal...\n");
    printf ("================\n");
}

// ------------------------------------------------------------------ 
// Desc: 
extern void run_test ();
// ------------------------------------------------------------------ 

int main( int argc, char *argv[] ) {

    printf ("================\n");
    printf ("start test in terminal...\n");
    printf ("================\n");

    // setup default media path
    ex_init_dev_mode ( "tests/terminal/res/" );
    
    // init
    if ( ex_core_init( &argc, &argv ) != -1 ) {

        // register exit function
        atexit(exit_fn);

        // general
        switch ( setjmp(exception) ) {
        case 0:
            run_test();
            break;

        default:
            printf ( "fatal error!" );
            break;
        }
    }

    //
    return 0;
}

