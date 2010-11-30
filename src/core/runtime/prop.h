// ======================================================================================
// File         : prop.h
// Author       : Wu Jie 
// Last Change  : 11/26/2010 | 17:34:31 PM | Friday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef PROP_H_1290764072
#define PROP_H_1290764072
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "../core/misc/ptr.h"

///////////////////////////////////////////////////////////////////////////////
// struct
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: ex_prop_t
// ------------------------------------------------------------------ 

typedef struct ex_prop_t {
    const char* _name;  // NOTE: we not use strid_t, cause property defined before strid_t initialize.
    uint32 _attrs;      // attribute flags, default is EX_PROP_ATTR_NONE
    size_t _offset;     // use function: size_t offsetof (type, member), if not use, set it to -1
    void (*set_property) ( void* _pObj, size_t _offset, const void* _value ); // if not use, set it to NULL
    void (*get_property) ( void* _pObj, size_t _offset, void* _value ); // if not use, set it to NULL
} ex_prop_t;

// ------------------------------------------------------------------ 
// Desc: flags
// ------------------------------------------------------------------ 

#define EX_PROP_ATTR_NONE           0x00000000
#define EX_PROP_ATTR_READ_ONLY      0x00000001
#define EX_PROP_ATTR_HIDE           0x00000002
#define EX_PROP_ATTR_NO_SERIALIZE   0x00000004
#define EX_PROP_ATTR_EDITOR_ONLY    0x00000008

// EXAMPLE { 
// ex_prop_t[] props = {
//     { "value1", EX_PROP_ATTR_NONE,      offsetof(struct foobar, value1), ex_prop_set_raw_int8,  ex_prop_get_raw_int8 }
//   , { "value2", EX_PROP_ATTR_READ_ONLY, offsetof(struct foobar, value2), ex_prop_set_raw_float, ex_prop_get_raw_float }
// }
//
// just show u how to set empty value for each item.
// { "unknown", 0, -1, NULL, NULL }
// } EXAMPLE end 

///////////////////////////////////////////////////////////////////////////////
// macros
///////////////////////////////////////////////////////////////////////////////

// // why not use strid_t ???
// // because if use strid_t for the property name, we can't define it in
// // compile-time, but the array below is store in compile time.
// static const ex_prop_t __PROPS_test_cls_t[] = {
//     { "id",     EX_PROP_ATTR_READ_ONLY, offsetof(struct test_cls_t, id),    ex_prop_set_raw_int32, ex_prop_get_raw_int32 }
//   , { "data1",  EX_PROP_ATTR_NONE,      offsetof(struct test_cls_t, data1), ex_prop_set_raw_float, ex_prop_get_raw_float }
//   , { "data2",  EX_PROP_ATTR_NONE,      offsetof(struct test_cls_t, data2), ex_prop_set_raw_float, ex_prop_get_raw_float }
// };

// ------------------------------------------------------------------ 
// Desc: EX_DEF_PROPS_BEGIN(_className)
// ------------------------------------------------------------------ 

#define EX_DEF_PROPS_BEGIN(_className,_superClassName) \
    strid_t __CLASSID_##_className; \
    void ex_register_class_##_className () { \
        ex_rtti_t* rtti = ex_rtti_register_class ( #_className, #_superClassName ); \
        static const ex_prop_t __PROPS_##_className[] = { \

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_PROP( _className, _member, _propName, _attrs, _set_func, _get_func ) \
    { _propName, _attrs, offsetof(struct _className, _member), _set_func, _get_func },

// ------------------------------------------------------------------ 
// Desc: EX_DEF_PROPS_END(_className)
// ------------------------------------------------------------------ 

#define EX_DEF_PROPS_END(_className) \
            { "", 0, -1, NULL, NULL } \
        }; /*end of __PROPS_##_className*/ \
        if ( rtti ) { \
            __CLASSID_##_className = rtti->_classid; /*for the use of EX_CLASSID(name)*/ \
            ex_rtti_register_properties ( rtti, __PROPS_##_className, EX_ARRAY_COUNT(__PROPS_##_className)-1 ); \
        } \
    }

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void ex_prop_set_raw_int8 ( void* _pObj, size_t _offset, const void* _value ) { *(int8*)ex_ptr_add(_pObj,_offset) = *(const int8*)_value; }
inline void ex_prop_get_raw_int8 ( void* _pObj, size_t _offset, void* _value ) { *(int8*)_value = *(int8*)ex_ptr_add(_pObj,_offset); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void ex_prop_set_raw_int16 ( void* _pObj, size_t _offset, const void* _value ) { *(int16*)ex_ptr_add(_pObj,_offset) = *(const int16*)_value; }
inline void ex_prop_get_raw_int16 ( void* _pObj, size_t _offset, void* _value ) { *(int16*)_value = *(int16*)ex_ptr_add(_pObj,_offset); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void ex_prop_set_raw_int32 ( void* _pObj, size_t _offset, const void* _value ) { *(int32*)ex_ptr_add(_pObj,_offset) = *(const int32*)_value; }
inline void ex_prop_get_raw_int32 ( void* _pObj, size_t _offset, void* _value ) { *(int32*)_value = *(int32*)ex_ptr_add(_pObj,_offset); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void ex_prop_set_raw_int64 ( void* _pObj, size_t _offset, const void* _value ) { *(int64*)ex_ptr_add(_pObj,_offset) = *(const int64*)_value; }
inline void ex_prop_get_raw_int64 ( void* _pObj, size_t _offset, void* _value ) { *(int64*)_value = *(int64*)ex_ptr_add(_pObj,_offset); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void ex_prop_set_raw_uint8 ( void* _pObj, size_t _offset, const void* _value ) { *(uint8*)ex_ptr_add(_pObj,_offset) = *(const uint8*)_value; }
inline void ex_prop_get_raw_uint8 ( void* _pObj, size_t _offset, void* _value ) { *(uint8*)_value = *(uint8*)ex_ptr_add(_pObj,_offset); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void ex_prop_set_raw_uint16 ( void* _pObj, size_t _offset, const void* _value ) { *(uint16*)ex_ptr_add(_pObj,_offset) = *(const uint16*)_value; }
inline void ex_prop_get_raw_uint16 ( void* _pObj, size_t _offset, void* _value ) { *(uint16*)_value = *(uint16*)ex_ptr_add(_pObj,_offset); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void ex_prop_set_raw_uint32 ( void* _pObj, size_t _offset, const void* _value ) { *(uint32*)ex_ptr_add(_pObj,_offset) = *(const uint32*)_value; }
inline void ex_prop_get_raw_uint32 ( void* _pObj, size_t _offset, void* _value ) { *(uint32*)_value = *(uint32*)ex_ptr_add(_pObj,_offset); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void ex_prop_set_raw_uint64 ( void* _pObj, size_t _offset, const void* _value ) { *(uint64*)ex_ptr_add(_pObj,_offset) = *(const uint64*)_value; }
inline void ex_prop_get_raw_uint64 ( void* _pObj, size_t _offset, void* _value ) { *(uint64*)_value = *(uint64*)ex_ptr_add(_pObj,_offset); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void ex_prop_set_raw_float ( void* _pObj, size_t _offset, const void* _value ) { *(float*)ex_ptr_add(_pObj,_offset) = *(const float*)_value; }
inline void ex_prop_get_raw_float ( void* _pObj, size_t _offset, void* _value ) { *(float*)_value = *(float*)ex_ptr_add(_pObj,_offset); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void ex_prop_set_raw_double ( void* _pObj, size_t _offset, const void* _value ) { *(double*)ex_ptr_add(_pObj,_offset) = *(const double*)_value; }
inline void ex_prop_get_raw_double ( void* _pObj, size_t _offset, void* _value ) { *(double*)_value = *(double*)ex_ptr_add(_pObj,_offset); }

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END PROP_H_1290764072
// #################################################################################


