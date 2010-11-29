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
#include "../string/strid.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

static ex_hashmap_t* _classid_to_rtti = NULL;
static bool _initialized = false;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_rtti_init () 
{
    // if the core already initialized, don't init it second times.
    if ( _initialized ) {
        ex_warning ( "rtti table already initialized" );
        return true;
    }

    // rtti must init after strid
    if ( ex_strid_is_inited() == false ) {
        ex_warning ( "strid not initialized, please init it before rtti init." );
        return false;
    }

    _classid_to_rtti = ex_hashmap_alloc ( sizeof(strid_t), 
                                          sizeof(ex_rtti_t*), 
                                          256,
                                          ex_hashkey_strid, 
                                          ex_keycmp_strid );

    _initialized = true;
    return true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_rtti_deinit ()
{
    if ( _initialized ) {
        // free all allocated string
        ex_hashmap_each ( ex_rtti_t*, _info, _classid_to_rtti ) {
            ex_free(_info->_props);
            ex_free(_info);
        } ex_hashmap_each_end;
        ex_hashmap_free(_classid_to_rtti);
        _initialized = false;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_rtti_is_inited () { return _initialized; }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_rtti_t* ex_rtti_register_class ( char* _class, char* _super )
{
    strid_t my_classid = ex_strid(_class);
    ex_rtti_t* my_rtti = ex_rtti_get(my_classid);
    strid_t super_classid;
    ex_rtti_t* super_rtti;

    // check if my class already exists.
    ex_assert_return ( my_rtti == NULL, NULL, "the class %s already registered.", _class );

    // if we have super class, get it.
    if ( _super ) {
        super_classid = ex_strid(_super);
        super_rtti = ex_rtti_get(super_classid);
        ex_assert_return ( super_rtti, NULL, "the class %s already registered.", _class );
    }

    // we got everything we want, now we can create rtti info.
    my_rtti = (ex_rtti_t*)ex_malloc ( sizeof(ex_rtti_t) );
    my_rtti->_super = super_rtti;
    my_rtti->_classid = my_classid;
    my_rtti->_props = NULL;

    return my_rtti;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_rtti_register_properties ( ex_rtti_t* _info, const ex_prop_t* _props, uint32 _count )
{
    ex_assert_return( _info->_props == NULL, /*dummy*/, "properties already registerd!" );
    ex_assert_return( _props != NULL && _count > 0, /*dummy*/, "incoming properties can't be NULL, or can't be empty!" );

    _info->_props = (ex_prop_t*)ex_malloc( sizeof(ex_prop_t) * _count );
    memcpy( _info->_props, _props, sizeof(ex_prop_t) * _count );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_rtti_t* ex_rtti_get ( strid_t _classid )
{
    ex_rtti_t** result = (ex_rtti_t**)ex_hashmap_get( _classid_to_rtti, &_classid, NULL );
    if ( result != NULL )
        return *result;
    return NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_rtti_subclass_of ( ex_rtti_t* _myclass, strid_t _super_classid ) 
{ 
    ex_rtti_t* tmp = ex_rtti_super(_myclass);
    while ( tmp ) {
        if ( ex_rtti_class_of(tmp, _super_classid) ) 
            return true;
        tmp = ex_rtti_super(tmp);
    }
    return false;
}