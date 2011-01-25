// ======================================================================================
// File         : test_stream.c
// Author       : Wu Jie 
// Last Change  : 01/17/2011 | 15:58:01 PM | Monday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "../../core/core_inc.h"
#include "../../entity/trans2d.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

#if (EX_PLATFORM == EX_WIN32)
    static const char *media_file = "e:/project/dev/exsdk/res/";
#else
    static const char *media_file = "/Users/Johnny/dev/projects/exdev/exsdk/res/";
#endif
#define maxPATH 256

// simple
EX_DECL_CLASS_BEGIN(simple_t)
    int8            m_int8;
    int16           m_int16;
    int32           m_int32;
    int64           m_int64;
    uint8           m_uint8;
    uint16          m_uint16;
    uint32          m_uint32;
    uint64          m_uint64;
    float           m_float;
    double          m_double;
    bool            m_bool;
    char*           m_cstr;
    ex_string_t*    m_string;
    strid_t         m_strid;
    ex_vec2f_t      m_vec2f;
    ex_vec3f_t      m_vec3f;
    ex_vec4f_t      m_vec4f;
    ex_mat22f_t     m_mat22f;
    ex_mat33f_t     m_mat33f;
    ex_mat44f_t     m_mat44f;
    ex_quatf_t      m_quatf;
    ex_angf_t       m_angf;
    ex_color3f_t    m_color3f;
    ex_color3u_t    m_color3u;
    ex_color4f_t    m_color4f;
    ex_color4u_t    m_color4u;
EX_DECL_CLASS_END(simple_t)

EX_DEF_CLASS_BEGIN(simple_t)
EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(simple_t)
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN(simple_t)
    EX_SERIALIZE ( int8, m_int8 );
    EX_SERIALIZE ( int16, m_int16 );
    EX_SERIALIZE ( int32, m_int32 );
    EX_SERIALIZE ( int64, m_int64 );
    EX_SERIALIZE ( uint8, m_uint8 );
    EX_SERIALIZE ( uint16, m_uint16 );
    EX_SERIALIZE ( uint32, m_uint32 );
    EX_SERIALIZE ( uint64, m_uint64 );
    EX_SERIALIZE ( float, m_float );
    EX_SERIALIZE ( double, m_double );
    EX_SERIALIZE ( bool, m_bool );
    EX_SERIALIZE ( cstr, m_cstr );
    EX_SERIALIZE_STRING ( m_string );
    EX_SERIALIZE ( strid, m_strid );
    EX_SERIALIZE ( vec2f, m_vec2f );
    EX_SERIALIZE ( vec3f, m_vec3f );
    EX_SERIALIZE ( vec4f, m_vec4f );
    EX_SERIALIZE ( mat22f, m_mat22f );
    EX_SERIALIZE ( mat33f, m_mat33f );
    EX_SERIALIZE ( mat44f, m_mat44f );
    EX_SERIALIZE ( quatf, m_quatf );
    EX_SERIALIZE ( angf, m_angf );
    EX_SERIALIZE ( color3f, m_color3f );
    EX_SERIALIZE ( color3u, m_color3u );
    EX_SERIALIZE ( color4f, m_color4f );
    EX_SERIALIZE ( color4u, m_color4u );
EX_SERIALIZE_END

EX_DEF_TOSTRING_BEGIN(simple_t)
    EX_MEMBER_TOSTRING ( int8,      "m_int8",       (self->m_int8) );
    EX_MEMBER_TOSTRING ( int16,     "m_int16",      (self->m_int16) );
    EX_MEMBER_TOSTRING ( int32,     "m_int32",      (self->m_int32) );
    EX_MEMBER_TOSTRING ( int64,     "m_int64",      (self->m_int64) );
    EX_MEMBER_TOSTRING ( uint8,     "m_uint8",      (self->m_uint8) );
    EX_MEMBER_TOSTRING ( uint16,    "m_uint16",     (self->m_uint16) );
    EX_MEMBER_TOSTRING ( uint32,    "m_uint32",     (self->m_uint32) );
    EX_MEMBER_TOSTRING ( uint64,    "m_uint64",     (self->m_uint64) );
    EX_MEMBER_TOSTRING ( float,     "m_float",      (self->m_float) );
    EX_MEMBER_TOSTRING ( double,    "m_double",     (self->m_double) );
    EX_MEMBER_TOSTRING ( bool,      "m_bool",       (self->m_bool) );
    EX_MEMBER_TOSTRING ( cstr,      "m_cstr",       (self->m_cstr) );
    EX_MEMBER_TOSTRING ( string,    "m_string",    *(self->m_string) );
    EX_MEMBER_TOSTRING ( strid,     "m_strid",      (self->m_strid) );
    EX_MEMBER_TOSTRING ( vec2f,     "m_vec2f",      (self->m_vec2f) );
    EX_MEMBER_TOSTRING ( vec3f,     "m_vec3f",      (self->m_vec3f) );
    EX_MEMBER_TOSTRING ( vec4f,     "m_vec4f",      (self->m_vec4f) );
    EX_MEMBER_TOSTRING ( mat22f,    "m_mat22f",     (self->m_mat22f) );
    EX_MEMBER_TOSTRING ( mat33f,    "m_mat33f",     (self->m_mat33f) );
    EX_MEMBER_TOSTRING ( mat44f,    "m_mat44f",     (self->m_mat44f) );
    EX_MEMBER_TOSTRING ( quatf,     "m_quatf",      (self->m_quatf) );
    EX_MEMBER_TOSTRING ( angf,      "m_angf",       (self->m_angf) );
    EX_MEMBER_TOSTRING ( color3f,   "m_color3f",    (self->m_color3f) );
    EX_MEMBER_TOSTRING ( color3u,   "m_color3u",    (self->m_color3u) );
    EX_MEMBER_TOSTRING ( color4f,   "m_color4f",    (self->m_color4f) );
    EX_MEMBER_TOSTRING ( color4u,   "m_color4u",    (self->m_color4u) );
EX_DEF_TOSTRING_END

void init_simple_t ( simple_t *_self ) {
    _self->m_cstr = NULL; 
    _self->m_string = ex_string(""); 
    _self->m_strid = EX_STRID_NULL; 
}

// complex
EX_DECL_CLASS_BEGIN(complex_t)
    ex_array_t *m_vec3f_list;
    ex_hashmap_t *m_strid_to_float;
EX_DECL_CLASS_END(complex_t)

EX_DEF_CLASS_BEGIN(complex_t)
EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(complex_t)
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN(complex_t)
    EX_SERIALIZE_ARRAY ( vec3f, m_vec3f_list);
    EX_SERIALIZE_MAP ( strid, float, m_strid_to_float);
EX_SERIALIZE_END

EX_DEF_TOSTRING_BEGIN(complex_t)
    EX_MEMBER_TOSTRING( array, "m_vec3f_list", *(self->m_vec3f_list) );
    EX_MEMBER_TOSTRING( map, "m_strid_to_float", *(self->m_strid_to_float) );
EX_DEF_TOSTRING_END

void init_complex_t ( complex_t *_self ) {
    _self->m_vec3f_list = ex_array(vec3f,8);
    _self->m_strid_to_float = ex_hashmap( strid, float, 8 );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void json_read_write () {
    char path[maxPATH];
    ex_stream_t *stream = NULL;

    strncpy ( path, media_file, maxPATH );
    stream = ex_create_json_read_stream( strcat(path, "simple_read.json") );
    stream->save_to_file ( stream, strcat(path, "simple_read_to_write.json" ) );
    ex_destroy_json_stream((ex_stream_json_t *)stream);

    strncpy ( path, media_file, maxPATH );
    stream = ex_create_json_read_stream( strcat(path, "complex_read.json") );
    stream->save_to_file ( stream, strcat(path, "complex_read_to_write.json" ) );
    ex_destroy_json_stream((ex_stream_json_t *)stream);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void simple_read_write () {
    char path[maxPATH];
    ex_serialize_pfn serialize_func;
    simple_t *s1, *s2;
    ex_string_t *str = ex_string("");
    ex_stream_t *stream_read, *stream_write;

    EX_REGISTER_CLASS(simple_t);
    serialize_func = ex_rtti_get(EX_TYPEID(simple_t))->serialize;

    // serialize read s1, s2 from simple_read.json
    strncpy ( path, media_file, maxPATH );
    stream_read = ex_create_json_read_stream( strcat(path, "simple_read.json") );

    s1 = ex_create_simple_t();
    init_simple_t(s1);
    serialize_func(stream_read, ex_strid("simple_01"), s1 );

    s2 = ex_create_simple_t();
    init_simple_t(s2);
    serialize_func(stream_read, ex_strid("simple_02"), s2 );
    ex_destroy_json_stream((ex_stream_json_t *)stream_read);

    // serialize write s1, s2 to simple_write.json
    stream_write = ex_create_json_write_stream();
    serialize_func(stream_write, ex_strid("simple_01"), s1 );
    serialize_func(stream_write, ex_strid("simple_02"), s2 );
    strncpy ( path, media_file, maxPATH );
    stream_write->save_to_file ( stream_write, strcat(path, "simple_write.json" ) );
    ex_destroy_json_stream((ex_stream_json_t *)stream_write);

    // destroy
    ex_string_free(s1->m_string);
    ex_free(s1->m_cstr);
    ex_free(s1);

    ex_string_free(s2->m_string);
    ex_free(s2->m_cstr);
    ex_free(s2);

    // try to serialize s1, s2 from simple_write.json
    strncpy ( path, media_file, maxPATH );
    stream_read = ex_create_json_read_stream( strcat(path, "simple_write.json") );

    s1 = ex_create_simple_t();
    init_simple_t(s1);
    serialize_func(stream_read, ex_strid("simple_01"), s1 );

    s2 = ex_create_simple_t();
    init_simple_t(s2);
    serialize_func(stream_read, ex_strid("simple_02"), s2 );

    ex_destroy_json_stream((ex_stream_json_t *)stream_read);

    ex_tostring( EX_TYPEID(simple_t), str, s1 );
    printf ("simple_01 is %s", str->text );
    ex_string_clear(str);
    ex_tostring( EX_TYPEID(simple_t), str, s2 );
    printf ("simple_02 is %s", str->text );
    ex_string_free(str);

    // destroy
    ex_string_free(s1->m_string);
    ex_free(s1->m_cstr);
    ex_free(s1);

    ex_string_free(s2->m_string);
    ex_free(s2->m_cstr);
    ex_free(s2);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void complex_read_write () {
    char path[maxPATH];
    ex_serialize_pfn serialize_func;
    complex_t *c1, *c2;
    ex_string_t *str = ex_string("");
    ex_stream_t *stream_read, *stream_write;

    EX_REGISTER_CLASS(complex_t);
    serialize_func = ex_rtti_get(EX_TYPEID(complex_t))->serialize;

    // serialize read c1, c2 from complex_read.json
    strncpy ( path, media_file, maxPATH );
    stream_read = ex_create_json_read_stream( strcat(path, "complex_read.json") );

    c1 = ex_create_complex_t();
    init_complex_t(c1);
    serialize_func(stream_read, ex_strid("complex_01"), c1 );

    c2 = ex_create_complex_t();
    init_complex_t(c2);
    serialize_func(stream_read, ex_strid("complex_02"), c2 );

    ex_destroy_json_stream((ex_stream_json_t *)stream_read);

    // serialize write c1, c2 to complex_write.json
    strncpy ( path, media_file, maxPATH );
    stream_write = ex_create_json_write_stream();

    serialize_func(stream_write, ex_strid("complex_01"), c1 );
    serialize_func(stream_write, ex_strid("complex_02"), c2 );

    strncpy ( path, media_file, maxPATH );
    stream_write->save_to_file ( stream_write, strcat(path, "complex_write.json" ) );
    ex_destroy_json_stream((ex_stream_json_t *)stream_write);

    // destroy
    ex_array_free(c1->m_vec3f_list);
    ex_hashmap_free(c1->m_strid_to_float);
    ex_free(c1);
    ex_array_free(c2->m_vec3f_list);
    ex_hashmap_free(c2->m_strid_to_float);
    ex_free(c2);

    // try to serialize c1, c2 from complex_write.json 
    strncpy ( path, media_file, maxPATH );
    stream_read = ex_create_json_read_stream( strcat(path, "complex_write.json") );

    c1 = ex_create_complex_t();
    init_complex_t(c1);
    serialize_func(stream_read, ex_strid("complex_01"), c1 );

    c2 = ex_create_complex_t();
    init_complex_t(c2);
    serialize_func(stream_read, ex_strid("complex_02"), c2 );

    ex_destroy_json_stream((ex_stream_json_t *)stream_read);

    ex_tostring( EX_TYPEID(complex_t), str, c1 );
    printf ("complex_01 is %s", str->text );
    ex_string_clear(str);
    ex_tostring( EX_TYPEID(complex_t), str, c2 );
    printf ("complex_02 is %s", str->text );
    ex_string_free(str);

    // destroy
    ex_array_free(c1->m_vec3f_list);
    ex_hashmap_free(c1->m_strid_to_float);
    ex_free(c1);
    ex_array_free(c2->m_vec3f_list);
    ex_hashmap_free(c2->m_strid_to_float);
    ex_free(c2);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_stream () {
    // json_read_write();
    simple_read_write();
    complex_read_write();
}
