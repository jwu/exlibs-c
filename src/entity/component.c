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
#include "entity.h"

#include "trans2d.h"

///////////////////////////////////////////////////////////////////////////////
// properties
///////////////////////////////////////////////////////////////////////////////

EX_DEF_CLASS_BEGIN(ex_component_t)
    NULL, // owner
    true, // active

    NULL, // init
    NULL, // deinit
EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_component_t)
    EX_PROP( ex_component_t, active, "active",  EX_PROP_ATTR_HIDE, ex_prop_set_raw_bool, ex_prop_get_raw_bool )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN(ex_component_t)
    EX_MEMBER_SERIALIZE( bool, active )
EX_SERIALIZE_END

EX_DEF_TOSTRING_BEGIN(ex_component_t)
    EX_MEMBER_TOSTRING ( bool, "active", self->active )
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_component_init ( void *_self ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_component_deinit ( void *_self ) {
}
