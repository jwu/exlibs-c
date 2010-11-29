// ======================================================================================
// File         : angf.h
// Author       : Wu Jie 
// Last Change  : 11/25/2010 | 17:41:11 PM | Thursday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef ANGF_H_1290678097
#define ANGF_H_1290678097
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// struct
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
/*!
 @struct ex_angf_t
 @details
 the angle is store in radians, the range is from [0,2pi]
*/// ------------------------------------------------------------------ 

typedef struct ex_angf_t {
    float rad;
} ex_angf_t;

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
/*!
 @fn inline void ex_angf_set_by_radians ( ex_angf_t* _a, float _radians ) 
 @param _a the in angle
 @param _radians the in radians
 @details
 set the angle by radians
*/// ------------------------------------------------------------------ 

inline void ex_angf_set_by_radians ( ex_angf_t* _a, float _radians ) { 
    if ( _a->rad >= 0.0f )
        _a->rad = fmodf(_radians,(float)EX_TWO_PI); 
    else
        _a->rad = (float)EX_TWO_PI + fmodf(_radians,(float)EX_TWO_PI); 
    ex_assert( !EX_IS_NAN(_a->rad), "error: NaN value" );
}

// ------------------------------------------------------------------ 
/*!
 @fn inline void ex_angf_set_by_degrees ( ex_angf_t* _a, float _degrees ) 
 @param _a the in angle
 @param _degrees the in degrees
 @details
 set the angle by degrees
*/// ------------------------------------------------------------------ 

inline void ex_angf_set_by_degrees ( ex_angf_t* _a, float _degrees ) {
    ex_angf_set_by_radians ( _a, ex_deg2rad_f(_degrees) );
} 

// ------------------------------------------------------------------ 
/*!
 @fn inline float ex_angf_to_degrees_180 ( ex_angf_t* _a ) 
 @param _a the in angle
 @return the result degrees
 @details
 convert angle to degrees in [-180,180]
*/// ------------------------------------------------------------------ 

inline float ex_angf_to_degrees_180 ( ex_angf_t* _a ) {
    float degrees = ex_rad2deg_f(_a->rad);
    return _a->rad >= (float)EX_PI ? degrees - 360.0f : degrees;
}

// ------------------------------------------------------------------ 
/*!
 @fn inline float ex_angf_to_degrees_360 ( ex_angf_t* _a ) 
 @param _a the in angle
 @return the result degrees
 @details
 convert angle to degrees in [0,360]
*/// ------------------------------------------------------------------ 

inline float ex_angf_to_degrees_360 ( ex_angf_t* _a ) { 
    return ex_rad2deg_f(_a->rad);
} 

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_angf_neg ( ex_angf_t* _a )
 @retval _a the result angle
 @param _a the in angle
 @details get the negtive value from angle _a, set and override it.
 @sa ex_angf_get_neg
*/// ------------------------------------------------------------------ 

inline void ex_angf_neg ( ex_angf_t* _a ) { 
    ex_angf_set_by_radians( _a, -_a->rad );
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_angf_get_neg ( ex_angf_t* _r, ex_angf_t* _a )
 @retval _r the result angle
 @param _v the in angle
 @details get the negtive value from angle _a, set and return it to angle _r
 @sa ex_angf_neg
*/// ------------------------------------------------------------------ 

inline void ex_angf_get_neg ( ex_angf_t* _r, ex_angf_t* _a ) { 
    ex_angf_set_by_radians( _r, -_a->rad );
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_angf_mul ( ex_angf_t* _r, ex_angf_t* _lhs, ex_angf_t* _rhs )
 @retval _r the result angle
 @param _lhs left hand side angle 
 @param _rhs right hand side angle 
 @details multiply two angles
*/// ------------------------------------------------------------------ 

inline void ex_angf_mul ( ex_angf_t* _r, ex_angf_t* _lhs, ex_angf_t* _rhs ) { 
    ex_angf_set_by_radians( _r, _lhs->rad * _rhs->rad );
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_angf_mul_scalar ( ex_angf_t* _r, ex_angf_t* _lhs, float _scalar )
 @retval _r the result angle
 @param _lhs left hand side angle 
 @param _rhs right hand scalar
 @details multiply angle with scalar
*/// ------------------------------------------------------------------ 

inline void ex_angf_mul_scalar ( ex_angf_t* _r, ex_angf_t* _lhs, float _scalar ) { 
    ex_angf_set_by_radians( _r, _lhs->rad * _scalar );
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_angf_div ( ex_angf_t* _r, ex_angf_t* _lhs, ex_angf_t* _rhs )
 @retval _r the result angle
 @param _lhs left hand side angle 
 @param _rhs right hand side angle 
 @details divide two angles
*/// ------------------------------------------------------------------ 

inline void ex_angf_div ( ex_angf_t* _r, ex_angf_t* _lhs, ex_angf_t* _rhs ) { 
    ex_angf_set_by_radians( _r, _lhs->rad / _rhs->rad );
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_angf_div_scalar ( ex_angf_t* _r, ex_angf_t* _lhs, float _scalar )
 @retval _r the result angle
 @param _lhs left hand side angle 
 @param _rhs right hand scalar
 @details divide angle with scalar
*/// ------------------------------------------------------------------ 

inline void ex_angf_div_scalar ( ex_angf_t* _r, ex_angf_t* _lhs, float _scalar ) { 
    ex_angf_set_by_radians( _r, _lhs->rad / _scalar );
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_angf_add ( ex_angf_t* _r, ex_angf_t* _lhs, ex_angf_t* _rhs )
 @retval _r the result angle
 @param _lhs left hand side angle 
 @param _rhs right hand side angle
 @details add two angles
*/// ------------------------------------------------------------------ 

inline void ex_angf_add ( ex_angf_t* _r, ex_angf_t* _lhs, ex_angf_t* _rhs ) { 
    ex_angf_set_by_radians( _r, _lhs->rad + _rhs->rad );
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline void ex_angf_sub ( ex_angf_t* _r, ex_angf_t* _lhs, ex_angf_t* _rhs )
 @retval _r the result angle
 @param _lhs left hand side angle 
 @param _rhs right hand side angle
 @details sub two angles
*/// ------------------------------------------------------------------ 

inline void ex_angf_sub ( ex_angf_t* _r, ex_angf_t* _lhs, ex_angf_t* _rhs ) { 
    ex_angf_set_by_radians( _r, _lhs->rad - _rhs->rad );
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline bool ex_angf_is_equal ( ex_angf_t* _lhs, ex_angf_t* _rhs )
 @param _lhs the left hand side angle
 @param _rhs the right hand side angle
 @return the result of the comparation
 @details compare if the two angles is equal to each other
*/// ------------------------------------------------------------------ 

inline bool ex_angf_is_equal ( ex_angf_t* _lhs, ex_angf_t* _rhs ) { 
    return ex_is_equalf(_lhs->rad,_rhs->rad,EX_FLOAT_EPS);
}

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 


// #################################################################################
#endif // END ANGF_H_1290678097
// #################################################################################

