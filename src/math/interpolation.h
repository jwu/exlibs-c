// ======================================================================================
// File         : interpolation.h
// Author       : Wu Jie 
// Last Change  : 11/24/2010 | 17:28:20 PM | Wednesday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef INTERPOLATION_H_1290591007
#define INTERPOLATION_H_1290591007
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

//! @defgroup Interpolation 
//! @ingroup Math
//! @{

// ------------------------------------------------------------------ 
/*! 
 @fn inline float ex_saturatef ( float _x )
 @param _x value for clamp
 @return the result will be clamp to [0,1]
 @sa ex_saturate

 @fn inline double ex_saturate ( double _x )
 @param _x value for clamp
 @return the result will be clamp to [0,1]
 @sa ex_saturatef
*/// ------------------------------------------------------------------ 

inline float ex_saturatef ( float _x ) {
    if (_x < 0.0f) return 0.0f;
    if (_x > 1.0f) return 1.0f;
    return _x;
}

inline double ex_saturatef ( double _x ) {
    if (_x < 0.0) return 0.0;
    if (_x > 1.0) return 1.0;
    return _x;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline float ex_clampf ( float _x, float _min, float _max )
 @param _x value for clamp
 @param _min the min bound for clamp
 @param _max the min bound for clamp
 @return the result will be clamp to [_min,_max]
 @sa ex_clamp

 @fn inline double ex_clamp ( double _x, double _min, double _max )
 @param _x value for clamp
 @param _min the min bound for clamp
 @param _max the min bound for clamp
 @return the result will be clamp to [_min,_max]
 @sa ex_clampf
*/// ------------------------------------------------------------------ 

inline float ex_clampf ( float _x, float _min, float _max ) {
    ex_assert ( _min < _max, "the _min must less than _max" );
    if (_x < _min) return _min;
    if (_x > _max) return _max;
    return _x;
}

inline double ex_clamp ( double _x, double _min, double _max ) {
    ex_assert ( _min < _max, "the _min must less than _max" );
    if (_x < _min) return _min;
    if (_x > _max) return _max;
    return _x;
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline float ex_lerpf ( float _from, float _to, float _ratio )
 @param _from source value
 @param _to dest value
 @param _ratio lerp ratio
 @return linear lerp the result between source and dest by ratio
 @sa ex_lerp

 @fn inline double ex_lerp ( double _from, double _to, double _ratio )
 @param _from source value
 @param _to dest value
 @param _ratio lerp ratio
 @return linear lerp the result between source and dest by ratio
 @sa ex_lerpf
*/// ------------------------------------------------------------------ 

inline float ex_lerpf ( float _from, float _to, float _ratio ) {
    return _from * (1.0f - _ratio) + _to * _ratio 
}

inline double ex_lerp ( double _from, double _to, double _ratio ) {
    return _from * (1.0 - _ratio) + _to * _ratio 
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline float ex_unit_remapf ( float _value, float _min, float _max )
 @param _value value between [min,max]
 @param _min min bound value
 @param _max max bound value
 @return remap the value between _min and _max to [0,1]
 @sa ex_unit_remap

 @fn inline double ex_unit_remap ( double _value, double _min, double _max )
 @param _value value between [min,max]
 @param _min min bound value
 @param _max max bound value
 @return remap the value between _min and _max to [0,1]
 @sa ex_unit_remapf
*/// ------------------------------------------------------------------ 

inline float ex_unit_remapf ( float _value, float _min, float _max ) {
    ex_assert ( _min < _max, "the _min value must less than _max" );
    ex_assert ( _value <= _max && _value >= _min, "_value must between [_min,_max]" );

    return (_value - _min) / (_max - _min);
}

inline double ex_unit_remap ( double _value, double _min, double _max ) {
    ex_assert ( _min < _max, "the _min value must less than _max" );
    ex_assert ( _value <= _max && _value >= _min, "_value must between [_min,_max]" );

    return (_value - _min) / (_max - _min);
}

// ------------------------------------------------------------------ 
/*! 
 @fn inline float ex_remapf ( float _value, float _from_min, float _from_max, float _to_min, float _to_max )
 @param _value value between [from_min,from_max]
 @param _from_min min bound value for source range
 @param _from_max max bound value for source range
 @param _to_min min bound value for dest range
 @param _to_max max bound value for dest range
 @return remap the value between _from_min and _from_max to [_to_min,_to_max]
 @sa ex_unit_remap

 @fn inline double ex_remap ( double _value, double _from_min, double _from_max, double _to_min, double _to_max )
 @param _value value between [from_min,from_max]
 @param _from_min min bound value for source range
 @param _from_max max bound value for source range
 @param _to_min min bound value for dest range
 @param _to_max max bound value for dest range
 @return remap the value between _from_min and _from_max to [_to_min,_to_max]
 @sa ex_unit_remapf
*/// ------------------------------------------------------------------ 

inline float ex_remapf ( float _value, 
                         float _from_min, float _from_max, 
                         float _to_min, float _to_max ) {
    ex_assert ( _from_min < _from_max, "the _from_min value must less than _from_max" );
    ex_assert ( _to_min < _to_max, "the _to_min value must less than _to_max" );
    ex_assert ( _value <= _from_max && _value >= _from_min, "_value must between [_from_min,_from_max]" );

    float ratio = (_value - _from_min) / (_from_max - _from_min);
    return ex_lerpf( _to_min, _to_max, ratio );
}

inline double ex_remap ( double _value, 
                         double _from_min, double _from_max, 
                         double _to_min, double _to_max ) {
    ex_assert ( _from_min < _from_max, "the _from_min value must less than _from_max" );
    ex_assert ( _to_min < _to_max, "the _to_min value must less than _to_max" );
    ex_assert ( _value <= _from_max && _value >= _from_min, "_value must between [_from_min,_from_max]" );

    double ratio = (_value - _from_min) / (_from_max - _from_min);
    return ex_lerp( _to_min, _to_max, ratio );
}

//! @}

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END INTERPOLATION_H_1290591007
// #################################################################################


