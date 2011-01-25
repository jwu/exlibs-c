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
extern void test_array();
extern void test_bitarry();
extern void test_btree();
extern void test_cstr();
extern void test_encoding();
extern void test_hashmap();
extern void test_list();
extern void test_memmng();
extern void test_pool();
extern void test_rapid();
extern void test_rtti();
extern void test_stream();
extern void test_string();
extern void test_stringID();
extern void test_thread();
extern void test_timer();
// ------------------------------------------------------------------ 

int main( int argc, char *argv[] )
{
    printf ("================\n");
    printf ("start test_base...\n");
    printf ("================\n");

    // init
    if ( ex_core_init() != -1 ) {
        // register exit function
        atexit(exit_fn);

        // general
        // test_array ();
        // test_bitarry ();
        // test_btree ();
        // test_cstr ();
        // test_encoding ();
        // test_hashmap ();
        // test_list ();
        // test_memmng ();
        test_pool ();
        // test_rtti ();
        // test_stream ();
        // test_string ();
        // test_stringID ();
        // test_thread();
        // test_timer();

        // special
        // test_rapid ();
    }

    return 0;
}

