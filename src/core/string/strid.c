// ======================================================================================
// File         : strid.c
// Author       : Wu Jie 
// Last Change  : 11/26/2010 | 14:20:11 PM | Friday,November
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "strid.h"

///////////////////////////////////////////////////////////////////////////////
// private variables
///////////////////////////////////////////////////////////////////////////////

static ex_hashmap_t* _string_set = NULL;
static bool _initialized = false;

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_strid_init ( size_t _size )
{
    // if the core already initialized, don't init it second times.
    if ( _initialized ) {
        ex_warning ( "string ID table already initialized" );
        return true;
    }

    _string_set = ex_hashmap_alloc ( sizeof(char*), sizeof(char*), _size, ex_hashkey_string, ex_keycmp_string );
    ex_assert_return ( _string_set, false, "string table alloc failed" );

    _initialized = true;
    return true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_strid_deinit ()
{
    if ( _initialized ) {
        // free all allocated string
        ex_hashmap_each ( char*, str, _string_set ) {
            ex_free(str);
        } ex_hashmap_each_end;
        ex_hashmap_free(_string_set);
        _initialized = false;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_strid_is_inited () { return _initialized; }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

strid_t ex_strid ( char* _string )
{
    size_t idx = -1;
    size_t hash_idx = -1;
    char* str_new = NULL;

    if ( _string == NULL )
        return -1;

    hash_idx = ex_hashmap_get_hashidx ( _string_set, &_string, &idx ); 
    if ( idx == -1 ) {
        size_t len = strlen(_string);
        str_new = ex_malloc(len+1);
        strncpy( str_new, _string, len+1 );
        ex_hashmap_insert_new ( _string_set, &str_new, &str_new, hash_idx, &idx );
    }
    return idx;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

strid_t ex_strid_from_wcs ( wchar_t* _string )
{
    size_t idx = -1;
    size_t hash_idx = -1;
    size_t str_size = wcslen(_string)+1;
    char* str_utf8 = NULL;
    char* str_new = NULL;

    if ( _string == NULL )
        return -1;

    str_utf8 = ex_stack_malloc( str_size );
    ex_ucs2_to_utf8 ( _string, str_size, str_utf8 );
    hash_idx = ex_hashmap_get_hashidx ( _string_set, &str_utf8, &idx ); 
    if ( idx == -1 ) {
        str_new = ex_malloc(str_size);
        strncpy( str_new, str_utf8, str_size );
        ex_hashmap_insert_new ( _string_set, &str_new, &str_new, hash_idx, &idx );
    }
    ex_stack_free(str_utf8);
    return idx;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

char* ex_strid_to_cstr ( strid_t _id )
{
    char* addr = (char*)_string_set->_keys + _id * _string_set->_key_bytes;
    return *((char**)addr);
}

