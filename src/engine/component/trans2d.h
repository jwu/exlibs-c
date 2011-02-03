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

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "component.h"

///////////////////////////////////////////////////////////////////////////////
// struct
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
/*! 
 @struct ex_trans2d_t
 @details
*/// ------------------------------------------------------------------ 

EX_DECL_CLASS_SUPER_BEGIN(ex_trans2d_t,ex_component_t)
    ex_vec2f_t  pos;   // local position
    ex_angf_t   ang;   // local angle
    ex_vec2f_t  scale; // local scale
    ex_ref_t   *parent; // ex_trans2d_t *
    ex_array_t *children; // ex_trans2d_t * [] 

    bool dirty;
    ex_mat33f_t localToWorld;
    ex_mat33f_t worldToLocal; // TODO: I do remember Unreal have an easy worldToLocal algorithm 
EX_DECL_CLASS_SUPER_END(ex_trans2d_t,ex_component_t)

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_trans2d_set_parent ( ex_ref_t *_self, ex_ref_t *_parent ); 
extern void ex_trans2d_detach_children ( ex_ref_t *_self ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_trans2d_world_position ( ex_ref_t *_self, ex_vec2f_t *_pos ); 
extern void ex_trans2d_world_rotation ( ex_ref_t *_self, ex_angf_t *_ang ); 
extern void ex_trans2d_world_scale ( ex_ref_t *_self, ex_vec2f_t *_scale ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_trans2d_local_to_world_mat33f ( ex_ref_t *_self, ex_mat33f_t *_result ); 
extern void ex_trans2d_world_to_local_mat33f ( ex_ref_t *_self, ex_mat33f_t *_result ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_trans2d_set_local_position ( ex_ref_t *_self, float _x, float _y ); 
extern void ex_trans2d_set_local_rotation ( ex_ref_t *_self, float _radians ); 
extern void ex_trans2d_set_local_scale ( ex_ref_t *_self, float _x, float _y ); 


// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END TRANS2D_H_1291107310
// #################################################################################
