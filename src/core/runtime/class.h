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
    ex_rtti_t* _rtti;
} ex_class_t;

//
extern ex_rtti_t* __RTTI_ex_class_t;
extern void __ex_register_class_ex_class_t ();

//
inline ex_class_t* alloc_ex_class_t() {
    ex_class_t* obj = (ex_class_t*)ex_malloc(sizeof(ex_class_t));
    obj->_rtti = __RTTI_ex_class_t;
    return obj;
}

//
inline void free_ex_class_t(void* _ptr) {
    ex_free (_ptr);
}

///////////////////////////////////////////////////////////////////////////////
// macros
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_RTTI(_name) \
    (__RTTI_##_name)

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_CLASSID(_name) \
    (EX_RTTI(_name)->_classid)

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_REGISTER_CLASS(_name) \
    __ex_register_class_##_name()

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DEF_CLASS_BEGIN(_name) \
    extern void __ex_register_properties_##_name (); \
    extern ex_rtti_t* __RTTI_##_name; /*for EX_RTTI, EX_CLASSID*/ \
    inline void __ex_register_class_##_name () { /*for EX_REGISTER_CLASS, define in EX_DEF_PROPS_BEGIN*/ \
        __RTTI_##_name = ex_rtti_register_class ( #_name, EX_RTTI(ex_class_t) ); \
        __ex_register_properties_##_name(); \
    } \
    typedef struct _name { \
        const struct ex_class_t _;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DEF_CLASS_SUPER_BEGIN(_name,_super) \
    extern void __ex_register_properties_##_name (); \
    extern ex_rtti_t* __RTTI_##_name; /*for EX_RTTI, EX_CLASSID*/ \
    inline void __ex_register_class_##_name () { /*for EX_REGISTER_CLASS, define in EX_DEF_PROPS_BEGIN*/ \
        __RTTI_##_name = ex_rtti_register_class ( #_name, EX_RTTI(_super) ); \
        __ex_register_properties_##_name(); \
    } \
    typedef struct _name { \
        const struct _super _;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DEF_CLASS_END(_name) \
    } _name; \
    inline _name* alloc_##_name() { \
        ex_class_t* obj = (ex_class_t*)ex_malloc(sizeof(_name)); \
        obj->_rtti = EX_RTTI(_name); \
        return (_name*)obj; \
    } \
    /*NOTE: the reason to add free function is for function pointer. 
     * remeber ex_free is a macro not a function, so can't be treat as 
     * a function pointer like void (*func) (void*) */ \
    inline void free_##_name(void* _ptr) { \
        ex_free(_ptr); \
    }

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline ex_rtti_t* ex_rtti_info ( void* _obj ) {
    return ((ex_class_t*)_obj)->_rtti;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define ex_classof(_type,_objPtr) __ex_classof((ex_class_t*)_objPtr,EX_RTTI(_type))
inline bool __ex_classof ( ex_class_t* _obj, ex_rtti_t* _rtti ) {
    return ex_rtti_classof( _obj->_rtti, _rtti );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define ex_childof(_type,_objPtr) __ex_childof((ex_class_t*)_objPtr,EX_RTTI(_type))
inline bool __ex_childof ( ex_class_t* _obj, ex_rtti_t* _rtti ) {
    return ex_rtti_childof( _obj->_rtti, _rtti );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define ex_superof(_type,_objPtr) __ex_superof((ex_class_t*)_objPtr,EX_RTTI(_type))
inline bool __ex_superof ( ex_class_t* _obj, ex_rtti_t* _rtti ) {
    return ex_rtti_superof( _obj->_rtti, _rtti );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define ex_isa(_type,_objPtr) __ex_isa((ex_class_t*)_objPtr,EX_RTTI(_type))
inline bool __ex_isa ( ex_class_t* _obj, ex_rtti_t* _rtti ) {
    return ex_rtti_isa( _obj->_rtti, _rtti );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define ex_as(_type,_objPtr) (_type*)__ex_as((ex_class_t*)_objPtr,EX_RTTI(_type))
void* __ex_as ( ex_class_t* _obj, ex_rtti_t* _rtti );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END CLASS_H_1291098183
// #################################################################################
