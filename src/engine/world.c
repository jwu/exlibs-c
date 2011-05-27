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

#include "component/lua_behavior.h"

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
    ex_world_t *self;
    
    self = EX_REF_CAST(ex_world_t,_self);

    //
    __object_init(_self);

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
    ex_world_t *self;

    self = EX_REF_CAST(ex_world_t,_self);
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
    ex_array_free ( self->be_list );

    //
    if ( __cur_world == _self ) {
        __cur_world = NULL;
    }

    __object_deinit(_self);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

EX_DEF_CLASS_BEGIN(ex_world_t)

    EX_OBJECT_DEFAULT_MEMBER

    EX_MEMBER( ex_object_t, name, ex_strid("World") )
    EX_MEMBER( ex_object_t, init, __world_init )
    EX_MEMBER( ex_object_t, deinit, __world_deinit )

    EX_MEMBER( ex_world_t, state, EX_WORLD_STATE_STOPPED )
    EX_MEMBER( ex_world_t, entities, ex_array_notype( sizeof(ex_ref_t *), 8 ) )
    EX_MEMBER( ex_world_t, cameras, ex_array_notype( sizeof(ex_ref_t *), 8 ) )
    EX_MEMBER( ex_world_t, main_camera, NULL )
    EX_MEMBER( ex_world_t, be_list, ex_array_notype( sizeof(ex_ref_t *), 1024 ) )

EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_world_t)
EX_DEF_PROPS_END

EX_SERIALIZE_SUPER_BEGIN(ex_world_t,ex_object_t)
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

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __prepare_update_list ( ex_world_t *_world ) {
    // clear all list first
    ex_array_remove_all( _world->be_list );

    //
    ex_array_each ( _world->entities, ex_ref_t *, entref ) {
        if ( entref->ptr == NULL || ex_object_is_dead(entref) ) {
            ex_array_remove_at_fast( _world->entities, __idx__ );
            --__idx__;
        }
        else {
            ex_entity_t *ent;

            ent = EX_REF_CAST(ex_entity_t,entref);
            ex_array_each ( ent->comps, ex_ref_t *, compref ) {
                ex_behavior_t *be;
                
                // add behavior to the be_list
                be = EX_REF_AS(ex_behavior_t,compref);
                if ( be ) {
                    ex_array_append( _world->be_list, &compref );
                }

                // TODO: add animation...
                // TODO: add physics...
                // TODO: add renderer...
            } ex_array_each_end
        }
    } ex_array_each_end
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __update_behaviors ( ex_world_t *_world ) {
    ex_array_each ( _world->be_list, ex_ref_t *, beref ) {
        ex_behavior_t *be;
        ex_ref_t *entref;

        //
        be = EX_REF_CAST(ex_behavior_t,beref);

        // NOTE: it is possible that we remove behaviors in other behaivor script.
        if ( be == NULL ) {
            ex_array_remove_at_fast( _world->be_list, __idx__ );
            continue;
        }

        // NOTE: it is possible the entity was dead.
        entref = ((ex_component_t *)be)->entity;
        if ( entref->ptr == NULL || ex_object_is_dead(entref) ) {
            ex_array_remove_at_fast( _world->entities, __idx__ );
            continue;
        }

        // do start/update
        if ( be->state == EX_BEHAVIOR_STATE_NEW ) {
            if ( be->start )
                be->start(beref);
            // DISABLE: we do this in __post_update_behaviors so that the update & post_update will exec after start 
            // be->state = EX_BEHAVIOR_STATE_STARTED;
        }
        else if ( be->update && be->enabled ) {
            be->update(beref);
        }
    } ex_array_each_end
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __post_update_behaviors ( ex_world_t *_world ) {
    ex_array_each ( _world->be_list, ex_ref_t *, beref ) {
        ex_behavior_t *be;
        ex_ref_t *entref;

        //
        be = EX_REF_CAST(ex_behavior_t,beref);

        // NOTE: it is possible that we remove behaviors in other behaivor script.
        if ( be == NULL ) {
            ex_array_remove_at_fast( _world->be_list, __idx__ );
            continue;
        }

        // NOTE: it is possible the entity was dead.
        entref = ((ex_component_t *)be)->entity;
        if ( entref->ptr == NULL || ex_object_is_dead(entref) ) {
            ex_array_remove_at_fast( _world->entities, __idx__ );
            continue;
        }

        // do post_update
        if ( be->state == EX_BEHAVIOR_STATE_NEW ) {
            be->state = EX_BEHAVIOR_STATE_STARTED;
        }
        else if ( be->post_update && be->enabled ) {
            be->post_update(beref);
        }
    } ex_array_each_end
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __on_render_behaviors ( ex_world_t *_world ) {
    ex_array_each ( _world->be_list, ex_ref_t *, beref ) {
        ex_behavior_t *be;
        ex_ref_t *entref;

        //
        be = EX_REF_CAST(ex_behavior_t,beref);

        // NOTE: it is possible that we remove behaviors in other behaivor script.
        if ( be == NULL ) {
            ex_array_remove_at_fast( _world->be_list, __idx__ );
            continue;
        }

        // NOTE: it is possible the entity was dead.
        entref = ((ex_component_t *)be)->entity;
        if ( entref->ptr == NULL || ex_object_is_dead(entref) ) {
            ex_array_remove_at_fast( _world->entities, __idx__ );
            continue;
        }

        // do post_update
        if ( be->on_render ) {
            be->on_render(beref);
        }
    } ex_array_each_end
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __invoke_behaviors ( ex_world_t *_world ) {
    ex_array_each ( _world->be_list, ex_ref_t *, beref ) {
        ex_behavior_t *be;
        ex_ref_t *entref;

        //
        be = EX_REF_CAST(ex_behavior_t,beref);

        // NOTE: it is possible that we remove behaviors in other behaivor script.
        if ( be == NULL ) {
            ex_array_remove_at_fast( _world->be_list, __idx__ );
            continue;
        }

        // NOTE: it is possible the entity was dead.
        entref = ((ex_component_t *)be)->entity;
        if ( entref->ptr == NULL || ex_object_is_dead(entref) ) {
            ex_array_remove_at_fast( _world->entities, __idx__ );
            continue;
        }

        // call,stop invokes
        ex_lua_behavior_process_invokes(beref);
    } ex_array_each_end
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __resume_co_behaviors ( ex_world_t *_world ) {
    ex_array_each ( _world->be_list, ex_ref_t *, beref ) {
        ex_behavior_t *be;
        ex_ref_t *entref;

        //
        be = EX_REF_CAST(ex_behavior_t,beref);

        // NOTE: it is possible that we remove behaviors in other behaivor script.
        if ( be == NULL ) {
            ex_array_remove_at_fast( _world->be_list, __idx__ );
            continue;
        }

        // NOTE: it is possible the entity was dead.
        entref = ((ex_component_t *)be)->entity;
        if ( entref->ptr == NULL || ex_object_is_dead(entref) ) {
            ex_array_remove_at_fast( _world->entities, __idx__ );
            continue;
        }

        // resume coroutine 
        ex_lua_behavior_process_coroutines(beref);
    } ex_array_each_end
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __resume_co_eof_behaviors ( ex_world_t *_world ) {
    ex_array_each ( _world->be_list, ex_ref_t *, beref ) {
        ex_behavior_t *be;
        ex_ref_t *entref;

        //
        be = EX_REF_CAST(ex_behavior_t,beref);

        // NOTE: it is possible that we remove behaviors in other behaivor script.
        if ( be == NULL ) {
            ex_array_remove_at_fast( _world->be_list, __idx__ );
            continue;
        }

        // NOTE: it is possible the entity was dead.
        entref = ((ex_component_t *)be)->entity;
        if ( entref->ptr == NULL || ex_object_is_dead(entref) ) {
            ex_array_remove_at_fast( _world->entities, __idx__ );
            continue;
        }

        // resume coroutine end_of_frame
        ex_lua_behavior_process_coroutines_eof(beref);
    } ex_array_each_end
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
    ex_world_t *self;
    ex_ref_t *ref;
    
    self = EX_REF_CAST(ex_world_t,_self);
    ref = ex_create_object( EX_RTTI(ex_entity_t), ex_generate_uid() );

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
    ex_ref_t *ent;

    ent = ex_world_create_entity( _self, _name );
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
    ex_array_remove_all ( self->be_list );
    ex_object_gc();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_add_camera ( ex_ref_t *_self, ex_ref_t *_camref ) {
    ex_world_t *self;
    
    self = EX_REF_CAST(ex_world_t,_self);
    ex_array_append( self->cameras, &_camref );
    ex_incref(_camref);
    if ( ex_array_count(self->cameras) == 1 ) {
        self->main_camera = _camref;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_remove_camera ( ex_ref_t *_self, ex_ref_t *_camref ) {
    ex_world_t *self;
    int result_idx = -1;

    self = EX_REF_CAST(ex_world_t,_self);
    ex_array_each ( self->cameras, ex_ref_t *, cam ) {
        if ( cam == _camref ) {
            result_idx = __idx__;
            break;
        }
    } ex_array_each_end;

    //
    if ( result_idx != -1 ) {
        ex_array_remove_at_fast( self->cameras, result_idx );
        ex_decref(_camref);
    }
    else {
        ex_component_t *comp;
        
        comp = EX_REF_CAST( ex_component_t, _camref );
        ex_warning ( "can't find camera %s", ex_strid_to_cstr( ex_object_name( comp->entity ) ) );
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
    ex_world_t *self;
    
    self = EX_REF_CAST(ex_world_t,_self);

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
    ex_world_t *self;
    
    self = EX_REF_CAST(ex_world_t,_self);

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
    ex_world_t *self;
    
    self = EX_REF_CAST(ex_world_t,_self);

    // the self is stopped, don't do anything, even the timer tick
    if ( self->state == EX_WORLD_STATE_STOPPED )
        return;

    // the self is paused, only timer ticks 
    if ( self->state == EX_WORLD_STATE_PAUSED )
        return;

    __tick_engine_time ();

    // prepare update list
    __prepare_update_list ( self );

    // TODO: __handle_input();

    // update behavior
    __update_behaviors ( self );

    // TODO: __update_animation(self);
    // TODO: __update_physics(self);

    // post-update behavior
    __post_update_behaviors ( self );

    // stop/call invokes
    __invoke_behaviors ( self );

    // resume coroutines
    __resume_co_behaviors ( self );

    // at the end of the update, we do a garbage collection
    ex_array_each ( self->entities, ex_ref_t *, entref ) {
        if ( entref->ptr == NULL || ex_object_is_dead(entref) ) {
            ex_array_remove_at_fast( self->entities, __idx__ );
            --__idx__;
        }
    } ex_array_each_end

    //
    ex_lua_gc( ex_lua_main_state() ); // NOTE: this makes frame-rate stable in Windows
    ex_object_gc();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_render ( ex_ref_t *_self ) {
    ex_world_t *self;
    
    self = EX_REF_CAST(ex_world_t,_self);

    ex_array_each ( self->cameras, ex_ref_t *, cam ) {
        ex_camera_clear (cam);
        ex_camera_apply_transform (cam);

        // invoke all on_render event in behavior
        __on_render_behaviors (self);

        // TODO: __render_scene(self);
        __debug_draw(self); // TEMP
    } ex_array_each_end;

    // before buffer swapped resume coroutine yield by "end_of_frame"
    __resume_co_eof_behaviors ( self );
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
    ex_world_t *self;
    
    self = EX_REF_CAST(ex_world_t,_self);

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
    ex_world_t *self;
    
    self = EX_REF_CAST(ex_world_t,_self);

    if ( self->state == EX_WORLD_STATE_STOPPED )
        return;
    self->state = EX_WORLD_STATE_STOPPED;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_pause ( ex_ref_t *_self ) {
    ex_world_t *self;
    
    self = EX_REF_CAST(ex_world_t,_self);

    if ( self->state != EX_WORLD_STATE_RUNNING )
        return;
    self->state = EX_WORLD_STATE_PAUSED;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_resume ( ex_ref_t *_self ) {
    ex_world_t *self;
    
    self = EX_REF_CAST(ex_world_t,_self);

    if ( self->state != EX_WORLD_STATE_PAUSED )
        return;
    self->state = EX_WORLD_STATE_RUNNING;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_world_is_running ( ex_ref_t *_self ) {
    ex_world_t *self;
    
    self = EX_REF_CAST(ex_world_t,_self);
    return self->state == EX_WORLD_STATE_RUNNING; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_world_is_paused ( ex_ref_t *_self ) {
    ex_world_t *self;
    
    self = EX_REF_CAST(ex_world_t,_self);
    return self->state == EX_WORLD_STATE_PAUSED; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_world_is_stopped ( ex_ref_t *_self ) {
    ex_world_t *self;
    
    self = EX_REF_CAST(ex_world_t,_self);
    return self->state == EX_WORLD_STATE_STOPPED; 
}
