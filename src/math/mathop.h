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

// ------------------------------------------------------------------ 
// Desc: round 
//  round to nearest
//  23.67 => 24, 23.35 => 23, -23.35 => -23, -23.67 => -24 
// ------------------------------------------------------------------ 

// float 
inline float roundf ( float _value ) { return _value > 0.0f ? (_value + 0.5f) : (_value - 0.5f); }

// double 
inline double round ( double _value ) { return _value > 0.0 ? (_value + 0.5) : (_value - 0.5); }

// ------------------------------------------------------------------ 
// Desc: trunc 
//  round towards zero
//  23.67 => 23, 23.35 => 23, -23.35 => -23, -23.67 => -23 
// ------------------------------------------------------------------ 

// float 
inline float truncf ( float _value ) { return _value > 0.0f ? floorf(_value) : ceilf(_value); }

// double 
inline double trunc ( double _value ) { return _value > 0.0 ? floor(_value) : ceil(_value); }

// ------------------------------------------------------------------ 
// Desc: roundaway 
//  round away zero
//  23.67 => 24, 23.35 => 24, -23.35 => -24, -23.67 => -24 
// ------------------------------------------------------------------ 

// float 
inline float roundawayf ( float _value ) { return _value > 0.0f ? ceilf(_value) : floorf(_value); }

// double 
inline double roundaway ( double _value ) { return _value > 0.0 ? ceil(_value) : floor(_value); }

// ------------------------------------------------------------------ 
// Desc: log2
// ------------------------------------------------------------------ 

// float 
inline float log2f ( float _value ) { return logf(_value)/logf(2.0f); }

// double 
inline double log2 ( double _value ) { return log(_value)/log(2.0); }

// ------------------------------------------------------------------ 
// Desc: ceilpow2
// ------------------------------------------------------------------ 

// uint32
inline uint32 ceilpow2u ( uint32 _value ) {
    uint32 result = 1;
    while ( result < _value ) {
        result <<= 1; 
    }
    return result;
}

// float
inline uint32 ceilpow2f ( float _value ) {
    return ceilpow2u ( (uint32)ceilf(_value) );
}

// double
inline uint32 ceilpow2 ( double _value ) {
    return ceilpow2u ( (uint32)ceil(_value) );
}

// ------------------------------------------------------------------ 
// Desc: floorpow2 
// ------------------------------------------------------------------ 

// uint32
inline uint32 floorpow2u ( uint32 _value ) {
    uint32 result = 1;
    while ( result < _value )
    {
        result <<= 1; 
    }
    result >>= 1;
    return result;
}

// float
inline uint32 floorpow2f ( float _value ) {
    return floorpow2u ( (uint32)floorf(_value) );
}

// double
inline uint32 floorpow2 ( double _value ) {
    return floorpow2u ( (uint32)floor(_value) );
}

// #################################################################################
#endif // END MATHOP_H_1277686663
// #################################################################################


