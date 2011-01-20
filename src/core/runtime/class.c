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

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void *__ex_as ( ex_class_t *_obj, const ex_rtti_t *_rtti ) {
    if ( ex_rtti_isa ( _obj->rtti, _rtti ) )
        return _obj;
    return NULL;
}

