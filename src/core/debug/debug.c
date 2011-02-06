// ======================================================================================
// File         : debug.c
// Author       : Wu Jie 
// Last Change  : 11/16/2010 | 13:25:02 PM | Tuesday,November
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"

#if (EX_PLATFORM == EX_WIN32)
    #include <windows.h>
#endif

#define BUF_SIZE 1024

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void __short_funcname ( char _short_name[], const char *_function_name, int _len ) {
    int len; 

    len = strlen(_function_name); 
    if ( len > _len-1 ) { 
        memcpy ( _short_name, _function_name, sizeof(char)*(_len-4) ); 
        _short_name[_len-4] = '.'; 
        _short_name[_len-3] = '.'; 
        _short_name[_len-2] = '.'; 
        _short_name[_len-1] = '\0'; 
    } else { 
        strncpy( _short_name, _function_name, len ); 
        _short_name[len] = '\0';
    } 
}

// ------------------------------------------------------------------
// Desc: 
// ------------------------------------------------------------------

bool assert_failed( bool *_pDoAssert, const char *_file_name, const char *_function_name, size_t _line_nr, const char *_expr, ... )
{
    int     result = -1;
    char    short_name[64];
    int     mbResult = -1;
    char    buf[BUF_SIZE];
    char   *buffer = NULL;

    EX_GET_VA_STRING_WITH_RESULT( buf, BUF_SIZE-1, _expr, &result ); // NOTE: the buffer_count-1 will leave 1 character for null terminal
    buffer = buf;
    if ( result == -1 ) {
        char *dyn_buf = NULL;
        int buffer_count = BUF_SIZE * 2;

        // keep get va string until success 
        do {
            dyn_buf = (char *)ex_realloc_nomng( dyn_buf, buffer_count * sizeof(char) );
            EX_GET_VA_STRING_WITH_RESULT( dyn_buf, buffer_count-1, _expr, &result ); // NOTE: the buffer_count-1 will leave 1 character for null terminal
            buffer_count *= 2;
        } while ( result == -1 );
        buffer = dyn_buf;
    }
    buffer[result] = '\0';


    // short the function name
    __short_funcname( short_name, _function_name, 64 );

    //
    ex_log ( "Assert Failed: %s(%d)::%s, %s", _file_name, (int)_line_nr, short_name, buffer );
    mbResult = ex_message_box( EX_MSG_BOX_FAILED, "Assert Failed", 
                            "|ASSERT_FAILED|\n"
                            "FileName: %s\n"
                            "Line: %d\n"
                            "FunctionName: %s\n" 
                            "Expr: %s\n", 
                            _file_name, (int)_line_nr, short_name, buffer );

    // if we use dynamic buffer, free it
    if ( buffer != buf )
        ex_free_nomng ( buffer );

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

bool error_msg( bool *_pDoAssert, const char *_file_name, const char *_function_name, size_t _line_nr, const char *_expr, ... )
{
    int     result = -1;
    char    short_name[64];
    int     mbResult = -1;
    char    buf[BUF_SIZE];
    char   *buffer = NULL;

    EX_GET_VA_STRING_WITH_RESULT( buf, BUF_SIZE-1, _expr, &result ); // NOTE: the buffer_count-1 will leave 1 character for null terminal
    buffer = buf;
    if ( result == -1 ) {
        char *dyn_buf = NULL;
        int buffer_count = BUF_SIZE * 2;

        // keep get va string until success 
        do {
            dyn_buf = (char *)ex_realloc_nomng( dyn_buf, buffer_count * sizeof(char) );
            EX_GET_VA_STRING_WITH_RESULT( dyn_buf, buffer_count-1, _expr, &result ); // NOTE: the buffer_count-1 will leave 1 character for null terminal
            buffer_count *= 2;
        } while ( result == -1 );
        buffer = dyn_buf;
    }
    buffer[result] = '\0';

    // short the function name
    __short_funcname( short_name, _function_name, 64 );

    //
    ex_log ( "Error: %s(%d)[%s], %s", _file_name, (int)_line_nr, short_name, buffer );
    mbResult = ex_message_box( EX_MSG_BOX_ERROR, "Error", "Error: %s(%d)[%s], %s", _file_name, (int)_line_nr, short_name, buffer );

    // if we use dynamic buffer, free it
    if ( buffer != buf )
        ex_free_nomng ( buffer );

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

bool warning_msg ( bool *_pDoAssert, const char *_file_name, const char *_function_name, size_t _line_nr, const char *_expr, ... )
{
    int     result = -1;
    char    short_name[64];
    int     mbResult = -1;
    char    buf[BUF_SIZE];
    char   *buffer = NULL;

    EX_GET_VA_STRING_WITH_RESULT( buf, BUF_SIZE-1, _expr, &result ); // NOTE: the buffer_count-1 will leave 1 character for null terminal
    buffer = buf;
    if ( result == -1 ) {
        char *dyn_buf = NULL;
        int buffer_count = BUF_SIZE * 2;

        // keep get va string until success 
        do {
            dyn_buf = (char *)ex_realloc_nomng( dyn_buf, buffer_count * sizeof(char) );
            EX_GET_VA_STRING_WITH_RESULT( dyn_buf, buffer_count-1, _expr, &result ); // NOTE: the buffer_count-1 will leave 1 character for null terminal
            buffer_count *= 2;
        } while ( result == -1 );
        buffer = dyn_buf;
    }
    buffer[result] = '\0';

    // short the function name
    __short_funcname( short_name, _function_name, 64 );

    //
    ex_log ( "Warning: %s(%d)[%s], %s", _file_name, (int)_line_nr, _function_name, buffer );
    mbResult = ex_message_box( EX_MSG_BOX_WARNING, "Warning", "Warning: %s(%d)[%s], %s", _file_name, (int)_line_nr, _function_name, buffer );

    // if we use dynamic buffer, free it
    if ( buffer != buf )
        ex_free_nomng ( buffer );

    //
#if (EX_PLATFORM == EX_WIN32) || (EX_PLATFORM == EX_XENON)
    (*_pDoAssert) = (mbResult != IDIGNORE);
    return (mbResult == IDRETRY);
#else
    return true;
#endif
}

