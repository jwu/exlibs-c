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
#include "../../container/list.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void normal () {
    list_t* list = list_alloc ( sizeof(uint32) );
    {
        int i; 
        list_node_t* node = NULL;

        // test push
        for ( i = 0; i < 512; ++i ) {
            list_push_back ( list, &i );
        }
        ex_check( list_len(list) == 512 );

        // check value
        node = list_head(list);
        i = 0;
        while ( node ) {
            ex_check( *((uint32*)node->value) == i );
            node = node->next;
            ++i;
        }

        // test erase
        node = list_head(list);
        while ( node ) {
            node = list_erase ( list, node );
        }
        ex_check( list_len(list) == 0 );
    }
    list_free (list);
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

