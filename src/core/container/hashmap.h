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

// ######################### 
#ifdef __cplusplus
extern "C" {
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "pool.h"
#include "core/algo/hash.h"

///////////////////////////////////////////////////////////////////////////////
// foreach
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
/*! 
 @def ex_hashmap_each
 @param _hashmap the in hashmap
 @param _type the type of the value in the hashmap.
 @param _el the value variable you want to define.
 @details macro for easy iterates the hashmap container.

 when use the macro, it will define the local variable below:
 - node: the current node of the hashmap
 - node_begin: the the beginning of the node

 to finish the code, you must write ex_hashmap_each_end.

 Usage:
 @code
 ex_hashmap_t* my_hashmap = ex_hashmap_alloc( sizeof(int), sizeof(float), 10 );
 ex_hashmap_each ( my_hashmap, float, item ) {
    printf( "item_%d is %f", idx, item );
 } ex_hashmap_each_end;
 @endcode
 @sa ex_hashmap_each_end
 @note DO NOT USE continue in this loop, use ex_hashmap_continue instead 
*/// ------------------------------------------------------------------ 

#define ex_hashmap_each( _hashmap, _type, _el ) \
    { \
        ex_pool_node_t* node = (_hashmap)->_nodes->_used_nodes_begin; \
        ex_pool_node_t* node_begin = (_hashmap)->_nodes->_nodes; \
        _type _el; \
        while ( node ) { \
            _el = *( (_type*) ( (char*)(_hashmap)->_values + (node - node_begin) * (_hashmap)->_value_bytes ) );

// ------------------------------------------------------------------ 
/*! 
 @def ex_hashmap_each
 @param _hashmap the in hashmap
 @param _type the point-type of the value in the hashmap.
 @param _el the value variable you want to define.
 @details macro for easy iterates the hashmap container.

 when use the macro, it will define the local variable below:
 - node: the current node of the hashmap
 - node_begin: the the beginning of the node

 to finish the code, you must write ex_hashmap_each_end.

 Usage:
 @code
 ex_hashmap_t* my_hashmap = ex_hashmap_alloc( sizeof(int), sizeof(float), 10 );
 ex_hashmap_raw_each ( my_hashmap, float*, item ) {
    printf( "item_%d is %f", idx, *item );
 } ex_hashmap_each_end;
 @endcode
 @sa ex_hashmap_each_end
 @note DO NOT USE continue in this loop, use ex_hashmap_continue instead 
*/// ------------------------------------------------------------------ 

#define ex_hashmap_raw_each( _hashmap, _type, _el ) \
    { \
        ex_pool_node_t* node = (_hashmap)->_nodes->_used_nodes_begin; \
        ex_pool_node_t* node_begin = (_hashmap)->_nodes->_nodes; \
        _type _el; \
        while ( node ) { \
            _el = (_type)( (char*)(_hashmap)->_values + (node - node_begin) * (_hashmap)->_value_bytes );

// ------------------------------------------------------------------ 
/*! 
 @def ex_hashmap_each_end
 @details macro to end the ex_hashmap_each, ex_hashmap_raw_each macro
 @sa ex_hashmap_each
 @sa ex_hashmap_raw_each
*/// ------------------------------------------------------------------ 

#define ex_hashmap_each_end \
            node = node->next; \
        } \
    }

// ------------------------------------------------------------------ 
/*! 
 @def ex_hashmap_continue
 @details macro to let the continue work in each
 @sa ex_hashmap_each
 @sa ex_hashmap_raw_each
*/// ------------------------------------------------------------------ 

#define ex_hashmap_continue \
    { \
        node = node->next; \
        continue; \
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
typedef uint32 (*hashkey_t) (const void*);
typedef int (*keycmp_t) (const void*, const void*);

//
typedef struct ex_hashmap_node_t {
    size_t prev; // prev hash index
    size_t next; // next hash index
} ex_hashmap_node_t;

//
typedef struct ex_hashmap_t {
    // public
    size_t _capacity;

    // private
    size_t _hashsize;
    void* _values;
    size_t _value_bytes;
    void* _keys;
    size_t _key_bytes;
    size_t* _indices;
    ex_pool_t*  _nodes;
    hashkey_t _hashkey;
    keycmp_t _keycmp;
} ex_hashmap_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern ex_hashmap_t* ex_hashmap_alloc ( size_t _key_bytes, size_t _value_bytes, size_t _count, hashkey_t _hashkey, keycmp_t _keycmp );
extern ex_hashmap_t* ex_hashmap_alloc_nomng ( size_t _key_bytes, size_t _value_bytes, size_t _count, hashkey_t _hashkey, keycmp_t _keycmp );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_hashmap_free ( ex_hashmap_t* _hashmap );
extern void ex_hashmap_free_nomng ( ex_hashmap_t* _hashmap );

// ------------------------------------------------------------------ 
// Desc: 
// NOTE: this operation is deprecated, you need to check if the value 
//  exists then go for it.
// ------------------------------------------------------------------ 

extern void ex_hashmap_insert_new ( ex_hashmap_t* _hashmap, const void* _key, const void* _val, size_t _hash_idx, size_t* _index );
extern void ex_hashmap_insert_new_nomng ( ex_hashmap_t* _hashmap, const void* _key, const void* _val, size_t _hash_idx, size_t* _index );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void* ex_hashmap_get ( const ex_hashmap_t* _hashmap, const void* _key, size_t* _index );
extern size_t ex_hashmap_get_hashidx ( const ex_hashmap_t* _hashmap, const void* _key, size_t* _index );

// ------------------------------------------------------------------ 
// Desc: 
//  @return: -1: already exists
// ------------------------------------------------------------------ 

extern bool ex_hashmap_insert ( ex_hashmap_t* _hashmap, const void* _key, const void* _val, size_t* _index );
extern bool ex_hashmap_insert_nomng ( ex_hashmap_t* _hashmap, const void* _key, const void* _val, size_t* _index );

// TODO: void ex_hashmap_set ( ex_hashmap_t* _hashmap, const void* _key, const void* _val );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void* ex_hashmap_remove_at ( ex_hashmap_t* _hashmap, const void* _key );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline size_t ex_hashmap_len ( const ex_hashmap_t* _hashmap ) { return ex_pool_len(_hashmap->_nodes); }
static inline size_t ex_hashmap_capacity ( const ex_hashmap_t* _hashmap ) { return _hashmap->_capacity; }

///////////////////////////////////////////////////////////////////////////////
// useful callback
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: useful hash functions 
// ------------------------------------------------------------------ 

static inline uint32 ex_hashkey_string ( const void* _val ) { return ex_hashstr( *((const char**)_val) ); }
// DISABLE can use ex_hashkey_string instead: static inline uint32 hashkey_wstring ( void* _val ) { return ex_hashstr_w( *((wchar_t**)_val) ); }
static inline uint32 ex_hashkey_strid ( const void* _val ) { return *((const strid_t*)_val); }
static inline uint32 ex_hashkey_uint32 ( const void* _val ) { return *((const uint32*)_val); }
static inline uint32 ex_hashkey_ptr ( const void* _val ) { return (uint32)((size_t)(*(const void**)_val) >> 4); }

// ------------------------------------------------------------------ 
// Desc: useful keycmp functions
// ------------------------------------------------------------------ 

static inline int ex_keycmp_string ( const void* _lhs, const void* _rhs ) { return strcmp ( *((const char**)_lhs), *((const char**)_rhs) ); }
// DISABLE can use ex_keycmp_string instead: static inline int keycmp_wstring ( void* _lhs, void* _rhs ) { return wcscmp ( *((wchar_t**)_lhs), *((wchar_t**)_rhs) ); }
static inline int ex_keycmp_strid ( const void* _lhs, const void* _rhs ) { return *((const strid_t*)_lhs) - *((const strid_t*)_rhs); }
static inline int ex_keycmp_uint32 ( const void* _lhs, const void* _rhs ) { return *((const uint32*)_lhs) - *((const uint32*)_rhs); }
static inline int ex_keycmp_ptr ( const void* _lhs, const void* _rhs ) { return (size_t)*((const void**)_lhs) - (size_t)*((const void**)_rhs); }

// ######################### 
#ifdef __cplusplus
} // end extern "C"
#endif
// ######################### 

// #################################################################################
#endif // END HASHMAP_H_1277683404
// #################################################################################


