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
// static defines
///////////////////////////////////////////////////////////////////////////////

static ex_ref_t *__cur_world = NULL;

///////////////////////////////////////////////////////////////////////////////
// class define
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
extern void __object_init( ex_ref_t * );
// ------------------------------------------------------------------ 

void __world_init ( ex_ref_t *_self ) {
    ex_world_t *self = (ex_world_t *)_self->ptr;

    //
    __object_init(_self);

    //
    ex_invoke_mng_init( &self->invoke_mng );

    //
    ex_array_each ( self->entities, ex_ref_t *, ref ) {
        EX_REF_CAST(ex_entity_t,ref)->world = _self;
        EX_REF_CAST(ex_object_t,ref)->init(ref);
    } ex_array_each_end;

    // if this is the first world
    if ( __cur_world == NULL ) {
        __cur_world = _self;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __object_deinit( ex_ref_t * );
// ------------------------------------------------------------------ 

void __world_deinit ( ex_ref_t *_self ) {
    ex_world_t *self = (ex_world_t *)_self->ptr;

    self->main_camera = NULL;

    // decrease reference count for all cameras
    ex_array_each ( self->cameras, ex_ref_t *, ref ) {
        ex_decref(ref);
    } ex_array_each_end;
    ex_array_free ( self->cameras );

    // decrease reference count and destroy all entities
    ex_array_each ( self->entities, ex_ref_t *, ref ) {
        ex_destroy_object_immediately( ref, true );
        ex_decref(ref);
    } ex_array_each_end;
    ex_array_free ( self->entities );

    //
    ex_invoke_mng_deinit( &self->invoke_mng );

    //
    if ( __cur_world == _self ) {
        __cur_world = NULL;
    }

    __object_deinit(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EX_DEF_OBJECT_BEGIN( ex_world_t, 
                     "World", 
                     __world_init, 
                     __world_deinit )

    EX_MEMBER( ex_world_t, state, EX_WORLD_STATE_STOPPED )
    EX_MEMBER( ex_world_t, entities, ex_array_notype( sizeof(ex_ref_t *), 8 ) )
    EX_MEMBER( ex_world_t, cameras, ex_array_notype( sizeof(ex_ref_t *), 8 ) )
    EX_MEMBER( ex_world_t, main_camera, NULL )
EX_DEF_OBJECT_END

EX_DEF_PROPS_BEGIN(ex_world_t)
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN_SUPER(ex_world_t,ex_object_t)
    EX_SERIALIZE_ARRAY( _stream, ref, "entities", self->entities );
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

ex_ref_t *ex_current_world () { return __cur_world; }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_set_current_world ( ex_ref_t *_world ) { __cur_world = _world; }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_world_create_entity ( ex_ref_t *_self, strid_t _name ) {
    ex_world_t *self = EX_REF_CAST(ex_world_t,_self);
    ex_ref_t *ref = ex_create_object( EX_RTTI(ex_entity_t), ex_generate_uid() );

    EX_REF_CAST(ex_object_t,ref)->name = _name;
    EX_REF_CAST(ex_entity_t,ref)->world = _self;
    EX_REF_CAST(ex_object_t,ref)->init(ref);

    ex_array_append( self->entities, &ref );
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
	ex_world_t *self;
    
    self = EX_REF_CAST(ex_world_t,_self);
    ex_lua_clear_refs( ex_lua_main_state() );
	
    // decrease reference count and destroy all entities
    ex_array_each ( self->entities, ex_ref_t *, ref ) {
        if ( ref->ptr == NULL || ex_object_is_dead(ref) )
            ex_array_continue;

        ex_destroy_object_immediately( ref, true );
        ex_decref(ref);
    } ex_array_each_end;
    ex_array_remove_all ( self->entities );
    ex_object_gc();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_add_camera ( ex_ref_t *_self, ex_ref_t *_cam ) {
    ex_world_t *self = EX_REF_CAST(ex_world_t,_self);

    ex_array_append( self->cameras, &_cam );
    ex_incref(_cam);
    if ( ex_array_count(self->cameras) == 1 ) {
        self->main_camera = _cam;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_remove_camera ( ex_ref_t *_self, ex_ref_t *_cam ) {
    ex_world_t *self = EX_REF_CAST(ex_world_t,_self);
    int result_idx = -1;

    ex_array_each ( self->cameras, ex_ref_t *, cam ) {
        if ( cam == _cam ) {
            result_idx = __idx__;
            break;
        }
    } ex_array_each_end;

    //
    if ( result_idx != -1 ) {
        ex_array_remove_at_fast( self->cameras, result_idx );
        ex_decref(_cam);
    }
    else {
        ex_warning ( "can't find camera %s", ex_strid_to_cstr( ex_object_name( ((ex_component_t *)_cam)->entity->ptr ) ) );
        return;
    }

    if ( ex_array_count(self->cameras) == 1 ) {
        self->main_camera = *((ex_ref_t **)ex_array_get(self->cameras,0));
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_world_main_camera ( ex_ref_t *_self ) {
    return EX_REF_CAST(ex_world_t,_self)->main_camera;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_world_find_entity_byname ( ex_ref_t *_self, strid_t _name ) {
    ex_world_t *self = EX_REF_CAST(ex_world_t,_self);

    ex_array_each ( self->entities, ex_ref_t *, ent ) {
        if ( ent->ptr == NULL || ex_object_is_dead(ent) )
            ex_array_continue;

        if ( EX_REF_CAST(ex_object_t,ent)->name == _name )
            return ent;
    } ex_array_each_end
    return NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_find_entities_byname ( ex_ref_t *_self, strid_t _name, ex_array_t *_result ) {
    ex_world_t *self = EX_REF_CAST(ex_world_t,_self);

    ex_array_each ( self->entities, ex_ref_t *, ent ) {
        if ( ent->ptr == NULL || ex_object_is_dead(ent) )
            ex_array_continue;

        if ( EX_REF_CAST(ex_object_t,ent)->name == _name )
            ex_array_append( _result, &ent );
    } ex_array_each_end
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __tick_engine_time ();
// ------------------------------------------------------------------ 

void ex_world_update ( ex_ref_t *_self ) {
    ex_world_t *self = EX_REF_CAST(ex_world_t,_self);

    // the self is stopped, don't do anything, even the timer tick
    if ( self->state == EX_WORLD_STATE_STOPPED )
        return;

    __tick_engine_time ();

    // the self is paused, only timer ticks 
    if ( self->state == EX_WORLD_STATE_PAUSED )
        return;

    // TODO: __handle_input();

    // update behavior
    ex_array_each ( self->entities, ex_ref_t *, ent ) {
        if ( ent->ptr == NULL || ex_object_is_dead(ent) )
            ex_array_continue;

        ex_entity_update_behaviors(ent);
    } ex_array_each_end

    // TODO: __update_animation(self);
    // TODO: __update_physics(self);

    // post-update behavior
    ex_array_each ( self->entities, ex_ref_t *, ent ) {
        if ( ent->ptr == NULL || ex_object_is_dead(ent) )
            ex_array_continue;

        ex_entity_post_update_behaviors(ent);
    } ex_array_each_end

    // 
    ex_invoke_mng_process(&self->invoke_mng);

    // at the end of the update, we do a garbage collection
    ex_array_each ( self->entities, ex_ref_t *, ent ) {
        if ( ent->ptr == NULL || ex_object_is_dead(ent) ) {
            ex_array_remove_at_fast( self->entities, __idx__ );
            --__idx__;
        }
    } ex_array_each_end

    //
    // ex_lua_gc( ex_lua_main_state() ); // DISABLE: we let lua do the gc automatically 
    ex_object_gc();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_render ( ex_ref_t *_self ) {
    ex_world_t *self = EX_REF_CAST(ex_world_t,_self);

    ex_array_each ( self->cameras, ex_ref_t *, cam ) {
        ex_camera_apply (cam);
        // TODO: __render_scene(self);
        __debug_draw(self); // TEMP

        // invoke all on_render event in behavior
        ex_array_each ( self->entities, ex_ref_t *, ref ) {
            ex_entity_on_render(ref);
        } ex_array_each_end;
    } ex_array_each_end;
}

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

void ex_world_run ( ex_ref_t *_self ) {
    ex_world_t *self = EX_REF_CAST(ex_world_t,_self);

    if ( self->state != EX_WORLD_STATE_STOPPED )
        return;

    // start all behavior
    ex_array_each ( self->entities, ex_ref_t *, ent ) {
        ex_entity_awake_behaviors(ent);
    } ex_array_each_end

    //
    self->state = EX_WORLD_STATE_RUNNING;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_stop ( ex_ref_t *_self ) {
    ex_world_t *self = EX_REF_CAST(ex_world_t,_self);

    if ( self->state == EX_WORLD_STATE_STOPPED )
        return;
    self->state = EX_WORLD_STATE_STOPPED;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_pause ( ex_ref_t *_self ) {
    ex_world_t *self = EX_REF_CAST(ex_world_t,_self);

    if ( self->state != EX_WORLD_STATE_RUNNING )
        return;
    self->state = EX_WORLD_STATE_PAUSED;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_resume ( ex_ref_t *_self ) {
    ex_world_t *self = EX_REF_CAST(ex_world_t,_self);

    if ( self->state != EX_WORLD_STATE_PAUSED )
        return;
    self->state = EX_WORLD_STATE_RUNNING;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_world_is_running ( ex_ref_t *_self ) {
    ex_world_t *self = EX_REF_CAST(ex_world_t,_self);
    return self->state == EX_WORLD_STATE_RUNNING; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_world_is_paused ( ex_ref_t *_self ) {
    ex_world_t *self = EX_REF_CAST(ex_world_t,_self);
    return self->state == EX_WORLD_STATE_PAUSED; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_world_is_stopped ( ex_ref_t *_self ) {
    ex_world_t *self = EX_REF_CAST(ex_world_t,_self);
    return self->state == EX_WORLD_STATE_STOPPED; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_add_invoke_to_call ( ex_ref_t *_self, ex_invoke_params_t *_params ) {
    ex_world_t *self = EX_REF_CAST(ex_world_t,_self);
    ex_invoke_mng_add_to_call( &self->invoke_mng, _params );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_add_invoke_to_stop ( ex_ref_t *_self, ex_invoke_params_t *_params ) {
    ex_world_t *self = EX_REF_CAST(ex_world_t,_self);
    ex_invoke_mng_add_to_stop( &self->invoke_mng, _params );
}
