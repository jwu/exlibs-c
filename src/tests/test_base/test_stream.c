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
EX_DEF_CLASS_BEGIN(simple_t)
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
    char*           m_string;
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
EX_DEF_CLASS_END(simple_t)

EX_DEF_CLASS_CREATOR(simple_t) { return __alloc_simple_t(); }

EX_DEF_PROPS_BEGIN(simple_t)
EX_DEF_PROPS_END(simple_t)

EX_SERIALIZE_BEGIN(simple_t)
    EX_SERIALIZE ( int8, m_int8);
    EX_SERIALIZE ( int16, m_int16);
    EX_SERIALIZE ( int32, m_int32);
    EX_SERIALIZE ( int64, m_int64);
    EX_SERIALIZE ( uint8, m_uint8);
    EX_SERIALIZE ( uint16, m_uint16);
    EX_SERIALIZE ( uint32, m_uint32);
    EX_SERIALIZE ( uint64, m_uint64);
    EX_SERIALIZE ( float, m_float);
    EX_SERIALIZE ( double, m_double);
    EX_SERIALIZE ( boolean, m_bool);
    EX_SERIALIZE ( string, m_string);
    EX_SERIALIZE ( strid, m_strid);
    EX_SERIALIZE ( vec2f, m_vec2f);
    EX_SERIALIZE ( vec3f, m_vec3f);
    EX_SERIALIZE ( vec4f, m_vec4f);
    EX_SERIALIZE ( mat22f, m_mat22f);
    EX_SERIALIZE ( mat33f, m_mat33f);
    EX_SERIALIZE ( mat44f, m_mat44f);
    EX_SERIALIZE ( quatf, m_quatf);
    EX_SERIALIZE ( angf, m_angf);
    EX_SERIALIZE ( color3f, m_color3f);
    EX_SERIALIZE ( color3u, m_color3u);
    EX_SERIALIZE ( color4f, m_color4f);
    EX_SERIALIZE ( color4u, m_color4u);
EX_SERIALIZE_END

// complex
EX_DEF_CLASS_BEGIN(complex_t)
    ex_array_t *m_vec3f_list;
    ex_hashmap_t *m_strid_to_float;
EX_DEF_CLASS_END(complex_t)

EX_DEF_CLASS_CREATOR(complex_t) { 
    complex_t* obj = __alloc_complex_t(); 
    obj->m_vec3f_list = ex_array(vec3f,8);
    obj->m_strid_to_float = ex_hashmap( strid, float, 8 );
	return obj;
}

EX_DEF_PROPS_BEGIN(complex_t)
EX_DEF_PROPS_END(complex_t)

EX_SERIALIZE_BEGIN(complex_t)
    EX_SERIALIZE_ARRAY ( vec3f, m_vec3f_list);
    EX_SERIALIZE_MAP ( strid, float, m_strid_to_float);
EX_SERIALIZE_END

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void json_read () {
    char path[maxPATH];

    strncpy ( path, media_file, maxPATH );
    ex_stream_t *stream = ex_create_json_read_stream( strcat(path, "simple_json.json") );
    ex_destroy_json_stream((ex_stream_json_t *)stream);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void simple_read () {
    char path[maxPATH];
    ex_serialize_pfn serialize_func;
    simple_t *s1, *s2;

    EX_REGISTER_CLASS(simple_t);

    strncpy ( path, media_file, maxPATH );
    ex_stream_t *stream = ex_create_json_read_stream( strcat(path, "simple_read.json") );
    serialize_func = ex_rtti_get(EX_TYPEID(simple_t))->serialize;

    // serialize s1
    s1 = ex_create_simple_t();
    serialize_func(stream, ex_strid("simple_01"), s1 );

    // serialize s2
    s2 = ex_create_simple_t();
    serialize_func(stream, ex_strid("simple_02"), s2 );

    //
    ex_destroy_json_stream((ex_stream_json_t *)stream);

    // destroy
    ex_free(s1);
    ex_free(s2);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void complex_read () {
    char path[maxPATH];
    ex_serialize_pfn serialize_func;
    complex_t *c1, *c2;

    EX_REGISTER_CLASS(complex_t);

    strncpy ( path, media_file, maxPATH );
    ex_stream_t *stream = ex_create_json_read_stream( strcat(path, "complex_read.json") );
    serialize_func = ex_rtti_get(EX_TYPEID(complex_t))->serialize;

    // serialize c1
    c1 = ex_create_complex_t();
    serialize_func(stream, ex_strid("complex_01"), c1 );

    // serialize c2
    c2 = ex_create_complex_t();
    serialize_func(stream, ex_strid("complex_02"), c2 );

    //
    ex_destroy_json_stream((ex_stream_json_t *)stream);

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
    // json_read();
    // simple_read();
    complex_read();
}
