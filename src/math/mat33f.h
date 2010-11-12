// ======================================================================================
// File         : mat33f.h
// Author       : Wu Jie 
// Last Change  : 11/12/2010 | 11:18:58 AM | Friday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef MAT33F_H_1289531940
#define MAT33F_H_1289531940
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "vec3f.h"

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

//! ------------------------------------------------------------------ 
//! \struct mat33f_t
//! \brief Matrix 2x2
//! ------------------------------------------------------------------ 

typedef struct mat33f_t {
    union {
        struct { 
            float m00, m01, m02;
            float m10, m11, m12;
            float m20, m21, m22;
        }; // end struct
        float m[9];
    };
} mat33f_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void mat33f_zero ( mat33f_t* _m ) { 
    _m->m00 = 0.0f, _m->m01 = 0.0f, _m->m02 = 0.0f; 
    _m->m10 = 0.0f, _m->m11 = 0.0f, _m->m12 = 0.0f; 
    _m->m20 = 0.0f, _m->m21 = 0.0f, _m->m22 = 0.0f; 
}
inline void mat33f_identity ( mat33f_t* _m ) { 
    _m->m00 = 1.0f, _m->m01 = 0.0f, _m->m02 = 0.0f; 
    _m->m10 = 0.0f, _m->m11 = 1.0f, _m->m12 = 0.0f; 
    _m->m20 = 0.0f, _m->m21 = 0.0f, _m->m22 = 1.0f; 
}
inline void mat33f_set ( mat33f_t* _m, 
                         float _m00, float _m01, float _m02,
                         float _m10, float _m11, float _m12,
                         float _m20, float _m21, float _m22 ) { 
    _m->m00 = _m00, _m->m01 = _m01, _m->m02 = _m02;
    _m->m10 = _m10, _m->m11 = _m11, _m->m12 = _m12; 
    _m->m20 = _m20, _m->m21 = _m21, _m->m22 = _m22; 
}
inline float mat33f_get ( mat33f_t* _m, uint _row, uint _col ) { 
    ex_assert( _row >= 0 && _row < 3, "out of range" );
    ex_assert( _col >= 0 && _col < 3, "out of range" );
    return _m->m[3*_row+_col];
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// row =============
inline void mat33f_row ( vec3f_t* _r, mat33f_t* _m, uint _row ) {
    ex_assert( _row >= 0 && _row < 3, "out of range" );
    _r->x = _m->m[3*_row+0]; 
    _r->y = _m->m[3*_row+1]; 
    _r->z = _m->m[3*_row+2]; 
}

// col
// ||
// ||
// ||
// ||
inline void mat33f_col ( vec3f_t* _r, mat33f_t* _m, uint _col ) {
    ex_assert( _col >= 0 && _col < 3, "out of range" );
    _r->x = _m->m[3*0+_col]; 
    _r->y = _m->m[3*1+_col]; 
    _r->z = _m->m[3*2+_col]; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void mat33f_neg ( mat33f_t* _m ) {
    _m->m00 = -_m->m00, _m->m01 = -_m->m01, _m->m02 = -_m->m02; 
    _m->m10 = -_m->m10, _m->m11 = -_m->m11, _m->m12 = -_m->m12; 
    _m->m20 = -_m->m20, _m->m21 = -_m->m21, _m->m22 = -_m->m22; 
}

inline void mat33f_get_neg ( mat33f_t* _r, mat33f_t* _m ) {
    ex_assert ( _r != _m, "can't use self as return value." );
    _r->m00 = -_m->m00, _r->m01 = -_m->m01, _r->m02 = -_m->m02; 
    _r->m10 = -_m->m10, _r->m11 = -_m->m11, _r->m12 = -_m->m12; 
    _r->m20 = -_m->m20, _r->m21 = -_m->m21, _r->m22 = -_m->m22; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void mat33f_abs ( mat33f_t* _m ) {
    _m->m00 = fabsf(_m->m00), _m->m01 = fabsf(_m->m01), _m->m02 = fabsf(_m->m02); 
    _m->m10 = fabsf(_m->m10), _m->m11 = fabsf(_m->m11), _m->m12 = fabsf(_m->m12); 
    _m->m20 = fabsf(_m->m20), _m->m21 = fabsf(_m->m21), _m->m22 = fabsf(_m->m22); 
}

inline void mat33f_get_abs ( mat33f_t* _r, mat33f_t* _m ) {
    ex_assert ( _r != _m, "can't use self as return value." );
    _r->m00 = fabsf(_m->m00), _r->m01 = fabsf(_m->m01), _r->m02 = fabsf(_m->m02); 
    _r->m10 = fabsf(_m->m10), _r->m11 = fabsf(_m->m11), _r->m12 = fabsf(_m->m12); 
    _r->m20 = fabsf(_m->m20), _r->m21 = fabsf(_m->m21), _r->m22 = fabsf(_m->m22); 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void mat33f_add ( mat33f_t* _r, mat33f_t* _lhs, mat33f_t* _rhs ) {
    _r->m00 = _lhs->m00 + _rhs->m00; 
    _r->m01 = _lhs->m01 + _rhs->m01; 
    _r->m02 = _lhs->m02 + _rhs->m02;

    _r->m10 = _lhs->m10 + _rhs->m10; 
    _r->m11 = _lhs->m11 + _rhs->m11;
    _r->m12 = _lhs->m12 + _rhs->m12;

    _r->m20 = _lhs->m20 + _rhs->m20; 
    _r->m21 = _lhs->m21 + _rhs->m21;
    _r->m22 = _lhs->m22 + _rhs->m22;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void mat33f_sub ( mat33f_t* _r, mat33f_t* _lhs, mat33f_t* _rhs ) {
    _r->m00 = _lhs->m00 - _rhs->m00; 
    _r->m01 = _lhs->m01 - _rhs->m01;
    _r->m02 = _lhs->m02 - _rhs->m02;

    _r->m10 = _lhs->m10 - _rhs->m10;
    _r->m11 = _lhs->m11 - _rhs->m11;
    _r->m12 = _lhs->m12 - _rhs->m12;

    _r->m20 = _lhs->m20 - _rhs->m20;
    _r->m21 = _lhs->m21 - _rhs->m21;
    _r->m22 = _lhs->m22 - _rhs->m22;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void mat33f_mul ( mat33f_t* _r, mat33f_t* _lhs, mat33f_t* _rhs ) {
    float m00, m01, m02, m10, m11, m12, m20, m21, m22;

    m00 = _lhs->m00 * _rhs->m00 + _lhs->m01 * _rhs->m10 + _lhs->m02 * _rhs->m20;
    m01 = _lhs->m00 * _rhs->m01 + _lhs->m01 * _rhs->m11 + _lhs->m02 * _rhs->m21;
    m02 = _lhs->m00 * _rhs->m02 + _lhs->m01 * _rhs->m12 + _lhs->m02 * _rhs->m22;

    m10 = _lhs->m10 * _rhs->m00 + _lhs->m11 * _rhs->m10 + _lhs->m12 * _rhs->m20;
    m11 = _lhs->m10 * _rhs->m01 + _lhs->m11 * _rhs->m11 + _lhs->m12 * _rhs->m21;
    m12 = _lhs->m10 * _rhs->m02 + _lhs->m11 * _rhs->m12 + _lhs->m12 * _rhs->m22;

    m20 = _lhs->m20 * _rhs->m00 + _lhs->m21 * _rhs->m10 + _lhs->m22 * _rhs->m20;
    m21 = _lhs->m20 * _rhs->m01 + _lhs->m21 * _rhs->m11 + _lhs->m22 * _rhs->m21;
    m22 = _lhs->m20 * _rhs->m02 + _lhs->m21 * _rhs->m12 + _lhs->m22 * _rhs->m22;

    mat33f_set( _r, 
                m00, m01, m02,
                m10, m11, m12,
                m20, m21, m22 );
}
inline void mat33f_mul_scalar ( mat33f_t* _r, mat33f_t* _lhs, float _rhs ) {
    _r->m00 = _lhs->m00 * _rhs; 
    _r->m01 = _lhs->m01 * _rhs; 
    _r->m02 = _lhs->m02 * _rhs;

    _r->m10 = _lhs->m10 * _rhs; 
    _r->m11 = _lhs->m11 * _rhs; 
    _r->m12 = _lhs->m12 * _rhs;

    _r->m20 = _lhs->m20 * _rhs; 
    _r->m21 = _lhs->m21 * _rhs; 
    _r->m22 = _lhs->m22 * _rhs;
}

// mat33f * vec3f
void mat33f_mul_vec3f ( vec3f_t* _r, mat33f_t* _lhs, vec3f_t* _rhs );

// vec3f * mat33f
void vec3f_mul_mat33f ( vec3f_t* _r, vec3f_t* _lhs, mat33f_t* _rhs );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void scalar_div_mat33f ( mat33f_t* _r, float _lhs, mat33f_t* _rhs ) {
    _r->m00 = _lhs / _rhs->m00; 
    _r->m01 = _lhs / _rhs->m01; 
    _r->m02 = _lhs / _rhs->m02;

    _r->m10 = _lhs / _rhs->m10;
    _r->m11 = _lhs / _rhs->m11;
    _r->m12 = _lhs / _rhs->m12;

    _r->m20 = _lhs / _rhs->m20;
    _r->m21 = _lhs / _rhs->m21;
    _r->m22 = _lhs / _rhs->m22;
}
inline void mat33f_div_scalar ( mat33f_t* _r, mat33f_t* _lhs, float _rhs ) {
    _r->m00 = _lhs->m00 / _rhs;
    _r->m01 = _lhs->m01 / _rhs;
    _r->m02 = _lhs->m02 / _rhs;

    _r->m10 = _lhs->m10 / _rhs;
    _r->m11 = _lhs->m11 / _rhs;
    _r->m12 = _lhs->m12 / _rhs;

    _r->m20 = _lhs->m20 / _rhs;
    _r->m21 = _lhs->m21 / _rhs;
    _r->m22 = _lhs->m22 / _rhs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void mat33f_transpose ( mat33f_t* _m ) {
    float swap;

    swap = _m->m01;
    _m->m01 = _m->m10;
    _m->m10 = swap;

    swap = _m->m02;
    _m->m02 = _m->m20;
    _m->m20 = swap;

    swap = _m->m12;
    _m->m12 = _m->m21;
    _m->m21 = swap;
}
inline void mat33f_get_transpose ( mat33f_t* _r, mat33f_t* _m ) {
    ex_assert ( _r != _m, "can't use self as return value." );
    mat33f_set( _r,
                _m->m00, _m->m10, _m->m20,
                _m->m01, _m->m11, _m->m21,
                _m->m02, _m->m12, _m->m22 );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool mat33f_inverse ( mat33f_t* _m );
bool mat33f_get_inverse ( mat33f_t* _r, mat33f_t* _m );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END MAT33F_H_1289531940
// #################################################################################
