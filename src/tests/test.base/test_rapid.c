// ======================================================================================
// File         : test_rapid.c
// Author       : Wu Jie 
// Last Change  : 07/01/2010 | 09:56:53 AM | Thursday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// include
///////////////////////////////////////////////////////////////////////////////

#include "../../core/core_inc.h"
#include "../../container/hashmap.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

void test_rapid ()
{
    uint indices[256];
    uint i = 0;

    for ( i = 0; i < 256; ++i ) {
        indices[i] = i;
    }

    // shuffle the indices
    for ( i = 0; i < 256-1; ++i ) {
        uint r = i + ( rand() % (256-i) ); // random remaining position.
        uint temp = indices[i]; 
        indices[i] = indices[r]; 
        indices[r] = temp;
    }

    for ( i = 0; i < 256; ++i ) {
        printf ( "%d is %d\n", i, indices[i] );
    }
}
