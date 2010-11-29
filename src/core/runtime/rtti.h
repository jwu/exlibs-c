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

typedef struct ex_rtti_t {
    struct ex_rtti_t* _super;
    strid_t _classid;
    ex_prop_t* _props;
    uint32 _prop_count;
} ex_rtti_t;

///////////////////////////////////////////////////////////////////////////////
// function
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_rtti_init ();
void ex_rtti_deinit ();
bool ex_rtti_is_inited ();

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_rtti_t* ex_rtti_register_class ( char* _class, char* _super );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_rtti_register_properties ( ex_rtti_t* _info, const ex_prop_t* _props, uint32 _count );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_rtti_t* ex_rtti_get ( strid_t _classid );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline const char* ex_rtti_classname ( ex_rtti_t* _info ) { 
    return ex_strid_to_cstr(_info->_classid); 
} 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline strid_t ex_rtti_classid ( ex_rtti_t* _info ) { 
    return _info->_classid; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline ex_rtti_t* ex_rtti_super ( ex_rtti_t* _info ) { 
    return _info->_super; 
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline bool ex_rtti_class_of ( ex_rtti_t* _myclass, strid_t _classid ) { 
    return _myclass->_classid == _classid;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_rtti_subclass_of ( ex_rtti_t* _myclass, strid_t _super_classid );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

inline bool ex_rtti_superclass_of ( ex_rtti_t* _myclass, strid_t _sub_classid ) { 
    return ex_rtti_subclass_of( ex_rtti_get(_sub_classid), ex_rtti_classid(_myclass) );
}

// ------------------------------------------------------------------ 
// Desc: 
// NOTE: usually, we use this one.
// ------------------------------------------------------------------ 

inline bool ex_rtti_isa ( ex_rtti_t* _myclass, strid_t _classid ) { 
    return ex_rtti_class_of(_myclass,_classid) || ex_rtti_subclass_of(_myclass,_classid);
}

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END RTTI_H_1290742341
// #################################################################################

