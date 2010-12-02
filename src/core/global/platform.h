// ======================================================================================
// File         : platform.h
// Author       : Wu Jie 
// Last Change  : 06/26/2010 | 08:38:11 AM | Saturday,June
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef PLATFORM_H_1277512693
#define PLATFORM_H_1277512693
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
//  Pre-Define Block
//  In this area user add new defines that will be use in entire system
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------
// Desc: If a macro no defined it will be EX_UNKNOWN
// ------------------------------------------------------------------

#define EX_UNKNOWN  0x00

// ------------------------------------------------------------------
// Desc: Platform define
//  use EX_PLATFORM to judge current operator system.
// ------------------------------------------------------------------

#define EX_WIN32    0x01
#define EX_LINUX    0x02
#define EX_MAC      0x03
#define EX_XENON    0x04
#define EX_PS3      0x05

// ------------------------------------------------------------------
// Desc: Platform choose
// ------------------------------------------------------------------

#if defined ( __EX_WIN32__ ) || defined ( WIN32 ) || defined ( _WIN32 )
    #define EX_PLATFORM EX_WIN32
#elif defined ( __EX_LINUX__ )
    #define EX_PLATFORM EX_LINUX 
#elif defined ( __EX_MAC__ )
    #define EX_PLATFORM EX_MAC 
#elif defined ( __EX_XENON__ )
    #define EX_PLATFORM EX_XENON 
#elif defined ( __EX_PS3__ )
    #define EX_PLATFORM EX_PS3
#else
    #define EX_PLATFORM EX_UNKNOWN
#endif

// ------------------------------------------------------------------
// Desc: Platform Names
//  This macro will be used in including cross platform files.
//  NOTE: all platform name formate will be ==> exPlatformName
// ------------------------------------------------------------------

#if ( EX_PLATFORM == EX_WIN32 )
    #define EX_PLATFORM_NAME exWin32
#elif ( EX_PLATFORM == EX_LINUX )
    #define EX_PLATFORM_NAME exLinux
#elif ( EX_PLATFORM == EX_XENON )
    #define EX_PLATFORM_NAME exXenon
#elif ( EX_PLATFORM == EX_PS3 )
    #define EX_PLATFORM_NAME exPS3
#else
    #define EX_PLATFORM_NAME exUnknown
#endif

// ------------------------------------------------------------------
// Desc: Debug Mode
// ------------------------------------------------------------------

#if defined ( DEBUG ) || defined ( _DEBUG ) || defined ( __EX_DEBUG__ )
    #define EX_DEBUG
#elif defined ( RELEASE ) || defined ( _RELEASE ) || defined ( __EX_RELEASE__ )
    #define EX_RELEASE
#endif

// ------------------------------------------------------------------
// Desc: Check if platform support unicode 
// ------------------------------------------------------------------

#if ( EX_PLATFORM == EX_WIN32 )
    #define EX_UNICODE_PLATFORM 1
#elif ( EX_PLATFORM == EX_LINUX )
    #define EX_UNICODE_PLATFORM 1
#elif ( EX_PLATFORM == EX_XENON )
    #define EX_UNICODE_PLATFORM 0
#elif ( EX_PLATFORM == EX_PS3 )
    #define EX_UNICODE_PLATFORM 0
#endif

// ------------------------------------------------------------------
// Desc: UNICODE MODE
// ------------------------------------------------------------------

#if defined ( UNICODE ) || defined ( _UNICODE )
    #define EX_USE_UNICODE 1
#else
    #define EX_USE_UNICODE 0
#endif

// ------------------------------------------------------------------
// Desc: Endian detect                                                   
//  First we define the usually platform endian known by experience 
//  Then we define the endian by detail after "External.h"    
// ------------------------------------------------------------------

#define EX_LITTLE_ENDIAN 0
#define EX_BIG_ENDIAN    1

#if ( EX_PLATFORM == EX_WIN32 )
    #define EX_ENDIAN   EX_LITTLE_ENDIAN
#elif ( EX_PLATFORM == EX_LINUX )
    #define EX_ENDIAN   EX_LITTLE_ENDIAN
#elif ( EX_PLATFORM == EX_XENON )
    #define EX_ENDIAN   EX_BIG_ENDIAN
#elif ( EX_PLATFORM == EX_PS3 )
    #define EX_ENDIAN   EX_BIG_ENDIAN
#endif

// ------------------------------------------------------------------
// Desc: define EX_DLL if we have DLL/_DLL macro
// ------------------------------------------------------------------

#if defined(__EX_DLL__) || defined (_WINDLL)
    #define EX_USE_DLL 1
#endif

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

#if (EX_USE_DLL)
    #if ( EX_PLATFORM == EX_WIN32 )
        #define EX_DLL_EXPORT __declspec(dllexport)
        #define EX_DLL_IMPORT __declspec(dllimport)
    #else
        #define EX_DLL_EXPORT
        #define EX_DLL_IMPORT
    #endif
#else
    #define EX_DLL_EXPORT
    #define EX_DLL_IMPORT
#endif

// ------------------------------------------------------------------
// Desc: Endian detect
//  First we define the usually platform endian known by experience
//  Then we define the endian by detail after "Base_GExternal.h"
//  endian define by detail
// ------------------------------------------------------------------

#if ( defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN) && __BYTE_ORDER == __LITTLE_ENDIAN ) || \
    ( defined(i386) || defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) || defined(vax) || defined(MIPSEL) )
    // Big-endian machine
    #ifdef EX_ENDIAN
        #undef EX_ENDIAN
    #endif
    #define EX_ENDIAN EX_LITTLE_ENDIAN
#elif ( defined(__BYTE_ORDER) && defined(__BIG_ENDIAN) && __BYTE_ORDER == __BIG_ENDIAN ) || \
      ( defined(sparc) || defined(POWERPC) || defined(mc68000) || defined(sel) )
    // Little-endian machine
    #ifdef EX_ENDIAN
        #undef EX_ENDIAN
    #endif
    #define EX_ENDIAN EX_BIG_ENDIAN
#endif

// #################################################################################
#endif // END PLATFORM_H_1277512693
// #################################################################################
