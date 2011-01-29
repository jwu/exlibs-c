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

#include "component/trans2d.h"
#include "component/camera.h"
#include "component/behavior.h"

///////////////////////////////////////////////////////////////////////////////
// properties
///////////////////////////////////////////////////////////////////////////////

EX_DEF_CLASS_BEGIN(ex_entity_t)
    // ======================================================== 
    // ex_object_t
    // ======================================================== 

    EX_UID_INVALID, // uid
    EX_STRID_NULL, // name

    // ======================================================== 
    // ex_entity_t
    // ======================================================== 

    NULL, // comps
    NULL, // world

    NULL, // trans2d
    NULL, // camera
EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_entity_t)
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN_SUPER(ex_entity_t,ex_object_t)
    int num_comps = 0;

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

EX_DEF_TOSTRING_SUPER_BEGIN(ex_entity_t,ex_object_t)
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_entity_init ( ex_entity_t *_ent ) {
    ex_object_init(_ent);
    ((ex_object_t *)_ent)->name = ex_strid("New Entity");

    _ent->comps = ex_array_notype(sizeof(void *),8);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_entity_deinit ( ex_entity_t *_ent ) {
    ex_array_each ( _ent->comps, ex_component_t *, comp ) {
        if ( comp->deinit ) comp->deinit(comp);
        ex_free(comp);
    } ex_array_each_end;
    ex_array_free ( _ent->comps );

    _ent->comps = NULL;
    _ent->world = NULL;
    _ent->trans2d = NULL;
    _ent->camera = NULL;
    
    ex_object_deinit(_ent);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_entity_reset ( ex_entity_t *_ent ) {
    // TODO: { 
    // ex_array_each ( _ent->comps, ex_component_t *, comp ) {
    //     if ( comp->reset )
    //         comp->reset(comp);
    // } ex_array_each_end;
    // } TODO end 
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

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_entity_level_start ( ex_entity_t *_ent ) {
    ex_behavior_t *be;
    ex_array_each ( _ent->comps, ex_component_t *, comp ) {
        if ( ex_childof(ex_behavior_t,comp) ) {
            be = (ex_behavior_t *)comp; 
            if ( be->level_start )
                be->level_start(be);
        }
    } ex_array_each_end
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_entity_update ( ex_entity_t *_ent ) {
    ex_behavior_t *be;
    ex_array_each ( _ent->comps, ex_component_t *, comp ) {
        if ( ex_childof(ex_behavior_t,comp) ) {
            be = (ex_behavior_t *)comp; 
            if ( be->state == EX_BEHAVIOR_STATE_NEW ) {
                if ( be->start )
                    be->start(be);
                be->state = EX_BEHAVIOR_STATE_STARTED;
            }
            else {
                if ( be->update ) 
                    be->update(be);
            }
        }
    } ex_array_each_end
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_entity_post_update ( ex_entity_t *_ent ) {
    ex_behavior_t *be;
    ex_array_each ( _ent->comps, ex_component_t *, comp ) {
        if ( ex_childof(ex_behavior_t,comp) ) {
            be = (ex_behavior_t *)comp; 
            if ( be->post_update )
                be->post_update(be);
        }
    } ex_array_each_end
}
