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
extern void __object_init( ex_ref_t * );
// ------------------------------------------------------------------ 

void __component_init ( ex_ref_t *_self ) {
    __object_init(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __entity_remove_comp( ex_ref_t *_ent, ex_ref_t *_comp );
extern void __object_deinit( ex_ref_t * );
// ------------------------------------------------------------------ 

void __component_deinit ( ex_ref_t *_self ) {
    ex_component_t *self = EX_REF_CAST(ex_component_t,_self);

    if ( self->entity->ptr ) {
        __entity_remove_comp( self->entity, _self );
        self->entity = NULL;
    }

    __object_deinit(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EX_DEF_CLASS_BEGIN(ex_component_t)

    EX_COMPONENT_DEFAULT_MEMBER

    EX_MEMBER( ex_object_t, name, ex_strid("Component") )
    EX_MEMBER( ex_object_t, init, __component_init )
    EX_MEMBER( ex_object_t, deinit, __component_deinit )

EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_component_t)
EX_DEF_PROPS_END

EX_SERIALIZE_SUPER_BEGIN(ex_component_t,ex_object_t)
EX_SERIALIZE_END

EX_DEF_TOSTRING_SUPER_BEGIN(ex_component_t,ex_object_t)
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////
