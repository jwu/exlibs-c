// ======================================================================================
// File         : bitarray.c
// Author       : Wu Jie 
// Last Change  : 06/26/2010 | 18:31:14 PM | Saturday,June
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "bitarray.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
ex_bitarray_t* ex_bitarray_alloc ( size_t _bitcount )
{
    ex_bitarray_t* bitArray = ex_malloc ( sizeof(ex_bitarray_t) );
    size_t bytes = (_bitcount + 7)/8;

    bitArray->_data = ex_malloc(bytes);
    ex_memzero ( bitArray->_data, bytes );

    bitArray->_length = _bitcount;

    return bitArray;
}

// no managed
ex_bitarray_t* ex_bitarray_alloc_nomng ( size_t _bitcount )
{
    ex_bitarray_t* bitArray = ex_malloc_nomng ( sizeof(ex_bitarray_t) );
    size_t bytes = (_bitcount + 7)/8;

    bitArray->_data = ex_malloc_nomng(bytes);
    bitArray->_length = _bitcount;
    ex_memzero ( bitArray->_data, bytes );

    return bitArray;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
void ex_bitarray_free ( ex_bitarray_t* _bitarray )
{
    ex_assert_return( _bitarray != NULL, /*void*/, "NULL input" );

    ex_free(_bitarray->_data);
    _bitarray->_length = 0;
    ex_free(_bitarray);
}

// no managed
void ex_bitarray_free_nomng ( ex_bitarray_t* _bitarray )
{
    ex_assert_return( _bitarray != NULL, /*void*/, "NULL input" );

    ex_free_nomng(_bitarray->_data);
    _bitarray->_length = 0;
    ex_free_nomng(_bitarray);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_bitarray_get ( const ex_bitarray_t* _bitarray, size_t _idx )
{
    char* ptr;

    ex_assert_return ( _idx >= 0 && _idx < _bitarray->_length, -1, "_idx is out of range" );
    ptr = _bitarray->_data + _idx / 8;
    return ( *ptr & (1 << (_idx % 8) ) ) != 0; /* 0 or 1 */
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_bitarray_set ( ex_bitarray_t* _bitarray, size_t _idx, int _value ) 
{
    char* ptr;

    ex_assert_return ( _idx >= 0 && _idx < _bitarray->_length, /*void*/, "_idx is out of range." );
    ex_assert_return ( _value == 0 || _value == 1, /*void*/, "invalid input value, should be 0 or 1." );

    ptr = _bitarray->_data + _idx / 8;
    if ( _value )
        *ptr |= 1 << ( _idx % 8 ); /*set bit*/
    else
        *ptr &= ~(1 << (_idx % 8)); /*clear bit*/
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
void ex_bitarray_resize ( ex_bitarray_t* _bitarray, size_t _bitcount ) 
{
    size_t old_bytes, new_bytes;

    if ( _bitarray->_length == _bitcount )
        return;

    old_bytes = (_bitarray->_length + 7)/8;
    new_bytes = (_bitcount + 7)/8;

    _bitarray->_data = ex_realloc(_bitarray->_data, new_bytes);
    // if new size is more than the old one, we need to set clear the memory to zero.
    if ( new_bytes > old_bytes )
        ex_memzero ( _bitarray->_data + old_bytes, new_bytes - old_bytes );
    _bitarray->_length = _bitcount;
}

// no managed
void ex_bitarray_resize_nomng ( ex_bitarray_t* _bitarray, size_t _bitcount ) 
{
    size_t old_bytes, new_bytes;

    if ( _bitarray->_length == _bitcount )
        return;

    old_bytes = (_bitarray->_length + 7)/8;
    new_bytes = (_bitcount + 7)/8;

    _bitarray->_data = ex_realloc_nomng(_bitarray->_data, new_bytes);
    // if new size is more than the old one, we need to set clear the memory to zero.
    if ( new_bytes > old_bytes )
        ex_memzero ( _bitarray->_data + old_bytes, new_bytes - old_bytes );
    _bitarray->_length = _bitcount;
}
