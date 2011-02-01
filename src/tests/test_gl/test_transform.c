// ======================================================================================
// File         : test_transform.c
// Author       : Wu Jie 
// Last Change  : 02/01/2011 | 17:28:05 PM | Tuesday,February
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

static ex_ref_t *create_simple_entity ( const char *_name ) {
    ex_ref_t *ent = ex_world_create_entity ( g_world, ex_strid(_name) ); 

    // trans2d
    ex_ref_t *trans2d = ex_entity_add_comp( ent, EX_TYPEID(ex_trans2d_t) );
    ex_trans2d_set_world_position( trans2d, 0.0f, 0.0f );
    ex_trans2d_set_world_scale ( trans2d, 1.0f, 1.0f );
    ex_trans2d_set_world_rotation ( trans2d, 0.0f );

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

    return ent;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_transform () {
    ex_log ("create test transform...");

    ex_ref_t *ent1, *ent2;
    ex_ref_t *simple_ref;

    ent1 = create_simple_entity("entity_01");
    ent2 = create_simple_entity("entity_02");

    ex_trans2d_set_parent ( EX_REF_PTR(ex_entity_t,ent2)->trans2d, 
                            EX_REF_PTR(ex_entity_t,ent1)->trans2d );

    simple_ref = ex_entity_get_comp( ent1, EX_TYPEID(ex_simple_t) );
    EX_REF_PTR( ex_simple_t, simple_ref )->move_speed = 0.0f;
    EX_REF_PTR( ex_simple_t, simple_ref )->rot_speed = 0.0f;

    simple_ref = ex_entity_get_comp( ent2, EX_TYPEID(ex_simple_t) );
    EX_REF_PTR( ex_simple_t, simple_ref )->move_speed = 10.0f;
    EX_REF_PTR( ex_simple_t, simple_ref )->rot_speed = -10.0f;

    ex_log ("done!");
}
