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

///////////////////////////////////////////////////////////////////////////////
// properties
///////////////////////////////////////////////////////////////////////////////

EX_DEF_CLASS_BEGIN(ex_ref_t)
    EX_UID_INVALID, // uid
    NULL, // ptr
    NULL, // refcount
EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_ref_t)
    EX_PROP( ex_ref_t, uid, uid, "uid",  EX_PROP_ATTR_HIDE )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN(ex_ref_t)
    EX_MEMBER_SERIALIZE(uid,uid)
EX_SERIALIZE_END

EX_DEF_TOSTRING_BEGIN(ex_ref_t)
    EX_MEMBER_TOSTRING( uid, "uid", self->uid )
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_newref ( ex_uid_t _uid ) {
    void *ptr = NULL;
    ex_ref_t *ref = ex_create_ex_ref_t();

    // TODO: use uid get the ptr { 
    // } TODO end 

    ex_assert_return( ptr != NULL, NULL, "can't find the pointer by uid %.16llX", _uid );

    ref->ptr = ptr;
    ref->uid = _uid;
    ref->refcount = ex_malloc(sizeof(int));
    ex_incref(ref);
    return ref;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_incref ( ex_ref_t *_ref ) {
    ex_assert_return ( _ref, /*dummy*/, "the ref can't not be NULL" );
    *(_ref->refcount) += 1;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_decref ( ex_ref_t *_ref ) {
    int ret;

    ex_assert_return ( _ref, -1, "the ref can't not be NULL" );
    *(_ref->refcount) -= 1;
    ret = *(_ref->refcount); 
    if ( *(_ref->refcount) == 0 ) {
        // TODO: send message to the world to return the reference.
        ex_free(_ref->refcount);
        ex_free(_ref);
    }

    return ret;
}
