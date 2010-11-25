// ======================================================================================
// File         : mathop.h
// Author       : Wu Jie 
// Last Change  : 06/28/2010 | 08:57:40 AM | Monday,June
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef MATHOP_H_1277686663
#define MATHOP_H_1277686663
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

//! @ingroup Math
//! @{

// ------------------------------------------------------------------ 
/*! 
 @fn inline float ex_roundf ( float _value )
 @param _value the in value
 @return the round result
 @details round to nearest

 examples:

 @code
 float r;
 r = ex_roundf (23.67f);  // r = 24
 r = ex_roundf (23.35f);  // r = 23
 r = ex_roundf (-23.35f); // r = -23
 r = ex_roundf (-23.67f); // r = -24
 @endcode
 @sa ex_round
*/// ------------------------------------------------------------------ 

inline float ex_roundf ( float _value ) { return _value > 0.0f ? (_value + 0.5f) : (_value - 0.5f); }

// ------------------------------------------------------------------ 
/*! 
 @fn inline float ex_round ( double _value )
 @param _value the in value
 @return the round result
 @details round to nearest
 @sa ex_roundf
*/// ------------------------------------------------------------------ 

inline double ex_round ( double _value ) { return _value > 0.0 ? (_value + 0.5) : (_value - 0.5); }

// ------------------------------------------------------------------ 
/*! 
 @fn inline float ex_truncf ( float _value )
 @param _value the in value
 @return the round result
 @details round towards zero

 examples:

 @code
 float r;
 r = ex_truncf (23.67f);  // r = 23
 r = ex_truncf (23.35f);  // r = 23
 r = ex_truncf (-23.35f); // r = -23
 r = ex_truncf (-23.67f); // r = -23
 @endcode
 @sa ex_trunc
*/// ------------------------------------------------------------------ 

inline float ex_truncf ( float _value ) { return _value > 0.0f ? floorf(_value) : ceilf(_value); }

// ------------------------------------------------------------------ 
/*! 
 @fn inline double ex_trunc ( double _value )
 @param _value the in value
 @return the round result
 @details round towards zero
 @sa ex_truncf
*/// ------------------------------------------------------------------ 

inline double ex_trunc ( double _value ) { return _value > 0.0 ? floor(_value) : ceil(_value); }

// ------------------------------------------------------------------ 
/*! 
 @fn inline float ex_roundawayf ( float _value )
 @param _value the in value
 @return the round result
 @details round away zero

 examples:

 @code
 float r;
 r = ex_roundawayf (23.67f);  // r = 24
 r = ex_roundawayf (23.35f);  // r = 24
 r = ex_roundawayf (-23.35f); // r = -24
 r = ex_roundawayf (-23.67f); // r = -24
 @endcode
 @sa ex_roundaway
*/// ------------------------------------------------------------------ 

inline float ex_roundawayf ( float _value ) { return _value > 0.0f ? ceilf(_value) : floorf(_value); }

// ------------------------------------------------------------------ 
/*! 
 @fn inline double ex_roundaway ( double _value )
 @param _value the in value
 @return the round result
 @details round away zero
 @sa ex_roundawayf
*/// ------------------------------------------------------------------ 

inline double ex_roundaway ( double _value ) { return _value > 0.0 ? ceil(_value) : floor(_value); }

// ------------------------------------------------------------------ 
/*! 
 @fn inline float ex_log2f ( float _value )
 @param _value the in value
 @return the result
 @details log2 result
 @sa ex_log2
*/// ------------------------------------------------------------------ 

inline float ex_log2f ( float _value ) { return logf(_value)/logf(2.0f); }

// ------------------------------------------------------------------ 
/*! 
 @fn inline double ex_log2 ( double _value )
 @param _value the in value
 @return the result
 @details log2 result
 @sa ex_log2f
*/// ------------------------------------------------------------------ 

inline double ex_log2 ( double _value ) { return log(_value)/log(2.0); }

// ------------------------------------------------------------------ 
/*! 
 @fn inline uint32 ex_ceilpow2u ( uint32 _value )
 @param _value the in value
 @return the result
 @details get the nearest power of 2 larger than it.
 @code
 uint32 r;
 r = ex_ceilpow2u (15) // r = 16
 @endcode
 @sa ex_ceilpow2f
 @sa ex_ceilpow2
*/// ------------------------------------------------------------------ 

inline uint32 ex_ceilpow2u ( uint32 _value ) {
    uint32 result = 1;
    while ( result < _value ) {
        result <<= 1; 
    }
    return result;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline uint32 ex_ceilpow2f ( float _value )
 @param _value the in value
 @return the result
 @details get the nearest power of 2 larger than it.
 @sa ex_ceilpow2u
 @sa ex_ceilpow2
*/// ------------------------------------------------------------------ 

inline uint32 ex_ceilpow2f ( float _value ) {
    return ex_ceilpow2u ( (uint32)ceilf(_value) );
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline uint32 ex_ceilpow2 ( double _value )
 @param _value the in value
 @return the result
 @details get the nearest power of 2 larger than it.
 @sa ex_ceilpow2u
 @sa ex_ceilpow2f
*/// ------------------------------------------------------------------ 

inline uint32 ex_ceilpow2 ( double _value ) {
    return ex_ceilpow2u ( (uint32)ceil(_value) );
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline uint32 ex_floorpow2u ( uint32 _value )
 @param _value the in value
 @return the result
 @details get the nearest power of 2 smaller than it.
 @code
 uint32 r;
 r = ex_floorpow2u (15) // r = 8
 @endcode
 @sa ex_floorpow2f
 @sa ex_floorpow2
*/// ------------------------------------------------------------------ 

inline uint32 ex_floorpow2u ( uint32 _value ) {
    uint32 result = 1;
    while ( result < _value )
    {
        result <<= 1; 
    }
    result >>= 1;
    return result;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline uint32 ex_floorpow2f ( float _value )
 @param _value the in value
 @return the result
 @details get the nearest power of 2 smaller than it.
 @sa ex_floorpow2u
 @sa ex_floorpow2
*/// ------------------------------------------------------------------ 

inline uint32 ex_floorpow2f ( float _value ) {
    return ex_floorpow2u ( (uint32)floorf(_value) );
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline uint32 ex_floorpow2 ( double _value )
 @param _value the in value
 @return the result
 @details get the nearest power of 2 smaller than it.
 @sa ex_floorpow2u
 @sa ex_floorpow2f
*/// ------------------------------------------------------------------ 

inline uint32 ex_floorpow2 ( double _value ) {
    return ex_floorpow2u ( (uint32)floor(_value) );
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline float ex_inv_sqrtf ( float _val )
 @param _val the in value
 @return the result
 @details squre root the value, and return the inversed result
 @sa ex_inv_sqrt
*/// ------------------------------------------------------------------ 

inline float ex_inv_sqrtf ( float _val ) { return 1.0f / sqrtf(_val); }

// ------------------------------------------------------------------ 
/*! 
 @fn inline double ex_inv_sqrt ( double _val )
 @param _val the in value
 @return the result
 @details squre root the value, and return the inversed result
 @sa ex_inv_sqrtf
*/// ------------------------------------------------------------------ 

inline double ex_inv_sqrt ( double _val ) { return 1.0 / sqrt(_val); }

// ------------------------------------------------------------------ 
/*! 
 @fn inline bool ex_is_equalf ( float _lhs, float _rhs, float _eps )
 @param _lhs left hand side value
 @param _rhs right hand side value
 @param _eps the epsilon
 @return the result of the comparation
 @sa ex_is_equal
*/// ------------------------------------------------------------------ 

inline bool ex_is_equalf ( float _lhs, float _rhs, float _eps ) {
    return fabsf (_lhs-_rhs) < _eps;
} 

// ------------------------------------------------------------------ 
/*! 
 @fn inline bool ex_is_equal ( double _lhs, double _rhs, double _eps )
 @param _lhs left hand side value
 @param _rhs right hand side value
 @param _eps the epsilon
 @return the result of the comparation
 @sa ex_is_equalf
*/// ------------------------------------------------------------------ 

inline bool ex_is_equal ( double _lhs, double _rhs, double _eps ) {
    return fabs (_lhs-_rhs) < _eps;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline bool ex_is_zerof ( float _v, float _eps )
 @param _v value for comparation
 @param _eps the epsilon
 @return the result of the comparation
 @sa ex_is_zero
*/// ------------------------------------------------------------------ 

inline bool ex_is_zerof ( float _v, float _eps ) {
    return fabsf (_v) < _eps;
} 

// ------------------------------------------------------------------ 
/*! 
 @fn inline bool ex_is_zero ( double _v, double _eps )
 @param _v value for comparation
 @param _eps the epsilon
 @return the result of the comparation
 @sa ex_is_zerof
*/// ------------------------------------------------------------------ 

inline bool ex_is_zero ( double _v, double _eps ) {
    return fabs (_v) < _eps;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline int ex_signf( float _x )
 @param _x in value
 @return the sign value
 get the sign of the value

 @fn inline int ex_sign( double _x )
 @param _x in value
 @return the sign value
 get the sign of the value

 @fn inline int ex_signi( int _x )
 @param _x in value
 @return the sign value
 get the sign of the value
*/// ------------------------------------------------------------------ 

inline int ex_signf( float _x ) {
    if ( ex_is_zerof (_x, EX_FLOAT_EPS) ) return 0;
    if ( _x > 0.0f ) return 1;
    if ( _x < 0.0f ) return -1;
}

inline int ex_sign( double _x ) {
    if ( ex_is_zero (_x, EX_DOUBLE_EPS) ) return 0;
    if ( _x > 0.0 ) return 1;
    if ( _x < 0.0 ) return -1;
}

inline int ex_signi( int _x ) {
    if ( _x == 0 ) return 0;
    if ( _x > 0 ) return 1;
    if ( _x < 0 ) return -1;
}

//! @}

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END MATHOP_H_1277686663
// #################################################################################


