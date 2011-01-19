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
// defines
///////////////////////////////////////////////////////////////////////////////

#define DEF_BUILTIN_SERIALIZE(_type) \
    static inline void __ex_serialize_##_type ( ex_stream_t* _stream, strid_t _name, void* _val ) { \
        if ( _stream->next ) { \
            int ret = _stream->next( _stream, _name, EX_TYPEID(_type) ); \
            if ( ret != 0 ) return; \
        } \
        _stream->serialize_##_type(_stream,(_type*)_val); \
    }

#define DEF_BUILTIN_SERIALIZE_2( _typename, _type ) \
    static inline void __ex_serialize_##_typename ( ex_stream_t* _stream, strid_t _name, void* _val ) { \
        if ( _stream->next ) { \
            int ret = _stream->next( _stream, _name, EX_TYPEID(_typename) ); \
            if ( ret != 0 ) return; \
        } \
        _stream->serialize_##_typename(_stream,(_type*)_val); \
    }

#define DEF_BUILTIN_SERIALIZE_PUSH_POP( _typename, _type ) \
    static inline void __ex_serialize_##_typename ( ex_stream_t* _stream, strid_t _name, void* _val ) { \
        if ( _stream->next ) { \
            int ret = _stream->next( _stream, _name, EX_TYPEID(_typename) ); \
            if ( ret != 0 ) return; \
        } \
        if ( _stream->push ) _stream->push(_stream); \
        _stream->serialize_##_typename(_stream,(_type*)_val); \
        if ( _stream->pop ) _stream->pop(_stream); \
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

DEF_BUILTIN_SERIALIZE_2(string, char*)
DEF_BUILTIN_SERIALIZE_2(strid, strid_t)

DEF_BUILTIN_SERIALIZE_PUSH_POP(array, ex_array_t)
DEF_BUILTIN_SERIALIZE_PUSH_POP(map, ex_hashmap_t)

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

#undef DEF_BUILTIN_SERIALIZE
#undef DEF_BUILTIN_SERIALIZE_2

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END SERIALIZE_BUILTIN_H_1295082972
// #################################################################################


