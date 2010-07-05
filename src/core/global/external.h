// ======================================================================================
// File         : external.h
// Author       : Wu Jie 
// Last Change  : 06/26/2010 | 19:11:11 PM | Saturday,June
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef EXTERNAL_H_1277550673
#define EXTERNAL_H_1277550673
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// platform include
///////////////////////////////////////////////////////////////////////////////

#if (EX_PLATFORM == EX_WIN32)

    // if use memory manager and in debug version
    #if ( EX_USE_SYSTEM_MEMORY || (EX_USE_MEMORY_MANAGER && EX_DBG_WITH_CRT) ) && defined (EX_DEBUG)
        #define _CRTDBG_MAP_ALLOC
        // #define _CRTDBG_MAP_ALLOC_NEW // not really helpful!

        #include <crtdbg.h>
    #endif

    #include <windows.h>
    // #include <io.h>
    // #include <direct.h>
    // #include <process.h>
    // #include <sys/stat.h>
    // #include <sys/utime.h>
    // #include <errno.h>

    // // to use the sse intrins in gcc, please add -msse compile flag in makefile of this project
    // #include <xmmintrin.h>  // for type __m128

#elif (EX_PLATFORM == EX_LINUX)

    #include <malloc.h> 
    #include <io.h>
    #include <endian.h>    /* attempt to define endianness */

    // to use the sse intrins in gcc, please add -msse compile flag in makefile of this project
    #include <xmmintrin.h>  // for type __m128

    // stl container
    #include <pair> 
    #include <multiset>
    #include <multimap>

#endif

///////////////////////////////////////////////////////////////////////////////
// compiler include
///////////////////////////////////////////////////////////////////////////////

#if ( EX_COMPILER == EX_MSVC ) // msvc


#elif ( EX_COMPILER == EX_GCC ) // gcc

    #ifndef CHAR_BIT
    #define CHAR_BIT __CHAR_BIT__
    #endif

#endif

///////////////////////////////////////////////////////////////////////////////
// general
///////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h> 
#include <wchar.h>
#include <math.h>

///////////////////////////////////////////////////////////////////////////////
// windows re-define
///////////////////////////////////////////////////////////////////////////////

#if (EX_PLATFORM == EX_WIN32)
    #define snprintf _snprintf
#endif

// #################################################################################
#endif // END EXTERNAL_H_1277550673
// #################################################################################


