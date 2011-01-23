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
    ex_string_t *tmp, *tmp2;
                   
    strncpy ( path, media_file, 1024 );
    strcat ( path, "foobar.txt" );

    txtFile = ex_text_fopen(path,false);
    for ( int i = 0; i < 100; ++i ) {
        ex_text_fwrite ( txtFile, "line %d: vector = %f, %f, %f\n", i, 1.0f, 2.0f, 3.0f );
    }
    for ( int i = 100; i < 200; ++i ) {
        ex_text_fwrite_line ( txtFile, "line %d: vector = %f, %f, %f", i, 1.0f, 2.0f, 3.0f );
    }

    // tmp = ex_string("");
    // tmp2 = ex_string("");
    for ( int i = 0; i < 5000; ++i ) {
    }

    ex_text_fclose(txtFile);
}
