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

extern void test_normal();

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int main( int argc, char* argv[] )
{
    printf ("================\n");
    printf ("start testing...\n");
    printf ("================\n");

    // init
    core_init();

    //
    test_normal ();

    // deinit
    core_deinit();

    return 0;
}

