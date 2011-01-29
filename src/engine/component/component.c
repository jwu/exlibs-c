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

EX_DEF_CLASS_BEGIN(ex_component_t)

    // ======================================================== 
    // ex_object_t 
    // ======================================================== 

    EX_UID_INVALID, // uid
    EX_STRID_NULL, // name

    // ======================================================== 
    // ex_component_t
    // ======================================================== 

    NULL, // owner
    true, // active

    NULL, // init
    NULL, // deinit
EX_DEF_CLASS_END

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

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_component_init ( void *_self ) {
    ex_object_init(_self);
    ((ex_object_t *)_self)->name = ex_strid("Component"); 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_component_deinit ( void *_self ) {
    ex_component_t *self = (ex_component_t *)_self;
    self->owner = NULL;
    ex_object_deinit(_self);
}
