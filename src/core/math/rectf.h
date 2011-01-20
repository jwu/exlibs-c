// ======================================================================================
// File         : rectf.h
// Author       : Wu Jie 
// Last Change  : 01/08/2011 | 22:34:03 PM | Saturday,January
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef RECTF_H_1294497245
#define RECTF_H_1294497245
// #################################################################################

// ######################### 
#ifdef __cplusplus
extern "C" { 
#endif
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// struct
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: ex_rectf_t
// ------------------------------------------------------------------ 

typedef struct ex_rectf_t {
    ex_vec2f_t center;
    float width;
    float height;
} ex_rectf_t;

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static inline void ex_rectf_set ( ex_rectf_t *_r, 
                                  ex_vec2f_t _center, 
                                  float _width, 
                                  float _height ) 
{ 
    _r->center = _center;
    _r->width = _width;
    _r->height = _height;
}

// ######################### 
#ifdef __cplusplus
} // end extern C 
#endif
// ######################### 

// #################################################################################
#endif // END RECTF_H_1294497245
// #################################################################################
