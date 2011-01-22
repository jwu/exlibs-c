// ======================================================================================
// File         : string.h
// Author       : Wu Jie 
// Last Change  : 01/22/2011 | 09:20:23 AM | Saturday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef STRING_H_1295659224
#define STRING_H_1295659224
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

typedef struct ex_string_t {
    char *text;
    size_t len; // the count of the character in the text, it dosen't include the null terminal (e.g. '\0')
    size_t capacity; // the capacity of the text buffer, it will consider the null terminal (e.g. '\0')
} ex_string_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// NOTE: ex_string only for static,const string that EX_ARRAY_COUNT can recorgnized. 
#define ex_string(_const_str) ex_string_alloc(_const_str,EX_ARRAY_COUNT(_const_str)-1)
#define ex_string_copy(_string) ex_string_alloc(_string->text,_string->len)

// NOTE: the len doesn't include the null terminal
extern ex_string_t *ex_string_alloc ( const char *_cstr, int _len );
extern ex_string_t *ex_string_printf ( const char *_fmt, ... );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_string_free ( ex_string_t *_string );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline bool ex_string_is_empty( ex_string_t *_string ) { return _string->len == 0; }

// ------------------------------------------------------------------ 
// Desc: 
// NOTE: you can cat a ex_string_t by ex_string_ncat ( my_string, your_string->text, your_string->len );
// ------------------------------------------------------------------ 

#define ex_string_cat(_inout,_const_str) ex_string_ncat(_inout,_const_str,EX_ARRAY_COUNT(_const_str)-1)
extern void ex_string_ncat ( ex_string_t *_inout, const char *_cat, int _len );

// ------------------------------------------------------------------ 
// Desc: 
// NOTE: you can cat a ex_string_t by ex_string_ncpy ( my_string, your_string->text, your_string->len );
// ------------------------------------------------------------------ 

#define ex_string_cpy(_to,_const_str) ex_string_ncpy(_to,_const_str,EX_ARRAY_COUNT(_const_str)-1)
extern void ex_string_ncpy ( ex_string_t *_to, const char *_from, int _len );

// ------------------------------------------------------------------ 
// Desc: 
// NOTE: you can cat a ex_string_t by ex_string_ncmp ( my_string, your_string->text, your_string->len );
// ------------------------------------------------------------------ 

#define ex_string_cmp(_inout,_const_str) ex_string_ncmp(_inout,_const_str,EX_ARRAY_COUNT(_const_str)-1)
extern int ex_string_ncmp ( ex_string_t *_lhs, const char *_rhs, int _len );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END STRING_H_1295659224
// #################################################################################


