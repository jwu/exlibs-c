// ======================================================================================
// File         : test_array.c
// Author       : Wu Jie 
// Last Change  : 07/07/2010 | 13:57:37 PM | Wednesday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "core/core_inc.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void normal () {
    ex_array_t *array = ex_array ( uint32, 20 ); 
    {
        size_t i = 0;
        while ( i != 100 ) {
            ex_array_append_uint32( array, i );
            ++i;
        }
    }
    ex_array_free ( array );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void invalid() {
    ex_array_t *array = ex_array ( uint32, 20 );
    {
        void *pval = ex_array_get( array, 40 );
		(void)pval;
    }
    ex_array_free ( array );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void run_test () {
    normal();
    invalid();
}
