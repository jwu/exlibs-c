// ======================================================================================
// File         : serialize_builtin.h
// Author       : Wu Jie 
// Last Change  : 01/17/2011 | 15:30:14 PM | Monday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef SERIALIZE_BUILTIN_H_1295082972
#define SERIALIZE_BUILTIN_H_1295082972
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "stream.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// DEF_BUILTIN_SERIALIZE
#define DEF_BUILTIN_SERIALIZE(_type) \
    extern strid_t __TYPEID_##_type##__; \
    static inline void __ex_serialize_##_type ( ex_stream_t *_stream, strid_t _name, void *_val ) { \
        if ( _stream->next ) { \
            int ret = _stream->next( _stream, _name, EX_TYPEID(_type) ); \
            if ( ret != 0 ) return; \
        } \
        _stream->serialize_##_type(_stream,(_type *)_val); \
    }

// DEF_BUILTIN_SERIALIZE_2
#define DEF_BUILTIN_SERIALIZE_2( _typename, _type ) \
    extern strid_t __TYPEID_##_typename##__; \
    static inline void __ex_serialize_##_typename ( ex_stream_t *_stream, strid_t _name, void *_val ) { \
        if ( _stream->next ) { \
            int ret = _stream->next( _stream, _name, EX_TYPEID(_typename) ); \
            if ( ret != 0 ) return; \
        } \
        _stream->serialize_##_typename(_stream,(_type *)_val); \
    }

// DEF_BUILTIN_SERIALIZE_ARRAY
#define DEF_BUILTIN_SERIALIZE_ARRAY( _typename, _type ) \
    extern strid_t __TYPEID_##_typename##__; \
    static inline void __ex_serialize_##_typename##_2 ( ex_stream_t *_stream, strid_t _name, void *_val, void *_pfn_serialize_el ) { \
        if ( _stream->next ) { \
            int ret = _stream->next( _stream, _name, EX_TYPEID(_typename) ); \
            if ( ret != 0 ) return; \
        } \
        if ( _stream->push ) _stream->push(_stream); \
        _stream->serialize_##_typename(_stream,(_type)_val,_pfn_serialize_el); \
        if ( _stream->pop ) _stream->pop(_stream); \
    } \
    static inline void __ex_serialize_##_typename ( ex_stream_t *_stream, strid_t _name, void *_val ) { \
        ex_array_t *_array = (ex_array_t *)_val; \
        __ex_serialize_##_typename##_2(_stream,_name,_val,ex_serialize_func(_array->element_typeid)); \
    }

// DEF_BUILTIN_SERIALIZE_MAP
#define DEF_BUILTIN_SERIALIZE_MAP( _typename, _type ) \
    extern strid_t __TYPEID_##_typename##__; \
    static inline void __ex_serialize_##_typename##_2 ( ex_stream_t *_stream, strid_t _name, void *_val, void *_pfn_serialize_key, void *_pfn_serialize_val ) { \
        if ( _stream->next ) { \
            int ret = _stream->next( _stream, _name, EX_TYPEID(_typename) ); \
            if ( ret != 0 ) return; \
        } \
        if ( _stream->push ) _stream->push(_stream); \
        _stream->serialize_##_typename(_stream,(_type)_val,_pfn_serialize_key,_pfn_serialize_val); \
        if ( _stream->pop ) _stream->pop(_stream); \
    } \
    static inline void __ex_serialize_##_typename ( ex_stream_t *_stream, strid_t _name, void *_val ) { \
        ex_hashmap_t *_map = (ex_hashmap_t *)_val; \
        __ex_serialize_##_typename##_2(_stream,_name,_val,ex_serialize_func(_map->key_typeid),ex_serialize_func(_map->value_typeid)); \
    }

DEF_BUILTIN_SERIALIZE(int8)
DEF_BUILTIN_SERIALIZE(int16)
DEF_BUILTIN_SERIALIZE(int32)
DEF_BUILTIN_SERIALIZE(int64)

DEF_BUILTIN_SERIALIZE(uint8)
DEF_BUILTIN_SERIALIZE(uint16)
DEF_BUILTIN_SERIALIZE(uint32)
DEF_BUILTIN_SERIALIZE(uint64)

DEF_BUILTIN_SERIALIZE(float)
DEF_BUILTIN_SERIALIZE(double)

DEF_BUILTIN_SERIALIZE_2(boolean, bool) // NOTE: bool will be replaced to "int"

DEF_BUILTIN_SERIALIZE_2(string, char *)
DEF_BUILTIN_SERIALIZE_2(strid, strid_t)

DEF_BUILTIN_SERIALIZE_2(vec2f, ex_vec2f_t)
DEF_BUILTIN_SERIALIZE_2(vec3f, ex_vec3f_t)
DEF_BUILTIN_SERIALIZE_2(vec4f, ex_vec4f_t)

DEF_BUILTIN_SERIALIZE_2(mat22f, ex_mat22f_t)
DEF_BUILTIN_SERIALIZE_2(mat33f, ex_mat33f_t)
DEF_BUILTIN_SERIALIZE_2(mat44f, ex_mat44f_t)

DEF_BUILTIN_SERIALIZE_2(quatf, ex_quatf_t)
DEF_BUILTIN_SERIALIZE_2(angf, ex_angf_t)

DEF_BUILTIN_SERIALIZE_2(color3u, ex_color3u_t)
DEF_BUILTIN_SERIALIZE_2(color3f, ex_color3f_t)
DEF_BUILTIN_SERIALIZE_2(color4u, ex_color4u_t)
DEF_BUILTIN_SERIALIZE_2(color4f, ex_color4f_t)

DEF_BUILTIN_SERIALIZE_ARRAY(array, ex_array_t *)
DEF_BUILTIN_SERIALIZE_MAP(map, ex_hashmap_t *)

#undef DEF_BUILTIN_SERIALIZE
#undef DEF_BUILTIN_SERIALIZE_2
#undef DEF_BUILTIN_SERIALIZE_ARRAY
#undef DEF_BUILTIN_SERIALIZE_MAP

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END SERIALIZE_BUILTIN_H_1295082972
// #################################################################################


