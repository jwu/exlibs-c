// ======================================================================================
// File         : test_encoding.c
// Author       : Wu Jie 
// Last Change  : 08/08/2010 | 18:38:30 PM | Sunday,August
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// include
///////////////////////////////////////////////////////////////////////////////

#include "../../core/core_inc.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void string ()
{
#if ( EX_PLATFORM == EX_WIN32 )
    {
        char char_src[128];
        wchar_t wchar_src[128];
        wchar_t wchar_unicode[128];

        memzero ( char_src, sizeof(char)*128 );
        strcpy ( char_src, "中文" );

        memzero ( wchar_src, sizeof(wchar_t)*128 );
        wcscpy( wchar_src, L"中文" );

        MultiByteToWideChar(CP_UTF8, 0, char_src, -1, wchar_unicode, strlen(char_src) * sizeof(wchar_t));
        // message_box ( MSG_BOX_NONE, "result", "char_src = %s, wchar_src = %ls, wchar_unicode = %ls", char_src, wchar_src, wchar_unicode );
        // message_box ( MSG_BOX_NONE, "result", "wchar_unicode = %s", wchar_unicode );
        ex_assert ( false, "wchar_unicode = %s", char_src );
        EX_HW_BREAK();
    }
#else
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static void filesystem ()
{
#if ( EX_PLATFORM == EX_WIN32 )
    {
        HANDLE hFile;
        wchar_t* wpath_srcEncoding = L"e:/project/dev/exsdk/res/path/中文/你好.txt";
        wchar_t wpath_unicode[MAX_PATH];
        char* path_srcEncoding = "e:/project/dev/exsdk/res/path/中文/你好.txt";

        MultiByteToWideChar(CP_UTF8, 0, path_srcEncoding, -1, wpath_unicode, strlen(path_srcEncoding) * sizeof(wchar_t));
        hFile = CreateFileW( wpath_unicode, FILE_READ_DATA, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
        if ( hFile == INVALID_HANDLE_VALUE ) {
            DWORD dwError = GetLastError ();
            ex_warning( "CreateFile failed. Error Code: %u.\n", dwError );
        }
    }
#else
#endif
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void test_encoding ()
{
    string();
    filesystem();
}

