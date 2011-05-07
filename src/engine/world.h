// ======================================================================================
// File         : world.h
// Author       : Wu Jie 
// Last Change  : 12/01/2010 | 17:28:04 PM | Wednesday,December
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef WORLD_H_1291195601
#define WORLD_H_1291195601
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "object.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

#define EX_WORLD_STATE_STOPPED 1
#define EX_WORLD_STATE_RUNNING 2
#define EX_WORLD_STATE_PAUSED  3

EX_DECL_CLASS_SUPER_BEGIN(ex_world_t,ex_object_t)
    int state;
    ex_array_t *entities; // array<entity_ref>
    ex_array_t *cameras; // array<camera_ref>
    ex_ref_t *main_camera; // camera_ref

    ex_array_t *be_list; // array<be_ref>
    // ex_array_t *anim_list; // array<animation>
    // ex_array_t *phys_list; // array<physics>
EX_DECL_CLASS_SUPER_END(ex_world_t,ex_object_t)

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern ex_ref_t *ex_current_world ();
extern void ex_set_current_world ( ex_ref_t *_world );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern ex_ref_t *ex_world_create_entity ( ex_ref_t *_self, strid_t _name );
extern ex_ref_t *ex_world_create_camera2d ( ex_ref_t *_self, strid_t _name );
extern ex_ref_t *ex_world_create_rect ( ex_ref_t *_self, strid_t _name );

extern void ex_world_clear ( ex_ref_t *_self );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// TODO: should this in camera_component ??? { 
extern void ex_world_add_camera ( ex_ref_t *_self, ex_ref_t *_camref );
extern void ex_world_remove_camera ( ex_ref_t *_self, ex_ref_t *_camref );
extern ex_ref_t *ex_world_main_camera ( ex_ref_t *_self );
// } TODO end 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern ex_ref_t *ex_world_find_entity_byname ( ex_ref_t *_self, strid_t _name );
extern void ex_world_find_entities_byname ( ex_ref_t *_self, strid_t _name, ex_array_t *_result );
// extern ex_ref_t *ex_world_find_entity_bytag ( ex_ref_t *_self, strid_t _name );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_world_update ( ex_ref_t *_self );
extern void ex_world_render ( ex_ref_t *_self );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_world_save ( ex_ref_t *_self, ex_stream_t *_stream );
extern ex_ref_t *ex_world_load ( ex_stream_t *_stream );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_world_run ( ex_ref_t *_self );
extern void ex_world_stop ( ex_ref_t *_self );
extern void ex_world_pause ( ex_ref_t *_self );
extern void ex_world_resume ( ex_ref_t *_self );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern bool ex_world_is_running ( ex_ref_t *_self );
extern bool ex_world_is_paused ( ex_ref_t *_self );
extern bool ex_world_is_stopped ( ex_ref_t *_self );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END WORLD_H_1291195601
// #################################################################################


