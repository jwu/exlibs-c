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

static ex_hashmap_t* __string_set = NULL;
static bool __initialized = false;

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_strid_init ( size_t _size )
{
    // if the core already initialized, don't init it second times.
    if ( __initialized ) {
        ex_warning ( "string ID table already initialized" );
        return 1;
    }

    __string_set = ex_hashmap_alloc ( sizeof(char*), sizeof(char*), _size, ex_hashkey_string, ex_keycmp_string );
    ex_assert_return ( __string_set, -1, "string table alloc failed" );

    __initialized = true;
    return 0;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_strid_deinit ()
{
    if ( __initialized ) {
        // free all allocated string
        ex_hashmap_each ( __string_set, char*, str ) {
            ex_free(str);
        } ex_hashmap_each_end;
        ex_hashmap_free(__string_set);
        __initialized = false;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool ex_strid_initialized () { return __initialized; }

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

strid_t ex_strid ( const char* _string )
{
    size_t idx = -1;
    size_t hash_idx = -1;
    char* str_new = NULL;

    if ( _string == NULL )
        return -1;

    hash_idx = ex_hashmap_get_hashidx ( __string_set, &_string, &idx ); 
    if ( idx == -1 ) {
        size_t len = strlen(_string);
        str_new = ex_malloc(len+1);
        strncpy( str_new, _string, len+1 );
        ex_hashmap_insert_new ( __string_set, &str_new, &str_new, hash_idx, &idx );
    }
    return idx;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

strid_t ex_strid_from_wcs ( const wchar_t* _string )
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
    hash_idx = ex_hashmap_get_hashidx ( __string_set, &str_utf8, &idx ); 
    if ( idx == -1 ) {
        str_new = ex_malloc(str_size);
        strncpy( str_new, str_utf8, str_size );
        ex_hashmap_insert_new ( __string_set, &str_new, &str_new, hash_idx, &idx );
    }
    ex_stack_free(str_utf8);
    return idx;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

char* ex_strid_to_cstr ( strid_t _id )
{
    char* addr = (char*)__string_set->_keys + _id * __string_set->_key_bytes;
    return *((char**)addr);
}

