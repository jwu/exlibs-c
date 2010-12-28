// ======================================================================================
// File         : encoding.h
// Author       : Wu Jie 
// Last Change  : 08/08/2010 | 23:00:28 PM | Sunday,August
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef ENCODING_H_1281279629
#define ENCODING_H_1281279629
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern int ex_ucs2_to_utf8 ( const wchar_t* _instr, int _inlen /*including NULL terminate*/, char* _outstr );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 


// #################################################################################
#endif // END ENCODING_H_1281279629
// #################################################################################


