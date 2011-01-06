// ======================================================================================
// File         : pool.c
// Author       : Wu Jie 
// Last Change  : 06/26/2010 | 14:15:26 PM | Saturday,June
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "pool.h"

///////////////////////////////////////////////////////////////////////////////
// static defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
static ex_pool_node_t* __request_free_node( ex_pool_t* _pool ) 
{
    ex_pool_node_t* node = NULL;

    if ( _pool->_free_nodes == NULL ) {
        ex_pool_reserve( _pool, _pool->_capacity * 2 );
    }

    node = _pool->_free_nodes;
    _pool->_free_nodes->next = NULL;
    _pool->_free_nodes = _pool->_free_nodes->prev;

    return node;
}

// no managed
static ex_pool_node_t* __request_free_node_nomng( ex_pool_t* _pool ) 
{
    ex_pool_node_t* node = NULL;

    if ( _pool->_free_nodes == NULL ) {
        ex_pool_reserve_nomng( _pool, _pool->_capacity * 2 );
    }

    node = _pool->_free_nodes;
    _pool->_free_nodes->next = NULL;
    _pool->_free_nodes = _pool->_free_nodes->prev;

    return node;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __push_to_used ( ex_pool_t* _pool, ex_pool_node_t* _node )
{
    if ( _pool->_used_nodes_end ) { // if we have used nodes
        _pool->_used_nodes_end->next = _node;
        _node->prev = _pool->_used_nodes_end;
        _node->next = NULL;
        _pool->_used_nodes_end = _node;
    }
    else { // if it is the first one of the used nodes
        _pool->_used_nodes_begin = _pool->_used_nodes_end = _node;
        _pool->_used_nodes_end->next = NULL;
        _pool->_used_nodes_end->prev = NULL;
    }
    ex_bitarray_set( _pool->_used_bits, _node - _pool->_nodes, 1 );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __push_to_used_reverse ( ex_pool_t* _pool, ex_pool_node_t* _node )
{
    if ( _pool->_used_nodes_begin ) { // if we have used nodes
        _pool->_used_nodes_begin->prev = _node;
        _node->next = _pool->_used_nodes_begin;
        _node->prev = NULL;
        _pool->_used_nodes_begin = _node;
    }
    else { // if it is the first one of the used nodes
        _pool->_used_nodes_begin = _pool->_used_nodes_end = _node;
        _pool->_used_nodes_end->next = NULL;
        _pool->_used_nodes_end->prev = NULL;
    }
    ex_bitarray_set( _pool->_used_bits, _node - _pool->_nodes, 1 );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __push_to_free ( ex_pool_t* _pool, ex_pool_node_t* _node )
{
    if ( _pool->_free_nodes ) { // if we have used nodes
        _pool->_free_nodes->next = _node;
        _node->prev = _pool->_free_nodes;
        _node->next = NULL;
        _pool->_free_nodes = _node;
    }
    else { // if it is the first one of the used nodes
        _pool->_free_nodes = _node;
        _pool->_free_nodes->next = NULL;
        _pool->_free_nodes->prev = NULL;
    }
    ex_bitarray_set( _pool->_used_bits, _node - _pool->_nodes, 0 );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void* __remove_at ( ex_pool_t* _pool, int _idx ) {
    ex_pool_node_t* node = NULL;

    // get current node.
    node = _pool->_nodes + _idx;

    // re-locate the used_nodes begin and end
    if ( node == _pool->_used_nodes_begin ) {
        _pool->_used_nodes_begin = node->next;
    } 
    if ( node == _pool->_used_nodes_end ) {
        _pool->_used_nodes_end = node->prev;
    } 

    // unlink the node from used node.
    if ( node->prev )
        node->prev->next = node->next;
    if ( node->next )
        node->next->prev = node->prev;

    // return it to free_nodes
    __push_to_free( _pool, node );
    --_pool->_length;

    return (char*)(_pool->_data) + _idx * _pool->_element_bytes;
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
ex_pool_t* ex_pool_alloc ( size_t _element_bytes, size_t _count )
{
    ex_pool_t* pool = ex_malloc ( sizeof(ex_pool_t) );
    int size = _element_bytes * _count;
    size_t i = 1;

    // init members
    pool->_length = 0;
    pool->_capacity = _count;
    pool->_element_bytes = _element_bytes;

    // init data
    pool->_data = ex_malloc( size );
    ex_memzero ( pool->_data, size );

    // init nodes
    pool->_nodes = ex_malloc( sizeof(ex_pool_node_t) * _count );
    pool->_used_nodes_begin = NULL;
    pool->_used_nodes_end = NULL;
    pool->_free_nodes = pool->_nodes + (_count-1);
    pool->_used_bits = ex_bitarray_alloc( _count );

    // init head node
    pool->_free_nodes->prev = NULL;
    pool->_free_nodes->next = pool->_free_nodes-1;
    --pool->_free_nodes;

    // init other node
    while ( i < _count ) {
        pool->_free_nodes->prev = pool->_free_nodes+1;

        if ( i == _count - 1 ) {
            pool->_free_nodes->next = NULL;
        }
        else {
            pool->_free_nodes->next = pool->_free_nodes-1;
            --pool->_free_nodes;
        }
        ++i;
    }

    return pool;
}

// no managed
ex_pool_t* ex_pool_alloc_nomng ( size_t _element_bytes, size_t _count )
{
    ex_pool_t* pool = ex_malloc_nomng ( sizeof(ex_pool_t) );
    int size = _element_bytes * _count;
    size_t i = 1;

    // init public members
    pool->_length = 0;
    pool->_capacity = _count;
    pool->_element_bytes = _element_bytes;

    // init data
    pool->_data = ex_malloc_nomng( size );
    ex_memzero ( pool->_data,  size );

    // init nodes
    pool->_nodes = ex_malloc_nomng( sizeof(ex_pool_node_t) * _count );
    pool->_used_nodes_begin = NULL;
    pool->_used_nodes_end = NULL;
    pool->_free_nodes = pool->_nodes + (_count-1);
    pool->_used_bits = ex_bitarray_alloc_nomng( _count );

    // init head node
    pool->_free_nodes->prev = NULL;
    pool->_free_nodes->next = pool->_free_nodes-1;
    --pool->_free_nodes;

    // init other node
    while ( i < _count ) {
        pool->_free_nodes->prev = pool->_free_nodes+1;

        if ( i == _count - 1 ) {
            pool->_free_nodes->next = NULL;
        }
        else {
            pool->_free_nodes->next = pool->_free_nodes-1;
            --pool->_free_nodes;
        }
        ++i;
    }

    return pool;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
void ex_pool_free ( ex_pool_t* _pool )
{
    ex_assert_return( _pool != NULL, /*void*/, "NULL input" );

    ex_free(_pool->_data);
    ex_free(_pool->_nodes);
    ex_bitarray_free( _pool->_used_bits );

    _pool->_length = 0;
    _pool->_element_bytes = 0;
    _pool->_capacity = 0;
    ex_free(_pool);
}

// no managed
void ex_pool_free_nomng ( ex_pool_t* _pool )
{
    ex_assert_return( _pool != NULL, /*void*/, "NULL input" );

    ex_free_nomng(_pool->_data);
    ex_free_nomng(_pool->_nodes);
    ex_bitarray_free_nomng( _pool->_used_bits );

    _pool->_length = 0;
    _pool->_element_bytes = 0;
    _pool->_capacity = 0;
    ex_free_nomng(_pool);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
void ex_pool_reserve ( ex_pool_t* _pool, size_t _count ) 
{
    size_t size = _count * _pool->_element_bytes;
    int i = _count - 1;

    // we don't process resizing if the new size is small than the _capacity
    if ( _count <= _pool->_capacity )
        return;

    //
    _pool->_data = ex_realloc( _pool->_data, size );
    _pool->_nodes = ex_realloc( _pool->_nodes,  sizeof(ex_pool_node_t) * _count  );
    ex_bitarray_resize ( _pool->_used_bits, _count );
    _pool->_capacity = _count;

    _pool->_used_nodes_begin = NULL;
    _pool->_used_nodes_end = NULL;
    _pool->_free_nodes = NULL;

    while ( i >= 0 ) {
        // if curren index is in used.
        if ( ex_bitarray_get ( _pool->_used_bits, i ) ) {
            __push_to_used_reverse ( _pool, _pool->_nodes + i );
        }
        else {
            __push_to_free ( _pool, _pool->_nodes + i );
        }
        --i;
    }
}

// no managed
void ex_pool_reserve_nomng ( ex_pool_t* _pool, size_t _count ) 
{
    size_t size = _count * _pool->_element_bytes;
    int i = _count - 1;

    // we don't process resizing if the new size is small than the _capacity
    if ( _count <= _pool->_capacity )
        return;

    //
    _pool->_data = ex_realloc_nomng( _pool->_data, size );
    _pool->_nodes = ex_realloc_nomng( _pool->_nodes,  sizeof(ex_pool_node_t) * _count  );
    ex_bitarray_resize_nomng ( _pool->_used_bits, _count );
    _pool->_capacity = _count;

    _pool->_used_nodes_begin = NULL;
    _pool->_used_nodes_end = NULL;
    _pool->_free_nodes = NULL;

    while ( i >= 0 ) {
        // if curren index is in used.
        if ( ex_bitarray_get ( _pool->_used_bits, i ) ) {
            __push_to_used_reverse ( _pool, _pool->_nodes + i );
        }
        else {
            __push_to_free ( _pool, _pool->_nodes + i );
        }
        --i;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
int ex_pool_insert ( ex_pool_t* _pool, const void* _value )
{
    ex_pool_node_t* node = NULL;
    int idx = -1;

    node = __request_free_node(_pool);
    ex_assert ( node != NULL, "error: can't get freed nodes." );

    __push_to_used( _pool, node );
    idx = node - _pool->_nodes;

    // if _value is NULL, that means insert an empty node.
    if ( _value )
        memcpy ( (char*)(_pool->_data) + idx * _pool->_element_bytes, _value, _pool->_element_bytes );

    ++_pool->_length;
    
    return idx;
}

// no managed
int ex_pool_insert_nomng ( ex_pool_t* _pool, const void* _value )
{
    ex_pool_node_t* node = NULL;
    int idx = -1;

    node = __request_free_node_nomng(_pool);
    ex_assert ( node != NULL, "error: can't get freed nodes." );

    __push_to_used( _pool, node );
    idx = node - _pool->_nodes;

    // if _value is NULL, that means insert an empty node.
    if ( _value )
        memcpy ( (char*)(_pool->_data) + idx * _pool->_element_bytes, _value, _pool->_element_bytes );

    ++_pool->_length;
    
    return idx;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_pool_isvalid ( const ex_pool_t* _pool, int _idx ) {
    return ex_bitarray_get(_pool->_used_bits, _idx) == 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void* ex_pool_remove_at ( ex_pool_t* _pool, int _idx ) {
    ex_assert_return( _idx >= 0 && _idx < (int)_pool->_capacity, NULL, "error: _idx out of range" );
    ex_assert_return( ex_bitarray_get(_pool->_used_bits, _idx) == 1, NULL, "error: the node is not in used." );

    return __remove_at ( _pool, _idx );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void* ex_pool_remove_at_safe ( ex_pool_t* _pool, int _idx ) {
    if ( _idx < 0 || _idx >= (int)_pool->_capacity )
        return NULL;
    if ( ex_bitarray_get(_pool->_used_bits, _idx) != 1 )
        return NULL;

    return __remove_at ( _pool, _idx );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void* ex_pool_get ( const ex_pool_t* _pool, int _idx )
{
    ex_assert_return( _idx >= 0 && (size_t)_idx < _pool->_capacity, NULL, "error: _idx out of range" );
    ex_assert_return( ex_bitarray_get(_pool->_used_bits, _idx) == 1, NULL, "error: the node is not in used." );

    return (char*)(_pool->_data) + _idx * _pool->_element_bytes;
}
