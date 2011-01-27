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
// properties
///////////////////////////////////////////////////////////////////////////////

EX_DEF_CLASS_BEGIN(ex_entity_t)
    EX_UID_INVALID, // uid, NOTE: only generated in ex_world_create_entity
    EX_STRID_NULL, // name
    NULL, // comps
    NULL, // world

    NULL, // trans2d
    NULL, // camera
EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_entity_t)
    EX_PROP( ex_entity_t, name, "name",  EX_PROP_ATTR_NONE, ex_prop_set_raw_strid, ex_prop_get_raw_strid )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN(ex_entity_t)
    int num_comps = 0;

    // serialize members
    EX_MEMBER_SERIALIZE( uid, uid )
    EX_MEMBER_SERIALIZE( strid, name )

    // serialize components
    num_comps = self->comps->count;
    EX_SERIALIZE( _stream, int, "num_comps", &num_comps  )

    if ( _stream->type == EX_STREAM_READ ) {
        for ( int i = 0; i < num_comps; ++i ) {
            strid_t typeID;
            void *comp;

            EX_SERIALIZE( _stream, strid, "comp_type", &typeID )
            comp = ex_entity_add_comp( self, typeID );
            ex_rtti_info(comp)->serialize( _stream, ex_strid("component"), comp );
        }
    }
    else if ( _stream->type == EX_STREAM_WRITE ) {
        ex_array_each ( self->comps, ex_component_t *, comp ) {
            EX_SERIALIZE( _stream, strid, "comp_type", &(ex_rtti_info(comp)->typeID) )
            ex_rtti_info(comp)->serialize( _stream, ex_strid("component"), comp );
        } ex_array_each_end
    }
EX_SERIALIZE_END

EX_DEF_TOSTRING_BEGIN(ex_entity_t)
    EX_MEMBER_TOSTRING( uid, "uid", self->uid )
    EX_MEMBER_TOSTRING( strid, "name", self->name )
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_entity_init ( ex_entity_t *_ent ) {
    _ent->comps = ex_array_notype(sizeof(void *),8);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_entity_deinit ( ex_entity_t *_ent ) {
    ex_array_each ( _ent->comps, ex_component_t *, comp ) {
        if ( comp->deinit )
            comp->deinit(comp);
        ex_free(comp);
    } ex_array_each_end;
    ex_array_free ( _ent->comps );
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
        if ( _typeID == EX_TYPEID(ex_trans2d_t) )
            _ent->trans2d = (ex_trans2d_t *)comp; 
        else if ( _typeID == EX_TYPEID(ex_camera_t) )
            _ent->camera = (ex_camera_t *)comp; 
    }

    return comp;
}
