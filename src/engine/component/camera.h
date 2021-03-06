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

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

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

EX_DECL_CLASS_SUPER_BEGIN(ex_camera_t,ex_component_t)
    bool isOrtho;
    float orthoSize; // camera's half-height when in orthographic mode.
    float aspect; // width/height
    uint32 clearFlags;
    ex_color3f_t bgColor; // background color
    ex_mat44f_t matWorldToView;
    ex_mat44f_t matProjection;
EX_DECL_CLASS_SUPER_END(ex_camera_t,ex_component_t)

#define EX_CAMERA_DEFAULT_MEMBER \
    EX_COMPONENT_DEFAULT_MEMBER \
    EX_MEMBER( ex_camera_t, isOrtho, false ) \
    EX_MEMBER( ex_camera_t, orthoSize, 600/2 ) \
    EX_MEMBER( ex_camera_t, aspect, 4.0f/3.0f ) \
    EX_MEMBER( ex_camera_t, clearFlags, EX_CLEAR_COLOR|EX_CLEAR_DEPTH ) \
    EX_MEMBER_PTR_FUNC ( ex_camera_t, bgColor, ex_color3f_set, 0.0f, 0.5f, 1.0f ); \
    EX_MEMBER( ex_camera_t, matWorldToView, ex_mat44f_identity ) \
    EX_MEMBER( ex_camera_t, matProjection, ex_mat44f_identity )

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_camera_set_ortho ( ex_ref_t *_self, bool _isOrtho ); 
extern bool ex_camera_is_ortho ( ex_ref_t *_self ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_camera_set_ortho_size ( ex_ref_t *_self, float _orthoSize ); 
extern float ex_camera_ortho_size ( ex_ref_t *_self ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_camera_set_aspect ( ex_ref_t *_self, float _aspect ); 
extern float ex_camera_aspect ( ex_ref_t *_self ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_camera_clear ( ex_ref_t *_self );
extern void ex_camera_apply_transform ( ex_ref_t *_self );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END CAMERA_H_1294540402
// #################################################################################
