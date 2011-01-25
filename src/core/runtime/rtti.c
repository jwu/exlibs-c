// ======================================================================================
// File         : rtti.c
// Author       : Wu Jie 
// Last Change  : 11/26/2010 | 14:08:14 PM | Friday,November
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "rtti.h"
#include "core/string/strid.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

static ex_hashmap_t *__typeid_to_rtti = NULL;
static bool __initialized = false;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_rtti_init () 
{

    // if the core already initialized, don't init it second times.
    if ( __initialized ) {
        ex_warning ( "rtti table already initialized" );
        return 1;
    }

    // rtti must init after strid
    if ( ex_strid_initialized() == false ) {
        ex_warning ( "strid not initialized, please init it before rtti init." );
        return -1;
    }

    __typeid_to_rtti = ex_hashmap_notype ( sizeof(strid_t), 
                                           sizeof(ex_rtti_t *), 
                                           256,
                                           ex_hashkey_strid, 
                                           ex_keycmp_strid );
    __initialized = true;
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_rtti_deinit ()
{
    if ( __initialized ) {
        // free all allocated string
        ex_hashmap_each ( __typeid_to_rtti, ex_rtti_t *, _info ) {
            ex_free(_info->props);
            ex_free(_info);
        } ex_hashmap_each_end;
        ex_hashmap_free(__typeid_to_rtti);
        __initialized = false;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_rtti_initialized () { return __initialized; }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_rtti_t *ex_rtti_register_class ( strid_t _typeID, 
                                    ex_rtti_t *_super, 
                                    size_t _typeSize,
                                    ex_create_pfn _pfn_create,
                                    ex_serialize_pfn _pfn_serialize,
                                    ex_tostring_pfn _pfn_tostring
                                  )
{
    ex_rtti_t *my_rtti = ex_rtti_get(_typeID);
    bool result = false;

    // check if my class already exists.
    ex_assert_return ( my_rtti == NULL, NULL, "the class %s already registered.", ex_strid_to_cstr(_typeID) );

    // we got everything we want, now we can create rtti info.
    my_rtti = (ex_rtti_t *)ex_malloc ( sizeof(ex_rtti_t) );
    my_rtti->super = _super;
    my_rtti->typeID = _typeID;
    my_rtti->size = _typeSize;
    my_rtti->props = NULL;
    my_rtti->prop_count = 0;
    my_rtti->create = _pfn_create;
    my_rtti->serialize = _pfn_serialize;
    my_rtti->tostring = _pfn_tostring;

    // insert the new rtti to the hashmap
    result = ex_hashmap_insert( __typeid_to_rtti, &_typeID, &my_rtti, NULL );
    if ( result == false ) {
        ex_warning( "failed to insert new rtti info in to hashmap" );
        ex_free (my_rtti);
        return NULL;
    }

    //
    return my_rtti;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_rtti_register_properties ( ex_rtti_t *_info, const ex_prop_t *_props, uint32 _count )
{
    ex_assert_return( _info->props == NULL, /*dummy*/, "properties already registerd!" );

    // NOTE: you could have empty properties for a structure.
    if ( _count != 0 ) {
        ex_assert_return( _props != NULL, /*dummy*/, "incoming properties can't be NULL, or can't be empty!" );

        _info->props = (ex_prop_t *)ex_malloc( sizeof(ex_prop_t) * _count );
        memcpy( _info->props, _props, sizeof(ex_prop_t) * _count );
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_rtti_t *ex_rtti_get ( strid_t _typeID )
{
    ex_rtti_t **result = (ex_rtti_t **)ex_hashmap_get( __typeid_to_rtti, &_typeID, NULL );
    if ( result != NULL )
        return *result;
    return NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_rtti_childof ( const ex_rtti_t *_myclass, const ex_rtti_t *_superclass ) 
{ 
    ex_rtti_t *tmp = ex_rtti_super(_myclass);
    while ( tmp ) {
        if ( ex_rtti_classof(tmp, _superclass) ) 
            return true;
        tmp = ex_rtti_super(tmp);
    }
    return false;
}
