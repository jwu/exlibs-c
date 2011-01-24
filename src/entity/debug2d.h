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

#include "component.h"

///////////////////////////////////////////////////////////////////////////////
// struct
///////////////////////////////////////////////////////////////////////////////

#define EX_DEBUG_SHAPE_RECT     0
#define EX_DEBUG_SHAPE_CIRCLE   1

// ------------------------------------------------------------------ 
/*! 
 @struct ex_debug2d_t
 @details
*/// ------------------------------------------------------------------ 

EX_DECL_CLASS_SUPER_BEGIN(ex_debug2d_t,ex_component_t)
    int shapeType;
    ex_rectf_t rect;
    ex_circlef_t circle;
EX_DECL_CLASS_SUPER_END(ex_debug2d_t,ex_component_t)

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_debug2d_init ( void *_self ); 
extern void ex_debug2d_deinit ( void *_self ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_debug2d_set_rect ( ex_debug2d_t *_self, 
                                  float _x, float _y, float _width, float _height ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_debug2d_draw ( ex_debug2d_t *_self ); 

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END DEBUG2D_H_1294476144
// #################################################################################
