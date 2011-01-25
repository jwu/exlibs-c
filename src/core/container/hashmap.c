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
#include "core/algo/bit.h"
#include "core/math/mathop.h"

///////////////////////////////////////////////////////////////////////////////
// fwd-decls
///////////////////////////////////////////////////////////////////////////////

static inline uint32 __hash_index ( const ex_hashmap_t *_hashmap, const void *_key ) {
    return _hashmap->hashkey(_key) & ( _hashmap->hashsize - 1 );
}

static inline ex_hashmap_node_t *__getnode ( const ex_hashmap_t *_hashmap, size_t _idx ) {
    return (ex_hashmap_node_t *)ex_pool_get(_hashmap->nodes,_idx);
}

static inline void *__getkey ( const ex_hashmap_t *_hashmap, size_t _idx ) {
    return (char *)_hashmap->keys + _idx * _hashmap->key_bytes;
}

static inline void *__getvalue ( const ex_hashmap_t *_hashmap, size_t _idx ) {
    return (char *)_hashmap->values + _idx * _hashmap->value_bytes;
}

///////////////////////////////////////////////////////////////////////////////
// public
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
ex_hashmap_t *ex_hashmap_alloc ( strid_t _key_typeid, size_t _key_bytes, 
                                 strid_t _value_typeid, size_t _value_bytes, 
                                 size_t _count, 
                                 hashkey_t _hashkey, keycmp_t _keycmp )
{
    ex_hashmap_t *hashmap = ex_malloc( sizeof(ex_hashmap_t) );
    ex_hashmap_init ( hashmap, 
                      _key_typeid, _key_bytes, 
                      _value_typeid, _value_bytes, 
                      _count,
                      _hashkey, _keycmp,
                      __ex_hashmap_alloc,
                      __ex_hashmap_realloc,
                      __ex_hashmap_dealloc
                      );
    return hashmap;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_hashmap_init ( ex_hashmap_t *_hashmap, 
                       strid_t _key_typeid, 
                       size_t _key_bytes, 
                       strid_t _value_typeid, 
                       size_t _value_bytes, 
                       size_t _count, 
                       hashkey_t _hashkey, 
                       keycmp_t _keycmp, 
                       void *(*_alloc) ( size_t ),
                       void *(*_realloc) ( void *, size_t ),
                       void  (*_dealloc) ( void * )
                     ) 
{
    // check if the count is pow of 2, if not, calc the nearest pow of 2 of the count.
    ex_assert_exec( ex_is_pow_of_2(_count), 
                    _count = ex_ceilpow2u(_count), 
                    "hash count must be power of 2, for bit operation & instead of %" );

    _hashmap->alloc = _alloc;
    _hashmap->realloc = _realloc;
    _hashmap->dealloc = _dealloc;

    // init length, _capacity, element bytes, hash and keycmp callbacks.
    _hashmap->capacity = _count;
    _hashmap->hashsize = _count;
    _hashmap->key_typeid = _key_typeid;
    _hashmap->key_bytes = _key_bytes;
    _hashmap->value_typeid = _value_typeid;
    _hashmap->value_bytes = _value_bytes;
    _hashmap->hashkey = _hashkey;
    _hashmap->keycmp = _keycmp;

    // init keys
    _hashmap->keys = _hashmap->alloc ( _count * _key_bytes );
    ex_memzero ( _hashmap->keys, _count * _key_bytes );

    // init data
    _hashmap->values = _hashmap->alloc ( _count * _value_bytes  );
    ex_memzero ( _hashmap->values, _count * _value_bytes  );

    // init hash indices
    _hashmap->indices = _hashmap->alloc ( _count * sizeof(size_t) );
    memset ( _hashmap->indices, -1, _count * sizeof(size_t) );

    // init hash nodes
    _hashmap->nodes = _hashmap->alloc ( sizeof(ex_pool_t) );
    ex_pool_init ( _hashmap->nodes, EX_STRID_NULL, sizeof(ex_hashmap_node_t), _count, 
                   _alloc,
                   _realloc,
                   _dealloc );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_hashmap_deinit ( ex_hashmap_t *_hashmap ) {
    _hashmap->dealloc (_hashmap->values);
    _hashmap->values = NULL;
    _hashmap->dealloc (_hashmap->keys);
    _hashmap->keys = NULL;
    _hashmap->dealloc (_hashmap->indices);
    _hashmap->indices = NULL;

    ex_pool_deinit(_hashmap->nodes);
    _hashmap->dealloc(_hashmap->nodes);

    _hashmap->key_typeid = EX_STRID_NULL;
    _hashmap->key_bytes = 0;
    _hashmap->value_typeid = EX_STRID_NULL;
    _hashmap->value_bytes = 0;
    _hashmap->capacity = 0;
    _hashmap->hashsize = 0;
    _hashmap->hashkey = NULL;
    _hashmap->keycmp = NULL;

    _hashmap->alloc = NULL;
    _hashmap->realloc = NULL;
    _hashmap->dealloc = NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
void ex_hashmap_free ( ex_hashmap_t *_hashmap )
{
    ex_assert_return( _hashmap != NULL, /*void*/, "NULL input" );
    ex_hashmap_deinit (_hashmap);
    ex_free(_hashmap);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_hashmap_insert_new ( ex_hashmap_t *_hashmap, const void *_key, const void *_val, size_t _hash_idx, size_t *_index )
{
    size_t cur_idx, next_idx;
    ex_hashmap_node_t *node;

    cur_idx = ex_pool_insert ( _hashmap->nodes, NULL );
    if ( cur_idx > _hashmap->capacity-1 ) {
        _hashmap->capacity *= 2;
        _hashmap->values = _hashmap->realloc ( _hashmap->values, _hashmap->capacity * _hashmap->value_bytes  );
        _hashmap->keys = _hashmap->realloc ( _hashmap->keys, _hashmap->capacity * _hashmap->key_bytes );
    }

    next_idx = _hashmap->indices[_hash_idx];

    node = (ex_hashmap_node_t *)ex_pool_get( _hashmap->nodes, cur_idx );
    node->next = next_idx;
    node->prev = -1;
    _hashmap->indices[_hash_idx] = cur_idx;

    //
    if ( next_idx != -1 )
        __getnode(_hashmap,next_idx)->prev = cur_idx;

    // set key and value
    memcpy ( (char *)_hashmap->keys + cur_idx * _hashmap->key_bytes,  _key, _hashmap->key_bytes );
    memcpy ( (char *)_hashmap->values + cur_idx * _hashmap->value_bytes,  _val, _hashmap->value_bytes );

    if ( _index ) *_index = cur_idx;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void *ex_hashmap_get ( const ex_hashmap_t *_hashmap, const void *_key, size_t *_index )
{
    size_t hash_next;
    uint32 hash_idx = __hash_index ( _hashmap, _key ); 

    // check if the key exists. if yes, don't do any thing.
    for ( hash_next = _hashmap->indices[hash_idx]; hash_next != -1; hash_next = __getnode(_hashmap,hash_next)->next )
    {
        // compare the key
        if ( _hashmap->keycmp(_key, __getkey( _hashmap, hash_next ) ) == 0 ) {
            if ( _index ) *_index = hash_next;
            return __getvalue(_hashmap, hash_next);
        }
    }

    //
    if ( _index ) *_index = -1;
    return NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

size_t ex_hashmap_get_hashidx ( const ex_hashmap_t *_hashmap, const void *_key, size_t *_index )
{
    size_t hash_next;
    uint32 hash_idx = __hash_index ( _hashmap, _key ); 

    // check if the key exists. if yes, don't do any thing.
    for ( hash_next = _hashmap->indices[hash_idx]; hash_next != -1; hash_next = __getnode(_hashmap,hash_next)->next )
    {
        // compare the key
        if ( _hashmap->keycmp(_key, __getkey( _hashmap, hash_next ) ) == 0 ) {
            if ( _index ) *_index = hash_next;
            return hash_idx;
        }
    }

    //
    if ( _index ) *_index = -1;
    return hash_idx;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_hashmap_insert ( ex_hashmap_t *_hashmap, const void *_key, const void *_val, size_t *_index )
{
    size_t hash_next;
    uint32 hash_idx = __hash_index ( _hashmap, _key ); 

    // check if the key exists. if yes, don't do any thing.
    for ( hash_next = _hashmap->indices[hash_idx]; hash_next != -1; hash_next = __getnode(_hashmap,hash_next)->next )
    {
        // compare the key
        if ( _hashmap->keycmp(_key, __getkey( _hashmap, hash_next ) ) == 0 ) {
            if ( _index ) *_index = hash_next;
            return false;
        }
    }

    ex_hashmap_insert_new( _hashmap, _key, _val, hash_idx, _index );
    return true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_hashmap_cpy ( ex_hashmap_t *_to, const ex_hashmap_t *_from ) {
    // TODO:
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void *ex_hashmap_remove_at ( ex_hashmap_t *_hashmap, const void *_key )
{
    size_t hash_next;
    uint32 hash_idx = __hash_index ( _hashmap, _key ); 
    size_t prev_idx, next_idx;
    ex_hashmap_node_t *node;

    // check if the key exists. if yes, don't do any thing.
    for ( hash_next = _hashmap->indices[hash_idx]; hash_next != -1; hash_next = __getnode(_hashmap,hash_next)->next )
    {
        // compare the key
        if ( _hashmap->keycmp(_key, __getkey( _hashmap, hash_next ) ) == 0 ) {
            break;
        }
    }

    if ( hash_next == -1 ) {
        ex_warning ( "the key is not found!" );
        NULL;
    }

    //
    prev_idx = __getnode(_hashmap,hash_next)->prev;
    next_idx = __getnode(_hashmap,hash_next)->next;

    // if the erase node is not the tail
    if ( next_idx != -1 ) {
        __getnode (_hashmap,next_idx)->prev = prev_idx;
    }

    // if the erase node is the header, change the index
    if ( prev_idx == -1 )
        _hashmap->indices[hash_idx] = next_idx;
    // if not header
    else 
        __getnode(_hashmap,prev_idx)->next = next_idx;

    // now erase node
    node = ex_pool_remove_at ( _hashmap->nodes, hash_next );
    node->next = -1;
    node->prev = -1;
    return __getvalue ( _hashmap, hash_next );
}
