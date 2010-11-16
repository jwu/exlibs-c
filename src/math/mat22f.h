// ======================================================================================
// File         : mat22f.h
// Author       : Wu Jie 
// Last Change  : 11/11/2010 | 12:04:03 PM | Thursday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef MAT22F_H_1289448244
#define MAT22F_H_1289448244
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "vec2f.h"

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

//! @defgroup mat22f_t matrix 2x2 float
//! @ingroup Math
//! @{

// ------------------------------------------------------------------ 
//! @struct mat22f_t
//! @brief Matrix 2x2
//! @details The data structure looks like this:\n
//!
//! | m00, m01 | \n
//! | m10, m11 | \n
//!
//! [ m00, m01, m10, m11 ]
// ------------------------------------------------------------------ 

typedef struct mat22f_t {
    //! @union member
    union {
        struct { 
            float m00, m01;
            float m10, m11;
        }; // end struct
        float m[4];
    };
} mat22f_t;

// ------------------------------------------------------------------ 
//! @fn inline void mat22f_zero ( mat22f_t* _m )
//! @param _m the result matrix
//! @details
//! make a zero matrix: \n
//! | 0.0f, 0.0f |\n
//! | 0.0f, 0.0f |\n
// ------------------------------------------------------------------ 

inline void mat22f_zero ( mat22f_t* _m ) { 
    _m->m00 = 0.0f, _m->m01 = 0.0f; 
    _m->m10 = 0.0f, _m->m11 = 0.0f; 
}

// ------------------------------------------------------------------ 
//! @fn inline void mat22f_identity ( mat22f_t* _m )
//! @param _m the result matrix
//! @details
//! make an identity matrix: \n
//! | 1.0f, 0.0f |\n
//! | 0.0f, 1.0f |\n
// ------------------------------------------------------------------ 

inline void mat22f_identity ( mat22f_t* _m ) { 
    _m->m00 = 1.0f, _m->m01 = 0.0f; 
    _m->m10 = 0.0f, _m->m11 = 1.0f; 
}

// ------------------------------------------------------------------ 
//! @fn inline void mat22f_set ( mat22f_t* _m, 
//!                      float _m00, float _m01,
//!                      float _m10, float _m11 )
//! @param _m the result matrix
//! @param _m00
//! @param _m01
//! @param _m10
//! @param _m11
//! @details
//! set the matrix _m by rest of the parameters: \n
//! | _m00, _m01 |\n
//! | _m10, _m11 |\n
// ------------------------------------------------------------------ 

inline void mat22f_set ( mat22f_t* _m, 
                         float _m00, float _m01,
                         float _m10, float _m11 ) { 
    _m->m00 = _m00, _m->m01 = _m01; 
    _m->m10 = _m10, _m->m11 = _m11; 
}

// ------------------------------------------------------------------ 
//! @fn inline inline float mat22f_get ( mat22f_t* _m, uint _row, uint _col ) 
//! @param _m the matrix
//! @param _row the row index, range in [0,2)
//! @param _col the col index, range in [0,2)
//! @return result
//! @details
//! get the matrix element in [_row,_col]
// ------------------------------------------------------------------ 

inline float mat22f_get ( mat22f_t* _m, uint _row, uint _col ) { 
    ex_assert( _row >= 0 && _row < 2, "out of range" );
    ex_assert( _col >= 0 && _col < 2, "out of range" );
    return _m->m[2*_row+_col];
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// row =============
inline void mat22f_row ( vec2f_t* _r, mat22f_t* _m, uint _row ) {
    ex_assert( _row >= 0 && _row < 2, "out of range" );
    _r->x = _m->m[2*_row+0]; 
    _r->y = _m->m[2*_row+1]; 
}

// col
// ||
// ||
// ||
// ||
inline void mat22f_col ( vec2f_t* _r, mat22f_t* _m, uint _col ) {
    ex_assert( _col >= 0 && _col < 2, "out of range" );
    _r->x = _m->m[2*0+_col]; 
    _r->y = _m->m[2*1+_col]; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void mat22f_neg ( mat22f_t* _m ) {
    _m->m00 = -_m->m00, _m->m01 = -_m->m01; 
    _m->m10 = -_m->m10, _m->m11 = -_m->m11; 
}

inline void mat22f_get_neg ( mat22f_t* _r, mat22f_t* _m ) {
    ex_assert ( _r != _m, "can't use self as return value." );
    _r->m00 = -_m->m00, _r->m01 = -_m->m01; 
    _r->m10 = -_m->m10, _r->m11 = -_m->m11; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void mat22f_abs ( mat22f_t* _m ) {
    _m->m00 = fabsf(_m->m00), _m->m01 = fabsf(_m->m01); 
    _m->m10 = fabsf(_m->m10), _m->m11 = fabsf(_m->m11); 
}

inline void mat22f_get_abs ( mat22f_t* _r, mat22f_t* _m ) {
    ex_assert ( _r != _m, "can't use self as return value." );
    _r->m00 = fabsf(_m->m00), _r->m01 = fabsf(_m->m01); 
    _r->m10 = fabsf(_m->m10), _r->m11 = fabsf(_m->m11); 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void mat22f_add ( mat22f_t* _r, mat22f_t* _lhs, mat22f_t* _rhs ) {
    _r->m00 = _lhs->m00 + _rhs->m00; _r->m01 = _lhs->m01 + _rhs->m01;
    _r->m10 = _lhs->m10 + _rhs->m10; _r->m11 = _lhs->m11 + _rhs->m11;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void mat22f_sub ( mat22f_t* _r, mat22f_t* _lhs, mat22f_t* _rhs ) {
    _r->m00 = _lhs->m00 - _rhs->m00; _r->m01 = _lhs->m01 - _rhs->m01;
    _r->m10 = _lhs->m10 - _rhs->m10; _r->m11 = _lhs->m11 - _rhs->m11;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void mat22f_mul ( mat22f_t* _r, mat22f_t* _lhs, mat22f_t* _rhs ) {
    float m00, m01, m10, m11;

    m00 = _lhs->m00 * _rhs->m00 + _lhs->m01 * _rhs->m10;
    m01 = _lhs->m00 * _rhs->m01 + _lhs->m01 * _rhs->m11;
    m10 = _lhs->m10 * _rhs->m00 + _lhs->m11 * _rhs->m10;
    m11 = _lhs->m10 * _rhs->m01 + _lhs->m11 * _rhs->m11;

    mat22f_set( _r, 
                m00, m01,
                m10, m11 );
}
inline void mat22f_mul_scalar ( mat22f_t* _r, mat22f_t* _lhs, float _rhs ) {
    _r->m00 = _lhs->m00 * _rhs;
    _r->m01 = _lhs->m01 * _rhs;
    _r->m10 = _lhs->m10 * _rhs;
    _r->m11 = _lhs->m11 * _rhs;
}

// mat22f * vec2f
void mat22f_mul_vec2f ( vec2f_t* _r, mat22f_t* _lhs, vec2f_t* _rhs );

// vec2f * mat22f
void vec2f_mul_mat22f ( vec2f_t* _r, vec2f_t* _lhs, mat22f_t* _rhs );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void scalar_div_mat22f ( mat22f_t* _r, float _lhs, mat22f_t* _rhs ) {
    _r->m00 = _lhs / _rhs->m00;
    _r->m01 = _lhs / _rhs->m01;
    _r->m10 = _lhs / _rhs->m10;
    _r->m11 = _lhs / _rhs->m11;
}
inline void mat22f_div_scalar ( mat22f_t* _r, mat22f_t* _lhs, float _rhs ) {
    _r->m00 = _lhs->m00 / _rhs;
    _r->m01 = _lhs->m01 / _rhs;
    _r->m10 = _lhs->m10 / _rhs;
    _r->m11 = _lhs->m11 / _rhs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void mat22f_transpose ( mat22f_t* _m ) {
    float swap;
    swap = _m->m01;
    _m->m01 = _m->m10;
    _m->m10 = swap; 
}
inline void mat22f_get_transpose ( mat22f_t* _r, mat22f_t* _m ) {
    ex_assert ( _r != _m, "can't use self as return value." );
    _r->m00 = _m->m00, _r->m01 = _m->m10;
    _r->m10 = _m->m01, _r->m11 = _m->m11;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool mat22f_inverse ( mat22f_t* _m );
bool mat22f_get_inverse ( mat22f_t* _r, mat22f_t* _m );

//! @}

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END MAT22F_H_1289448244
// #################################################################################


