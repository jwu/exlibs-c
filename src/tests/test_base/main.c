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

static void exit_fn (void) {
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
extern void test_uid();

extern const char *exsdk_dev_path;
// ------------------------------------------------------------------ 

int main( int argc, char *argv[] ) {
    char media_path[1024];

    printf ("================\n");
    printf ("start test_base...\n");
    printf ("================\n");

    // init
    if ( ex_core_init() != -1 ) {

        // register exit function
        atexit(exit_fn);

        // setup the fsys path
        strncpy ( media_path, exsdk_dev_path, 1024 );
        strcat ( media_path, "res/test_base/" );
        if ( ex_fsys_set_write_dir(media_path) == 0 )
            ex_log("set write dir dir: %s", media_path );
        if ( ex_fsys_mount( media_path, "/", true ) == 0 )
            ex_log("mount dir: %s", media_path );

        // general
        // test_array ();
        // test_bitarry ();
        // test_btree ();
        // test_cstr ();
        // test_encoding ();
        // test_hashmap ();
        // test_list ();
        // test_memmng ();
        // test_pool ();
        // test_rtti ();
        test_stream ();
        // test_string ();
        // test_stringID ();
        // test_thread();
        // test_timer();
        test_uid();

        // special
        // test_rapid ();
    }

    return 0;
}

