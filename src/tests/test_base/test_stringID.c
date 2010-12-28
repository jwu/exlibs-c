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

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

const char* media_file = "e:/project/dev/exsdk/res/";
// const char* media_file = "e:/dev/exsdk/res/";
#define maxPATH 256

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void normal () {
    char path[maxPATH];
    FILE* fp;
    size_t fsize;
    char* data;
    ex_array_t *words, *word_IDs;

    strncpy ( path, media_file, maxPATH );
    fp = fopen ( strcat(  path, "sid.test.txt" ), "r" );
    ex_assert_return ( fp, /*void*/, "file %s not found!", path );

    fseek ( fp, 0L, SEEK_END );
    fsize = ftell(fp);
    fseek ( fp, 0L, SEEK_SET );

    data = (char*)ex_malloc( fsize * sizeof(char) );
    fread ( data, sizeof(char), fsize, fp );

    {
        uint cur = 0,prev = 0;
        size_t sid;

        words = ex_array_alloc ( sizeof(char*), 256 );
        word_IDs = ex_array_alloc ( sizeof(size_t), 256 );

        // dump string and string ID
        while ( cur < fsize ) {
            if ( data[cur] == ' ' ) {
                char* word = ex_malloc(sizeof(char)*128);
                uint word_len = cur - prev;

                ex_assert ( word_len < 128, "the word exceed the length of the string (e.g 128) " );
                strncpy ( word, data + prev, word_len );
                word[word_len] = '\0';

                sid = ex_strid( word );
                ex_assert(sid != -1, "can't get string ID of %s", word);

                ex_array_append ( words, &word );
                ex_array_append ( word_IDs, &sid );
                prev = cur;
            }
            ++cur;
        }

        // check if the string ID consistent with string
        cur = 0;
        while ( cur < ex_array_len(words) ) {
            char* word = *((char**)ex_array_get( words, cur ));
            size_t sid = *((size_t*)ex_array_get( word_IDs, cur )); 
            // EX_TEST ( wcscmp(ex_strid_to_cstr(sid), word) == 0 );
            ex_assert ( strcmp(ex_strid_to_cstr(sid), word) == 0, "%s(%d) is not equal to %s", ex_strid_to_cstr(sid), sid, word );
            ++cur;
        }

        // release all memory
        cur = 0;
        while ( cur < ex_array_len(words) ) {
            void* ptr = *((char**)ex_array_get( words, cur ));
            ex_free(ptr);
            ++cur;
        }
        ex_array_free ( words );
        ex_array_free ( word_IDs );
    }

    ex_free (data);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void widechar() {
    size_t id = -1;
    id = ex_strid_from_wcs( L"Hello World" );
    ex_log ( "id = %zd", id );
    id = ex_strid( "Hello World" );
    ex_log ( "id = %zd", id );

    // TODO { 
    // {
    //     size_t id = ex_strid ("c");
    //     char* str = ex_strid_to_cstr(id);
    //     EX_TEST ( strcmp(str, "c") == 0 );
    // }

    // {
    //     size_t id = ex_strid ( "中c");
    //     char* str = ex_strid_to_cstr(id);
    //     EX_TEST ( strcmp(str, "中c") == 0 );
    // }

    // {
    //     char s[128];
    //     wchar_t ws[128];

    //     memset ( s, 0xcc, sizeof(char)*128 );
    //     strcpy ( s, "中c" );
    //     memset ( ws, 0xcc, sizeof(wchar_t)*128 );
    //     wcscpy ( ws, L"中c" );

    //     EX_HW_BREAK();
    // }
    // } TODO end 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void invalid() {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_stringID () {
    normal();
    widechar();
    invalid();
}

