// ======================================================================================
// File         : ref.c
// Author       : Wu Jie 
// Last Change  : 01/29/2011 | 09:59:50 AM | Saturday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "ref.h"
#include "object.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t ex_invalidref () {
    static const ex_ref_t ref = {
        EX_UID_INVALID, // uid
        NULL, // ptr
        NULL, // refcount
        NULL, // valid
    };
    return ref;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t ex_newref ( void *_obj ) {
    ex_object_t *obj = (ex_object_t *)_obj;
    ex_ref_t ref = ex_invalidref();
    ex_assert_return( _obj != NULL, ref, "object can't be NULL" );

    ref.ptr = obj;
    ref.uid = obj->uid;
    ref.refcount = ex_malloc(sizeof(int)+sizeof(bool));
    ref.isvalid = (bool *)((char *)ref.refcount + sizeof(int));
    *(ref.refcount) = 0;
    *(ref.isvalid) = false;

    return ref;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_delref ( ex_ref_t _ref ) {
    ex_assert ( ex_ref_isvalid(_ref) == false, "the object still valid!" );

    if ( _ref.refcount )
        ex_free(_ref.refcount);
    _ref.ptr = NULL;
    _ref.uid = EX_UID_INVALID;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_incref ( ex_ref_t _ref ) {
    ex_assert_return ( ex_ref_isvalid(_ref), /*dummy*/, "the ref is invalid" );
    *(_ref.refcount) += 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_decref ( ex_ref_t _ref ) {
    int ret;

    ex_assert_return ( ex_ref_isvalid(_ref), -1, "the ref is invalid" );
    *(_ref.refcount) -= 1;
    ret = *(_ref.refcount); 
    if ( *(_ref.refcount) == 0 ) {
        ex_destroy_object(_ref.ptr);
    }
    return ret;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_ref_isvalid ( ex_ref_t _ref ) {
    if ( _ref.isvalid )
        return *_ref.isvalid; 
    return false;
}
