// ======================================================================================
// File         : test_pool.c
// Author       : Wu Jie 
// Last Change  : 06/27/2010 | 19:57:32 PM | Sunday,June
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

static void normal () {
    //
    ex_pool_t* pool = ex_pool_alloc ( sizeof(int32), 10 );
    {
        // normal insert
        ex_pool_insert_int32 ( pool, 10 );
        ex_pool_insert_int32 ( pool, 20 );
        ex_pool_insert_int32 ( pool, 30 );
        ex_pool_insert_int32 ( pool, 40 );

        EX_TEST( ex_pool_get_int32(pool,0) == 10 );
        EX_TEST( ex_pool_get_int32(pool,1) == 20 );
        EX_TEST( ex_pool_get_int32(pool,2) == 30 );
        EX_TEST( ex_pool_get_int32(pool,3) == 40 );

        {
            int i = 0;
            ex_pool_each ( int32, el, pool ) {
                printf ("[%d] value is %d\n", i, el );
                ++i;
            } ex_pool_each_end
        }

        // insert exceed the capacity
        ex_pool_insert_int32 ( pool, 50 );
        ex_pool_insert_int32 ( pool, 60 );
        ex_pool_insert_int32 ( pool, 70 );
        ex_pool_insert_int32 ( pool, 80 );
        ex_pool_insert_int32 ( pool, 90 );
        ex_pool_insert_int32 ( pool, 100 );
        ex_pool_insert_int32 ( pool, 110 );
        ex_pool_insert_int32 ( pool, 120 );

        EX_TEST( ex_pool_get_int32(pool,0 ) == 10 );
        EX_TEST( ex_pool_get_int32(pool,1 ) == 20 );
        EX_TEST( ex_pool_get_int32(pool,2 ) == 30 );
        EX_TEST( ex_pool_get_int32(pool,3 ) == 40 );
        EX_TEST( ex_pool_get_int32(pool,4 ) == 50 );
        EX_TEST( ex_pool_get_int32(pool,5 ) == 60 );
        EX_TEST( ex_pool_get_int32(pool,6 ) == 70 );
        EX_TEST( ex_pool_get_int32(pool,7 ) == 80 );
        EX_TEST( ex_pool_get_int32(pool,8 ) == 90 );
        EX_TEST( ex_pool_get_int32(pool,9 ) == 100 );
        EX_TEST( ex_pool_get_int32(pool,10) == 110 );
        EX_TEST( ex_pool_get_int32(pool,11) == 120 );

        {
            int i = 0;
            ex_pool_each ( int32, el, pool ) {
                printf ("[%d] value is %d\n", i, el );
                ++i;
            } ex_pool_each_end
        }

        // erase
        ex_pool_remove_at( pool, 4 );
        ex_pool_remove_at( pool, 10 );
        ex_pool_insert_int32( pool, 200 );
        ex_pool_insert_int32( pool, 210 );
        EX_TEST( ex_pool_get_int32(pool,10) == 200 );
        EX_TEST( ex_pool_get_int32(pool,4) == 210 );
    }
    ex_pool_free ( pool );

    //
    pool = ex_pool_alloc ( sizeof(char*), 10 );
    {
        // normal insert
        ex_pool_insert_string ( pool, "hello world!" );
        EX_TEST ( strcmp ( ex_pool_get_string(pool,0), "hello world!" ) == 0 );
    }
    ex_pool_free ( pool );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void invalid() {
    //
    ex_pool_t* pool = ex_pool_alloc ( sizeof(int32), 10 );
    {
        int val = 0;

        // 
        ex_pool_insert_int32 ( pool, 10 );
        ex_pool_insert_int32 ( pool, 20 );
        ex_pool_insert_int32 ( pool, 30 );
        ex_pool_insert_int32 ( pool, 40 );
        ex_pool_insert_int32 ( pool, 50 );
        ex_pool_insert_int32 ( pool, 60 );
        ex_pool_insert_int32 ( pool, 70 );
        ex_pool_insert_int32 ( pool, 80 );
        ex_pool_insert_int32 ( pool, 90 );
        ex_pool_insert_int32 ( pool, 100 );

        // get
        val = ex_pool_get_int32 ( pool, 40 ); // ERROR

        // erase
        ex_pool_remove_at( pool, 40  ); // ERROR
        ex_pool_remove_at( pool, 0  );
        ex_pool_remove_at( pool, 1  );
        ex_pool_remove_at( pool, 2  );
        ex_pool_remove_at( pool, 3  );
        ex_pool_remove_at( pool, 4  );
        ex_pool_remove_at( pool, 4  ); // ERROR
        ex_pool_remove_at( pool, 5  );
        ex_pool_remove_at( pool, 6  );
        ex_pool_remove_at( pool, 7  );
        ex_pool_remove_at( pool, 8  );
        ex_pool_remove_at( pool, 9  );
        ex_pool_remove_at( pool, 10 ); // ERROR
        ex_pool_remove_at( pool, 11 ); // ERROR
        ex_pool_remove_at( pool, 12 ); // ERROR
        ex_pool_remove_at( pool, 13 ); // ERROR
    }
    ex_pool_free ( pool );
}


// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_pool () {
    normal();
    invalid();
}
