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
    ex_behavior_t *self;

    self = EX_REF_CAST(ex_behavior_t,_self);

    __component_deinit(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EX_DEF_CLASS_BEGIN(ex_behavior_t)

    EX_BEHAVIOR_DEFAULT_MEMBER

    EX_MEMBER( ex_object_t, name, ex_strid("Behavior") )
    EX_MEMBER( ex_object_t, init, __behavior_init )
    EX_MEMBER( ex_object_t, deinit, __behavior_deinit )

EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_behavior_t)
    EX_PROP( ex_behavior_t, bool, enabled, "enabled",  EX_PROP_ATTR_HIDE )
EX_DEF_PROPS_END

EX_SERIALIZE_SUPER_BEGIN(ex_behavior_t,ex_component_t)
    EX_MEMBER_SERIALIZE( bool, enabled )
EX_SERIALIZE_END

EX_DEF_TOSTRING_SUPER_BEGIN(ex_behavior_t,ex_component_t)
    EX_MEMBER_TOSTRING ( bool, "enabled", self->enabled )
EX_DEF_TOSTRING_END
