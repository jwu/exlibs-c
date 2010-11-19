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

//! @defgroup ex_vec2f_t vector2 float
//! @ingroup Math
//! @{

///////////////////////////////////////////////////////////////////////////////
// define
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
/*! 
 @struct ex_vec2f_t
 @details
 
 The data of vector2 is construct by an union structure with float elements.\n
 The vector2 can be expressed in formular as:
 
 \f$
   \left[\begin{array}{ c c }
   x & y
   \end{array} \right]
 \f$
 
 and in array as:

 \f$
   \left[\begin{array}{ c c }
   v_{0}, v_{1}
   \end{array} \right]
 \f$
*/// ------------------------------------------------------------------ 

typedef struct ex_vec2f_t {
    union {
        struct { float x, y; }; // end struct
        float v[2];
    };
} ex_vec2f_t;

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_vec2f_zero ( ex_vec2f_t* _r )
 @retval _r the result vector2
 @details make a zero vector2:

 \f[
   q = \left[\begin{array}{ c c }
   0.0 & 0.0
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void ex_vec2f_zero ( ex_vec2f_t* _r ) { _r->x = 0.0f, _r->y = 0.0f; }

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_vec2f_one ( ex_vec2f_t* _r )
 @retval _r the result vector2
 @details make an one vector2:

 \f[
   q = \left[\begin{array}{ c c }
   1.0 & 1.0
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void ex_vec2f_one ( ex_vec2f_t* _r ) { _r->x = 1.0f, _r->y = 1.0f; }

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_vec2f_set ( ex_vec2f_t* _r, float _x, float _y )
 @retval _r the result vector2
 @param x
 @param y
 @details set the vector2 _r by rest of the parameters:

 \f[
   q = \left[\begin{array}{ c c }
   \_x & \_y
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void ex_vec2f_set ( ex_vec2f_t* _r, float _x, float _y ) { _r->x = _x, _r->y = _y; }

// ------------------------------------------------------------------ 
/*! 
 @fn inline bool ex_vec2f_is_equal ( ex_vec2f_t* _lhs, ex_vec2f_t* _rhs )
 @param _lhs the left hand side vector2
 @param _rhs the right hand side vector2
 @return the result of the comparation
 @details compare if the two vector2 is equal to each other
*/// ------------------------------------------------------------------ 

inline bool ex_vec2f_is_equal ( ex_vec2f_t* _lhs, ex_vec2f_t* _rhs ) { 
    return ex_is_equal_float(_lhs->x,_rhs->x,EX_FLOAT_EPS)
        && ex_is_equal_float(_lhs->y,_rhs->y,EX_FLOAT_EPS);
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_vec2f_neg ( ex_vec2f_t* _v )
 @retval _v the result vector2
 @param _v the in vector2
 @details get the negtive value from vector2 _v, override and return it as the result:

 \f[
   q = -\left[\begin{array}{ c c }
   x & y
   \end{array} \right]
   = \left[\begin{array}{ c c }
   -x & -y
   \end{array} \right]
 \f]
 @sa ex_vec2f_get_neg
*/// ------------------------------------------------------------------ 

inline void ex_vec2f_neg ( ex_vec2f_t* _v ) { 
    _v->x = -_v->x; 
    _v->y = -_v->y; 
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_vec2f_get_neg ( ex_vec2f_t* _r, ex_vec2f_t* _v )
 @retval _r the result vector2
 @param _v the in vector2
 @details get the negtive value from vector2 _v, set and return it to vector2 _v
 @sa ex_vec2f_neg
*/// ------------------------------------------------------------------ 

inline void ex_vec2f_get_neg ( ex_vec2f_t* _r, ex_vec2f_t* _v ) { 
    ex_assert ( _r != _v, "can't use self as return value." );
    _r->x = -_v->x; 
    _r->y = -_v->y; 
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_vec2f_add ( ex_vec2f_t* _r, ex_vec2f_t* _lhs, ex_vec2f_t* _rhs )
 @retval _r the result vector2
 @param _lhs the left hand side vector2
 @param _rhs the right hand side vector2
 @details
 \f[
   \_r = Vec_a + Vec_b = 
   \left[\begin{array}{ c c }
   a_{x} & a_{y}
   \end{array} \right] +
   \left[\begin{array}{ c c }
   b_{x} & b_{y}
   \end{array} \right]
   =
   \left[\begin{array}{ c c }
   a_{x} + b_{x} & 
   a_{y} + b_{y}
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void ex_vec2f_add ( ex_vec2f_t* _r, ex_vec2f_t* _lhs, ex_vec2f_t* _rhs ) { 
    _r->x = _lhs->x + _rhs->x;
    _r->y = _lhs->y + _rhs->y;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_vec2f_add_scalar ( ex_vec2f_t* _r, ex_vec2f_t* _lhs, float _rhs )
 @retval _r the result vector2
 @param _lhs the left hand side vector2
 @param _rhs the right hand side float
 @details
 \f[
   \_r = Vec_a + s = 
   \left[\begin{array}{ c c }
   a_{x} & a_{y}
   \end{array} \right] + s
   =
   \left[\begin{array}{ c c }
   a_{x} + s & 
   a_{y} + s
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void ex_vec2f_add_scalar ( ex_vec2f_t* _r, ex_vec2f_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x + _rhs;
    _r->y = _lhs->y + _rhs;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_vec2f_sub ( ex_vec2f_t* _r, ex_vec2f_t* _lhs, ex_vec2f_t* _rhs )
 @retval _r the result vector2
 @param _lhs the left hand side vector2
 @param _rhs the right hand side vector2
 @details
 \f[
   \_r = Vec_a - Vec_b = 
   \left[\begin{array}{ c c }
   a_{x} & a_{y}
   \end{array} \right] -
   \left[\begin{array}{ c c }
   b_{x} & b_{y}
   \end{array} \right]
   =
   \left[\begin{array}{ c c }
   a_{x} - b_{x} & 
   a_{y} - b_{y}
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void ex_vec2f_sub ( ex_vec2f_t* _r, ex_vec2f_t* _lhs, ex_vec2f_t* _rhs ) { 
    _r->x = _lhs->x - _rhs->x;
    _r->y = _lhs->y - _rhs->y;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_vec2f_sub_scalar ( ex_vec2f_t* _r, ex_vec2f_t* _lhs, float _rhs )
 @retval _r the result vector2
 @param _lhs the left hand side vector2
 @param _rhs the right hand side float
 @details
 \f[
   \_r = Vec_a - s = 
   \left[\begin{array}{ c c }
   a_{x} & a_{y}
   \end{array} \right] - s
   =
   \left[\begin{array}{ c c }
   a_{x} - s & 
   a_{y} - s
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void ex_vec2f_sub_scalar ( ex_vec2f_t* _r, ex_vec2f_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x - _rhs;
    _r->y = _lhs->y - _rhs;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_scalar_sub_vec2f ( ex_vec2f_t* _r, float _lhs, ex_vec2f_t* _rhs )
 @retval _r the result vector2
 @param _lhs the left hand side float
 @param _rhs the right hand side vector2
 @details
 \f[
   \_r = s - Vec_b = 
   s - 
   \left[\begin{array}{ c c }
   b_{x} & b_{y}
   \end{array} \right]
   =
   \left[\begin{array}{ c c }
   s - b_{x} & 
   s - b_{y}
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void ex_scalar_sub_vec2f ( ex_vec2f_t* _r, float _lhs, ex_vec2f_t* _rhs ) { 
    _r->x = _lhs - _rhs->x;
    _r->y = _lhs - _rhs->y;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_vec2f_mul ( ex_vec2f_t* _r, ex_vec2f_t* _lhs, ex_vec2f_t* _rhs )
 @retval _r the result vector2
 @param _lhs the left hand side vector2
 @param _rhs the right hand side vector2
 @details
 \f[
   \_r = Vec_a * Vec_b = 
   \left[\begin{array}{ c c }
   a_{x} & a_{y}
   \end{array} \right] *
   \left[\begin{array}{ c c }
   b_{x} & b_{y}
   \end{array} \right]
   =
   \left[\begin{array}{ c c }
   a_{x} * b_{x} & 
   a_{y} * b_{y}
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void ex_vec2f_mul ( ex_vec2f_t* _r, ex_vec2f_t* _lhs, ex_vec2f_t* _rhs ) { 
    _r->x = _lhs->x * _rhs->x;
    _r->y = _lhs->y * _rhs->y;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_vec2f_mul_scalar ( ex_vec2f_t* _r, ex_vec2f_t* _lhs, float _rhs )
 @retval _r the result vector2
 @param _lhs the left hand side vector2
 @param _rhs the right hand side float
 @details
 \f[
   \_r = Vec_a * s = 
   \left[\begin{array}{ c c }
   a_{x} & a_{y}
   \end{array} \right] * s
   =
   \left[\begin{array}{ c c }
   a_{x} * s & 
   a_{y} * s
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void ex_vec2f_mul_scalar ( ex_vec2f_t* _r, ex_vec2f_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x * _rhs;
    _r->y = _lhs->y * _rhs;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_vec2f_div ( ex_vec2f_t* _r, ex_vec2f_t* _lhs, ex_vec2f_t* _rhs )
 @retval _r the result vector2
 @param _lhs the left hand side vector2
 @param _rhs the right hand side vector2
 @details
 \f[
   \_r = Vec_a / Vec_b = 
   \left[\begin{array}{ c c }
   a_{x} & a_{y}
   \end{array} \right] /
   \left[\begin{array}{ c c }
   b_{x} & b_{y}
   \end{array} \right]
   =
   \left[\begin{array}{ c c }
   a_{x} / b_{x} & 
   a_{y} / b_{y}
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void ex_vec2f_div ( ex_vec2f_t* _r, ex_vec2f_t* _lhs, ex_vec2f_t* _rhs ) { 
    _r->x = _lhs->x / _rhs->x;
    _r->y = _lhs->y / _rhs->y;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_vec2f_div_scalar ( ex_vec2f_t* _r, ex_vec2f_t* _lhs, float _rhs )
 @retval _r the result vector2
 @param _lhs the left hand side vector2
 @param _rhs the right hand side float
 @details
 \f[
   \_r = Vec_a / s = 
   \left[\begin{array}{ c c }
   a_{x} & a_{y}
   \end{array} \right] / s
   =
   \left[\begin{array}{ c c }
   a_{x} / s & 
   a_{y} / s
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void ex_vec2f_div_scalar ( ex_vec2f_t* _r, ex_vec2f_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x / _rhs;
    _r->y = _lhs->y / _rhs;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_scalar_div_vec2f ( ex_vec2f_t* _r, float _lhs, ex_vec2f_t* _rhs )
 @retval _r the result vector2
 @param _lhs the left hand side float
 @param _rhs the right hand side vector2
 @details
 \f[
   \_r = s / Vec_b = 
   s / 
   \left[\begin{array}{ c c }
   b_{x} & b_{y}
   \end{array} \right]
   =
   \left[\begin{array}{ c c }
   s / b_{x} & 
   s / b_{y}
   \end{array} \right]
 \f]
*/// ------------------------------------------------------------------ 

inline void ex_scalar_div_vec2f ( ex_vec2f_t* _r, float _lhs, ex_vec2f_t* _rhs ) { 
    _r->x = _lhs / _rhs->x;
    _r->y = _lhs / _rhs->y;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline float ex_vec2f_dot ( ex_vec2f_t* _lhs, ex_vec2f_t* _rhs )
 @param _lhs the left hand vector2
 @param _rhs the right hand vector2
 @return the dot product result
 @details get the dot product
 \f[
   \left[\begin{array}{ c c }
   a_{x} & a_{y}
   \end{array} \right]
   dot
   \left[\begin{array}{ c }
   b_{x} \\ 
   b_{y}
   \end{array} \right]
   =
   a_{x} * b_{x} + 
   a_{y} * b_{y}
 \f]
*/// ------------------------------------------------------------------ 

inline float ex_vec2f_dot ( ex_vec2f_t* _lhs, ex_vec2f_t* _rhs ) {
    return _lhs->x * _rhs->x + _lhs->y * _rhs->y;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline float ex_vec2f_cross ( ex_vec2f_t* _lhs, ex_vec2f_t* _rhs )
 @param _lhs the left hand vector2
 @param _rhs the right hand vector2
 @return the cross product result
 @details get the cross product
 \f[
   \left[\begin{array}{ c c }
   a_{x} & a_{y}
   \end{array} \right]
   cross
   \left[\begin{array}{ c }
   b_{x} \\ 
   b_{y}
   \end{array} \right]
   =
   a_{x} * b_{y} - 
   a_{y} * b_{x}
 \f]
*/// ------------------------------------------------------------------ 

inline float ex_vec2f_cross ( ex_vec2f_t* _lhs, ex_vec2f_t* _rhs ) {
    return _lhs->x * _rhs->y - _lhs->y * _rhs->x;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline float ex_vec2f_len( ex_vec2f_t* _v )
 @param _v the in vector2
 @return the length of the in vector2
 @details return the length of vector2 _v
 @sa ex_vec2f_lenSQR
*/// ------------------------------------------------------------------ 

inline float ex_vec2f_len ( ex_vec2f_t* _v ) {
    return sqrtf ( ex_vec2f_dot(_v,_v) );
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline float ex_vec2f_lenSQR( ex_vec2f_t* _v )
 @param _v the in vector2
 @return the length square of the in vector2
 @details return the length square of vector2 _v
 @sa ex_vec2f_len
*/// ------------------------------------------------------------------ 

inline float ex_vec2f_lenSQR ( ex_vec2f_t* _v ) {
    return ex_vec2f_dot(_v,_v);
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline bool ex_vec2f_is_normalized ( ex_vec2f_t* _v )
 @param _v the in vector2
 @return if the inverse operation successed 
 - true: successful
 - false: failed
 @details check if the in vector2 is normalized 
 @sa ex_vec2f_get_normalize
 @sa ex_vec2f_normalize
*/// ------------------------------------------------------------------ 

inline bool ex_vec2f_is_normalized ( ex_vec2f_t* _v ) {
    return ex_is_equal_float( ex_vec2f_lenSQR(_v), 1.0f, EX_FLOAT_EPS);
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline bool ex_vec2f_normalize ( ex_vec2f_t* _v )
 @param _v the in vector2
 @return if the inverse operation successed 
 - true: successful
 - false: failed
 @details check if the in vector2 is normalized 
 @sa ex_vec2f_is_normalized
 @sa ex_vec2f_get_normalize
*/// ------------------------------------------------------------------ 

inline bool ex_vec2f_normalize ( ex_vec2f_t* _v ) {
    float length_sqr = ex_vec2f_lenSQR(_v);
    float inv_length = 0.0f; 

    if ( ex_is_equal_float( length_sqr, 1.0f, EX_FLOAT_EPS ) )
        return true;

    if( ex_is_zero_float( length_sqr, EX_FLOAT_EPS ) )
        return false;

    inv_length = ex_inv_sqrtf( length_sqr );
    ex_vec2f_mul_scalar ( _v, _v, inv_length );
}

// ------------------------------------------------------------------ 
/*! 
 @fn bool ex_vec2f_get_normalize ( ex_vec2f_t* _r, ex_vec2f_t* _v )
 @retval _r the result vector2
 @param _v the in vector2
 @return if the inverse operation successed 
 - true: successful
 - false: failed
 @details get the normalized vector2 from _r, set and return it to vector2 _v
 @sa ex_vec2f_is_normalized
 @sa ex_vec2f_normalize
*/// ------------------------------------------------------------------ 

inline bool ex_vec2f_get_normalize ( ex_vec2f_t* _r, ex_vec2f_t* _v ) {
    float length_sqr, inv_length;

    ex_assert ( _r != _v, "can't use self as return value." );
    length_sqr = ex_vec2f_lenSQR(_v);
    inv_length = 0.0f; 

    if ( ex_is_equal_float( length_sqr, 1.0f, EX_FLOAT_EPS ) ) {
        return true;
    }

    if( ex_is_zero_float( length_sqr, EX_FLOAT_EPS ) ) {
        return false;
    }

    inv_length = ex_inv_sqrtf( length_sqr );
    ex_vec2f_mul_scalar ( _r, _v, inv_length );
}

//! @}

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END VEC2F_H_1289444900
// #################################################################################
