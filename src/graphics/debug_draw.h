// ======================================================================================
// File         : debug_draw.h
// Author       : Wu Jie 
// Last Change  : 12/27/2010 | 00:15:19 AM | Monday,December
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef DEBUG_DRAW_H_1293380124
#define DEBUG_DRAW_H_1293380124
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// declare
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

extern void ex_screen_print ( int _x, int _y, const char *_fmt, ... );
extern void ex_draw_text ( float _x, float _y, float _z, const char *_fmt, ... );
extern void ex_draw_point ( float _x, float _y );
// TODO: extern void ex_draw_points ( float _verts[] );
extern void ex_draw_line ( const ex_vec2f_t *_start, const ex_vec2f_t *_end, const ex_color4f_t *_color );
extern void ex_draw_poly ( const ex_vec2f_t *_verts, int _num, const ex_color4f_t *_color, bool _close );
extern void ex_draw_circle ( const ex_vec2f_t *_center, float _r, float _a, int _segs, const ex_color4f_t *_color, bool _lineToCenter );

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 


// #################################################################################
#endif // END DEBUG_DRAW_H_1293380124
// #################################################################################


