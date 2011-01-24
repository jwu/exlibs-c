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

#include "core/misc/ptr.h"

///////////////////////////////////////////////////////////////////////////////
// struct
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: ex_prop_t
// ------------------------------------------------------------------ 

typedef struct ex_prop_t {
    const char *name;  // NOTE: we not use strid_t, cause property defined before strid_t initialize.
    uint32 attrs;      // attribute flags, default is EX_PROP_ATTR_NONE
    size_t offset;     // use function: size_t offsetof (type, member), if not use, set it to -1
    void (*set_property) ( void *_pObj, size_t _offset, const void *_value ); // if not use, set it to NULL
    void (*get_property) ( void *_pObj, size_t _offset, void *_value ); // if not use, set it to NULL
} ex_prop_t;

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline void ex_prop_set_raw_int8 ( void *_pObj, size_t _offset, const void *_value ) { *(int8 *)ex_ptr_add(_pObj,_offset) = *(const int8 *)_value; }
static inline void ex_prop_get_raw_int8 ( void *_pObj, size_t _offset, void *_value ) { *(int8 *)_value = *(int8 *)ex_ptr_add(_pObj,_offset); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline void ex_prop_set_raw_int16 ( void *_pObj, size_t _offset, const void *_value ) { *(int16 *)ex_ptr_add(_pObj,_offset) = *(const int16 *)_value; }
static inline void ex_prop_get_raw_int16 ( void *_pObj, size_t _offset, void *_value ) { *(int16 *)_value = *(int16 *)ex_ptr_add(_pObj,_offset); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline void ex_prop_set_raw_int32 ( void *_pObj, size_t _offset, const void *_value ) { *(int32 *)ex_ptr_add(_pObj,_offset) = *(const int32 *)_value; }
static inline void ex_prop_get_raw_int32 ( void *_pObj, size_t _offset, void * _value ) { *(int32 *)_value = *(int32 *)ex_ptr_add(_pObj,_offset); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline void ex_prop_set_raw_int64 ( void *_pObj, size_t _offset, const void *_value ) { *(int64 *)ex_ptr_add(_pObj,_offset) = *(const int64 *)_value; }
static inline void ex_prop_get_raw_int64 ( void *_pObj, size_t _offset, void *_value ) { *(int64 *)_value = *(int64 *)ex_ptr_add(_pObj,_offset); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline void ex_prop_set_raw_uint8 ( void *_pObj, size_t _offset, const void *_value ) { *(uint8 *)ex_ptr_add(_pObj,_offset) = *(const uint8 *)_value; }
static inline void ex_prop_get_raw_uint8 ( void *_pObj, size_t _offset, void *_value ) { *(uint8 *)_value = *(uint8 *)ex_ptr_add(_pObj,_offset); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline void ex_prop_set_raw_uint16 ( void *_pObj, size_t _offset, const void *_value ) { *(uint16 *)ex_ptr_add(_pObj,_offset) = *(const uint16 *)_value; }
static inline void ex_prop_get_raw_uint16 ( void *_pObj, size_t _offset, void *_value ) { *(uint16 *)_value = *(uint16 *)ex_ptr_add(_pObj,_offset); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline void ex_prop_set_raw_uint32 ( void *_pObj, size_t _offset, const void *_value ) { *(uint32 *)ex_ptr_add(_pObj,_offset) = *(const uint32 *)_value; }
static inline void ex_prop_get_raw_uint32 ( void *_pObj, size_t _offset, void *_value ) { *(uint32 *)_value = *(uint32 *)ex_ptr_add(_pObj,_offset); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline void ex_prop_set_raw_uint64 ( void *_pObj, size_t _offset, const void *_value ) { *(uint64 *)ex_ptr_add(_pObj,_offset) = *(const uint64 *)_value; }
static inline void ex_prop_get_raw_uint64 ( void *_pObj, size_t _offset, void *_value ) { *(uint64 *)_value = *(uint64 *)ex_ptr_add(_pObj,_offset); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline void ex_prop_set_raw_float ( void *_pObj, size_t _offset, const void *_value ) { *(float *)ex_ptr_add(_pObj,_offset) = *(const float *)_value; }
static inline void ex_prop_get_raw_float ( void *_pObj, size_t _offset, void *_value ) { *(float *)_value = *(float *)ex_ptr_add(_pObj,_offset); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline void ex_prop_set_raw_double ( void *_pObj, size_t _offset, const void *_value ) { *(double *)ex_ptr_add(_pObj,_offset) = *(const double *)_value; }
static inline void ex_prop_get_raw_double ( void *_pObj, size_t _offset, void *_value ) { *(double *)_value = *(double *)ex_ptr_add(_pObj,_offset); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline void ex_prop_set_raw_strid ( void *_pObj, size_t _offset, const void *_value ) { *(strid_t *)ex_ptr_add(_pObj,_offset) = ex_strid(*(const char **)_value); }
static inline void ex_prop_get_raw_strid ( void *_pObj, size_t _offset, void *_value ) { *(char **)_value = ex_strid_to_cstr( *(strid_t *)ex_ptr_add(_pObj,_offset) ); }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline void ex_prop_set_raw_angf ( void *_pObj, size_t _offset, const void *_value ) { 
    const ex_angf_t *a = (const ex_angf_t *)_value;
    ex_angf_set_by_radians_nosafe( (ex_angf_t *)ex_ptr_add(_pObj,_offset), a->rad );
}
static inline void ex_prop_get_raw_angf ( void *_pObj, size_t _offset, void *_value ) { 
    ex_angf_t *a = (ex_angf_t *)ex_ptr_add(_pObj,_offset);
    ex_angf_set_by_radians_nosafe ( (ex_angf_t *)_value, a->rad ); 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline void ex_prop_set_raw_vec2f ( void *_pObj, size_t _offset, const void *_value ) { 
    const ex_vec2f_t *v = (const ex_vec2f_t *)_value;
    ex_vec2f_set ( (ex_vec2f_t *)ex_ptr_add(_pObj,_offset), v->x, v->y ); 
}
static inline void ex_prop_get_raw_vec2f ( void *_pObj, size_t _offset, void *_value ) { 
    ex_vec2f_t *v = (ex_vec2f_t *)ex_ptr_add(_pObj,_offset);
    ex_vec2f_set ( (ex_vec2f_t *)_value, v->x, v->y ); 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline void ex_prop_set_raw_vec3f ( void *_pObj, size_t _offset, const void *_value ) { 
    const ex_vec3f_t *v = (const ex_vec3f_t *)_value;
    ex_vec3f_set ( (ex_vec3f_t *)ex_ptr_add(_pObj,_offset), v->x, v->y, v->z ); 
}
static inline void ex_prop_get_raw_vec3f ( void *_pObj, size_t _offset, void *_value ) { 
    ex_vec3f_t *v = (ex_vec3f_t *)ex_ptr_add(_pObj,_offset);
    ex_vec3f_set ( (ex_vec3f_t *)_value, v->x, v->y, v->z ); 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline void ex_prop_set_raw_vec4f ( void *_pObj, size_t _offset, const void *_value ) { 
    const ex_vec4f_t *v = (const ex_vec4f_t *)_value;
    ex_vec4f_set ( (ex_vec4f_t *)ex_ptr_add(_pObj,_offset), v->x, v->y, v->z, v->w ); 
}
static inline void ex_prop_get_raw_vec4f ( void *_pObj, size_t _offset, void *_value ) { 
    ex_vec4f_t *v = (ex_vec4f_t *)ex_ptr_add(_pObj,_offset);
    ex_vec4f_set ( (ex_vec4f_t *)_value, v->x, v->y, v->z, v->w ); 
}

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END PROP_H_1290764072
// #################################################################################


