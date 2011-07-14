// ======================================================================================
// File         : misc.c
// Author       : Wu Jie 
// Last Change  : 07/14/2011 | 09:25:17 AM | Thursday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "macro.h"
#include "misc.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_texture_fmt_to_bytes ( int _fmt ) {
    switch ( _fmt ) {
    case EX_TEXTURE_FORMAT_ALPHA8:
        return 1;

    case EX_TEXTURE_FORMAT_ARGB4444:
        return 3;

    case EX_TEXTURE_FORMAT_RGB24:
        return 3;

    case EX_TEXTURE_FORMAT_ARGB32:
        return 4;

    case EX_TEXTURE_FORMAT_RGB565:
        return 2;

    // TODO: I'm not sure { 
    case EX_TEXTURE_FORMAT_DXT1:
        return 4;

    case EX_TEXTURE_FORMAT_DXT5:
        return 4;

    case EX_TEXTURE_FORMAT_PVRTC_RGB2:
        return 1;

    case EX_TEXTURE_FORMAT_PVRTC_RGBA2:
        return 1;

    case EX_TEXTURE_FORMAT_PVRTC_RGB4:
        return 3;

    case EX_TEXTURE_FORMAT_PVRTC_RGBA4:
        return 3;

    case EX_TEXTURE_FORMAT_ETC_RGB4:
        return 3;
    // } TODO end 

    default:
        return 4;
    }
}

