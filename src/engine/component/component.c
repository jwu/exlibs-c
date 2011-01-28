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
    -1, // id
    NULL, // owner
    true, // active

    NULL, // init
    NULL, // deinit
EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_component_t)
    EX_PROP( ex_component_t, bool, active, "active",  EX_PROP_ATTR_HIDE )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN(ex_component_t)
    EX_MEMBER_SERIALIZE( uint32, id )
    EX_MEMBER_SERIALIZE( bool, active )
EX_SERIALIZE_END

EX_DEF_TOSTRING_BEGIN(ex_component_t)
    EX_MEMBER_TOSTRING ( uint32, "id", self->id )
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
