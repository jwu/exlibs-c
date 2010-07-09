// ======================================================================================
// File         : hashmap.h
// Author       : Wu Jie 
// Last Change  : 06/28/2010 | 08:03:23 AM | Monday,June
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef HASHMAP_H_1277683404
#define HASHMAP_H_1277683404
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "pool.h"
#include "../algo/hash.h"

///////////////////////////////////////////////////////////////////////////////
// foreach
///////////////////////////////////////////////////////////////////////////////

/**
Usage: 

    hashmap_each ( uint32, i, &hashmap ) {
        ...
    } hashmap_each_end;
*/

#define hashmap_each( _type, _el, _hashmap ) \
    { \
        pool_node_t* node = (_hashmap)->_nodes->_used_nodes_begin; \
        pool_node_t* node_begin = (_hashmap)->_nodes->_nodes; \
        _type _el; \
        while ( node ) { \
            _el = *( (_type*) ( (char*)(_hashmap)->_values + (node - node_begin) * (_hashmap)->_value_bytes ) );

#define hashmap_each_end \
            node = node->next; \
        } \
    }

///////////////////////////////////////////////////////////////////////////////
// decls
// the size of indices, keys, values is fixed when inited.
// the node pool is flexible.
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

//
typedef uint32 (*hashkey_t) (void*);
typedef int (*keycmp_t) (void*,void*);

//
typedef struct hashmap_node_t {
    size_t prev; // prev hash index
    size_t next; // next hash index
} hashmap_node_t;

//
typedef struct hashmap_t {
    // public
    size_t _capacity;

    // private
    size_t _hashsize;
    void* _values;
    size_t _value_bytes;
    void* _keys;
    size_t _key_bytes;
    size_t* _indices;
    pool_t*  _nodes;
    hashkey_t _hashkey;
    keycmp_t _keycmp;
} hashmap_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

hashmap_t* hashmap_alloc ( size_t _key_bytes, size_t _value_bytes, size_t _count, hashkey_t _hashkey, keycmp_t _keycmp );
hashmap_t* hashmap_alloc_nomng ( size_t _key_bytes, size_t _value_bytes, size_t _count, hashkey_t _hashkey, keycmp_t _keycmp );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void hashmap_free ( hashmap_t* _hashmap );
void hashmap_free_nomng ( hashmap_t* _hashmap );

// ------------------------------------------------------------------ 
// Desc: 
//  @return: -1: already exists
// ------------------------------------------------------------------ 

bool hashmap_insert ( hashmap_t* _hashmap, void* _key, void* _val, size_t* _index );
bool hashmap_insert_nomng ( hashmap_t* _hashmap, void* _key, void* _val, size_t* _index );

// TODO: void hashmap_set ( hashmap_t* _hashmap, void* _key, void* _val );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void* hashmap_get ( hashmap_t* _hashmap, void* _key );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void* hashmap_erase ( hashmap_t* _hashmap, void* _key );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline size_t hashmap_len ( hashmap_t* _hashmap ) { return pool_len(_hashmap->_nodes); }
inline size_t hashmap_capacity ( hashmap_t* _hashmap ) { return _hashmap->_capacity; }

///////////////////////////////////////////////////////////////////////////////
// useful callback
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: useful hash functions 
// ------------------------------------------------------------------ 

inline uint32 hashkey_string ( void* _val ) { return hashstr( *((char**)_val) ); }
inline uint32 hashkey_wstring ( void* _val ) { return hashstr_w( *((wchar_t**)_val) ); }
inline uint32 hashkey_uint32 ( void* _val ) { return *((uint32*)_val); }
inline uint32 hashkey_ptr ( void* _val ) { return (uint32)((size_t)(*(void**)_val) >> 4); }

// ------------------------------------------------------------------ 
// Desc: useful keycmp functions
// ------------------------------------------------------------------ 

inline int keycmp_string ( void* _lhs, void* _rhs ) { return strcmp ( *((char**)_lhs), *((char**)_rhs) ); }
inline int keycmp_wstring ( void* _lhs, void* _rhs ) { return wcscmp ( *((wchar_t**)_lhs), *((wchar_t**)_rhs) ); }
inline int keycmp_uint32 ( void* _lhs, void* _rhs ) { return *((uint32*)_lhs) - *((uint32*)_rhs); }
inline int keycmp_ptr ( void* _lhs, void* _rhs ) { return (size_t)*((void**)_lhs) - (size_t)*((void**)_rhs); }

// #################################################################################
#endif // END HASHMAP_H_1277683404
// #################################################################################


