// ======================================================================================
// File         : stream.h
// Author       : Wu Jie 
// Last Change  : 01/17/2011 | 09:24:45 AM | Monday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef STREAM_H_1295227487
#define STREAM_H_1295227487
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

#define EX_STREAM_READ  0
#define EX_STREAM_WRITE 1

// ------------------------------------------------------------------ 
// Desc: ex_stream_t
// ------------------------------------------------------------------ 

typedef struct ex_stream_t {
    // properties
    int type; // read or write

    // common functions
    int (*save_to_file) ( struct ex_stream_t *_stream, const char *_filename );
    int (*next) ( struct ex_stream_t *_stream, strid_t _name, strid_t _typeID );
    void (*push) ( struct ex_stream_t *_stream );
    void (*pop) ( struct ex_stream_t *_stream );

    // serialize methods
    void (*serialize_bool)      ( struct ex_stream_t *_stream, bool *_val );
    void (*serialize_int)       ( struct ex_stream_t *_stream, int *_val );
    void (*serialize_size_t)    ( struct ex_stream_t *_stream, size_t *_val );
    void (*serialize_uid)       ( struct ex_stream_t *_stream, ex_uid_t *_val );

    void (*serialize_int8)      ( struct ex_stream_t *_stream, int8 *_val );
    void (*serialize_int16)     ( struct ex_stream_t *_stream, int16 *_val );
    void (*serialize_int32)     ( struct ex_stream_t *_stream, int32 *_val );
    void (*serialize_int64)     ( struct ex_stream_t *_stream, int64 *_val );

    void (*serialize_uint8)     ( struct ex_stream_t *_stream, uint8 *_val );
    void (*serialize_uint16)    ( struct ex_stream_t *_stream, uint16 *_val );
    void (*serialize_uint32)    ( struct ex_stream_t *_stream, uint32 *_val );
    void (*serialize_uint64)    ( struct ex_stream_t *_stream, uint64 *_val );

    void (*serialize_float)     ( struct ex_stream_t *_stream, float *_val );
    void (*serialize_double)    ( struct ex_stream_t *_stream, double *_val );

    void (*serialize_cstr)      ( struct ex_stream_t *_stream, char **_val );
    void (*serialize_string)    ( struct ex_stream_t *_stream, ex_string_t *_val );
    void (*serialize_strid)     ( struct ex_stream_t *_stream, strid_t *_val );

    void (*serialize_angf)      ( struct ex_stream_t *_stream, ex_angf_t *_val );

    void (*serialize_vec2f)     ( struct ex_stream_t *_stream, ex_vec2f_t *_val );
    void (*serialize_vec3f)     ( struct ex_stream_t *_stream, ex_vec3f_t *_val );
    void (*serialize_vec4f)     ( struct ex_stream_t *_stream, ex_vec4f_t *_val );

    void (*serialize_mat22f)    ( struct ex_stream_t *_stream, ex_mat22f_t *_val );
    void (*serialize_mat33f)    ( struct ex_stream_t *_stream, ex_mat33f_t *_val );
    void (*serialize_mat44f)    ( struct ex_stream_t *_stream, ex_mat44f_t *_val );

    void (*serialize_quatf)     ( struct ex_stream_t *_stream, ex_quatf_t *_val );

    void (*serialize_color3u)   ( struct ex_stream_t *_stream, ex_color3u_t *_val );
    void (*serialize_color3f)   ( struct ex_stream_t *_stream, ex_color3f_t *_val );
    void (*serialize_color4u)   ( struct ex_stream_t *_stream, ex_color4u_t *_val );
    void (*serialize_color4f)   ( struct ex_stream_t *_stream, ex_color4f_t *_val );

    void (*serialize_array)     ( struct ex_stream_t *_stream, ex_array_t *_val, ex_serialize_pfn _pfn_serialize_el );
    void (*serialize_map)       ( struct ex_stream_t *_stream, ex_hashmap_t *_val, ex_serialize_pfn _pfn_serialize_key, ex_serialize_pfn _pfn_serialize_val );
    void (*serialize_ref)       ( struct ex_stream_t *_stream, ex_ref_t *_val );

} ex_stream_t;

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END STREAM_H_1295227487
// #################################################################################
