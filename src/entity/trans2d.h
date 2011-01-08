// ======================================================================================
// File         : trans2d.h
// Author       : Wu Jie 
// Last Change  : 11/30/2010 | 16:55:09 PM | Tuesday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef TRANS2D_H_1291107310
#define TRANS2D_H_1291107310
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

#include "component.h"

///////////////////////////////////////////////////////////////////////////////
// struct
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
/*! 
 @struct ex_trans2d_t
 @details
*/// ------------------------------------------------------------------ 

EX_DEF_CLASS_SUPER_BEGIN(ex_trans2d_t,ex_component_t)
    ex_vec2f_t  _pos;
    ex_angf_t   _ang;
    ex_vec2f_t  _scale;
    struct ex_trans2d_t* _parent;
    ex_array_t* _childrent;
EX_DEF_CLASS_END(ex_trans2d_t)

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_trans2d_init ( void* _self ); 
extern void ex_trans2d_deinit ( void* _self ); 

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END TRANS2D_H_1291107310
// #################################################################################
