// ======================================================================================
// File         : builtin_type_registry.c
// Author       : Wu Jie 
// Last Change  : 01/17/2011 | 16:07:12 PM | Monday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

///////////////////////////////////////////////////////////////////////////////
// register builtin types 
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EX_DEF_BUILTIN_TYPE(bool)
EX_DEF_BUILTIN_TYPE(int)
EX_DEF_BUILTIN_TYPE(size_t)
EX_DEF_BUILTIN_TYPE(int8)
EX_DEF_BUILTIN_TYPE(int16)
EX_DEF_BUILTIN_TYPE(int32)
EX_DEF_BUILTIN_TYPE(int64)
EX_DEF_BUILTIN_TYPE(uint8)
EX_DEF_BUILTIN_TYPE(uint16)
EX_DEF_BUILTIN_TYPE(uint32)
EX_DEF_BUILTIN_TYPE(uint64)
EX_DEF_BUILTIN_TYPE(float)
EX_DEF_BUILTIN_TYPE(double)
EX_DEF_BUILTIN_TYPE(cstr)
EX_DEF_BUILTIN_TYPE(string)
EX_DEF_BUILTIN_TYPE(strid)
EX_DEF_BUILTIN_TYPE(vec2f)
EX_DEF_BUILTIN_TYPE(vec3f)
EX_DEF_BUILTIN_TYPE(vec4f)
EX_DEF_BUILTIN_TYPE(mat22f)
EX_DEF_BUILTIN_TYPE(mat33f)
EX_DEF_BUILTIN_TYPE(mat44f)
EX_DEF_BUILTIN_TYPE(quatf)
EX_DEF_BUILTIN_TYPE(angf)
EX_DEF_BUILTIN_TYPE(color3u)
EX_DEF_BUILTIN_TYPE(color3f)
EX_DEF_BUILTIN_TYPE(color4u)
EX_DEF_BUILTIN_TYPE(color4f)
EX_DEF_BUILTIN_TYPE(array)
EX_DEF_BUILTIN_TYPE(map)

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_register_builtin_types () {
    EX_REGISTER_BUILTIN_TYPE(bool);
    EX_REGISTER_BUILTIN_TYPE(int);
    EX_REGISTER_BUILTIN_TYPE(size_t);
    EX_REGISTER_BUILTIN_TYPE(int8);
    EX_REGISTER_BUILTIN_TYPE(int16);
    EX_REGISTER_BUILTIN_TYPE(int32);
    EX_REGISTER_BUILTIN_TYPE(int64);
    EX_REGISTER_BUILTIN_TYPE(uint8);
    EX_REGISTER_BUILTIN_TYPE(uint16);
    EX_REGISTER_BUILTIN_TYPE(uint32);
    EX_REGISTER_BUILTIN_TYPE(uint64);
    EX_REGISTER_BUILTIN_TYPE(float);
    EX_REGISTER_BUILTIN_TYPE(double);
    EX_REGISTER_BUILTIN_TYPE(cstr);
    EX_REGISTER_BUILTIN_TYPE(string);
    EX_REGISTER_BUILTIN_TYPE(strid);
    EX_REGISTER_BUILTIN_TYPE(vec2f);
    EX_REGISTER_BUILTIN_TYPE(vec3f);
    EX_REGISTER_BUILTIN_TYPE(vec4f);
    EX_REGISTER_BUILTIN_TYPE(mat22f);
    EX_REGISTER_BUILTIN_TYPE(mat33f);
    EX_REGISTER_BUILTIN_TYPE(mat44f);
    EX_REGISTER_BUILTIN_TYPE(quatf);
    EX_REGISTER_BUILTIN_TYPE(angf);
    EX_REGISTER_BUILTIN_TYPE(color3u);
    EX_REGISTER_BUILTIN_TYPE(color3f);
    EX_REGISTER_BUILTIN_TYPE(color4u);
    EX_REGISTER_BUILTIN_TYPE(color4f);
    EX_REGISTER_BUILTIN_TYPE(array);
    EX_REGISTER_BUILTIN_TYPE(map);
}
