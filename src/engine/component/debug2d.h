// ======================================================================================
// File         : debug2d.h
// Author       : Wu Jie 
// Last Change  : 01/08/2011 | 16:42:18 PM | Saturday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef DEBUG2D_H_1294476144
#define DEBUG2D_H_1294476144
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "behavior.h"

///////////////////////////////////////////////////////////////////////////////
// struct
///////////////////////////////////////////////////////////////////////////////

#define EX_DEBUG_SHAPE_RECT     0
#define EX_DEBUG_SHAPE_CIRCLE   1
#define EX_MAX_TRAIL_VERTS 16

// ------------------------------------------------------------------ 
/*! 
 @struct ex_debug2d_t
 @details
*/// ------------------------------------------------------------------ 

EX_DECL_CLASS_SUPER_BEGIN(ex_debug2d_t,ex_behavior_t)
    int shapeType;
    ex_rectf_t rect;
    ex_circlef_t circle;

    int trail_timer;
    int trail_idx; 
    ex_vec2f_t trails[EX_MAX_TRAIL_VERTS];
EX_DECL_CLASS_SUPER_END(ex_debug2d_t,ex_behavior_t)

#define EX_DEBUG2D_DEFAULT_MEMBER \
    EX_BEHAVIOR_DEFAULT_MEMBER \
    EX_MEMBER( ex_debug2d_t, shapeType, EX_DEBUG_SHAPE_RECT ) \
    EX_MEMBER_PTR_FUNC( ex_debug2d_t, rect, ex_rectf_set, ex_vec2f_zero, 1.0f, 1.0f ); \
    EX_MEMBER_PTR_FUNC( ex_debug2d_t, circle, ex_circlef_set, ex_vec2f_zero, 1.0f ); \
    EX_MEMBER( ex_debug2d_t, trail_idx, 0 ) \
    EX_MEMBER( ex_debug2d_t, trail_timer, -1 )

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_debug2d_set_rect ( ex_ref_t *_self, 
                                  float _x, float _y, float _width, float _height ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_debug2d_draw ( ex_ref_t *_self ); 

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END DEBUG2D_H_1294476144
// #################################################################################
