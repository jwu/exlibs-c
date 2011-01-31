// ======================================================================================
// File         : component.c
// Author       : Wu Jie 
// Last Change  : 11/25/2010 | 17:39:47 PM | Thursday,November
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "component.h"
#include "trans2d.h"

#include "../entity.h"

///////////////////////////////////////////////////////////////////////////////
// properties
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void __component_init ( ex_ref_t *_self ) {
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __entity_remove_comp( ex_ref_t *_ent, ex_ref_t *_comp );
// ------------------------------------------------------------------ 

void __component_deinit ( ex_ref_t *_self ) {
    ex_component_t *self = EX_REF_PTR(ex_component_t,_self);

    if ( self->owner->ptr ) {
        __entity_remove_comp( self->owner, _self );
        self->owner = NULL;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EX_DEF_OBJECT_BEGIN( ex_component_t,
                     "Component",
                     __component_init,
                     __component_deinit )

    EX_MEMBER( ex_component_t, owner, NULL )
    EX_MEMBER( ex_component_t, active, true )

EX_DEF_OBJECT_END

EX_DEF_PROPS_BEGIN(ex_component_t)
    EX_PROP( ex_component_t, bool, active, "active",  EX_PROP_ATTR_HIDE )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN_SUPER(ex_component_t,ex_object_t)
    EX_MEMBER_SERIALIZE( bool, active )
EX_SERIALIZE_END

EX_DEF_TOSTRING_SUPER_BEGIN(ex_component_t,ex_object_t)
    EX_MEMBER_TOSTRING ( bool, "active", self->active )
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////
