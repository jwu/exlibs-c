// ======================================================================================
// File         : str_op.h
// Author       : Wu Jie 
// Last Change  : 10/12/2010 | 13:55:52 PM | Tuesday,October
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef STR_OP_H_1286862954
#define STR_OP_H_1286862954
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

// char
static inline int ex_str_nicmp ( const char *_str1, const char *_str2, uint _count )
{
#if (EX_COMPILER == EX_MSVC)
    return _strnicmp( _str1, _str2, _count );
#else
    return strncasecmp( _str1, _str2, _count );
#endif
}

// wchar_t
static inline int ex_wcs_nicmp( const wchar_t *_str1, const wchar_t *_str2, uint _count )
{
#if (EX_COMPILER == EX_MSVC)
    return _wcsnicmp( (const wchar_t *)_str1, (const wchar_t *)_str2, _count );
#else
    ex_assert( false, "wstr case n compare not support" );
    return 0;
#endif
}

// ------------------------------------------------------------------ 
// Desc: the inverse strstr 
// ------------------------------------------------------------------ 

extern char *ex_strrstr( const char *_str, const char *_needle );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern int ex_str_split_into_array( struct ex_array_t *_outList, const char *_token, const char *_text );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END STR_OP_H_1286862954
// #################################################################################
