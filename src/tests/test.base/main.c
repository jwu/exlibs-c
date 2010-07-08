// ======================================================================================
// File         : main.c
// Author       : Wu Jie 
// Last Change  : 06/27/2010 | 08:50:48 AM | Sunday,June
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "../../core/core_inc.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

extern void test_bitarry();
extern void test_pool();
extern void test_array();
extern void test_hashmap();
extern void test_list();
extern void test_memmng();
extern void test_rapid();
extern void test_stringID();

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

    // test_bitarry ();
    // test_pool ();
    // test_array ();
    // test_hashmap ();
    // test_list ();
    // test_memmng ();
    test_stringID ();
    // test_rapid ();

    // deinit
    core_deinit();

    return 0;
}

