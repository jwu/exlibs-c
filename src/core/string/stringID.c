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

hashmap_t* _string_set = NULL;

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void strID_init ( size_t _size )
{
    _string_set = hashmap_alloc ( sizeof(wchar_t**), sizeof(wchar_t**), _size, hashkey_wstring, keycmp_wstring );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void strID_deinit ()
{
    hashmap_free(_string_set);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

size_t wcsID ( wchar_t* _string )
{
    return hashmap_insert ( _string_set, &_string, &_string );
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

wchar_t* sid_toString ( size_t _id )
{
    return pool_get_wstring( _string_set->_nodes, _id );
}

