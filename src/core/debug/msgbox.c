// ======================================================================================
// File         : msgbox.c
// Author       : Wu Jie 
// Last Change  : 06/27/2010 | 11:38:43 AM | Sunday,June
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

int ex_message_box ( int _msgType, const char* _caption, const char* _expr,  ... )
{
    //
    int     result = -1;
    int     buffer_count = 1024;
    char*   pBuffer = NULL;

#if (EX_PLATFORM == EX_WIN32) || (EX_PLATFORM == EX_XENON)
    uint type = MB_OK;
    int ret;
#endif

    // keep get va string until success 
    while ( result == -1 )
    {
        pBuffer = (char*)ex_realloc_nomng( pBuffer, buffer_count * sizeof(char) );
        EX_GET_VA_STRING_WITH_RESULT( pBuffer, buffer_count, _expr, &result );
        buffer_count *= 2;
    }
    pBuffer[result] = 0;

#if (EX_PLATFORM == EX_WIN32) || (EX_PLATFORM == EX_XENON)
    if ( _msgType == EX_MSG_BOX_FAILED )
        type = MB_ABORTRETRYIGNORE|MB_ICONWARNING; // TODO: should be "Skip, Break, Ignore"
    else if ( _msgType == EX_MSG_BOX_ERROR )
        type = MB_ICONERROR;
    else if ( _msgType == EX_MSG_BOX_WARNING )
        type = MB_ICONWARNING;

    ret = MessageBox( NULL, pBuffer, _caption, type );
#endif

    // release buffer we allocate
    ex_free_nomng ( pBuffer );

    //
    return ret;
}


