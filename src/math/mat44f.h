// ======================================================================================
// File         : mat44f.h
// Author       : Wu Jie 
// Last Change  : 11/12/2010 | 13:38:33 PM | Friday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef MAT44F_H_1289540317
#define MAT44F_H_1289540317
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "vec4f.h"

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

// ------------------------------------------------------------------ 
//! @struct mat44f_t
//! @brief Matrix 4x4
// ------------------------------------------------------------------ 

typedef struct mat44f_t {
    union {
        struct { 
            float m00, m01, m02, m03;
            float m10, m11, m12, m13;
            float m20, m21, m22, m23;
            float m30, m31, m32, m33;
        }; // end struct
        float m[16];
    };
} mat44f_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void mat44f_zero ( mat44f_t* _m ) { 
    _m->m00 = 0.0f, _m->m01 = 0.0f, _m->m02 = 0.0f; _m->m03 = 0.0f;
    _m->m10 = 0.0f, _m->m11 = 0.0f, _m->m12 = 0.0f; _m->m13 = 0.0f;
    _m->m20 = 0.0f, _m->m21 = 0.0f, _m->m22 = 0.0f; _m->m23 = 0.0f;
    _m->m30 = 0.0f, _m->m31 = 0.0f, _m->m32 = 0.0f; _m->m33 = 0.0f;
}
inline void mat44f_identity ( mat44f_t* _m ) { 
    _m->m00 = 1.0f, _m->m01 = 0.0f, _m->m02 = 0.0f; _m->m03 = 0.0f;
    _m->m10 = 0.0f, _m->m11 = 1.0f, _m->m12 = 0.0f; _m->m13 = 0.0f;
    _m->m20 = 0.0f, _m->m21 = 0.0f, _m->m22 = 1.0f; _m->m23 = 0.0f;
    _m->m30 = 0.0f, _m->m31 = 0.0f, _m->m32 = 0.0f; _m->m33 = 1.0f;
}
inline void mat44f_set ( mat44f_t* _m, 
                         float _m00, float _m01, float _m02, float _m03,
                         float _m10, float _m11, float _m12, float _m13,
                         float _m20, float _m21, float _m22, float _m23,
                         float _m30, float _m31, float _m32, float _m33 ) { 
    _m->m00 = _m00, _m->m01 = _m01, _m->m02 = _m02, _m->m03 = _m03;
    _m->m10 = _m10, _m->m11 = _m11, _m->m12 = _m12, _m->m13 = _m13; 
    _m->m20 = _m20, _m->m21 = _m21, _m->m22 = _m22, _m->m23 = _m23; 
    _m->m30 = _m30, _m->m31 = _m31, _m->m32 = _m32, _m->m33 = _m33; 
}
inline float mat44f_get ( mat44f_t* _m, uint _row, uint _col ) { 
    ex_assert( _row >= 0 && _row < 4, "out of range" );
    ex_assert( _col >= 0 && _col < 4, "out of range" );
    return _m->m[4*_row+_col];
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// row =============
inline void mat44f_row ( vec4f_t* _r, mat44f_t* _m, uint _row ) {
    ex_assert( _row >= 0 && _row < 4, "out of range" );
    _r->x = _m->m[4*_row+0]; 
    _r->y = _m->m[4*_row+1]; 
    _r->z = _m->m[4*_row+2]; 
    _r->w = _m->m[4*_row+3]; 
}

// col
// ||
// ||
// ||
// ||
inline void mat44f_col ( vec4f_t* _r, mat44f_t* _m, uint _col ) {
    ex_assert( _col >= 0 && _col < 4, "out of range" );
    _r->x = _m->m[4*0+_col]; 
    _r->y = _m->m[4*1+_col]; 
    _r->z = _m->m[4*2+_col]; 
    _r->w = _m->m[4*3+_col]; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void mat44f_neg ( mat44f_t* _m ) {
    _m->m00 = -_m->m00, _m->m01 = -_m->m01, _m->m02 = -_m->m02, _m->m03 = -_m->m03; 
    _m->m10 = -_m->m10, _m->m11 = -_m->m11, _m->m12 = -_m->m12, _m->m13 = -_m->m13; 
    _m->m20 = -_m->m20, _m->m21 = -_m->m21, _m->m22 = -_m->m22, _m->m23 = -_m->m23; 
    _m->m30 = -_m->m30, _m->m31 = -_m->m31, _m->m32 = -_m->m32, _m->m33 = -_m->m33; 
}

inline void mat44f_get_neg ( mat44f_t* _r, mat44f_t* _m ) {
    ex_assert ( _r != _m, "can't use self as return value." );
    _r->m00 = -_m->m00, _r->m01 = -_m->m01, _r->m02 = -_m->m02, _r->m03 = -_m->m03; 
    _r->m10 = -_m->m10, _r->m11 = -_m->m11, _r->m12 = -_m->m12, _r->m13 = -_m->m13; 
    _r->m20 = -_m->m20, _r->m21 = -_m->m21, _r->m22 = -_m->m22, _r->m23 = -_m->m23; 
    _r->m30 = -_m->m30, _r->m31 = -_m->m31, _r->m32 = -_m->m32, _r->m33 = -_m->m33; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void mat44f_abs ( mat44f_t* _m ) {
    _m->m00 = fabsf(_m->m00), _m->m01 = fabsf(_m->m01), _m->m02 = fabsf(_m->m02), _m->m03 = fabsf(_m->m03); 
    _m->m10 = fabsf(_m->m10), _m->m11 = fabsf(_m->m11), _m->m12 = fabsf(_m->m12), _m->m13 = fabsf(_m->m13); 
    _m->m20 = fabsf(_m->m20), _m->m21 = fabsf(_m->m21), _m->m22 = fabsf(_m->m22), _m->m23 = fabsf(_m->m23); 
    _m->m30 = fabsf(_m->m30), _m->m31 = fabsf(_m->m31), _m->m32 = fabsf(_m->m32), _m->m33 = fabsf(_m->m33); 
}

inline void mat44f_get_abs ( mat44f_t* _r, mat44f_t* _m ) {
    ex_assert ( _r != _m, "can't use self as return value." );
    _r->m00 = fabsf(_m->m00), _r->m01 = fabsf(_m->m01), _r->m02 = fabsf(_m->m02), _r->m03 = fabsf(_m->m03); 
    _r->m10 = fabsf(_m->m10), _r->m11 = fabsf(_m->m11), _r->m12 = fabsf(_m->m12), _r->m13 = fabsf(_m->m13); 
    _r->m20 = fabsf(_m->m20), _r->m21 = fabsf(_m->m21), _r->m22 = fabsf(_m->m22), _r->m23 = fabsf(_m->m23); 
    _r->m30 = fabsf(_m->m30), _r->m31 = fabsf(_m->m31), _r->m32 = fabsf(_m->m32), _r->m33 = fabsf(_m->m33); 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void mat44f_add ( mat44f_t* _r, mat44f_t* _lhs, mat44f_t* _rhs ) {
    _r->m00 = _lhs->m00 + _rhs->m00; 
    _r->m01 = _lhs->m01 + _rhs->m01; 
    _r->m02 = _lhs->m02 + _rhs->m02;
    _r->m03 = _lhs->m03 + _rhs->m03;

    _r->m10 = _lhs->m10 + _rhs->m10; 
    _r->m11 = _lhs->m11 + _rhs->m11;
    _r->m12 = _lhs->m12 + _rhs->m12;
    _r->m13 = _lhs->m13 + _rhs->m13;

    _r->m20 = _lhs->m20 + _rhs->m20; 
    _r->m21 = _lhs->m21 + _rhs->m21;
    _r->m22 = _lhs->m22 + _rhs->m22;
    _r->m23 = _lhs->m23 + _rhs->m23;

    _r->m30 = _lhs->m30 + _rhs->m30; 
    _r->m31 = _lhs->m31 + _rhs->m31;
    _r->m32 = _lhs->m32 + _rhs->m32;
    _r->m33 = _lhs->m33 + _rhs->m33;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void mat44f_sub ( mat44f_t* _r, mat44f_t* _lhs, mat44f_t* _rhs ) {
    _r->m00 = _lhs->m00 - _rhs->m00; 
    _r->m01 = _lhs->m01 - _rhs->m01;
    _r->m02 = _lhs->m02 - _rhs->m02;
    _r->m03 = _lhs->m03 - _rhs->m03;

    _r->m10 = _lhs->m10 - _rhs->m10;
    _r->m11 = _lhs->m11 - _rhs->m11;
    _r->m12 = _lhs->m12 - _rhs->m12;
    _r->m13 = _lhs->m13 - _rhs->m13;

    _r->m20 = _lhs->m20 - _rhs->m20;
    _r->m21 = _lhs->m21 - _rhs->m21;
    _r->m22 = _lhs->m22 - _rhs->m22;
    _r->m23 = _lhs->m23 - _rhs->m23;

    _r->m30 = _lhs->m30 - _rhs->m30;
    _r->m31 = _lhs->m31 - _rhs->m31;
    _r->m32 = _lhs->m32 - _rhs->m32;
    _r->m33 = _lhs->m33 - _rhs->m33;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void mat44f_mul ( mat44f_t* _r, mat44f_t* _lhs, mat44f_t* _rhs ) {
    float m00, m01, m02, m03; 
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    float m30, m31, m32, m33;

    m00 = _lhs->m00 * _rhs->m00 + _lhs->m01 * _rhs->m10 + _lhs->m02 * _rhs->m20 + _lhs->m03 * _rhs->m30;
    m01 = _lhs->m00 * _rhs->m01 + _lhs->m01 * _rhs->m11 + _lhs->m02 * _rhs->m21 + _lhs->m03 * _rhs->m31;
    m02 = _lhs->m00 * _rhs->m02 + _lhs->m01 * _rhs->m12 + _lhs->m02 * _rhs->m22 + _lhs->m03 * _rhs->m32;
    m03 = _lhs->m00 * _rhs->m03 + _lhs->m01 * _rhs->m13 + _lhs->m02 * _rhs->m23 + _lhs->m03 * _rhs->m33;

    m10 = _lhs->m10 * _rhs->m00 + _lhs->m11 * _rhs->m10 + _lhs->m12 * _rhs->m20 + _lhs->m13 * _rhs->m30;
    m11 = _lhs->m10 * _rhs->m01 + _lhs->m11 * _rhs->m11 + _lhs->m12 * _rhs->m21 + _lhs->m13 * _rhs->m31;
    m12 = _lhs->m10 * _rhs->m02 + _lhs->m11 * _rhs->m12 + _lhs->m12 * _rhs->m22 + _lhs->m13 * _rhs->m32;
    m13 = _lhs->m10 * _rhs->m03 + _lhs->m11 * _rhs->m13 + _lhs->m12 * _rhs->m23 + _lhs->m13 * _rhs->m33;

    m20 = _lhs->m20 * _rhs->m00 + _lhs->m21 * _rhs->m10 + _lhs->m22 * _rhs->m20 + _lhs->m23 * _rhs->m30;
    m21 = _lhs->m20 * _rhs->m01 + _lhs->m21 * _rhs->m11 + _lhs->m22 * _rhs->m21 + _lhs->m23 * _rhs->m31;
    m22 = _lhs->m20 * _rhs->m02 + _lhs->m21 * _rhs->m12 + _lhs->m22 * _rhs->m22 + _lhs->m23 * _rhs->m32;
    m23 = _lhs->m20 * _rhs->m03 + _lhs->m21 * _rhs->m13 + _lhs->m22 * _rhs->m23 + _lhs->m23 * _rhs->m33;

    m30 = _lhs->m30 * _rhs->m00 + _lhs->m31 * _rhs->m10 + _lhs->m32 * _rhs->m20 + _lhs->m33 * _rhs->m30;
    m31 = _lhs->m30 * _rhs->m01 + _lhs->m31 * _rhs->m11 + _lhs->m32 * _rhs->m21 + _lhs->m33 * _rhs->m31;
    m32 = _lhs->m30 * _rhs->m02 + _lhs->m31 * _rhs->m12 + _lhs->m32 * _rhs->m22 + _lhs->m33 * _rhs->m32;
    m33 = _lhs->m30 * _rhs->m03 + _lhs->m31 * _rhs->m13 + _lhs->m32 * _rhs->m23 + _lhs->m33 * _rhs->m33;

    mat44f_set( _r, 
                m00, m01, m02, m03,
                m10, m11, m12, m13,
                m20, m21, m22, m23,
                m30, m31, m32, m33 );
}
inline void mat44f_mul_scalar ( mat44f_t* _r, mat44f_t* _lhs, float _rhs ) {
    _r->m00 = _lhs->m00 * _rhs; 
    _r->m01 = _lhs->m01 * _rhs; 
    _r->m02 = _lhs->m02 * _rhs;
    _r->m03 = _lhs->m03 * _rhs;

    _r->m10 = _lhs->m10 * _rhs; 
    _r->m11 = _lhs->m11 * _rhs; 
    _r->m12 = _lhs->m12 * _rhs;
    _r->m13 = _lhs->m13 * _rhs;

    _r->m20 = _lhs->m20 * _rhs; 
    _r->m21 = _lhs->m21 * _rhs; 
    _r->m22 = _lhs->m22 * _rhs;
    _r->m23 = _lhs->m23 * _rhs;

    _r->m30 = _lhs->m30 * _rhs; 
    _r->m31 = _lhs->m31 * _rhs; 
    _r->m32 = _lhs->m32 * _rhs;
    _r->m33 = _lhs->m33 * _rhs;
}

// mat44f * vec4f
void mat44f_mul_vec4f ( vec4f_t* _r, mat44f_t* _lhs, vec4f_t* _rhs );

// vec4f * mat44f
void vec4f_mul_mat44f ( vec4f_t* _r, vec4f_t* _lhs, mat44f_t* _rhs );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void scalar_div_mat44f ( mat44f_t* _r, float _lhs, mat44f_t* _rhs ) {
    _r->m00 = _lhs / _rhs->m00; 
    _r->m01 = _lhs / _rhs->m01; 
    _r->m02 = _lhs / _rhs->m02;
    _r->m03 = _lhs / _rhs->m03;

    _r->m10 = _lhs / _rhs->m10;
    _r->m11 = _lhs / _rhs->m11;
    _r->m12 = _lhs / _rhs->m12;
    _r->m13 = _lhs / _rhs->m13;

    _r->m20 = _lhs / _rhs->m20;
    _r->m21 = _lhs / _rhs->m21;
    _r->m22 = _lhs / _rhs->m22;
    _r->m23 = _lhs / _rhs->m23;

    _r->m30 = _lhs / _rhs->m30;
    _r->m31 = _lhs / _rhs->m31;
    _r->m32 = _lhs / _rhs->m32;
    _r->m33 = _lhs / _rhs->m33;
}
inline void mat44f_div_scalar ( mat44f_t* _r, mat44f_t* _lhs, float _rhs ) {
    _r->m00 = _lhs->m00 / _rhs;
    _r->m01 = _lhs->m01 / _rhs;
    _r->m02 = _lhs->m02 / _rhs;
    _r->m03 = _lhs->m03 / _rhs;

    _r->m10 = _lhs->m10 / _rhs;
    _r->m11 = _lhs->m11 / _rhs;
    _r->m12 = _lhs->m12 / _rhs;
    _r->m13 = _lhs->m13 / _rhs;

    _r->m20 = _lhs->m20 / _rhs;
    _r->m21 = _lhs->m21 / _rhs;
    _r->m22 = _lhs->m22 / _rhs;
    _r->m23 = _lhs->m23 / _rhs;

    _r->m30 = _lhs->m30 / _rhs;
    _r->m31 = _lhs->m31 / _rhs;
    _r->m32 = _lhs->m32 / _rhs;
    _r->m33 = _lhs->m33 / _rhs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void mat44f_transpose ( mat44f_t* _m ) {
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

    swap = _m->m03;
    _m->m03 = _m->m30;
    _m->m30 = swap;

    swap = _m->m13;
    _m->m13 = _m->m31;
    _m->m31 = swap;

    swap = _m->m23;
    _m->m23 = _m->m32;
    _m->m32 = swap;
}
inline void mat44f_get_transpose ( mat44f_t* _r, mat44f_t* _m ) {
    ex_assert ( _r != _m, "can't use self as return value." );
    mat44f_set( _r,
                _m->m00, _m->m10, _m->m20, _m->m30,
                _m->m01, _m->m11, _m->m21, _m->m31,
                _m->m02, _m->m12, _m->m22, _m->m32,
                _m->m03, _m->m13, _m->m23, _m->m33 );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool mat44f_inverse ( mat44f_t* _m );
bool mat44f_get_inverse ( mat44f_t* _r, mat44f_t* _m );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END MAT44F_H_1289540317
// #################################################################################


