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

static ex_hashmap_t *__id_to_serialize = NULL;
static ex_hashmap_t *__classid_to_rtti = NULL;
static bool __initialized = false;

// ------------------------------------------------------------------ 
// Desc: 
extern void __factory_init ();
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

    __id_to_serialize = ex_hashmap_alloc ( sizeof(strid_t), 
                                           sizeof(void *), 
                                           256,
                                           ex_hashkey_strid, 
                                           ex_keycmp_strid );

    __classid_to_rtti = ex_hashmap_alloc ( sizeof(strid_t), 
                                           sizeof(ex_rtti_t *), 
                                           256,
                                           ex_hashkey_strid, 
                                           ex_keycmp_strid );
    __factory_init();

    __initialized = true;
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
extern void __factory_deinit ();
// ------------------------------------------------------------------ 

void ex_rtti_deinit ()
{
    if ( __initialized ) {
        __factory_deinit();

        // free all allocated string
        ex_hashmap_each ( __classid_to_rtti, ex_rtti_t *, _info ) {
            ex_free(_info->props);
            ex_free(_info);
        } ex_hashmap_each_end;
        ex_hashmap_free(__classid_to_rtti);
        ex_hashmap_free(__id_to_serialize);
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

void ex_rtti_register_serialize ( strid_t _typeID, void *_pfn ) {
    ex_hashmap_insert ( __id_to_serialize, &_typeID, &_pfn, NULL );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_rtti_t *ex_rtti_register_class ( strid_t _classID, ex_rtti_t *_super )
{
    ex_rtti_t *my_rtti = ex_rtti_get(_classID);
    bool result = false;

    // check if my class already exists.
    ex_assert_return ( my_rtti == NULL, NULL, "the class %s already registered.", ex_strid_to_cstr(_classID) );

    // we got everything we want, now we can create rtti info.
    my_rtti = (ex_rtti_t *)ex_malloc ( sizeof(ex_rtti_t) );
    my_rtti->super = _super;
    my_rtti->classid = _classID;
    my_rtti->props = NULL;

    // insert the new rtti to the hashmap
    result = ex_hashmap_insert( __classid_to_rtti, &_classID, &my_rtti, NULL );
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

ex_rtti_t *ex_rtti_get ( strid_t _classID )
{
    ex_rtti_t **result = (ex_rtti_t **)ex_hashmap_get( __classid_to_rtti, &_classID, NULL );
    if ( result != NULL )
        return *result;
    return NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void *ex_rtti_get_serialize_pfn ( strid_t _typeID ) {
    void **result = (void **)ex_hashmap_get( __id_to_serialize, &_typeID, NULL );
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
