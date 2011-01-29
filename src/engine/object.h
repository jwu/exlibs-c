// ======================================================================================
// File         : object.h
// Author       : Wu Jie 
// Last Change  : 01/28/2011 | 14:06:07 PM | Friday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef OBJECT_H_1296194768
#define OBJECT_H_1296194768
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "ref.h"

///////////////////////////////////////////////////////////////////////////////
// struct
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
/*! 
 @struct ex_object_t
 @details
*/// ------------------------------------------------------------------ 

EX_DECL_CLASS_BEGIN(ex_object_t)
    ex_uid_t uid;
    strid_t name;

    // override functions
    void (*init) ( void *_self ); // invoked when object created.
    void (*deinit) ( void *_self ); // invoked when ex_destroy_object called.
EX_DECL_CLASS_END(ex_object_t)

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DEF_OBJECT_BEGIN(_typename,_default_name,_init,_deinit) \
    strid_t __TYPEID_##_typename##__ = EX_STRID_NULL; \
    ex_rtti_t *__RTTI_##_typename##__ = NULL; \
    void *__ex_create_##_typename() { \
        void *__obj__ = ex_malloc(sizeof(_typename)); \
        ((ex_class_t *)__obj__)->rtti = EX_RTTI(_typename); \
        ((ex_object_t *)__obj__)->uid = EX_UID_INVALID; \
        ((ex_object_t *)__obj__)->name = ex_strid(_default_name); \
        ((ex_object_t *)__obj__)->init = _init; \
        ((ex_object_t *)__obj__)->deinit = _deinit;

#define EX_DEF_OBJECT_END \
        return __obj__; \
    }

///////////////////////////////////////////////////////////////////////////////
// function defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_init_object ( void *_obj );
extern void ex_destroy_object ( void *_obj );
extern ex_ref_t ex_get_objref ( ex_uid_t _uid );
extern void ex_object_gc ();

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline ex_uid_t ex_object_uid ( void *_obj ) { return ((ex_object_t *)_obj)->uid; }
static inline strid_t ex_object_name ( void *_obj ) { return ((ex_object_t *)_obj)->name; }

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 


// #################################################################################
#endif // END OBJECT_H_1296194768
// #################################################################################


