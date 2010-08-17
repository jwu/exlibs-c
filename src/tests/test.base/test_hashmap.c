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
#include "../../container/hashmap.h"

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

        hashmap_t* hashmap = hashmap_alloc ( sizeof(uint32), sizeof(float), 256, hashkey_uint32, keycmp_uint32 );

        for ( i = 0; i < 512; ++i ) {
            key = 10 + i * 10;
            val = 1.0f + i;
            hashmap_insert ( hashmap, &key, &val, NULL );
        }

        i = 0;
        hashmap_each ( float, el, hashmap ) {
            printf ( "[%d] value is %f\n", i, el );
            ++i;
        } hashmap_each_end;

        // check
        for ( i = 0; i < 512; ++i ) {
            key = 10 + i * 10;
            val = 1.0f + i;
            EX_TEST ( *((float*)hashmap_get ( hashmap, &key )) == val );
        }
        EX_TEST ( hashmap_len(hashmap) == 512 );

        // erase
        for ( i = 0; i < 512; ++i ) {
            key = 10 + i * 10;
            EX_TEST ( hashmap_erase(hashmap, &key) != NULL );
        }
        EX_TEST ( hashmap_len(hashmap) == 0 );

        hashmap_free ( hashmap );
    }

    {
        char* key; 
        float val;

        hashmap_t* hashmap = hashmap_alloc ( sizeof(char**), sizeof(float), 256, hashkey_string, keycmp_string );

	    key = "foo"; val = 1.0f; hashmap_insert ( hashmap, &key, &val, NULL );
        key = "bar"; val = 2.0f; hashmap_insert ( hashmap, &key, &val, NULL );
        key = "hello"; val = 3.0f; hashmap_insert ( hashmap, &key, &val, NULL );
        key = "world"; val = 4.0f; hashmap_insert ( hashmap, &key, &val, NULL );

        key = "foo"; EX_TEST ( *((float*)hashmap_get ( hashmap, &key )) == 1.0f );
        key = "bar"; EX_TEST ( *((float*)hashmap_get ( hashmap, &key )) == 2.0f );
        key = "hello"; EX_TEST ( *((float*)hashmap_get ( hashmap, &key )) == 3.0f );
        key = "world"; EX_TEST ( *((float*)hashmap_get ( hashmap, &key )) == 4.0f );

        EX_TEST ( hashmap_len(hashmap) == 4 );

        hashmap_free ( hashmap );
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void invalid ()
{
    hashmap_t* hashmap = hashmap_alloc ( sizeof(uint32), sizeof(float), 10, hashkey_uint32, keycmp_uint32 );
    hashmap_free ( hashmap );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_hashmap ()
{
    normal();
    invalid();
}
