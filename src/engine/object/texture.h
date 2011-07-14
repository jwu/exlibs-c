// ======================================================================================
// File         : texture.h
// Author       : Wu Jie 
// Last Change  : 07/13/2011 | 23:56:58 PM | Wednesday,July
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef TEXTURE_H_1310572619
#define TEXTURE_H_1310572619
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "../macro.h"
#include "../object.h"

///////////////////////////////////////////////////////////////////////////////
// struct
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
/*! 
 @struct ex_texture_t
 @details
*/// ------------------------------------------------------------------ 

EX_DECL_CLASS_SUPER_BEGIN(ex_texture_t,ex_object_t)
    int width;
    int height;
    int filterMode;
    int wrapMode;

    // private
    uint textureID; // opengl texture-id
EX_DECL_CLASS_SUPER_END(ex_texture_t,ex_object_t)

#define EX_TEXTURE_DEFAULT_MEMBER \
    EX_OBJECT_DEFAULT_MEMBER \
    EX_MEMBER( ex_texture_t, width, -1 ) \
    EX_MEMBER( ex_texture_t, height, -1 ) \
    EX_MEMBER( ex_texture_t, filterMode, EX_FILTER_MODE_POINT ) \
    EX_MEMBER( ex_texture_t, wrapMode, EX_TEXTURE_WRAP_MODE_REPEAT ) \
    EX_MEMBER( ex_texture_t, textureID, -1 )

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END TEXTURE_H_1310572619
// #################################################################################
