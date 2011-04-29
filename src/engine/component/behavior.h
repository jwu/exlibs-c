// ======================================================================================
// File         : behavior.h
// Author       : Wu Jie 
// Last Change  : 01/27/2011 | 11:48:41 AM | Thursday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef BEHAVIOR_H_1296100122
#define BEHAVIOR_H_1296100122
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "component.h"
#include "../invoke_mng.h"
#include "../coroutine_mng.h"

///////////////////////////////////////////////////////////////////////////////
// macros
///////////////////////////////////////////////////////////////////////////////

#define EX_BEHAVIOR_STATE_NEW      1
#define EX_BEHAVIOR_STATE_STARTED  2
#define EX_BEHAVIOR_STATE_DEAD     3

// ------------------------------------------------------------------ 
/*! 
 @struct ex_behavior_t
 @details
*/// ------------------------------------------------------------------ 

EX_DECL_CLASS_SUPER_BEGIN(ex_behavior_t,ex_component_t)
    int state;
    bool enabled;
    ex_invoke_mng_t invoke_mng;
    ex_coroutine_mng_t coroutine_mng;

    // ======================================================== 
    // invokes 
    // ======================================================== 

    // invoked in ex_world_start start
    void (*awake) ( ex_ref_t *_self );

    // invoked at the next frame's update after the component added to the entity. 
    void (*start) ( ex_ref_t *_self );

    // invoked in ex_world_update if the behavior is stated.
    void (*update) ( ex_ref_t *_self );

    // invoked in ex_world_update after animation, phsyics update if the behavior is started.
    void (*post_update) ( ex_ref_t *_self );

    // ======================================================== 
    // events
    // ======================================================== 

    // invoked after camera has rendered the scene
    void (*on_render) ( ex_ref_t *_self );

EX_DECL_CLASS_SUPER_END(ex_behavior_t,ex_component_t)

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 


// #################################################################################
#endif // END BEHAVIOR_H_1296100122
// #################################################################################


