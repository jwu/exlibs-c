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

#include "graphics/graphics_inc.h"
#include "engine/engine_inc.h"
#include "simple_behavior.h"

///////////////////////////////////////////////////////////////////////////////
// world
extern ex_ref_t *g_world;
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void init () {
    int i;

    EX_REGISTER_CLASS_2( ex_simple_t, ex.simple );

    ex_log ("create simple world...");
    ex_log ("create simple entities...");
    for ( i = 0; i < 1000; ++i ) {
        ex_ref_t *ent = ex_world_create_entity ( g_world, ex_strid("my_entity") ); 
        {
            ex_ref_t *trans2d, *dbg2d, *simple_ref;
            ex_simple_t *simple;

            // trans2d
            trans2d = ex_entity_add_comp( ent, EX_TYPEID(ex_trans2d_t) );
            ex_trans2d_set_local_position( trans2d, ex_range_randf(-400.0f,400.0f), ex_range_randf(-400.0f,400.0f) );
            ex_trans2d_set_local_scale ( trans2d, ex_range_randf(0.0f,1.0f), ex_range_randf(0.0f,1.0f) );
            ex_trans2d_set_local_angle ( trans2d, ex_range_randf(0.0f,EX_TWO_PI) );

            // dbg2d
            dbg2d = ex_entity_add_comp( ent, EX_TYPEID(ex_debug2d_t) );
            ex_debug2d_set_rect ( dbg2d, 0.0f, 0.0f, 100.0f, 100.0f );

            // simple
            simple_ref = ex_entity_add_comp_auto_awake( ent, EX_TYPEID(ex_simple_t) );
            simple = EX_REF_CAST(ex_simple_t,simple_ref);

            ex_vec2f_set ( &simple->move_dir, ex_range_randf(-1.0f,1.0f), ex_range_randf(-1.0f,1.0f) );
            ex_vec2f_normalize(&simple->move_dir);
            simple->move_speed = ex_range_randf(1.0f,100.0f);
            simple->rot_speed = ex_range_randf(-5.0f,5.0f);
            simple->rot_around = ROT_AROUND_SELF;
        }
    }
    ex_log ("done!");
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void deinit () {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void update () {
    ex_world_update(g_world);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void render () {
    ex_ref_t *mainCam;

    mainCam = ex_world_main_camera (g_world);
    ex_camera_clear (mainCam);
    ex_camera_apply_transform (mainCam);

    ex_world_render(g_world);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void on_keyboard ( uint8 _key ) {
    if ( _key == EX_KEY_d ) {
        ex_ref_t *ref = ex_world_find_entity_byname (g_world, ex_strid("my_entity") );
        if ( ref ) {
            void *ptr = ref->ptr;
            ex_destroy_object(ref);
            ex_log( "entity %s destroyed. uid: %llu", 
                    ex_strid_to_cstr( ((ex_object_t *)ptr)->name ),
                    ((ex_object_t *)ptr)->uid
                  );
        }
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void on_mouse ( int _x, int _y ) {
}
