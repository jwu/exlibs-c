// ======================================================================================
// File         : ease_curve_float.cpp
// Author       : Wu Jie 
// Last Change  : 11/25/2010 | 11:29:52 AM | Thursday,November
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static float _ease_in_elasticf_helper ( float _t, 
                                          float _b, 
                                          float _c, 
                                          float _d, 
                                          float _a, 
                                          float _p )
{
    // TODO
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static float _ease_out_elasticf_helper ( float _t, 
                                           float _b /*dummy*/, 
                                           float _c, 
                                           float _d /*dummy*/, 
                                           float _a, 
                                           float _p )
{
    // TODO
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static float _ease_out_bouncef_helper ( float _t, 
                                          float _c, 
                                          float _a )
{
    // TODO:
}

static float _sin_progress ( float _v ) {
    return sinf((_v * EX_PI) - EX_HALF_PI) / 2.0f + 0.5f;
}

static float _smooth_begin_end_mix_factor ( float _v ) { 
    return EX_MIN( EX_MAX( 1.0f - _v * 2.0f + 0.3f, 0.0f ), 1.0f );
}
