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
#include "mat33f.h"
#include "mat44f.h"
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

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void quatf_to_mat33( quatf_t* _q, ex_mat33f_t* _m ) {
    float Tx, Ty, Tz; 
    float Twx, Twy, Twz; 
    float Txx, Txy, Txz;
    float Tyy, Tyz, Tzz;

    Tx  = 2.0f * _q->x;
    Ty  = 2.0f * _q->y;
    Tz  = 2.0f * _q->z;
    Twx = Tx * _q->w;
    Twy = Ty * _q->w;
    Twz = Tz * _q->w;
    Txx = Tx * _q->x;
    Txy = Ty * _q->x;
    Txz = Tz * _q->x;
    Tyy = Ty * _q->y;
    Tyz = Tz * _q->y;
    Tzz = Tz * _q->z;

    ex_mat33f_set ( _m,  
                 1.0f - ( Tyy + Tzz ), Txy + Twz,            Txz - Twy, 
                 Txy - Twz,            1.0f - ( Txx + Tzz ), Tyz + Twx,
                 Txz + Twy,            Tyz - Twx,            1.0f - ( Txx + Tyy ) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void quatf_to_mat44( quatf_t* _q, ex_mat44f_t* _m ) {
    float Tx, Ty, Tz; 
    float Twx, Twy, Twz; 
    float Txx, Txy, Txz;
    float Tyy, Tyz, Tzz;

    Tx  = 2.0f * _q->x;
    Ty  = 2.0f * _q->y;
    Tz  = 2.0f * _q->z;
    Twx = Tx * _q->w;
    Twy = Ty * _q->w;
    Twz = Tz * _q->w;
    Txx = Tx * _q->x;
    Txy = Ty * _q->x;
    Txz = Tz * _q->x;
    Tyy = Ty * _q->y;
    Tyz = Tz * _q->y;
    Tzz = Tz * _q->z;

    ex_mat44f_set ( _m,  
                    1.0f - ( Tyy + Tzz ), Txy + Twz,            Txz - Twy,            0.0f,
                    Txy - Twz,            1.0f - ( Txx + Tzz ), Tyz + Twx,            0.0f,
                    Txz + Twy,            Tyz - Twx,            1.0f - ( Txx + Tyy ), 0.0f,  
                    0.0f,                 0.0f,                 0.0f,                 1.0f );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void quatf_get_axis_x( vec3f_t* _r, quatf_t* _q  ) {
    float Tx, Ty, Tz; 
    float Twx, Twy, Twz; 
    float Txx, Txy, Txz;
    float Tyy, Tyz, Tzz;

    //
    Tx  = 2.0f * _q->x;
    Ty  = 2.0f * _q->y;
    Tz  = 2.0f * _q->z;
    Twx = Tx * _q->w;
    Twy = Ty * _q->w;
    Twz = Tz * _q->w;
    Txx = Tx * _q->x;
    Txy = Ty * _q->x;
    Txz = Tz * _q->x;
    Tyy = Ty * _q->y;
    Tyz = Tz * _q->y;
    Tzz = Tz * _q->z;

    vec3f_set ( _r, 1.0f - ( Tyy + Tzz ), Txy + Twz, Txz - Twy );
}

// ------------------------------------------------------------------
// Desc: 
// ------------------------------------------------------------------

void quatf_get_axis_y( vec3f_t* _r, quatf_t* _q  ) {
    float Tx, Ty, Tz; 
    float Twx, Twy, Twz; 
    float Txx, Txy, Txz;
    float Tyy, Tyz, Tzz;

    //
    Tx  = 2.0f * _q->x;
    Ty  = 2.0f * _q->y;
    Tz  = 2.0f * _q->z;
    Twx = Tx * _q->w;
    Twy = Ty * _q->w;
    Twz = Tz * _q->w;
    Txx = Tx * _q->x;
    Txy = Ty * _q->x;
    Txz = Tz * _q->x;
    Tyy = Ty * _q->y;
    Tyz = Tz * _q->y;
    Tzz = Tz * _q->z;

    vec3f_set ( _r, Txy - Twz, 1.0f - ( Txx + Tzz ), Tyz + Twx );
}

// ------------------------------------------------------------------
// Desc: 
// ------------------------------------------------------------------

void quatf_get_axis_z( vec3f_t* _r, quatf_t* _q  ) {
    float Tx, Ty, Tz; 
    float Twx, Twy, Twz; 
    float Txx, Txy, Txz;
    float Tyy, Tyz, Tzz;

    //
    Tx  = 2.0f * _q->x;
    Ty  = 2.0f * _q->y;
    Tz  = 2.0f * _q->z;
    Twx = Tx * _q->w;
    Twy = Ty * _q->w;
    Twz = Tz * _q->w;
    Txx = Tx * _q->x;
    Txy = Ty * _q->x;
    Txz = Tz * _q->x;
    Tyy = Ty * _q->y;
    Tyz = Tz * _q->y;
    Tzz = Tz * _q->z;

    vec3f_set ( _r, Txz + Twy, Tyz - Twx, 1.0f - ( Txx + Tyy ) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void quatf_to_axis_xyz( quatf_t* _q, vec3f_t* _x, vec3f_t* _y, vec3f_t* _z ) {
    float Tx, Ty, Tz; 
    float Twx, Twy, Twz; 
    float Txx, Txy, Txz;
    float Tyy, Tyz, Tzz;

    //
    Tx  = 2.0f * _q->x;
    Ty  = 2.0f * _q->y;
    Tz  = 2.0f * _q->z;
    Twx = Tx * _q->w;
    Twy = Ty * _q->w;
    Twz = Tz * _q->w;
    Txx = Tx * _q->x;
    Txy = Ty * _q->x;
    Txz = Tz * _q->x;
    Tyy = Ty * _q->y;
    Tyz = Tz * _q->y;
    Tzz = Tz * _q->z;

    vec3f_set ( _x, 1.0f - ( Tyy + Tzz ), Txy + Twz, Txz - Twy );
    vec3f_set ( _y, Txy - Twz, 1.0f - ( Txx + Tzz ), Tyz + Twx );
    vec3f_set ( _z, Txz + Twy, Tyz - Twx, 1.0f - ( Txx + Tyy ) );
}
