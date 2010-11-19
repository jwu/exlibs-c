// ======================================================================================
// File         : node.c
// Author       : Wu Jie 
// Last Change  : 07/14/2010 | 21:11:14 PM | Wednesday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "../container/array.h"
#include "node.h"

///////////////////////////////////////////////////////////////////////////////
// private functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void bt_node_add ( bt_node_t* _self, bt_node_t* _node )
{
    ex_array_push_back( _self->children, &_node );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static bt_node_t* _bt_node_alloc ()
{
    bt_node_t* node = ex_malloc( sizeof(bt_node_t) );
    node->children = ex_array_alloc ( sizeof(bt_node_t*), 8 );
    node->user_data = NULL;
    node->exec = NULL;
    node->free = NULL;
    return node;
}

///////////////////////////////////////////////////////////////////////////////
// public functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void bt_node_free ( bt_node_t* _node )
{
    // the array could be NULL;
    if ( _node->children ) {
        // free children nodes
        ex_array_each ( bt_node_t*, child, _node->children ) {
            bt_node_free ( child );
        } ex_array_each_end

        // free array
        if ( _node->children ) {
            ex_array_free (_node->children);
        }
    }

    // if node have custom free function, call it
    if ( _node->free )
        (*(_node->free))(_node);
    ex_free(_node);
}

///////////////////////////////////////////////////////////////////////////////
// implements
///////////////////////////////////////////////////////////////////////////////

#include "sequence.inl"
#include "selector.inl"
#include "parallel.inl"
#include "queue.inl"
