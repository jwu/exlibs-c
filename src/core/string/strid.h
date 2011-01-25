// ======================================================================================
// File         : strid.h
// Author       : Wu Jie 
// Last Change  : 11/26/2010 | 14:20:10 PM | Friday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef STRID_H_1286863015
#define STRID_H_1286863015
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// decls
///////////////////////////////////////////////////////////////////////////////

#define EX_STRID_NULL -1

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

// success: 0
// already inited: 1
// failed: -1
extern int ex_strid_init ( size_t _size );
extern void ex_strid_deinit ();
extern bool ex_strid_initialized ();

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern strid_t ex_strid ( const char *_cstr );
extern strid_t ex_strid_from_wcs ( const wchar_t *_wcs );

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern const char *ex_strid_to_cstr ( strid_t _id );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END STRID_H_1286863015
// #################################################################################
