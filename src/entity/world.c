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

#include "trans2d.h"
#include "debug2d.h"
#include "camera.h"

///////////////////////////////////////////////////////////////////////////////
// class define
///////////////////////////////////////////////////////////////////////////////

EX_DEF_CLASS_BEGIN(ex_world_t)
    EX_STRID_NULL, // name
    NULL, // entities
    NULL, // cameras
    NULL, // mainCamera
EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_world_t)
    EX_PROP( ex_world_t, name, "name",  EX_PROP_ATTR_NONE, ex_prop_set_raw_strid, ex_prop_get_raw_strid )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN(ex_world_t)
    int num_entities = 0;

    EX_MEMBER_SERIALIZE( strid, name )
    num_entities = self->entities->count;
    EX_SERIALIZE( _stream, int, "num_entities", &num_entities  )

    if ( _stream->type == EX_STREAM_READ ) {
        ex_entity_t *ent = NULL;
        for ( int i = 0; i < num_entities; ++i ) {
            ent = ex_create_ex_entity_t();
            ex_entity_init(ent);
            ent->world = self;
            ex_array_append( self->entities, &ent );
            EX_SERIALIZE( _stream, ex_entity_t, "entity", ent );
        }
    }
    else if ( _stream->type == EX_STREAM_WRITE ) {
        ex_array_each ( self->entities, ex_entity_t *, ent ) {
            EX_SERIALIZE( _stream, ex_entity_t, "entity", ent );
        } ex_array_each_end
    }  
EX_SERIALIZE_END

EX_DEF_TOSTRING_BEGIN(ex_world_t)
    EX_MEMBER_TOSTRING ( strid, "name", self->name )
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// internal defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __debug_draw ( ex_world_t *_world ) {
    ex_array_each ( _world->entities, ex_entity_t *, ent ) {
        ex_debug2d_t *dbg2d = (ex_debug2d_t *)ex_entity_get_comp( ent, EX_TYPEID(ex_debug2d_t) );
        if ( dbg2d ) {
            ex_debug2d_draw(dbg2d);
        }
    } ex_array_each_end;
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_init ( ex_world_t *_world ) {
    _world->entities = ex_array_notype( sizeof(void *), 8 );
    _world->cameras = ex_array_notype( sizeof(void *), 8 );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_deinit ( ex_world_t *_world ) {
    ex_array_each ( _world->entities, ex_entity_t *, ent ) {
        ex_entity_deinit(ent);
        ex_free(ent);
    } ex_array_each_end;
    ex_array_free ( _world->entities );
    ex_array_free ( _world->cameras );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_entity_t *ex_world_create_entity ( ex_world_t *_world, strid_t _name ) {
    ex_entity_t *ent = ex_create_ex_entity_t();
    ex_entity_init(ent);

    ent->name = _name;
    ent->world = _world;

    ex_array_append( _world->entities, &ent );

    return ent;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_entity_t *ex_world_create_camera2d ( ex_world_t *_world, strid_t _name ) {
    ex_entity_t *ent = ex_world_create_entity( _world, _name );

    ex_entity_add_comp( ent, EX_TYPEID(ex_trans2d_t) );
    ex_camera_t *cam2d = (ex_camera_t *)ex_entity_add_comp( ent, EX_TYPEID(ex_camera_t) );
    ex_camera_set_ortho( cam2d, true );

    return ent;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_entity_t *ex_world_create_rect ( ex_world_t *_world, strid_t _name ) {
    // TODO:
    return NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_add_camera ( ex_world_t *_world, ex_camera_t *_cam ) {
    ex_array_append( _world->cameras, &_cam );
    if ( ex_array_count(_world->cameras) == 1 ) {
        _world->mainCamera = _cam;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_remove_camera ( ex_world_t *_world, ex_camera_t *_cam ) {
    int result_idx = -1;
    ex_array_each ( _world->cameras, ex_camera_t *, cam ) {
        if ( cam == _cam ) {
            result_idx = __idx__;
            break;
        }
    } ex_array_each_end;

    if ( result_idx != -1 )
        ex_array_remove_at_fast( _world->cameras, result_idx );
    else {
        ex_warning ( "can't find camera %s", ((ex_component_t *)_cam)->owner->name );
        return;
    }

    if ( ex_array_count(_world->cameras) == 1 ) {
        _world->mainCamera = *((ex_camera_t **)ex_array_get(_world->cameras,0));
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_camera_t *ex_world_main_camera ( ex_world_t *_world ) {
    return _world->mainCamera;
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __eng_time_tick ();
// ------------------------------------------------------------------ 

void ex_world_update ( ex_world_t *_world ) {
    __eng_time_tick ();

    // TODO { 
    // __handle_input();
    // __update_ai(_world); // including scripts (or we call behaviors)
    // __update_animation(_world);
    // __update_physics(_world);
    // } TODO end 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_render ( ex_world_t *_world ) {
    ex_array_each ( _world->cameras, ex_camera_t *, cam ) {
        ex_camera_apply (cam);

        // TODO { 
        // __render_scene(_world);
        // } TODO end 
        __debug_draw(_world);
    } ex_array_each_end;
}
