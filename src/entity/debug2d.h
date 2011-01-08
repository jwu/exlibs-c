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

// ------------------------------------------------------------------ 
/*! 
 @struct ex_debug2d_t
 @details
*/// ------------------------------------------------------------------ 

EX_DEF_CLASS_SUPER_BEGIN(ex_debug2d_t,ex_component_t)
    // ex_rect_t _rect;
EX_DEF_CLASS_END(ex_debug2d_t)

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_debug2d_init ( void* _self ); 
extern void ex_debug2d_deinit ( void* _self ); 

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END DEBUG2D_H_1294476144
// #################################################################################
