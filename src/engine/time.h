// ======================================================================================
// File         : time.h
// Author       : Wu Jie 
// Last Change  : 01/28/2011 | 13:39:14 PM | Friday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef TIME_H_1296193155
#define TIME_H_1296193155
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

extern float ex_time (); // the time this frame has started, time in seconds since the engine started (affect by time-scale)
extern float ex_time_noscale (); // the time this frame has started, time in seconds since the engine started (not affect by time-scale)
extern float ex_worldtime (); // the time this frame has started, time in seconds since the world has been loaded (affect by time-scale)
extern float ex_realtime (); // the realtime in seconds since the engine started.

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern float ex_dt (); // the time in seconds it took to complete the last frame (affect by time-scale).
extern float ex_dt_noscale (); // the time in seconds it took to complete the last frame (not affect by time-scale).
extern float ex_dt_fixed ();

// ------------------------------------------------------------------ 
// Desc: 
// NOTE: some game write code like yield waitForSeconds( 10.0f * timeScale )
// ------------------------------------------------------------------ 

extern float ex_time_scale ();
extern void ex_set_time_scale ( float _scale );
extern void ex_pause ();
extern void ex_resume ();

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern float ex_fps ();

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END TIME_H_1296193155
// #################################################################################
