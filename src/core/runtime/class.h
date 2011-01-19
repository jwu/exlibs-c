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
#include "factory.h"

///////////////////////////////////////////////////////////////////////////////
// struct
///////////////////////////////////////////////////////////////////////////////

//
typedef struct ex_class_t {
    ex_rtti_t* rtti;
} ex_class_t;

//
extern ex_rtti_t* __RTTI_ex_class_t__;
static inline void __ex_register_class_ex_class_t () { 
    __RTTI_ex_class_t__ = ex_rtti_register_class ( ex_strid("ex_class_t"), NULL );
}

//
static inline ex_class_t* alloc_ex_class_t() {
    ex_class_t* obj = (ex_class_t*)ex_malloc(sizeof(ex_class_t));
    obj->rtti = __RTTI_ex_class_t__;
    return obj;
}

//
static inline void free_ex_class_t(void* _ptr) {
    ex_free (_ptr);
}

///////////////////////////////////////////////////////////////////////////////
// macros
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_RTTI(_class_name) \
    (__RTTI_##_class_name##__)

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_CLASSID(_class_name) \
    (EX_RTTI(_class_name)->classid)

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_REGISTER_CLASS(_class_name) \
    __ex_register_class_##_class_name() \

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DEF_CLASS_BEGIN(_class_name) \
    extern void* __ex_create_##_class_name(); \
    extern void __ex_register_properties_##_class_name (); \
    extern void __ex_serialize_##_class_name( struct ex_stream_t*, strid_t, void* ); \
    extern ex_rtti_t* __RTTI_##_class_name##__; /*for EX_RTTI, EX_CLASSID*/ \
    static inline void __ex_register_class_##_class_name () { /*for EX_REGISTER_CLASS, define in EX_DEF_PROPS_BEGIN*/ \
        strid_t classID = ex_strid(#_class_name); \
        __RTTI_##_class_name##__ = ex_rtti_register_class ( classID, EX_RTTI(ex_class_t) ); \
        __ex_register_properties_##_class_name(); \
        ex_factory_register(classID,__ex_create_##_class_name); \
        ex_rtti_register_serialize(classID,__ex_serialize_##_class_name); \
    } \
    typedef struct _class_name { \
        const struct ex_class_t _;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DEF_CLASS_SUPER_BEGIN(_class_name,_super) \
    extern void* __ex_create_##_class_name(); \
    extern void __ex_register_properties_##_class_name (); \
    extern void __ex_serialize_##_class_name( struct ex_stream_t*, strid_t, void* ); \
    extern ex_rtti_t* __RTTI_##_class_name##__; /*for EX_RTTI, EX_CLASSID*/ \
    static inline void __ex_register_class_##_class_name () { /*for EX_REGISTER_CLASS, define in EX_DEF_PROPS_BEGIN*/ \
        strid_t classID = ex_strid(#_class_name); \
        __RTTI_##_class_name##__ = ex_rtti_register_class ( classID, EX_RTTI(_super) ); \
        __ex_register_properties_##_class_name(); \
        ex_factory_register(classID,__ex_create_##_class_name); \
        ex_rtti_register_serialize(classID,__ex_serialize_##_class_name); \
    } \
    typedef struct _class_name { \
        const struct _super _;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DEF_CLASS_END(_class_name) \
    } _class_name; \
    static inline _class_name* alloc_##_class_name() { \
        ex_class_t* obj = (ex_class_t*)ex_malloc(sizeof(_class_name)); \
        obj->rtti = EX_RTTI(_class_name); \
        return (_class_name*)obj; \
    } \
    /*NOTE: the reason to add free function is for function pointer. 
     * remeber ex_free is a macro not a function, so can't be treat as 
     * a function pointer like void (*func) (void*) */ \
    static inline void free_##_class_name(void* _ptr) { \
        ex_free(_ptr); \
    }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DEF_CLASS_CREATOR(_class_name) \
    void* __ex_create_##_class_name()

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline ex_rtti_t* ex_rtti_info ( void* _obj ) {
    return ((ex_class_t*)_obj)->rtti;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define ex_classof(_type,_objPtr) __ex_classof((ex_class_t*)_objPtr,EX_RTTI(_type))
static inline bool __ex_classof ( const ex_class_t* _obj, const ex_rtti_t* _rtti ) {
    return ex_rtti_classof( _obj->rtti, _rtti );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define ex_childof(_type,_objPtr) __ex_childof((ex_class_t*)_objPtr,EX_RTTI(_type))
static inline bool __ex_childof ( const ex_class_t* _obj, const ex_rtti_t* _rtti ) {
    return ex_rtti_childof( _obj->rtti, _rtti );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define ex_superof(_type,_objPtr) __ex_superof((ex_class_t*)_objPtr,EX_RTTI(_type))
static inline bool __ex_superof ( const ex_class_t* _obj, const ex_rtti_t* _rtti ) {
    return ex_rtti_superof( _obj->rtti, _rtti );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define ex_isa(_type,_objPtr) __ex_isa((ex_class_t*)_objPtr,EX_RTTI(_type))
static inline bool __ex_isa ( const ex_class_t* _obj, const ex_rtti_t* _rtti ) {
    return ex_rtti_isa( _obj->rtti, _rtti );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define ex_as(_type,_objPtr) (_type*)__ex_as((ex_class_t*)_objPtr,EX_RTTI(_type))
void* __ex_as ( ex_class_t* _obj, const ex_rtti_t* _rtti );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END CLASS_H_1291098183
// #################################################################################
