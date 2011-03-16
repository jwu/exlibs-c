// ======================================================================================
// File         : class.h
// Author       : Wu Jie 
// Last Change  : 11/30/2010 | 14:23:02 PM | Tuesday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef CLASS_H_1291098183
#define CLASS_H_1291098183
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "rtti.h"

///////////////////////////////////////////////////////////////////////////////
// struct
///////////////////////////////////////////////////////////////////////////////

//
typedef struct ex_class_t {
    const ex_rtti_t *rtti;
} ex_class_t;
extern strid_t __TYPEID_ex_class_t__;
extern ex_rtti_t *__RTTI_ex_class_t__;
extern void *__ex_create_ex_class_t( const ex_rtti_t * );

//
static inline void __ex_register_class_ex_class_t ( strid_t _typeID ) { 
    __TYPEID_ex_class_t__ = _typeID;
    __RTTI_ex_class_t__ = ex_rtti_register_class ( __TYPEID_ex_class_t__,
                                                   NULL,
                                                   sizeof(ex_class_t),
                                                   __ex_create_ex_class_t,
                                                   NULL,
                                                   NULL
                                                  );
}

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline const ex_rtti_t *ex_rtti_info ( void *_obj ) {
    return ((ex_class_t *)_obj)->rtti;
}

// ------------------------------------------------------------------ 
// Desc: helper function for using ex_instanceof
// ------------------------------------------------------------------ 

static inline bool __ex_instanceof ( const ex_class_t *_obj, const ex_rtti_t *_rtti ) {
    return ex_rtti_instanceof( _obj->rtti, _rtti );
}

// ------------------------------------------------------------------ 
// Desc: helper function for using ex_childof
// ------------------------------------------------------------------ 

static inline bool __ex_childof ( const ex_class_t *_obj, const ex_rtti_t *_rtti ) {
    return ex_rtti_childof( _obj->rtti, _rtti );
}

// ------------------------------------------------------------------ 
// Desc: helper function for using ex_superof
// ------------------------------------------------------------------ 

static inline bool __ex_superof ( const ex_class_t *_obj, const ex_rtti_t *_rtti ) {
    return ex_rtti_superof( _obj->rtti, _rtti );
}

// ------------------------------------------------------------------ 
// Desc: helper function for using ex_isa
// ------------------------------------------------------------------ 

static inline bool __ex_isa ( const ex_class_t *_obj, const ex_rtti_t *_rtti ) {
    return ex_rtti_isa( _obj->rtti, _rtti );
}

// ------------------------------------------------------------------ 
// Desc: helper function for using ex_as
// ------------------------------------------------------------------ 

extern void *__ex_cast ( ex_class_t *_obj, const ex_rtti_t *_rtti, bool _no_error );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END CLASS_H_1291098183
// #################################################################################
