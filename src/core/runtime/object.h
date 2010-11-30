// ======================================================================================
// File         : object.h
// Author       : Wu Jie 
// Last Change  : 11/26/2010 | 16:35:55 PM | Friday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef OBJECT_H_1290760556
#define OBJECT_H_1290760556
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
typedef struct ex_object_t {
    ex_rtti_t* _rtti;
} ex_object_t;

//
extern strid_t __CLASSID_ex_object_t;
extern void ex_register_class_ex_object_t ();

//
inline ex_object_t* ex_object_t_alloc() {
    ex_object_t* obj = (ex_object_t*)ex_malloc(sizeof(ex_object_t));
    obj->_rtti = ex_rtti_get(__CLASSID_ex_object_t);
    return obj;
}

//
inline void ex_object_t_free(void* _ptr) {
    ex_free (_ptr);
}

///////////////////////////////////////////////////////////////////////////////
// macros
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_CLASSID(_name) \
    __CLASSID_##_name

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_REGISTER_CLASS(_name) \
    ex_register_class_##_name()

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DECL_CLASS_BEGIN(_name) \
    typedef struct _name { \
        const struct ex_object_t _;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DECL_CLASS_SUPER_BEGIN(_name,_super) \
    typedef struct _name { \
        const struct _super _;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DECL_CLASS_END(_name) \
    } _name; \
    extern strid_t __CLASSID_##_name; /*for EX_CLASSID*/ \
    extern void ex_register_class_##_name (); /*for EX_REGISTER_CLASS, define in EX_DEF_PROPS_BEGIN*/ \
    inline _name* _name##_alloc() { \
        ex_object_t* obj = (ex_object_t*)ex_malloc(sizeof(_name)); \
        obj->_rtti = ex_rtti_get(EX_CLASSID(_name)); \
        return obj; \
    } \
    /*NOTE: the reason to add free function is for function pointer. 
     * remeber ex_free is a macro not a function, so can't be treat as 
     * a function pointer like void (*func) (void*) */ \
    inline void _name##_free(void* _ptr) { \
        ex_free(_ptr); \
    }

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#define EX_DYNAMIC_CAST(_type,_obj) (_type*)ex_object_as((ex_object_t*)_obj,EX_CLASSID(_type))
void* ex_object_as ( ex_object_t* _obj, strid_t _typeID );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END OBJECT_H_1290760556
// #################################################################################


