// ======================================================================================
// File         : array.c
// Author       : Wu Jie 
// Last Change  : 07/07/2010 | 13:23:51 PM | Wednesday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "array.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
ex_array_t* ex_array_alloc ( size_t _element_bytes, size_t _count )
{
    size_t bytes = _element_bytes * _count; 

    ex_array_t* array = ex_malloc ( sizeof(ex_array_t) );

    // init members
    array->_element_bytes = _element_bytes;
    array->_length = 0;
    array->_capacity = _count;

    // init data
    array->_data = ex_malloc( bytes );
    ex_memzero ( array->_data, bytes );

    //
    return array;
}

// no managed
ex_array_t* ex_array_alloc_nomng ( size_t _element_bytes, size_t _count )
{
    size_t bytes = _element_bytes * _count; 

    ex_array_t* array = ex_malloc_nomng ( sizeof(ex_array_t) );

    // init members
    array->_element_bytes = _element_bytes;
    array->_length = 0;
    array->_capacity = _count;

    // init data
    array->_data = ex_malloc_nomng( bytes );
    ex_memzero ( array->_data, bytes );

    //
    return array;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
void ex_array_free ( ex_array_t* _array )
{
    ex_assert_return( _array != NULL, /*void*/, "NULL input" );

    ex_free(_array->_data);
    ex_free(_array);
}

// no managed
void ex_array_free_nomng ( ex_array_t* _array )
{
    ex_assert_return( _array != NULL, /*void*/, "NULL input" );

    ex_free_nomng(_array->_data);
    ex_free_nomng(_array);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void* ex_array_get ( ex_array_t* _array, size_t _idx )
{
    ex_assert_return( _idx >= 0 && _idx < (int)_array->_length, NULL, "error: _idx out of range" );
    return (char*)(_array->_data) + _idx * _array->_element_bytes;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
void* ex_array_push_back ( ex_array_t* _array, void* _value )
{
    void* val_addr;

    if ( _array->_length >= _array->_capacity ) {
        _array->_capacity *= 2;
        _array->_data = ex_realloc ( _array->_data, _array->_capacity * _array->_element_bytes );
    }

    //
    val_addr = (char*)(_array->_data) + _array->_length * _array->_element_bytes;

    // if _value is NULL, that means insert an empty node.
    if ( _value )
        memcpy ( val_addr, _value, _array->_element_bytes );

    ++_array->_length;
    return val_addr;
}

// no managed
void* ex_array_push_back_nomng ( ex_array_t* _array, void* _value )
{
    void* val_addr;

    if ( _array->_length >= _array->_capacity ) {
        _array->_capacity *= 2;
        _array->_data = ex_realloc_nomng ( _array->_data, _array->_capacity * _array->_element_bytes );
    }

    //
    val_addr = (char*)(_array->_data) + _array->_length * _array->_element_bytes;

    // if _value is NULL, that means insert an empty node.
    if ( _value )
        memcpy ( val_addr, _value, _array->_element_bytes );

    ++_array->_length;
    return val_addr;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// managed
void ex_array_fast_erase ( ex_array_t* _array, size_t _idx )
{
    void *val_addr, *last_val_addr;

    ex_assert_return( _idx >= 0 && _idx < (int)_array->_length, /*dummy*/, "error: _idx out of range" );

    // don't do any thing if we only have one element in the array.
    if ( ex_array_len(_array) != 1 ) {
        // over write the erased element address by fill it with the last element.
        val_addr = (char*)(_array->_data) + _idx * _array->_element_bytes;
        last_val_addr = (char*)(_array->_data) + _array->_length * _array->_element_bytes;
        memcpy ( val_addr, last_val_addr, _array->_element_bytes );
    }

    // count the size
    --_array->_length;

    // TODO: shrink the array by exsdk's memory manager.
}

// no managed
void ex_array_fast_erase_nomng ( ex_array_t* _array, size_t _idx )
{
    void *val_addr, *last_val_addr;

    ex_assert_return( _idx >= 0 && _idx < (int)_array->_length, /*dummy*/, "error: _idx out of range" );

    // don't do any thing if we only have one element in the array.
    if ( ex_array_len(_array) != 1 ) {
        // over write the erased element address by fill it with the last element.
        val_addr = (char*)(_array->_data) + _idx * _array->_element_bytes;
        last_val_addr = (char*)(_array->_data) + _array->_length * _array->_element_bytes;
        memcpy ( val_addr, last_val_addr, _array->_element_bytes );
    }

    // count the size
    --_array->_length;

    // TODO: shrink the array by system memory operation.
}
