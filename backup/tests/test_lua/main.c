// ======================================================================================
// File         : main.c
// Author       : Wu Jie 
// Last Change  : 07/06/2010 | 16:04:43 PM | Tuesday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "../../core/core_inc.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

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
extern void test_normal();
// ------------------------------------------------------------------ 

int main( int argc, char *argv[] )
{
    printf ("================\n");
    printf ("start testing...\n");
    printf ("================\n");

    // init
    if ( ex_core_init() != -1 ) {
        // register exit function
        atexit(exit_fn);

        //
        test_normal ();
    }

    return 0;
}

