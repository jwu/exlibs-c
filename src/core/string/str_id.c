// ======================================================================================
// File         : str_id.c
// Author       : Wu Jie 
// Last Change  : 10/12/2010 | 13:57:04 PM | Tuesday,October
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "../../container/hashmap.h"
#include "str_id.h"

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

bool str_id_init ( size_t _size )
{
    // if the core already initialized, don't init it second times.
    if ( _initialized ) {
        ex_warning ( "string ID table already initialized" );
        return true;
    }

    _string_set = hashmap_alloc ( sizeof(char**), sizeof(char**), _size, hashkey_string, keycmp_string );
    ex_assert_return ( _string_set, false, "string table alloc failed" );

    _initialized = true;
    return true;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void str_id_deinit ()
{
    if ( _initialized ) {
        // free all allocated string
        hashmap_each ( char*, str, _string_set ) {
            ex_free(str);
        } hashmap_each_end;
        hashmap_free(_string_set);
        _initialized = false;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

size_t wcs_id ( wchar_t* _string )
{
    size_t idx = -1;
    size_t hash_idx = -1;
    size_t str_size = wcslen(_string)+1;
    char* str_utf8 = NULL;
    char* str_new = NULL;

    if ( _string == NULL )
        return -1;

    str_utf8 = ex_stack_malloc( str_size );
    ucs2_to_utf8 ( _string, str_size, str_utf8 );
    hash_idx = hashmap_get_hashidx ( _string_set, &str_utf8, &idx ); 
    if ( idx == -1 ) {
        str_new = ex_malloc(str_size);
        strncpy( str_new, str_utf8, str_size );
        hashmap_insert_new ( _string_set, &str_new, &str_new, hash_idx, &idx );
    }
    ex_stack_free(str_utf8);
    return idx;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

size_t str_id ( char* _string )
{
    size_t idx = -1;
    size_t hash_idx = -1;
    char* str_new = NULL;

    if ( _string == NULL )
        return -1;

    hash_idx = hashmap_get_hashidx ( _string_set, &_string, &idx ); 
    if ( idx == -1 ) {
        size_t len = strlen(_string);
        str_new = ex_malloc(len+1);
        strncpy( str_new, _string, len+1 );
        hashmap_insert_new ( _string_set, &str_new, &str_new, hash_idx, &idx );
    }
    return idx;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

char* str_id_to_cstr ( size_t _id )
{
    char* addr = (char*)_string_set->_keys + _id * _string_set->_key_bytes;
    return *((char**)addr);
}

