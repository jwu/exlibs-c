// ======================================================================================
// File         : test_bitarry.c
// Author       : Wu Jie 
// Last Change  : 06/27/2010 | 08:59:42 AM | Sunday,June
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
    int result = -1;

    ex_bitarray_t* bitArray = ex_bitarray_alloc ( 20 );
    {
        ex_bitarray_set ( bitArray, 0, 1 );
        ex_bitarray_set ( bitArray, 10, 1 );

        result = ex_bitarray_get( bitArray, 0 ); 
        EX_TEST ( result == 1 );

        result = ex_bitarray_get( bitArray, 1 ); 
        EX_TEST ( result == 0 );

        result = ex_bitarray_get( bitArray, 10 ); 
        EX_TEST ( result == 1 );

        ex_bitarray_set ( bitArray, 10, 0 );
        result = ex_bitarray_get( bitArray, 10 ); 
        EX_TEST ( result == 0 );
    }
    ex_bitarray_free ( bitArray );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void invalid ()
{
    ex_bitarray_t* bitArray = ex_bitarray_alloc ( 20 );
    {
        // set out of range
        ex_bitarray_set ( bitArray, 30, 1 );

        // get out of range
        ex_bitarray_set ( bitArray, 50, 1 );

        // set not 0 or 1.
        ex_bitarray_set ( bitArray, 0, 3 );
    }
    ex_bitarray_free ( bitArray );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_bitarry ()
{
    normal ();
    invalid();
}


