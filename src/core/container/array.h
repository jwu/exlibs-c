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

 @note you need to use \ref ex_array_free to release the memeory, other wise it will
 lead to memory leak.

 @sa ex_array_alloc_nomng

 @fn ex_array_t* ex_array_alloc_nomng ( size_t _element_bytes, size_t _count )
 @param _element_bytes the bytes of the element in the array
 @param _count the default count of the element in the array
 @details create and return an array by given size and count of the element in it.
 the array is alloc by system memory, and you must use same memory operation 
 command on it (those function \b with \em _nomng suffix)

 @note you need to use \ref ex_array_free_nomng to release the memeory, other wise it will
 lead to memory leak.

 @sa ex_array_alloc
*/// ------------------------------------------------------------------ 

ex_array_t* ex_array_alloc ( size_t _element_bytes, size_t _count );
ex_array_t* ex_array_alloc_nomng ( size_t _element_bytes, size_t _count );

// ------------------------------------------------------------------ 
/*! 
 @fn void ex_array_free ( ex_array_t* _array )
 @param _array the allocated array.
 @details destroy the allocated array, release the memory by exsdk's memory manager.
 @sa ex_array_free_nomng

 @fn void ex_array_free_nomng ( ex_array_t* _array )
 @param _array the allocated array.
 @details destroy the allocated array, release the memory by system memory operation.
 @sa ex_array_free
*/// ------------------------------------------------------------------ 

void ex_array_free ( ex_array_t* _array );
void ex_array_free_nomng ( ex_array_t* _array );

// ------------------------------------------------------------------ 
/*! 
 @fn void* ex_array_get ( ex_array_t* _array, size_t _idx )
 @param _array the in array.
 @param _idx the element index
 @return get the element pointer address
 @details get the array element by index

 Example:
 @code
 float* result = NULL;
 // create an array to store float, the default size is 10.
 ex_array_t* my_array = ex_array_alloc ( sizeof(float), 10 );
 ex_array_append_float ( my_array, 10.0f ); // push at index 0
 ex_array_append_float ( my_array, 20.0f ); // push at index 1
 ex_array_append_float ( my_array, 30.0f ); // push at index 2
 ex_array_append_float ( my_array, 40.0f ); // push at index 3
 result = (float*)ex_array_get ( my_array, 1 ); // get from index 1
 printf( "the result is %f", *result ); // the result is 20.0f
 @endcode

 @note you must cast the type of the element by yourself.
*/// ------------------------------------------------------------------ 

void* ex_array_get ( ex_array_t* _array, size_t _idx );

// ------------------------------------------------------------------ 
/*! 
 @fn inline size_t ex_array_len ( ex_array_t* _array )
 @param _array the in array
 @details get the array element count.
*/// ------------------------------------------------------------------ 

inline size_t ex_array_len ( ex_array_t* _array ) { return _array->_length; } 

// ------------------------------------------------------------------ 
/*! 
 @fn void* ex_array_append ( ex_array_t* _array, void* _value )
 @param _array the in array.
 @param _value the value add to the array
 @return the pointer address of the appended element in the array
 @details append the value to the array, the array will copy the value through
 the parameter _value by allocating it in the passing array use exsdk's
 memory manage method. 

 Example:
 @code
 float* result;
 float val = 10.0f;
 // create an array to store float, the default size is 10.
 ex_array_t* my_array = ex_array_alloc ( sizeof(float), 10 );
 {
     float val = 10.0f;
     result = (float*)ex_array_append ( my_array, &val );
 }
 printf( "the result is %f", *result ); // the result is 10.0f
 @endcode

 @sa ex_array_append_nomng

 @fn void* ex_array_append_nomng ( ex_array_t* _array, void* _value )
 @param _array the in array.
 @param _value the value add to the array
 @return the pointer address of the appended element in the array
 @details append the value to the array, the array will copy the value through
 the parameter _value by allocating it in the passing array use system memory 
 operation method. 
 @sa ex_array_append
*/// ------------------------------------------------------------------ 

void* ex_array_append ( ex_array_t* _array, const void* _value );
void* ex_array_append_nomng ( ex_array_t* _array, const void* _value );

// ------------------------------------------------------------------ 
/*! 
 @fn inline int8* ex_array_append_int8 ( ex_array_t* _array, int8 _value )
 @details append int8 to the array

 @fn inline int16* ex_array_append_int16 ( ex_array_t* _array, int16 _value )
 @details append int16 to the array

 @fn inline int32* ex_array_append_int32 ( ex_array_t* _array, int32 _value )
 @details append int32 to the array

 @fn inline int64* ex_array_append_int64 ( ex_array_t* _array, int64 _value )
 @details append int64 to the array

 @fn inline uint8* ex_array_append_uint8 ( ex_array_t* _array, uint8 _value )
 @details append uint8 to the array

 @fn inline uint16* ex_array_append_uint16 ( ex_array_t* _array, uint16 _value )
 @details append uint16 to the array

 @fn inline uint32* ex_array_append_uint32 ( ex_array_t* _array, uint32 _value )
 @details append uint32 to the array

 @fn inline uint64* ex_array_append_uint64 ( ex_array_t* _array, uint64 _value )
 @details append uint64 to the array

 @fn inline float* ex_array_append_float ( ex_array_t* _array, float _value )
 @details append float to the array

 @fn inline double* ex_array_append_double ( ex_array_t* _array, double _value )
 @details append double to the array

 @fn inline char** ex_array_append_string ( ex_array_t* _array, char* _value )
 @details append c-string to the array

 @fn inline wchar_t** ex_array_append_wstring ( ex_array_t* _array, wchar_t* _value )
 @details append w-string to the array

*/// ------------------------------------------------------------------ 

inline int8* ex_array_append_int8 ( ex_array_t* _array, int8 _value ) { return (int8*)ex_array_append ( _array, &_value ); }
inline int16* ex_array_append_int16 ( ex_array_t* _array, int16 _value ) { return (int16*)ex_array_append ( _array, &_value ); }
inline int32* ex_array_append_int32 ( ex_array_t* _array, int32 _value ) { return (int32*)ex_array_append ( _array, &_value ); }
inline int64* ex_array_append_int64 ( ex_array_t* _array, int64 _value ) { return (int64*)ex_array_append ( _array, &_value ); }

inline uint8* ex_array_append_uint8 ( ex_array_t* _array, uint8 _value ) { return (uint8*)ex_array_append ( _array, &_value ); }
inline uint16* ex_array_append_uint16 ( ex_array_t* _array, uint16 _value ) { return (uint16*)ex_array_append ( _array, &_value ); }
inline uint32* ex_array_append_uint32 ( ex_array_t* _array, uint32 _value ) { return (uint32*)ex_array_append ( _array, &_value ); }
inline uint64* ex_array_append_uint64 ( ex_array_t* _array, uint64 _value ) { return (uint64*)ex_array_append ( _array, &_value ); }

inline float* ex_array_append_float ( ex_array_t* _array, float _value ) { return (float*)ex_array_append ( _array, &_value ); }
inline double* ex_array_append_double ( ex_array_t* _array, double _value ) { return (double*)ex_array_append ( _array, &_value ); }

inline char** ex_array_append_string ( ex_array_t* _array, char* _value ) { return (char**)ex_array_append ( _array, &_value ); }
inline wchar_t** ex_array_append_wstring ( ex_array_t* _array, wchar_t* _value ) { return (wchar_t**)ex_array_append ( _array, &_value ); }

// ------------------------------------------------------------------ 
/*! 
 @fn void ex_array_remove_index ( ex_array_t* _array, size_t _idx )
 @param _array the in array
 @param _idx the remove element index. 
 @details remove the element by index.
 @sa ex_array_remove_index_fast
*/// ------------------------------------------------------------------ 

void ex_array_remove_index ( ex_array_t* _array, size_t _idx );

// ------------------------------------------------------------------ 
/*! 
 @fn void ex_array_remove_index_fast ( ex_array_t* _array, size_t _idx )
 @param _array the in array
 @param _idx the remove element index. 
 @details remove the element by index. the method will move the last
 element to the removed element adress, which make it faster than \ref
 ex_array_erase.
 @note using the method will break the indexing order in the array.
 @sa ex_array_remove_index
*/// ------------------------------------------------------------------ 

void ex_array_remove_index_fast ( ex_array_t* _array, size_t _idx );

// ------------------------------------------------------------------ 
/*! 
 @fn void ex_array_remove_range ( ex_array_t* _array, size_t _idx, size_t _count )
 @param _array the in array
 @param _idx the start index to remove
 @param _count the count of element to remove
 @details remove n count of element from index i
*/// ------------------------------------------------------------------ 

void ex_array_remove_range ( ex_array_t* _array, size_t _idx, size_t _count );

// TODO { 
// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_array_shrink ( ex_array_t* _array );
void ex_array_shrink_nomng ( ex_array_t* _array );
// } TODO end 

//! @}

// ######################### 
#ifdef __cplusplus
} // end extern "C"
#endif
// ######################### 

// #################################################################################
#endif // END ARRAY_H_1278479457
// #################################################################################


