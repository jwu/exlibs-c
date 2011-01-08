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

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __debug_draw ( ex_world_t* _world ) {
    ex_array_each ( _world->_entities, ex_entity_t*, ent ) {
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
    world->_entities = ex_array_alloc( sizeof(ex_entity_t*), 8 );
    return world;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_world_free ( ex_world_t* _world ) {
    ex_array_each ( _world->_entities, ex_entity_t*, ent ) {
        ex_entity_free(ent);
    } ex_array_each_end;
    ex_array_free ( _world->_entities );
    ex_free ( _world );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_entity_t* ex_world_create_entity ( ex_world_t* _world, strid_t _name ) {
    ex_entity_t* ent = ex_entity_alloc();
    ent->_name = _name;
    ex_array_append( _world->_entities, &ent );
    return ent;
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
    // __render_scene(_world);
    // } TODO end 
    __debug_draw(_world);
}
