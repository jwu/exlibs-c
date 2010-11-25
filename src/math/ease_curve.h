// ======================================================================================
// File         : ease_curve.h
// Author       : Wu Jie 
// Last Change  : 11/25/2010 | 10:39:44 AM | Thursday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef EASE_CURVE_H_1290652786
#define EASE_CURVE_H_1290652786
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

// ------------------------------------------------------------------ 
// linear
// ------------------------------------------------------------------ 

inline float ex_ease_linearf ( float _t ) { return _t; }
inline double ex_ease_linear ( double _t ) { return _t; }

// ------------------------------------------------------------------ 
// quad
//  Easing equation function for a quadratic (_t^2)
//  @param _t: Current time (in frames or seconds).
//  @return: The correct value.
// ------------------------------------------------------------------ 

float ex_ease_in_quadf ( float _t );
double ex_ease_in_quad ( double _t );

float ex_ease_out_quadf ( float _t );
double ex_ease_out_quad ( double _t );

float ex_ease_inout_quadf ( float _t ); 
double ex_ease_inout_quad ( double _t ); 

float ex_ease_outin_quadf ( float _t ); 
double ex_ease_outin_quad ( double _t ); 

// ------------------------------------------------------------------ 
// cubic 
//  Easing equation function for a cubic (_t^3)
//  @param _t: Current time (in frames or seconds).
//  @return: The correct value.
// ------------------------------------------------------------------ 

float ex_ease_in_cubicf ( float _t );
double ex_ease_in_cubic ( double _t );

float ex_ease_out_cubicf ( float _t );
double ex_ease_out_cubic ( double _t );

float ex_ease_inout_cubicf ( float _t ); 
double ex_ease_inout_cubic ( double _t ); 

float ex_ease_outin_cubicf ( float _t ); 
double ex_ease_outin_cubic ( double _t ); 

// ------------------------------------------------------------------ 
// quart 
//  Easing equation function for a quartic (_t^4)
//  @param _t: Current time (in frames or seconds).
//  @return: The correct value.
// ------------------------------------------------------------------ 

float ex_ease_in_quartf ( float _t );
double ex_ease_in_quart ( double _t );

float ex_ease_out_quartf ( float _t );
double ex_ease_out_quart ( double _t );

float ex_ease_inout_quartf ( float _t ); 
double ex_ease_inout_quart ( double _t ); 

float ex_ease_outin_quartf ( float _t ); 
double ex_ease_outin_quart ( double _t ); 

// ------------------------------------------------------------------ 
// quint
//  Easing equation function for a quintic (_t^5)
//  @param _t: Current time (in frames or seconds).
//  @return: The correct value.
// ------------------------------------------------------------------ 

float ex_ease_in_quintf ( float _t );
double ex_ease_in_quint ( double _t );

float ex_ease_out_quintf ( float _t );
double ex_ease_out_quint ( double _t );

float ex_ease_inout_quintf ( float _t );
double ex_ease_inout_quint ( double _t );

float ex_ease_outin_quintf ( float _t );
double ex_ease_outin_quint ( double _t );

// ------------------------------------------------------------------ 
// sine
//  Easing equation function for a sinusoidal (sin(_t))
//  @param _t: Current time (in frames or seconds).
//  @return: The correct value.
// ------------------------------------------------------------------ 

float ex_ease_in_sinef ( float _t );
double ex_ease_in_sine ( double _t );

float ex_ease_out_sinef ( float _t );
double ex_ease_out_sine ( double _t );

float ex_ease_inout_sinef ( float _t );
double ex_ease_inout_sine ( double _t );

float ex_ease_outin_sinef ( float _t ); 
double ex_ease_outin_sine ( double _t ); 

// ------------------------------------------------------------------ 
// class expo 
//  Easing equation function for an exponential (2^_t)
//  param _t: Current time (in frames or seconds).
//  return: The correct value.
// ------------------------------------------------------------------ 

float ex_ease_in_expof ( float _t );
double ex_ease_in_expo ( double _t );

float ex_ease_out_expof ( float _t );
double ex_ease_out_expo ( double _t );

float ex_ease_inout_expof ( float _t );
double ex_ease_inout_expo ( double _t );

float ex_ease_outin_expof ( float _t );
double ex_ease_outin_expo ( double _t );

// ------------------------------------------------------------------ 
// circ 
//  Easing equation function for a circular (sqrt(1-_t^2))
//  @param _t: Current time (in frames or seconds).
//  @return:	The correct value.
// ------------------------------------------------------------------ 

float ex_ease_in_circf ( float _t );
double ex_ease_in_circ ( double _t );

float ex_ease_out_circf ( float _t );
double ex_ease_out_circ ( double _t );

float ex_ease_inout_circf ( float _t ); 
double ex_ease_inout_circ ( double _t ); 

float ex_ease_outin_circf ( float _t );
double ex_ease_outin_circ ( double _t );

// ------------------------------------------------------------------ 
// elastic 
//  Easing equation function for an elastic (exponentially decaying sine wave)
//  @param _t: Current time (in frames or seconds).
//  @param a: Amplitude.
//  @param p: Period.
//  @return: The correct value.
//  recommand value: elastic (t, 0.1, 0.05)
// ------------------------------------------------------------------ 

float ex_ease_in_elasticf ( float _t, float _a, float _p );
double ex_ease_in_elastic ( double _t, double _a, double _p );

float ex_ease_out_elasticf ( float _t, float _a, float _p );
double ex_ease_out_elastic ( double _t, double _a, double _p );

float ex_ease_inout_elasticf ( float _t, float _a, float _p ); 
double ex_ease_inout_elastic ( double _t, double _a, double _p ); 

float ex_ease_outin_elasticf ( float _t, float _a, float _p ); 
double ex_ease_outin_elastic ( double _t, double _a, double _p ); 

// ------------------------------------------------------------------ 
// back 
//  Easing equation function for a back (overshooting cubic easing: (s+1)*_t^3 - s*_t^2)
//  @param _t: Current time (in frames or seconds).
//  @param s: Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
//  @return: The correct value.
// ------------------------------------------------------------------ 

float ex_ease_in_backf ( float _t, float _s );
double ex_ease_in_back ( double _t, double _s );

float ex_ease_out_backf ( float _t, float _s );
double ex_ease_out_back ( double _t, double _s );

float ex_ease_inout_backf ( float _t, float _s ); 
double ex_ease_inout_back ( double _t, double _s ); 

float ex_ease_outin_backf ( float _t, float _s ); 
double ex_ease_outin_back ( double _t, double _s ); 

// ------------------------------------------------------------------ 
// bounce 
//  Easing equation function for a bounce (exponentially decaying parabolic bounce)
//  @param _t: Current time (in frames or seconds).
//  @param a: Amplitude.
//  @return: The correct value.
// ------------------------------------------------------------------ 

float ex_ease_out_bouncef ( float _t, float _a );
double ex_ease_out_bounce ( double _t, double _a );

float ex_ease_in_bouncef ( float _t, float _a );
double ex_ease_in_bounce ( double _t, double _a );

float ex_ease_inout_bouncef ( float _t, float _a ); 
double ex_ease_inout_bounce ( double _t, double _a ); 

float ex_ease_outin_bouncef ( float _t, float _a ); 
double ex_ease_outin_bounce ( double _t, double _a ); 

// ------------------------------------------------------------------ 
// smooth 
// _t<=0: 0 | 0<_t<1: 3*_t^2 - 2*t^3 | _t>=1: 1
// ------------------------------------------------------------------ 

float ex_ease_smoothf ( float _t );
double ex_ease_smooth ( double _t );

// ------------------------------------------------------------------ 
// fade 
// _t<=0: 0 | 0<_t<1: 6*_t^5 - 15*_t^4 + 10*_t^3 | _t>=1: 1
// ------------------------------------------------------------------ 

float ex_ease_fadef ( float _t );
double ex_ease_fade ( double _t );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// SmoothBegin blends Smooth and Linear Interpolation.
// Progress 0 - 0.3      : Smooth only
// Progress 0.3 - ~ 0.5  : Mix of Smooth and Linear
// Progress ~ 0.5  - 1   : Linear only

// Easing function that starts growing slowly, then increases in speed. At the end of the curve the speed will be constant.
function ex_ease_in_curve (_t)
    local sinProgress = sin_progress(_t)
    local mix = smooth_begin_end_mix_factor(_t)
    return sinProgress * mix + _t * (1 - mix)
end

// Easing function that starts growing steadily, then ends slowly. The speed will be constant at the beginning of the curve.
function ex_ease_out_curve (_t)
    local sinProgress = sin_progress(_t)
    local mix = smooth_begin_end_mix_factor(1 - _t)
    return sinProgress * mix + _t * (1 - mix)
end

// Easing function where the value grows sinusoidally. Note that the calculated  end value will be 0 rather than 1.
function curve_sine (_t) 
    return (sin(((_t * two_pi)) - half_pi) + 1) / 2 
end

// Easing function where the value grows cosinusoidally. Note that the calculated start value will be 0.5 and the end value will be 0.5
// contrary to the usual 0 to 1 easing curve.
function curve_cosine (_t) 
    return (cos(((_t * two_pi)) - half_pi) + 1) / 2 
end

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 


// #################################################################################
#endif // END EASE_CURVE_H_1290652786
// #################################################################################


