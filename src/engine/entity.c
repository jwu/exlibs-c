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
#include "world.h"

#include "component/trans2d.h"
#include "component/camera.h"
#include "component/behavior.h"

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __add_to_cache ( ex_entity_t *_ent, strid_t _typeID, ex_ref_t *_compref ) {
    // cache internal component
    if ( _typeID == EX_TYPEID(ex_trans2d_t) )
        _ent->trans2d = _compref;
    else if ( _typeID == EX_TYPEID(ex_camera_t) )
        _ent->camera = _compref;
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

static void __add_comp ( ex_ref_t *_self, strid_t _typeID, ex_ref_t *_compref ) {
    ex_entity_t *ent;
    ex_component_t *comp;

    ent = EX_REF_CAST(ex_entity_t,_self);
    comp = EX_REF_CAST(ex_component_t,_compref);
    comp->entity = _self; // set the entity of the component before init.
    ((ex_object_t *)comp)->init(_compref);

    ex_array_append( ent->comps, &_compref );
    ex_incref(_compref);
    __add_to_cache ( ent, _typeID, _compref );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static ex_ref_t *__entity_get_comp ( const ex_ref_t *_self, ex_rtti_t *_rtti ) {
    ex_entity_t *ent;
    
    ent = EX_REF_CAST(ex_entity_t,_self);
    ex_array_each ( ent->comps, ex_ref_t *, comp ) {
        if ( ex_rtti_isa( ex_rtti_info(comp->ptr), _rtti ) )
            return comp;
    } ex_array_each_end

    return NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void __entity_remove_comp( ex_ref_t *_self, ex_ref_t *_comp ) {
    ex_entity_t *ent;
    
    ent = EX_REF_CAST(ex_entity_t,_self);
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
extern void __object_init( ex_ref_t * );
// ------------------------------------------------------------------ 

void __entity_init ( ex_ref_t *_self ) {
    ex_entity_t *self;
    
    self = EX_REF_CAST(ex_entity_t,_self);
    __object_init(_self);

    ex_array_each ( self->comps, ex_ref_t *, comp ) {
        EX_REF_CAST(ex_component_t,comp)->entity = _self;
        EX_REF_CAST(ex_object_t,comp)->init(comp);
        __add_to_cache ( self, ex_rtti_info(comp->ptr)->typeID, comp );
    } ex_array_each_end
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __object_deinit( ex_ref_t * );
// ------------------------------------------------------------------ 

void __entity_deinit ( ex_ref_t *_self ) {
    ex_entity_t *self;
    ex_trans2d_t *trans2d;
    ex_ref_t *comp;
    int i;

    self = EX_REF_CAST(ex_entity_t,_self);
    trans2d = EX_REF_CAST(ex_trans2d_t,self->trans2d);

    // do children-parent deinit.
    if ( trans2d ) {
        // detach from parent
        ex_trans2d_set_parent( self->trans2d, NULL );

        //
        ex_array_each ( trans2d->children, ex_ref_t *, ref ) {
            ex_trans2d_t *child = EX_REF_CAST( ex_trans2d_t, ref );
            // NOTE: this will prevent child remove from parent, which we will do it by ourself
            child->parent = NULL;
            ex_destroy_object_immediately( ((ex_component_t *)child)->entity, true );
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

    __object_deinit(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EX_DEF_CLASS_BEGIN(ex_entity_t)

    EX_ENTITY_DEFAULT_MEMBER

    EX_MEMBER( ex_object_t, name, ex_strid("Entity") )
    EX_MEMBER( ex_object_t, init, __entity_init )
    EX_MEMBER( ex_object_t, deinit, __entity_deinit )

EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_entity_t)
EX_DEF_PROPS_END

EX_SERIALIZE_SUPER_BEGIN(ex_entity_t,ex_object_t)
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

    rtti = ex_rtti_get(_typeID);
    ex_assert_return( rtti, 
                      NULL,
                      "can't find the rtti type: %s", ex_strid_to_cstr(_typeID) );

    return __entity_get_comp( _self, rtti );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_entity_add_comp ( ex_ref_t *_self, strid_t _typeID ) {
    ex_ref_t *compref;
    ex_rtti_t *rtti;

    rtti = ex_rtti_get(_typeID);
    ex_assert_return( rtti, 
                      NULL,
                      "can't find the rtti type: %s", ex_strid_to_cstr(_typeID) );
    
    // if the component already exists, show a warning and return it.
    compref = __entity_get_comp( _self, rtti );
    if ( compref ) {
        ex_warning("the component %s already added in this entity", ex_strid_to_cstr(_typeID));
        return NULL;
    }

    // create a component and added to the component list, then return it.
    compref = ex_create_object(rtti,ex_generate_uid());
    if ( compref ) {
        __add_comp( _self, _typeID, compref );
        return compref;
    }

    return NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern ex_ref_t *ex_entity_add_comp_auto_awake ( ex_ref_t *_self, strid_t _typeID ) {
    ex_ref_t *compref;
    ex_behavior_t *be;
    ex_entity_t *ent;
    
    ent = EX_REF_CAST(ex_entity_t,_self);
    compref = ex_entity_add_comp( _self, _typeID );
    if ( compref ) {
        be = EX_REF_AS( ex_behavior_t, compref);
        if ( be && be->awake && 
             ( ex_world_is_running ( ent->world ) || ex_world_is_paused ( ent->world ) ) ) 
        {
            be->awake(compref);
        }
    }
    return compref;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_entity_awake_behaviors ( ex_ref_t *_self ) {
    ex_entity_t *ent;
    ex_behavior_t *be;

    ent = EX_REF_CAST(ex_entity_t,_self);
    ex_array_each ( ent->comps, ex_ref_t *, compref ) {
        be = EX_REF_AS(ex_behavior_t,compref);
        if ( be && be->awake )
            be->awake(compref);
    } ex_array_each_end
}
