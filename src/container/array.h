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

///////////////////////////////////////////////////////////////////////////////
// decls
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

typedef struct array_t {
    char* _data;
    size_t _element_bytes;
    size_t _length;
    size_t _capacity;
} array_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

array_t* array_alloc ( size_t _element_bytes, size_t _count );
array_t* array_alloc_nomng ( size_t _element_bytes, size_t _count );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void array_free ( array_t* _array );
void array_free_nomng ( array_t* _array );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void* array_get ( array_t* _array, size_t _idx );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void* array_push_back ( array_t* _array, void* _value );
void* array_push_back_nomng ( array_t* _array, void* _value );

inline void* array_push_back_int8 ( array_t* _array, int8 _value ) { return array_push_back ( _array, &_value ); }
inline void* array_push_back_int16 ( array_t* _array, int16 _value ) { return array_push_back ( _array, &_value ); }
inline void* array_push_back_int32 ( array_t* _array, int32 _value ) { return array_push_back ( _array, &_value ); }
inline void* array_push_back_int64 ( array_t* _array, int64 _value ) { return array_push_back ( _array, &_value ); }

inline void* array_push_back_uint8 ( array_t* _array, uint8 _value ) { return array_push_back ( _array, &_value ); }
inline void* array_push_back_uint16 ( array_t* _array, uint16 _value ) { return array_push_back ( _array, &_value ); }
inline void* array_push_back_uint32 ( array_t* _array, uint32 _value ) { return array_push_back ( _array, &_value ); }
inline void* array_push_back_uint64 ( array_t* _array, uint64 _value ) { return array_push_back ( _array, &_value ); }

inline void* array_push_back_float ( array_t* _array, float _value ) { return array_push_back ( _array, &_value ); }
inline void* array_push_back_double ( array_t* _array, double _value ) { return array_push_back ( _array, &_value ); }

inline void* array_push_back_string ( array_t* _array, char* _value ) { return array_push_back ( _array, &_value ); }
inline void* array_push_back_wstring ( array_t* _array, wchar_t* _value ) { return array_push_back ( _array, &_value ); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline size_t array_len ( array_t* _array ) { return _array->_length; } 

// #################################################################################
#endif // END ARRAY_H_1278479457
// #################################################################################


