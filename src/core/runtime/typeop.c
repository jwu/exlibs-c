// ======================================================================================
// File         : typeop.c
// Author       : Wu Jie 
// Last Change  : 01/21/2011 | 10:22:26 AM | Friday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void *ex_create ( strid_t _typeID ) {
    ex_rtti_t *rtti = ex_rtti_get(_typeID);
    if ( rtti ) {
        if ( rtti->create ) return rtti->create();
        else ex_warning ("can't find creator for type %s", ex_strid_to_cstr(_typeID) );
    }
    else {
        ex_warning ("can't find rtti for type %s", ex_strid_to_cstr(_typeID) );
    }
    return NULL;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_serialize ( strid_t _typeID, 
                    ex_stream_t *_stream, strid_t _name, void *_val ) 
{
	ex_rtti_t *rtti = ex_rtti_get(_typeID);
    if ( rtti ) {
        if ( rtti->serialize ) 
            rtti->serialize(_stream,_name,_val);
        else 
            ex_warning ( "can't find serialize function for type %s", ex_strid_to_cstr(_typeID) );
    }
    else {
        ex_warning ( "can't find rtti for type %s", ex_strid_to_cstr(_typeID) );
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

size_t ex_sizeof ( strid_t _typeID ) {
    ex_rtti_t *rtti = ex_rtti_get(_typeID);
    if ( rtti ) 
        return rtti->size; 
    else
        ex_warning ( "can't find rtti for type %s", ex_strid_to_cstr(_typeID) );
    return 0;
}
