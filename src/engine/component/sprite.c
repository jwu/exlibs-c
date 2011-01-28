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

EX_DEF_CLASS_BEGIN(ex_sprite_t)
    // ex_component_t
    -1, // id
    NULL, // owner
    true, // active
    ex_sprite_init, // init
    ex_sprite_deinit, // deinit

    // ex_sprite_t
EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_sprite_t)
    // EX_PROP( ex_sprite_t, pos, "position",  EX_PROP_ATTR_NONE, ex_prop_set_raw_vec2f, ex_prop_get_raw_vec2f )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN_SUPER(ex_sprite_t,ex_component_t)
    // EX_MEMBER_SERIALIZE( vec2f, pos )
EX_SERIALIZE_END

EX_DEF_TOSTRING_BEGIN(ex_sprite_t)
    // EX_MEMBER_TOSTRING ( vec2f, "position", self->pos )
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_sprite_init ( void *_self ) {
    ex_sprite_t *sprite = (ex_sprite_t *)_self; 

    ex_component_init(_self); // parent init
    // TODO: sprite->pos = ex_vec2f_zero;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_sprite_deinit ( void *_self ) {
    ex_sprite_t *sprite = (ex_sprite_t *)_self; 

    ex_component_deinit(_self); // parent deinint
}

// NOTE: I think if the material, textures are the same, they can batch into one draw call
// glDrawArrays should count one draw call.
