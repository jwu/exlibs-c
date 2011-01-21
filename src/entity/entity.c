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

#include "trans2d.h"
#include "camera.h"

///////////////////////////////////////////////////////////////////////////////
// private
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static EX_RESULT __entity_add_comp ( ex_entity_t *_ent, ex_component_t *_comp ) {
    // TODO:
    return EX_RESULT_OK;
}

///////////////////////////////////////////////////////////////////////////////
// properties
///////////////////////////////////////////////////////////////////////////////

EX_DEF_CLASS_CREATOR(ex_entity_t) {
    return __alloc_ex_entity_t();
}

EX_DEF_PROPS_BEGIN(ex_entity_t)
    EX_PROP( ex_entity_t, name, "name",  EX_PROP_ATTR_NONE, ex_prop_set_raw_strid, ex_prop_get_raw_strid )
EX_DEF_PROPS_END(ex_entity_t)

EX_SERIALIZE_BEGIN(ex_entity_t)
EX_SERIALIZE_END

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_entity_t *ex_entity_alloc () {
    ex_entity_t *ent = (ex_entity_t *)ex_malloc(sizeof(ex_entity_t));
    ent->name = EX_STRID_NULL;
    ent->comps = ex_array_alloc( sizeof(ex_component_t *), 1 );
    return ent;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_entity_free ( ex_entity_t *_ent ) {
    ex_array_each ( _ent->comps, ex_component_t *, comp ) {
        if ( comp->deinit )
            comp->deinit(comp);
        ex_free(comp);
    } ex_array_each_end;
    ex_array_free ( _ent->comps );
    ex_free ( _ent );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_component_t *ex_entity_get_comp ( const ex_entity_t *_ent, strid_t _typeID ) {
    ex_rtti_t *rtti;

    rtti = ex_rtti_get(_typeID);
    ex_assert( rtti, "can't find the rtti type: %s", ex_strid_to_cstr(_typeID) );

    ex_array_each ( _ent->comps, ex_component_t *, comp ) {
        if ( ex_rtti_isa( ex_rtti_info(comp), rtti ) )
            return comp;
    } ex_array_each_end

    return NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_component_t *ex_entity_add_comp ( ex_entity_t *_ent, strid_t _typeID ) {
    ex_component_t *comp;
    
    // if the component already exists, show a warning and return it.
    comp = ex_entity_get_comp(_ent,_typeID);
    if ( comp ) {
        ex_warning("the component %s already added in this entity", ex_strid_to_cstr(_typeID));
        return comp;
    }

    // create a component and added to the component list, then return it.
    comp = (ex_component_t *)ex_create(_typeID);
    if ( comp ) {
        comp->owner = _ent; // set the owner of the component before init.
        if ( comp->init )
            comp->init(comp);
        ex_array_append( _ent->comps, &comp );

        // cache internal component
        if ( _typeID == EX_TYPEID(ex_trans2d_t) ) {
            _ent->trans2d = (ex_trans2d_t *)comp; 
        }
        else if ( _typeID == EX_TYPEID(ex_camera_t) ) {
            _ent->camera = (ex_camera_t *)comp; 
        }
    }

    return comp;
}
