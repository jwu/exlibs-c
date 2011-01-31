// ======================================================================================
// File         : builtin_type.h
// Author       : Wu Jie 
// Last Change  : 01/17/2011 | 16:08:46 PM | Monday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef BUILTIN_TYPE_H_1295251727
#define BUILTIN_TYPE_H_1295251727
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "../algo/uid.h"
#include "../../engine/object.h"

#include "rtti.h"
#include "builtin_serialize.h"
#include "builtin_tostring.h"
#include "builtin_prop_getset.h"

///////////////////////////////////////////////////////////////////////////////
// defiens
///////////////////////////////////////////////////////////////////////////////

#define EX_DEF_BUILTIN_TYPE(_typename) \
    ex_rtti_t *__RTTI_##_typename##__ = NULL; \
    strid_t __TYPEID_##_typename##__ = EX_STRID_NULL;

#define EX_REGISTER_BUILTIN_TYPE(_typename) \
    __ex_register_builtin_type_##_typename()

///////////////////////////////////////////////////////////////////////////////
// declares
///////////////////////////////////////////////////////////////////////////////

#define DECL_BUILTIN_TYPE(_typename) \
    extern ex_rtti_t *__RTTI_##_typename##__; /*for EX_RTTI*/ \
    static inline void __ex_register_builtin_type_##_typename () { /*for EX_REGISTER_CLASS, define in EX_DEF_PROPS_BEGIN*/ \
        __TYPEID_##_typename##__ = ex_strid(#_typename); \
        __RTTI_##_typename##__ = ex_rtti_register_class ( __TYPEID_##_typename##__, \
                                                          NULL, \
                                                          sizeof(_typename), \
                                                          NULL, \
                                                          __ex_serialize_##_typename, \
                                                          __ex_tostring_##_typename \
                                                          ); \
    }

#define DECL_BUILTIN_TYPE_2(_typename,_type) \
    extern ex_rtti_t *__RTTI_##_typename##__; /*for EX_RTTI*/ \
    static inline void __ex_register_builtin_type_##_typename () { /*for EX_REGISTER_CLASS, define in EX_DEF_PROPS_BEGIN*/ \
        __TYPEID_##_typename##__ = ex_strid(#_typename); \
        __RTTI_##_typename##__ = ex_rtti_register_class ( __TYPEID_##_typename##__, \
                                                          NULL, \
                                                          sizeof(_type), \
                                                          NULL, \
                                                          __ex_serialize_##_typename, \
                                                          __ex_tostring_##_typename \
                                                          ); \
    }

DECL_BUILTIN_TYPE(bool);
DECL_BUILTIN_TYPE(int);
DECL_BUILTIN_TYPE(size_t);
DECL_BUILTIN_TYPE_2(uid, ex_uid_t);
DECL_BUILTIN_TYPE(int8);
DECL_BUILTIN_TYPE(int16);
DECL_BUILTIN_TYPE(int32);
DECL_BUILTIN_TYPE(int64);
DECL_BUILTIN_TYPE(uint8);
DECL_BUILTIN_TYPE(uint16);
DECL_BUILTIN_TYPE(uint32);
DECL_BUILTIN_TYPE(uint64);
DECL_BUILTIN_TYPE(float);
DECL_BUILTIN_TYPE(double);
DECL_BUILTIN_TYPE_2(cstr, char *);
DECL_BUILTIN_TYPE_2(string, ex_string_t);
DECL_BUILTIN_TYPE_2(strid, strid_t);
DECL_BUILTIN_TYPE_2(angf, ex_angf_t);
DECL_BUILTIN_TYPE_2(vec2f, ex_vec2f_t);
DECL_BUILTIN_TYPE_2(vec3f, ex_vec3f_t);
DECL_BUILTIN_TYPE_2(vec4f, ex_vec4f_t);
DECL_BUILTIN_TYPE_2(mat22f, ex_mat22f_t);
DECL_BUILTIN_TYPE_2(mat33f, ex_mat33f_t);
DECL_BUILTIN_TYPE_2(mat44f, ex_mat44f_t);
DECL_BUILTIN_TYPE_2(quatf, ex_quatf_t);
DECL_BUILTIN_TYPE_2(color3u, ex_color3u_t);
DECL_BUILTIN_TYPE_2(color3f, ex_color3f_t);
DECL_BUILTIN_TYPE_2(color4u, ex_color4u_t);
DECL_BUILTIN_TYPE_2(color4f, ex_color4f_t);
DECL_BUILTIN_TYPE_2(array, ex_array_t *);
DECL_BUILTIN_TYPE_2(map, ex_hashmap_t *);
DECL_BUILTIN_TYPE_2(ref, ex_ref_t *);

#undef DECL_BUILTIN_TYPE
#undef DECL_BUILTIN_TYPE_2

///////////////////////////////////////////////////////////////////////////////
// func
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_is_builtin_type ( strid_t _typeID ) {
    if ( _typeID >= EX_TYPEID(int8) && _typeID <= EX_TYPEID(ref) )
        return true;
    return false;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_is_integer ( strid_t _typeID ) {
    if ( _typeID >= EX_TYPEID(int) && _typeID <= EX_TYPEID(uint64) )
        return true;
    return false;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_is_fp ( strid_t _typeID ) {
    if ( _typeID >= EX_TYPEID(float) && _typeID <= EX_TYPEID(double) )
        return true;
    return false;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_is_string ( strid_t _typeID ) {
    if ( _typeID >= EX_TYPEID(cstr) && _typeID <= EX_TYPEID(strid) )
        return true;
    return false;
}

// #################################################################################
#endif // END BUILTIN_TYPE_H_1295251727
// #################################################################################


