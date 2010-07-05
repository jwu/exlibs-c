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
#include "../../container/pool.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void normal ()
{
    //
    pool_t* pool = pool_alloc ( sizeof(int32), 10 );
    {
        // normal insert
        pool_insert_int32 ( pool, 10 );
        pool_insert_int32 ( pool, 20 );
        pool_insert_int32 ( pool, 30 );
        pool_insert_int32 ( pool, 40 );

        ex_check( pool_get_int32(pool,0) == 10 );
        ex_check( pool_get_int32(pool,1) == 20 );
        ex_check( pool_get_int32(pool,2) == 30 );
        ex_check( pool_get_int32(pool,3) == 40 );

        // insert exceed the capacity
        pool_insert_int32 ( pool, 50 );
        pool_insert_int32 ( pool, 60 );
        pool_insert_int32 ( pool, 70 );
        pool_insert_int32 ( pool, 80 );
        pool_insert_int32 ( pool, 90 );
        pool_insert_int32 ( pool, 100 );
        pool_insert_int32 ( pool, 110 );
        pool_insert_int32 ( pool, 120 );

        ex_check( pool_get_int32(pool,0 ) == 10 );
        ex_check( pool_get_int32(pool,1 ) == 20 );
        ex_check( pool_get_int32(pool,2 ) == 30 );
        ex_check( pool_get_int32(pool,3 ) == 40 );
        ex_check( pool_get_int32(pool,4 ) == 50 );
        ex_check( pool_get_int32(pool,5 ) == 60 );
        ex_check( pool_get_int32(pool,6 ) == 70 );
        ex_check( pool_get_int32(pool,7 ) == 80 );
        ex_check( pool_get_int32(pool,8 ) == 90 );
        ex_check( pool_get_int32(pool,9 ) == 100 );
        ex_check( pool_get_int32(pool,10) == 110 );
        ex_check( pool_get_int32(pool,11) == 120 );

        // erase
        pool_erase( pool, 4 );
        pool_erase( pool, 10 );
        pool_insert_int32( pool, 200 );
        pool_insert_int32( pool, 210 );
        ex_check( pool_get_int32(pool,10) == 200 );
        ex_check( pool_get_int32(pool,4) == 210 );
    }
    pool_free ( pool );

    //
    pool = pool_alloc ( sizeof(char*), 10 );
    {
        // normal insert
        pool_insert_string ( pool, "hello world!" );
        ex_check ( strcmp ( pool_get_string(pool,0), "hello world!" ) == 0 );
    }
    pool_free ( pool );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void invalid()
{
    //
    pool_t* pool = pool_alloc ( sizeof(int32), 10 );
    {
        int val = 0;

        // 
        pool_insert_int32 ( pool, 10 );
        pool_insert_int32 ( pool, 20 );
        pool_insert_int32 ( pool, 30 );
        pool_insert_int32 ( pool, 40 );
        pool_insert_int32 ( pool, 50 );
        pool_insert_int32 ( pool, 60 );
        pool_insert_int32 ( pool, 70 );
        pool_insert_int32 ( pool, 80 );
        pool_insert_int32 ( pool, 90 );
        pool_insert_int32 ( pool, 100 );

        // get
        val = pool_get_int32 ( pool, 40 ); // ERROR

        // erase
        pool_erase( pool, 40  ); // ERROR
        pool_erase( pool, 0  );
        pool_erase( pool, 1  );
        pool_erase( pool, 2  );
        pool_erase( pool, 3  );
        pool_erase( pool, 4  );
        pool_erase( pool, 4  ); // ERROR
        pool_erase( pool, 5  );
        pool_erase( pool, 6  );
        pool_erase( pool, 7  );
        pool_erase( pool, 8  );
        pool_erase( pool, 9  );
        pool_erase( pool, 10 ); // ERROR
        pool_erase( pool, 11 ); // ERROR
        pool_erase( pool, 12 ); // ERROR
        pool_erase( pool, 13 ); // ERROR
    }
    pool_free ( pool );
}


// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_pool ()
{
    normal();
    invalid();
}
