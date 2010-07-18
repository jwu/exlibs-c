// ======================================================================================
// File         : debug.h
// Author       : Wu Jie 
// Last Change  : 06/26/2010 | 11:51:05 AM | Saturday,June
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef DEBUG_H_1277524266
#define DEBUG_H_1277524266
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// config
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------
// Desc: Enable Assert or not
// Params: SERIOUS
//         NORMAL
//         DISABLE
// ------------------------------------------------------------------

#define SERIOUS 0
#define NORMAL  1
#define DISABLE 2

#ifdef EX_DEBUG
    #define EX_DEBUG_LEVEL SERIOUS // could be NORMAL
#elif defined (EX_RELEASE)
    #define EX_DEBUG_LEVEL DISABLE
#endif

///////////////////////////////////////////////////////////////////////////////
// function decl
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool assert_failed ( bool* _pDoAssert, const char* _file_name, const char* _function_name, size_t _line_nr, const char* _expr, ... );
bool error_msg ( bool* _pDoAssert, const char* _file_name, const char* _function_name, size_t _line_nr, const char* _expr, ... );
bool warning_msg ( bool* _pDoAssert, const char* _file_name, const char* _function_name, size_t _line_nr, const char* _expr, ... );

///////////////////////////////////////////////////////////////////////////////
// help macors 
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------
// Desc: Hardware Break
// ------------------------------------------------------------------

#ifdef EX_DEBUG
    #if ( EX_PLATFORM == EX_WIN32 )
        #if ( EX_COMPILER == EX_MSVC )
            #define EX_HW_BREAK()   {__asm {int 3}}
        #elif ( EX_COMPILER == EX_GCC )
            #define EX_HW_BREAK()   {__asm__ __volatile__ ("int3");}
        #endif
    #elif ( EX_PLATFORM == EX_LINUX )
        #define EX_HW_BREAK()   {__asm__ __volatile__ ("int3");}
    #elif ( EX_PLATFORM == EX_XENON )
        #define EX_HW_BREAK()   {__debugbreak();}
    #elif ( EX_PLATFORM == EX_PS3 )
        #define EX_HW_BREAK()   __asm__ volatile ( "tw 31,1,1" ) // found in CELL_SDK samples, found "BREAK" tag to get it.
        // #define EX_HW_BREAK()   snPause // need #include <libsn.h> || use __builtin_snpause() #ifdef __SNC__
    #endif
#else
    #define EX_HW_BREAK() EX_NOOP
#endif

// ------------------------------------------------------------------
// Desc: Debug Break
//  debug break is used in debug mode, support user a way to that his condition be break.
//  We use !! below to ensure that any overloaded operators used to evaluate expr do not end up at operator || 
// ------------------------------------------------------------------

#define EX_HW_BREAK_IF(expr) { if(!!(expr)) EX_HW_BREAK(); }

// ------------------------------------------------------------------ 
// Desc: ex_log, ex_warning, ex_error
// ------------------------------------------------------------------ 

// ======================================================== 
// internal warning/error break macro  
// ======================================================== 

// __EX_ERROR_BREAK
#define __EX_ERROR_BREAK(msg,...) \
	do { \
		static bool s_AskBreak = true; \
		if ( s_AskBreak ) \
		{ \
            if( error_msg( &s_AskBreak, __FILE__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__ ) ) \
			{ \
				EX_HW_BREAK(); \
			} \
		} \
        else \
        { \
            /*TODO: log_error( __FILE__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__ );*/ \
        } \
	} while( false )

// __EX_WARNING_BREAK
#define __EX_WARNING_BREAK(msg,...) \
	do { \
		static bool s_AskBreak = true; \
		if ( s_AskBreak ) \
		{ \
            if( warning_msg( &s_AskBreak, __FILE__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__ ) ) \
			{ \
				EX_HW_BREAK(); \
			} \
		} \
        else \
        { \
            /*TODO: log_warning( __FILE__, __LINE__, __FUNCTION__, msg, ##__VA_ARGS__ );*/ \
        } \
	} while( false )

// ======================================================== 
// macro defines 
// ======================================================== 

// TODO { 
#define ex_log( msg, ... ) printf( msg"\n", ##__VA_ARGS__ )
// } TODO end 

#if ( EX_DEBUG_LEVEL == SERIOUS )
    #define ex_error(msg,...) __EX_ERROR_BREAK(msg"\n", ##__VA_ARGS__) 
    #define ex_warning(msg,...) __EX_WARNING_BREAK(msg"\n", ##__VA_ARGS__)
#else
    // TODO { 
    #define ex_error(msg,...) EX_GP(::ex::CLog)->LogError( __FILE__, __LINE__, __FUNCTION__, msg"\n", ##__VA_ARGS__ )
    #define ex_warning(msg,...) EX_GP(::ex::CLog)->LogWarning( __FILE__, __LINE__, __FUNCTION__, msg"\n", ##__VA_ARGS__ )
    // } TODO end 
#endif

// ------------------------------------------------------------------ 
// Desc: ex_log_func
// ------------------------------------------------------------------ 

#if ( EX_DEBUG_LEVEL == SERIOUS )
    #define ex_log_func() ex_log ("%s", EX_FUNC_INFO);
#else
    #define ex_log_func()
#endif

// ------------------------------------------------------------------
// Desc: Assert
// ------------------------------------------------------------------

// ========================================================
// internal assert define with break
// ========================================================

// __EX_ASSERT_BREAK
#define __EX_ASSERT_BREAK(expr,msg,...) \
	do { \
		static bool s_DoAssert = true; \
        bool eval = !(expr); \
		if ( s_DoAssert ) \
		{ \
            if( eval && assert_failed( &s_DoAssert, __FILE__, __FUNCTION__, __LINE__, #expr" : "msg, ##__VA_ARGS__ ) ) \
			{ \
				EX_HW_BREAK(); \
			} \
		} \
	} while( false )

// __EX_ASSERT_RETURN_BREAK
#define __EX_ASSERT_RETURN_BREAK(expr,ret,msg,...) \
	do { \
		static bool s_DoAssert = true; \
        bool eval = !(expr); \
		if ( s_DoAssert ) \
		{ \
            if( eval && assert_failed( &s_DoAssert, __FILE__, __FUNCTION__, __LINE__, #expr" : "msg, ##__VA_ARGS__ ) ) \
			{ \
				EX_HW_BREAK(); \
			} \
		} \
        if ( eval ) \
            return ret; \
	} while( false )

// __EX_ASSERT_EXEC_BREAK
#define __EX_ASSERT_EXEC_BREAK(expr,func,msg,...) \
	do { \
		static bool s_DoAssert = true; \
        bool eval = !(expr); \
        if ( eval ) \
            func; \
		if ( s_DoAssert ) \
		{ \
            if( eval && assert_failed( &s_DoAssert, __FILE__, __FUNCTION__, __LINE__, #expr" : "msg, ##__VA_ARGS__ ) ) \
			{ \
				EX_HW_BREAK(); \
			} \
		} \
	} while( false )

// ========================================================
// internal assert define without break
// ========================================================

// #define __EX_ASSERT(expr,msg,...)                           { if(!(expr)) { ::ex::AssertFailed( __FILE__, __LINE__, __FUNCTION__, #expr" : "msg, ##__VA_ARGS__ ); } }
// #define __EX_ASSERT_RETURN(expr,ret,msg,...)                { if(!(expr)) { ::ex::AssertFailed( __FILE__, __LINE__, __FUNCTION__, #expr" : "msg, ##__VA_ARGS__ ); return ret; } }
// #define __EX_ASSERT_EXEC(expr,func,msg,...)                 { if(!(expr)) { ::ex::AssertFailed( __FILE__, __LINE__, __FUNCTION__, #expr" : "msg, ##__VA_ARGS__ ); func; } }

// ========================================================
// define assert with debug level
// ========================================================

#if ( EX_DEBUG_LEVEL == SERIOUS )
    #define ex_assert(expr,msg,...)                     __EX_ASSERT_BREAK(expr, msg, ##__VA_ARGS__)             
    #define ex_assert_slow(expr,msg,...)                __EX_ASSERT_BREAK(expr, msg, ##__VA_ARGS__)
    #define ex_assert_return(expr,ret,msg,...)          __EX_ASSERT_RETURN_BREAK(expr, ret, msg, ##__VA_ARGS__)
    #define ex_assert_exec(expr,func,msg,...)           __EX_ASSERT_EXEC_BREAK(expr, func, msg, ##__VA_ARGS__)
#elif ( EX_DEBUG_LEVEL == NORMAL )
    #define ex_assert(expr,msg,...)                     __EX_ASSERT_BREAK(expr, msg, ##__VA_ARGS__)             
    #define ex_assert_slow(expr,msg,...)                EX_NOOP
    #define ex_assert_return(expr,ret,msg,...)          __EX_ASSERT_RETURN_BREAK(expr, ret, msg, ##__VA_ARGS__)
    #define ex_assert_exec(expr,func,msg,...)           __EX_ASSERT_EXEC_BREAK(expr, func, msg, ##__VA_ARGS__)
#elif ( EX_DEBUG_LEVEL == DISABLE )
    #define ex_assert(expr,msg,...)                     EX_NOOP
    #define ex_assert_slow(expr,msg,...)                EX_NOOP
    #define ex_assert_return(expr,ret,msg,...)          { if(!(expr)) { return ret; } }
    #define ex_assert_exec(expr,func,msg,...)           EX_NOOP
#endif

// ------------------------------------------------------------------ 
// Desc: Test
// ------------------------------------------------------------------ 

#define ex_check( expr ) __EX_ASSERT_BREAK(expr,"check failed!")

// #################################################################################
#endif // END DEBUG_H_1277524266
// #################################################################################


