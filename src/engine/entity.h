// ======================================================================================
// File         : entity.h
// Author       : Wu Jie 
// Last Change  : 11/25/2010 | 17:16:24 PM | Thursday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef ENTITY_H_1290676586
#define ENTITY_H_1290676586
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
// struct
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
/*! 
 @struct ex_entity_t
 @details
*/// ------------------------------------------------------------------ 

EX_DECL_CLASS_SUPER_BEGIN(ex_entity_t,ex_object_t)
    // NOTE: we don't use "ex_hashmap_t* _type_to_comp" because we may get base_type of the component 
    ex_array_t *comps; // array<component_ref>
    ex_ref_t *world; // the world it belongs

    // cached components 
    ex_ref_t *trans2d;
    ex_ref_t *camera;
EX_DECL_CLASS_SUPER_END(ex_entity_t,ex_object_t)

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern ex_ref_t *ex_entity_get_comp ( const ex_ref_t *_self, strid_t _typeID );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern ex_ref_t *ex_entity_add_comp ( ex_ref_t *_self, strid_t _typeID );
extern ex_ref_t *ex_entity_add_comp_auto_awake ( ex_ref_t *_self, strid_t _typeID );
// TODO: extern int ex_entity_remove_comp ( ex_ref_t *_self, ex_ref_t *_comp );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_entity_awake_behaviors ( ex_ref_t *_self );
extern void ex_entity_update_behaviors ( ex_ref_t *_self );
extern void ex_entity_post_update_behaviors ( ex_ref_t *_self );
extern void ex_entity_on_render ( ex_ref_t *_self );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 


// #################################################################################
#endif // END ENTITY_H_1290676586
// #################################################################################


