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

void __component_init ( void *_self ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void __component_deinit ( void *_self ) {
    ex_component_t *self = (ex_component_t *)_self;
    self->owner = NULL;
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
