// ======================================================================================
// File         : eng_time.h
// Author       : Wu Jie 
// Last Change  : 01/06/2011 | 23:39:59 PM | Thursday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef ENG_TIME_H_1294328419
#define ENG_TIME_H_1294328419
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

float ex_time (); // time in seconds since the engine start (affect by time-scale)
float ex_worldtime (); // time in seconds since the world has been loaded
float ex_realtime (); // time in seconds since the engine start (not affect by time-scale)

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

float ex_dt (); // the time in seconds it took to complete the last frame.
float ex_dt_physics ();

// ------------------------------------------------------------------ 
// Desc: 
// NOTE: some game write code like yield waitForSeconds( 10.0f * timeScale )
// ------------------------------------------------------------------ 

float ex_time_scale ();
void ex_set_time_scale ( float _scale );
void ex_pause ();
void ex_resume ();

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

float ex_fps ();

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END ENG_TIME_H_1294328419
// #################################################################################
