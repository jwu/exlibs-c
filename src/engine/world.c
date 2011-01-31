// ======================================================================================
// File         : world.c
// Author       : Wu Jie 
// Last Change  : 12/01/2010 | 17:28:17 PM | Wednesday,December
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "world.h"
#include "entity.h"

#include "component/trans2d.h"
#include "component/camera.h"
// TEMP { 
#include "component/debug2d.h"
// } TEMP end 

///////////////////////////////////////////////////////////////////////////////
// class define
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void __world_init ( ex_ref_t *_self ) {
    ex_world_t *self = (ex_world_t *)_self->ptr;

    ex_array_each ( self->entities, ex_ref_t *, ref ) {
        EX_REF_PTR(ex_entity_t,ref)->world = _self;
        EX_REF_PTR(ex_object_t,ref)->init(ref);
    } ex_array_each_end;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void __world_deinit ( ex_ref_t *_self ) {
    ex_world_t *self = (ex_world_t *)_self->ptr;

    self->mainCamera = NULL;

    // decrease reference count for all cameras
    ex_array_each ( self->cameras, ex_ref_t *, ref ) {
        ex_decref(ref);
    } ex_array_each_end;
    ex_array_free ( self->cameras );

    // decrease reference count and destroy all entities
    ex_array_each ( self->entities, ex_ref_t *, ref ) {
        ex_decref(ref);
        ex_destroy_object_immediately( ref );
    } ex_array_each_end;
    ex_array_free ( self->entities );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EX_DEF_OBJECT_BEGIN( ex_world_t, 
                     "World", 
                     __world_init, 
                     __world_deinit )

    EX_MEMBER( ex_world_t, state, EX_WORLD_STATE_STOPPED )

    ((ex_world_t *)__obj__)->entities = ex_array_notype( sizeof(ex_ref_t *), 8 );
    ((ex_world_t *)__obj__)->cameras = ex_array_notype( sizeof(ex_ref_t *), 8 );

    EX_MEMBER( ex_world_t, mainCamera, NULL )

EX_DEF_OBJECT_END

EX_DEF_PROPS_BEGIN(ex_world_t)
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN_SUPER(ex_world_t,ex_object_t)
#if 0
    int num_entities = 0;

    num_entities = self->entities->count;
    EX_SERIALIZE( _stream, int, "num_entities", &num_entities  )

    if ( _stream->type == EX_STREAM_READ ) {
        ex_entity_t *ent = NULL;
        for ( int i = 0; i < num_entities; ++i ) {

            ent = EX_RTTI(ex_entity_t)->create();
            EX_SERIALIZE( _stream, ex_entity_t, "entity", ent );
            ent->world = self->ref;
            ex_init_object(ent);

            ex_array_append( self->entities, ex_object_ref(ent) );
            ex_incref( ex_object_ref(ent) );
        }
    }
    else if ( _stream->type == EX_STREAM_WRITE ) {
        ex_array_each ( self->entities, ex_ref_t *, ref ) {
            EX_SERIALIZE( _stream, ex_entity_t, "entity", EX_REF_PTR(ex_entity_t,ref) );
        } ex_array_each_end
    }  
#else
    EX_SERIALIZE_ARRAY( _stream, ref, "entities", self->entities );
#endif
EX_SERIALIZE_END

EX_DEF_TOSTRING_SUPER_BEGIN(ex_world_t,ex_object_t)
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// internal defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __debug_draw ( ex_world_t *_world ) {
    ex_array_each ( _world->entities, ex_ref_t *, ref ) {
        ex_ref_t *dbg2d = ex_entity_get_comp( ref, EX_TYPEID(ex_debug2d_t) );
        if ( dbg2d )
            ex_debug2d_draw(dbg2d);
    } ex_array_each_end;
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_save ( ex_ref_t *_self, ex_stream_t *_stream ) {
    ex_uid_t uid; 

    ex_serialize_objects(_stream);
    uid = ex_object_uid(_self);
    EX_SERIALIZE( _stream, uid, "uid", &uid );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_world_load ( ex_stream_t *_stream ) {
    ex_uid_t uid;

    ex_serialize_objects(_stream);
    EX_SERIALIZE( _stream, uid, "uid", &uid );
    return ex_getref(uid);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_reset ( ex_ref_t *_self, ex_stream_t *_stream ) {
    // TODO:
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_world_create_entity ( ex_ref_t *_self, strid_t _name ) {
    ex_world_t *world = EX_REF_PTR(ex_world_t,_self);
    ex_ref_t *ref = ex_create_object( EX_TYPEID(ex_entity_t), ex_generate_uid() );

    EX_REF_PTR(ex_object_t,ref)->name = _name;
    EX_REF_PTR(ex_entity_t,ref)->world = _self;
    EX_REF_PTR(ex_object_t,ref)->init(ref);

    ex_array_append( world->entities, &ref );
    ex_incref( ref );

    return ref;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_world_create_camera2d ( ex_ref_t *_self, strid_t _name ) {
    ex_ref_t *cam;
    ex_ref_t *ent = ex_world_create_entity( _self, _name );

    ex_entity_add_comp( ent, EX_TYPEID(ex_trans2d_t) );
    cam = ex_entity_add_comp( ent, EX_TYPEID(ex_camera_t) );
    ex_camera_set_ortho( cam, true );

    return ent;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_world_create_rect ( ex_ref_t *_self, strid_t _name ) {
    // TODO:
    return NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_clear ( ex_ref_t *_self ) {
	ex_world_t *world = EX_REF_PTR(ex_world_t,_self);
	
    // decrease reference count and destroy all entities
    ex_array_each ( world->entities, ex_ref_t *, ref ) {
        ex_decref(ref);
        ex_destroy_object_immediately( ref );
    } ex_array_each_end;
    ex_array_remove_all ( world->entities );
    ex_object_gc();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_add_camera ( ex_ref_t *_self, ex_ref_t *_cam ) {
    ex_world_t *world = EX_REF_PTR(ex_world_t,_self);

    ex_array_append( world->cameras, &_cam );
    if ( ex_array_count(world->cameras) == 1 ) {
        world->mainCamera = _cam;
    }
    ex_incref(_cam);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_remove_camera ( ex_ref_t *_self, ex_ref_t *_cam ) {
    ex_world_t *world = EX_REF_PTR(ex_world_t,_self);
    int result_idx = -1;

    ex_array_each ( world->cameras, ex_ref_t *, cam ) {
        if ( cam == _cam ) {
            result_idx = __idx__;
            break;
        }
    } ex_array_each_end;

    //
    if ( result_idx != -1 ) {
        ex_array_remove_at_fast( world->cameras, result_idx );
        ex_decref(_cam);
    }
    else {
        ex_warning ( "can't find camera %s", ex_strid_to_cstr( ex_object_name( ((ex_component_t *)_cam)->owner->ptr ) ) );
        return;
    }

    if ( ex_array_count(world->cameras) == 1 ) {
        world->mainCamera = *((ex_ref_t **)ex_array_get(world->cameras,0));
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_world_main_camera ( ex_ref_t *_self ) {
    return EX_REF_PTR(ex_world_t,_self)->mainCamera;
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __tick_engine_time ();
// ------------------------------------------------------------------ 

void ex_world_update ( ex_ref_t *_self ) {
    ex_world_t *world = EX_REF_PTR(ex_world_t,_self);

    // the world is stopped, don't do anything, even the timer tick
    if ( world->state == EX_WORLD_STATE_STOPPED )
        return;

    __tick_engine_time ();

    // the world is paused, only timer ticks 
    if ( world->state == EX_WORLD_STATE_PAUSED )
        return;

    // TODO: __handle_input();

    // update behavior
    ex_array_each ( world->entities, ex_ref_t *, ent ) {
        if ( ex_object_is_dead(ent) )
            ex_array_continue;

        ex_entity_update(ent);
    } ex_array_each_end

    // TODO: __update_animation(world);
    // TODO: __update_physics(world);

    // post-update behavior
    ex_array_each ( world->entities, ex_ref_t *, ent ) {
        if ( ex_object_is_dead(ent) )
            ex_array_continue;

        ex_entity_post_update(ent);
    } ex_array_each_end

    // at the end of the update, we do a garbage collection
    ex_array_each ( world->entities, ex_ref_t *, ent ) {
        if ( ent->ptr == NULL || ex_object_is_dead(ent) ) {
            ex_array_remove_at_fast( world->entities, __idx__ );
            --__idx__;
        }
    } ex_array_each_end
    ex_object_gc();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_render ( ex_ref_t *_self ) {
    ex_world_t *world = EX_REF_PTR(ex_world_t,_self);

    ex_array_each ( world->cameras, ex_ref_t *, cam ) {
        ex_camera_apply (cam);
        // TODO: __render_scene(_world);
        __debug_draw(world); // TEMP
    } ex_array_each_end;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_run ( ex_ref_t *_self ) {
    ex_world_t *world = EX_REF_PTR(ex_world_t,_self);

    if ( world->state != EX_WORLD_STATE_STOPPED )
        return;

    // start all behavior
    ex_array_each ( world->entities, ex_ref_t *, ent ) {
        ex_entity_level_start(ent);
    } ex_array_each_end

    //
    world->state = EX_WORLD_STATE_RUNNING;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_stop ( ex_ref_t *_self ) {
    ex_world_t *world = EX_REF_PTR(ex_world_t,_self);

    if ( world->state == EX_WORLD_STATE_STOPPED )
        return;
    world->state = EX_WORLD_STATE_STOPPED;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_pause ( ex_ref_t *_self ) {
    ex_world_t *world = EX_REF_PTR(ex_world_t,_self);

    if ( world->state != EX_WORLD_STATE_RUNNING )
        return;
    world->state = EX_WORLD_STATE_PAUSED;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_resume ( ex_ref_t *_self ) {
    ex_world_t *world = EX_REF_PTR(ex_world_t,_self);

    if ( world->state != EX_WORLD_STATE_PAUSED )
        return;
    world->state = EX_WORLD_STATE_RUNNING;
}
