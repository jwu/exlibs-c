// ======================================================================================
// File         : sprite.c
// Author       : Wu Jie 
// Last Change  : 01/26/2011 | 19:42:37 PM | Wednesday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "sprite.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
extern void __component_init ( ex_ref_t * );
// ------------------------------------------------------------------ 

void __sprite_init ( ex_ref_t *_self ) {
    // ex_sprite_t *sprite = (ex_sprite_t *)_self; 

    __component_init(_self);

    // TODO: sprite->pos = ex_vec2f_zero;
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __component_deinit ( ex_ref_t * );
// ------------------------------------------------------------------ 

void __sprite_deinit ( ex_ref_t *_self ) {
    // ex_sprite_t *sprite = (ex_sprite_t *)_self; 

    __component_deinit(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EX_DEF_CLASS_BEGIN(ex_sprite_t)

    EX_COMPONENT_DEFAULT_MEMBER

    EX_MEMBER( ex_object_t, name, ex_strid("Sprite") )
    EX_MEMBER( ex_object_t, init, __sprite_init )
    EX_MEMBER( ex_object_t, deinit, __sprite_deinit )

EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_sprite_t)
    // EX_PROP( ex_sprite_t, pos, "position",  EX_PROP_ATTR_NONE, ex_prop_set_raw_vec2f, ex_prop_get_raw_vec2f )
EX_DEF_PROPS_END

EX_SERIALIZE_SUPER_BEGIN(ex_sprite_t,ex_component_t)
    // EX_MEMBER_SERIALIZE( vec2f, pos )
EX_SERIALIZE_END

EX_DEF_TOSTRING_SUPER_BEGIN(ex_sprite_t,ex_component_t)
    // EX_MEMBER_TOSTRING ( vec2f, "position", self->pos )
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// NOTE: I think if the material, textures are the same, they can batch into one draw call
// glDrawArrays should count one draw call.
