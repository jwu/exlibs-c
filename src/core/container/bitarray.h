// ======================================================================================
// File         : bitarray.h
// Author       : Wu Jie 
// Last Change  : 06/26/2010 | 14:36:05 PM | Saturday,June
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef BITARRAY_H_1277534167
#define BITARRAY_H_1277534167
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" {
#endif
// ######################### 

/**********************************
USAGE:

    ex_bitarray_t* bitArray = ex_bitarray_alloc ( 10 );
    ex_bitarray_free ( bitArray );
***********************************/

// ------------------------------------------------------------------ 
// Desc: ex_bitarray_t 
// ------------------------------------------------------------------ 

typedef struct ex_bitarray_t {
    // private
    size_t _length;
    char* _data;
} ex_bitarray_t; // end struct _bitarray

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_bitarray_t* ex_bitarray_alloc ( size_t _bitcount );
ex_bitarray_t* ex_bitarray_alloc_nomng ( size_t _bitcount );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_bitarray_free ( ex_bitarray_t* _bitarray );
void ex_bitarray_free_nomng ( ex_bitarray_t* _bitarray );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_bitarray_get ( ex_bitarray_t* _bitarray, size_t _idx );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_bitarray_set ( ex_bitarray_t* _bitarray, size_t _idx, int _value );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_bitarray_resize ( ex_bitarray_t* _bitarray, size_t _bitcount ); 
void ex_bitarray_resize_nomng ( ex_bitarray_t* _bitarray, size_t _bitcount ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline size_t ex_bitarray_len ( ex_bitarray_t* _bitarray ) { return _bitarray->_length; } 

// ######################### 
#ifdef __cplusplus
} // end extern "C"
#endif
// ######################### 

// #################################################################################
#endif // END BITARRAY_H_1277534167
// #################################################################################

