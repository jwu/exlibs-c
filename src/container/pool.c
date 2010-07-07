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
// fwd-decls
///////////////////////////////////////////////////////////////////////////////

static pool_node_t* _request_free_node( pool_t* ); 
static pool_node_t* _request_free_node_nomng( pool_t* ); 
static void _push_to_used ( pool_t*, pool_node_t* );
static void _push_to_free ( pool_t*, pool_node_t* );

///////////////////////////////////////////////////////////////////////////////
// public
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
pool_t* pool_alloc ( size_t _element_bytes, size_t _count )
{
    pool_t* pool = ex_malloc ( sizeof(pool_t) );
    int size = _element_bytes * _count;
    size_t i = 1;

    // init members
    pool->_length = 0;
    pool->_capacity = _count;
    pool->_element_bytes = _element_bytes;

    // init data
    pool->_data = ex_malloc( size );
    memzero ( pool->_data, size );

    // init nodes
    pool->_nodes = ex_malloc( sizeof(pool_node_t) * _count );
    pool->_used_nodes = NULL;
    pool->_free_nodes = pool->_nodes + (_count-1);
    pool->_used_bits = bitarray_alloc( _count );

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
pool_t* pool_alloc_nomng ( size_t _element_bytes, size_t _count )
{
    pool_t* pool = ex_malloc_nomng ( sizeof(pool_t) );
    int size = _element_bytes * _count;
    size_t i = 1;

    // init public members
    pool->_length = 0;
    pool->_capacity = _count;
    pool->_element_bytes = _element_bytes;

    // init data
    pool->_data = ex_malloc_nomng( size );
    memzero ( pool->_data,  size );

    // init nodes
    pool->_nodes = ex_malloc_nomng( sizeof(pool_node_t) * _count );
    pool->_used_nodes = NULL;
    pool->_free_nodes = pool->_nodes + (_count-1);
    pool->_used_bits = bitarray_alloc_nomng( _count );

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
void pool_free ( pool_t* _pool )
{
    ex_free(_pool->_data);
    ex_free(_pool->_nodes);
    bitarray_free( _pool->_used_bits );

    _pool->_length = 0;
    _pool->_element_bytes = 0;
    _pool->_capacity = 0;
    ex_free(_pool);
}

// no managed
void pool_free_nomng ( pool_t* _pool )
{
    ex_free_nomng(_pool->_data);
    ex_free_nomng(_pool->_nodes);
    bitarray_free_nomng( _pool->_used_bits );

    _pool->_length = 0;
    _pool->_element_bytes = 0;
    _pool->_capacity = 0;
    ex_free_nomng(_pool);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
void pool_reserve ( pool_t* _pool, size_t _count ) 
{
    size_t size = _count * _pool->_element_bytes;
    size_t i = _count-1;

    // we don't process resizing if the new size is small than the _capacity
    if ( _count <= _pool->_capacity )
        return;

    //
    _pool->_data = ex_realloc( _pool->_data, size );
    _pool->_nodes = ex_realloc( _pool->_nodes,  sizeof(pool_node_t) * _count  );
    bitarray_resize ( _pool->_used_bits, _count );
    _pool->_capacity = _count;

    _pool->_used_nodes = NULL;
    _pool->_free_nodes = NULL;

    while ( i > 0 ) {
        // if curren index is in used.
        if ( bitarray_get ( _pool->_used_bits, i ) ) {
            _push_to_used ( _pool, _pool->_nodes + i );
        }
        else {
            _push_to_free ( _pool, _pool->_nodes + i );
        }
        --i;
    }
}

// no managed
void pool_reserve_nomng ( pool_t* _pool, size_t _count ) 
{
    size_t size = _count * _pool->_element_bytes;
    size_t i = _count-1;

    // we don't process resizing if the new size is small than the _capacity
    if ( _count <= _pool->_capacity )
        return;

    //
    _pool->_data = ex_realloc_nomng( _pool->_data, size );
    _pool->_nodes = ex_realloc_nomng( _pool->_nodes,  sizeof(pool_node_t) * _count  );
    bitarray_resize_nomng ( _pool->_used_bits, _count );
    _pool->_capacity = _count;

    _pool->_used_nodes = NULL;
    _pool->_free_nodes = NULL;

    while ( i > 0 ) {
        // if curren index is in used.
        if ( bitarray_get ( _pool->_used_bits, i ) ) {
            _push_to_used ( _pool, _pool->_nodes + i );
        }
        else {
            _push_to_free ( _pool, _pool->_nodes + i );
        }
        --i;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
size_t pool_insert ( pool_t* _pool, void* _value )
{
    pool_node_t* node = NULL;
    void* element = NULL;
    int idx = -1;

    node = _request_free_node(_pool);
    ex_assert ( node != NULL, "error: can't get freed nodes." );

    _push_to_used( _pool, node );
    idx = node - _pool->_nodes;

    // if _value is NULL, that means insert an empty node.
    if ( _value )
        memcpy ( (char*)(_pool->_data) + idx * _pool->_element_bytes, _value, _pool->_element_bytes );

    ++_pool->_length;
    
    return idx;
}

// no managed
size_t pool_insert_nomng ( pool_t* _pool, void* _value )
{
    pool_node_t* node = NULL;
    void* element = NULL;
    int idx = -1;

    node = _request_free_node_nomng(_pool);
    ex_assert ( node != NULL, "error: can't get freed nodes." );

    _push_to_used( _pool, node );
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

void* pool_erase ( pool_t* _pool, int _idx )
{
    pool_node_t* node = NULL;

    ex_assert_return( _idx >= 0 && _idx < (int)_pool->_capacity, NULL, "error: _idx out of range" );
    ex_assert_return( bitarray_get(_pool->_used_bits, _idx) == 1, NULL, "error: the node is not in used." );

    // get current node.
    node = _pool->_nodes + _idx;

    // if this is the end of used_nodes
    // re-locate the used_nodes head
    if ( node == _pool->_used_nodes ) {
        _pool->_used_nodes = node->prev;
    }

    // unlink the node from used node.
    if ( node->prev )
        node->prev->next = node->next;
    if ( node->next )
        node->next->prev = node->prev;

    // return it to free_nodes
    _push_to_free( _pool, node );
    --_pool->_length;

    return (char*)(_pool->_data) + _idx * _pool->_element_bytes;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void* pool_get ( pool_t* _pool, size_t _idx )
{
    ex_assert_return( _idx >= 0 && _idx < _pool->_capacity, NULL, "error: _idx out of range" );
    ex_assert_return( bitarray_get(_pool->_used_bits, _idx) == 1, NULL, "error: the node is not in used." );

    return (char*)(_pool->_data) + _idx * _pool->_element_bytes;
}

///////////////////////////////////////////////////////////////////////////////
// private
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
pool_node_t* _request_free_node( pool_t* _pool ) 
{
    pool_node_t* node = NULL;

    if ( _pool->_free_nodes == NULL ) {
        pool_reserve( _pool, _pool->_capacity * 2 );
    }

    node = _pool->_free_nodes;
    _pool->_free_nodes->next = NULL;
    _pool->_free_nodes = _pool->_free_nodes->prev;

    return node;
}

// no managed
pool_node_t* _request_free_node_nomng( pool_t* _pool ) 
{
    pool_node_t* node = NULL;

    if ( _pool->_free_nodes == NULL ) {
        pool_reserve_nomng( _pool, _pool->_capacity * 2 );
    }

    node = _pool->_free_nodes;
    _pool->_free_nodes->next = NULL;
    _pool->_free_nodes = _pool->_free_nodes->prev;

    return node;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void _push_to_used ( pool_t* _pool, pool_node_t* _node )
{
    if ( _pool->_used_nodes ) { // if we have used nodes
        _pool->_used_nodes->next = _node;
        _node->prev = _pool->_used_nodes;
        _node->next = NULL;
        _pool->_used_nodes = _node;
    }
    else { // if it is the first one of the used nodes
        _pool->_used_nodes = _node;
        _pool->_used_nodes->next = NULL;
        _pool->_used_nodes->prev = NULL;
    }
    bitarray_set( _pool->_used_bits, _node - _pool->_nodes, 1 );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void _push_to_free ( pool_t* _pool, pool_node_t* _node )
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
    bitarray_set( _pool->_used_bits, _node - _pool->_nodes, 0 );
}
