// ======================================================================================
// File         : test_uid.c
// Author       : Wu Jie 
// Last Change  : 01/27/2011 | 10:01:13 AM | Thursday,January
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

void test_uid () {
    ex_text_file_t *txtFile;
    ex_string_t *tmp;
    ex_hashmap_t *map;
    ex_uid_t uid, last_uid;
    size_t idx;
    int i;

    txtFile = ex_text_fopen( "misc/uid_test.txt", false );

    map = ex_hashset(uint64,2048);
    tmp = ex_string("");

    for ( i = 0; i < 2000; ++i ) {
        uid = ex_generate_uid();

        // __ex_tostring_uid( tmp, &uid );
        // ex_string_cat_printf ( tmp, "\n" );
        ex_string_cat_printf ( tmp, "0x%.16llX\n", uid );

        ex_hashmap_insert ( map, &uid, &uid, &idx );
        ex_assert ( idx == i, 
                    "idx = %d, i = %d are not the same."
                    "the uid is 0x%.16llX," 
                    "the last uid is 0x%.16llX.", 
                    idx, i, uid, last_uid );
        last_uid = uid;
    }
    ex_text_fwrite ( txtFile, tmp->text, tmp->len );

    ex_hashmap_free(map);
    ex_string_free(tmp);
    ex_text_fclose(txtFile);
}
