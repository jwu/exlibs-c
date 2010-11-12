// ======================================================================================
// File         : quatf.c
// Author       : Wu Jie 
// Last Change  : 11/12/2010 | 17:05:42 PM | Friday,November
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "mathop.h"
#include "quatf.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool quatf_inverse( quatf_t* _r ) {
    float len_sqr;

    len_sqr = quatf_lenSQR(_r);
    if ( len_sqr > 0.0f ) {
        float inv_len_sqr = 1.0f/len_sqr;
        quatf_mul_scalar( _r, _r, inv_len_sqr );
        quatf_conjugate(_r);
        return true;
    }

    return false;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool quatf_get_inverse ( quatf_t* _r, quatf_t* _q ) {
    float len_sqr;

    ex_assert ( _r != _q, "can't use self as return value." );

    len_sqr = quatf_lenSQR(_q);
    if ( len_sqr > 0.0f ) {
        float inv_len_sqr = 1.0f/len_sqr;
        quatf_mul_scalar( _r, _q, inv_len_sqr );
        quatf_conjugate(_r);
        return true;
    }

    return false;
}

