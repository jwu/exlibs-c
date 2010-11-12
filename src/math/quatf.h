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

///////////////////////////////////////////////////////////////////////////////
// defines 
///////////////////////////////////////////////////////////////////////////////

//! ------------------------------------------------------------------ 
//! \struct quatf_t
//! \brief Matrix 2x2
//! ------------------------------------------------------------------ 

typedef struct quatf_t {
    union {
        struct { 
            float x, y, z, w;
        }; // end struct
        float v[4];
    };
} quatf_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void quatf_zero ( quatf_t* _r ) { _r->x = 0.0f, _r->y = 0.0f, _r->z = 0.0f, _r->w = 0.0f; }
inline void quatf_identity ( quatf_t* _r ) { _r->x = 0.0f, _r->y = 0.0f, _r->z = 0.0f, _r->w = 1.0f; }

inline void quatf_set ( quatf_t* _r, float _x, float _y, float _z, float _w ) { _r->x = _x, _r->y = _y, _r->z = _z, _r->w = _w; }
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
// Desc: 
// ------------------------------------------------------------------ 

inline void quatf_neg ( quatf_t* _r ) { _r->x = -_r->x; _r->y = -_r->y; _r->z = -_r->z; _r->w = -_r->w; }
inline void quatf_get_neg ( quatf_t* _r, quatf_t* _q ) { 
    ex_assert ( _r != _q, "can't use self as return value." );
    _r->x = -_q->x; 
    _r->y = -_q->y; 
    _r->z = -_q->z; 
    _r->w = -_q->w; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void quatf_add ( quatf_t* _r, quatf_t* _lhs, quatf_t* _rhs ) { 
    _r->x = _lhs->x + _rhs->x; 
    _r->y = _lhs->y + _rhs->y; 
    _r->z = _lhs->z + _rhs->z; 
    _r->w = _lhs->w + _rhs->w; 
}
inline void quatf_add_scalar ( quatf_t* _r, quatf_t* _lhs, float _rhs ) {
    _r->x = _lhs->x + _rhs; 
    _r->y = _lhs->y + _rhs; 
    _r->z = _lhs->z + _rhs; 
    _r->w = _lhs->w + _rhs; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void quatf_sub ( quatf_t* _r, quatf_t* _lhs, quatf_t* _rhs ) { 
    _r->x = _lhs->x - _rhs->x; 
    _r->y = _lhs->y - _rhs->y; 
    _r->z = _lhs->z - _rhs->z; 
    _r->w = _lhs->w - _rhs->w; 
}
inline void quatf_sub_scalar ( quatf_t* _r, quatf_t* _lhs, float _rhs ) {
    _r->x = _lhs->x - _rhs; 
    _r->y = _lhs->y - _rhs; 
    _r->z = _lhs->z - _rhs; 
    _r->w = _lhs->w - _rhs; 
}
inline void scalar_sub_quatf ( quatf_t* _r, float _lhs, quatf_t* _rhs ) { 
    _r->x = _lhs - _rhs->x;
    _r->y = _lhs - _rhs->y;
    _r->z = _lhs - _rhs->z;
    _r->w = _lhs - _rhs->w;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void quatf_mul ( quatf_t* _r, quatf_t* _lhs, quatf_t* _rhs ) { 
    float x, y, z, w;

    x = _lhs->x * _rhs->w + _lhs->w * _rhs->x + _lhs->z * _rhs->y - _lhs->y * _rhs->z;
    y = _lhs->y * _rhs->w + _lhs->w * _rhs->y + _lhs->x * _rhs->z - _lhs->z * _rhs->x;
    z = _lhs->z * _rhs->w + _lhs->w * _rhs->z + _lhs->y * _rhs->x - _lhs->x * _rhs->y;
    w = _lhs->w * _rhs->w - _lhs->x * _rhs->x - _lhs->y * _rhs->y - _lhs->z * _rhs->z;

    quatf_set( _r, x, y, z, w );
}

inline void quatf_mul_scalar ( quatf_t* _r, quatf_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x * _rhs;
    _r->y = _lhs->y * _rhs;
    _r->z = _lhs->z * _rhs;
    _r->w = _lhs->w * _rhs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void scalar_div_quatf ( quatf_t* _r, float _lhs, quatf_t* _rhs ) { 
    _r->x = _lhs / _rhs->x;
    _r->y = _lhs / _rhs->y;
    _r->z = _lhs / _rhs->z;
    _r->w = _lhs / _rhs->w;
}

inline void quatf_div_scalar ( quatf_t* _r, quatf_t* _lhs, float _rhs ) { 
    _r->x = _lhs->x / _rhs;
    _r->y = _lhs->y / _rhs;
    _r->z = _lhs->z / _rhs;
    _r->w = _lhs->w / _rhs;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool quatf_inverse( quatf_t* _r );
bool quatf_get_inverse ( quatf_t* _r, quatf_t* _q );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline void quatf_conjugate( quatf_t* _r ) {
    _r->x = -_r->x;
    _r->y = -_r->y;
    _r->z = -_r->z;
    _r->w =  _r->w;
}
inline void quatf_get_conjugate ( quatf_t* _r, quatf_t* _q ) {
    _r->x = -_q->x;
    _r->y = -_q->y;
    _r->z = -_q->z;
    _r->w =  _q->w;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline float quatf_dot( quatf_t* _lhs, quatf_t* _rhs ) {
    return _lhs->x * _rhs->x 
         + _lhs->y * _rhs->y 
         + _lhs->z * _rhs->z 
         + _lhs->w * _rhs->w;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline float quatf_len( quatf_t* _r ) {
    return sqrtf ( quatf_dot(_r,_r) );
}

inline float quatf_lenSQR( quatf_t* _r ) {
    return quatf_dot(_r,_r);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline bool quatf_is_normalized( quatf_t* _r ) {
    return is_equal_float( quatf_lenSQR(_r), 1.0f, EX_FLOAT_EPS );
}

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
// Desc: 
// ------------------------------------------------------------------ 

void quatf_to_mat33( quatf_t* _q, mat33f_t* _m );
void quatf_to_mat44( quatf_t* _q, mat44f_t* _m );
void quatf_to_axis_xyz( quatf_t* _q, vec3f_t* _x, vec3f_t* _y, vec3f_t* _z );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void quatf_get_axis_x( vec3f_t* _r, quatf_t* _q );
void quatf_get_axis_y( vec3f_t* _r, quatf_t* _q );
void quatf_get_axis_z( vec3f_t* _r, quatf_t* _q );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END QUATF_H_1289552718
// #################################################################################
