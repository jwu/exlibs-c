// ======================================================================================
// File         : vec3f.h
// Author       : Wu Jie 
// Last Change  : 11/11/2010 | 11:08:34 AM | Thursday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef VEC3F_H_1289444915
#define VEC3F_H_1289444915
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

typedef struct vec3f_t {
    union {
        struct { float x, y, z; }; // end struct
        float v[3];
    };
} vec3f_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void vec3f_zero ( vec3f_t* _r ) { _r->x = 0.0f, _r->y = 0.0f; _r->z = 0.0f; }
inline void vec3f_one ( vec3f_t* _r ) { _r->x = 1.0f, _r->y = 1.0f; _r->z = 1.0f; }
inline void vec3f_set ( vec3f_t* _r, float _x, float _y, float _z ) { _r->x = _x, _r->y = _y, _r->z = _z; }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline bool vec3f_is_equal ( vec3f_t* _lhs, vec3f_t* _rhs ) { 
    return is_equal_float(_lhs->x,_rhs->x,EX_FLOAT_EPS)
        && is_equal_float(_lhs->y,_rhs->y,EX_FLOAT_EPS)
        && is_equal_float(_lhs->z,_rhs->z,EX_FLOAT_EPS);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void vec3f_neg ( vec3f_t* _v ) { 
    _v->x = -_v->x; 
    _v->y = -_v->y; 
    _v->z = -_v->z; 
}
inline void vec3f_get_neg ( vec3f_t* _r, vec3f_t* _v ) { 
    ex_assert ( _r != _v, "can't use self as return value." );
    _r->x = -_v->x; 
    _r->y = -_v->y; 
    _r->z = -_v->z; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void vec3f_add ( vec3f_t* _r, vec3f_t* _lhs, vec3f_t* _rhs ) { 
    _r->x = _lhs->x + _rhs->x;
    _r->y = _lhs->y + _rhs->y;
    _r->z = _lhs->z + _rhs->z;
}
inline void vec3f_add_scalar ( vec3f_t* _r, vec3f_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x + _rhs;
    _r->y = _lhs->y + _rhs;
    _r->z = _lhs->z + _rhs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void vec3f_sub ( vec3f_t* _r, vec3f_t* _lhs, vec3f_t* _rhs ) { 
    _r->x = _lhs->x - _rhs->x;
    _r->y = _lhs->y - _rhs->y;
    _r->z = _lhs->z - _rhs->z;
}
inline void vec3f_sub_scalar ( vec3f_t* _r, vec3f_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x - _rhs;
    _r->y = _lhs->y - _rhs;
    _r->z = _lhs->z - _rhs;
}
inline void scalar_sub_vec3f ( vec3f_t* _r, float _lhs, vec3f_t* _rhs ) { 
    _r->x = _lhs - _rhs->x;
    _r->y = _lhs - _rhs->y;
    _r->z = _lhs - _rhs->z;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void vec3f_mul ( vec3f_t* _r, vec3f_t* _lhs, vec3f_t* _rhs ) { 
    _r->x = _lhs->x * _rhs->x;
    _r->y = _lhs->y * _rhs->y;
    _r->z = _lhs->z * _rhs->z;
}
inline void vec3f_mul_scalar ( vec3f_t* _r, vec3f_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x * _rhs;
    _r->y = _lhs->y * _rhs;
    _r->z = _lhs->z * _rhs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void vec3f_div ( vec3f_t* _r, vec3f_t* _lhs, vec3f_t* _rhs ) { 
    _r->x = _lhs->x / _rhs->x;
    _r->y = _lhs->y / _rhs->y;
    _r->z = _lhs->z / _rhs->z;
}
inline void vec3f_div_scalar ( vec3f_t* _r, vec3f_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x / _rhs;
    _r->y = _lhs->y / _rhs;
    _r->z = _lhs->z / _rhs;
}
inline void scalar_div_vec3f ( vec3f_t* _r, float _lhs, vec3f_t* _rhs ) { 
    _r->x = _lhs / _rhs->x;
    _r->y = _lhs / _rhs->y;
    _r->z = _lhs / _rhs->z;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline float vec3f_dot ( vec3f_t* _lhs, vec3f_t* _rhs ) {
    return _lhs->x * _rhs->x 
         + _lhs->y * _rhs->y 
         + _lhs->z * _rhs->z;
}
inline void vec3f_cross ( vec3f_t* _r, vec3f_t* _lhs, vec3f_t* _rhs ) {
    _r->x = _lhs->y * _rhs->z - _lhs->z * _rhs->y;
    _r->y = _lhs->z * _rhs->x - _lhs->x * _rhs->z;
    _r->z = _lhs->x * _rhs->y - _lhs->y * _rhs->x;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline float vec3f_len ( vec3f_t* _v ) {
    return sqrtf ( vec3f_dot(_v,_v) );
}
inline float vec3f_lenSQR ( vec3f_t* _v ) {
    return vec3f_dot(_v,_v);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

//
inline bool vec3f_is_normalized ( vec3f_t* _v ) {
    return is_equal_float( vec3f_lenSQR(_v), 1.0f, EX_FLOAT_EPS);
}

//
inline bool vec3f_normalize ( vec3f_t* _v ) {
    float length_sqr = vec3f_lenSQR(_v);
    float inv_length = 0.0f; 

    if ( is_equal_float( length_sqr, 1.0f, EX_FLOAT_EPS ) )
        return true;

    if( is_zero_float( length_sqr, EX_FLOAT_EPS ) )
        return false;

    inv_length = inv_sqrtf( length_sqr );
    vec3f_mul_scalar ( _v, _v, inv_length );
}

//
inline bool vec3f_get_normalize ( vec3f_t* _r, vec3f_t* _v ) {
    float length_sqr, inv_length;

    ex_assert ( _r != _v, "can't use self as return value." );
    length_sqr = vec3f_lenSQR(_v);
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
    vec3f_mul_scalar ( _r, _v, inv_length );
}

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END VEC3F_H_1289444915
// #################################################################################
