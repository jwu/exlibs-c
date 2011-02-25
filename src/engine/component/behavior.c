// ======================================================================================
// File         : behavior.c
// Author       : Wu Jie 
// Last Change  : 01/27/2011 | 11:49:24 AM | Thursday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "behavior.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
extern void __component_init ( ex_ref_t * );
// ------------------------------------------------------------------ 

void __behavior_init ( ex_ref_t *_self ) {
    __component_init(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __component_deinit ( ex_ref_t * );
// ------------------------------------------------------------------ 

void __behavior_deinit ( ex_ref_t *_self ) {
    __component_deinit(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EX_DEF_OBJECT_BEGIN( ex_behavior_t,
                     "Behavior",
                     __behavior_init,
                     __behavior_deinit )

    EX_MEMBER( ex_component_t, entity, NULL )
    EX_MEMBER( ex_component_t, enabled, true )

    EX_MEMBER( ex_behavior_t, state, EX_BEHAVIOR_STATE_NEW )
    EX_MEMBER( ex_behavior_t, level_start, NULL )
    EX_MEMBER( ex_behavior_t, start, NULL )
    EX_MEMBER( ex_behavior_t, update, NULL )
    EX_MEMBER( ex_behavior_t, post_update, NULL )

EX_DEF_OBJECT_END

EX_DEF_PROPS_BEGIN(ex_behavior_t)
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN_SUPER(ex_behavior_t,ex_component_t)
EX_SERIALIZE_END

EX_DEF_TOSTRING_SUPER_BEGIN(ex_behavior_t,ex_component_t)
EX_DEF_TOSTRING_END
