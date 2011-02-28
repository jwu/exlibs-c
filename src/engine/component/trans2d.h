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
    ex_vec2f_t  local_pos;   // local position
    ex_angf_t   local_ang;   // local angle
    ex_vec2f_t  local_scale; // local scale
    ex_ref_t   *parent; // ex_trans2d_t *
    ex_array_t *children; // ex_trans2d_t * [] 

    ex_mutex_t *dirty_mutex; // NOTE: it is possible user use timer change things
    bool dirty;
    ex_mat33f_t localToWorld;
    ex_mat33f_t worldToLocal;
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

extern void ex_trans2d_local_position ( ex_ref_t *_self, ex_vec2f_t *_pos ); 
extern void ex_trans2d_local_rotation ( ex_ref_t *_self, ex_angf_t *_ang ); 
extern void ex_trans2d_local_scale ( ex_ref_t *_self, ex_vec2f_t *_scale ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_trans2d_right ( ex_ref_t *_self, ex_vec2f_t *_right ); 
extern void ex_trans2d_up ( ex_ref_t *_self, ex_vec2f_t *_up ); 

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

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_trans2d_set_world_position ( ex_ref_t *_self, float _x, float _y ); 
extern void ex_trans2d_set_world_rotation ( ex_ref_t *_self, float _radians ); 
extern void ex_trans2d_set_world_scale ( ex_ref_t *_self, float _x, float _y ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_trans2d_set_right ( ex_ref_t *_self, const ex_vec2f_t *_right ); 
extern void ex_trans2d_set_up ( ex_ref_t *_self, const ex_vec2f_t *_up ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_trans2d_transform_point ( ex_ref_t *_self, const ex_vec2f_t *_pos, ex_vec2f_t *_oPos ); 
extern void ex_trans2d_inverse_transform_point ( ex_ref_t *_self, const ex_vec2f_t *_pos, ex_vec2f_t *_oPos ); 

extern void ex_trans2d_transform_dir ( ex_ref_t *_self, const ex_vec2f_t *_dir, ex_vec2f_t *_oDir ); 
extern void ex_trans2d_inverse_transform_dir ( ex_ref_t *_self, const ex_vec2f_t *_dir, ex_vec2f_t *_oDir ); 

// ------------------------------------------------------------------ 
// Desc: 
#define EX_SPACE_LOCAL 0
#define EX_SPACE_WORLD 1
// ------------------------------------------------------------------ 

extern void ex_trans2d_translate ( ex_ref_t *_self, float _x, float _y, int _space ); 
extern void ex_trans2d_translate_relative_to ( ex_ref_t *_self, float _x, float _y, ex_ref_t *_transform ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_trans2d_rotate ( ex_ref_t *_self, float _radians ); 
extern void ex_trans2d_rotate_around ( ex_ref_t *_self, float _radians, const ex_vec2f_t *_worldpos ); 
extern void ex_trans2d_rotate_around_fix_orient ( ex_ref_t *_self, float _radians, const ex_vec2f_t *_worldpos ); 

// ------------------------------------------------------------------ 
// Desc: 
// NOTE: the name could be foo/bar/foobar 
// ------------------------------------------------------------------ 

extern ex_ref_t *ex_trans2d_find ( ex_ref_t *_self, const char *_name ); 

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END TRANS2D_H_1291107310
// #################################################################################
