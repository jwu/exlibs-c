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
// const char* media_file = "e:/dev/exsdk/res/";

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void normal ()
{
    char path[MAX_PATH];
    FILE* fp;
    size_t fsize;
    char* data;
    array_t *words, *word_IDs;

    strncpy ( path, media_file, MAX_PATH );
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

        words = array_alloc ( sizeof(wchar_t*), 256 );
        word_IDs = array_alloc ( sizeof(size_t), 256 );

        // dump string and string ID
        while ( cur < fsize ) {
            if ( data[cur] == ' ' ) {
                char word[128];
                wchar_t* w_word;
                uint word_len = cur - prev;

                ex_assert ( word_len < 128, "the word exceed the length of the string (e.g 128) " );
                strncpy ( word, data + prev, word_len );
                word[word_len] = '\0';

                w_word = ex_malloc (128 * sizeof(wchar_t));
                sid = strID( to_wcs( word, w_word ) );
                ex_assert(sid != -1, "can't get string ID of %ls", w_word);

                array_push_back ( words, &w_word );
                array_push_back ( word_IDs, &sid );
                prev = cur;
            }
            ++cur;
        }

        // check if the string ID consistent with string
        cur = 0;
        while ( cur < array_len(words) ) {
            wchar_t* word = *((wchar_t**)array_get( words, cur ));
            size_t sid = *((size_t*)array_get( word_IDs, cur )); 
            // ex_check ( wcscmp(sid_toString(sid), word) == 0 );
            ex_assert ( wcscmp(sid_toString(sid), word) == 0, "%s(%d) is not equal to %s", sid_toString(sid), sid, word );
            ++cur;
        }

        // release all memory
        cur = 0;
        while ( cur < array_len(words) ) {
            void* ptr = *((wchar_t**)array_get( words, cur ));
            ex_free(ptr);
            ++cur;
        }
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

