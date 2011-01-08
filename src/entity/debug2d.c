// ======================================================================================
// File         : debug2d.c
// Author       : Wu Jie 
// Last Change  : 01/08/2011 | 16:44:18 PM | Saturday,January
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "debug2d.h"

///////////////////////////////////////////////////////////////////////////////
// properties
///////////////////////////////////////////////////////////////////////////////

EX_DEF_CLASS_CREATOR(ex_debug2d_t) {
    ex_debug2d_t* obj = alloc_ex_debug2d_t();
    ex_component_t* comp = (ex_component_t*)obj;
    comp->init = ex_debug2d_init;
    comp->deinit = ex_debug2d_deinit;
    return comp;
}

EX_DEF_PROPS_BEGIN(ex_debug2d_t)
    // EX_PROP( ex_trans2d_t, _pos, "position",  EX_PROP_ATTR_NONE, ex_prop_set_raw_vec2f, ex_prop_get_raw_vec2f )
    // EX_PROP( ex_trans2d_t, _ang, "angle",  EX_PROP_ATTR_NONE, ex_prop_set_raw_angf, ex_prop_get_raw_angf )
    // EX_PROP( ex_trans2d_t, _scale, "scale",  EX_PROP_ATTR_NONE, ex_prop_set_raw_vec2f, ex_prop_get_raw_vec2f )
EX_DEF_PROPS_END(ex_debug2d_t)

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_debug2d_init ( void* _self ) {
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void ex_debug2d_deinit ( void* _self ) {
}
