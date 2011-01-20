// ======================================================================================
// File         : component.h
// Author       : Wu Jie 
// Last Change  : 11/25/2010 | 17:25:33 PM | Thursday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef COMPONENT_H_1290677135
#define COMPONENT_H_1290677135
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// struct
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
/*! 
 @struct ex_component_t
 @details
*/// ------------------------------------------------------------------ 

EX_DEF_CLASS_BEGIN(ex_component_t)
    struct ex_entity_t *owner;

    // override functions
    void (*init) ( void *_self ); // invoked after the component created
    void (*deinit) ( void *_self ); // invoked before the component destroyed
    void (*start) ( void *_self ); // invoked when all the component in the entity been added.
    void (*update) ( void *_self );
    void (*post_update) ( void *_self );
EX_DEF_CLASS_END(ex_component_t)

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_component_init ( void *_self ); 
extern void ex_component_deinit ( void *_self ); 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern struct ex_entity_t *ex_component_owner ( const ex_component_t *_self ); 

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 


// #################################################################################
#endif // END COMPONENT_H_1290677135
// #################################################################################


