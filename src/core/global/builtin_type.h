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
// defiens
///////////////////////////////////////////////////////////////////////////////

#define EX_TYPEID(_name) \
    (__TYPEID_##_name##__)

#define EX_DEF_BUILTIN_TYPE(_name) \
    strid_t __TYPEID_##_name##__ = EX_STRID_INVALID

#define EX_REGISTER_BUILTIN_TYPE(_name) \
    __TYPEID_##_name##__ = ex_strid(#_name); \
    ex_rtti_register_serialize ( __TYPEID_##_name##__, __ex_serialize_##_name );

#define EX_DECL_BUILTIN_TYPE(_name) \
    extern strid_t __TYPEID_##_name##__ 

///////////////////////////////////////////////////////////////////////////////
// declares
///////////////////////////////////////////////////////////////////////////////

EX_DECL_BUILTIN_TYPE(int8);
EX_DECL_BUILTIN_TYPE(int16);
EX_DECL_BUILTIN_TYPE(int32);
EX_DECL_BUILTIN_TYPE(int64);
EX_DECL_BUILTIN_TYPE(uint8);
EX_DECL_BUILTIN_TYPE(uint16);
EX_DECL_BUILTIN_TYPE(uint32);
EX_DECL_BUILTIN_TYPE(uint64);
EX_DECL_BUILTIN_TYPE(float);
EX_DECL_BUILTIN_TYPE(double);
EX_DECL_BUILTIN_TYPE(boolean);
EX_DECL_BUILTIN_TYPE(string);
EX_DECL_BUILTIN_TYPE(strid);
EX_DECL_BUILTIN_TYPE(array);
EX_DECL_BUILTIN_TYPE(map);
EX_DECL_BUILTIN_TYPE(vec2f);
EX_DECL_BUILTIN_TYPE(vec3f);
EX_DECL_BUILTIN_TYPE(vec4f);
EX_DECL_BUILTIN_TYPE(mat22f);
EX_DECL_BUILTIN_TYPE(mat33f);
EX_DECL_BUILTIN_TYPE(mat44f);
EX_DECL_BUILTIN_TYPE(quatf);
EX_DECL_BUILTIN_TYPE(angf);
EX_DECL_BUILTIN_TYPE(color3u);
EX_DECL_BUILTIN_TYPE(color3f);
EX_DECL_BUILTIN_TYPE(color4u);
EX_DECL_BUILTIN_TYPE(color4f);

///////////////////////////////////////////////////////////////////////////////
// func
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_is_builtin_type ( strid_t _typeID ) {
    if ( _typeID >= EX_TYPEID(int8) && _typeID <= EX_TYPEID(color4f) )
        return true;
    return false;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_is_integer ( strid_t _typeID ) {
    if ( _typeID >= EX_TYPEID(int8) && _typeID <= EX_TYPEID(uint64) )
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

// #################################################################################
#endif // END BUILTIN_TYPE_H_1295251727
// #################################################################################


