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

//! @defgroup ex_mat33f_t matrix 3x3 float
//! @ingroup Math
//! @{

// ------------------------------------------------------------------ 
/*! 
 @struct ex_mat33f_t
 @details
 
 The data of matrix 3x3 is construct by an union structure with float elements.\n
 The matrix can be expressed in formular as:
 
 \f$
   \left[\begin{array}{ c c c }
   m_{00} & m_{01} & m_{02} \\
   m_{10} & m_{11} & m_{12} \\
   m_{20} & m_{21} & m_{22}
   \end{array} \right]
 \f$
 
 and in array as:

 \f$
   \left[\begin{array}{ c c c c c c c c c }
   m_{00}, m_{01}, m_{02}, m_{10}, m_{11}, m_{12}, m_{20}, m_{21}, m_{22}
   \end{array} \right]
 \f$
*/// ------------------------------------------------------------------ 

typedef struct ex_mat33f_t {
    union {
        struct { 
            float m00, m01, m02;
            float m10, m11, m12;
            float m20, m21, m22;
        }; // end struct
        float m[9];
    };
} ex_mat33f_t;

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_mat33f_zero ( ex_mat33f_t* _m )
 @retval _m the result matrix
 @details make a zero matrix:

 \f[
   m = \left[\begin{array}{ c c c }
   0.0 & 0.0 & 0.0 \\
   0.0 & 0.0 & 0.0 \\
   0.0 & 0.0 & 0.0
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void ex_mat33f_zero ( ex_mat33f_t* _m ) { 
    _m->m00 = 0.0f, _m->m01 = 0.0f, _m->m02 = 0.0f; 
    _m->m10 = 0.0f, _m->m11 = 0.0f, _m->m12 = 0.0f; 
    _m->m20 = 0.0f, _m->m21 = 0.0f, _m->m22 = 0.0f; 
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_mat33f_identity ( ex_mat33f_t* _m )
 @retval _m the result matrix
 @details make an identity matrix:

 \f[
   m = \left[\begin{array}{ c c c }
   1.0 & 0.0 & 0.0 \\
   0.0 & 1.0 & 0.0 \\
   0.0 & 0.0 & 1.0
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void ex_mat33f_identity ( ex_mat33f_t* _m ) { 
    _m->m00 = 1.0f, _m->m01 = 0.0f, _m->m02 = 0.0f; 
    _m->m10 = 0.0f, _m->m11 = 1.0f, _m->m12 = 0.0f; 
    _m->m20 = 0.0f, _m->m21 = 0.0f, _m->m22 = 1.0f; 
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_mat33f_set ( ex_mat33f_t* _m, 
                         float _m00, float _m01, float _m02,
                         float _m10, float _m11, float _m12,
                         float _m20, float _m21, float _m22 )
 @retval _m the result matrix
 @param _m00
 @param _m01
 @param _m02
 @param _m10
 @param _m11
 @param _m12
 @param _m20
 @param _m21
 @param _m22
 @details set the matrix _m by rest of the parameters:

 \f[
   m = \left[\begin{array}{ c c c }
   \_m_{00} & \_m_{01} & \_m_{02} \\
   \_m_{10} & \_m_{11} & \_m_{12} \\
   \_m_{20} & \_m_{21} & \_m_{22}
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void ex_mat33f_set ( ex_mat33f_t* _m, 
                         float _m00, float _m01, float _m02,
                         float _m10, float _m11, float _m12,
                         float _m20, float _m21, float _m22 ) { 
    _m->m00 = _m00, _m->m01 = _m01, _m->m02 = _m02;
    _m->m10 = _m10, _m->m11 = _m11, _m->m12 = _m12; 
    _m->m20 = _m20, _m->m21 = _m21, _m->m22 = _m22; 
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline inline float ex_mat33f_get ( ex_mat33f_t* _m, uint _row, uint _col ) 
 @param _m the matrix
 @param _row the row index, range in [0,3)
 @param _col the col index, range in [0,3)
 @return result
 @details get the matrix element in (_row, _col)
*/// ------------------------------------------------------------------ 

inline float ex_mat33f_get ( ex_mat33f_t* _m, uint _row, uint _col ) { 
    ex_assert( _row >= 0 && _row < 3, "out of range" );
    ex_assert( _col >= 0 && _col < 3, "out of range" );
    return _m->m[3*_row+_col];
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_mat33f_row ( ex_vec3f_t* _r, ex_mat33f_t* _m, uint _row )
 @retval _r the result vector
 @param _m the matrix
 @param _row the row index, range in [0,3)
 @details get the matrix by _row and return it in vector3, for example: 
 @code
 ex_vec3f_t r;
 ex_mat33f_t m;
 ex_mat33f_zero(&m);
 ex_mat33f_row( &r, &m, 0 ); // set r to row 0 as -- (m.m00, m.m01, m.m02)
 @endcode
 the r is equal to vector3 (m.m00, m.m01, m.m02) 
*/// ------------------------------------------------------------------ 

inline void ex_mat33f_row ( ex_vec3f_t* _r, ex_mat33f_t* _m, uint _row ) {
    ex_assert( _row >= 0 && _row < 3, "out of range" );
    _r->x = _m->m[3*_row+0]; 
    _r->y = _m->m[3*_row+1]; 
    _r->z = _m->m[3*_row+2]; 
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_mat33f_col ( ex_vec3f_t* _r, ex_mat33f_t* _m, uint _col )
 @retval _r the result vector
 @param _m the matrix
 @param _col the column index, range in [0,3)
 @details get the matrix by _row and return it in vector3, for example: 
 @code
 ex_vec3f_t r;
 ex_mat33f_t m;
 ex_mat33f_zero(&m);
 ex_mat33f_col( &r, &m, 0 ); // set r to column 0 as -- (m.m00, m.m10, m.m20)
 @endcode
 the r is equal to vector3 (m.m00, m.m10, m.m20) 
*/// ------------------------------------------------------------------ 

inline void ex_mat33f_col ( ex_vec3f_t* _r, ex_mat33f_t* _m, uint _col ) {
    ex_assert( _col >= 0 && _col < 3, "out of range" );
    _r->x = _m->m[3*0+_col]; 
    _r->y = _m->m[3*1+_col]; 
    _r->z = _m->m[3*2+_col]; 
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_mat33f_neg ( ex_mat33f_t* _m )
 @param _m in matrix
 @retval _m the result matrix
 @details get the negtive value from matrix _m, override and return it as the result:

 \f[
   \_m =
   -\left[\begin{array}{ c c c }
   m_{00} & m_{01} & m_{02} \\
   m_{10} & m_{11} & m_{12} \\
   m_{20} & m_{21} & m_{22}
   \end{array} \right]
   = 
   \left[\begin{array}{ c c c }
   -m_{00} & -m_{01} & -m_{02} \\
   -m_{10} & -m_{11} & -m_{12} \\
   -m_{20} & -m_{21} & -m_{22}
   \end{array} \right]
 \f]
 @sa ex_mat33f_get_neg
*/// ------------------------------------------------------------------ 

inline void ex_mat33f_neg ( ex_mat33f_t* _m ) {
    _m->m00 = -_m->m00, _m->m01 = -_m->m01, _m->m02 = -_m->m02; 
    _m->m10 = -_m->m10, _m->m11 = -_m->m11, _m->m12 = -_m->m12; 
    _m->m20 = -_m->m20, _m->m21 = -_m->m21, _m->m22 = -_m->m22; 
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_mat33f_get_neg ( ex_mat33f_t* _r, ex_mat33f_t* _m )
 @retval _r the result matrix
 @param _m in matrix
 @details get the negtive value from matrix _m and set it in matrix _r as the result:
 @sa ex_mat33f_neg
*/// ------------------------------------------------------------------ 

inline void ex_mat33f_get_neg ( ex_mat33f_t* _r, ex_mat33f_t* _m ) {
    ex_assert ( _r != _m, "can't use self as return value." );
    _r->m00 = -_m->m00, _r->m01 = -_m->m01, _r->m02 = -_m->m02; 
    _r->m10 = -_m->m10, _r->m11 = -_m->m11, _r->m12 = -_m->m12; 
    _r->m20 = -_m->m20, _r->m21 = -_m->m21, _r->m22 = -_m->m22; 
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_mat33f_abs ( ex_mat33f_t* _m )
 @param _m in matrix
 @retval _m the result matrix
 @details get the absolute value from matrix _m, override and return it as the result:

 \f[
   \_m = |Mat_m| = 
   \left|\begin{array}{ c c c }
   m_{00} & m_{01} & m_{02} \\
   m_{10} & m_{11} & m_{12} \\
   m_{20} & m_{21} & m_{22}
   \end{array} \right|
   =
   \left[\begin{array}{ c c c }
   |m_{00}| & |m_{01}| & |m_{02}| \\
   |m_{10}| & |m_{11}| & |m_{12}| \\
   |m_{20}| & |m_{21}| & |m_{22}|
   \end{array} \right]
 \f]
 @sa ex_mat33f_get_abs
*/// ------------------------------------------------------------------ 

inline void ex_mat33f_abs ( ex_mat33f_t* _m ) {
    _m->m00 = fabsf(_m->m00), _m->m01 = fabsf(_m->m01), _m->m02 = fabsf(_m->m02); 
    _m->m10 = fabsf(_m->m10), _m->m11 = fabsf(_m->m11), _m->m12 = fabsf(_m->m12); 
    _m->m20 = fabsf(_m->m20), _m->m21 = fabsf(_m->m21), _m->m22 = fabsf(_m->m22); 
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_mat33f_get_abs ( ex_mat33f_t* _r, ex_mat33f_t* _m )
 @retval _r the result matrix
 @param _m in matrix
 @details get the absolute value from matrix _m and set it in matrix _r as the result:
 @sa ex_mat33f_abs
*/// ------------------------------------------------------------------ 

inline void ex_mat33f_get_abs ( ex_mat33f_t* _r, ex_mat33f_t* _m ) {
    ex_assert ( _r != _m, "can't use self as return value." );
    _r->m00 = fabsf(_m->m00), _r->m01 = fabsf(_m->m01), _r->m02 = fabsf(_m->m02); 
    _r->m10 = fabsf(_m->m10), _r->m11 = fabsf(_m->m11), _r->m12 = fabsf(_m->m12); 
    _r->m20 = fabsf(_m->m20), _r->m21 = fabsf(_m->m21), _r->m22 = fabsf(_m->m22); 
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_mat33f_add ( ex_mat33f_t* _r, ex_mat33f_t* _lhs, ex_mat33f_t* _rhs )
 @retval _r the result matrix
 @param _lhs left hand side matrix 
 @param _rhs right hand side matrix 
 @details
 \f[
   \_r = Mat_a + Mat_b = 
   \left[\begin{array}{ c c c }
   a_{00} & a_{01} & a_{02} \\
   a_{10} & a_{11} & a_{12} \\
   a_{20} & a_{21} & a_{22}
   \end{array} \right] +
   \left[\begin{array}{ c c c }
   b_{00} & b_{01} & b_{02} \\
   b_{10} & b_{11} & b_{12} \\
   b_{20} & b_{21} & b_{22}
   \end{array} \right]
   =
   \left[\begin{array}{ c c c }
   a_{00} + b_{00} & a_{01} + b_{01} & a_{02} + b_{02} \\
   a_{10} + b_{10} & a_{11} + b_{11} & a_{12} + b_{12} \\
   a_{20} + b_{20} & a_{21} + b_{21} & a_{22} + b_{22}
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void ex_mat33f_add ( ex_mat33f_t* _r, ex_mat33f_t* _lhs, ex_mat33f_t* _rhs ) {
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
/*! 
 @fn inline void ex_mat33f_sub ( ex_mat33f_t* _r, ex_mat33f_t* _lhs, ex_mat33f_t* _rhs )
 @retval _r the result matrix
 @param _lhs left hand side matrix 
 @param _rhs right hand side matrix 
 @details
 \f[
   \_r = Mat_a - Mat_b = 
   \left[\begin{array}{ c c c }
   a_{00} & a_{01} & a_{02} \\
   a_{10} & a_{11} & a_{12} \\
   a_{20} & a_{21} & a_{22}
   \end{array} \right] -
   \left[\begin{array}{ c c c }
   b_{00} & b_{01} & b_{02} \\
   b_{10} & b_{11} & b_{12} \\
   b_{20} & b_{21} & b_{22}
   \end{array} \right]
   =
   \left[\begin{array}{ c c c }
   a_{00} - b_{00} & a_{01} - b_{01} & a_{02} - b_{02} \\
   a_{10} - b_{10} & a_{11} - b_{11} & a_{12} - b_{12} \\
   a_{20} - b_{20} & a_{21} - b_{21} & a_{22} - b_{22}
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void ex_mat33f_sub ( ex_mat33f_t* _r, ex_mat33f_t* _lhs, ex_mat33f_t* _rhs ) {
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
/*! 
 @fn inline void ex_mat33f_mul ( ex_mat33f_t* _r, ex_mat33f_t* _lhs, ex_mat33f_t* _rhs )
 @retval _r the result matrix
 @param _lhs left hand side matrix 
 @param _rhs right hand side matrix 
 @details
 \f[
   \_r = Mat_a * Mat_b = 
   \left[\begin{array}{ c c c }
   a_{00} & a_{01} & a_{02} \\
   a_{10} & a_{11} & a_{12} \\
   a_{20} & a_{21} & a_{22}
   \end{array} \right] *
   \left[\begin{array}{ c c c }
   b_{00} & b_{01} & b_{02} \\
   b_{10} & b_{11} & b_{12} \\
   b_{20} & b_{21} & b_{22}
   \end{array} \right]
   =
   \left[\begin{array}{ c c c }
    a_{00} * b_{00} + a_{01} * b_{10} + a_{02} * b_{20} &
    a_{00} * b_{01} + a_{01} * b_{11} + a_{02} * b_{21} &
    a_{00} * b_{02} + a_{01} * b_{12} + a_{02} * b_{22} \\
    a_{10} * b_{00} + a_{11} * b_{10} + a_{12} * b_{20} &
    a_{10} * b_{01} + a_{11} * b_{11} + a_{12} * b_{21} &
    a_{10} * b_{02} + a_{11} * b_{12} + a_{12} * b_{22} \\
    a_{20} * b_{00} + a_{21} * b_{10} + a_{22} * b_{20} &
    a_{20} * b_{01} + a_{21} * b_{11} + a_{22} * b_{21} &
    a_{20} * b_{02} + a_{21} * b_{12} + a_{22} * b_{22} \\
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void ex_mat33f_mul ( ex_mat33f_t* _r, ex_mat33f_t* _lhs, ex_mat33f_t* _rhs ) {
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

    ex_mat33f_set( _r, 
                m00, m01, m02,
                m10, m11, m12,
                m20, m21, m22 );
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_mat33f_mul_scalar ( ex_mat33f_t* _r, ex_mat33f_t* _lhs, float _rhs )
 @retval _r the result matrix
 @param _lhs left hand side matrix 
 @param _rhs right hand side scalar 
 @details
 \f[
   \_r = Mat_a * s = 
   \left[\begin{array}{ c c c }
   a_{00} & a_{01} & a_{02} \\
   a_{10} & a_{11} & a_{12} \\
   a_{20} & a_{21} & a_{22}
   \end{array} \right] *
   s
   =
   \left[\begin{array}{ c c c }
   a_{00} * s & a_{01} * s & a_{02} * s \\
   a_{10} * s & a_{11} * s & a_{12} * s \\
   a_{20} * s & a_{21} * s & a_{22} * s
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void ex_mat33f_mul_scalar ( ex_mat33f_t* _r, ex_mat33f_t* _lhs, float _rhs ) {
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

// ------------------------------------------------------------------ 
/*! 
 @fn void ex_mat33f_mul_vec3f ( ex_vec3f_t* _r, ex_mat33f_t* _lhs, ex_vec3f_t* _rhs )
 @retval _r the result vector
 @param _lhs left hand side matrix 
 @param _rhs right hand side vector 
 @details
 \f[
   \_r = Mat_a * Vec_b = \left[\begin{array}{ c c c }
   a_{00} & a_{01} & a_{02} \\
   a_{10} & a_{11} & a_{12} \\
   a_{20} & a_{21} & a_{22}
   \end{array} \right] * 
   \left[\begin{array}{ c }
   b_{0} \\
   b_{1} \\
   b_{2} \\
   \end{array} \right] = \left[\begin{array}{ c c c }
   a_{00} * b_{0} + a_{01} * b_{1} + a_{02} * b_{2} & 
   a_{10} * b_{0} + a_{11} * b_{1} + a_{12} * b_{2} &
   a_{20} * b_{0} + a_{21} * b_{1} + a_{22} * b_{2}
   \end{array} \right]
 \f]
 @sa ex_vec3f_mul_mat33f
*/// ------------------------------------------------------------------ 

void ex_mat33f_mul_vec3f ( ex_vec3f_t* _r, ex_mat33f_t* _lhs, ex_vec3f_t* _rhs );

// ------------------------------------------------------------------ 
/*! 
 @fn void ex_vec3f_mul_mat33f ( ex_vec3f_t* _r, ex_vec3f_t* _lhs, ex_mat33f_t* _rhs )
 @retval _r the result vector
 @param _lhs left hand side vector
 @param _rhs right hand side matrix  
 @details
 \f[
   \_r = Vec_a * Mat_b = \left[\begin{array}{ c c c }
   a_{0} & a_{1} & a_{2}
   \end{array} \right] * 
   \left[\begin{array}{ c c c }
   b_{00} & b_{01} & b_{02} \\
   b_{10} & b_{11} & b_{12} \\
   b_{20} & b_{21} & b_{22}
   \end{array} \right] = \left[\begin{array}{ c c c }
   a_{0} * b_{00} + a_{1} * b_{10} + a_{2} * b_{20} & 
   a_{0} * b_{01} + a_{1} * b_{11} + a_{2} * b_{21} &
   a_{0} * b_{02} + a_{1} * b_{12} + a_{2} * b_{22}
   \end{array} \right]
 \f]
 @sa ex_mat33f_mul_vec3f
*/// ------------------------------------------------------------------ 

void ex_vec3f_mul_mat33f ( ex_vec3f_t* _r, ex_vec3f_t* _lhs, ex_mat33f_t* _rhs );

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_scalar_div_mat33f ( ex_mat33f_t* _r, float _lhs, ex_mat33f_t* _rhs )
 @retval _r the result vector
 @param _lhs left hand side float
 @param _rhs right hand side matrix  
 @details
 \f[
   \_r = s / Mat_b = 
   s /
   \left[\begin{array}{ c c c }
   b_{00} & b_{01} & b_{02} \\
   b_{10} & b_{11} & b_{12} \\
   b_{20} & b_{21} & b_{22} 
   \end{array} \right]
   =
   \left[\begin{array}{ c c c }
   s / b_{00} & s / b_{01} & s / b_{02} \\
   s / b_{10} & s / b_{11} & s / b_{12} \\
   s / b_{20} & s / b_{21} & s / b_{22}
   \end{array} \right]
 \f]
 @sa ex_mat33f_div_scalar
*/// ------------------------------------------------------------------ 

inline void ex_scalar_div_mat33f ( ex_mat33f_t* _r, float _lhs, ex_mat33f_t* _rhs ) {
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

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_mat33f_div_scalar ( ex_mat33f_t* _r, ex_mat33f_t* _lhs, float _rhs )
 @retval _r the result vector
 @param _lhs left hand side matrix  
 @param _rhs right hand side float
 @details
 \f[
   \_r = Mat_a / s = 
   \left[\begin{array}{ c c c }
   a_{00} & a_{01} & a_{02} \\
   a_{10} & a_{11} & a_{12} \\
   a_{20} & a_{21} & a_{22}
   \end{array} \right] /
   s
   =
   \left[\begin{array}{ c c c }
   a_{00} / s & a_{01} / s & a_{02} / s \\
   a_{10} / s & a_{11} / s & a_{12} / s \\
   a_{20} / s & a_{21} / s & a_{22} / s
   \end{array} \right]
 \f]
 @sa ex_scalar_div_mat33f
*/// ------------------------------------------------------------------ 

inline void ex_mat33f_div_scalar ( ex_mat33f_t* _r, ex_mat33f_t* _lhs, float _rhs ) {
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
/*! 
 @fn inline void ex_mat33f_transpose ( ex_mat33f_t* _m )
 @retval _m the result vector
 @param _m the in matrix
 @details get the transposed matrix from matrix _m, override and return it.

 \f[
   \left[\begin{array}{ c c c }
   a_{00} & a_{01} & a_{02} \\
   a_{10} & a_{11} & a_{12} \\
   a_{20} & a_{21} & a_{22}
   \end{array} \right] 
   \stackrel{transpose}\Longrightarrow
   \left[\begin{array}{ c c c }
   a_{00} & a_{10} & a_{20} \\
   a_{01} & a_{11} & a_{21} \\
   a_{02} & a_{12} & a_{22}
   \end{array} \right]
 \f]

 @sa ex_mat33f_get_transpose
*/// ------------------------------------------------------------------ 

inline void ex_mat33f_transpose ( ex_mat33f_t* _m ) {
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

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_mat33f_get_transpose ( ex_mat33f_t* _r, ex_mat33f_t* _m )
 @retval _r the result vector
 @param _m the in matrix
 @details get the transposed matrix from matrix _m, return it to matrix _r
 @sa ex_mat33f_transpose
*/// ------------------------------------------------------------------ 

inline void ex_mat33f_get_transpose ( ex_mat33f_t* _r, ex_mat33f_t* _m ) {
    ex_assert ( _r != _m, "can't use self as return value." );
    ex_mat33f_set( _r,
                _m->m00, _m->m10, _m->m20,
                _m->m01, _m->m11, _m->m21,
                _m->m02, _m->m12, _m->m22 );
}

// ------------------------------------------------------------------ 
/*! 
 @fn bool ex_mat33f_inverse ( ex_mat33f_t* _m )
 @retval _m the result vector
 @param _m the in matrix
 @return if the inverse operation successed 
 - true: successful
 - false: failed
 @details get the inversed matrix from matrix _m, override and return it.
 @sa ex_mat33f_get_inverse
*/// ------------------------------------------------------------------ 

bool ex_mat33f_inverse ( ex_mat33f_t* _m );

// ------------------------------------------------------------------ 
/*! 
 @fn bool ex_mat33f_get_inverse ( ex_mat33f_t* _r, ex_mat33f_t* _m )
 @retval _r the result vector
 @param _m the in matrix
 @return if the inverse operation successed 
 - true: successful
 - false: failed
 @details get the inversed matrix from matrix _m, return it to matrix _r
 @sa ex_mat33f_inverse
*/// ------------------------------------------------------------------ 

bool ex_mat33f_get_inverse ( ex_mat33f_t* _r, ex_mat33f_t* _m );

//! @}

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END MAT33F_H_1289531940
// #################################################################################