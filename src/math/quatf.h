// ======================================================================================
// File         : quatf.h
// Author       : Wu Jie 
// Last Change  : 11/12/2010 | 17:05:16 PM | Friday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef QUATF_H_1289552718
#define QUATF_H_1289552718
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

extern struct mat33f_t;
extern struct mat44f_t;

//! @defgroup quatf_t quaternion float
//! @ingroup Math
//! @{

///////////////////////////////////////////////////////////////////////////////
// defines 
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
/*! 
 @struct quatf_t
 @details
 
 The data of quaternion is construct by an union structure with float elements.\n
 The quaternion can be expressed in formular as:
 
 \f$
   \left[\begin{array}{ c c c c }
   x y z w
   \end{array} \right]
 \f$
 
 and in array as:

 \f$
   \left[\begin{array}{ c c c c }
   v_{0}, v_{1}, v_{2}, v_{3}
   \end{array} \right]
 \f$
*/// ------------------------------------------------------------------ 

typedef struct quatf_t {
    union {
        struct { 
            float x, y, z, w;
        }; // end struct
        float v[4];
    };
} quatf_t;

// ------------------------------------------------------------------ 
/*! 
 @fn inline void quatf_zero ( quatf_t* _r )
 @retval _r the result quaternion
 @details make a zero quaternion:

 \f[
   q = \left[\begin{array}{ c c c c }
   0.0 & 0.0 & 0.0 & 0.0
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void quatf_zero ( quatf_t* _r ) { _r->x = 0.0f, _r->y = 0.0f, _r->z = 0.0f, _r->w = 0.0f; }

// ------------------------------------------------------------------ 
/*! 
 @fn inline void quatf_identity ( quatf_t* _r )
 @retval _r the result quaternion
 @details make a identity quaternion:

 \f[
   q = \left[\begin{array}{ c c c c }
   0.0 & 0.0 & 0.0 & 1.0
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void quatf_identity ( quatf_t* _r ) { _r->x = 0.0f, _r->y = 0.0f, _r->z = 0.0f, _r->w = 1.0f; }

// ------------------------------------------------------------------ 
/*! 
 @fn inline void quatf_set ( quatf_t* _r, float _x, float _y, float _z, float _w )
 @retval _r the result quaternion
 @param x
 @param y
 @param z
 @param w
 @details set the quaternion _r by rest of the parameters:

 \f[
   q = \left[\begin{array}{ c c c c }
   \_x & \_y & \_z & \_w
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void quatf_set ( quatf_t* _r, float _x, float _y, float _z, float _w ) { _r->x = _x, _r->y = _y, _r->z = _z, _r->w = _w; }

// ------------------------------------------------------------------ 
/*! 
 @fn inline void quatf_set_from_axis_radians ( quatf_t* _r, vec3f_t* _axis, float _radians )
 @retval _r the result quaternion
 @param _axis
 @param _radians
 @details set the quaternion _r from axis and radians

 \f[
   q = \left[\begin{array}{ c c c c }
   axis_x * sin(radians/2) & 
   axis_y * sin(radians/2) & 
   axis_z * sin(radians/2) & 
   cos(radians/2)
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void quatf_set_from_axis_radians ( quatf_t* _r, vec3f_t* _axis, float _radians ) { 
    float half_radian;
    ex_assert( vec3f_is_normalized(_axis), "axis have not been normalized." );

    half_radian = _radians * 0.5f;

    _r->x = _axis->x * sinf( half_radian );
    _r->y = _axis->y * sinf( half_radian );
    _r->z = _axis->z * sinf( half_radian );
    _r->w = cosf( half_radian );
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline bool quatf_is_equal ( quatf* _lhs, quatf* _rhs )
 @param _lhs the left hand side quaternion
 @param _rhs the right hand side quaternion
 @return the result of the comparation
 @details compare if the two quaternion is equal to each other
*/// ------------------------------------------------------------------ 

inline bool quatf_is_equal ( quatf* _lhs, quatf* _rhs ) {
    return is_equal_float(_lhs->x,_rhs->x,EX_FLOAT_EPS)
        && is_equal_float(_lhs->y,_rhs->y,EX_FLOAT_EPS)
        && is_equal_float(_lhs->z,_rhs->z,EX_FLOAT_EPS)
        && is_equal_float(_lhs->w,_rhs->w,EX_FLOAT_EPS)
        ;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void quatf_neg ( quatf_t* _r )
 @retval _r the result quaternion
 @param _r the in quaternion
 @details get the negtive value from quaternion _r, override and return it as the result:

 \f[
   q = -\left[\begin{array}{ c c c c }
   x & y & z & w
   \end{array} \right]
   = \left[\begin{array}{ c c c c }
   -x & -y & -z & -w
   \end{array} \right]
 \f]
 @sa quatf_get_neg
*/// ------------------------------------------------------------------ 

inline void quatf_neg ( quatf_t* _r ) { _r->x = -_r->x; _r->y = -_r->y; _r->z = -_r->z; _r->w = -_r->w; }

// ------------------------------------------------------------------ 
/*! 
 @fn inline void quatf_get_neg ( quatf_t* _r, quatf_t* _q )
 @retval _r the result quaternion
 @param _q
 @details get the negtive value from quaternion _q and set it in quaternion _r.
 @sa quatf_neg
*/// ------------------------------------------------------------------ 

inline void quatf_get_neg ( quatf_t* _r, quatf_t* _q ) { 
    ex_assert ( _r != _q, "can't use self as return value." );
    _r->x = -_q->x; 
    _r->y = -_q->y; 
    _r->z = -_q->z; 
    _r->w = -_q->w; 
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void quatf_add ( quatf_t* _r, quatf_t* _lhs, quatf_t* _rhs )
 @retval _r the result quaternion
 @param _lhs left hand side quaternion 
 @param _rhs right hand side quaternion 
 @details
 \f[
   \_r = Quat_a + Quat_b = 
   \left[\begin{array}{ c c c c }
   a_{x} & a_{y} & a_{z} & a_{w}
   \end{array} \right] +
   \left[\begin{array}{ c c c c }
   b_{x} & b_{y} & b_{z} & b_{w}
   \end{array} \right]
   =
   \left[\begin{array}{ c c c c }
   a_{x} + b_{x} & 
   a_{y} + b_{y} & 
   a_{z} + b_{z} & 
   a_{w} + b_{w}
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void quatf_add ( quatf_t* _r, quatf_t* _lhs, quatf_t* _rhs ) { 
    _r->x = _lhs->x + _rhs->x; 
    _r->y = _lhs->y + _rhs->y; 
    _r->z = _lhs->z + _rhs->z; 
    _r->w = _lhs->w + _rhs->w; 
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void quatf_add_scalar ( quatf_t* _r, quatf_t* _lhs, float _rhs )
 @retval _r the result quaternion
 @param _lhs left hand side quaternion 
 @param _rhs right hand side float 
 @details
 \f[
   \_r = Quat_a + s = 
   \left[\begin{array}{ c c c c }
   a_{x} & a_{y} & a_{z} & a_{w}
   \end{array} \right] + s
   =
   \left[\begin{array}{ c c c c }
   a_{x} + s & 
   a_{y} + s & 
   a_{z} + s & 
   a_{w} + s
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void quatf_add_scalar ( quatf_t* _r, quatf_t* _lhs, float _rhs ) {
    _r->x = _lhs->x + _rhs; 
    _r->y = _lhs->y + _rhs; 
    _r->z = _lhs->z + _rhs; 
    _r->w = _lhs->w + _rhs; 
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void quatf_sub ( quatf_t* _r, quatf_t* _lhs, quatf_t* _rhs )
 @retval _r the result quaternion
 @param _lhs left hand side quaternion 
 @param _rhs right hand side quaternion 
 @details
 \f[
   \_r = Quat_a - Quat_b = 
   \left[\begin{array}{ c c c c }
   a_{x} & a_{y} & a_{z} & a_{w}
   \end{array} \right] -
   \left[\begin{array}{ c c c c }
   b_{x} & b_{y} & b_{z} & b_{w}
   \end{array} \right]
   =
   \left[\begin{array}{ c c c c }
   a_{x} - b_{x} & 
   a_{y} - b_{y} & 
   a_{z} - b_{z} & 
   a_{w} - b_{w}
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void quatf_sub ( quatf_t* _r, quatf_t* _lhs, quatf_t* _rhs ) { 
    _r->x = _lhs->x - _rhs->x; 
    _r->y = _lhs->y - _rhs->y; 
    _r->z = _lhs->z - _rhs->z; 
    _r->w = _lhs->w - _rhs->w; 
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void quatf_sub_scalar ( quatf_t* _r, quatf_t* _lhs, float _rhs )
 @retval _r the result quaternion
 @param _lhs left hand side quaternion 
 @param _rhs right hand side quaternion 
 @details
 \f[
   \_r = Quat_a - s = 
   \left[\begin{array}{ c c c c }
   a_{x} & a_{y} & a_{z} & a_{w}
   \end{array} \right] - s
   =
   \left[\begin{array}{ c c c c }
   a_{x} - s & 
   a_{y} - s & 
   a_{z} - s & 
   a_{w} - s
   \end{array} \right]
 \f]
 @sa scalar_sub_quatf
*/// ------------------------------------------------------------------ 

inline void quatf_sub_scalar ( quatf_t* _r, quatf_t* _lhs, float _rhs ) {
    _r->x = _lhs->x - _rhs; 
    _r->y = _lhs->y - _rhs; 
    _r->z = _lhs->z - _rhs; 
    _r->w = _lhs->w - _rhs; 
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void scalar_sub_quatf ( quatf_t* _r, float _lhs, quatf_t* _rhs )
 @retval _r the result quaternion
 @param _lhs left hand side quaternion 
 @param _rhs right hand side quaternion 
 @details
 \f[
   \_r = s - Quat_b = 
   s - 
   \left[\begin{array}{ c c c c }
   b_{x} & b_{y} & b_{z} & b_{w}
   \end{array} \right]
   =
   \left[\begin{array}{ c c c c }
   s - b_{x} & 
   s - b_{y} & 
   s - b_{z} & 
   s - b_{w}
   \end{array} \right]
 \f]
 @sa quatf_sub_scalar
*/// ------------------------------------------------------------------ 

inline void scalar_sub_quatf ( quatf_t* _r, float _lhs, quatf_t* _rhs ) { 
    _r->x = _lhs - _rhs->x;
    _r->y = _lhs - _rhs->y;
    _r->z = _lhs - _rhs->z;
    _r->w = _lhs - _rhs->w;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void quatf_mul ( quatf_t* _r, quatf_t* _lhs, quatf_t* _rhs )
 @retval _r the result quaternion
 @param _lhs left hand side quaternion 
 @param _rhs right hand side quaternion 
 @details
 \f[
   \_r = Quat_a * Quat_b = 
   \left[\begin{array}{ c c c c }
   a_{x} & a_{y} & a_{z} & a_{w}
   \end{array} \right] *
   \left[\begin{array}{ c }
   b_{x} \\ 
   b_{y} \\
   b_{z} \\
   b_{w}
   \end{array} \right]
   =
   \left[\begin{array}{ c c c c}
   m_{x} & m_{y} & m_{z} & m_{w}
   \end{array} \right]
 \f]

 where:
 \f[
   \left|\begin{array}{ c }
    m_{x} = a_{x} * b_{w} + a_{w} * b_{x} + a_{z} * b_{y} - a_{y} * b_{z} \\
    m_{y} = a_{y} * b_{w} + a_{w} * b_{y} + a_{x} * b_{z} - a_{z} * b_{x} \\
    m_{z} = a_{z} * b_{w} + a_{w} * b_{z} + a_{y} * b_{x} - a_{x} * b_{y} \\
    m_{w} = a_{w} * b_{w} - a_{x} * b_{x} - a_{y} * b_{y} - a_{z} * b_{z}
   \end{array} \right|
 \f]

*/// ------------------------------------------------------------------ 

inline void quatf_mul ( quatf_t* _r, quatf_t* _lhs, quatf_t* _rhs ) { 
    float x, y, z, w;

    x = _lhs->x * _rhs->w + _lhs->w * _rhs->x + _lhs->z * _rhs->y - _lhs->y * _rhs->z;
    y = _lhs->y * _rhs->w + _lhs->w * _rhs->y + _lhs->x * _rhs->z - _lhs->z * _rhs->x;
    z = _lhs->z * _rhs->w + _lhs->w * _rhs->z + _lhs->y * _rhs->x - _lhs->x * _rhs->y;
    w = _lhs->w * _rhs->w - _lhs->x * _rhs->x - _lhs->y * _rhs->y - _lhs->z * _rhs->z;

    quatf_set( _r, x, y, z, w );
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void quatf_mul_scalar ( quatf_t* _r, quatf_t* _lhs, float _rhs )
 @retval _r the result quaternion
 @param _lhs left hand side quaternion 
 @param _rhs right hand side float 
 @details
 \f[
   \_r = Quat_a * s = 
   \left[\begin{array}{ c c c c }
   a_{x} & a_{y} & a_{z} & a_{w}
   \end{array} \right] * s
   =
   \left[\begin{array}{ c c c c }
   a_{x} * s & 
   a_{y} * s & 
   a_{z} * s & 
   a_{w} * s
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void quatf_mul_scalar ( quatf_t* _r, quatf_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x * _rhs;
    _r->y = _lhs->y * _rhs;
    _r->z = _lhs->z * _rhs;
    _r->w = _lhs->w * _rhs;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void scalar_div_quatf ( quatf_t* _r, float _lhs, quatf_t* _rhs )
 @retval _r the result quaternion
 @param _lhs left hand side float 
 @param _rhs right hand side quaternion 
 @details
 \f[
   \_r = s / Quat_b = 
   s /
   \left[\begin{array}{ c c c c }
   b_{x} & b_{y} & b_{z} & b_{w}
   \end{array} \right]
   =
   \left[\begin{array}{ c c c c }
   s / b_{x} & 
   s / b_{y} & 
   s / b_{z} & 
   s / b_{w}
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void scalar_div_quatf ( quatf_t* _r, float _lhs, quatf_t* _rhs ) { 
    _r->x = _lhs / _rhs->x;
    _r->y = _lhs / _rhs->y;
    _r->z = _lhs / _rhs->z;
    _r->w = _lhs / _rhs->w;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void quatf_div_scalar ( quatf_t* _r, quatf_t* _lhs, float _rhs )
 @retval _r the result quaternion
 @param _lhs left hand side quaternion
 @param _rhs right hand side float  
 @details
 \f[
   \_r = s / Quat_b = 
   \left[\begin{array}{ c c c c }
   a_{x} & a_{y} & a_{z} & a_{w}
   \end{array} \right] / s
   =
   \left[\begin{array}{ c c c c }
   a_{x} / s & 
   a_{y} / s & 
   a_{z} / s & 
   a_{w} / s 
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void quatf_div_scalar ( quatf_t* _r, quatf_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x / _rhs;
    _r->y = _lhs->y / _rhs;
    _r->z = _lhs->z / _rhs;
    _r->w = _lhs->w / _rhs;
}

// ------------------------------------------------------------------ 
/*! 
 @fn bool quatf_inverse( quatf_t* _r )
 @retval _r the result quaternion
 @param _r then in quaternion
 @details get the inversed quaternion from _r, override and return it.
 @sa quatf_get_inverse
*/// ------------------------------------------------------------------ 

bool quatf_inverse( quatf_t* _r );

// ------------------------------------------------------------------ 
/*! 
 @fn bool quatf_get_inverse ( quatf_t* _r, quatf_t* _q )
 @retval _r the result quaternion
 @param _q then in quaternion
 @details get the inversed quaternion from _r, set and return it to quaternion _q.
 @sa quatf_inverse
*/// ------------------------------------------------------------------ 

bool quatf_get_inverse ( quatf_t* _r, quatf_t* _q );

// ------------------------------------------------------------------ 
/*! 
 @fn inline void quatf_conjugate( quatf_t* _r )
 @retval _r the result quaternion
 @param _r then in quaternion
 @details get the conjugate quaternion from _r, override and return it.
 \f[
   \left[\begin{array}{ c c c c }
   a_{x} & a_{y} & a_{z} & a_{w}
   \end{array} \right]
   \stackrel{conjugate}\Longrightarrow
   \left[\begin{array}{ c c c c }
   -a_{x} & -a_{y} & -a_{z} & a_{w}
   \end{array} \right]
 \f]
 @sa quatf_get_conjugate
*/// ------------------------------------------------------------------ 

inline void quatf_conjugate( quatf_t* _r ) {
    _r->x = -_r->x;
    _r->y = -_r->y;
    _r->z = -_r->z;
    _r->w =  _r->w;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void quatf_get_conjugate ( quatf_t* _r, quatf_t* _q )
 @retval _r the result quaternion
 @param _q then in quaternion
 @details get the conjugate quaternion from _r, set and return it to _q. 
 @sa quatf_conjugate
*/// ------------------------------------------------------------------ 

inline void quatf_get_conjugate ( quatf_t* _r, quatf_t* _q ) {
    _r->x = -_q->x;
    _r->y = -_q->y;
    _r->z = -_q->z;
    _r->w =  _q->w;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline float quatf_dot( quatf_t* _lhs, quatf_t* _rhs )
 @param _lhs the left hand quaternion
 @param _rhs the right hand quaternion
 @return the dot product result
 @details get the conjugate quaternion from _r, override and return it.
 \f[
   \left[\begin{array}{ c c c c }
   a_{x} & a_{y} & a_{z} & a_{w}
   \end{array} \right]
   dot
   \left[\begin{array}{ c }
   b_{x} \\ 
   b_{y} \\
   b_{z} \\
   b_{w}
   \end{array} \right]
   =
   a_{x} * b_{x} + 
   a_{y} * b_{y} +
   a_{z} * b_{z} + 
   a_{w} * b_{w}
 \f]
*/// ------------------------------------------------------------------ 

inline float quatf_dot( quatf_t* _lhs, quatf_t* _rhs ) {
    return _lhs->x * _rhs->x 
         + _lhs->y * _rhs->y 
         + _lhs->z * _rhs->z 
         + _lhs->w * _rhs->w;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline float quatf_len( quatf_t* _r )
 @param _r the in quaternion
 @return the length of the in quaternion
 @details return the length of quaternion _r
 @sa quatf_lenSQR
*/// ------------------------------------------------------------------ 

inline float quatf_len( quatf_t* _r ) {
    return sqrtf ( quatf_dot(_r,_r) );
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline float quatf_lenSQR( quatf_t* _r )
 @param _r the in quaternion
 @return the length square of the in quaternion
 @details return the length squre of quaternion _r
 @sa quatf_len
*/// ------------------------------------------------------------------ 

inline float quatf_lenSQR( quatf_t* _r ) {
    return quatf_dot(_r,_r);
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline bool quatf_is_normalized( quatf_t* _r )
 @param _r the in quaternion
 @return if the inverse operation successed 
 - true: successful
 - false: failed
 @details check if the in quaternion is normalized 
 @sa quatf_get_normalize
 @sa quatf_normalize
*/// ------------------------------------------------------------------ 

inline bool quatf_is_normalized( quatf_t* _r ) {
    return is_equal_float( quatf_lenSQR(_r), 1.0f, EX_FLOAT_EPS );
}

// ------------------------------------------------------------------ 
/*! 
 @fn bool quatf_get_normalize( quatf_t* _r, quatf_t* _q )
 @retval _r the result quaternion
 @param _q the in quaternion
 @return if the inverse operation successed 
 - true: successful
 - false: failed
 @details get the normalized quaternion from _r, set and return it to quaternion _q
 @sa quatf_is_normalized
 @sa quatf_normalize
*/// ------------------------------------------------------------------ 

inline bool quatf_get_normalize( quatf_t* _r, quatf_t* _q ) {
    float len_sqr, inv_len;

    len_sqr = quatf_lenSQR(_q);
    if ( is_equal_float( len_sqr, 1.0f, EX_FLOAT_EPS ) ) {
        *_r = *_q;
        return true;
    }

    if ( is_zero_float( len_sqr, EX_FLOAT_EPS ) )
        return false;

    inv_len = inv_sqrtf (len_sqr);
    quatf_mul_scalar ( _r, _q, inv_len );
    return true;
}

// ------------------------------------------------------------------ 
/*! 
 @fn bool quatf_normalize( quatf_t* _r )
 @retval _r the result quaternion
 @param _r the in quaternion
 @return if the inverse operation successed 
 - true: successful
 - false: failed
 @details get the normalized quaternion from _r, override and return it.
 @sa quatf_is_normalized
 @sa quatf_get_normalize
*/// ------------------------------------------------------------------ 

inline bool quatf_normalize( quatf_t* _r ) {
    float len_sqr, inv_len;

    len_sqr = quatf_lenSQR(_r);
    if ( is_equal_float( len_sqr, 1.0f, EX_FLOAT_EPS ) )
        return true;

    if( is_zero_float( len_sqr, EX_FLOAT_EPS ) )
        return false;

    inv_len = inv_sqrtf (len_sqr);
    quatf_mul_scalar ( _r, _r, inv_len );
    return true;
}

// ------------------------------------------------------------------ 
/*! 
 @fn void quatf_to_mat33( quatf_t* _q, mat33f_t* _m )
 @param _q the in quaternion
 @retval _m the converted matrix
 @details convert a quaternion to a matrix 3x3
 @sa quatf_to_mat44
 @sa quatf_to_axis_xyz
*/// ------------------------------------------------------------------ 

void quatf_to_mat33( quatf_t* _q, mat33f_t* _m );

// ------------------------------------------------------------------ 
/*! 
 @fn void quatf_to_mat44( quatf_t* _q, mat44f_t* _m )
 @param _q the in quaternion
 @retval _m the converted matrix
 @details convert a quaternion to a matrix 4x4
 @sa quatf_to_mat33
 @sa quatf_to_axis_xyz
*/// ------------------------------------------------------------------ 

void quatf_to_mat44( quatf_t* _q, mat44f_t* _m );

// ------------------------------------------------------------------ 
/*! 
 @fn void quatf_to_axis_xyz( quatf_t* _q, vec3f_t* _x, vec3f_t* _y, vec3f_t* _z )
 @param _q the in quaternion
 @retval _x the axis x
 @retval _y the axis y
 @retval _z the axis z
 @details convert a quaternion to axis x,y,z
 @sa quatf_to_mat33
 @sa quatf_to_mat44
*/// ------------------------------------------------------------------ 

void quatf_to_axis_xyz( quatf_t* _q, vec3f_t* _x, vec3f_t* _y, vec3f_t* _z );

// ------------------------------------------------------------------ 
/*! 
 @fn void quatf_get_axis_x( vec3f_t* _r, quatf_t* _q )
 @retval _r the axis x
 @param _q the in quaternion
 @details get axis x from quaternion _q
 @sa quatf_to_axis_xyz
 @sa quatf_get_axis_y
 @sa quatf_get_axis_z
*/// ------------------------------------------------------------------ 

void quatf_get_axis_x( vec3f_t* _r, quatf_t* _q );

// ------------------------------------------------------------------ 
/*! 
 @fn void quatf_get_axis_y( vec3f_t* _r, quatf_t* _q )
 @retval _r the axis y
 @param _q the in quaternion
 @details get axis y from quaternion _q
 @sa quatf_to_axis_xyz
 @sa quatf_get_axis_x
 @sa quatf_get_axis_z
*/// ------------------------------------------------------------------ 

void quatf_get_axis_y( vec3f_t* _r, quatf_t* _q );

// ------------------------------------------------------------------ 
/*! 
 @fn void quatf_get_axis_z( vec3f_t* _r, quatf_t* _q )
 @retval _r the axis z
 @param _q the in quaternion
 @details get axis z from quaternion _q
 @sa quatf_to_axis_xyz
 @sa quatf_get_axis_x
 @sa quatf_get_axis_y
*/// ------------------------------------------------------------------ 

void quatf_get_axis_z( vec3f_t* _r, quatf_t* _q );

//! @}

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END QUATF_H_1289552718
// #################################################################################
