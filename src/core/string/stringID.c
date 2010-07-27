// ======================================================================================
// File         : stringID.c
// Author       : Wu Jie 
// Last Change  : 07/07/2010 | 15:02:54 PM | Wednesday,July
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "../../container/hashmap.h"
#include "stringID.h"

///////////////////////////////////////////////////////////////////////////////
// private variables
///////////////////////////////////////////////////////////////////////////////

static hashmap_t* _string_set = NULL;
static bool _initialized = false;

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool sid_init ( size_t _size )
{
    // if the core already initialized, don't init it second times.
    if ( _initialized ) {
        ex_warning ( "string ID table already initialized" );
        return true;
    }

    _string_set = hashmap_alloc ( sizeof(wchar_t**), sizeof(wchar_t**), _size, hashkey_wstring, keycmp_wstring );
    ex_assert_return ( _string_set, false, "string table alloc failed" );

    _initialized = true;
    return true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void sid_deinit ()
{
    if ( _initialized ) {
        hashmap_free(_string_set);
        _initialized = false;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

size_t strID ( wchar_t* _string )
{
    size_t idx = -1;
    hashmap_insert ( _string_set, &_string, &_string, &idx );
    return idx;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

wchar_t* sid_toString ( size_t _id )
{
    char* addr = (char*)_string_set->_keys + _id * _string_set->_key_bytes;
    return *((wchar_t**)addr);
}

