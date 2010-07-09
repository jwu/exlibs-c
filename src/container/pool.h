// ======================================================================================
// File         : pool.h
// Author       : Wu Jie 
// Last Change  : 06/26/2010 | 14:15:10 PM | Saturday,June
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef POOL_H_1277532911
#define POOL_H_1277532911
// #################################################################################

// Purpose: pool_t is index fixed array 
//          When erase a item from pool_t, the index of the other item will not changed.

///////////////////////////////////////////////////////////////////////////////
// decls
///////////////////////////////////////////////////////////////////////////////

#include "bitarray.h"

///////////////////////////////////////////////////////////////////////////////
// foreach
///////////////////////////////////////////////////////////////////////////////

/**
Usage: 

    pool_each ( uint32, i, &pool ) {
        ...
    } pool_each_end;
*/

#define pool_each( _type, _el, _pool ) \
    { \
        pool_node_t* node = (_pool)->_used_nodes_begin; \
        _type _el; \
        while ( node ) { \
            _el = *( (_type*) pool_get( _pool, node - (_pool)->_nodes ) );

#define pool_each_end \
            node = node->next; \
        } \
    }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

//
typedef struct pool_node_t {
    struct pool_node_t* prev;
    struct pool_node_t* next;
} pool_node_t;

//
typedef struct pool_t {
    // private
    size_t _length;
    size_t _capacity;
    size_t _element_bytes;
    void* _data;
    pool_node_t* _nodes;
    bitarray_t* _used_bits;

    pool_node_t* _used_nodes_begin;
    pool_node_t* _used_nodes_end;
    pool_node_t* _free_nodes;
} pool_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

pool_t* pool_alloc ( size_t _element_bytes, size_t _count );
pool_t* pool_alloc_nomng ( size_t _element_bytes, size_t _count );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void pool_free ( pool_t* _pool );
void pool_free_nomng ( pool_t* _pool );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void pool_reserve ( pool_t* _pool, size_t _count ); 
void pool_reserve_nomng ( pool_t* _pool, size_t _count ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline size_t pool_len ( pool_t* _pool ) { return _pool->_length; }
inline size_t pool_capacity ( pool_t* _pool ) { return _pool->_capacity; }

// ------------------------------------------------------------------ 
// Desc: 
// @param _value: if NULL, means insert an empty node.
// ------------------------------------------------------------------ 

int pool_insert ( pool_t* _pool, void* _value );
int pool_insert_nomng ( pool_t* _pool, void* _value );

inline int pool_insert_int8 ( pool_t* _pool, int8 _value ) { return pool_insert ( _pool, &_value ); }
inline int pool_insert_int16 ( pool_t* _pool, int16 _value ) { return pool_insert ( _pool, &_value ); }
inline int pool_insert_int32 ( pool_t* _pool, int32 _value ) { return pool_insert ( _pool, &_value ); }
inline int pool_insert_int64 ( pool_t* _pool, int64 _value ) { return pool_insert ( _pool, &_value ); }

inline int pool_insert_uint8 ( pool_t* _pool, uint8 _value ) { return pool_insert ( _pool, &_value ); }
inline int pool_insert_uint16 ( pool_t* _pool, uint16 _value ) { return pool_insert ( _pool, &_value ); }
inline int pool_insert_uint32 ( pool_t* _pool, uint32 _value ) { return pool_insert ( _pool, &_value ); }
inline int pool_insert_uint64 ( pool_t* _pool, uint64 _value ) { return pool_insert ( _pool, &_value ); }

inline int pool_insert_float ( pool_t* _pool, float _value ) { return pool_insert ( _pool, &_value ); }
inline int pool_insert_double ( pool_t* _pool, double _value ) { return pool_insert ( _pool, &_value ); }

inline int pool_insert_string ( pool_t* _pool, char* _value ) { return pool_insert ( _pool, &_value ); }
inline int pool_insert_wstring ( pool_t* _pool, wchar_t* _value ) { return pool_insert ( _pool, &_value ); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void* pool_get ( pool_t* _pool, int _idx );

inline int8 pool_get_int8 ( pool_t* _pool, int _idx ) { return *((int8*)pool_get ( _pool, _idx )); }
inline int16 pool_get_int16 ( pool_t* _pool, int _idx ) { return *((int16*)pool_get ( _pool, _idx )); }
inline int32 pool_get_int32 ( pool_t* _pool, int _idx ) { return *((int32*)pool_get ( _pool, _idx )); }
inline int64 pool_get_int64 ( pool_t* _pool, int _idx ) { return *((int64*)pool_get ( _pool, _idx )); }

inline uint8 pool_get_uint8 ( pool_t* _pool, int _idx ) { return *((uint8*)(pool_get ( _pool, _idx ))); }
inline uint16 pool_get_uint16 ( pool_t* _pool, int _idx ) { return *((uint16*)pool_get ( _pool, _idx )); }
inline uint32 pool_get_uint32 ( pool_t* _pool, int _idx ) { return *((uint32*)pool_get ( _pool, _idx )); }
inline uint64 pool_get_uint64 ( pool_t* _pool, int _idx ) { return *((uint64*)pool_get ( _pool, _idx )); }

inline float pool_get_float ( pool_t* _pool, int _idx ) { return *((float*)pool_get ( _pool, _idx )); }
inline double pool_get_double ( pool_t* _pool, int _idx ) { return *((double*)pool_get ( _pool, _idx )); }

inline char* pool_get_string ( pool_t* _pool, int _idx ) { return *((char**)pool_get ( _pool, _idx )); }
inline wchar_t* pool_get_wstring ( pool_t* _pool, int _idx ) { return *((wchar_t**)pool_get ( _pool, _idx )); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void* pool_erase ( pool_t* _pool, int _idx );

// #################################################################################
#endif // END POOL_H_1277532911
// #################################################################################


