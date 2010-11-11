// ======================================================================================
// File         : vec2f.h
// Author       : Wu Jie 
// Last Change  : 11/11/2010 | 11:08:19 AM | Thursday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef VEC2F_H_1289444900
#define VEC2F_H_1289444900
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

typedef struct vec2f_t {
    union {
        struct { float x, y; }; // end struct
        float v[2];
    };
} vec2f_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void vec2f_zero ( vec2f_t* _r ) { _r->x = 0.0f, _r->y = 0.0f; }
inline void vec2f_one ( vec2f_t* _r ) { _r->x = 1.0f, _r->y = 1.0f; }
inline void vec2f_set ( vec2f_t* _r, float _x, float _y ) { _r->x = _x, _r->y = _y; }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline bool vec2f_is_equal ( vec2f_t* _lhs, vec2f_t* _rhs ) { 
    return is_equal_float(_lhs->x,_rhs->x,EX_FLOAT_EPS)
        && is_equal_float(_lhs->y,_rhs->y,EX_FLOAT_EPS);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void vec2f_neg ( vec2f_t* _v ) { 
    _v->x = -_v->x; 
    _v->y = -_v->y; 
}
inline void vec2f_get_neg ( vec2f_t* _r, vec2f_t* _v ) { 
    ex_assert ( _r != _v, "can't use self as return value." );
    _r->x = -_v->x; 
    _r->y = -_v->y; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void vec2f_add ( vec2f_t* _r, vec2f_t* _lhs, vec2f_t* _rhs ) { 
    _r->x = _lhs->x + _rhs->x;
    _r->y = _lhs->y + _rhs->y;
}
inline void vec2f_add_scalar ( vec2f_t* _r, vec2f_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x + _rhs;
    _r->y = _lhs->y + _rhs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void vec2f_sub ( vec2f_t* _r, vec2f_t* _lhs, vec2f_t* _rhs ) { 
    _r->x = _lhs->x - _rhs->x;
    _r->y = _lhs->y - _rhs->y;
}
inline void vec2f_sub_scalar ( vec2f_t* _r, vec2f_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x - _rhs;
    _r->y = _lhs->y - _rhs;
}
inline void scalar_sub_vec2f ( vec2f_t* _r, float _lhs, vec2f_t* _rhs ) { 
    _r->x = _lhs - _rhs->x;
    _r->y = _lhs - _rhs->y;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void vec2f_mul ( vec2f_t* _r, vec2f_t* _lhs, vec2f_t* _rhs ) { 
    _r->x = _lhs->x * _rhs->x;
    _r->y = _lhs->y * _rhs->y;
}
inline void vec2f_mul_scalar ( vec2f_t* _r, vec2f_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x * _rhs;
    _r->y = _lhs->y * _rhs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void vec2f_div ( vec2f_t* _r, vec2f_t* _lhs, vec2f_t* _rhs ) { 
    _r->x = _lhs->x / _rhs->x;
    _r->y = _lhs->y / _rhs->y;
}
inline void vec2f_div_scalar ( vec2f_t* _r, vec2f_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x / _rhs;
    _r->y = _lhs->y / _rhs;
}
inline void scalar_div_vec2f ( vec2f_t* _r, float _lhs, vec2f_t* _rhs ) { 
    _r->x = _lhs / _rhs->x;
    _r->y = _lhs / _rhs->y;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline float vec2f_dot ( vec2f_t* _lhs, vec2f_t* _rhs ) {
    return _lhs->x * _rhs->x + _lhs->y * _rhs->y;
}
inline float vec2f_cross ( vec2f_t* _lhs, vec2f_t* _rhs ) {
    return _lhs->x * _rhs->y - _lhs->y * _rhs->x;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline float vec2f_len ( vec2f_t* _v ) {
    return sqrtf ( vec2f_dot(_v,_v) );
}
inline float vec2f_lenSQR ( vec2f_t* _v ) {
    return vec2f_dot(_v,_v);
}

// ------------------------------------------------------------------ 
// Desc: normalize
// ------------------------------------------------------------------ 

//
inline bool vec2f_is_normalized ( vec2f_t* _v ) {
    return is_equal_float( vec2f_lenSQR(_v), 1.0f, EX_FLOAT_EPS);
}

//
inline bool vec2f_normalize ( vec2f_t* _v ) {
    float length_sqr = vec2f_lenSQR(_v);
    float inv_length = 0.0f; 

    if ( is_equal_float( length_sqr, 1.0f, EX_FLOAT_EPS ) )
        return true;

    if( is_zero_float( length_sqr, EX_FLOAT_EPS ) )
        return false;

    inv_length = inv_sqrtf( length_sqr );
    vec2f_mul_scalar ( _v, _v, inv_length );
}

//
inline bool vec2f_get_normalize ( vec2f_t* _r, vec2f_t* _v ) {
    float length_sqr, inv_length;

    ex_assert ( _r != _v, "can't use self as return value." );
    length_sqr = vec2f_lenSQR(_v);
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
    vec2f_mul_scalar ( _r, _v, inv_length );
}

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END VEC2F_H_1289444900
// #################################################################################
