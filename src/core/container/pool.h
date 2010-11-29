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

// ######################### 
#ifdef __cplusplus
extern "C" {
#endif
// ######################### 

// Purpose: ex_pool_t is index fixed array 
//          When erase a item from ex_pool_t, the index of the other item will not changed.

///////////////////////////////////////////////////////////////////////////////
// decls
///////////////////////////////////////////////////////////////////////////////

#include "bitarray.h"

///////////////////////////////////////////////////////////////////////////////
// foreach
///////////////////////////////////////////////////////////////////////////////

/**
Usage: 

    ex_pool_each ( uint32, i, &pool ) {
        ...
    } ex_pool_each_end;
*/

#define ex_pool_each( _type, _el, _pool ) \
    { \
        ex_pool_node_t* node = (_pool)->_used_nodes_begin; \
        _type _el; \
        while ( node ) { \
            _el = *( (_type*) ex_pool_get( _pool, node - (_pool)->_nodes ) );

#define ex_pool_each_end \
            node = node->next; \
        } \
    }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

//
typedef struct ex_pool_node_t {
    struct ex_pool_node_t* prev;
    struct ex_pool_node_t* next;
} ex_pool_node_t;

//
typedef struct ex_pool_t {
    // private
    size_t _length;
    size_t _capacity;
    size_t _element_bytes;
    void* _data;
    ex_pool_node_t* _nodes;
    ex_bitarray_t* _used_bits;

    ex_pool_node_t* _used_nodes_begin;
    ex_pool_node_t* _used_nodes_end;
    ex_pool_node_t* _free_nodes;
} ex_pool_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_pool_t* ex_pool_alloc ( size_t _element_bytes, size_t _count );
ex_pool_t* ex_pool_alloc_nomng ( size_t _element_bytes, size_t _count );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_pool_free ( ex_pool_t* _pool );
void ex_pool_free_nomng ( ex_pool_t* _pool );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_pool_reserve ( ex_pool_t* _pool, size_t _count ); 
void ex_pool_reserve_nomng ( ex_pool_t* _pool, size_t _count ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline size_t ex_pool_len ( ex_pool_t* _pool ) { return _pool->_length; }
inline size_t ex_pool_capacity ( ex_pool_t* _pool ) { return _pool->_capacity; }

// ------------------------------------------------------------------ 
// Desc: 
// @param _value: if NULL, means insert an empty node.
// ------------------------------------------------------------------ 

int ex_pool_insert ( ex_pool_t* _pool, void* _value );
int ex_pool_insert_nomng ( ex_pool_t* _pool, void* _value );

inline int ex_pool_insert_int8 ( ex_pool_t* _pool, int8 _value ) { return ex_pool_insert ( _pool, &_value ); }
inline int ex_pool_insert_int16 ( ex_pool_t* _pool, int16 _value ) { return ex_pool_insert ( _pool, &_value ); }
inline int ex_pool_insert_int32 ( ex_pool_t* _pool, int32 _value ) { return ex_pool_insert ( _pool, &_value ); }
inline int ex_pool_insert_int64 ( ex_pool_t* _pool, int64 _value ) { return ex_pool_insert ( _pool, &_value ); }

inline int ex_pool_insert_uint8 ( ex_pool_t* _pool, uint8 _value ) { return ex_pool_insert ( _pool, &_value ); }
inline int ex_pool_insert_uint16 ( ex_pool_t* _pool, uint16 _value ) { return ex_pool_insert ( _pool, &_value ); }
inline int ex_pool_insert_uint32 ( ex_pool_t* _pool, uint32 _value ) { return ex_pool_insert ( _pool, &_value ); }
inline int ex_pool_insert_uint64 ( ex_pool_t* _pool, uint64 _value ) { return ex_pool_insert ( _pool, &_value ); }

inline int ex_pool_insert_float ( ex_pool_t* _pool, float _value ) { return ex_pool_insert ( _pool, &_value ); }
inline int ex_pool_insert_double ( ex_pool_t* _pool, double _value ) { return ex_pool_insert ( _pool, &_value ); }

inline int ex_pool_insert_string ( ex_pool_t* _pool, char* _value ) { return ex_pool_insert ( _pool, &_value ); }
inline int ex_pool_insert_wstring ( ex_pool_t* _pool, wchar_t* _value ) { return ex_pool_insert ( _pool, &_value ); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void* ex_pool_get ( ex_pool_t* _pool, int _idx );

inline int8 ex_pool_get_int8 ( ex_pool_t* _pool, int _idx ) { return *((int8*)ex_pool_get ( _pool, _idx )); }
inline int16 ex_pool_get_int16 ( ex_pool_t* _pool, int _idx ) { return *((int16*)ex_pool_get ( _pool, _idx )); }
inline int32 ex_pool_get_int32 ( ex_pool_t* _pool, int _idx ) { return *((int32*)ex_pool_get ( _pool, _idx )); }
inline int64 ex_pool_get_int64 ( ex_pool_t* _pool, int _idx ) { return *((int64*)ex_pool_get ( _pool, _idx )); }

inline uint8 ex_pool_get_uint8 ( ex_pool_t* _pool, int _idx ) { return *((uint8*)(ex_pool_get ( _pool, _idx ))); }
inline uint16 ex_pool_get_uint16 ( ex_pool_t* _pool, int _idx ) { return *((uint16*)ex_pool_get ( _pool, _idx )); }
inline uint32 ex_pool_get_uint32 ( ex_pool_t* _pool, int _idx ) { return *((uint32*)ex_pool_get ( _pool, _idx )); }
inline uint64 ex_pool_get_uint64 ( ex_pool_t* _pool, int _idx ) { return *((uint64*)ex_pool_get ( _pool, _idx )); }

inline float ex_pool_get_float ( ex_pool_t* _pool, int _idx ) { return *((float*)ex_pool_get ( _pool, _idx )); }
inline double ex_pool_get_double ( ex_pool_t* _pool, int _idx ) { return *((double*)ex_pool_get ( _pool, _idx )); }

inline char* ex_pool_get_string ( ex_pool_t* _pool, int _idx ) { return *((char**)ex_pool_get ( _pool, _idx )); }
inline wchar_t* ex_pool_get_wstring ( ex_pool_t* _pool, int _idx ) { return *((wchar_t**)ex_pool_get ( _pool, _idx )); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void* ex_pool_erase ( ex_pool_t* _pool, int _idx );

// ######################### 
#ifdef __cplusplus
} // end extern "C"
#endif
// ######################### 

// #################################################################################
#endif // END POOL_H_1277532911
// #################################################################################


