// ======================================================================================
// File         : object.c
// Author       : Wu Jie 
// Last Change  : 01/28/2011 | 14:10:24 PM | Friday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "object.h"

///////////////////////////////////////////////////////////////////////////////
// properties
///////////////////////////////////////////////////////////////////////////////

EX_DEF_CLASS_BEGIN(ex_object_t)
    EX_MEMBER(ex_object_t, uid, EX_UID_INVALID)
    EX_MEMBER(ex_object_t, name, EX_STRID_NULL)
EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_object_t)
    EX_PROP( ex_object_t, uid, uid, "uid",  EX_PROP_ATTR_HIDE )
    EX_PROP( ex_object_t, strid, name, "name",  EX_PROP_ATTR_HIDE )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN(ex_object_t)
    EX_MEMBER_SERIALIZE(uid,uid)
    EX_MEMBER_SERIALIZE(strid,name)
EX_SERIALIZE_END

EX_DEF_TOSTRING_BEGIN(ex_object_t)
    EX_MEMBER_TOSTRING( uid, "uid", self->uid )
    EX_MEMBER_TOSTRING( strid, "name", self->name )
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// internal defines
///////////////////////////////////////////////////////////////////////////////

static ex_hashmap_t __uid_to_objptr;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void __init_object_table () {
    ex_hashmap_init ( &__uid_to_objptr,
                      EX_STRID_NULL, sizeof(ex_uid_t),
                      EX_STRID_NULL, sizeof(ex_ref_t),
                      2048,
                      ex_hashkey_uid, ex_keycmp_uid,
                      __ex_hashmap_alloc,
                      __ex_hashmap_realloc,
                      __ex_hashmap_dealloc
                    );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void __deinit_object_table () {
    ex_hashmap_each ( &__uid_to_objptr, ex_ref_t, objref ) {
        // it is possible the object already destroyed.
        if ( ex_ref_isvalid(objref) ) {
            int refcount = ex_decref(objref);
            ex_assert( refcount == 0, 
                       "reference count is not zero for object uid: name: %s, 0x%.16llX",  
                       ex_strid_to_cstr( ((ex_object_t *)objref.ptr)->name),
                       ((ex_object_t *)objref.ptr)->uid );
        }
        ex_delref(objref);
    } ex_hashmap_each_end
    ex_hashmap_deinit( &__uid_to_objptr );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __add_object ( void *_obj ) {
    ex_object_t *obj = (ex_object_t *)_obj; 
    ex_ref_t newref = ex_newref(obj);
    if ( ex_hashmap_insert ( &__uid_to_objptr, &(obj->uid), &newref, NULL ) == false ) {
        ex_error ( "Failed to add object %s, the uid 0x%.16llX already been used!", 
                   ex_strid_to_cstr(obj->name),
                   obj->uid );
        return;
    }
    *(newref.isvalid) = true;
    ex_incref(newref);
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_init_object ( void *_obj ) { 
    ex_object_t *obj = (ex_object_t *)_obj;

    if ( obj->uid == EX_UID_INVALID )
        obj->uid = ex_generate_uid();
    obj->init(obj);
    __add_object (obj);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_destroy_object ( void *_obj ) {
    ex_object_t *obj = (ex_object_t *)_obj; 
    ex_ref_t objref;

    obj->deinit(obj);
    objref = ex_get_objref ( obj->uid );
    if ( ex_ref_isvalid(objref) ) {
        obj->name = EX_STRID_NULL;
        obj->uid = EX_UID_INVALID;
        ex_free(_obj);
        *(objref.isvalid) = false;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t ex_get_objref ( ex_uid_t _uid ) {
    ex_ref_t *refptr;
    refptr = ex_hashmap_get ( &__uid_to_objptr, &_uid, NULL );
    if ( refptr == NULL ) {
        ex_error ( "Failed to get object reference by uid 0x%.16llX!", _uid );
        return ex_invalidref(); 
    }
    return *refptr;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_object_gc () {
    int obj_counter = 0;
    int obj_gc_counter = 0;

    ex_log("object: %ld", ex_hashmap_count(&__uid_to_objptr) );

    ex_hashmap_each ( &__uid_to_objptr, ex_ref_t, objref ) {
        // if the object reference count is 1, and not destroyed, de-reference it.
        if ( ex_ref_isvalid(objref) && *(objref.refcount) == 1 ) {
            ex_decref(objref);
        }

        // if the object is not valid, destroy it
        if ( ex_ref_isvalid(objref) == false ) {
            uint32 hash_idx = __uid_to_objptr.hashkey(&(objref.uid)) & ( __uid_to_objptr.hashsize - 1 );
            ex_delref(objref);
            ex_hashmap_remove_by_idx( &__uid_to_objptr, hash_idx, __idx__ );

            ++obj_gc_counter;
        }

        ++obj_counter;
    } ex_hashmap_each_end

    // 
    ex_log("object in loop: %d", obj_counter );
    ex_log("object gc: %d", obj_gc_counter );
    ex_log("remain object: %ld", ex_hashmap_count(&__uid_to_objptr) );
}
