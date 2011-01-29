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
    ex_array_t *entities;
    ex_array_t *cameras;
    struct ex_camera_t *mainCamera;
EX_DECL_CLASS_SUPER_END(ex_world_t,ex_object_t)

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_world_init ( ex_world_t *_world );
extern void ex_world_deinit ( ex_world_t *_world );
extern void ex_world_reset ( ex_world_t *_world, ex_stream_t *_stream );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern struct ex_entity_t *ex_world_create_entity ( ex_world_t *_world, strid_t _name );
extern struct ex_entity_t *ex_world_create_camera2d ( ex_world_t *_world, strid_t _name );
extern struct ex_entity_t *ex_world_create_rect ( ex_world_t *_world, strid_t _name );

// ------------------------------------------------------------------ 
// Desc: 
typedef struct ex_camera_t *ex_camera_ptr_t; // forward declaration
// ------------------------------------------------------------------ 

extern void ex_world_add_camera ( ex_world_t *_world, ex_camera_ptr_t _cam );
extern void ex_world_remove_camera ( ex_world_t *_world, ex_camera_ptr_t _cam );
extern struct ex_camera_t *ex_world_main_camera ( ex_world_t *_world );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_world_update ( ex_world_t *_world );
extern void ex_world_render ( ex_world_t *_world );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_world_run ( ex_world_t *_world );
extern void ex_world_stop ( ex_world_t *_world );
extern void ex_world_pause ( ex_world_t *_world );
extern void ex_world_resume ( ex_world_t *_world );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 


// #################################################################################
#endif // END WORLD_H_1291195601
// #################################################################################


