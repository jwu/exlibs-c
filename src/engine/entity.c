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
//
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __add_to_cache ( ex_entity_t *_ent, strid_t _typeID, ex_ref_t *_comp_ref ) {
    // cache internal component
    if ( _typeID == EX_TYPEID(ex_trans2d_t) )
        _ent->trans2d = _comp_ref;
    else if ( _typeID == EX_TYPEID(ex_camera_t) )
        _ent->camera = _comp_ref;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __rm_from_cache ( ex_entity_t *_ent, strid_t _typeID ) {
    // cache internal component
    if ( _typeID == EX_TYPEID(ex_trans2d_t) )
        _ent->trans2d = NULL;
    else if ( _typeID == EX_TYPEID(ex_camera_t) )
        _ent->camera = NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __add_comp ( ex_ref_t *_self, strid_t _typeID, ex_ref_t *_comp_ref ) {
    ex_entity_t *ent = EX_REF_PTR(ex_entity_t,_self);
    ex_component_t *comp = EX_REF_PTR(ex_component_t,_comp_ref);

    comp->owner = _self; // set the owner of the component before init.
    ((ex_object_t *)comp)->init(_comp_ref);

    ex_array_append( ent->comps, &_comp_ref );
    ex_incref(_comp_ref);
    __add_to_cache ( ent, _typeID, _comp_ref );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void __entity_remove_comp( ex_ref_t *_self, ex_ref_t *_comp ) {
    ex_entity_t *ent = EX_REF_PTR(ex_entity_t,_self);

    ex_array_each ( ent->comps, ex_ref_t *, comp ) {
        if ( comp == _comp ) {
            ex_array_remove_at_fast( ent->comps, __idx__ );
            ex_decref(_comp);
            __rm_from_cache ( ent, ex_rtti_info(comp->ptr)->typeID );
            break;
        }
    } ex_array_each_end
}

///////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void __entity_init ( ex_ref_t *_self ) {
    ex_entity_t *self = EX_REF_PTR(ex_entity_t,_self);

    ex_array_each ( self->comps, ex_ref_t *, comp ) {
        EX_REF_PTR(ex_component_t,comp)->owner = _self;
        EX_REF_PTR(ex_object_t,comp)->init(comp);
        __add_to_cache ( self, ex_rtti_info(comp->ptr)->typeID, comp );
    } ex_array_each_end
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void __entity_deinit ( ex_ref_t *_self ) {
    ex_entity_t *self = EX_REF_PTR(ex_entity_t,_self);
    ex_trans2d_t *trans2d = EX_REF_PTR(ex_trans2d_t,self->trans2d);
    ex_ref_t *comp;
    int i;

    // do children-parent deinit.
    if ( trans2d ) {
        // detach from parent
        ex_trans2d_set_parent( self->trans2d, NULL );

        //
        ex_array_each ( trans2d->children, ex_ref_t *, ref ) {
            ex_trans2d_t *child = EX_REF_PTR( ex_trans2d_t, ref );
            // NOTE: this will prevent child remove from parent, which we will do it by ourself
            child->parent = NULL;
            ex_destroy_object_immediately( ((ex_component_t *)child)->owner, true );
        } ex_array_each_end
        ex_array_remove_all(trans2d->children);
    }

    // NOTE: the destroy will invoke __entity_remove_comp, so we remove the component inversed.
    // NOTE: you don't need to call ex_decref, cause the component->deinit will do it.
    for ( i = ex_array_count(self->comps)-1; i >= 0; --i ) {
        comp = *((ex_ref_t **)ex_array_get(self->comps, i));
        ex_destroy_object_immediately(comp,true);
    }
    ex_array_free ( self->comps );

    self->comps = NULL;
    self->world = NULL;
    self->trans2d = NULL;
    self->camera = NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EX_DEF_OBJECT_BEGIN( ex_entity_t, 
                     "Entity", 
                     __entity_init, 
                     __entity_deinit )

    EX_MEMBER( ex_entity_t, comps, ex_array_notype(sizeof(ex_ref_t *),8) )
    EX_MEMBER( ex_entity_t, world, NULL )
    EX_MEMBER( ex_entity_t, trans2d, NULL )
    EX_MEMBER( ex_entity_t, camera, NULL )

EX_DEF_OBJECT_END

EX_DEF_PROPS_BEGIN(ex_entity_t)
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN_SUPER(ex_entity_t,ex_object_t)
    EX_SERIALIZE_ARRAY( _stream, ref, "components", self->comps );
EX_SERIALIZE_END

EX_DEF_TOSTRING_SUPER_BEGIN(ex_entity_t,ex_object_t)
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_entity_get_comp ( const ex_ref_t *_self, strid_t _typeID ) {
    ex_rtti_t *rtti;
    ex_entity_t *ent = EX_REF_PTR(ex_entity_t,_self);

    rtti = ex_rtti_get(_typeID);
    ex_assert_return( rtti, 
                      NULL,
                      "can't find the rtti type: %s", ex_strid_to_cstr(_typeID) );

    ex_array_each ( ent->comps, ex_ref_t *, comp ) {
        if ( ex_rtti_isa( ex_rtti_info(comp->ptr), rtti ) )
            return comp;
    } ex_array_each_end

    return NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_entity_add_comp ( ex_ref_t *_self, strid_t _typeID ) {
    ex_ref_t *compref;
    
    // if the component already exists, show a warning and return it.
    compref = ex_entity_get_comp(_self,_typeID);
    if ( compref ) {
        ex_warning("the component %s already added in this entity", ex_strid_to_cstr(_typeID));
        return NULL;
    }

    // create a component and added to the component list, then return it.
    compref = ex_create_object(_typeID,ex_generate_uid());
    if ( compref ) {
        __add_comp( _self, _typeID, compref );
        return compref;
    }

    return NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_entity_level_start ( ex_ref_t *_self ) {
    ex_entity_t *ent = EX_REF_PTR(ex_entity_t,_self);
    ex_behavior_t *be;

    ex_array_each ( ent->comps, ex_ref_t *, compref ) {
        be = EX_REF_PTR(ex_behavior_t,compref);
        if ( ex_childof(ex_behavior_t,be) ) {
            if ( be->level_start )
                be->level_start(compref);
        }
    } ex_array_each_end
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_entity_update ( ex_ref_t *_self ) {
    ex_entity_t *ent = EX_REF_PTR(ex_entity_t,_self);
    ex_behavior_t *be;

    ex_array_each ( ent->comps, ex_ref_t *, compref ) {
        be = EX_REF_PTR(ex_behavior_t,compref);

        if ( ex_childof(ex_behavior_t,be) ) {
            if ( be->state == EX_BEHAVIOR_STATE_NEW ) {
                if ( be->start )
                    be->start(compref);
                be->state = EX_BEHAVIOR_STATE_STARTED;
            }
            else {
                if ( be->update ) 
                    be->update(compref);
            }
        }
    } ex_array_each_end
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_entity_post_update ( ex_ref_t *_self ) {
    ex_entity_t *ent = EX_REF_PTR(ex_entity_t,_self);
    ex_behavior_t *be;

    ex_array_each ( ent->comps, ex_ref_t *, compref ) {
        be = EX_REF_PTR(ex_behavior_t,compref);

        if ( ex_childof(ex_behavior_t,be) ) {
            if ( be->post_update )
                be->post_update(compref);
        }
    } ex_array_each_end
}
