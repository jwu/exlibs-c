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
// internal defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __debug_draw ( ex_world_t* _world ) {
    ex_array_each ( _world->entities, ex_entity_t*, ent ) {
        ex_debug2d_t* dbg2d = (ex_debug2d_t*)ex_entity_get_comp( ent, EX_CLASSID(ex_debug2d_t) );
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

ex_world_t* ex_world_alloc () {
    ex_world_t* world = (ex_world_t*)ex_malloc(sizeof(ex_world_t));
    world->entities = ex_array_alloc( sizeof(ex_entity_t*), 8 );
    world->cameras = ex_array_alloc( sizeof(ex_camera_t*), 8 );
    world->mainCamera = NULL;
    return world;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_free ( ex_world_t* _world ) {
    ex_array_each ( _world->entities, ex_entity_t*, ent ) {
        ex_entity_free(ent);
    } ex_array_each_end;
    ex_array_free ( _world->entities );
    ex_array_free ( _world->cameras );
    ex_free ( _world );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_entity_t* ex_world_create_entity ( ex_world_t* _world, strid_t _name ) {
    ex_entity_t* ent = ex_entity_alloc();
    ent->name = _name;
    ent->world = _world;
    ex_array_append( _world->entities, &ent );

    return ent;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_entity_t* ex_world_create_camera2d ( ex_world_t* _world, strid_t _name ) {
    ex_entity_t* ent = ex_world_create_entity( _world, _name );

    ex_entity_add_comp( ent, EX_CLASSID(ex_trans2d_t) );
    ex_camera_t* cam2d = (ex_camera_t*)ex_entity_add_comp( ent, EX_CLASSID(ex_camera_t) );
    ex_camera_set_ortho( cam2d, true );

    return ent;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_entity_t* ex_world_create_rect ( ex_world_t* _world, strid_t _name ) {
    // TODO:
    return NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_add_camera ( ex_world_t* _world, ex_camera_t* _cam ) {
    ex_array_append( _world->cameras, &_cam );
    if ( ex_array_count(_world->cameras) == 1 ) {
        _world->mainCamera = _cam;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_remove_camera ( ex_world_t* _world, ex_camera_t* _cam ) {
    int result_idx = -1;
    ex_array_each ( _world->cameras, ex_camera_t*, cam ) {
        if ( cam == _cam ) {
            result_idx = __idx__;
            break;
        }
    } ex_array_each_end;

    if ( result_idx != -1 )
        ex_array_remove_at_fast( _world->cameras, result_idx );
    else {
        ex_warning ( "can't find camera %s", ((ex_component_t*)_cam)->owner->name );
        return;
    }

    if ( ex_array_count(_world->cameras) == 1 ) {
        _world->mainCamera = *((ex_camera_t**)ex_array_get(_world->cameras,0));
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_camera_t* ex_world_main_camera ( ex_world_t* _world ) {
    return _world->mainCamera;
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __eng_time_tick ();
// ------------------------------------------------------------------ 

void ex_world_update ( ex_world_t* _world ) {
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

void ex_world_render ( ex_world_t* _world ) {
    ex_array_each ( _world->cameras, ex_camera_t*, cam ) {
        ex_camera_apply (cam);

        // TODO { 
        // __render_scene(_world);
        // } TODO end 
        __debug_draw(_world);
    } ex_array_each_end;
}
