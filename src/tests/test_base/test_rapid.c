// ======================================================================================
// File         : test_rapid.c
// Author       : Wu Jie 
// Last Change  : 07/01/2010 | 09:56:53 AM | Thursday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// include
///////////////////////////////////////////////////////////////////////////////

#include "../../core/core_inc.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_rapid () {
    static const char *media_file = "/Users/Johnny/dev/projects/exdev/exsdk/res/";
    ex_text_file_t *txtFile;
    char path[1024];
                   
    strncpy ( path, media_file, 1024 );
    strcat ( path, "foobar.txt" );

    txtFile = ex_create_text_file(path,false);
    for ( int i = 0; i < 5000; ++i ) {
        ex_text_file_write_line ( txtFile, "hello world!\n" );
        ex_text_file_write_line ( txtFile, "vector = %f, %f, %f\n", 1.0f, 2.0f, 3.0f );
    }
    ex_destroy_text_file(txtFile);
}
