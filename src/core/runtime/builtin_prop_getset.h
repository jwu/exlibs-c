// ======================================================================================
// File         : builtin_prop_getset.h
// Author       : Wu Jie 
// Last Change  : 01/25/2011 | 10:34:39 AM | Tuesday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef BUILTIN_PROP_GETSET_H_1295922880
#define BUILTIN_PROP_GETSET_H_1295922880
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "core/misc/ptr.h"

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

#define DEF_BUILTIN_PROP_SET_RAW(_type) \
    static inline void ex_prop_set_raw_##_type ( void *_obj, size_t _offset, const void *_value ) {  \
        *(_type *)ex_ptr_add(_obj,_offset) = *(const _type *)_value;  \
    }

#define DEF_BUILTIN_PROP_SET_RAW_2(_typename,_type) \
    static inline void ex_prop_set_raw_##_typename ( void *_obj, size_t _offset, const void *_value ) {  \
        *(_type *)ex_ptr_add(_obj,_offset) = *(const _type *)_value;  \
    }

#define DEF_BUILTIN_PROP_GET_RAW(_type) \
    static inline void ex_prop_get_raw_##_type ( void *_obj, size_t _offset, void *_value ) {  \
        *(_type *)_value = *(_type *)ex_ptr_add(_obj,_offset); \
    }

#define DEF_BUILTIN_PROP_GET_RAW_2(_typename,_type) \
    static inline void ex_prop_get_raw_##_typename ( void *_obj, size_t _offset, void *_value ) {  \
        *(_type *)_value = *(_type *)ex_ptr_add(_obj,_offset); \
    }

DEF_BUILTIN_PROP_SET_RAW(bool);
DEF_BUILTIN_PROP_GET_RAW(bool);

DEF_BUILTIN_PROP_SET_RAW(int);
DEF_BUILTIN_PROP_GET_RAW(int);

DEF_BUILTIN_PROP_SET_RAW(size_t);
DEF_BUILTIN_PROP_GET_RAW(size_t);

DEF_BUILTIN_PROP_SET_RAW(int8);
DEF_BUILTIN_PROP_GET_RAW(int8);

DEF_BUILTIN_PROP_SET_RAW(int16);
DEF_BUILTIN_PROP_GET_RAW(int16);

DEF_BUILTIN_PROP_SET_RAW(int32);
DEF_BUILTIN_PROP_GET_RAW(int32);

DEF_BUILTIN_PROP_SET_RAW(int64);
DEF_BUILTIN_PROP_GET_RAW(int64);

DEF_BUILTIN_PROP_SET_RAW(uint8);
DEF_BUILTIN_PROP_GET_RAW(uint8);

DEF_BUILTIN_PROP_SET_RAW(uint16);
DEF_BUILTIN_PROP_GET_RAW(uint16);

DEF_BUILTIN_PROP_SET_RAW(uint32);
DEF_BUILTIN_PROP_GET_RAW(uint32);

DEF_BUILTIN_PROP_SET_RAW(uint64);
DEF_BUILTIN_PROP_GET_RAW(uint64);

DEF_BUILTIN_PROP_SET_RAW(float);
DEF_BUILTIN_PROP_GET_RAW(float);

DEF_BUILTIN_PROP_SET_RAW(double);
DEF_BUILTIN_PROP_GET_RAW(double);

DEF_BUILTIN_PROP_SET_RAW_2(cstr, const char *);
DEF_BUILTIN_PROP_GET_RAW_2(cstr, char *);

// string - ex_string_t
static inline void ex_prop_set_raw_string ( void *_obj, size_t _offset, const void *_value ) {
    ex_string_t *prop = (ex_string_t *)ex_ptr_add(_obj,_offset);
    const ex_string_t *val = (const ex_string_t *)_value;
    ex_string_ncpy ( prop, val->text, val->len );
}
static inline void ex_prop_get_raw_string ( void *_obj, size_t _offset, void *_value ) {
    ex_string_t *prop = (ex_string_t *)ex_ptr_add(_obj,_offset);
    ex_string_t *val = (ex_string_t *)_value;
    ex_string_ncpy ( val, prop->text, prop->len );
}

// NOTE: strid will return cstr and set from cstr
// strid - strid_t
static inline void ex_prop_set_raw_strid ( void *_obj, size_t _offset, const void *_value ) {
    strid_t *prop = (strid_t *)ex_ptr_add(_obj,_offset);
    *prop = ex_strid( *(const char **)_value );
}
static inline void ex_prop_get_raw_strid ( void *_obj, size_t _offset, void *_value ) {
    strid_t *prop = (strid_t *)ex_ptr_add(_obj,_offset);
    const char *val = *(const char **)_value;
    val = ex_strid_to_cstr(*prop);
}

DEF_BUILTIN_PROP_SET_RAW_2(angf, ex_angf_t);
DEF_BUILTIN_PROP_GET_RAW_2(angf, ex_angf_t);

DEF_BUILTIN_PROP_SET_RAW_2(vec2f, ex_vec2f_t);
DEF_BUILTIN_PROP_GET_RAW_2(vec2f, ex_vec2f_t);

DEF_BUILTIN_PROP_SET_RAW_2(vec3f, ex_vec3f_t);
DEF_BUILTIN_PROP_GET_RAW_2(vec3f, ex_vec3f_t);

DEF_BUILTIN_PROP_SET_RAW_2(vec4f, ex_vec4f_t);
DEF_BUILTIN_PROP_GET_RAW_2(vec4f, ex_vec4f_t);

DEF_BUILTIN_PROP_SET_RAW_2(mat22f, ex_mat22f_t);
DEF_BUILTIN_PROP_GET_RAW_2(mat22f, ex_mat22f_t);

DEF_BUILTIN_PROP_SET_RAW_2(mat33f, ex_mat33f_t);
DEF_BUILTIN_PROP_GET_RAW_2(mat33f, ex_mat33f_t);

DEF_BUILTIN_PROP_SET_RAW_2(mat44f, ex_mat44f_t);
DEF_BUILTIN_PROP_GET_RAW_2(mat44f, ex_mat44f_t);

DEF_BUILTIN_PROP_SET_RAW_2(quatf, ex_quatf_t);
DEF_BUILTIN_PROP_GET_RAW_2(quatf, ex_quatf_t);

DEF_BUILTIN_PROP_SET_RAW_2(color3u, ex_color3u_t);
DEF_BUILTIN_PROP_GET_RAW_2(color3u, ex_color3u_t);

DEF_BUILTIN_PROP_SET_RAW_2(color3f, ex_color3f_t);
DEF_BUILTIN_PROP_GET_RAW_2(color3f, ex_color3f_t);

DEF_BUILTIN_PROP_SET_RAW_2(color4u, ex_color4u_t);
DEF_BUILTIN_PROP_GET_RAW_2(color4u, ex_color4u_t);

DEF_BUILTIN_PROP_SET_RAW_2(color4f, ex_color4f_t);
DEF_BUILTIN_PROP_GET_RAW_2(color4f, ex_color4f_t);

// array - ex_array_t
static inline void ex_prop_set_raw_array ( void *_obj, size_t _offset, const void *_value ) {
    ex_array_t *prop = (ex_array_t *)ex_ptr_add(_obj,_offset);
    const ex_array_t *val = (const ex_array_t *)_value;
    ex_array_cpy ( prop, val );
}
static inline void ex_prop_get_raw_array ( void *_obj, size_t _offset, void *_value ) {
    ex_array_t *prop = (ex_array_t *)ex_ptr_add(_obj,_offset);
    ex_array_t *val = (ex_array_t *)_value;
    ex_array_cpy ( val, prop );
}

// map - ex_hashmap_t
static inline void ex_prop_set_raw_map ( void *_obj, size_t _offset, const void *_value ) {
    ex_hashmap_t *prop = (ex_hashmap_t *)ex_ptr_add(_obj,_offset);
    const ex_hashmap_t *val = (const ex_hashmap_t *)_value;
    ex_hashmap_cpy ( prop, val );
}
static inline void ex_prop_get_raw_map ( void *_obj, size_t _offset, void *_value ) {
    ex_hashmap_t *prop = (ex_hashmap_t *)ex_ptr_add(_obj,_offset);
    ex_hashmap_t *val = (ex_hashmap_t *)_value;
    ex_hashmap_cpy ( val, prop );
}

#undef DEF_BUILTIN_PROP_SET_RAW
#undef DEF_BUILTIN_PROP_GET_RAW
#undef DEF_BUILTIN_PROP_SET_RAW_2
#undef DEF_BUILTIN_PROP_GET_RAW_2

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END BUILTIN_PROP_GETSET_H_1295922880
// #################################################################################


