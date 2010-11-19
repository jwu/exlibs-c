// ======================================================================================
// File         : array.h
// Author       : Wu Jie 
// Last Change  : 07/07/2010 | 13:10:55 PM | Wednesday,July
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef ARRAY_H_1278479457
#define ARRAY_H_1278479457
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" {
#endif
// ######################### 

//! @defgroup Container

//! @defgroup ex_array_t dynamic array (like stl::vector)
//! @ingroup Container
//! @{

///////////////////////////////////////////////////////////////////////////////
// decls
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
/*! 
 @def ex_array_each
 @param _type the type of the element in the array.
 @param _el the element variable you want to define.
 @param _array the in array
 @details macro for easy iterates the array container.

 when use the macro, it will define the local variable below:
 - count: the count of the element in the array.
 - idx: the current index.

 to finish the code, you must write ex_array_each_end.

 Usage:
 @code
 ex_array_t* my_array = ex_array_alloc( sizeof(float), 10 );
 ex_array_each ( float, item, my_array ) {
    printf( "item_%d is %f", idx, item );
 } ex_array_each_end;
 @endcode
 @sa ex_array_each_end
*/// ------------------------------------------------------------------ 

#define ex_array_each( _type, _el, _array ) \
    { \
        _type _el; \
        size_t count = _array->_length; \
        size_t idx = 0; \
        while ( idx < count ) { \
            _el = *( (_type*) (_array->_data) + idx );

// ------------------------------------------------------------------ 
/*! 
 @def ex_array_each_end
 @details macro to end the ex_array_each macro
 @sa ex_array_each
*/// ------------------------------------------------------------------ 

#define ex_array_each_end \
            ++idx; \
        } \
    }

// ------------------------------------------------------------------ 
/*! 
 @struct ex_array_t
 @details

 The data structure of dynamic array used in ex_array_ functions 
 
*/// ------------------------------------------------------------------ 

typedef struct ex_array_t {
    char* _data;
    size_t _element_bytes;
    size_t _length;
    size_t _capacity;
} ex_array_t;

// ------------------------------------------------------------------ 
/*! 
 @fn ex_array_t* ex_array_alloc ( size_t _element_bytes, size_t _count )
 @param _element_bytes the bytes of the element in the array
 @param _count the default count of the element in the array
 @details create and return an array by given size and count of the element in it.
 the array is alloc by exsdk's memory manage method, and you must use same memory
 operation command on it (those function \b without \em _nomng suffix)

 Example:
 @code
 // create an array to store float, the default size is 10.
 ex_array_t* my_array = ex_array_alloc ( sizeof(float), 10 );
 @endcode

 @note you need to use ex_array_free to release the memeory, other wise it will
 have memory leak.

 @sa ex_array_alloc_nomng
*/// ------------------------------------------------------------------ 

ex_array_t* ex_array_alloc ( size_t _element_bytes, size_t _count );
ex_array_t* ex_array_alloc_nomng ( size_t _element_bytes, size_t _count );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_array_free ( ex_array_t* _array );
void ex_array_free_nomng ( ex_array_t* _array );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void* ex_array_get ( ex_array_t* _array, size_t _idx );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void* ex_array_push_back ( ex_array_t* _array, void* _value );
void* ex_array_push_back_nomng ( ex_array_t* _array, void* _value );

inline void* ex_array_push_back_int8 ( ex_array_t* _array, int8 _value ) { return ex_array_push_back ( _array, &_value ); }
inline void* ex_array_push_back_int16 ( ex_array_t* _array, int16 _value ) { return ex_array_push_back ( _array, &_value ); }
inline void* ex_array_push_back_int32 ( ex_array_t* _array, int32 _value ) { return ex_array_push_back ( _array, &_value ); }
inline void* ex_array_push_back_int64 ( ex_array_t* _array, int64 _value ) { return ex_array_push_back ( _array, &_value ); }

inline void* ex_array_push_back_uint8 ( ex_array_t* _array, uint8 _value ) { return ex_array_push_back ( _array, &_value ); }
inline void* ex_array_push_back_uint16 ( ex_array_t* _array, uint16 _value ) { return ex_array_push_back ( _array, &_value ); }
inline void* ex_array_push_back_uint32 ( ex_array_t* _array, uint32 _value ) { return ex_array_push_back ( _array, &_value ); }
inline void* ex_array_push_back_uint64 ( ex_array_t* _array, uint64 _value ) { return ex_array_push_back ( _array, &_value ); }

inline void* ex_array_push_back_float ( ex_array_t* _array, float _value ) { return ex_array_push_back ( _array, &_value ); }
inline void* ex_array_push_back_double ( ex_array_t* _array, double _value ) { return ex_array_push_back ( _array, &_value ); }

inline void* ex_array_push_back_string ( ex_array_t* _array, char* _value ) { return ex_array_push_back ( _array, &_value ); }
inline void* ex_array_push_back_wstring ( ex_array_t* _array, wchar_t* _value ) { return ex_array_push_back ( _array, &_value ); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline size_t ex_array_len ( ex_array_t* _array ) { return _array->_length; } 

//! @}

// ######################### 
#ifdef __cplusplus
} // end extern "C"
#endif
// ######################### 

// #################################################################################
#endif // END ARRAY_H_1278479457
// #################################################################################


