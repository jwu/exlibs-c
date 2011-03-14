// ======================================================================================
// File         : rtti.h
// Author       : Wu Jie 
// Last Change  : 11/26/2010 | 11:32:20 AM | Friday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef RTTI_H_1290742341
#define RTTI_H_1290742341
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "prop.h"

///////////////////////////////////////////////////////////////////////////////
// struct
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
typedef struct ex_stream_t * ex_stream_ptr_t;
typedef void *(*ex_create_pfn) ();
typedef void (*ex_serialize_pfn) ( ex_stream_ptr_t, strid_t, void * );
typedef void (*ex_tostring_pfn) ( ex_string_t *, void * );
// ------------------------------------------------------------------ 

typedef struct ex_rtti_t {
    struct ex_rtti_t *super;
    strid_t typeID;
    size_t size; // the sizeof(type)
    ex_prop_t *props;
    uint32 prop_count;
    ex_create_pfn create; // the creator 
    ex_serialize_pfn serialize; // the serializer
    ex_tostring_pfn tostring; // the tostring method
} ex_rtti_t;

///////////////////////////////////////////////////////////////////////////////
// function
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// success: 0
// already inited: 1
// failed: -1
extern int ex_rtti_init ();
extern void ex_rtti_deinit ();
extern bool ex_rtti_initialized ();

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern ex_rtti_t *ex_rtti_register_class ( strid_t _typeID, 
                                           ex_rtti_t *_super, 
                                           size_t _typeSize,
                                           ex_create_pfn _pfn_create,
                                           ex_serialize_pfn _pfn_serialize,
                                           ex_tostring_pfn _pfn_tostring
                                           );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_rtti_register_properties ( ex_rtti_t *_info, const ex_prop_t* _props, uint32 _count );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern ex_rtti_t *ex_rtti_get ( strid_t _typeID );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline const char *ex_rtti_classname ( const ex_rtti_t *_info ) { 
    return ex_strid_to_cstr(_info->typeID); 
} 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline strid_t ex_rtti_typeid ( const ex_rtti_t *_info ) { 
    return _info->typeID; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline ex_rtti_t *ex_rtti_super ( const ex_rtti_t *_info ) { 
    return _info->super; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_rtti_instanceof ( const ex_rtti_t *_myclass, const ex_rtti_t *_yourclass ) { 
    return _myclass->typeID == _yourclass->typeID;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern bool ex_rtti_childof ( const ex_rtti_t *_myclass, const ex_rtti_t *_superclass );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_rtti_superof ( const ex_rtti_t *_myclass, const ex_rtti_t *_subclass ) { 
    return ex_rtti_childof( _subclass, _myclass );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_rtti_isa ( const ex_rtti_t *_myclass, const ex_rtti_t *_class ) { 
    return ex_rtti_instanceof(_myclass,_class) || ex_rtti_childof(_myclass,_class);
}

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END RTTI_H_1290742341
// #################################################################################


