// ======================================================================================
// File         : test_hashmap.c
// Author       : Wu Jie 
// Last Change  : 06/28/2010 | 18:36:49 PM | Monday,June
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

static void normal ()
{
    {
        uint32 key; 
        float val;
        int i;

        ex_hashmap_t* hashmap = ex_hashmap_alloc ( sizeof(uint32), sizeof(float), 256, ex_hashkey_uint32, ex_keycmp_uint32 );

        for ( i = 0; i < 512; ++i ) {
            key = 10 + i * 10;
            val = 1.0f + i;
            ex_hashmap_insert ( hashmap, &key, &val, NULL );
        }

        i = 0;
        ex_hashmap_each ( float, el, hashmap ) {
            printf ( "[%d] value is %f\n", i, el );
            ++i;
        } ex_hashmap_each_end;

        // check
        for ( i = 0; i < 512; ++i ) {
            key = 10 + i * 10;
            val = 1.0f + i;
            EX_TEST ( *((float*)ex_hashmap_get ( hashmap, &key, NULL )) == val );
        }
        EX_TEST ( ex_hashmap_len(hashmap) == 512 );

        // erase
        for ( i = 0; i < 512; ++i ) {
            key = 10 + i * 10;
            EX_TEST ( ex_hashmap_erase(hashmap, &key) != NULL );
        }
        EX_TEST ( ex_hashmap_len(hashmap) == 0 );

        ex_hashmap_free ( hashmap );
    }

    {
        char* key; 
        float val;

        ex_hashmap_t* hashmap = ex_hashmap_alloc ( sizeof(char*), sizeof(float), 256, ex_hashkey_string, ex_keycmp_string );

	    key = "foo"; val = 1.0f; ex_hashmap_insert ( hashmap, &key, &val, NULL );
        key = "bar"; val = 2.0f; ex_hashmap_insert ( hashmap, &key, &val, NULL );
        key = "hello"; val = 3.0f; ex_hashmap_insert ( hashmap, &key, &val, NULL );
        key = "world"; val = 4.0f; ex_hashmap_insert ( hashmap, &key, &val, NULL );

        key = "foo"; EX_TEST ( *((float*)ex_hashmap_get ( hashmap, &key, NULL )) == 1.0f );
        key = "bar"; EX_TEST ( *((float*)ex_hashmap_get ( hashmap, &key, NULL )) == 2.0f );
        key = "hello"; EX_TEST ( *((float*)ex_hashmap_get ( hashmap, &key, NULL )) == 3.0f );
        key = "world"; EX_TEST ( *((float*)ex_hashmap_get ( hashmap, &key, NULL )) == 4.0f );

        EX_TEST ( ex_hashmap_len(hashmap) == 4 );

        ex_hashmap_free ( hashmap );
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void invalid ()
{
    ex_hashmap_t* hashmap = ex_hashmap_alloc ( sizeof(uint32), sizeof(float), 10, ex_hashkey_uint32, ex_keycmp_uint32 );
    ex_hashmap_free ( hashmap );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_hashmap ()
{
    normal();
    invalid();
}
