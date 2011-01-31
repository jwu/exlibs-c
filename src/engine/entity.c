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
            ex_decref(_comp);
            ex_array_remove_at_fast( ent->comps, __idx__ );
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

    ex_array_each ( self->comps, ex_ref_t *, comp ) {
        ex_decref(comp);
        ex_destroy_object(comp);
    } ex_array_each_end;
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

    ((ex_entity_t *)__obj__)->comps = ex_array_notype(sizeof(ex_ref_t *),8);
    EX_MEMBER( ex_entity_t, world, NULL )
    EX_MEMBER( ex_entity_t, trans2d, NULL )
    EX_MEMBER( ex_entity_t, camera, NULL )

EX_DEF_OBJECT_END

EX_DEF_PROPS_BEGIN(ex_entity_t)
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN_SUPER(ex_entity_t,ex_object_t)
#if 0
    int num_comps = 0;

    // serialize components
    num_comps = self->comps->count;
    EX_SERIALIZE( _stream, int, "num_comps", &num_comps  )

    if ( _stream->type == EX_STREAM_READ ) {
        for ( int i = 0; i < num_comps; ++i ) {
            strid_t typeID;
            void *comp;
            ex_ref_t *compref;

            // get the component type first.
            EX_SERIALIZE( _stream, strid, "comp_type", &typeID )

            // check if we have the component
            compref = ex_entity_get_comp(self,typeID);
            if ( compref ) {
                ex_warning("the component %s already added in this entity", ex_strid_to_cstr(typeID));
                continue;
            }

            // create a component serialize, and finially add it.
            comp = ex_create(typeID);
            if ( comp ) {
                ex_rtti_info(comp)->serialize( _stream, ex_strid("component"), comp );
                __add_comp( self, typeID, comp );
            }
        }
    }
    else if ( _stream->type == EX_STREAM_WRITE ) {
        ex_array_each ( self->comps, ex_ref_t *, compref ) {
            void *comp = compref->ptr;
            EX_SERIALIZE( _stream, strid, "comp_type", &(ex_rtti_info(comp)->typeID) )
            ex_rtti_info(comp)->serialize( _stream, ex_strid("component"), comp );
        } ex_array_each_end
    }
#else
    EX_SERIALIZE_ARRAY( _stream, ref, "components", self->comps );
#endif
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

// TODO { 
// int ex_entity_remove_comp ( ex_entity_t *_ent, ex_ref_t *_comp ) {
//     ex_array_each ( self->comps, ex_ref_t *, compref ) {
//         if ( _comp == compref ) {
//             strid_t typeID;

//             ex_array_remove_at_fast( self->comps, __idx__ );

//             // cache internal component
//             typeID = ex_rtti_info(_comp->ptr)->typeID;
//             if ( _typeID == EX_TYPEID(ex_trans2d_t) ) {
//                 _ent->trans2d = NULL;
//             }
//             else if ( _typeID == EX_TYPEID(ex_camera_t) ) {
//                 _ent->camera = NULL;
//             }

//             //
//             ex_decref(_comp);
//             return 0;
//         }
//     } ex_array_each_end
//     return -1;
// }
// } TODO end 

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
