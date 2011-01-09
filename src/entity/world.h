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
// defines
///////////////////////////////////////////////////////////////////////////////

typedef struct ex_world_t {
    ex_array_t* _entities;
    ex_array_t* _cameras;
    struct ex_camera_t* _mainCamera;
} ex_world_t;

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern ex_world_t* ex_world_alloc ();
extern void ex_world_free ( ex_world_t* _world );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern struct ex_entity_t* ex_world_create_entity ( ex_world_t* _world, strid_t _name );
extern struct ex_entity_t* ex_world_create_camera2d ( ex_world_t* _world, strid_t _name );
extern struct ex_entity_t* ex_world_create_rect ( ex_world_t* _world, strid_t _name );

// ------------------------------------------------------------------ 
// Desc: 
typedef struct ex_camera_t* ex_camera_ptr_t; // forward declaration
// ------------------------------------------------------------------ 

extern void ex_world_add_camera ( ex_world_t* _world, ex_camera_ptr_t _cam );
extern void ex_world_remove_camera ( ex_world_t* _world, ex_camera_ptr_t _cam );
extern struct ex_camera_t* ex_world_main_camera ( ex_world_t* _world );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_world_update ( ex_world_t* _world );
extern void ex_world_render ( ex_world_t* _world );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 


// #################################################################################
#endif // END WORLD_H_1291195601
// #################################################################################


