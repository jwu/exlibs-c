// ======================================================================================
// File         : debug.c
// Author       : Wu Jie 
// Last Change  : 06/27/2010 | 10:10:52 AM | Sunday,June
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

#if (EX_PLATFORM == EX_WIN32)
    #include <windows.h>
#endif

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void _short_funcname ( char _short_name[], const char* _function_name, int _len )
{
    int len; 

    memzero ( _short_name, sizeof(char)*_len ); 
    len = strlen(_function_name); 
    if ( len > _len ) { 
        memcpy ( _short_name, _function_name, sizeof(char)*(_len-4) ); 
        _short_name[_len-4] = '.'; 
        _short_name[_len-3] = '.'; 
        _short_name[_len-2] = '.'; 
        _short_name[_len-1] = 0; 
    } 
    else 
    { 
        strncpy( _short_name, _function_name, len ); 
    } 
}

// ------------------------------------------------------------------
// Desc: 
// ------------------------------------------------------------------

bool assert_failed( bool* _pDoAssert, const char* _file_name, const char* _function_name, size_t _line_nr, const char* _expr, ... )
{
    //
    int     result = -1;
    int     buffer_count = 1024;
    char*   pBuffer = NULL;
    char    short_name[64];
    int     mbResult = -1;

    // keep get va string until success 
    while ( result == -1 )
    {
        pBuffer = (char*)ex_realloc_nomng( pBuffer, buffer_count * sizeof(char) );
        EX_GET_VA_STRING_WITH_RESULT( pBuffer, buffer_count, _expr, &result );
        buffer_count *= 2;
    }
    pBuffer[result] = 0;

    // short the function name
    _short_funcname( short_name, _function_name, 64 );

    //
    ex_log ( "Assert Failed: %s(%d)::%s, %s", _file_name, _line_nr, short_name, pBuffer );
    mbResult = message_box( MSG_BOX_FAILED, "Assert Failed", 
                            "|ASSERT_FAILED|\n"
                            "FileName: %s\n"
                            "Line: %d\n"
                            "FunctionName: %s\n" 
                            "Expr: %s\n", 
                            _file_name, _line_nr, short_name, pBuffer );

    // release buffer we allocate
    ex_free_nomng ( pBuffer );

    //
#if (EX_PLATFORM == EX_WIN32) || (EX_PLATFORM == EX_XENON)
    (*_pDoAssert) = (mbResult != IDIGNORE);
    return (mbResult == IDRETRY);
#else
    return true;
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool error_msg( bool* _pDoAssert, const char* _file_name, const char* _function_name, size_t _line_nr, const char* _expr, ... )
{
    //
    int     result = -1;
    int     buffer_count = 1024;
    char*   pBuffer = NULL;
    char    short_name[64];
    int     mbResult;

    // keep get va string until success 
    while ( result == -1 )
    {
        pBuffer = (char*)ex_realloc_nomng( pBuffer, buffer_count * sizeof(char) );
        EX_GET_VA_STRING_WITH_RESULT( pBuffer, buffer_count, _expr, &result );
        buffer_count *= 2;
    }
    pBuffer[result] = 0;

    // short the function name
    _short_funcname( short_name, _function_name, 64 );

    //
    ex_log ( "Error: %s(%d)[%s], %s", _file_name, _line_nr, short_name, pBuffer );
    mbResult = message_box( MSG_BOX_ERROR, "Error", "Error: %s(%d)[%s], %s", _file_name, _line_nr, short_name, pBuffer );

    // release buffer we allocate
    ex_free_nomng ( pBuffer );

    //
#if (EX_PLATFORM == EX_WIN32) || (EX_PLATFORM == EX_XENON)
    (*_pDoAssert) = (mbResult != IDIGNORE);
    return (mbResult == IDRETRY);
#else
    return true;
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool warning_msg ( bool* _pDoAssert, const char* _file_name, const char* _function_name, size_t _line_nr, const char* _expr, ... )
{
    //
    int     result = -1;
    int     buffer_count = 1024;
    char*   pBuffer = NULL;
    char    short_name[64];
    int     mbResult;

    // keep get va string until success 
    while ( result == -1 )
    {
        pBuffer = (char*)ex_realloc_nomng( pBuffer, buffer_count * sizeof(char) );
        EX_GET_VA_STRING_WITH_RESULT( pBuffer, buffer_count, _expr, &result );
        buffer_count *= 2;
    }
    pBuffer[result] = 0;

    // short the function name
    _short_funcname( short_name, _function_name, 64 );

    //
    ex_log ( "Warning: %s(%d)[%s], %s", _file_name, _line_nr, _function_name, pBuffer );
    mbResult = message_box( MSG_BOX_WARNING, "Warning", "Warning: %s(%d)[%s], %s", _file_name, _line_nr, _function_name, pBuffer );

    // release buffer we allocate
    ex_free_nomng ( pBuffer );

    //
#if (EX_PLATFORM == EX_WIN32) || (EX_PLATFORM == EX_XENON)
    (*_pDoAssert) = (mbResult != IDIGNORE);
    return (mbResult == IDRETRY);
#else
    return true;
#endif
}

