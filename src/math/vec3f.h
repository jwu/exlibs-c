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

//! @defgroup vec3f_t vector3 float
//! @ingroup Math
//! @{

///////////////////////////////////////////////////////////////////////////////
// define
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
/*! 
 @struct vec3f_t
 @details
 
 The data of vector3 is construct by an union structure with float elements.\n
 The vector3 can be expressed in formular as:
 
 \f$
   \left[\begin{array}{ c c c }
   x & y & z
   \end{array} \right]
 \f$
 
 and in array as:

 \f$
   \left[\begin{array}{ c c c }
   v_{0}, v_{1}, v_{2}
   \end{array} \right]
 \f$
*/// ------------------------------------------------------------------ 

typedef struct vec3f_t {
    union {
        struct { float x, y, z; }; // end struct
        float v[3];
    };
} vec3f_t;

// ------------------------------------------------------------------ 
/*! 
 @fn inline void vec3f_zero ( vec3f_t* _r )
 @retval _r the result vector3
 @details make a zero vector3:

 \f[
   q = \left[\begin{array}{ c c c }
   0.0 & 0.0 & 0.0
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void vec3f_zero ( vec3f_t* _r ) { _r->x = 0.0f, _r->y = 0.0f; _r->z = 0.0f; }

// ------------------------------------------------------------------ 
/*! 
 @fn inline void vec3f_one ( vec3f_t* _r )
 @retval _r the result vector3
 @details make an one vector3:

 \f[
   q = \left[\begin{array}{ c c c }
   1.0 & 1.0 & 1.0
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void vec3f_one ( vec3f_t* _r ) { _r->x = 1.0f, _r->y = 1.0f; _r->z = 1.0f; }

// ------------------------------------------------------------------ 
/*! 
 @fn inline void vec3f_set ( vec3f_t* _r, float _x, float _y, float _z )
 @retval _r the result vector3
 @param x
 @param y
 @param z
 @details set the vector3 _r by rest of the parameters:

 \f[
   q = \left[\begin{array}{ c c c }
   \_x & \_y & \_z
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void vec3f_set ( vec3f_t* _r, float _x, float _y, float _z ) { _r->x = _x, _r->y = _y, _r->z = _z; }

// ------------------------------------------------------------------ 
/*! 
 @fn inline bool vec3f_is_equal ( vec3f_t* _lhs, vec3f_t* _rhs )
 @param _lhs the left hand side vector3
 @param _rhs the right hand side vector3
 @return the result of the comparation
 @details compare if the two vector3 is equal to each other
*/// ------------------------------------------------------------------ 

inline bool vec3f_is_equal ( vec3f_t* _lhs, vec3f_t* _rhs ) { 
    return ex_is_equal_float(_lhs->x,_rhs->x,EX_FLOAT_EPS)
        && ex_is_equal_float(_lhs->y,_rhs->y,EX_FLOAT_EPS)
        && ex_is_equal_float(_lhs->z,_rhs->z,EX_FLOAT_EPS);
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void vec3f_neg ( vec3f_t* _v )
 @retval _v the result vector3
 @param _v the in vector3
 @details get the negtive value from vector3 _v, override and return it as the result:

 \f[
   q = -\left[\begin{array}{ c c c }
   x & y & z
   \end{array} \right]
   = \left[\begin{array}{ c c c }
   -x & -y & -z
   \end{array} \right]
 \f]
 @sa vec3f_get_neg
*/// ------------------------------------------------------------------ 

inline void vec3f_neg ( vec3f_t* _v ) { 
    _v->x = -_v->x; 
    _v->y = -_v->y; 
    _v->z = -_v->z; 
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void vec3f_get_neg ( vec3f_t* _r, vec3f_t* _v )
 @retval _r the result vector3
 @param _v the in vector3
 @details get the negtive value from vector3 _v, set and return it to vector3 _v
 @sa vec3f_neg
*/// ------------------------------------------------------------------ 

inline void vec3f_get_neg ( vec3f_t* _r, vec3f_t* _v ) { 
    ex_assert ( _r != _v, "can't use self as return value." );
    _r->x = -_v->x; 
    _r->y = -_v->y; 
    _r->z = -_v->z; 
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void vec3f_add ( vec3f_t* _r, vec3f_t* _lhs, vec3f_t* _rhs )
 @retval _r the result vector3
 @param _lhs the left hand side vector3
 @param _rhs the right hand side vector3
 @details
 \f[
   \_r = Vec_a + Vec_b = 
   \left[\begin{array}{ c c c }
   a_{x} & a_{y} & a_{z}
   \end{array} \right] +
   \left[\begin{array}{ c c c }
   b_{x} & b_{y} & b_{z}
   \end{array} \right]
   =
   \left[\begin{array}{ c c c }
   a_{x} + b_{x} & 
   a_{y} + b_{y} &
   a_{z} + b_{z}
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void vec3f_add ( vec3f_t* _r, vec3f_t* _lhs, vec3f_t* _rhs ) { 
    _r->x = _lhs->x + _rhs->x;
    _r->y = _lhs->y + _rhs->y;
    _r->z = _lhs->z + _rhs->z;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void vec3f_add_scalar ( vec3f_t* _r, vec3f_t* _lhs, float _rhs )
 @retval _r the result vector3
 @param _lhs the left hand side vector3
 @param _rhs the right hand side float
 @details
 \f[
   \_r = Vec_a + s = 
   \left[\begin{array}{ c c c }
   a_{x} & a_{y} & a_{z}
   \end{array} \right] + s
   =
   \left[\begin{array}{ c c }
   a_{x} + s & 
   a_{y} + s &
   a_{z} + s
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void vec3f_add_scalar ( vec3f_t* _r, vec3f_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x + _rhs;
    _r->y = _lhs->y + _rhs;
    _r->z = _lhs->z + _rhs;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void vec3f_sub ( vec3f_t* _r, vec3f_t* _lhs, vec3f_t* _rhs )
 @retval _r the result vector3
 @param _lhs the left hand side vector3
 @param _rhs the right hand side vector3
 @details
 \f[
   \_r = Vec_a - Vec_b = 
   \left[\begin{array}{ c c c }
   a_{x} & a_{y} & a_{z}
   \end{array} \right] -
   \left[\begin{array}{ c c c }
   b_{x} & b_{y} & b_{z}
   \end{array} \right]
   =
   \left[\begin{array}{ c c c }
   a_{x} - b_{x} & 
   a_{y} - b_{y} &
   a_{z} - b_{z}
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void vec3f_sub ( vec3f_t* _r, vec3f_t* _lhs, vec3f_t* _rhs ) { 
    _r->x = _lhs->x - _rhs->x;
    _r->y = _lhs->y - _rhs->y;
    _r->z = _lhs->z - _rhs->z;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void vec3f_sub_scalar ( vec3f_t* _r, vec3f_t* _lhs, float _rhs )
 @retval _r the result vector3
 @param _lhs the left hand side vector3
 @param _rhs the right hand side float
 @details
 \f[
   \_r = Vec_a - s = 
   \left[\begin{array}{ c c c }
   a_{x} & a_{y} & a_{z}
   \end{array} \right] - s
   =
   \left[\begin{array}{ c c c }
   a_{x} - s & 
   a_{y} - s &
   a_{z} - s
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void vec3f_sub_scalar ( vec3f_t* _r, vec3f_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x - _rhs;
    _r->y = _lhs->y - _rhs;
    _r->z = _lhs->z - _rhs;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void scalar_sub_vec3f ( vec3f_t* _r, float _lhs, vec3f_t* _rhs )
 @retval _r the result vector3
 @param _lhs the left hand side float
 @param _rhs the right hand side vector3
 @details
 \f[
   \_r = s - Vec_b = 
   s - 
   \left[\begin{array}{ c c c }
   b_{x} & b_{y} & b_{z}
   \end{array} \right]
   =
   \left[\begin{array}{ c c c }
   s - b_{x} & 
   s - b_{y} &
   s - b_{z}
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void scalar_sub_vec3f ( vec3f_t* _r, float _lhs, vec3f_t* _rhs ) { 
    _r->x = _lhs - _rhs->x;
    _r->y = _lhs - _rhs->y;
    _r->z = _lhs - _rhs->z;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void vec3f_mul ( vec3f_t* _r, vec3f_t* _lhs, vec3f_t* _rhs )
 @retval _r the result vector3
 @param _lhs the left hand side vector3
 @param _rhs the right hand side vector3
 @details
 \f[
   \_r = Vec_a * Vec_b = 
   \left[\begin{array}{ c c c }
   a_{x} & a_{y} & a_{z}
   \end{array} \right] *
   \left[\begin{array}{ c c c }
   b_{x} & b_{y} & b_{z}
   \end{array} \right]
   =
   \left[\begin{array}{ c c c }
   a_{x} * b_{x} & 
   a_{y} * b_{y} &
   a_{z} * b_{z}
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void vec3f_mul ( vec3f_t* _r, vec3f_t* _lhs, vec3f_t* _rhs ) { 
    _r->x = _lhs->x * _rhs->x;
    _r->y = _lhs->y * _rhs->y;
    _r->z = _lhs->z * _rhs->z;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void vec3f_mul_scalar ( vec3f_t* _r, vec3f_t* _lhs, float _rhs )
 @retval _r the result vector3
 @param _lhs the left hand side vector3
 @param _rhs the right hand side float
 @details
 \f[
   \_r = Vec_a * s = 
   \left[\begin{array}{ c c c }
   a_{x} & a_{y} & a_{z}
   \end{array} \right] * s
   =
   \left[\begin{array}{ c c c }
   a_{x} * s & 
   a_{y} * s &
   a_{z} * s
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void vec3f_mul_scalar ( vec3f_t* _r, vec3f_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x * _rhs;
    _r->y = _lhs->y * _rhs;
    _r->z = _lhs->z * _rhs;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void vec3f_div ( vec3f_t* _r, vec3f_t* _lhs, vec3f_t* _rhs )
 @retval _r the result vector3
 @param _lhs the left hand side vector3
 @param _rhs the right hand side vector3
 @details
 \f[
   \_r = Vec_a / Vec_b = 
   \left[\begin{array}{ c c c }
   a_{x} & a_{y} & a_{z}
   \end{array} \right] /
   \left[\begin{array}{ c c c }
   b_{x} & b_{y} & b_{z}
   \end{array} \right]
   =
   \left[\begin{array}{ c c c }
   a_{x} / b_{x} & 
   a_{y} / b_{y} &
   a_{z} / b_{z}
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void vec3f_div ( vec3f_t* _r, vec3f_t* _lhs, vec3f_t* _rhs ) { 
    _r->x = _lhs->x / _rhs->x;
    _r->y = _lhs->y / _rhs->y;
    _r->z = _lhs->z / _rhs->z;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void vec3f_div_scalar ( vec3f_t* _r, vec3f_t* _lhs, float _rhs )
 @retval _r the result vector3
 @param _lhs the left hand side vector3
 @param _rhs the right hand side float
 @details
 \f[
   \_r = Vec_a / s = 
   \left[\begin{array}{ c c c }
   a_{x} & a_{y} & a_{z}
   \end{array} \right] / s
   =
   \left[\begin{array}{ c c c }
   a_{x} / s & 
   a_{y} / s &
   a_{z} / s
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void vec3f_div_scalar ( vec3f_t* _r, vec3f_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x / _rhs;
    _r->y = _lhs->y / _rhs;
    _r->z = _lhs->z / _rhs;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void scalar_div_vec3f ( vec3f_t* _r, float _lhs, vec3f_t* _rhs )
 @retval _r the result vector3
 @param _lhs the left hand side float
 @param _rhs the right hand side vector3
 @details
 \f[
   \_r = s / Vec_b = 
   s / 
   \left[\begin{array}{ c c c }
   b_{x} & b_{y} & b_{z}
   \end{array} \right]
   =
   \left[\begin{array}{ c c c }
   s / b_{x} & 
   s / b_{y} &
   s / b_{z}
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void scalar_div_vec3f ( vec3f_t* _r, float _lhs, vec3f_t* _rhs ) { 
    _r->x = _lhs / _rhs->x;
    _r->y = _lhs / _rhs->y;
    _r->z = _lhs / _rhs->z;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline float vec3f_dot ( vec3f_t* _lhs, vec3f_t* _rhs )
 @param _lhs the left hand vector3
 @param _rhs the right hand vector3
 @return the dot product result
 @details get the dot product
 \f[
   \left[\begin{array}{ c c c }
   a_{x} & a_{y} & a_{z}
   \end{array} \right]
   dot
   \left[\begin{array}{ c }
   b_{x} \\ 
   b_{y} \\
   b_{z}
   \end{array} \right]
   =
   a_{x} * b_{x} + 
   a_{y} * b_{y} +
   a_{z} * b_{z}
 \f]
*/// ------------------------------------------------------------------ 

inline float vec3f_dot ( vec3f_t* _lhs, vec3f_t* _rhs ) {
    return _lhs->x * _rhs->x 
         + _lhs->y * _rhs->y 
         + _lhs->z * _rhs->z;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline float vec3f_cross ( vec3f_t* _r, vec3f_t* _lhs, vec3f_t* _rhs )
 @retval _r the cross product result
 @param _lhs the left hand vector3
 @param _rhs the right hand vector3
 @details get the cross product
 \f[
   \left[\begin{array}{ c c c }
   a_{x} & a_{y} & a_{z}
   \end{array} \right]
   cross
   \left[\begin{array}{ c }
   b_{x} \\ 
   b_{y} \\
   b_{z}
   \end{array} \right]
   =
   \left[\begin{array}{ c c c }
   a_{y} * b_{z} - a_{z} * b_{y} &
   a_{z} * b_{x} - a_{x} * b_{z} &
   a_{x} * b_{y} - a_{y} * b_{x}
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void vec3f_cross ( vec3f_t* _r, vec3f_t* _lhs, vec3f_t* _rhs ) {
    _r->x = _lhs->y * _rhs->z - _lhs->z * _rhs->y;
    _r->y = _lhs->z * _rhs->x - _lhs->x * _rhs->z;
    _r->z = _lhs->x * _rhs->y - _lhs->y * _rhs->x;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline float vec3f_len( vec3f_t* _v )
 @param _v the in vector3
 @return the length of the in vector3
 @details return the length of vector3 _v
 @sa vec3f_lenSQR
*/// ------------------------------------------------------------------ 

inline float vec3f_len ( vec3f_t* _v ) {
    return sqrtf ( vec3f_dot(_v,_v) );
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline float vec3f_lenSQR( vec3f_t* _v )
 @param _v the in vector3
 @return the length square of the in vector3
 @details return the length square of vector3 _v
 @sa vec3f_len
*/// ------------------------------------------------------------------ 

inline float vec3f_lenSQR ( vec3f_t* _v ) {
    return vec3f_dot(_v,_v);
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline bool vec3f_is_normalized ( vec3f_t* _v )
 @param _v the in vector3
 @return if the inverse operation successed 
 - true: successful
 - false: failed
 @details check if the in vector3 is normalized 
 @sa vec3f_get_normalize
 @sa vec3f_normalize
*/// ------------------------------------------------------------------ 

inline bool vec3f_is_normalized ( vec3f_t* _v ) {
    return ex_is_equal_float( vec3f_lenSQR(_v), 1.0f, EX_FLOAT_EPS);
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline bool vec3f_normalize ( vec3f_t* _v )
 @param _v the in vector3
 @return if the inverse operation successed 
 - true: successful
 - false: failed
 @details check if the in vector3 is normalized 
 @sa vec3f_is_normalized
 @sa vec3f_get_normalize
*/// ------------------------------------------------------------------ 

inline bool vec3f_normalize ( vec3f_t* _v ) {
    float length_sqr = vec3f_lenSQR(_v);
    float inv_length = 0.0f; 

    if ( ex_is_equal_float( length_sqr, 1.0f, EX_FLOAT_EPS ) )
        return true;

    if( ex_is_zero_float( length_sqr, EX_FLOAT_EPS ) )
        return false;

    inv_length = ex_inv_sqrtf( length_sqr );
    vec3f_mul_scalar ( _v, _v, inv_length );
}

// ------------------------------------------------------------------ 
/*! 
 @fn bool vec3f_get_normalize ( vec3f_t* _r, vec3f_t* _v )
 @retval _r the result vector3
 @param _v the in vector3
 @return if the inverse operation successed 
 - true: successful
 - false: failed
 @details get the normalized vector3 from _r, set and return it to vector3 _v
 @sa vec3f_is_normalized
 @sa vec3f_normalize
*/// ------------------------------------------------------------------ 

inline bool vec3f_get_normalize ( vec3f_t* _r, vec3f_t* _v ) {
    float length_sqr, inv_length;

    ex_assert ( _r != _v, "can't use self as return value." );
    length_sqr = vec3f_lenSQR(_v);
    inv_length = 0.0f; 

    if ( ex_is_equal_float( length_sqr, 1.0f, EX_FLOAT_EPS ) ) {
        return true;
    }

    if( ex_is_zero_float( length_sqr, EX_FLOAT_EPS ) ) {
        return false;
    }

    inv_length = ex_inv_sqrtf( length_sqr );
    vec3f_mul_scalar ( _r, _v, inv_length );
}

//! @}

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END VEC3F_H_1289444915
// #################################################################################
