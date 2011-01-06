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

    // _handle_input();
    // _update_ai();
    // ??? _update_animation();
    // _update_physics();
    // _render_scene();
}
