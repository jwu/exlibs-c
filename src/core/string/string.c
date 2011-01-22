// ======================================================================================
// File         : string.c
// Author       : Wu Jie 
// Last Change  : 01/22/2011 | 09:20:36 AM | Saturday,January
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

ex_string_t *ex_string_alloc ( const char *_cstr, int _len ) {
    ex_string_t *string;
    ex_assert_return ( _cstr, NULL, "can't create a NULL string." );
    
    string = (ex_string_t *)ex_malloc( sizeof(ex_string_t) );
    string->len = _len;
    string->capacity = EX_ALIGN_ROUND(_len+1,8);
    string->text = (char *)ex_malloc( sizeof(char) * string->capacity );
    strncpy ( string->text, _cstr, _len );
    string->text[_len] = '\0';

    return string;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

ex_string_t *ex_string_printf ( const char *_fmt, ... ) {
    ex_string_t *string;
    int result = -1;
    int buffer_count = 1024;

    ex_assert_return ( _fmt, NULL, "can't create a NULL string." );

    string = (ex_string_t *)ex_malloc( sizeof(ex_string_t) );
    string->text = NULL;
    string->len = 0;
    string->capacity = 0;

    // keep get va string until success 
    while ( result == -1 ) {
        string->text = (char *)ex_realloc( string->text, buffer_count * sizeof(char) );
        string->capacity = buffer_count;
        EX_GET_VA_STRING_WITH_RESULT( string->text, buffer_count, _fmt, &result );
        buffer_count *= 2;
    }
    string->text[result] = '\0';
    string->len = result;
    return string;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_string_free ( ex_string_t *_string ) {
    ex_free (_string->text);
    _string->len = 0;
    _string->capacity = 0;
    _string->text = NULL;
    ex_free (_string);
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_string_ncat ( ex_string_t *_inout, const char *_cat, int _len ) {
    size_t new_size; 

    if ( _len == 0 )
        return;

    new_size = _inout->len + _len + 1;
    if ( _inout->capacity < new_size ) {
        _inout->capacity = EX_ALIGN_ROUND( new_size, 8 );
        _inout->text = (char *)ex_realloc( _inout->text, sizeof(char) * _inout->capacity );
    }
    _inout->len = new_size - 1; 
    strncat ( _inout->text, _cat, _len );
    _inout->text[_inout->len] = '\0';
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_string_ncpy ( ex_string_t *_to, const char *_from, int _len ) {
    size_t new_size; 

    if ( _len == 0 ) {
        _to->len = 0;
        _to->text[_len] = '\0';
        return;
    }

    new_size = _to->len + _len + 1;
    if ( _to->capacity < new_size ) {
        _to->capacity = EX_ALIGN_ROUND( new_size, 8 );
        _to->text = (char *)ex_realloc( _to->text, sizeof(char) * _to->capacity );
    }
    _to->len = new_size - 1;
    strncpy ( _to->text, _from, _len );
    _to->text[_len] = '\0';
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_string_ncmp ( ex_string_t *_lhs, const char *_rhs, int _len ) {
    if ( _lhs->len != _len )
        return _lhs->len - _len; 
    return strncmp ( _lhs->text, _rhs, _len );
}
