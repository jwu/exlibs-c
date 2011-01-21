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
    ex_rtti_t *rtti;
} ex_class_t;
extern strid_t __TYPEID_ex_class_t__;
extern ex_rtti_t *__RTTI_ex_class_t__;

//
static inline ex_class_t *__alloc_ex_class_t() {
    ex_class_t *obj = (ex_class_t *)ex_malloc(sizeof(ex_class_t));
    obj->rtti = __RTTI_ex_class_t__;
    return obj;
}

//
static inline void *ex_create_ex_class_t() {
    return __alloc_ex_class_t();
}

//
static inline void __ex_register_class_ex_class_t () { 
    __TYPEID_ex_class_t__ = ex_strid("ex_class_t"); \
    __RTTI_ex_class_t__ = ex_rtti_register_class ( __TYPEID_ex_class_t__,
                                                   NULL,
                                                   sizeof(ex_class_t),
                                                   ex_create_ex_class_t,
                                                   NULL
                                                  );
}

///////////////////////////////////////////////////////////////////////////////
// macros
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_RTTI(_typename) \
    (__RTTI_##_typename##__)

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_REGISTER_CLASS(_typename) \
    __ex_register_class_##_typename()

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DEF_CLASS_BEGIN(_typename) \
    typedef struct _typename { \
        const struct ex_class_t _;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DEF_CLASS_END(_typename) \
    } _typename; \
    extern strid_t __TYPEID_##_typename##__; /*for EX_TYPEID*/ \
    extern ex_rtti_t *__RTTI_##_typename##__; /*for EX_RTTI*/ \
    extern void *ex_create_##_typename(); \
    extern void __ex_register_properties_##_typename (); \
    extern void __ex_serialize_##_typename( struct ex_stream_t *, strid_t, void * ); \
    static inline void __ex_register_class_##_typename () { /*for EX_REGISTER_CLASS, define in EX_DEF_PROPS_BEGIN*/ \
        __TYPEID_##_typename##__ = ex_strid(#_typename); \
        __RTTI_##_typename##__ = ex_rtti_register_class ( __TYPEID_##_typename##__, \
                                                          EX_RTTI(ex_class_t), \
                                                          sizeof(_typename), \
                                                          ex_create_##_typename, \
                                                          __ex_serialize_##_typename \
                                                          ); \
        __ex_register_properties_##_typename(); \
    }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DEF_CLASS_SUPER_BEGIN(_typename,_super) \
    typedef struct _typename { \
        const struct _super _;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DEF_CLASS_SUPER_END(_typename,_super) \
    } _typename; \
    extern strid_t __TYPEID_##_typename##__; /*for EX_TYPEID*/ \
    extern ex_rtti_t *__RTTI_##_typename##__; /*for EX_RTTI*/ \
    extern void *ex_create_##_typename(); \
    extern void __ex_register_properties_##_typename (); \
    extern void __ex_serialize_##_typename( struct ex_stream_t *, strid_t, void * ); \
    static inline void __ex_register_class_##_typename () { /*for EX_REGISTER_CLASS, define in EX_DEF_PROPS_BEGIN*/ \
        __TYPEID_##_typename##__ = ex_strid(#_typename); \
        __RTTI_##_typename##__ = ex_rtti_register_class ( __TYPEID_##_typename##__, \
                                                          EX_RTTI(_super), \
                                                          sizeof(_typename), \
                                                          ex_create_##_typename, \
                                                          __ex_serialize_##_typename \
                                                          ); \
        __ex_register_properties_##_typename(); \
    }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DEF_CLASS_CREATOR(_typename) \
    static inline _typename *__alloc_##_typename() { \
        ex_class_t *obj = (ex_class_t *)ex_malloc(sizeof(_typename)); \
        obj->rtti = EX_RTTI(_typename); \
        return (_typename *)obj; \
    } \
    void *ex_create_##_typename()

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline ex_rtti_t *ex_rtti_info ( void *_obj ) {
    return ((ex_class_t *)_obj)->rtti;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define ex_classof(_type,_objPtr) __ex_classof((ex_class_t *)_objPtr,EX_RTTI(_type))
static inline bool __ex_classof ( const ex_class_t *_obj, const ex_rtti_t *_rtti ) {
    return ex_rtti_classof( _obj->rtti, _rtti );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define ex_childof(_type,_objPtr) __ex_childof((ex_class_t *)_objPtr,EX_RTTI(_type))
static inline bool __ex_childof ( const ex_class_t *_obj, const ex_rtti_t *_rtti ) {
    return ex_rtti_childof( _obj->rtti, _rtti );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define ex_superof(_type,_objPtr) __ex_superof((ex_class_t *)_objPtr,EX_RTTI(_type))
static inline bool __ex_superof ( const ex_class_t *_obj, const ex_rtti_t *_rtti ) {
    return ex_rtti_superof( _obj->rtti, _rtti );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define ex_isa(_type,_objPtr) __ex_isa((ex_class_t *)_objPtr,EX_RTTI(_type))
static inline bool __ex_isa ( const ex_class_t *_obj, const ex_rtti_t *_rtti ) {
    return ex_rtti_isa( _obj->rtti, _rtti );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define ex_as(_type,_objPtr) (_type *)__ex_as((ex_class_t *)_objPtr,EX_RTTI(_type))
void *__ex_as ( ex_class_t *_obj, const ex_rtti_t *_rtti );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END CLASS_H_1291098183
// #################################################################################
