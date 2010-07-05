// ======================================================================================
// File         : hashmap.c
// Author       : Wu Jie 
// Last Change  : 06/28/2010 | 08:54:19 AM | Monday,June
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "hashmap.h"
#include "../algo/bit.h"
#include "../math/mathop.h"

///////////////////////////////////////////////////////////////////////////////
// fwd-decls
///////////////////////////////////////////////////////////////////////////////

static inline uint32 _hash_index ( hashmap_t* _hashmap, void* _key ) {
    return _hashmap->_hashkey(_key) & ( _hashmap->_hashsize - 1 );
}

static inline hashmap_node_t* _getnode ( hashmap_t* _hashmap, size_t _idx ) {
    return (hashmap_node_t*)pool_get(_hashmap->_nodes,_idx);
}

static inline void* _getkey ( hashmap_t* _hashmap, size_t _idx ) {
    return (char*)_hashmap->_keys + _idx * _hashmap->_key_bytes;
}

static inline void* _getvalue ( hashmap_t* _hashmap, size_t _idx ) {
    return (char*)_hashmap->_values + _idx * _hashmap->_value_bytes;
}

///////////////////////////////////////////////////////////////////////////////
// public
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
hashmap_t* hashmap_alloc ( size_t _key_bytes, 
                           size_t _value_bytes, 
                           size_t _count, 
                           hashkey_t _hashkey, 
                           keycmp_t _keycmp )
{
    hashmap_t* hashmap = ex_malloc( sizeof(hashmap_t) );
    size_t i = 0;

    // check if the count is pow of 2, if not, calc the nearest pow of 2 of the count.
    ex_assert_exec( is_pow_of_2(_count), 
                    _count = ceilpow2u(_count), 
                    "hash count must be power of 2, for bit operation & instead of %" );

    // init length, _capacity, element bytes, hash and keycmp callbacks.
    hashmap->_capacity = _count;
    hashmap->_hashsize = _count;
    hashmap->_key_bytes = _key_bytes;
    hashmap->_value_bytes = _value_bytes;
    hashmap->_hashkey = _hashkey;
    hashmap->_keycmp = _keycmp;

    // init data
    hashmap->_values = ex_malloc ( _count * _value_bytes  );
    memzero ( hashmap->_values, _count * _value_bytes  );

    // init keys
    hashmap->_keys = ex_malloc ( _count * _key_bytes );
    memzero ( hashmap->_keys, _count * _key_bytes );

    // init hash indices
    hashmap->_indices = ex_malloc ( _count * sizeof(size_t) );
    memset ( hashmap->_indices, -1, _count * sizeof(size_t) );

    // init hash nodes
    hashmap->_nodes = pool_alloc ( sizeof(hashmap_node_t), _count );

    //
    return hashmap;
}

// no managed
hashmap_t* hashmap_alloc_nomng ( size_t _key_bytes, size_t _value_bytes, size_t _count, hashkey_t _hashkey, keycmp_t _keycmp )
{
    hashmap_t* hashmap = ex_malloc_nomng( sizeof(hashmap_t) );
    size_t i = 0;

    // check if the count is pow of 2, if not, calc the nearest pow of 2 of the count.
    ex_assert_exec( is_pow_of_2(_count), 
                    _count = ceilpow2u(_count), 
                    "hash count must be power of 2, for bit operation & instead of %" );

    // init length, _capacity, element bytes, hash and keycmp callbacks.
    hashmap->_capacity = _count;
    hashmap->_hashsize = _count;
    hashmap->_key_bytes = _key_bytes;
    hashmap->_value_bytes = _value_bytes;
    hashmap->_hashkey = _hashkey;
    hashmap->_keycmp = _keycmp;

    // init data
    hashmap->_values = ex_malloc_nomng ( _count * _value_bytes  );
    memzero ( hashmap->_values, _count * _value_bytes  );

    // init keys
    hashmap->_keys = ex_malloc_nomng ( _count * _key_bytes );
    memzero ( hashmap->_keys, _count * _key_bytes );

    // init hash indices
    hashmap->_indices = ex_malloc_nomng ( _count * sizeof(size_t) );
    memset ( hashmap->_indices, -1, _count * sizeof(size_t) );

    // init hash nodes
    hashmap->_nodes = pool_alloc_nomng ( sizeof(hashmap_node_t), _count );

    //
    return hashmap;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
void hashmap_free ( hashmap_t* _hashmap )
{
    ex_free (_hashmap->_values);
    ex_free (_hashmap->_keys);
    ex_free (_hashmap->_indices);
    pool_free(_hashmap->_nodes);

    _hashmap->_capacity = 0;
    _hashmap->_hashsize = 0;
    _hashmap->_value_bytes = 0;
    _hashmap->_hashkey = NULL;
    _hashmap->_keycmp = NULL;
    ex_free(_hashmap);
}

// no managed
void hashmap_free_nomng ( hashmap_t* _hashmap )
{
    ex_free_nomng (_hashmap->_values);
    ex_free_nomng (_hashmap->_keys);
    ex_free_nomng (_hashmap->_indices);
    pool_free_nomng(_hashmap->_nodes);

    _hashmap->_capacity = 0;
    _hashmap->_hashsize = 0;
    _hashmap->_value_bytes = 0;
    _hashmap->_hashkey = NULL;
    _hashmap->_keycmp = NULL;
    ex_free_nomng(_hashmap);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
int hashmap_insert ( hashmap_t* _hashmap, void* _key, void* _val )
{
    size_t hash_next;
    uint32 hash_idx = _hash_index ( _hashmap, _key ); 

    // check if the key exists. if yes, don't do any thing.
    for ( hash_next = _hashmap->_indices[hash_idx]; hash_next != -1; hash_next = _getnode(_hashmap,hash_next)->next )
    {
        // compare the key
        if ( _hashmap->_keycmp(_key, _getkey( _hashmap, hash_next ) ) == 0 ) {
            return -1;
        }
    }

    // process insert
    {
        size_t cur_idx, next_idx;
        hashmap_node_t* node;

        cur_idx = pool_insert ( _hashmap->_nodes, NULL );
        if ( cur_idx > _hashmap->_capacity-1 ) {
            _hashmap->_capacity *= 2;
            _hashmap->_values = ex_realloc ( _hashmap->_values, _hashmap->_capacity * _hashmap->_value_bytes  );
            _hashmap->_keys = ex_realloc ( _hashmap->_keys, _hashmap->_capacity * _hashmap->_key_bytes );
        }

        next_idx = _hashmap->_indices[hash_idx];

        node = (hashmap_node_t*)pool_get( _hashmap->_nodes, cur_idx );
        node->next = next_idx;
        node->prev = -1;
        _hashmap->_indices[hash_idx] = cur_idx;

        //
        if ( next_idx != -1 )
            _getnode(_hashmap,next_idx)->prev = cur_idx;

        // set key and value
        memcpy ( (char*)_hashmap->_keys + cur_idx * _hashmap->_key_bytes,  _key, _hashmap->_key_bytes );
        memcpy ( (char*)_hashmap->_values + cur_idx * _hashmap->_value_bytes,  _val, _hashmap->_value_bytes );

        return cur_idx;
    }
}

// no managed
int hashmap_insert_nomng ( hashmap_t* _hashmap, void* _key, void* _val )
{
    size_t hash_next;
    uint32 hash_idx = _hash_index ( _hashmap, _key ); 

    // check if the key exists. if yes, don't do any thing.
    for ( hash_next = _hashmap->_indices[hash_idx]; hash_next != -1; hash_next = _getnode(_hashmap,hash_next)->next )
    {
        // compare the key
        if ( _hashmap->_keycmp(_key, _getkey( _hashmap, hash_next ) ) == 0 ) {
            return -1;
        }
    }

    // process insert
    {
        size_t cur_idx, next_idx;
        hashmap_node_t* node;

        cur_idx = pool_insert_nomng ( _hashmap->_nodes, NULL );
        if ( cur_idx > _hashmap->_capacity-1 ) {
            _hashmap->_capacity *= 2;
            _hashmap->_values = ex_realloc_nomng ( _hashmap->_values, _hashmap->_capacity * _hashmap->_value_bytes  );
            _hashmap->_keys = ex_realloc_nomng ( _hashmap->_keys, _hashmap->_capacity * _hashmap->_key_bytes );
        }

        next_idx = _hashmap->_indices[hash_idx];

        node = (hashmap_node_t*)pool_get( _hashmap->_nodes, cur_idx );
        node->next = next_idx;
        node->prev = -1;
        _hashmap->_indices[hash_idx] = cur_idx;

        //
        if ( next_idx != -1 )
            _getnode(_hashmap,next_idx)->prev = cur_idx;

        // set key and value
        memcpy ( (char*)_hashmap->_keys + cur_idx * _hashmap->_key_bytes,  _key, _hashmap->_key_bytes );
        memcpy ( (char*)_hashmap->_values + cur_idx * _hashmap->_value_bytes,  _val, _hashmap->_value_bytes );

        return cur_idx;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void* hashmap_get ( hashmap_t* _hashmap, void* _key )
{
    size_t hash_next;
    uint32 hash_idx = _hash_index ( _hashmap, _key ); 

    // check if the key exists. if yes, don't do any thing.
    for ( hash_next = _hashmap->_indices[hash_idx]; hash_next != -1; hash_next = _getnode(_hashmap,hash_next)->next )
    {
        // compare the key
        if ( _hashmap->_keycmp(_key, _getkey( _hashmap, hash_next ) ) == 0 ) {
            return _getvalue(_hashmap, hash_next);
        }
    }

    return NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void* hashmap_erase ( hashmap_t* _hashmap, void* _key )
{
    size_t hash_next;
    uint32 hash_idx = _hash_index ( _hashmap, _key ); 
    size_t prev_idx, next_idx;
    hashmap_node_t* node;

    // check if the key exists. if yes, don't do any thing.
    for ( hash_next = _hashmap->_indices[hash_idx]; hash_next != -1; hash_next = _getnode(_hashmap,hash_next)->next )
    {
        // compare the key
        if ( _hashmap->_keycmp(_key, _getkey( _hashmap, hash_next ) ) == 0 ) {
            break;
        }
    }

    if ( hash_next == -1 ) {
        ex_warning ( "the key is not found!" );
        NULL;
    }

    //
    prev_idx = _getnode(_hashmap,hash_next)->prev;
    next_idx = _getnode(_hashmap,hash_next)->next;

    // if the erase node is not the tail
    if ( next_idx != -1 ) {
        _getnode (_hashmap,next_idx)->prev = prev_idx;
    }

    // if the erase node is the header, change the index
    if ( prev_idx == -1 )
        _hashmap->_indices[hash_idx] = next_idx;
    // if not header
    else 
        _getnode(_hashmap,prev_idx)->next = next_idx;

    // now erase node
    node = pool_erase ( _hashmap->_nodes, hash_next );
    node->next = -1;
    node->prev = -1;
    return _getvalue ( _hashmap, hash_next );
}

///////////////////////////////////////////////////////////////////////////////
// private
///////////////////////////////////////////////////////////////////////////////
