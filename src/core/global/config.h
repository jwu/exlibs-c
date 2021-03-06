// ======================================================================================
// File         : config.h
// Author       : Wu Jie 
// Last Change  : 11/16/2010 | 13:25:07 PM | Tuesday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef CONFIG_H_1277512370
#define CONFIG_H_1277512370
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
//  User Config Block
//  In this area user can change the value to order his own version.
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------
// Desc: Inline function debug
//  by default, we enable inline function debug in debug mode.
// ------------------------------------------------------------------

#ifdef EX_DEBUG
    #define EX_USE_FORCEINLINE 0
#elif defined (EX_RELEASE)
    #define EX_USE_FORCEINLINE 1
#endif

///////////////////////////////////////////////////////////////////////////////
// memory configurations 
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------
// Desc: System Memory Alignment
// ------------------------------------------------------------------

#define EX_DEFAULT_ALIGNMENT 16

// ------------------------------------------------------------------
// Desc: Set if use system memory alloc
// NOTE: EX_USE_SYSTEM_MEMORY == 1 has same effect as
//          EX_USE_MEMORY_MANAGER = 0 
//          EX_DBG_WITH_CRT = 1 
// ------------------------------------------------------------------

#define EX_USE_SYSTEM_MEMORY 0

// ------------------------------------------------------------------
// Desc: Set if use memory manager or not
// ------------------------------------------------------------------

#define EX_USE_MEMORY_MANAGER 1

// ------------------------------------------------------------------
// Desc: Set if use crt memory debug with memory manager
// ------------------------------------------------------------------

#define EX_DBG_WITH_CRT 0

// ------------------------------------------------------------------
// Desc: Set the default allocator
// ------------------------------------------------------------------

// TODO: #define EX_ALLOCATOR ::ex::CAllocator_dl283

///////////////////////////////////////////////////////////////////////////////
//  math configurations
///////////////////////////////////////////////////////////////////////////////

//! @addtogroup Math
//! @{

// ------------------------------------------------------------------
/*!
 @def EX_FLOAT_EPS
 Set the default float precision for math calculation such as compare
*/// ------------------------------------------------------------------

#define EX_FLOAT_EPS 1e-06f

// ------------------------------------------------------------------
/*!
 @def EX_DOUBLE_EPS
 Set the default double precision for math calculation such as compare
*/// ------------------------------------------------------------------

#define EX_DOUBLE_EPS 1e-08

// ------------------------------------------------------------------
/*!
 @def EX_REAL_PRECISION
 Set the default double precision for math calculation such as compare
 @par options:
 - EX_SINGLE (default)
 - EX_DOUBLE
*/// ------------------------------------------------------------------

#define EX_SINGLE 0
#define EX_DOUBLE 1

#define EX_REAL_PRECISION EX_SINGLE

//! @}

///////////////////////////////////////////////////////////////////////////////
// ext-lib defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: Set if use qt or not
// ------------------------------------------------------------------ 

#define EX_USE_QT 0

// ------------------------------------------------------------------ 
// Desc: Set if use qt or not
// ------------------------------------------------------------------ 

#define EX_USE_SDL 1

///////////////////////////////////////////////////////////////////////////////
// after configuration ( some deps define should be reset here ) 
///////////////////////////////////////////////////////////////////////////////

#if EX_DBG_WITH_CRT
    #undef EX_ALLOCATOR
    // TODO #define EX_ALLOCATOR ::ex::CAllocator_system
#endif 

#ifdef EX_NO_EXTERNAL
    #undef EX_USE_QT
    #define EX_USE_QT 0

    #undef EX_USE_SDL
    #define EX_USE_SDL 0
#endif

// #################################################################################
#endif // END CONFIG_H_1277512370
// #################################################################################


