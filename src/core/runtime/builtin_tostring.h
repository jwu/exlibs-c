// ======================================================================================
// File         : builtin_tostring.h
// Author       : Wu Jie 
// Last Change  : 01/22/2011 | 00:01:00 AM | Saturday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef BUILTIN_TOSTRING_H_1295625662
#define BUILTIN_TOSTRING_H_1295625662
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// macros
///////////////////////////////////////////////////////////////////////////////

// DEF_BUILTIN_TOSTRING
#define DEF_BUILTIN_TOSTRING(_type) \
    static inline void __ex_tostring_##_type ( ex_string_t *_string, void *_val )

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

DEF_BUILTIN_TOSTRING(bool) {
    bool *val = (bool *)_val;

    if ( *val == true )
        ex_string_cat( _string, "true" );
    else
        ex_string_cat( _string, "false" );
}
DEF_BUILTIN_TOSTRING(int) {
    char buf[1024];
    int *val = (int *)_val;

    snprintf ( buf, 1024, "%d", *val );
    ex_string_ncat( _string, buf, strlen(buf) );
}
DEF_BUILTIN_TOSTRING(size_t) {
    char buf[1024];
    size_t *val = (size_t *)_val;

    snprintf ( buf, 1024, "%lu", *val );
    ex_string_ncat( _string, buf, strlen(buf) );
}
DEF_BUILTIN_TOSTRING(uid) {
    char buf[1024];
    ex_uid_t *val = (ex_uid_t *)_val;

    snprintf ( buf, 1024, "0x%.16llX", *val );
    ex_string_ncat( _string, buf, strlen(buf) );
}

DEF_BUILTIN_TOSTRING(int8) {
    char buf[1024];
    int8 *val = (int8 *)_val;

    snprintf ( buf, 1024, "%hhd", *val );
    ex_string_ncat( _string, buf, strlen(buf) );
}
DEF_BUILTIN_TOSTRING(int16) {
    char buf[1024];
    int16 *val = (int16 *)_val;

    snprintf ( buf, 1024, "%hd", *val );
    ex_string_ncat( _string, buf, strlen(buf) );
}
DEF_BUILTIN_TOSTRING(int32) {
    char buf[1024];
    int32 *val = (int32 *)_val;

    snprintf ( buf, 1024, "%d", *val );
    ex_string_ncat( _string, buf, strlen(buf) );
}
DEF_BUILTIN_TOSTRING(int64) {
    char buf[1024];
    int64 *val = (int64 *)_val;

    snprintf ( buf, 1024, "%lld", *val );
    ex_string_ncat( _string, buf, strlen(buf) );
}

DEF_BUILTIN_TOSTRING(uint8) {
    char buf[1024];
    uint8 *val = (uint8 *)_val;

    snprintf ( buf, 1024, "%hhu", *val );
    ex_string_ncat( _string, buf, strlen(buf) );
}
DEF_BUILTIN_TOSTRING(uint16) {
    char buf[1024];
    uint16 *val = (uint16 *)_val;

    snprintf ( buf, 1024, "%hu", *val );
    ex_string_ncat( _string, buf, strlen(buf) );
}
DEF_BUILTIN_TOSTRING(uint32) {
    char buf[1024];
    uint32 *val = (uint32 *)_val;

    snprintf ( buf, 1024, "%u", *val );
    ex_string_ncat( _string, buf, strlen(buf) );
}
DEF_BUILTIN_TOSTRING(uint64) {
    char buf[1024];
    uint64 *val = (uint64 *)_val;

    snprintf ( buf, 1024, "%llu", *val );
    ex_string_ncat( _string, buf, strlen(buf) );
}

DEF_BUILTIN_TOSTRING(float) {
    char buf[1024];
    float *val = (float *)_val;

    snprintf ( buf, 1024, "%f", *val );
    ex_string_ncat( _string, buf, strlen(buf) );
}
DEF_BUILTIN_TOSTRING(double) {
    char buf[1024];
    double *val = (double *)_val;

    snprintf ( buf, 1024, "%f", *val );
    ex_string_ncat( _string, buf, strlen(buf) );
}

DEF_BUILTIN_TOSTRING(cstr) {
    char **val = (char **)_val;

    if ( *val != NULL ) {
        ex_string_cat( _string, "\"" );
        ex_string_ncat( _string, *val, strlen(*val) );
        ex_string_cat( _string, "\"" );
    }
    else {
        ex_string_cat( _string, "\"\"" );
    }
}
DEF_BUILTIN_TOSTRING(string) {
    ex_string_t *val = (ex_string_t *)_val;

    ex_string_cat( _string, "\"" );
    ex_string_ncat( _string, val->text, val->len );
    ex_string_cat( _string, "\"" );
}
DEF_BUILTIN_TOSTRING(strid) {
    strid_t *val = (strid_t *)_val;
    const char *text = ex_strid_to_cstr(*val);

    if ( *val != EX_STRID_NULL ) {
        ex_string_cat( _string, "\"" );
        ex_string_ncat( _string, text, strlen(text) );
        ex_string_cat( _string, "\"" );
    }
    else {
        ex_string_cat( _string, "\"\"" );
    }
}

DEF_BUILTIN_TOSTRING(angf) {
    // NOTE: degrees symbol is alt-shift-8
    char buf[1024];
    ex_angf_t *val = (ex_angf_t *)_val;

    snprintf ( buf, 1024, "(r:%.3f, d:%.3f°)", val->rad, ex_angf_to_degrees_360(val) );
    ex_string_ncat( _string, buf, strlen(buf) );
}

DEF_BUILTIN_TOSTRING(vec2f) {
    char buf[1024];
    ex_vec2f_t *val = (ex_vec2f_t *)_val;

    snprintf ( buf, 1024, "(%.3f, %.3f)", val->x, val->y );
    ex_string_ncat( _string, buf, strlen(buf) );
}
DEF_BUILTIN_TOSTRING(vec3f) {
    char buf[1024];
    ex_vec3f_t *val = (ex_vec3f_t *)_val;

    snprintf ( buf, 1024, "(%.3f, %.3f, %.3f)", val->x, val->y, val->z );
    ex_string_ncat( _string, buf, strlen(buf) );
}
DEF_BUILTIN_TOSTRING(vec4f) {
    char buf[1024];
    ex_vec4f_t *val = (ex_vec4f_t *)_val;

    snprintf ( buf, 1024, "(%.3f, %.3f, %.3f, %.3f)", val->x, val->y, val->z, val->w );
    ex_string_ncat( _string, buf, strlen(buf) );
}

DEF_BUILTIN_TOSTRING(mat22f) {
    char buf[1024];
    ex_mat22f_t *val = (ex_mat22f_t *)_val;

    snprintf ( buf, 1024, "([%.3f, %.3f], [%.3f, %.3f])", 
               val->m00, val->m01, 
               val->m10, val->m11 );
    ex_string_ncat( _string, buf, strlen(buf) );
}
DEF_BUILTIN_TOSTRING(mat33f) {
    char buf[1024];
    ex_mat33f_t *val = (ex_mat33f_t *)_val;

    snprintf ( buf, 1024, "([%.3f, %.3f, %.3f], [%.3f, %.3f, %.3f], [%.3f, %.3f, %.3f])", 
               val->m00, val->m01, val->m02, 
               val->m10, val->m11, val->m12,
               val->m20, val->m21, val->m22 );
    ex_string_ncat( _string, buf, strlen(buf) );
}
DEF_BUILTIN_TOSTRING(mat44f) {
    char buf[1024];
    ex_mat44f_t *val = (ex_mat44f_t *)_val;

    snprintf ( buf, 1024, "([%.3f, %.3f, %.3f, %.3f], [%.3f, %.3f, %.3f, %.3f], [%.3f, %.3f, %.3f, %.3f], [%.3f, %.3f, %.3f, %.3f])", 
               val->m00, val->m01, val->m02, val->m03, 
               val->m10, val->m11, val->m12, val->m13,
               val->m20, val->m21, val->m22, val->m23,
               val->m30, val->m31, val->m32, val->m33 );
    ex_string_ncat( _string, buf, strlen(buf) );
}

DEF_BUILTIN_TOSTRING(quatf) {
    char buf[1024];
    ex_quatf_t *val = (ex_quatf_t *)_val;

    snprintf ( buf, 1024, "(%.3f, %.3f, %.3f, %.3f)", val->x, val->y, val->z, val->w );
    ex_string_ncat( _string, buf, strlen(buf) );
}

DEF_BUILTIN_TOSTRING(color3u) {
    char buf[1024];
    ex_color3u_t *val = (ex_color3u_t *)_val;

    snprintf ( buf, 1024, "(%hhu, %hhu, %hhu)", val->r, val->g, val->b );
    ex_string_ncat( _string, buf, strlen(buf) );
}
DEF_BUILTIN_TOSTRING(color3f) {
    char buf[1024];
    ex_color3f_t *val = (ex_color3f_t *)_val;

    snprintf ( buf, 1024, "(%.3f, %.3f, %.3f)", val->r, val->g, val->b );
    ex_string_ncat( _string, buf, strlen(buf) );
}
DEF_BUILTIN_TOSTRING(color4u) {
    char buf[1024];
    ex_color4u_t *val = (ex_color4u_t *)_val;

    snprintf ( buf, 1024, "(%hhu, %hhu, %hhu, %hhu)", val->r, val->g, val->b, val->a );
    ex_string_ncat( _string, buf, strlen(buf) );
}
DEF_BUILTIN_TOSTRING(color4f) {
    char buf[1024];
    ex_color4f_t *val = (ex_color4f_t *)_val;

    snprintf ( buf, 1024, "(%.3f, %.3f, %.3f, %.3f)", val->r, val->g, val->b, val->a );
    ex_string_ncat( _string, buf, strlen(buf) );
}

static inline void __ex_tostring_array_2 ( ex_string_t *_string, void *_val, void *_tostring_pfn ) {
    typedef void (*pfn) ( ex_string_t *, void *);
    ex_array_t *val = (ex_array_t *)_val;
    size_t idx = 0;

    ex_string_cat( _string, "[\n" );
    while ( idx < val->count ) {
        void *_el = ex_array_get(val,idx);

        ex_string_cat( _string, "\t" );
        ((pfn)_tostring_pfn)( _string, _el );
        ex_string_cat( _string, ",\n" );

        ++idx;
    }
    ex_string_cat( _string, "]" );
}
static inline void __ex_tostring_array ( ex_string_t *_string, void *_val ) {
    ex_array_t *val = (ex_array_t *)_val;
    void *element_tostring_pfn = ex_tostring_func(val->element_typeid);

    if ( element_tostring_pfn == NULL ) return; // the array not support generic to string
    __ex_tostring_array_2( _string, _val, element_tostring_pfn );
}

static inline void __ex_tostring_map_2 ( ex_string_t *_string, void *_val, 
                                         void *_key_tostring_pfn, void *_value_tostring_pfn ) 
{
    typedef void (*pfn) ( ex_string_t *, void *);
    ex_hashmap_t *val = (ex_hashmap_t *)_val;
    size_t count = ex_hashmap_count(val);
	size_t idx = 0;

    ex_string_cat( _string, "{\n" );
    while ( idx < count ) {
        void *key = (char *)val->keys + idx * val->key_bytes;
        void *value = (char *)val->values + idx * val->value_bytes;

        ex_string_cat( _string, "\t( key: " );
        ((pfn)_key_tostring_pfn)( _string, key );
        ex_string_cat( _string, ", value: " );
        ((pfn)_value_tostring_pfn)( _string, value );
        ex_string_cat( _string, " ),\n" );

        ++idx;
    }
    ex_string_cat( _string, "}" );
}
static inline void __ex_tostring_map ( ex_string_t *_string, void *_val ) {
    ex_hashmap_t *val = (ex_hashmap_t *)_val;
    void *key_tostring_pfn = ex_tostring_func(val->key_typeid);
    void *value_tostring_pfn = ex_tostring_func(val->value_typeid);

    if ( key_tostring_pfn == NULL || value_tostring_pfn == NULL ) return; // the map not support generic to string
    __ex_tostring_map_2( _string, _val, key_tostring_pfn, value_tostring_pfn );
}

DEF_BUILTIN_TOSTRING(ref) {
    char buf[1024];
    ex_ref_t *val = (ex_ref_t *)_val;

    snprintf ( buf, 1024, "uid: 0x%.16llX, valid: %s, refcount: %d", 
               val->uid, 
               *(val->isvalid) ? "true" : "false", 
               *(val->refcount) );
    ex_string_ncat( _string, buf, strlen(buf) );
}

#undef DEF_BUILTIN_TOSTRING

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END BUILTIN_TOSTRING_H_1295625662
// #################################################################################


