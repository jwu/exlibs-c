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

ex_ref_t *ex_newref ( void *_obj ) {
    ex_ref_t *ref;
    ref = ex_malloc(sizeof(ex_ref_t));
    ref->ptr = _obj;
    ref->refcount = 0;

    return ref;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_delref ( ex_ref_t *_ref ) {
    ex_assert_return ( _ref != NULL, /**/, "can't delete NULL ref" );
    ex_assert_return ( _ref->ptr == NULL, /**/, "can't delete the ref, the object still valid!" );

    _ref->refcount = 0;
    ex_free(_ref);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_incref ( ex_ref_t *_ref ) {
    _ref->refcount += 1;
    return _ref->refcount; 
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __reftable_remove ( ex_ref_t );
// ------------------------------------------------------------------ 

int ex_decref ( ex_ref_t *_ref ) {
    int ret;
    ex_assert_return( _ref->refcount > 0, -1, "the reference count already zero, you dereference it twice." );

    _ref->refcount -= 1;
    if ( _ref->refcount == 0 ) {
        if ( _ref->ptr )
            ex_destroy_object(_ref);
    }

    return ret;
}
