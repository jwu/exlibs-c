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

// ######################### 
#ifdef __cplusplus
extern "C" {
#endif
// ######################### 

/**
  =======================================
  printf doc
  =======================================

  <General>
      %[flags][width][.precision][modifier]type

  <flags>           control convertion (optional)
      --------------------------------------------------------------------------------------------
      flag  | Description
      ------+-------------------------------------------------------------------------------------
       [-]  | value is left-justified (default is right-justified)
      ------+-------------------------------------------------------------------------------------
       [+]  | forces to show sign of value ([+] or [-])
            | default is to show only [-] for negative values, overrules space
      ------+-------------------------------------------------------------------------------------
      space | print " " in front of positive value, print - for negative value
      ------+-------------------------------------------------------------------------------------
       [#]  | convert to alternative form:
            |   [o]                                 increase precision to force the first digit to be a zero
            |
            |   [x]/[X]                             nonzero result will have prefix "0x" or "0X"
            |
            |   [a]/[A], [e]/[E], [f]/[F], [g]/[G]  result will always have decimal point
            |
            |   [g]/[G]                             trailing zeros are not removed
      ------+-------------------------------------------------------------------------------------
       [0]  | for  [d], [i], [o], [u], [x], [X], [a], [A], [e], [E], [f], [F], [g], [G] 
            | conversions leading zeros are used to pad to the field width instead of spaces
      --------------------------------------------------------------------------------------------

  <width>           number of characters to output (optional)
      ----------------------------------------------------------------------------------------------
      width   | Description
      --------+-------------------------------------------------------------------------------------
      general | This is a decimal integer specifying the minimum field width. If the normal  
              | conversion produces fewer characters than this, the field is padded with     
              | spaces to the specified width. If the normal conversion produces more        
              | characters than this, the field is not truncated. Normally, the output is    
              | right-justified within the field.                                            
      --------+-------------------------------------------------------------------------------------
        [*]   | A field width of [*] may be specified. This means that the next argument       
              | in the argument list (before the actual value to be printed) is used as the  
              | field width. The value must be an integer of type [int]. If the value is      
              | negative, this means to set the [-] flag and to use the absolute value as      
              | the field width.                                                             
      --------+-------------------------------------------------------------------------------------
      example | ( "%*d", 8,20 )  ==> "      20"  (8 width of value 20)
      --------------------------------------------------------------------------------------------
  
  <.precision>      precision of number (optional)
      --------------------------------------------------------------------------------------------
      precision | Description
     -----------+---------------------------------------------------------------------------------
       general  | The precision specifies the number of digits to be written for the numeric  
                | conversions. If the precision is specified, it consists of a period ([.])  
                | followed optionally by a decimal integer (which defaults to zero if        
                | omitted).                                                                  
     -----------+---------------------------------------------------------------------------------
         [*]    | A precision of [*] may be specified. This means that the next argument         
                | in the argument list (before the actual value to be printed) is used as the 
                | precision. The value must be an integer of type [int], and is ignored if it  
                | is negative.                                                                
                | If [*] is specified for both the field width and precision, the field width   
                | argument precedes the precision argument.                                   
     -----------+---------------------------------------------------------------------------------
        (none)  | default precision:
                | 1 for [d], [i], [o], [u], [x], [X] types. Precision gives the minimum number
                | of digits to appear.
                | 6 for [f], [F], [e], [E] types. Precision gives the number of digits to
                | appear after decimal point.
                | For [g], [G] types all digits are printed.
                | For [s] type all characters of the string are printed, not
                | including the terminating null character.
                | For [a], [A] and [FLT_RADIX] of 2: precision is sufficient for an
                | exact representation of the value.
                | For [a], [A] and [FLT_RADIX] not equal to 2: precision is sufficient
                | to distinguish values of type [double].
     -----------+---------------------------------------------------------------------------------
      [.]/[.0]  | For [d], [i], [o], [u], [x], [X] types the default precision is used, unless
                | the value is 0, then no characters are printed.
                | For [f], [F], [e], [E] types no decimal-point and no decimal-digits are
                | printed..
                | For [g], [G] types precision is assumed to be 1.
                | For [s] type nothing is printed.
                | For [a], [A] types no decimal-point and no decimal-digits appear.
     -----------+---------------------------------------------------------------------------------
        [.N]    | For [d], [i], [o], [u], [x], [X] types. At least N digits appear, if 
                | necessary output is expanded with leading zeros.
                | For [f], [F], [e], [E] types. N digits appear after decimal point.
                | For [g], [G] types N digits are printed.
                | For [s] type a maximum of N characters of the string are printed,
                | not including the terminating null character.
                | For [a], [A] types N specifies the number of digits after decimal 
                | point.
     ---------------------------------------------------------------------------------------------
  
  <modifier>        overrides size (type) of argument (optional)
     --------------------------------------------------------------------------------------------
      modifier | Description
     ----------+---------------------------------------------------------------------------------
        [hh]   | Specifies that the argument is a [signed char] or [unsigned char], 
               | as appropriate. A [char] argument is converted to an [int] or [unsigned int] 
               | by the default argument promotions anyway, but this modifier says to convert it 
               | back to a [char] again.
     ----------+---------------------------------------------------------------------------------
        [h]    | Specifies that the argument is a [short int] or [unsigned short int]
               | as appropriate. A [short] argument is converted to an [int] or [unsigned int]
               | by the default argument promotions anyway, but this modifier says to convert it
               | back to a [short] again.
     ----------+---------------------------------------------------------------------------------
        [l]    | Specifies that the argument is a [long int] or [unsigned long int],
               | as appropriate. Two l characters is like the L modifier, below
               | If used with [%c] or [%s] the corresponding parameter is considered
               | as a wide character or wide character string respectively.
     ----------+---------------------------------------------------------------------------------
        [ll]   | Specifies that a following [d], [i], [o], [u], [x], [X] applies to a
               | [long long int] or [unsigned long long int] argument; or that a
               | [n] applies to a pointer to [long long int].
     ----------+---------------------------------------------------------------------------------
        [L]    | Specifies that a following [a], [A], [e], [E], [f], [F], [g], [G] conversion
               | specifier applies to a [long double] argument.
     ----------+---------------------------------------------------------------------------------
        [j]    | Specifies that a following [d], [i], [o], [u], [x], [X] applies to [intmax_t]
               | or [uintmax_t], or that a following [n] applies to pointer to [intmax_t].
     ----------+---------------------------------------------------------------------------------
        [t]    | Specifies that a following [d], [i], [o], [u], [x], [X] applies to a [ptrdiff_t] 
               | or the corresponding unsigned integer type argument,
               | or that a [n] applies to a pointer to a [ptrdiff_t] argument.
     ----------+---------------------------------------------------------------------------------
        [z]    | Specifies that the following [d], [i], [o], [u], [x], [X] applies to a [size_t] 
               | or the corresponding singed integer type argument,
               | or that a [n] applies to a pointer to a signed integer type corresponding to 
               | [size_t] argument.
     ---------------------------------------------------------------------------------------------

  <type>            type of conversion (required)
     ---------------------------------------------------------------------------------------------
         type  | Description
     ----------+----------------------------------------------------------------------------------
       [d],[i] | type [signed int],   output decicmal, style [-]dddd
     ----------+----------------------------------------------------------------------------------
         [o]   | type [unsigned int], output octal,    style:  dddd
     ----------+----------------------------------------------------------------------------------
         [u]   | type [unsigned int], output decimal,  style   dddd
     ----------+----------------------------------------------------------------------------------
         [x]   | type [unsigned int], output hexadecimal, style dddd using a...f
     ----------+----------------------------------------------------------------------------------
         [X]   | type [unsigned int], output hexadecimal, style dddd using A...F
     ----------+----------------------------------------------------------------------------------
       [f],[F] | type [double], output decimal, style [-]ddd.ddd
     ----------+----------------------------------------------------------------------------------
         [e]   | type [double], output decimal, style [-]d.ddde+/-dd
     ----------+----------------------------------------------------------------------------------
         [E]   | type [double], output decimal, style [-]d.dddE+/-dd
     ----------+----------------------------------------------------------------------------------
         [g]   | type [double], printed as type [e] if exponent is less than -4
               | or greater than or equal to the precision. Otherwise [f] is used.
     ----------+----------------------------------------------------------------------------------
         [G]   | type [double], printed as type [E] if exponent is less than -4
               | or greater than or equal to the precision. Otherwise [F] is used.
     ----------+----------------------------------------------------------------------------------
         [a]   | type [double], style [-]0xh.hhhhp+/-d
               |                          h: hexadecimal digit 0...9, a...f
               |                          d: decimal digit
     ----------+----------------------------------------------------------------------------------
         [A]   | type [double], style [-]0Xh.hhhhP+/-d
               |                          h: hexadecimal digit 0...9, A...F
               |                          d: decimal digit
     ----------+----------------------------------------------------------------------------------
         [c]   | no [l] modifier:   type [char], single character is printed
               | with [l] modifier: type [wchar_t], single character is printed 
     ----------+----------------------------------------------------------------------------------
         [s]   | no [l] modifier:   pointer to array of [char], string is printed
               | with [l] modifier: pointer to array of [wchar_t], string is printed 
     ----------+----------------------------------------------------------------------------------
         [p]   | type pointer to [void], value of pointer is printed in an
               | implementation-defined way
     ----------+----------------------------------------------------------------------------------
         [n]   | argument is a pointer to [signed int] into which is written the
               | number of characters written to the output stream so far
     ----------+----------------------------------------------------------------------------------
         [%]   | a % is printed
     ---------------------------------------------------------------------------------------------
 */

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
    #if ( (EX_COMPILER==EX_MSVC) && ((_M_IX86) || (_M_X64)) )
        #define EX_HW_BREAK()   {__asm {int 3}}
    #elif ( (EX_COMPILER==EX_GCC) && ((__i386__) || (__x86_64__)) )
        #define EX_HW_BREAK()   {__asm__ __volatile__ ("int3");}
    #elif ( EX_PLATFORM == EX_ANDROID ) || ( EX_PLATFORM == EX_IOS )
        #define EX_HW_BREAK()   raise(SIGTRAP)
    #elif ( EX_PLATFORM == EX_XENON )
        #define EX_HW_BREAK()   {__debugbreak();}
    #elif ( EX_PLATFORM == EX_PS3 )
        #define EX_HW_BREAK()   {__asm__ volatile ( "tw 31,1,1" )} // found in CELL_SDK samples, found "BREAK" tag to get it.
        // #define EX_HW_BREAK()   snPause // need #include <libsn.h> || use __builtin_snpause() #ifdef __SNC__
    #else
        #define EX_HW_BREAK() EX_NOOP
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
		if ( s_AskBreak ) { \
            if( error_msg( &s_AskBreak, __FILE__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__ ) ) \
			{ \
				EX_HW_BREAK(); \
			} \
		} \
        else { \
            /*TODO: log_error( __FILE__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__ );*/ \
        } \
	} while( false )

// __EX_WARNING_BREAK
#define __EX_WARNING_BREAK(msg,...) \
	do { \
		static bool s_AskBreak = true; \
		if ( s_AskBreak ) { \
            if( warning_msg( &s_AskBreak, __FILE__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__ ) ) \
			{ \
				EX_HW_BREAK(); \
			} \
		} \
        else { \
            /*TODO: log_warning( __FILE__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__ );*/ \
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
    #define ex_error(msg,...) log_error( __FILE__, __FUNCTION__, __LINE__, msg"\n", ##__VA_ARGS__ )
    #define ex_warning(msg,...) log_warning( __FILE__, __FUNCTION__, __LINE__, msg"\n", ##__VA_ARGS__ )
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
		if ( s_DoAssert ) { \
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
		if ( s_DoAssert ) { \
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
		if ( s_DoAssert ) { \
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
    #define ex_assert_return(expr,ret,msg,...)          EX_NOOP
    #define ex_assert_exec(expr,func,msg,...)           EX_NOOP
#endif

// ------------------------------------------------------------------ 
// Desc: check
// NOTE: unlike assert, check will run the expr even in release version.
// ------------------------------------------------------------------ 

#if ( EX_DEBUG_LEVEL == SERIOUS )
    #define ex_check(expr,msg,...)              __EX_ASSERT_BREAK(expr, msg, ##__VA_ARGS__)
    #define ex_check_return(expr,ret,msg,...)   __EX_ASSERT_RETURN_BREAK(expr, ret, msg, ##__VA_ARGS__)
    #define ex_check_exec(expr,func,msg,...)    __EX_ASSERT_EXEC_BREAK(expr, func, msg, ##__VA_ARGS__)
#elif ( EX_DEBUG_LEVEL == NORMAL )
    #define ex_check(expr,msg,...)              __EX_ASSERT_BREAK(expr, msg, ##__VA_ARGS__)
    #define ex_check_return(expr,ret,msg,...)   __EX_ASSERT_RETURN_BREAK(expr, ret, msg, ##__VA_ARGS__)
    #define ex_check_exec(expr,func,msg,...)    __EX_ASSERT_EXEC_BREAK(expr, func, msg, ##__VA_ARGS__)
#elif ( EX_DEBUG_LEVEL == DISABLE )
    #define ex_check(expr,msg,...)              (expr)
    #define ex_check_return(expr,ret,msg,...)   { if(!(expr)) { return ret; } }
    #define ex_check_exec(expr,func,msg,...)    { if(!(expr)) { func; } }
#endif

// ------------------------------------------------------------------ 
// Desc: for unit test
// ------------------------------------------------------------------ 

#define EX_TEST( expr ) __EX_ASSERT_BREAK(expr,"check failed!")

// ######################### 
#ifdef __cplusplus
} // end extern "C"
#endif
// ######################### 

// #################################################################################
#endif // END DEBUG_H_1277524266
// #################################################################################


