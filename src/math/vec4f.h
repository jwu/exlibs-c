// ======================================================================================
// File         : vec4f.h
// Author       : Wu Jie 
// Last Change  : 11/11/2010 | 11:19:25 AM | Thursday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef VEC4F_H_1289445568
#define VEC4F_H_1289445568
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

typedef struct vec4f_t {
    union {
        struct { float x, y, z, w; }; // end struct
        float v[4];
    };
} vec4f_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void vec4f_zero ( vec4f_t* _r ) { _r->x = 0.0f, _r->y = 0.0f; _r->z = 0.0f; _r->w = 0.0f; }
inline void vec4f_one ( vec4f_t* _r ) { _r->x = 1.0f, _r->y = 1.0f; _r->z = 1.0f; _r->w = 1.0f; }
inline void vec4f_set ( vec4f_t* _r, float _x, float _y, float _z, float _w ) { _r->x = _x, _r->y = _y, _r->z = _z; _r->w = _w; }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline bool vec4f_is_equal ( vec4f_t* _lhs, vec4f_t* _rhs ) { 
    return is_equal_float(_lhs->x,_rhs->x,EX_FLOAT_EPS)
        && is_equal_float(_lhs->y,_rhs->y,EX_FLOAT_EPS)
        && is_equal_float(_lhs->z,_rhs->z,EX_FLOAT_EPS)
        && is_equal_float(_lhs->w,_rhs->w,EX_FLOAT_EPS);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void vec4f_neg ( vec4f_t* _v ) { 
    _v->x = -_v->x; 
    _v->y = -_v->y; 
    _v->z = -_v->z; 
    _v->w = -_v->w; 
}
inline void vec4f_get_neg ( vec4f_t* _r, vec4f_t* _v ) { 
    ex_assert ( _r != _v, "can't use self as return value." );
    _r->x = -_v->x; 
    _r->y = -_v->y; 
    _r->z = -_v->z; 
    _r->w = -_v->w; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void vec4f_add ( vec4f_t* _r, vec4f_t* _lhs, vec4f_t* _rhs ) { 
    _r->x = _lhs->x + _rhs->x;
    _r->y = _lhs->y + _rhs->y;
    _r->z = _lhs->z + _rhs->z;
    _r->w = _lhs->w + _rhs->w;
}
inline void vec4f_add_scalar ( vec4f_t* _r, vec4f_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x + _rhs;
    _r->y = _lhs->y + _rhs;
    _r->z = _lhs->z + _rhs;
    _r->w = _lhs->w + _rhs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void vec4f_sub ( vec4f_t* _r, vec4f_t* _lhs, vec4f_t* _rhs ) { 
    _r->x = _lhs->x - _rhs->x;
    _r->y = _lhs->y - _rhs->y;
    _r->z = _lhs->z - _rhs->z;
    _r->w = _lhs->w - _rhs->w;
}
inline void vec4f_sub_scalar ( vec4f_t* _r, vec4f_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x - _rhs;
    _r->y = _lhs->y - _rhs;
    _r->z = _lhs->z - _rhs;
    _r->w = _lhs->w - _rhs;
}
inline void scalar_sub_vec4f ( vec4f_t* _r, float _lhs, vec4f_t* _rhs ) { 
    _r->x = _lhs - _rhs->x;
    _r->y = _lhs - _rhs->y;
    _r->z = _lhs - _rhs->z;
    _r->w = _lhs - _rhs->w;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void vec4f_mul ( vec4f_t* _r, vec4f_t* _lhs, vec4f_t* _rhs ) { 
    _r->x = _lhs->x * _rhs->x;
    _r->y = _lhs->y * _rhs->y;
    _r->z = _lhs->z * _rhs->z;
    _r->w = _lhs->w * _rhs->w;
}
inline void vec4f_mul_scalar ( vec4f_t* _r, vec4f_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x * _rhs;
    _r->y = _lhs->y * _rhs;
    _r->z = _lhs->z * _rhs;
    _r->w = _lhs->w * _rhs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void vec4f_div ( vec4f_t* _r, vec4f_t* _lhs, vec4f_t* _rhs ) { 
    _r->x = _lhs->x / _rhs->x;
    _r->y = _lhs->y / _rhs->y;
    _r->z = _lhs->z / _rhs->z;
    _r->w = _lhs->w / _rhs->w;
}
inline void vec4f_div_scalar ( vec4f_t* _r, vec4f_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x / _rhs;
    _r->y = _lhs->y / _rhs;
    _r->z = _lhs->z / _rhs;
    _r->w = _lhs->w / _rhs;
}
inline void scalar_div_vec4f ( vec4f_t* _r, float _lhs, vec4f_t* _rhs ) { 
    _r->x = _lhs / _rhs->x;
    _r->y = _lhs / _rhs->y;
    _r->z = _lhs / _rhs->z;
    _r->w = _lhs / _rhs->w;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline float vec4f_dot ( vec4f_t* _lhs, vec4f_t* _rhs ) {
    return _lhs->x * _rhs->x 
         + _lhs->y * _rhs->y 
         + _lhs->z * _rhs->z 
         + _lhs->w * _rhs->w;
}
inline void vec4f_cross ( vec4f_t* _r, vec4f_t* _lhs, vec4f_t* _rhs ) {
    _r->x = _lhs->y * _rhs->z - _lhs->z * _rhs->y;
    _r->y = _lhs->z * _rhs->x - _lhs->x * _rhs->z;
    _r->z = _lhs->x * _rhs->y - _lhs->y * _rhs->x;
    _r->w = 0.0f;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline float vec4f_len ( vec4f_t* _v ) {
    return sqrtf ( vec4f_dot(_v,_v) );
}
inline float vec4f_lenSQR ( vec4f_t* _v ) {
    return vec4f_dot(_v,_v);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

//
inline bool vec4f_is_normalized ( vec4f_t* _v ) {
    return is_equal_float( vec4f_lenSQR(_v), 1.0f, EX_FLOAT_EPS);
}

//
inline bool vec4f_normalize ( vec4f_t* _v ) {
    float length_sqr = vec4f_lenSQR(_v);
    float inv_length = 0.0f; 

    if ( is_equal_float( length_sqr, 1.0f, EX_FLOAT_EPS ) )
        return true;

    if( is_zero_float( length_sqr, EX_FLOAT_EPS ) )
        return false;

    inv_length = inv_sqrtf( length_sqr );
    vec4f_mul_scalar ( _v, _v, inv_length );
}

//
inline bool vec4f_get_normalize ( vec4f_t* _r, vec4f_t* _v ) {
    float length_sqr, inv_length;

    ex_assert ( _r != _v, "can't use self as return value." );
    length_sqr = vec4f_lenSQR(_v);
    inv_length = 0.0f; 

    if ( is_equal_float( length_sqr, 1.0f, EX_FLOAT_EPS ) ) {
        _r = _v;
        return true;
    }

    if( is_zero_float( length_sqr, EX_FLOAT_EPS ) ) {
        _r = _v;
        return false;
    }

    inv_length = inv_sqrtf( length_sqr );
    vec4f_mul_scalar ( _r, _v, inv_length );
}

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END VEC4F_H_1289445568
// #################################################################################
