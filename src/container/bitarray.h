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

/**********************************
USAGE:

    bitarray_t* bitArray = bitarray_alloc ( 10 );
    bitarray_free ( bitArray );
***********************************/

// ------------------------------------------------------------------ 
// Desc: bitarray_t 
// ------------------------------------------------------------------ 

typedef struct bitarray_ {
    // private
    int _length;
    char* _data;
} bitarray_t; // end struct _bitarray

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bitarray_t* bitarray_alloc ( size_t _bitcount );
bitarray_t* bitarray_alloc_nomng ( size_t _bitcount );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void bitarray_free ( bitarray_t* _bitarray );
void bitarray_free_nomng ( bitarray_t* _bitarray );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int bitarray_get ( bitarray_t* _bitarray, int _idx );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void bitarray_set ( bitarray_t* _bitarray, int _idx, int _value );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void bitarray_resize ( bitarray_t* _bitarray, size_t _bitcount ); 
void bitarray_resize_nomng ( bitarray_t* _bitarray, size_t _bitcount ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline size_t bitarray_len ( bitarray_t* _bitarray ) { return _bitarray->_length; } 

// #################################################################################
#endif // END BITARRAY_H_1277534167
// #################################################################################


