// ======================================================================================
// File         : factory.c
// Author       : Wu Jie 
// Last Change  : 01/08/2011 | 13:23:37 PM | Saturday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

static ex_hashmap_t* __classid_to_creator = NULL;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void __factory_init () {
    __classid_to_creator = ex_hashmap_alloc ( sizeof(strid_t), 
                                              sizeof(ex_create_pfn_t), 
                                              256,
                                              ex_hashkey_strid, 
                                              ex_keycmp_strid );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void __factory_deinit () {
    ex_hashmap_free(__classid_to_creator);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_factory_register ( strid_t _classID, ex_create_pfn_t _pfn ) {
    ex_hashmap_insert( __classid_to_creator, &_classID, &_pfn, NULL );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_factory_unregister ( strid_t _classID ) {
    ex_hashmap_remove_at( __classid_to_creator, &_classID );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void* ex_factory_create ( strid_t _classID ) {
    void* pfn = ex_hashmap_get( __classid_to_creator, &_classID, NULL );
    if ( pfn == NULL ) {
        ex_error ("can't find creator for classID %s", ex_strid_to_cstr(_classID) );
        return NULL;
    }
    return (*(ex_create_pfn_t*)pfn)();
}
