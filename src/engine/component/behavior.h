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

#define EX_BEHAVIOR_DEFAULT_MEMBER \
    EX_COMPONENT_DEFAULT_MEMBER \
    EX_MEMBER( ex_behavior_t, state, EX_BEHAVIOR_STATE_NEW ) \
    EX_MEMBER( ex_behavior_t, enabled, true ) \
    EX_MEMBER( ex_behavior_t, awake, NULL ) \
    EX_MEMBER( ex_behavior_t, start, NULL ) \
    EX_MEMBER( ex_behavior_t, update, NULL ) \
    EX_MEMBER( ex_behavior_t, post_update, NULL ) \
    EX_MEMBER( ex_behavior_t, on_render, NULL )

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


