// ======================================================================================
// File         : test_list.c
// Author       : Wu Jie 
// Last Change  : 06/29/2010 | 20:20:39 PM | Tuesday,June
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
    ex_list_t *list = ex_list_alloc ( sizeof(uint32) );
    {
        int i; 
        ex_list_node_t *node = NULL;

        // test push
        for ( i = 0; i < 512; ++i ) {
            ex_list_append ( list, &i );
        }
        EX_TEST( ex_list_count(list) == 512 );

        // check value
        node = ex_list_head(list);
        i = 0;
        while ( node ) {
            EX_TEST( *((uint32 *)node->value) == i );
            node = node->next;
            ++i;
        }

        // test erase
        node = ex_list_head(list);
        while ( node ) {
            node = ex_list_remove_at ( list, node );
        }
        EX_TEST( ex_list_count(list) == 0 );
    }
    ex_list_free (list);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void invalid () {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_list () {
    normal ();
    invalid ();
}

