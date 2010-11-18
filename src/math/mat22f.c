// ======================================================================================
// File         : ex_mat22f.c
// Author       : Wu Jie 
// Last Change  : 11/11/2010 | 14:23:21 PM | Thursday,November
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "mathop.h"
#include "vec2f.h"
#include "mat22f.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_mat22f_mul_vec2f ( vec2f_t* _r, ex_mat22f_t* _lhs, vec2f_t* _rhs ) {
    float x, y;
    vec2f_t tmp;

    ex_mat22f_row( &tmp, _lhs, 0 );
    x = vec2f_dot( _rhs, &tmp );
    ex_mat22f_row( &tmp, _lhs, 1 );
    y = vec2f_dot( _rhs, &tmp );

    vec2f_set( _r, x, y );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_vec2f_mul_ex_mat22f ( vec2f_t* _r, vec2f_t* _lhs, ex_mat22f_t* _rhs ) {
    float x, y;
    vec2f_t tmp;

    ex_mat22f_col( &tmp, _rhs, 0 );
    x = vec2f_dot( _lhs, &tmp );
    ex_mat22f_col( &tmp, _rhs, 1 );
    y = vec2f_dot( _lhs, &tmp );

    vec2f_set( _r, x, y );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_mat22f_inverse ( ex_mat22f_t* _m ) {
    float det, inv;
    float m00, m01, m10, m11;
    
    det = _m->m00 * _m->m11 - _m->m10 * _m->m01;
    if ( ex_is_zero_float( det, EX_FLOAT_EPS ) )
        return false;

    inv = 1.0f / det;
    m00 = _m->m11 * inv;
    m01 = _m->m01 * inv;
    m10 = _m->m10 * inv;
    m11 = _m->m00 * inv;

    ex_mat22f_set ( _m, 
                 m00, m01,
                 m10, m11 );
    return true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_mat22f_get_inverse ( ex_mat22f_t* _r, ex_mat22f_t* _m ) {
    float det, inv;

    ex_assert ( _r != _m, "can't use self as return value." );
    
    det = _m->m00 * _m->m11 - _m->m10 * _m->m01;
    if ( ex_is_zero_float( det, EX_FLOAT_EPS ) )
        return false;

    inv = 1.0f / det;
    _r->m00 = _m->m11 * inv;
    _r->m01 = _m->m01 * inv;
    _r->m10 = _m->m10 * inv;
    _r->m11 = _m->m00 * inv;

    return true;
}
