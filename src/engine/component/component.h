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

EX_DECL_CLASS_BEGIN(ex_component_t)
    uint32 id; // a random value to identity component when reference.
    struct ex_entity_t *owner;
    bool active;

    // override functions
    void (*init) ( void *_self ); // invoked after the component created
    void (*deinit) ( void *_self ); // invoked before the component destroyed
EX_DECL_CLASS_END(ex_component_t)

// TODO { 
// EX_DECL_CLASS_BEGIN(ex_component_ref_t)
    // entity_id
    // component_id
// EX_DECL_CLASS_END(ex_component_ref_t)
// } TODO end 

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_component_init ( void *_self ); 
extern void ex_component_deinit ( void *_self ); 

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 


// #################################################################################
#endif // END COMPONENT_H_1290677135
// #################################################################################


