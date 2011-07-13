// ======================================================================================
// File         : texture.c
// Author       : Wu Jie 
// Last Change  : 07/14/2011 | 00:05:00 AM | Thursday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "texture.h"

///////////////////////////////////////////////////////////////////////////////
// properties
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
extern void __object_init( ex_ref_t * );
// ------------------------------------------------------------------ 

void __texture_init ( ex_ref_t *_self ) {
    __object_init(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __entity_remove_comp( ex_ref_t *_ent, ex_ref_t *_comp );
extern void __object_deinit( ex_ref_t * );
// ------------------------------------------------------------------ 

void __texture_deinit ( ex_ref_t *_self ) {
    ex_texture_t *self = EX_REF_CAST(ex_texture_t,_self);

    // TODO: unload opengl textureID

    __object_deinit(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EX_DEF_CLASS_BEGIN(ex_texture_t)

    EX_TEXTURE_DEFAULT_MEMBER

    EX_MEMBER( ex_object_t, name, ex_strid("Texture") )
    EX_MEMBER( ex_object_t, init, __texture_init )
    EX_MEMBER( ex_object_t, deinit, __texture_deinit )

EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_texture_t)
    EX_PROP( ex_texture_t, int, width, "Width", EX_PROP_ATTR_NONE )
    EX_PROP( ex_texture_t, int, height, "Height", EX_PROP_ATTR_NONE )
    EX_PROP( ex_texture_t, int, filterMode, "Filter Mode", EX_PROP_ATTR_NONE )
    EX_PROP( ex_texture_t, int, wrapMode, "Wrap Mode", EX_PROP_ATTR_NONE )
EX_DEF_PROPS_END

EX_SERIALIZE_SUPER_BEGIN(ex_texture_t,ex_object_t)
    EX_MEMBER_SERIALIZE( int, width )
    EX_MEMBER_SERIALIZE( int, height )
    EX_MEMBER_SERIALIZE( int, filterMode )
    EX_MEMBER_SERIALIZE( int, wrapMode )
EX_SERIALIZE_END

EX_DEF_TOSTRING_SUPER_BEGIN(ex_texture_t,ex_object_t)
    EX_MEMBER_TOSTRING ( int, "width", self->width )
    EX_MEMBER_TOSTRING ( int, "height", self->height )
    EX_MEMBER_TOSTRING ( int, "filterMode", self->filterMode )
    EX_MEMBER_TOSTRING ( int, "wrapMode", self->wrapMode )
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////
