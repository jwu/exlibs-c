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

#include "main.h"

///////////////////////////////////////////////////////////////////////////////
// world
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static ex_ref_t *create_simple_entity ( const char *_name ) {
    ex_ref_t *trans2d, *dbg2d, *simple_ref;
    ex_simple_t *simple;
    ex_ref_t *ent = ex_world_create_entity ( g_world, ex_strid(_name) ); 

    // trans2d
    trans2d = ex_entity_add_comp( ent, EX_TYPEID(ex_trans2d_t) );
    ex_trans2d_set_local_position( trans2d, 0.0f, 0.0f );
    ex_trans2d_set_local_scale ( trans2d, 1.0f, 1.0f );
    ex_trans2d_set_local_angle ( trans2d, 0.0f );

    // dbg2d
    dbg2d = ex_entity_add_comp( ent, EX_TYPEID(ex_debug2d_t) );
    ex_debug2d_set_rect ( dbg2d, 0.0f, 0.0f, 20.0f, 20.0f );

    // simple
    simple_ref = ex_entity_add_comp_auto_awake( ent, EX_TYPEID(ex_simple_t) );
    simple = EX_REF_CAST(ex_simple_t,simple_ref);

    ex_vec2f_set ( &simple->move_dir, ex_range_randf(-1.0f,1.0f), ex_range_randf(-1.0f,1.0f) );
    ex_vec2f_normalize(&simple->move_dir);
    simple->move_speed = 0.0f;
    simple->rot_speed = 0.0f;

    return ent;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void init () {
    ex_ref_t *ent1, *ent2, *ent3;
    ex_ref_t *simple_ref;
    ex_ref_t *trans2d_ref;
    int i;

    ex_log ("create test transform...");

#if 1
    ent1 = create_simple_entity("entity_01");
    for ( i = 0; i < 2; ++i ) {
        ent2 = create_simple_entity("entity_02");
        ent3 = create_simple_entity("entity_03");

        ex_trans2d_set_parent ( EX_REF_CAST(ex_entity_t,ent2)->trans2d, 
                                EX_REF_CAST(ex_entity_t,ent1)->trans2d );

        ex_trans2d_set_parent ( EX_REF_CAST(ex_entity_t,ent3)->trans2d, 
                                EX_REF_CAST(ex_entity_t,ent2)->trans2d );

        simple_ref = ex_entity_get_comp( ent1, EX_TYPEID(ex_simple_t) );
        EX_REF_CAST( ex_simple_t, simple_ref )->move_speed = 0.0f;
        // EX_REF_CAST( ex_simple_t, simple_ref )->rot_speed = 1.0f;
        // EX_REF_CAST( ex_simple_t, simple_ref )->rot_around = ROT_AROUND_SELF;
        trans2d_ref = ex_entity_get_comp( ent1, EX_TYPEID(ex_trans2d_t) );
        // ex_trans2d_set_local_scale( trans2d_ref, 0.5f, 0.5f );

        simple_ref = ex_entity_get_comp( ent2, EX_TYPEID(ex_simple_t) );
        EX_REF_CAST( ex_simple_t, simple_ref )->move_speed = 0.0f;
        EX_REF_CAST( ex_simple_t, simple_ref )->rot_speed = 2.0f;
        EX_REF_CAST( ex_simple_t, simple_ref )->rot_around = ROT_AROUND_PARENT;
        trans2d_ref = ex_entity_get_comp( ent2, EX_TYPEID(ex_trans2d_t) );
        // ex_trans2d_set_local_scale( trans2d_ref, 1.5f, 1.5f );
        if ( i == 0 ) {
            ex_trans2d_set_local_position( trans2d_ref, 160.0f, 0.0f );
        }
        else {
            ex_trans2d_set_local_position( trans2d_ref, -160.0f, 0.0f );
        }

        simple_ref = ex_entity_get_comp( ent3, EX_TYPEID(ex_simple_t) );
        EX_REF_CAST( ex_simple_t, simple_ref )->move_speed = 0.0f;
        // EX_REF_CAST( ex_simple_t, simple_ref )->rot_speed = -3.0f;
        // EX_REF_CAST( ex_simple_t, simple_ref )->rot_around = ROT_AROUND_PARENT;
        trans2d_ref = ex_entity_get_comp( ent3, EX_TYPEID(ex_trans2d_t) );
        // ex_trans2d_set_local_scale( trans2d_ref, 2.0f, 2.0f );
        if ( i == 0 ) {
            ex_trans2d_set_local_position( trans2d_ref, 0.0f, 80.0f );
        }
        else {
            ex_trans2d_set_local_position( trans2d_ref, 0.0f, -80.0f );
        }
    }
#else
    // test translate
    {
        ex_ref_t *e1 = create_simple_entity("foobar");
        ex_ref_t *e2 = create_simple_entity("foo");
        ex_ref_t *e3 = create_simple_entity("bar");
        ex_ref_t *e = NULL;

        ex_trans2d_set_parent ( EX_REF_CAST(ex_entity_t,e2)->trans2d, 
                                EX_REF_CAST(ex_entity_t,e1)->trans2d );

        ex_trans2d_set_parent ( EX_REF_CAST(ex_entity_t,e3)->trans2d, 
                                EX_REF_CAST(ex_entity_t,e2)->trans2d );

        trans2d_ref = ex_entity_get_comp( e1, EX_TYPEID(ex_trans2d_t) );
        ex_trans2d_set_local_angle ( trans2d_ref, EX_PI/4.0f );

        trans2d_ref = ex_entity_get_comp( e2, EX_TYPEID(ex_trans2d_t) );
        ex_trans2d_set_local_angle ( trans2d_ref, -EX_PI/8.0f );
        ex_trans2d_translate( trans2d_ref, 40.0f, 0.0f, EX_SPACE_SELF );

        trans2d_ref = ex_entity_get_comp( e3, EX_TYPEID(ex_trans2d_t) );
        ex_trans2d_set_local_angle ( trans2d_ref, -EX_PI/8.0f );
        ex_trans2d_translate( trans2d_ref, 80.0f, 0.0f, EX_SPACE_WORLD );
        // ex_trans2d_translate( trans2d_ref, 80.0f, 0.0f, EX_SPACE_SELF );

        // test entity get
        trans2d_ref = ex_entity_get_comp( e1, EX_TYPEID(ex_trans2d_t) );
        e = ex_trans2d_find( trans2d_ref, "foo/bar" );
        ex_assert ( e && 
                    EX_REF_CAST(ex_object_t,EX_REF_CAST(ex_component_t,e)->entity)->name == ex_strid("bar"),
                    "failed to find foo/bar from foobar" );
    }
#endif


    ex_log ("done!");
} 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void keyboard ( uint8 _key ) {
    if ( _key == EX_KEY_d ) {
        ex_ref_t *ref = ex_world_find_entity_byname (g_world, ex_strid("entity_03") );
        ex_ref_t *trans2d_ref = NULL;

        if ( ref ) {
            trans2d_ref = ex_entity_get_comp( ref, EX_TYPEID(ex_trans2d_t) );
            ex_trans2d_set_parent( trans2d_ref, NULL );
        }
    }
    else if ( _key == EX_KEY_a ) {
        ex_ref_t *ref = ex_world_find_entity_byname (g_world, ex_strid("entity_03") );
        ex_ref_t *ref2 = ex_world_find_entity_byname (g_world, ex_strid("entity_02") );
        ex_ref_t *trans2d_ref = NULL;
        ex_ref_t *trans2d_ref2 = NULL;

        if ( ref ) {
            trans2d_ref = ex_entity_get_comp( ref, EX_TYPEID(ex_trans2d_t) );
            trans2d_ref2 = ex_entity_get_comp( ref2, EX_TYPEID(ex_trans2d_t) );
            ex_trans2d_set_parent( trans2d_ref, trans2d_ref2 );
        }
    }
    else if ( _key == EX_KEY_s ) {
        ex_ref_t *ref = ex_world_find_entity_byname (g_world, ex_strid("entity_01") );
        ex_ref_t *trans2d_ref = NULL;

        if ( ref ) {
            trans2d_ref = ex_entity_get_comp( ref, EX_TYPEID(ex_trans2d_t) );
            ex_trans2d_detach_children( trans2d_ref );
        }
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_transform () {
    // register game classes
    EX_REGISTER_CLASS(ex_simple_t);

    g_game.init = init;
    g_game.update = NULL;
    g_game.render = NULL;
    g_game.keyboard = keyboard;
    g_game.mouse = NULL;
}

