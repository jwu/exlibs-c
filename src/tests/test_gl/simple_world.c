// ======================================================================================
// File         : simple_world.c
// Author       : Wu Jie 
// Last Change  : 02/01/2011 | 17:26:15 PM | Tuesday,February
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// externs
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

#include "../../graphics/graphics_inc.h"
#include "../../engine/engine_inc.h"
#include "simple_behavior.h"

extern ex_ref_t *g_world;

///////////////////////////////////////////////////////////////////////////////
// world
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void simple_world () {
    ex_log ("create simple world...");

    ex_log ("create simple entities...");
    for ( int i = 0; i < 100; ++i ) {
        ex_ref_t *ent = ex_world_create_entity ( g_world, ex_strid("my_entity") ); 
        {
            // trans2d
            ex_ref_t *trans2d = ex_entity_add_comp( ent, EX_TYPEID(ex_trans2d_t) );
            ex_trans2d_set_world_position( trans2d, ex_range_randf(-400.0f,400.0f), ex_range_randf(-400.0f,400.0f) );
            ex_trans2d_set_world_scale ( trans2d, ex_range_randf(0.0f,1.0f), ex_range_randf(0.0f,1.0f) );
            ex_trans2d_set_world_rotation ( trans2d, ex_range_randf(0.0f,EX_TWO_PI) );

            // dbg2d
            ex_ref_t *dbg2d = ex_entity_add_comp( ent, EX_TYPEID(ex_debug2d_t) );
            ex_debug2d_set_rect ( dbg2d, 0.0f, 0.0f, 100.0f, 100.0f );

            // simple
            ex_ref_t *simple_ref = ex_entity_add_comp( ent, EX_TYPEID(ex_simple_t) );
            ex_simple_t *simple = EX_REF_PTR(ex_simple_t,simple_ref);

            ex_vec2f_set ( &simple->move_dir, ex_range_randf(-1.0f,1.0f), ex_range_randf(-1.0f,1.0f) );
            ex_vec2f_normalize(&simple->move_dir);
            simple->move_speed = ex_range_randf(1.0f,100.0f);
            simple->rot_speed = ex_range_randf(-100.0f,100.0f);
        }
    }

    ex_log ("done!");
}



