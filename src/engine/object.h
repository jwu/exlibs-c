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
#include "../core/misc/flags.h"

///////////////////////////////////////////////////////////////////////////////
// struct
///////////////////////////////////////////////////////////////////////////////

#define EX_OBJECT_NONE      0x00000000      
#define EX_OBJECT_DEAD      0x00000001      

// ------------------------------------------------------------------ 
/*! 
 @struct ex_object_t
 @details
*/// ------------------------------------------------------------------ 

EX_DECL_CLASS_BEGIN(ex_object_t)
    ex_uid_t uid; 
    strid_t name;
    uint32 flags;

    // override functions
    void (*init) ( ex_ref_t *_self ); // invoked when object created.
    void (*deinit) ( ex_ref_t *_self ); // invoked when ex_destroy_object called.
EX_DECL_CLASS_END(ex_object_t)

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DEF_OBJECT_BEGIN(_ctype,_default_name,_init,_deinit) \
    strid_t __TYPEID_##_ctype##__ = EX_STRID_NULL; \
    ex_rtti_t *__RTTI_##_ctype##__ = NULL; \
    void *__ex_create_##_ctype() { \
        void *__obj__ = ex_malloc(sizeof(_ctype)); \
        ((ex_class_t *)__obj__)->rtti = EX_RTTI(_ctype); \
        ((ex_object_t *)__obj__)->uid = EX_UID_INVALID; \
        ((ex_object_t *)__obj__)->name = ex_strid(_default_name); \
        ((ex_object_t *)__obj__)->flags = EX_OBJECT_NONE; \
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

extern ex_ref_t *ex_create_object ( const ex_rtti_t *_rtti, ex_uid_t _uid );
extern void ex_destroy_object ( ex_ref_t *_ref );
extern void ex_destroy_object_immediately ( ex_ref_t *_ref, bool _no_error );

extern ex_ref_t *ex_getref ( ex_uid_t _uid );

extern void ex_object_gc ();

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_serialize_objects ( struct ex_stream_t *_stream );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline ex_uid_t ex_object_uid ( ex_ref_t *_self ) { return ((ex_object_t *)_self->ptr)->uid; }
static inline strid_t ex_object_name ( ex_ref_t *_self ) { return ((ex_object_t *)_self->ptr)->name; }

static inline uint32 ex_object_flags ( ex_ref_t *_self ) { return ((ex_object_t *)_self->ptr)->flags; }
static inline bool ex_object_is_dead ( ex_ref_t *_self ) { 
    return ex_flags_has ( ex_object_flags(_self), EX_OBJECT_DEAD );
}

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 


// #################################################################################
#endif // END OBJECT_H_1296194768
// #################################################################################


