// ======================================================================================
// File         : texture2d.c
// Author       : Wu Jie 
// Last Change  : 07/14/2011 | 00:14:45 AM | Thursday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "texture2d.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
extern void __texture_init ( ex_ref_t * );
// ------------------------------------------------------------------ 

void __texture2d_init ( ex_ref_t *_self ) {
    __texture_init(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __texture_deinit ( ex_ref_t * );
// ------------------------------------------------------------------ 

void __texture2d_deinit ( ex_ref_t *_self ) {
    ex_texture2d_t *self;

    self = EX_REF_CAST(ex_texture2d_t,_self);

    __texture_deinit(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EX_DEF_CLASS_BEGIN(ex_texture2d_t)

    EX_TEXTURE2D_DEFAULT_MEMBER

    EX_MEMBER( ex_object_t, name, ex_strid("Texture2D") )
    EX_MEMBER( ex_object_t, init, __texture2d_init )
    EX_MEMBER( ex_object_t, deinit, __texture2d_deinit )

EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_texture2d_t)
    EX_PROP( ex_texture2d_t, int, mipmapCount, "Mipmap Count", EX_PROP_ATTR_NONE )
    EX_PROP( ex_texture2d_t, int, format, "Format", EX_PROP_ATTR_NONE )
EX_DEF_PROPS_END

EX_SERIALIZE_SUPER_BEGIN(ex_texture2d_t,ex_texture_t)
    EX_MEMBER_SERIALIZE( int, mipmapCount )
    EX_MEMBER_SERIALIZE( int, format )
EX_SERIALIZE_END

EX_DEF_TOSTRING_SUPER_BEGIN(ex_texture2d_t,ex_texture_t)
    EX_MEMBER_TOSTRING( int, "mipmapCount", self->mipmapCount )
    EX_MEMBER_TOSTRING( int, "format", self->format )
EX_DEF_TOSTRING_END
