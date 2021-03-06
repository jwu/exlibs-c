// ======================================================================================
// File         : class.c
// Author       : Wu Jie 
// Last Change  : 11/30/2010 | 14:23:16 PM | Tuesday,November
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

ex_rtti_t *__RTTI_ex_class_t__ = NULL;
strid_t __TYPEID_ex_class_t__ = EX_STRID_NULL;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void *__ex_create_ex_class_t( const ex_rtti_t *_rtti ) {
    ex_class_t *obj = (ex_class_t *)ex_malloc(sizeof(ex_class_t));
    obj->rtti = _rtti;
    return obj;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void *__ex_cast ( ex_class_t *_obj, const ex_rtti_t *_rtti, bool _no_error ) {
    if ( ex_rtti_isa ( _obj->rtti, _rtti ) )
        return _obj;

    if ( !_no_error ) {
        ex_error ( "can't cast object to type %s", 
                   ex_strid_to_cstr(ex_rtti_typeid(_rtti)) );
    }
    return NULL;
}

