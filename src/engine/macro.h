// ======================================================================================
// File         : macro.h
// Author       : Wu Jie 
// Last Change  : 07/14/2011 | 00:00:13 AM | Thursday,July
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef MACRO_H_1310572815
#define MACRO_H_1310572815
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: Filter Mode 
// ------------------------------------------------------------------ 

#define EX_FILTER_MODE_POINT        0
#define EX_FILTER_MODE_BILINEAR     1
#define EX_FILTER_MODE_TRILINEAR    2

// ------------------------------------------------------------------ 
// Desc: Texture Wrap Mode
// ------------------------------------------------------------------ 

#define EX_TEXTURE_WRAP_MODE_REPEAT    0
#define EX_TEXTURE_WRAP_MODE_CLAMP     1

// ------------------------------------------------------------------ 
// Desc: Texture Format 
// ------------------------------------------------------------------ 

#define EX_TEXTURE_FORMAT_ALPHA8        0
#define EX_TEXTURE_FORMAT_ARGB4444      1
#define EX_TEXTURE_FORMAT_RGB24         2
#define EX_TEXTURE_FORMAT_ARGB32        3
#define EX_TEXTURE_FORMAT_RGB565        4
#define EX_TEXTURE_FORMAT_DXT1          5
#define EX_TEXTURE_FORMAT_DXT5          6
#define EX_TEXTURE_FORMAT_PVRTC_RGB2    7
#define EX_TEXTURE_FORMAT_PVRTC_RGBA2   8
#define EX_TEXTURE_FORMAT_PVRTC_RGB4    9
#define EX_TEXTURE_FORMAT_PVRTC_RGBA4   10
#define EX_TEXTURE_FORMAT_ETC_RGB4      11


// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END MACRO_H_1310572815
// #################################################################################
