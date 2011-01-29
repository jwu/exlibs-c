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
extern void __component_init ( void * );
// ------------------------------------------------------------------ 

void __sprite_init ( void *_self ) {
    // ex_sprite_t *sprite = (ex_sprite_t *)_self; 

    __component_init(_self);

    // TODO: sprite->pos = ex_vec2f_zero;
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __component_deinit ( void * );
// ------------------------------------------------------------------ 

void __sprite_deinit ( void *_self ) {
    // ex_sprite_t *sprite = (ex_sprite_t *)_self; 

    __component_deinit(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EX_DEF_OBJECT_BEGIN( ex_sprite_t,
                     "Sprite",
                     __sprite_init,
                     __sprite_deinit )

    EX_MEMBER( ex_component_t, owner, NULL )
    EX_MEMBER( ex_component_t, active, true )

EX_DEF_OBJECT_END

EX_DEF_PROPS_BEGIN(ex_sprite_t)
    // EX_PROP( ex_sprite_t, pos, "position",  EX_PROP_ATTR_NONE, ex_prop_set_raw_vec2f, ex_prop_get_raw_vec2f )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN_SUPER(ex_sprite_t,ex_component_t)
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
