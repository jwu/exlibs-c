// ======================================================================================
// File         : clutter_world_render.c
// Author       : Wu Jie 
// Last Change  : 05/06/2011 | 17:41:47 PM | Friday,May
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "engine/engine_inc.h"

#include <cogl/cogl.h>
#include "view2d.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __setup_camera ( ex_ref_t *_camref ) {
    ex_camera_t *cam;
    float rx, ry;
    CoglMatrix projection;
    CoglMatrix modelview;

    //
    cam = EX_REF_CAST(ex_camera_t,_camref);
    rx = cam->orthoSize * cam->aspect; // half viewport width
    ry = cam->orthoSize; // half viewport height

    // setup projection matrix
    cogl_matrix_init_identity (&projection);
    cogl_set_projection_matrix (&projection);
    cogl_ortho (-rx, rx, -ry, ry, -100.0f, 100.0f);
    cogl_translate(0.5f, 0.5f, 0.0f);

    // setup model view matrix
    cogl_matrix_init_identity (&modelview);
    cogl_set_modelview_matrix (&modelview);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __render_entities ( ex_ref_t *_world ) {
    ex_world_t *world;
    world = EX_REF_CAST(ex_world_t,_world);

    ex_array_each ( world->entities, ex_ref_t *, entref ) {
        ex_ref_t *trans2d;
        ex_vec2f_t wpos;

        if ( ex_entity_get_comp(entref, EX_TYPEID(ex_camera_t)) ) {
            ex_ref_t *camref = ex_entity_get_comp(entref, EX_TYPEID(ex_camera_t));
            ex_camera_t *cam = EX_REF_CAST(ex_camera_t,camref);
            float rx = cam->orthoSize * cam->aspect; // half viewport width
            float ry = cam->orthoSize; // half viewport height

            cogl_set_source_color4ub ( 0x3f, 0x3f, 0x3f, 0xff );
            cogl_path_line_to ( -rx, -ry);
            cogl_path_line_to ( rx, ry);
            cogl_path_stroke ();
            cogl_path_line_to ( -rx, ry);
            cogl_path_line_to ( rx, -ry);
            cogl_path_stroke ();

            ex_array_continue;
        }

        trans2d = ex_entity_get_comp( entref, EX_TYPEID(ex_trans2d_t) );
        ex_trans2d_world_position ( trans2d, &wpos );

        cogl_set_source_color4ub ( 0xff, 0, 0, 0x9f );
        // cogl_path_round_rectangle ( wpos.x - 10, wpos.y - 10, 
        //                             wpos.x + 10, wpos.y + 10, 5, 15 );
        // cogl_path_stroke ();
        cogl_rectangle ( wpos.x - 10, wpos.y - 10, wpos.x + 10, wpos.y + 10 );
    } ex_array_each_end;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void clutter_render_world ( ex_ref_t *_world ) {
    //
    __setup_camera ( ex_world_main_camera (_world) );
    __render_entities (_world);

    // //
    // cogl_set_source_color4ub (0xff, 0, 0, 0x9f);
    // cogl_rectangle (0, 0, 100, 100);

    // cogl_set_source_color4ub (0, 0xff, 0, 0x9f);
    // cogl_rectangle ( -20, -20, -20 + 40, -20 + 40 );
}
