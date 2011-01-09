// ======================================================================================
// File         : camera.h
// Author       : Wu Jie 
// Last Change  : 01/09/2011 | 10:33:20 AM | Sunday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef CAMERA_H_1294540402
#define CAMERA_H_1294540402
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

#define EX_CLEAR_NONE  0x00000000
#define EX_CLEAR_COLOR 0x00000001
#define EX_CLEAR_DEPTH 0x00000002

// ------------------------------------------------------------------ 
/*! 
 @struct ex_camera_t
 @details
*/// ------------------------------------------------------------------ 

EX_DEF_CLASS_SUPER_BEGIN(ex_camera_t,ex_component_t)
    bool _isOrtho;
    float _orthoSize; // camera's half-height when in orthographic mode.
    uint32 _clearFlags;
    float _aspect; // width/height
    ex_color3f_t _bgColor; // background color
    ex_mat44f_t _matWorldToView;
    ex_mat44f_t _matProjection;
EX_DEF_CLASS_END(ex_camera_t)

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_camera_init ( void* _self ); 
extern void ex_camera_deinit ( void* _self ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_camera_set_ortho ( ex_camera_t* _self, bool _isOrtho ); 
extern bool ex_camera_is_ortho ( ex_camera_t* _self ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_camera_set_ortho_size ( ex_camera_t* _self, float _orthoSize ); 
extern float ex_camera_ortho_size ( ex_camera_t* _self ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_camera_set_aspect ( ex_camera_t* _self, float _aspect ); 
extern float ex_camera_aspect ( ex_camera_t* _self ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_camera_apply ( ex_camera_t* _self );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END CAMERA_H_1294540402
// #################################################################################
