// ======================================================================================
// File         : entity.c
// Author       : Wu Jie 
// Last Change  : 11/25/2010 | 17:40:01 PM | Thursday,November
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "entity.h"

///////////////////////////////////////////////////////////////////////////////
// properties
///////////////////////////////////////////////////////////////////////////////

EX_DEF_PROPS_BEGIN(ex_entity_t)
    EX_PROP( ex_entity_t, _name, "name",  EX_PROP_ATTR_NONE, ex_prop_set_raw_strid, ex_prop_get_raw_strid )
EX_DEF_PROPS_END(ex_entity_t)

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_entity_t* ex_entity_alloc () {
    ex_entity_t* ent = (ex_entity_t*)ex_malloc(sizeof(ex_entity_t));
    ent->_name = EX_STRID_INVALID;
    ent->_comps = ex_array_alloc( sizeof(ex_component_t*), 1 );
    return ent;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_entity_free ( ex_entity_t* _entity ) {
    ex_array_free ( _entity->_comps );
    ex_free ( _entity );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EX_RESULT ex_entity_add_comp ( ex_entity_t* _ent, ex_component_t* _comp ) {
    return EX_RESULT_OK;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_component_t* ex_entity_get_comp ( ex_entity_t* _ent, const char* _typename ) {
    ex_rtti_t* rtti;
    ex_array_t* comps;

    rtti = ex_rtti_get(_typename);
    ex_assert( rtti, "can't find the rtti type: %s", _typename );
    comps = _ent->_comps;

    ex_array_each ( ex_component_t*, comp, comps ) {
        if ( ex_rtti_isa( ex_rtti_info(comp), rtti ) )
            return comp;
    } ex_array_each_end

    return NULL;
}
