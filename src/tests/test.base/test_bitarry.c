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
#include "../../container/bitarray.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void normal ()
{
    int result = -1;

    bitarray_t* bitArray = bitarray_alloc ( 20 );
    {
        bitarray_set ( bitArray, 0, 1 );
        bitarray_set ( bitArray, 10, 1 );

        result = bitarray_get( bitArray, 0 ); 
        ex_check ( result == 1 );

        result = bitarray_get( bitArray, 1 ); 
        ex_check ( result == 0 );

        result = bitarray_get( bitArray, 10 ); 
        ex_check ( result == 1 );

        bitarray_set ( bitArray, 10, 0 );
        result = bitarray_get( bitArray, 10 ); 
        ex_check ( result == 0 );
    }
    bitarray_free ( bitArray );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void invalid ()
{
    bitarray_t* bitArray = bitarray_alloc ( 20 );
    {
        // set out of range
        bitarray_set ( bitArray, 30, 1 );

        // get out of range
        bitarray_set ( bitArray, 50, 1 );

        // set not 0 or 1.
        bitarray_set ( bitArray, 0, 3 );
    }
    bitarray_free ( bitArray );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_bitarry ()
{
    normal ();
    invalid();
}


