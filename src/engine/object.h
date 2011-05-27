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
    void *l; // lua state
    int luaRefID;

    // override functions
    void (*init) ( ex_ref_t *_self ); // invoked when object created.
    void (*deinit) ( ex_ref_t *_self ); // invoked when ex_destroy_object called.
EX_DECL_CLASS_END(ex_object_t)

#define EX_OBJECT_DEFAULT_MEMBER \
    EX_MEMBER(ex_object_t, uid, EX_UID_INVALID) \
    EX_MEMBER(ex_object_t, name, EX_STRID_NULL) \
    EX_MEMBER(ex_object_t, flags, EX_OBJECT_NONE) \
    EX_MEMBER(ex_object_t, l, NULL ) \
    EX_MEMBER(ex_object_t, luaRefID, -1 ) \
    EX_MEMBER(ex_object_t, init, __object_init) \
    EX_MEMBER(ex_object_t, deinit, __object_deinit)

extern void __object_init( ex_ref_t * );
extern void __object_deinit( ex_ref_t * );

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


