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
    ex_string_t *tmp;
    ex_hashmap_t *map;
    ex_uid_t uid, last_uid;
    size_t idx;

    strncpy ( path, media_file, 1024 );
    strcat ( path, "uid_test.txt" );
    txtFile = ex_text_fopen(path,false);

    map = ex_hashset(uint64,2048);
    tmp = ex_string("");

    for ( int i = 0; i < 2000; ++i ) {
        uid = ex_generate_uid();
        ex_string_cat_printf ( tmp, "%.16llX\n", uid );
        ex_hashmap_insert ( map, &uid, &uid, &idx );
        ex_assert ( idx == i, 
                    "idx = %d, i = %d are not the same."
                    "the uid is %.16llX," 
                    "the last uid is %.16llX.", 
                    idx, i, uid, last_uid );
        last_uid = uid;
    }
    ex_text_fwrite ( txtFile, tmp->text, tmp->len );

    ex_hashmap_free(map);
    ex_string_free(tmp);
    ex_text_fclose(txtFile);
}
