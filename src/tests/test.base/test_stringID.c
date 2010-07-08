// ======================================================================================
// File         : test_stringID.c
// Author       : Wu Jie 
// Last Change  : 07/07/2010 | 16:34:45 PM | Wednesday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "../../core/core_inc.h"
#include "../../core/string/stringID.h"
#include "../../container/array.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

const char* media_file = "e:/project/dev/exsdk/res/";

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void normal ()
{
    char path[MAX_PATH];
    FILE* fp;
    size_t fsize;
    wchar_t* data;
    array_t *words, *word_IDs;

    strncpy ( path, media_file, MAX_PATH );
    fp = fopen ( strcat(  path, "sid.test.txt" ), "r" );

    fseek ( fp, 0L, SEEK_END );
    fsize = ftell(fp);
    fseek ( fp, 0L, SEEK_SET );

    data = (wchar_t*)ex_malloc( fsize * sizeof(wchar_t) );
    fread ( data, sizeof(wchar_t), fsize, fp );

    {
        int cur,prev = 0;
        words = array_alloc ( sizeof(char*), 256 );
        word_IDs = array_alloc ( sizeof(size_t), 256 );

        // while ( cur < fsize ) {
        //     if ( data[cur] == ' ' ) {
        //         char word[128];
        //         ex_assert (prev - cur < 128 );
        //         strncpy ( word, data + prev, prev - cur, );
        //         array_push_back ( words, &word );
        //         array_push_back ( word_IDs, strID( latin1_to_utf8(word) ) );
        //         prev = cur;
        //     }
        //     ++cur;
        // }

        array_free ( words );
        array_free ( word_IDs );
    }

    ex_free (data);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void invalid()
{
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_stringID ()
{
    normal();
    invalid();
}

