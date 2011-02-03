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
    EX_MEMBER(ex_object_t, flags, EX_OBJECT_NONE)
EX_DEF_CLASS_END

EX_DEF_PROPS_BEGIN(ex_object_t)
    EX_PROP( ex_object_t, uid, uid, "uid",  EX_PROP_ATTR_HIDE )
    EX_PROP( ex_object_t, strid, name, "name",  EX_PROP_ATTR_HIDE )
    EX_PROP( ex_object_t, uint32, flags, "flags",  EX_PROP_ATTR_HIDE )
EX_DEF_PROPS_END

EX_SERIALIZE_BEGIN(ex_object_t)
    EX_MEMBER_SERIALIZE(uid,uid)
    EX_MEMBER_SERIALIZE(strid,name)
    EX_MEMBER_SERIALIZE(uint32,flags)
EX_SERIALIZE_END

EX_DEF_TOSTRING_BEGIN(ex_object_t)
    EX_MEMBER_TOSTRING( uid, "uid", self->uid )
    EX_MEMBER_TOSTRING( strid, "name", self->name )
    EX_MEMBER_TOSTRING( uint32, "flags", self->flags )
EX_DEF_TOSTRING_END

///////////////////////////////////////////////////////////////////////////////
// internal defines
///////////////////////////////////////////////////////////////////////////////

static ex_hashmap_t __uid_to_refptr;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void __init_ref_table () {
    ex_hashmap_init ( &__uid_to_refptr,
                      EX_STRID_NULL, sizeof(ex_uid_t),
                      EX_STRID_NULL, sizeof(ex_ref_t *),
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

void __deinit_ref_table () {
    // first we doing a gc so that we can collect most objects.
    ex_object_gc();

    //
    ex_hashmap_each ( &__uid_to_refptr, ex_ref_t *, ref ) {
        // it is possible the object already destroyed.
        if ( ref->ptr != NULL ) {
            ex_error( "reference count is not zero for object uid: name: %s, 0x%.16llX",  
                      ex_strid_to_cstr( ((ex_object_t *)ref->ptr)->name ),
                      ((ex_object_t *)ref->ptr)->uid  );
        }
        ex_delref(ref);
    } ex_hashmap_each_end
    ex_hashmap_deinit( &__uid_to_refptr );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static ex_ref_t *__reftable_add ( void *_obj ) {
    ex_object_t *obj = (ex_object_t *)_obj; 
    ex_ref_t *newref;
    size_t idx, hash_idx; 

    //
    hash_idx = ex_hashmap_get_hashidx ( &__uid_to_refptr, &(obj->uid), &idx ); 
    if ( idx == -1 ) {
        newref = ex_newref(obj);
        ex_hashmap_insert_new ( &__uid_to_refptr, &(obj->uid), &newref, hash_idx, &idx );
    }
    else {
        newref = *((ex_ref_t **)ex_hashmap_get_by_idx( &__uid_to_refptr, idx ));
        if ( newref->ptr != NULL ) {
            ex_error ( "Failed to add object %s to reference table,"
                       "the uid 0x%.16llX already been used!", 
                       ex_strid_to_cstr(obj->name),
                       obj->uid );
            return NULL;
        }
        newref->ptr = obj;
    }

    //
    return newref;
}

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_create_object ( strid_t _typeID, ex_uid_t _uid ) {
    ex_assert_return( _uid != EX_UID_INVALID, NULL, "faield to create object. uid is invalid." );
    ex_object_t *obj = (ex_object_t *)ex_create(_typeID);
    obj->uid = _uid;
    return __reftable_add (obj);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_destroy_object ( ex_ref_t *_ref ) {
    ex_object_t *obj = (ex_object_t *)(_ref->ptr);
    ex_assert_return ( obj != NULL, /*dummy*/, "the object already been destroyed!" );

    obj->flags = ex_flags_add ( obj->flags, EX_OBJECT_DEAD );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_destroy_object_immediately ( ex_ref_t *_ref, bool _no_error ) {
    ex_object_t *obj = (ex_object_t *)(_ref->ptr);
    ex_assert_return ( obj != NULL, /*dummy*/, "the object already been destroyed!" );

    if ( _no_error == false ) {
        if ( _ref->refcount != 0 )
            ex_warning ( "warning: the refcount is not zero" );
    }

    obj->deinit(_ref);
    obj->uid = EX_UID_INVALID;
    obj->name = EX_STRID_NULL;
    obj->flags = EX_OBJECT_NONE;
    ex_free(obj);
    _ref->ptr = NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_ref_t *ex_getref ( ex_uid_t _uid ) {
    ex_ref_t *ref;
    size_t hash_idx, idx;

    //
    hash_idx = ex_hashmap_get_hashidx ( &__uid_to_refptr, &_uid, &idx ); 
    if ( idx == -1 ) {
        ref = ex_newref(NULL);
        ex_hashmap_insert_new ( &__uid_to_refptr, &_uid, &ref, hash_idx, &idx );
    }
    else {
        ref = *((ex_ref_t **)ex_hashmap_get_by_idx( &__uid_to_refptr, idx ));
    }

    ex_incref(ref);
    return ref;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_object_gc () {
    int obj_counter = 0;
    int obj_gc_counter = 0;

    ex_hashmap_each ( &__uid_to_refptr, ex_ref_t *, ref ) {
        ex_uid_t uid = *((ex_uid_t *)__key__);
        ex_object_t *obj = (ex_object_t *)ref->ptr;

        // if we found dead object.
        if ( obj && ex_flags_has(obj->flags,EX_OBJECT_DEAD) ) {
            obj->deinit(ref);
            obj->uid = EX_UID_INVALID;
            obj->name = EX_STRID_NULL;
            obj->flags = EX_OBJECT_NONE;
            ex_free(obj);

            ref->ptr = NULL;
            ++obj_gc_counter;
        }

        // if the reference count is zero, delete the reference from reference-table
        if ( ref->refcount == 0 ) {
            uint32 hash_idx = __uid_to_refptr.hashkey(&uid) & ( __uid_to_refptr.hashsize - 1 );
            ex_delref(ref);
            ex_hashmap_remove_by_idx( &__uid_to_refptr, hash_idx, __idx__ );
        }

        ++obj_counter;
    } ex_hashmap_each_end

    // 
    // ex_log("total objects in loop: %d", obj_counter );
    // ex_log("garbage collected objects : %d", obj_gc_counter );
    // ex_log("remain objects: %ld", ex_hashmap_count(&__uid_to_refptr) );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_serialize_objects ( ex_stream_t *_stream ) {
    int num_objects;
    strid_t typeID;
    ex_object_t *obj;
    
    num_objects = ex_hashmap_count(&__uid_to_refptr);
    EX_SERIALIZE( _stream, int, "num_objects", &num_objects );

    if ( _stream->type == EX_STREAM_READ ) {
        for ( int i = 0; i < num_objects; ++i ) {
            EX_SERIALIZE( _stream, strid, "type", &typeID );
            obj = ex_create(typeID);
            ex_rtti_info(obj)->serialize( _stream, ex_strid("object"), obj );
            __reftable_add (obj);
        }
    }
    else if ( _stream->type == EX_STREAM_WRITE ) {
        ex_hashmap_each ( &__uid_to_refptr, ex_ref_t *, ref ) {
            ex_rtti_t *rtti = NULL;
            obj = EX_REF_PTR(ex_object_t,ref);

            // do NOT save NULL objects
            if ( obj == NULL || ex_flags_has(obj->flags,EX_OBJECT_DEAD) ) {
                ex_hashmap_continue;
            }

            rtti = ex_rtti_info(obj);
            typeID = rtti->typeID;
            EX_SERIALIZE( _stream, strid, "type", &typeID );
            rtti->serialize( _stream, ex_strid("object"), obj );
        } ex_hashmap_each_end
    }
}

