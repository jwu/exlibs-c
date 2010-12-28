// ======================================================================================
// File         : engine.h
// Author       : Wu Jie 
// Last Change  : 12/27/2010 | 09:53:49 AM | Monday,December
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef ENGINE_H_1293414834
#define ENGINE_H_1293414834
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

typedef struct ex_engine_t {
} ex_engine_t;

///////////////////////////////////////////////////////////////////////////////
// functions
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern bool ex_engine_init ();
extern void ex_engine_deinit ();
extern bool ex_engine_initialized ();

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END ENGINE_H_1293414834
// #################################################################################

